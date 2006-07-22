#ifndef __doxygen_HIDE  /* This file is not part of the API */
#ifndef IxEthNpe_H
#define IxEthNpe_H

/**
 * @file IxEthNpe.h
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

/**
 * @defgroup IxEthNpe Intel (R) IXP400 Software Ethernet NPE (IxEthNpe) API
 *
 * @brief Contains the API for Ethernet NPE.
 * 
 * All messages given to NPE, get back an acknowledgment. The acknowledgment 
 * is identical to the message sent to the NPE (except for NPE_GETSTATUS message).
 *
 * @{
 */

/*--------------------------------------------------------------------------
 * Total number of ethernet NPEs supported
 *------------------------------------------------------------------------*/
/**
 * @def IX_ETHNPE_NUM_NPES
 */
#define IX_ETHNPE_NUM_NPES	 	(3)

/*--------------------------------------------------------------------------
 * Maximum number of ports per NPE supported
 *------------------------------------------------------------------------*/
/**
 * @def IX_ETHNPE_NUM_PORTS_PER_NPE
 */
#define IX_ETHNPE_NUM_PORTS_PER_NPE     (1)

/*--------------------------------------------------------------------------
 * Total number of ethernet ports on all NPEs supported
 *------------------------------------------------------------------------*/
/**
 * @def IX_ETHNPE_NUM_PHYSICAL_PORTS
 */
#ifdef __ixp46X
#define IX_ETHNPE_NUM_PHYSICAL_PORTS 	(3)
#else
#define IX_ETHNPE_NUM_PHYSICAL_PORTS 	(2)
#endif

/*--------------------------------------------------------------------------
 * NPE Logical PortID field conversions and data types
 *------------------------------------------------------------------------*/
/**
 * @brief The following section deals with the NPE Logical ID used in
 *        queue entries.
 *  Logical ID field definition (8bits):
 *        Bits:   | 7   6 | 5   4 | 3   2   1   0 | 
 *        Field:  | RSVD  | NPEID |    PORT ID    |
 */

/**
 * @brief NPE Node ID (ID of the NPE)
 */
typedef UINT8 IxEthNpeNodeId;

/**
 * @brief NPE Port ID (port number on an NPE)
 */
typedef UINT8 IxEthNpePortId;

/**
 * @brief Logical Port ID including both NPE and port numbers
 */
typedef UINT8 IxEthNpeLogicalId;

/**
 * @brief Physical Port ID (index into array of all physical ports)
 */
typedef UINT32 IxEthNpePhysicalId;

/**
 * @def IX_ETHNPE_NODE_AND_PORT_TO_LOGICAL_ID
 */
#define IX_ETHNPE_NODE_AND_PORT_TO_LOGICAL_ID(npe,port)	((npe << 4) | (port & 0xF))

/**
 * @def IX_ETHNPE_LOGICAL_ID_TO_NODE
 */
#define IX_ETHNPE_LOGICAL_ID_TO_NODE(id)			(id >> 4)

/**
 * @def IX_ETHNPE_LOGICAL_ID_TO_PORT
 */
#define IX_ETHNPE_LOGICAL_ID_TO_PORT(id)		(id & 0xF)

/**
 * @def IxEthNpePorts
 * @brief Port logical ID array
 */
static const
IxEthNpeLogicalId IxEthNpePorts[IX_ETHNPE_NUM_PHYSICAL_PORTS] =
{
    IX_ETHNPE_NODE_AND_PORT_TO_LOGICAL_ID(1,0), /* physical port 0 */
    IX_ETHNPE_NODE_AND_PORT_TO_LOGICAL_ID(2,0)  /* physical port 1 */
#ifdef __ixp46X
    ,
    IX_ETHNPE_NODE_AND_PORT_TO_LOGICAL_ID(0,0) /* physical port 2 */
#endif
};

