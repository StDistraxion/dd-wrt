/**
* @file IxAtmdDescMgmt.c
*
 * @author Intel Corporation
* @date 17 March 2002
*
* @brief NPE descriptor management
*
* NPE descriptors are shared between XScale and NPE processors. The
* physical memory allocation is system-dependant
*
* Design Notes:
* Descriptors are allocated at initialisation at a memory cache boundary.
*
 * 
 * @par
 * IXP400 SW Release version  2.1
 * 
 * -- Intel Copyright Notice --
 * 
 * @par
 * Copyright (c) 2002-2005 Intel Corporation All Rights Reserved.
 * 
 * @par
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel Corporation
 * or its suppliers or licensors.  Title to the Material remains with
 * Intel Corporation or its suppliers and licensors.
 * 
 * @par
 * The Material is protected by worldwide copyright and trade secret laws
 * and treaty provisions. No part of the Material may be used, copied,
 * reproduced, modified, published, uploaded, posted, transmitted,
 * distributed, or disclosed in any way except in accordance with the
 * applicable license agreement .
 * 
 * @par
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you by
 * disclosure or delivery of the Materials, either expressly, by
 * implication, inducement, estoppel, except in accordance with the
 * applicable license agreement.
 * 
 * @par
 * Unless otherwise agreed by Intel in writing, you may not remove or
 * alter this notice or any other notice embedded in Materials by Intel
 * or Intel's suppliers or licensors in any way.
 * 
 * @par
 * For further details, please see the file README.TXT distributed with
 * this software.
 * 
 * @par
 * -- End Intel Copyright Notice --
*/

/*
* Put the system defined include files required.
*/

/*
* Put the user defined include files required.
*/
#include "IxOsal.h"

#include "IxAtmdDescMgmt_p.h"
#include "IxAtmdAssert_p.h"
#include "IxAtmdDefines_p.h"

/*
* Variable declarations global to this file only.  Externs are followed by
* static variables.
*/

typedef struct
{
    unsigned int allocateCount;  /* count of NPE descriptors allocated */
    unsigned int releaseCount;   /* count of NPE descriptors released */
} IxAtmdAccDescMgmtStats;

static IxAtmdAccDescMgmtStats descMgmtStats;  /* module statistics */
static IxAtmdAccNpeDescriptor* npeDescriptorArray[IX_ATMDACC_MAX_NPE_DESCRIPTORS];  /* array of NPE descriptors pointers */
static unsigned int npeDescCount = 0; /* array of NPE descriptors pointers */
static BOOL initDone = FALSE;         /* flag to prevent multiple initialisations */
static IxOsalMutex descMgmtLock;          /* protect critical sections in this file  */


static UINT8 *IxAtmdDmaDescPointer = NULL; /* Pointer used for deallocating memory by ixAtmdAccDescMgmtUninit() */
static UINT8 *descPointer	=	NULL;


/* ------------------------------------------------------
* lock utilities
*/

#define IX_ATMDACC_DESCMGMT_LOCK() (void)ixOsalMutexLock (&descMgmtLock, IX_OSAL_WAIT_FOREVER)
#define IX_ATMDACC_DESCMGMT_UNLOCK() (void)ixOsalMutexUnlock (&descMgmtLock)

