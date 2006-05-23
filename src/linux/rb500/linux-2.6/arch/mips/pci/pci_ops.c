/*
 * Copyright 2001 MontaVista Software Inc.
 * Author: MontaVista Software, Inc.
 *         	stevel@mvista.com or source@mvista.com
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <linux/config.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/pci_channel.h>
#include <asm/rb/rb500.h>
#include <asm/rb/rb500_pci.h> 
#include <asm/rb/rb500_korina.h> 

#define PCI_ACCESS_READ  0
#define PCI_ACCESS_WRITE 1

static unsigned int korinaCnfgRegs[25] = {
	KORINA_CNFG1, KORINA_CNFG2, KORINA_CNFG3, KORINA_CNFG4,	
	KORINA_CNFG5, KORINA_CNFG6, KORINA_CNFG7, KORINA_CNFG8,
	KORINA_CNFG9, KORINA_CNFG10, KORINA_CNFG11, KORINA_CNFG12,
	KORINA_CNFG13, KORINA_CNFG14, KORINA_CNFG15, KORINA_CNFG16,
	KORINA_CNFG17, KORINA_CNFG18, KORINA_CNFG19, KORINA_CNFG20,
	KORINA_CNFG21, KORINA_CNFG22, KORINA_CNFG23, KORINA_CNFG24
};

extern struct resource rb500_res_pci_io1;
extern struct resource rb500_res_pci_mem1;

#define PCI_CFG_SET(bus, slot, func, off) \
	(rc32434_pci->pcicfga = (0x80000000 | ((bus) << 16) | ((slot)<<11) | \
			    ((func)<<8) | (off)))

static int config_access(u32 type, u32 bus, u8 devfn, u32 where, u32 *data)
{
	/* 
	 * config cycles are on 4 byte boundary only
	 */
	u32 slot = PCI_SLOT(devfn);
	u32 func = PCI_FUNC(devfn);
	
	PCI_CFG_SET(bus, slot, func, where);
	rc32434_sync();

	if (type == PCI_ACCESS_WRITE) {
	        rc32434_pci->pcicfgd = *data;
	}
	else {
		*data = rc32434_pci->pcicfgd;
	}
	rc32434_sync();

	/*
	 * Revisit: check for master or target abort.
	 */
	return 0;
}

static inline int config_write(u8 bus, u8 devfn, u8 where, u32 data)
{
	return config_access(PCI_ACCESS_WRITE, bus, devfn, where, &data);
}

static inline int config_read(u8 bus, u8 devfn, u8 where, u32 *data)
{
	return config_access(PCI_ACCESS_READ, bus, devfn, where, data);
}

/*
 * We can't address 8 and 16 bit words directly.  Instead we have to
 * read/write a 32bit word and mask/modify the data we actually want.
 */
static int read_config_byte (struct pci_dev *dev, int where, u8 *val)
{
	u32 data = 0;
	*val = 0xff;

	if (config_read(dev->bus->number, dev->devfn, where, &data))
		return -1;

	*val = (data >> ((where & 3) << 3)) & 0xff;

	return PCIBIOS_SUCCESSFUL;
}


static int read_config_word (struct pci_dev *dev, int where, u16 *val)
{
	u32 data = 0;
	*val = 0xffff;

	if (where & 1)
		return PCIBIOS_BAD_REGISTER_NUMBER;

	if (config_read(dev->bus->number, dev->devfn, where, &data))
		return -1;

	*val = (data >> ((where & 3) << 3)) & 0xffff;

	return PCIBIOS_SUCCESSFUL;
}

static int read_config_dword (struct pci_dev *dev, int where, u32 *val)
{
	u32 data = 0;
	*val = 0xffffffff;

	if (where & 3)
		return PCIBIOS_BAD_REGISTER_NUMBER;
	
	if (config_read(dev->bus->number, dev->devfn, where, &data))
		return -1;

	*val = data;

	return PCIBIOS_SUCCESSFUL;
}


static int write_config_byte (struct pci_dev *dev, int where, u8 val)
{
	u32 data = 0;
       
	if (config_read(dev->bus->number, dev->devfn, where, &data))
		return -1;

	data = (data & ~(0xff << ((where & 3) << 3))) |
	       (val << ((where & 3) << 3));

	if (config_write(dev->bus->number, dev->devfn, where, data))
		return -1;

	return PCIBIOS_SUCCESSFUL;
}

static int write_config_word (struct pci_dev *dev, int where, u16 val)
{
        u32 data = 0;

	if (where & 1)
		return PCIBIOS_BAD_REGISTER_NUMBER;
       
        if (config_read(dev->bus->number, dev->devfn, where, &data))
		return -1;

	data = (data & ~(0xffff << ((where & 3) << 3))) | 
	       (val << ((where & 3) << 3));

	if (config_write(dev->bus->number, dev->devfn, where, data))
	       return -1;

	return PCIBIOS_SUCCESSFUL;
}

static int write_config_dword(struct pci_dev *dev, int where, u32 val)
{
	if (where & 3)
		return PCIBIOS_BAD_REGISTER_NUMBER;

	if (config_write(dev->bus->number, dev->devfn, where, val))
		return -1;

	return PCIBIOS_SUCCESSFUL;
}

static struct pci_ops rb500_pci_ops = {
	read_config_byte,
        read_config_word,
	read_config_dword,
	write_config_byte,
	write_config_word,
	write_config_dword
};

void rb500_init_pci_channels(void) {
	mips_pci_channels[0].pci_ops = &rb500_pci_ops;
	mips_pci_channels[0].io_resource = &rb500_res_pci_io1;
	mips_pci_channels[0].mem_resource = &rb500_res_pci_mem1;
	mips_pci_channels[0].first_devfn = PCI_DEVFN(1, 0);
	mips_pci_channels[0].last_devfn = PCI_DEVFN(31, 0);
}