/**
 * @def IX_ETHNPE_LOGICAL_ID_TO_PHYSICAL_ID
 * @brief This hard-coded conversion depends on how IxEthNpePorts
 *        is defined and must be updated accordingly
 */
#define IX_ETHNPE_LOGICAL_ID_TO_PHYSICAL_ID(id) \
            IX_ETHNPE_NODE_AND_PORT_TO_PHYSICAL_ID( \
                IX_ETHNPE_LOGICAL_ID_TO_NODE(id), \
                IX_ETHNPE_LOGICAL_ID_TO_PORT(id) )

/**
 * @def IX_ETHNPE_NODE_AND_PORT_TO_PHYSICAL_ID
 * @brief This hard-coded conversion depends on how IxEthNpePorts
 *        is defined and must be updated accordingly
 */
#define IX_ETHNPE_NODE_AND_PORT_TO_PHYSICAL_ID(npe,port) \
            npe ? npe - 1 : 2

/**
 * @def IX_ETHNPE_PHYSICAL_ID_TO_LOGICAL_ID
 */
#define IX_ETHNPE_PHYSICAL_ID_TO_LOGICAL_ID(index)	(IxEthNpePorts[index])

/**
 * @def IX_ETHNPE_PHYSICAL_ID_TO_NODE
 */
#define IX_ETHNPE_PHYSICAL_ID_TO_NODE(index) \
            (IX_ETHNPE_LOGICAL_ID_TO_NODE(IxEthNpePorts[index]))

/**
 * @def IX_ETHNPE_PHYSICAL_ID_TO_PORT
 */
#define IX_ETHNPE_PHYSICAL_ID_TO_PORT(index) \
            (IX_ETHNPE_LOGICAL_ID_TO_PORT(IxEthNpePorts[index]))


/*--------------------------------------------------------------------------
 * APB Message IDs - Intel XScale(R) Core ->NPE
 *------------------------------------------------------------------------*/

/**
 * @def IX_ETHNPE_NPE_GETSTATUS
 *
 * @brief Request from the Intel XScale(R) Core  client for the NPE to return the firmware
 * version of the currently executing image.
 *
 * Acknowledgment message id is same as the request message id. 
 * NPE returns the firmware version ID to Intel XScale(R) Core .
 */
#define IX_ETHNPE_NPE_GETSTATUS                 0x00

/**
 * @def IX_ETHNPE_EDB_SETPORTADDRESS
 *
 * @brief Request from the Intel XScale(R) Core  client for the NPE to set the Ethernet 
 * port's port ID and MAC address. 
 */
#define IX_ETHNPE_EDB_SETPORTADDRESS            0x01

/**
 * @def IX_ETHNPE_EDB_GETMACADDRESSDATABASE
 *
 * @brief Request from Intel XScale(R) Core  client to the NPE requesting upload of 
 * Ethernet Filtering Database or Header Conversion Database from NPE's 
 * data memory to Intel XScale(R) Core  accessible SDRAM.
 */
#define IX_ETHNPE_EDB_GETMACADDRESSDATABASE     0x02

/**
 * @def IX_ETHNPE_EDB_SETMACADDRESSSDATABASE
 *
 * @brief Request from Intel XScale(R) Core  client to the NPE requesting download of 
 * Ethernet Filtering Database or Header Conversion Database from SDRAM
 * to the NPE's datamemory.
 */
#define IX_ETHNPE_EDB_SETMACADDRESSSDATABASE    0x03

/**
 * @def IX_ETHNPE_GETSTATS
 *
 * @brief Request from the Intel XScale(R) Core  client for the current MAC port statistics 
 * data to be written to the (empty) statistics structure and the specified
 * location in externa memory.
 */
#define IX_ETHNPE_GETSTATS                      0x04

/**
 * @def IX_ETHNPE_RESETSTATS
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to reset all of its internal 
 * MAC port statistics state variables. 
 *
 * As a side effect, this message entails an implicit request that the NPE
 *  write the current MAC port statistics into the MAC statistics structure 
 * at the specified location in external memory.
 */