/* ----------------------------------------------------
* Initialisation
*/
IX_STATUS
ixAtmdAccDescMgmtInit (void)
{
    IX_STATUS returnStatus = IX_FAIL;
    IxAtmdAccNpeDescriptor* npeDescriptorPtr = NULL;
    void *physicalAddress;
    unsigned int descSize = 0;
    unsigned char *descPtr = NULL;
    unsigned char *descPtrAlign = NULL;

    if (!initDone)
    {
        returnStatus = ixOsalMutexInit (&descMgmtLock);
        if (returnStatus != IX_SUCCESS)
        {
            returnStatus = IX_FAIL;
        }
        else
        {
            ixAtmdAccDescMgmtStatsReset ();
            
            /* compute the size of each element and ensure descriptors are 64-byte aligned */
            /* NPE_ADDR_ALIGN is multiple of IX_OSAL_CACHE_LINE_SIZE which is 64 */
            descSize = (((sizeof (IxAtmdAccNpeDescriptor)) + (NPE_ADDR_ALIGN - 1))
                / NPE_ADDR_ALIGN)
                * NPE_ADDR_ALIGN;

            /* allocate a big buffer containing all elements */
            descPtr = IX_OSAL_CACHE_DMA_MALLOC((IX_ATMDACC_MAX_NPE_DESCRIPTORS * descSize)
                + (NPE_ADDR_ALIGN - 1));
 
	    descPtr += (NPE_ADDR_ALIGN - 1);  
	
		descPointer	=	descPtr;
        /* mask descriptor pointer with 0xffffffc0 to ensure lower 6 bits are 0*/   
         descPtrAlign = (unsigned char *)(((UINT32)descPtr) & NPE_DESCRIPTOR_MASK);


		IxAtmdDmaDescPointer = descPtrAlign;

  
            if (descPtrAlign == NULL)
            {
                returnStatus = IX_FAIL;
            }
            else
            {
                for (npeDescCount = 0;
                    (npeDescCount < IX_ATMDACC_MAX_NPE_DESCRIPTORS);
                    npeDescCount++)
                {
                    
                    /* allocate a NPE descriptor from the big buffer */
                    npeDescriptorPtr = (IxAtmdAccNpeDescriptor *)descPtrAlign;
                    descPtrAlign += descSize;

                    /* initialise the array of descriptors */
                    npeDescriptorArray[npeDescCount] = npeDescriptorPtr;
                    physicalAddress = npeDescriptorPtr;
                    IX_ATMDACC_CONVERT_TO_PHYSICAL_ADDRESS (physicalAddress);
                    npeDescriptorPtr->atmd.physicalAddress = (unsigned int)physicalAddress;
#ifndef NDEBUG
                    npeDescriptorPtr->atmd.signature = IX_ATMDACC_DESCRIPTOR_SIGNATURE;
#endif
                } /* end of for(npeDescCount) */
                if (returnStatus == IX_SUCCESS)
                {
                    initDone = TRUE;
                }
            }
        } /* end of if-else(returnStatus) */
    } /* end of if(initDone) */

    return returnStatus;
}



/* DescMgmt Uninitialisation */
IX_STATUS
ixAtmdAccDescMgmtUninit (void)
{
    IX_STATUS returnStatus = IX_FAIL;

    if (initDone)
    {

		descPointer -= (NPE_ADDR_ALIGN - 1);
        /* de-allocate the buffer containing all elements */
        IX_OSAL_CACHE_DMA_FREE(descPointer);
        IxAtmdDmaDescPointer = NULL;

        returnStatus = ixOsalMutexDestroy (&descMgmtLock);

        if (IX_SUCCESS == returnStatus)
        {  /*uninitialisation complete */
            initDone = FALSE;
        }
    } /* end of if(initDone) */
    return returnStatus;
}




/* ----------------------------------------------------
* Display current stats
*/
void
ixAtmdAccDescMgmtStatsShow (void)
{
    unsigned int currentDescCount; /* module descriptor allocated snapshot */
    IxAtmdAccDescMgmtStats currentDescMgmtStats;  /* module statistics snapshot */
    unsigned int descSize = ((sizeof (IxAtmdAccNpeDescriptor) + (NPE_ADDR_ALIGN - 1))
        / NPE_ADDR_ALIGN)
        * NPE_ADDR_ALIGN;


    /* get a snapshot */
    IX_ATMDACC_DESCMGMT_LOCK();
    currentDescMgmtStats = descMgmtStats;
    currentDescCount = npeDescCount;

    /* sanity check */
    IX_ATMDACC_ENSURE((descMgmtStats.allocateCount -
        descMgmtStats.releaseCount) ==
        (IX_ATMDACC_MAX_NPE_DESCRIPTORS - npeDescCount) ,
        "descriptor pool index corrupted");

    IX_ATMDACC_DESCMGMT_UNLOCK();

    /* display the snapshot */
    printf ("Npe Descriptors memory allocation\n");
    printf ("Pool size in bytes ......... : %10u bytes\n",
        descSize * IX_ATMDACC_MAX_NPE_DESCRIPTORS);
    printf ("Descriptor size ............ : %10u bytes\n",
        (unsigned int)sizeof (IxAtmdAccNpeDescriptor));
    printf ("Pool depth ................. : %10u descriptors\n",
        IX_ATMDACC_MAX_NPE_DESCRIPTORS);
    printf ("Alloc'd .................... : %10u\n",
        currentDescMgmtStats.allocateCount);
    printf ("Free'd ..................... : %10u\n",
        currentDescMgmtStats.releaseCount);
    printf ("Pool usage ................. : %10u descriptors\n",
        descMgmtStats.allocateCount - descMgmtStats.releaseCount);
}