void __init rb500_pcibridge_init(void)
{
	unsigned int pcicValue, pcicData = 0;
        unsigned int dummyRead, pciCntlVal;
	int loopCount;
	unsigned int pciConfigAddr;

	printk("PCI: Initializing PCI\n");
        pcicValue = rc32434_pci->pcic;
	pcicValue = (pcicValue >> PCIM_SHFT) & PCIM_BIT_LEN;
        if (!((pcicValue == PCIM_H_EA) ||
	     (pcicValue == PCIM_H_IA_FIX) ||
		(pcicValue == PCIM_H_IA_RR))) {
		printk("PCI init error!!!\n");
		/* Not in Host Mode, return ERROR */
		return;
	}

        /* Enables the Idle Grant mode, Arbiter Parking */
	pcicData |=(PCIC_igm_m|PCIC_eap_m|PCIC_en_m);
        rc32434_pci->pcic = pcicData; /* Enable the PCI bus Interface */
	/* Zero out the PCI status & PCI Status Mask */
        for(;;)
        {
	   pcicData = rc32434_pci->pcis;
           if (!(pcicData & PCIS_rip_m))
                break;
        }

	rc32434_pci->pcis = 0;
        rc32434_pci->pcism = 0xFFFFFFFF;
	/* Zero out the PCI decoupled registers */
	rc32434_pci->pcidac=0; /* disable PCI decoupled accesses at initialization */
	rc32434_pci->pcidas=0; /* clear the status */
	rc32434_pci->pcidasm=0x0000007F; /* Mask all the interrupts */
	/* Mask PCI Messaging Interrupts */
	rc32434_pci_msg->pciiic = 0;
	rc32434_pci_msg->pciiim = 0xFFFFFFFF;
	rc32434_pci_msg->pciioic = 0;
	rc32434_pci_msg->pciioim = 0;


        /* Setup PCILB0 as Memory Window */
	rc32434_pci->pcilba[0].a = (unsigned int) (PCI_ADDR_START);

	/* setup the PCI map address as same as the local address */

	rc32434_pci->pcilba[0].m = (unsigned int) (PCI_ADDR_START);

	/* Setup PCILBA1 as MEM */
#ifdef __MIPSEB__
	        rc32434_pci->pcilba[0].c = ( ((SIZE_16MB & 0x1f) << PCILBAC_size_b) | PCILBAC_sb_m);
#else
	        rc32434_pci->pcilba[0].c = ( ((SIZE_16MB & 0x1f) << PCILBAC_size_b) );
#endif
	dummyRead = rc32434_pci->pcilba[0].c; /* flush the CPU write Buffers */

	rc32434_pci->pcilba[1].a = 0x60000000;
    
	rc32434_pci->pcilba[1].m = 0x60000000;
        /* setup PCILBA2 as IO Window*/
#ifdef __MIPSEB__
	rc32434_pci->pcilba[1].c = (((SIZE_256MB & 0x1f) << PCILBAC_size_b )| PCILBAC_sb_m);
#else
	rc32434_pci->pcilba[1].c = ( ((SIZE_256MB & 0x1f) << PCILBAC_size_b));
#endif
  
	dummyRead = rc32434_pci->pcilba[1].c; /* flush the CPU write Buffers */
	rc32434_pci->pcilba[2].a = 0x18C00000;
    
	rc32434_pci->pcilba[2].m = 0x18FFFFFF;
        /* setup PCILBA2 as IO Window*/
#ifdef __MIPSEB__
	rc32434_pci->pcilba[2].c = (((SIZE_4MB & 0x1f) << PCILBAC_size_b) | PCILBAC_sb_m );
#else
	rc32434_pci->pcilba[2].c = ( ((SIZE_4MB & 0x1f) << PCILBAC_size_b)  );
#endif  
  
	dummyRead = rc32434_pci->pcilba[2].c; /* flush the CPU write Buffers */


	rc32434_pci->pcilba[3].a = 0x18800000;

	rc32434_pci->pcilba[3].m = 0x18800000;
	/* Setup PCILBA3 as IO Window */

#ifdef __MIPSEB__
	rc32434_pci->pcilba[3].c = ( (((SIZE_1MB & 0x1ff) << PCILBAC_size_b) | PCILBAC_msi_m)   |  PCILBAC_sb_m);
#else
	rc32434_pci->pcilba[3].c = (((SIZE_1MB & 0x1ff) << PCILBAC_size_b) | PCILBAC_msi_m);
#endif
	dummyRead = rc32434_pci->pcilba[2].c; /* flush the CPU write Buffers */

	pciConfigAddr=(unsigned int)(0x80000004);
	for(loopCount=0;loopCount<24;loopCount++){
		rc32434_pci->pcicfga=pciConfigAddr;
		dummyRead=rc32434_pci->pcicfga;
		rc32434_pci->pcicfgd = korinaCnfgRegs[loopCount];
		dummyRead=rc32434_pci->pcicfgd;
		pciConfigAddr += 4;
	}
	rc32434_pci->pcitc = (unsigned int)((PCITC_RTIMER_VAL&0xff) << PCITC_rtimer_b)
		| ((PCITC_DTIMER_VAL&0xff) << PCITC_dtimer_b);

	pciCntlVal=rc32434_pci->pcic;
	pciCntlVal &=~(PCIC_tnr_m);
	rc32434_pci->pcic = pciCntlVal;
	pciCntlVal=rc32434_pci->pcic;

	rc32434_sync();   
}