#define IX_ETHNPE_RESETSTATS                    0x05

/**
 * @def IX_ETHNPE_SETMAXFRAMELENGTHS
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to configure maximum framelengths
 * and block sizes in receive and transmit direction.
 */
#define IX_ETHNPE_SETMAXFRAMELENGTHS            0x06

/**
 * @def IX_ETHNPE_VLAN_SETRXTAGMODE
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to configure VLAN frame type
 * filtering and VLAN the tagging mode for the receiver.
 */
#define IX_ETHNPE_VLAN_SETRXTAGMODE             0x07

/**
 * @def IX_ETHNPE_VLAN_SETDEFAULTRXVID
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to set receiver's default 
 * VLAN tag (PVID)and internal traffic class.
 */
#define IX_ETHNPE_VLAN_SETDEFAULTRXVID          0x08

/**
 * @def IX_ETHNPE_VLAN_SETPORTVLANTABLEENTRY
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to configure VLAN Port 
 * membership and Tx tagging for 8 consecutive VLANID's.
 */
#define IX_ETHNPE_VLAN_SETPORTVLANTABLEENTRY    0x09

/**
 * @def IX_ETHNPE_VLAN_SETPORTVLANTABLERANGE
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to configure VLAN Port
 * membership and Tx tagging for a range of VLANID's.
 */
#define IX_ETHNPE_VLAN_SETPORTVLANTABLERANGE    0x0A

/**
 * @def IX_ETHNPE_VLAN_SETRXQOSENTRY
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to map a user priority
 * to QoS class and an AQM queue number.
 */
#define IX_ETHNPE_VLAN_SETRXQOSENTRY            0x0B

/**
 * @def IX_ETHNPE_VLAN_SETPORTIDEXTRACTIONMODE
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to enable or disable
 * portID extraction from VLAN-tagged frames for the specified port.
 */
#define IX_ETHNPE_VLAN_SETPORTIDEXTRACTIONMODE  0x0C

/**
 * @def IX_ETHNPE_STP_SETBLOCKINGSTATE
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to block or unblock
 * forwarding for spanning tree BPDUs.
 */
#define IX_ETHNPE_STP_SETBLOCKINGSTATE          0x0D

/**
 * @def IX_ETHNPE_FW_SETFIREWALLMODE
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to configure firewall
 * services modes of operation and/or download Ethernet Firewall Database from
 * SDRAM to NPE.
 */
#define IX_ETHNPE_FW_SETFIREWALLMODE            0x0E

/**
 * @def IX_ETHNPE_PC_SETFRAMECONTROLDURATIONID 
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to set global frame control
 * and duration/ID field for the 802.3 to 802.11 protocol header conversion
 * service.
 */
#define IX_ETHNPE_PC_SETFRAMECONTROLDURATIONID  0x0F

/**
 * @def IX_ETHNPE_PC_SETAPMACTABLE
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to update a block/section/
 * range of the AP MAC Address Table.
 */
#define IX_ETHNPE_PC_SETAPMACTABLE              0x11

/**
 * @def IX_ETHNPE_SETLOOPBACK_MODE
 *
 * @brief Turn on or off the NPE frame loopback.
 */
#define IX_ETHNPE_SETLOOPBACK_MODE              0x12

/**
 * @def IX_ETHNPE_PC_SETBSSIDTABLE
 *
 * @brief Request from the Intel XScale(R) Core  client to the NPE to update a block/section/
 * range of the BSSID MAC Address Table.
 */
#define IX_ETHNPE_PC_SETBSSIDTABLE              0x13

/**
 * @def IX_ETHNPE_APPENDFCSCONFIG
 *
 * @brief Notification from the Intel XScale(R) Core  client to the NPE telling the NPE the 
 * MAC FCS append configuration
 */
#define IX_ETHNPE_APPENDFCSCONFIG              0x15