/* ----------------------------------------------------
reset counters
*/
void
ixAtmdAccDescMgmtStatsReset (void)
{
    descMgmtStats.allocateCount = 0;
    descMgmtStats.releaseCount = 0;
}

/* ----------------------------------------------------
get a NPE descriptor
*/
IX_STATUS
ixAtmdAccDescNpeDescriptorGet (IxAtmdAccNpeDescriptor** npeDescriptorPtr)
{
    IX_STATUS returnStatus = IX_SUCCESS;

    IX_ATMDACC_ENSURE (initDone, "Initialisation Error");

    IX_ATMDACC_DESCMGMT_LOCK();

    /* check parameter */
    IX_ATMDACC_ENSURE (npeDescriptorPtr != NULL, "null pointer parameter");

    /* sanity check */
    IX_ATMDACC_ENSURE(npeDescCount <= IX_ATMDACC_MAX_NPE_DESCRIPTORS,
        "descriptor pool index corrupted");

    /* check there is an entry in the pool */
    IX_ATMDACC_ENSURE (npeDescCount > 0, "Pool size mismatch");

    if (npeDescCount > 0)
    {

        /* get the descriptor from the pool */
        *npeDescriptorPtr = npeDescriptorArray[--npeDescCount];

        ixOsalMemSet(&(*npeDescriptorPtr)->npe, 0, sizeof((*npeDescriptorPtr)->npe));

        /* update stats */
        descMgmtStats.allocateCount++;
    }
    else
    {
        returnStatus = IX_FAIL;
    } /* end of if-else(npeDescCount) */

    IX_ATMDACC_DESCMGMT_UNLOCK();

    return returnStatus;
}

/* ----------------------------------------------------
release a NPE descriptor
*/
IX_STATUS
ixAtmdAccDescNpeDescriptorRelease (IxAtmdAccNpeDescriptor* npeDescriptorPtr)
{
    IX_STATUS returnStatus = IX_SUCCESS;

    IX_ATMDACC_ENSURE (initDone, "Initialisation Error");

    IX_ATMDACC_ENSURE (npeDescriptorPtr != NULL, "Release null descriptor");

    IX_ATMDACC_DESCMGMT_LOCK();

    /* ensure pool availabilility */
    IX_ATMDACC_ENSURE(npeDescCount < IX_ATMDACC_MAX_NPE_DESCRIPTORS,
        "running out of space in pool descriptor");

    /* check pool availabilility */
    if (npeDescCount < IX_ATMDACC_MAX_NPE_DESCRIPTORS)
    {
        /* put element to the pool */
        npeDescriptorArray[npeDescCount++] = npeDescriptorPtr;

        /* update stats */
        descMgmtStats.releaseCount++;
    }
    else
    {
        returnStatus = IX_FAIL;
    } /* end of if-else(npeDescCount) */

    IX_ATMDACC_DESCMGMT_UNLOCK();

    return returnStatus;
}

#ifndef NDEBUG
/* ----------------------------------------------------
get the descriptor memory usage in bytes
*/
IX_STATUS
ixAtmdAccDescMgmtMemoryUsageGet (unsigned int *descriptorMemoryUsagePtr)
{
    IX_ATMDACC_ENSURE (initDone, "Initailisation Error");

    IX_ATMDACC_DESCMGMT_LOCK();

    /* sanity check */
    IX_ATMDACC_ENSURE(npeDescCount <= IX_ATMDACC_MAX_NPE_DESCRIPTORS,
        "descriptor pool index corrupted");

    /* sanity check */
    IX_ATMDACC_ENSURE((descMgmtStats.allocateCount -
        descMgmtStats.releaseCount) ==
        (IX_ATMDACC_MAX_NPE_DESCRIPTORS - npeDescCount) ,
        "descriptor pool index corrupted");

    /* get the memory usage */
    *descriptorMemoryUsagePtr = (descMgmtStats.allocateCount -
        descMgmtStats.releaseCount) * sizeof (IxAtmdAccNpeDescriptor);

    IX_ATMDACC_DESCMGMT_UNLOCK();

    return IX_SUCCESS;
}
#endif