/*--------------------------------------------------------------------------
 * APB Message IDs - NPE->Intel XScale(R) Core 
 *------------------------------------------------------------------------*/

/**
 * @def IX_ETHNPE_NPE_GETSTATUS_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_NPE_GETSTATUS message. NPE firmware version 
 * id is returned in the message.
 */
#define IX_ETHNPE_NPE_GETSTATUS_ACK                 0x00

/**
 * @def IX_ETHNPE_EDB_SETPORTADDRESS_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_EDB_SETPORTADDRESS message.
 */
#define IX_ETHNPE_EDB_SETPORTADDRESS_ACK            0x01

/**
 * @def IX_ETHNPE_EDB_GETMACADDRESSDATABASE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_EDB_GETMACADDRESSDATABASE message
 */
#define IX_ETHNPE_EDB_GETMACADDRESSDATABASE_ACK     0x02

/**
 * @def IX_ETHNPE_EDB_SETMACADDRESSSDATABASE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_EDB_SETMACADDRESSSDATABASE message.
 */
#define IX_ETHNPE_EDB_SETMACADDRESSSDATABASE_ACK    0x03

/**
 * @def IX_ETHNPE_GETSTATS_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_GETSTATS message.
 */
#define IX_ETHNPE_GETSTATS_ACK                      0x04

/**
 * @def IX_ETHNPE_RESETSTATS_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_RESETSTATS message.
 */
#define IX_ETHNPE_RESETSTATS_ACK                    0x05

/**
 * @def IX_ETHNPE_SETMAXFRAMELENGTHS_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_SETMAXFRAMELENGTHS message.
 */
#define IX_ETHNPE_SETMAXFRAMELENGTHS_ACK            0x06

/**
 * @def IX_ETHNPE_VLAN_SETRXTAGMODE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_VLAN_SETRXTAGMODE message.
 */
#define IX_ETHNPE_VLAN_SETRXTAGMODE_ACK             0x07

/**
 * @def IX_ETHNPE_VLAN_SETDEFAULTRXVID_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_VLAN_SETDEFAULTRXVID  message.
 */
#define IX_ETHNPE_VLAN_SETDEFAULTRXVID_ACK          0x08

/**
 * @def IX_ETHNPE_VLAN_SETPORTVLANTABLEENTRY_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_VLAN_SETPORTVLANTABLEENTRY message.
 */
#define IX_ETHNPE_VLAN_SETPORTVLANTABLEENTRY_ACK    0x09

/**
 * @def IX_ETHNPE_VLAN_SETPORTVLANTABLERANGE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_VLAN_SETPORTVLANTABLERANGE message.
 */
#define IX_ETHNPE_VLAN_SETPORTVLANTABLERANGE_ACK    0x0A

/**
 * @def IX_ETHNPE_VLAN_SETRXQOSENTRY_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_VLAN_SETRXQOSENTRY message.
 */
#define IX_ETHNPE_VLAN_SETRXQOSENTRY_ACK            0x0B

/**
 * @def IX_ETHNPE_VLAN_SETPORTIDEXTRACTIONMODE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_VLAN_SETPORTIDEXTRACTIONMODE message.
 */
#define IX_ETHNPE_VLAN_SETPORTIDEXTRACTIONMODE_ACK  0x0C

/**
 * @def IX_ETHNPE_STP_SETBLOCKINGSTATE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_STP_SETBLOCKINGSTATE message.
 */
#define IX_ETHNPE_STP_SETBLOCKINGSTATE_ACK          0x0D

/**
 * @def IX_ETHNPE_FW_SETFIREWALLMODE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_FW_SETFIREWALLMODE message. 
 */
#define IX_ETHNPE_FW_SETFIREWALLMODE_ACK            0x0E

/**
 * @def IX_ETHNPE_PC_SETFRAMECONTROLDURATIONID_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_PC_SETFRAMECONTROLDURATIONID message.
 */
#define IX_ETHNPE_PC_SETFRAMECONTROLDURATIONID_ACK  0x0F

/**
 * @def IX_ETHNPE_PC_SETAPMACTABLE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_PC_SETAPMACTABLE message.
 */
#define IX_ETHNPE_PC_SETAPMACTABLE_ACK              0x11

/**
 * @def IX_ETHNPE_SETLOOPBACK_MODE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_SETLOOPBACK_MODE message.
 */
#define IX_ETHNPE_SETLOOPBACK_MODE_ACK              0x12

/**
 * @def IX_ETHNPE_PC_SETBSSIDTABLE_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_PC_SETBSSIDTABLE message.
 */
#define IX_ETHNPE_PC_SETBSSIDTABLE_ACK              0x13

/**
 * @def IX_ETHNPE_APPENDFCSCONFIG_ACK
 *
 * @brief Acknowledgment to IX_ETHNPE_APPENDFCSCONFIG message.
 */
#define IX_ETHNPE_APPENDFCSCONFIG_ACK              0x15


/*--------------------------------------------------------------------------
 * Queue Manager Queue entry bit field boundary definitions
 *------------------------------------------------------------------------*/

/**
 * @def MASK(hi,lo)
 *
 * @brief Macro for mask
 */
#define MASK(hi,lo)                    (((1 << (1 + ((hi) - (lo)))) - 1) << (lo))

/**
 * @def BITS(x,hi,lo)
 *
 * @brief Macro for bits
 */
#define BITS(x,hi,lo)                  (((x) & MASK(hi,lo)) >> (lo))

/**
 * @def IX_ETHNPE_QM_Q_RXENET_LENGTH_MASK
 *
 * @brief QMgr Queue LENGTH field mask
 */
#define IX_ETHNPE_QM_Q_RXENET_LENGTH_MASK   0x3fff

/**
 * @def IX_ETHNPE_QM_Q_FIELD_FLAG_R
 *
 * @brief QMgr Queue FLAG field right boundary
 */
#define IX_ETHNPE_QM_Q_FIELD_FLAG_R             20

/**
 * @def IX_ETHNPE_QM_Q_FIELD_FLAG_MASK
 *
 * @brief QMgr Queue FLAG field mask
 *
 * Multicast bit : BIT(4)
 * Broadcast bit : BIT(5)
 * IP bit        : BIT(6) (linux only)
 *
 */
#ifdef __vxworks
#define IX_ETHNPE_QM_Q_FIELD_FLAG_MASK           0x30
#else
#define IX_ETHNPE_QM_Q_FIELD_FLAG_MASK           0x70
#endif


/**
 * @def IX_ETHNPE_QM_Q_FIELD_NPEID_L
 *
 * @brief QMgr Queue NPE ID field left boundary
 */
#define IX_ETHNPE_QM_Q_FIELD_NPEID_L            1

/**
 * @def IX_ETHNPE_QM_Q_FIELD_NPEID_R
 *
 * @brief QMgr Queue NPE ID field right boundary
 */
#define IX_ETHNPE_QM_Q_FIELD_NPEID_R            0

/**
 * @def IX_ETHNPE_QM_Q_FIELD_PID_L
 *
 * @brief QMgr Queue Port ID field left boundary
 */
#define IX_ETHNPE_QM_Q_FIELD_PID_L            4

/**
 * @def IX_ETHNPE_QM_Q_FIELD_PID_R
 *
 * @brief QMgr Queue Port ID field right boundary
 */
#define IX_ETHNPE_QM_Q_FIELD_PID_R            3

/**
 * @def IX_ETHNPE_QM_Q_FIELD_PRIOR_L
 *
 * @brief QMgr Queue Priority field left boundary
 */
#define IX_ETHNPE_QM_Q_FIELD_PRIOR_L            2

/**
 * @def IX_ETHNPE_QM_Q_FIELD_PRIOR_R
 *
 * @brief QMgr Queue Priority field right boundary
 */
#define IX_ETHNPE_QM_Q_FIELD_PRIOR_R            0

/**
 * @def IX_ETHNPE_QM_Q_FIELD_ADDR_L 
 *
 * @brief QMgr Queue Address field left boundary
 */
#define IX_ETHNPE_QM_Q_FIELD_ADDR_L             31

/**
 * @def IX_ETHNPE_QM_Q_FIELD_ADDR_R
 *
 * @brief QMgr Queue Address field right boundary
 */
#define IX_ETHNPE_QM_Q_FIELD_ADDR_R              5

/*--------------------------------------------------------------------------
 * Queue Manager Queue entry bit field masks
 *------------------------------------------------------------------------*/

/**
 * @def IX_ETHNPE_QM_Q_PID_MASK
 *
 * @brief Macro to mask the Port ID field of the Queue Manager Entry
 *        Applies to any queue entry
 */
#define IX_ETHNPE_QM_Q_PID_MASK \
            MASK (IX_ETHNPE_QM_Q_FIELD_PID_L, \
                  IX_ETHNPE_QM_Q_FIELD_PID_R)

/**
 * @def IX_ETHNPE_QM_Q_FREEENET_ADDR_MASK 
 *
 * @brief Macro to mask the Address field of the FreeEnet Queue Manager Entry
 */
#define IX_ETHNPE_QM_Q_FREEENET_ADDR_MASK \
            MASK (IX_ETHNPE_QM_Q_FIELD_ADDR_L, \
                  IX_ETHNPE_QM_Q_FIELD_ADDR_R)

/**
 * @def IX_ETHNPE_QM_Q_RXENET_ADDR_MASK 
 *
 * @brief Macro to mask the Mbuf Address field of the RxEnet Queue Manager Entry
 */
#define IX_ETHNPE_QM_Q_RXENET_ADDR_MASK \
            MASK (IX_ETHNPE_QM_Q_FIELD_ADDR_L, \
                  IX_ETHNPE_QM_Q_FIELD_ADDR_R)

/**
 * @def IX_ETHNPE_QM_Q_RXENET_NPEID_MASK  
 *
 * @brief Macro to mask the NPE ID field of the RxEnet Queue Manager Entry
 */
#define IX_ETHNPE_QM_Q_RXENET_NPEID_MASK \
            MASK (IX_ETHNPE_QM_Q_FIELD_NPEID_L, \
                  IX_ETHNPE_QM_Q_FIELD_NPEID_R)

/**
 * @def IX_ETHNPE_QM_Q_TXENET_PRIOR_MASK
 *
 * @brief Macro to mask the Priority field of the TxEnet Queue Manager Entry
 */
#define IX_ETHNPE_QM_Q_TXENET_PRIOR_MASK \
            MASK (IX_ETHNPE_QM_Q_FIELD_PRIOR_L, \
                  IX_ETHNPE_QM_Q_FIELD_PRIOR_R)

/**
 * @def IX_ETHNPE_QM_Q_TXENET_ADDR_MASK 
 *
 * @brief Macro to mask the Mbuf Address field of the TxEnet Queue Manager Entry
 */
#define IX_ETHNPE_QM_Q_TXENET_ADDR_MASK \
            MASK (IX_ETHNPE_QM_Q_FIELD_ADDR_L, \
                  IX_ETHNPE_QM_Q_FIELD_ADDR_R)

/**
 * @def IX_ETHNPE_QM_Q_TXENETDONE_NPEID_MASK 
 *
 * @brief Macro to mask the NPE ID field of the TxEnetDone Queue Manager Entry
 */
#define IX_ETHNPE_QM_Q_TXENETDONE_NPEID_MASK \
            MASK (IX_ETHNPE_QM_Q_FIELD_NPEID_L, \
                  IX_ETHNPE_QM_Q_FIELD_NPEID_R)

/**
 * @def IX_ETHNPE_QM_Q_TXENETDONE_ADDR_MASK 
 * 
 * @brief Macro to mask the Mbuf Address field of the TxEnetDone Queue Manager 
 * Entry
 */
#define IX_ETHNPE_QM_Q_TXENETDONE_ADDR_MASK \
            MASK (IX_ETHNPE_QM_Q_FIELD_ADDR_L, \
                  IX_ETHNPE_QM_Q_FIELD_ADDR_R)

/*--------------------------------------------------------------------------
 * Queue Manager Queue entry bit field value extraction macros
 *------------------------------------------------------------------------*/

/**
 * @def IX_ETHNPE_QM_Q_FREEENET_ADDR_VAL(x)
 *
 * @brief Extraction macro for Address field of FreeNet Queue Manager Entry 
 *
 * Pointer to an mbuf buffer descriptor
 */
#define IX_ETHNPE_QM_Q_FREEENET_ADDR_VAL(x) \
            ((x) & IX_ETHNPE_QM_Q_FREEENET_ADDR_MASK)

/**
 * @def IX_ETHNPE_QM_Q_RXENET_NPEID_VAL(x)
 *
 * @brief Extraction macro for NPE ID field of RxEnet Queue Manager Entry 
 *
 * Set to 0 for entries originating from the Eth0 NPE; 
 * Set to 1 for entries originating from the Eth1 NPE.
 */
#define IX_ETHNPE_QM_Q_RXENET_NPEID_VAL(x) \
            BITS (x, IX_ETHNPE_QM_Q_FIELD_NPEID_L, \
                     IX_ETHNPE_QM_Q_FIELD_NPEID_R)

/**
 * @def IX_ETHNPE_QM_Q_RXENET_ADDR_VAL(x)
 *
 * @brief Extraction macro for Address field of RxEnet Queue Manager Entry
 *
 * Pointer to an mbuf buffer descriptor
 */
#define IX_ETHNPE_QM_Q_RXENET_ADDR_VAL(x) \
            ((x) & IX_ETHNPE_QM_Q_RXENET_ADDR_MASK)

/**
 * @def IX_ETHNPE_QM_Q_TXENET_PRIOR_VAL(x)
 *
 * @brief Extraction macro for Priority field of TxEnet Queue Manager Entry
 *
 * Priority of the packet (as described in IEEE 802.1D). This field is
 * cleared upon return from the Ethernet NPE to the TxEnetDone queue.
 */
#define IX_ETHNPE_QM_Q_TXENET_PRIOR_VAL(x) \
            BITS (x, IX_ETHNPE_QM_Q_FIELD_PRIOR_L, \
                     IX_ETHNPE_QM_Q_FIELD_PRIOR_R)

/**
 * @def IX_ETHNPE_QM_Q_TXENET_ADDR_VAL(x)
 *
 * @brief Extraction macro for Address field of Queue Manager TxEnet Queue 
 * Manager Entry
 *
 * Pointer to an mbuf buffer descriptor
 */
#define IX_ETHNPE_QM_Q_TXENET_ADDR_VAL(x) \
            ((x) & IX_ETHNPE_QM_Q_TXENET_ADDR_MASK)

/**
 * @def IX_ETHNPE_QM_Q_TXENETDONE_NPEID_VAL(x)
 *
 * @brief Extraction macro for NPE ID field of TxEnetDone Queue Manager Entry
 *
 * Set to 0 for entries originating from the Eth0 NPE; set to 1 for en-tries 
 * originating from the Eth1 NPE.
 */
#define IX_ETHNPE_QM_Q_TXENETDONE_NPEID_VAL(x) \
            BITS (x, IX_ETHNPE_QM_Q_FIELD_NPEID_L, \
                     IX_ETHNPE_QM_Q_FIELD_NPEID_R)

/**
 * @def IX_ETHNPE_QM_Q_TXENETDONE_ADDR_VAL(x)
 *
 * @brief Extraction macro for Address field of TxEnetDone Queue Manager Entry
 *
 * Pointer to an mbuf buffer descriptor
 */
#define IX_ETHNPE_QM_Q_TXENETDONE_ADDR_VAL(x) \
            ((x) & IX_ETHNPE_QM_Q_TXENETDONE_ADDR_MASK)

/**
 * @def IX_ETHNPE_QM_Q_PID_VAL(x)
 * @brief Extraction macro for Port ID field of Queue Manager Entry 
 */
#define IX_ETHNPE_QM_Q_PID_VAL(x) \
            BITS (x, IX_ETHNPE_QM_Q_FIELD_PID_L, \
                     IX_ETHNPE_QM_Q_FIELD_PID_R)


/*--------------------------------------------------------------------------
 * NPE limits
 *------------------------------------------------------------------------*/

/**
 * @def IX_ETHNPE_ACC_RXFREE_BUFFER_LENGTH_MIN
 * 
 * @brief Macro to check the minimum length of a rx free buffer
 */
#define IX_ETHNPE_ACC_RXFREE_BUFFER_LENGTH_MIN (64)

/**
 * @def IX_ETHNPE_ACC_RXFREE_BUFFER_LENGTH_MASK
 * 
 * @brief Mask to apply to the mbuf length before submitting it to the NPE
 * (the NPE handles only rx free mbufs which are multiple of 64)
 * 
 * @sa IX_ETHNPE_ACC_RXFREE_BUFFER_LENGTH_MASK
 */
#define IX_ETHNPE_ACC_RXFREE_BUFFER_LENGTH_MASK (~63)

/**
 * @def IX_ETHNPE_ACC_RXFREE_BUFFER_ROUND_UP(size)
 * 
 * @brief  Round up to get the size necessary to receive without chaining
 * the frames which are (size) bytes (the NPE operates by multiple of 64)
 * e.g. To receive 1514 bytes frames, the size of the buffers in replenish
 * has to be at least (1514+63)&(~63) = 1536 bytes.
 *
 */
#define IX_ETHNPE_ACC_RXFREE_BUFFER_ROUND_UP(size) (((size) + 63) & ~63)

/**
 * @def IX_ETHNPE_ACC_RXFREE_BUFFER_ROUND_DOWN(size)
 *
 * @brief Round down to apply to the mbuf length before submitting
 * it to the NPE. (the NPE operates by multiple of 64)
 *
 */
#define IX_ETHNPE_ACC_RXFREE_BUFFER_ROUND_DOWN(size) ((size) & ~63)

/**
 * @def IX_ETHNPE_ACC_FRAME_LENGTH_MAX
 * 
 * @brief maximum mbuf length supported by the NPE
 * 
 * @sa IX_ETHNPE_ACC_FRAME_LENGTH_MAX
 */
#define IX_ETHNPE_ACC_FRAME_LENGTH_MAX (16320)

/**
 * @def IX_ETHNPE_ACC_FRAME_LENGTH_DEFAULT
 * 
 * @brief default mbuf length supported by the NPE
 * 
 * @sa IX_ETHNPE_ACC_FRAME_LENGTH_DEFAULT
 */
#define IX_ETHNPE_ACC_FRAME_LENGTH_DEFAULT (1522)

/**
 * @def IX_ETHNPE_ACC_LENGTH_OFFSET
 *
 * @brief Offset of the cluster length field in the word shared with the NPEs
 */
#define IX_ETHNPE_ACC_LENGTH_OFFSET 16

/**
 * @def IX_ETHNPE_ACC_PKTLENGTH_MASK
 *
 * @brief Mask of the cluster length field in the word shared with the NPEs
 */
#define IX_ETHNPE_ACC_PKTLENGTH_MASK 0x3fff


/**
 *@}
 */

#endif /* IxEthNpe_H */
#endif /* __doxygen_HIDE */
