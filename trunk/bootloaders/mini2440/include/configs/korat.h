/*
 * (C) Copyright 2007-2008
 * Larry Johnson, lrj@acm.org
 *
 * (C) Copyright 2006-2007
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
 *
 * (C) Copyright 2006
 * Jacqueline Pira-Ferriol, AMCC/IBM, jpira-ferriol@fr.ibm.com
 * Alain Saurel,            AMCC/IBM, alain.saurel@fr.ibm.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * korat.h - configuration for Korat board
 */
#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 */
#define CONFIG_440EPX		1	/* Specific PPC440EPx		*/
#define CONFIG_4xx		1	/* ... PPC4xx family		*/
#define CONFIG_SYS_CLK_FREQ	33333333

#define CONFIG_BOARD_EARLY_INIT_F 1	/* Call board_early_init_f	*/
#define CONFIG_MISC_INIT_R	1	/* Call misc_init_r		*/

/*
 * Manufacturer's information serial EEPROM parameters
 */
#define MAN_DATA_EEPROM_ADDR	0x53	/* EEPROM I2C address		*/
#define MAN_INFO_FIELD		2
#define MAN_INFO_LENGTH		9
#define MAN_MAC_ADDR_FIELD	3
#define MAN_MAC_ADDR_LENGTH	12

/*
 * Base addresses -- Note these are effective addresses where the actual
 * resources get mapped (not physical addresses).
 */
#define CFG_MONITOR_LEN		(384 * 1024) /* Reserve 384 kiB for Monitor  */
#define CFG_MALLOC_LEN		(256 * 1024) /* Reserve 256 kiB for malloc() */

#define CFG_SDRAM_BASE		0x00000000	/* _must_ be 0		*/
#define CFG_FLASH0_SIZE		0x01000000
#define CFG_FLASH0_ADDR		(-CFG_FLASH0_SIZE)
#define CFG_FLASH1_TOP		0xF8000000
#define CFG_FLASH1_MAX_SIZE	0x08000000
#define CFG_FLASH1_ADDR		(CFG_FLASH1_TOP - CFG_FLASH1_MAX_SIZE)
#define CFG_FLASH_BASE		CFG_FLASH1_ADDR	/* start of FLASH	*/
#define CFG_MONITOR_BASE	TEXT_BASE
#define CFG_OCM_BASE		0xe0010000	/* ocm			*/
#define CFG_OCM_DATA_ADDR	CFG_OCM_BASE
#define CFG_PCI_BASE		0xe0000000	/* Internal PCI regs	*/
#define CFG_PCI_MEMBASE		0x80000000	/* mapped pci memory	*/

/* Don't change either of these */
#define CFG_PERIPHERAL_BASE	0xef600000	/* internal peripherals	*/

#define CFG_USB2D0_BASE		0xe0000100
#define CFG_USB_DEVICE		0xe0000000
#define CFG_USB_HOST		0xe0000400
#define CFG_CPLD_BASE		0xc0000000

/*
 * Initial RAM & stack pointer
 */
/* 440EPx has 16KB of internal SRAM, so no need for D-Cache		*/
#undef CFG_INIT_RAM_DCACHE
#define CFG_INIT_RAM_ADDR	CFG_OCM_BASE	/* OCM			*/
#define CFG_INIT_RAM_END	(4 << 10)
#define CFG_GBL_DATA_SIZE	256	/* num bytes initial data	*/
#define CFG_GBL_DATA_OFFSET	(CFG_INIT_RAM_END - CFG_GBL_DATA_SIZE)
#define CFG_INIT_SP_OFFSET	CFG_POST_WORD_ADDR

/*
 * Serial Port
 */
#define CFG_EXT_SERIAL_CLOCK	11059200	/* ext. 11.059MHz clk	*/
#define CONFIG_BAUDRATE		115200
#define CONFIG_SERIAL_MULTI	1
/* define this if you want console on UART1 */
#undef CONFIG_UART1_CONSOLE

#define CFG_BAUDRATE_TABLE						\
	{300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200}

/*
 * Environment
 */
#define CFG_ENV_IS_IN_FLASH	1	/* use FLASH for environ vars	*/

/*
 * FLASH related
 */
#define CFG_FLASH_CFI			/* The flash is CFI compatible	      */
#define CFG_FLASH_CFI_DRIVER		/* Use common CFI driver	      */
#define CONFIG_FLASH_CFI_LEGACY		/* Allow hard-coded config for FLASH0 */

#define CFG_FLASH_BANKS_LIST	{ CFG_FLASH1_ADDR, CFG_FLASH0_ADDR }

#define CFG_MAX_FLASH_BANKS	2	/* max number of memory banks	      */
#define CFG_MAX_FLASH_SECT	1024	/* max number of sectors on one chip  */

#define CFG_FLASH_ERASE_TOUT	120000	/* Timeout for Flash Erase (in ms)    */
#define CFG_FLASH_WRITE_TOUT	500	/* Timeout for Flash Write (in ms)    */

#define CFG_FLASH_USE_BUFFER_WRITE 1	/* use buffered writes (20x faster)   */
#define CFG_FLASH_PROTECTION	1	/* use hardware flash protection      */

#define CFG_FLASH_EMPTY_INFO	      /* print 'E' for empty sector on flinfo */
#define CFG_FLASH_QUIET_TEST	1	/* don't warn upon unknown flash      */

#define CFG_ENV_SECT_SIZE	0x20000	/* size of one complete sector	      */
#define CFG_ENV_ADDR		(CFG_FLASH1_TOP - CFG_ENV_SECT_SIZE)
#define	CFG_ENV_SIZE		0x2000	/* Total Size of Environment Sector   */

/* Address and size of Redundant Environment Sector */
#define CFG_ENV_ADDR_REDUND	(CFG_ENV_ADDR - CFG_ENV_SECT_SIZE)
#define CFG_ENV_SIZE_REDUND	CFG_ENV_SIZE

/*
 * DDR SDRAM
 */
#define CFG_MBYTES_SDRAM        (512)	/* 512 MiB	TODO: remove	*/
#define CONFIG_DDR_DATA_EYE		/* use DDR2 optimization	*/
#define CONFIG_SPD_EEPROM		/* Use SPD EEPROM for setup	*/
#define CONFIG_ZERO_SDRAM		/* Zero SDRAM after setup	*/
#define CONFIG_DDR_ECC			/* Use ECC when available	*/
#define SPD_EEPROM_ADDRESS	{0x50}
#define CONFIG_PROG_SDRAM_TLB
#define CFG_DRAM_TEST
#define CFG_MEM_TOP_HIDE	(4 << 10) /* don't use last 4kbytes	*/
					/* 440EPx errata CHIP 11	*/

/*
 * I2C
 */
#define CONFIG_HARD_I2C		1	/* I2C with hardware support	*/
#undef	CONFIG_SOFT_I2C			/* I2C bit-banged		*/
#define CFG_I2C_SPEED		400000	/* I2C speed and slave address	*/
#define CFG_I2C_SLAVE		0x7F

#define CFG_I2C_MULTI_EEPROMS
#define CFG_I2C_EEPROM_ADDR	(0xa8>>1)
#define CFG_I2C_EEPROM_ADDR_LEN 1
#define CFG_EEPROM_PAGE_WRITE_BITS 3
#define CFG_EEPROM_PAGE_WRITE_DELAY_MS 10

/* I2C RTC */
#define CONFIG_RTC_M41T60	1
#define CFG_I2C_RTC_ADDR	0x68

/* I2C SYSMON (LM73)							*/
#define CONFIG_DTT_LM73		1	/* National Semi's LM73		*/
#define CONFIG_DTT_SENSORS	{2}	/* Sensor addresses		*/
#define CFG_DTT_MAX_TEMP	70
#define CFG_DTT_MIN_TEMP	-30

#define CONFIG_PREBOOT	"echo;"						\
	"echo Type \\\"run flash_nfs\\\" to mount root filesystem over NFS;" \
	"echo"

#undef	CONFIG_BOOTARGS

/* Setup some board specific values for the default environment variables */
#define CONFIG_HOSTNAME		korat
#define CFG_BOOTFILE		"bootfile=/tftpboot/korat/uImage\0"
#define CFG_ROOTPATH		"rootpath=/opt/eldk/ppc_4xxFP\0"

/* Note: kernel_addr and ramdisk_addr assume that FLASH1 is 64 MiB. */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
	CFG_BOOTFILE							\
	CFG_ROOTPATH							\
	"netdev=eth0\0"							\
	"nfsargs=setenv bootargs root=/dev/nfs rw "			\
		"nfsroot=${serverip}:${rootpath}\0"			\
	"ramargs=setenv bootargs root=/dev/ram rw\0"			\
	"addip=setenv bootargs ${bootargs} "				\
		"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}"	\
		":${hostname}:${netdev}:off panic=1\0"			\
	"addtty=setenv bootargs ${bootargs} console=ttyS0,${baudrate}\0"\
	"flash_nfs=run nfsargs addip addtty;"				\
		"bootm ${kernel_addr}\0"				\
	"flash_self=run ramargs addip addtty;"				\
		"bootm ${kernel_addr} ${ramdisk_addr}\0"		\
	"net_nfs=tftp 200000 ${bootfile};run nfsargs addip addtty;"     \
	        "bootm\0"						\
	"kernel_addr=F4000000\0"					\
	"ramdisk_addr=F4400000\0"					\
	"load=tftp 200000 /tftpboot/${hostname}/u-boot.bin\0"		\
	"update=protect off FFFA0000 FFFFFFFF;era FFFA0000 FFFFFFFF;"	\
		"cp.b 200000 FFFA0000 60000\0"			        \
	"upd=run load update\0"						\
	""
#define CONFIG_BOOTCOMMAND	"run flash_self"

#define CONFIG_BOOTDELAY	5	/* autoboot after 5 seconds	*/

#define CONFIG_LOADS_ECHO	1	/* echo on for serial download	*/
#define CFG_LOADS_BAUD_CHANGE	1	/* allow baudrate change	*/

#define	CONFIG_IBM_EMAC4_V4	1
#define CONFIG_MII		1	/* MII PHY management		*/
#define CONFIG_PHY_ADDR		2	/* PHY address, See schematics	*/
#define CONFIG_PHY_DYNAMIC_ANEG	1

#undef CONFIG_PHY_RESET			/* Don't do software PHY reset	*/
#define CONFIG_PHY_GIGE		1	/* Include GbE speed/duplex detection */

#define CONFIG_HAS_ETH0
#define CFG_RX_ETH_BUFFER	32	/* Number of ethernet rx 	*/
					/*   buffers & descriptors	*/
#define CONFIG_NET_MULTI	1
#define CONFIG_HAS_ETH1		1	/* add support for "eth1addr"	*/
#define CONFIG_PHY1_ADDR	3

/* USB */
#define CONFIG_USB_OHCI
#define CONFIG_USB_STORAGE

/* Comment this out to enable USB 1.1 device */
#define USB_2_0_DEVICE

/* Partitions */
#define CONFIG_MAC_PARTITION
#define CONFIG_DOS_PARTITION
#define CONFIG_ISO_PARTITION

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_SUBNETMASK

/*
 * Command line configuration.
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_ASKENV
#define CONFIG_CMD_DATE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_DTT
#define CONFIG_CMD_DIAG
#define CONFIG_CMD_EEPROM
#define CONFIG_CMD_ELF
#define CONFIG_CMD_FAT
#define CONFIG_CMD_I2C
#define CONFIG_I2C_CMD_TREE
#define CONFIG_CMD_IRQ
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET
#define CONFIG_CMD_NFS
#define CONFIG_CMD_PCI
#define CONFIG_CMD_PING
#define CONFIG_CMD_REGINFO
#define CONFIG_CMD_SDRAM
#define CONFIG_CMD_USB

/* POST support */
#define CONFIG_POST		(CFG_POST_CACHE	   | \
				 CFG_POST_CPU	   | \
				 CFG_POST_ECC	   | \
				 CFG_POST_ETHER	   | \
				 CFG_POST_FPU	   | \
				 CFG_POST_I2C	   | \
				 CFG_POST_MEMORY   | \
				 CFG_POST_RTC	   | \
				 CFG_POST_SPR	   | \
				 CFG_POST_UART)

#define CFG_POST_WORD_ADDR	(CFG_GBL_DATA_OFFSET - 0x4)
#define CONFIG_LOGBUFFER
#define CFG_POST_CACHE_ADDR	0xC8000000	/* free virtual address     */

#define CFG_CONSOLE_IS_IN_ENV	/* Otherwise it catches logbuffer as output */

#define CONFIG_SUPPORT_VFAT

/*
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP			/* undef to save memory		*/
#define CFG_PROMPT	        "=> "	/* Monitor Command Prompt	*/
#if defined(CONFIG_CMD_KGDB)
#define CFG_CBSIZE	        1024	/* Console I/O Buffer Size	*/
#else
#define CFG_CBSIZE	        256	/* Console I/O Buffer Size	*/
#endif
#define CFG_PBSIZE              (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)
					/* Print Buffer Size 		*/
#define CFG_MAXARGS	        16	/* max number of command args	*/
#define CFG_BARGSIZE	        CFG_CBSIZE /* Boot Argument Buffer Size	*/

#define CFG_MEMTEST_START	0x0400000 /* memtest works on		*/
#define CFG_MEMTEST_END		0x0C00000 /* 4 ... 12 MB in DRAM	*/

#define CFG_LOAD_ADDR		0x100000  /* default load address	*/
#define CFG_EXTBDINFO		1  /* To use extended board_into (bd_t)	*/

#define CFG_HZ		        1000	/* decrementer freq: 1 ms ticks	*/

#define CONFIG_CMDLINE_EDITING	1	/* add command line history	*/
#define CONFIG_LOOPW		1	/* enable loopw command		*/
#define CONFIG_MX_CYCLIC	1	/* enable mdc/mwc commands	*/
#define CONFIG_ZERO_BOOTDELAY_CHECK	/* check for keypress on bootdelay==0 */
#define CONFIG_VERSION_VARIABLE 1	/* include version env variable	*/

/*
 * Korat-specific options
 */
#define CFG_KORAT_MAN_RESET_MS	10000	/* timeout for manufacturer reset */

/*
 * PCI stuff
 */
/* General PCI */
#define CONFIG_PCI			/* include pci support		*/
#define CONFIG_PCI_PNP			/* do pci plug-and-play		*/
#define CFG_PCI_CACHE_LINE_SIZE	0	/* to avoid problems with PNP	*/
#define CONFIG_PCI_SCAN_SHOW		/* show pci devices on startup	*/
#define CFG_PCI_TARGBASE	0x80000000 	/* PCIaddr mapped to	*/
						/*   CFG_PCI_MEMBASE	*/
/* Board-specific PCI */
#define CFG_PCI_TARGET_INIT
#define CFG_PCI_MASTER_INIT

#define CFG_PCI_SUBSYS_VENDORID 0x10e8	/* AMCC				*/
#define CFG_PCI_SUBSYS_ID       0xcafe	/* Whatever			*/

/*
 * For booting Linux, the board info and command line data have to be in the
 * first 8 MB of memory, since this is the maximum mapped by the Linux kernel
 * during initialization.
 */
#define CFG_BOOTMAPSZ		(8 << 20) /* Initial Memory map for Linux */

/*
 * External Bus Controller (EBC) Setup
 */

/* Memory Bank 0 (NOR-FLASH) initialization				*/
#if CFG_FLASH0_SIZE == 0x01000000
#define CFG_EBC_PB0AP		0x04017300
#define CFG_EBC_PB0CR		(CFG_FLASH0_ADDR | 0x0009A000)
#elif CFG_FLASH0_SIZE == 0x04000000
#define CFG_EBC_PB0AP		0x04017300
#define CFG_EBC_PB0CR		(CFG_FLASH0_ADDR | 0x000DA000)
#else
#error Unable to configure chip select for current CFG_FLASH0_SIZE
#endif

/* Memory Bank 1 (NOR-FLASH) initialization				*/
#if CFG_FLASH1_MAX_SIZE == 0x08000000
#define CFG_EBC_PB1AP		0x04017300
#define CFG_EBC_PB1CR		(CFG_FLASH1_ADDR | 0x000FA000)
#else
#error Unable to configure chip select for current CFG_FLASH1_MAX_SIZE
#endif

/* Memory Bank 2 (CPLD) initialization					*/
#define CFG_EBC_PB2AP		0x04017300
#define CFG_EBC_PB2CR		(CFG_CPLD_BASE | 0x00038000)

/*
 * GPIO Setup
 *
 * Korat GPIO usage:
 *
 *                   Init.
 * Pin    Source I/O value Function
 * ------ ------ --- ----- ---------------------------------
 * GPIO00  Alt1  I/O   x   PerAddr07
 * GPIO01  Alt1  I/O   x   PerAddr06
 * GPIO02  Alt1  I/O   x   PerAddr05
 * GPIO03  GPIO   x    x   GPIO03 to expansion bus connector
 * GPIO04  GPIO   x    x   GPIO04 to expansion bus connector
 * GPIO05  GPIO   x    x   GPIO05 to expansion bus connector
 * GPIO06  Alt1   O    x   PerCS1 (2nd NOR flash)
 * GPIO07  Alt1   O    x   PerCS2 (CPLD)
 * GPIO08  Alt1   O    x   PerCS3 to expansion bus connector
 * GPIO09  Alt1   O    x   PerCS4 to expansion bus connector
 * GPIO10  Alt1   O    x   PerCS5 to expansion bus connector
 * GPIO11  Alt1   I    x   PerErr
 * GPIO12  GPIO   O    0   ATMega !Reset
 * GPIO13  GPIO   O    1   SPI Atmega !SS
 * GPIO14  GPIO   O    1   Write protect EEPROM #1 (0xA8)
 * GPIO15  GPIO   O    0   CPU Run LED !On
 * GPIO16  Alt1   O    x   GMC1TxD0
 * GPIO17  Alt1   O    x   GMC1TxD1
 * GPIO18  Alt1   O    x   GMC1TxD2
 * GPIO19  Alt1   O    x   GMC1TxD3
 * GPIO20  Alt1   I    x   RejectPkt0
 * GPIO21  Alt1   I    x   RejectPkt1
 * GPIO22  GPIO   I    x   PGOOD_DDR
 * GPIO23  Alt1   O    x   SCPD0
 * GPIO24  Alt1   O    x   GMC0TxD2
 * GPIO25  Alt1   O    x   GMC0TxD3
 * GPIO26  GPIO? I/O   x   IIC0SDA (selected in SDR0_PFC4)
 * GPIO27  GPIO   O    0   PHY #0 1000BASE-X select
 * GPIO28  GPIO   O    0   PHY #1 1000BASE-X select
 * GPIO29  GPIO   I    x   Test jumper !Present
 * GPIO30  GPIO   I    x   SFP module #0 !Present
 * GPIO31  GPIO   I    x   SFP module #1 !Present
 *
 * GPIO32  GPIO   O    1   SFP module #0 Tx !Enable
 * GPIO33  GPIO   O    1   SFP module #1 Tx !Enable
 * GPIO34  Alt2   I    x   !UART1_CTS
 * GPIO35  Alt2   O    x   !UART1_RTS
 * GPIO36  Alt1   I    x   !UART0_CTS
 * GPIO37  Alt1   O    x   !UART0_RTS
 * GPIO38  Alt2   O    x   UART1_Tx
 * GPIO39  Alt2   I    x   UART1_Rx
 * GPIO40  Alt1   I    x   IRQ0 (Ethernet 0)
 * GPIO41  Alt1   I    x   IRQ1 (Ethernet 1)
 * GPIO42  Alt1   I    x   IRQ2 (PCI interrupt)
 * GPIO43  Alt1   I    x   IRQ3 (System Alert from CPLD)
 * GPIO44  xxxx   x    x   (grounded through pulldown)
 * GPIO45  GPIO   O    0   PHY #0 Enable
 * GPIO46  GPIO   O    0   PHY #1 Enable
 * GPIO47  GPIO   I    x   Reset switch !Pressed
 * GPIO48  GPIO   I    x   Shutdown switch !Pressed
 * GPIO49  xxxx   x    x   (reserved for trace port)
 *   .      .     .    .               .
 *   .      .     .    .               .
 *   .      .     .    .               .
 * GPIO63  xxxx   x    x   (reserved for trace port)
 */

#define CFG_GPIO_ATMEGA_RESET_	12
#define CFG_GPIO_ATMEGA_SS_	13
#define CFG_GPIO_PHY0_FIBER_SEL	27
#define CFG_GPIO_PHY1_FIBER_SEL	28
#define CFG_GPIO_SFP0_PRESENT_	30
#define CFG_GPIO_SFP1_PRESENT_	31
#define CFG_GPIO_SFP0_TX_EN_	32
#define CFG_GPIO_SFP1_TX_EN_	33
#define CFG_GPIO_PHY0_EN	45
#define CFG_GPIO_PHY1_EN	46
#define CFG_GPIO_RESET_PRESSED_	47

/*
 * PPC440 GPIO Configuration
 */
#define CFG_4xx_GPIO_TABLE { /*	  Out		  GPIO	Alternate1	Alternate2	Alternate3 */ \
{											\
/* GPIO Core 0 */									\
{GPIO0_BASE, GPIO_BI , GPIO_ALT1, GPIO_OUT_0}, /* GPIO0	EBC_ADDR(7)	DMA_REQ(2)	*/	\
{GPIO0_BASE, GPIO_BI , GPIO_ALT1, GPIO_OUT_0}, /* GPIO1	EBC_ADDR(6)	DMA_ACK(2)	*/	\
{GPIO0_BASE, GPIO_BI , GPIO_ALT1, GPIO_OUT_0}, /* GPIO2	EBC_ADDR(5)	DMA_EOT/TC(2)	*/	\
{GPIO0_BASE, GPIO_DIS, GPIO_SEL , GPIO_OUT_0}, /* GPIO3	EBC_ADDR(4)	DMA_REQ(3)	*/	\
{GPIO0_BASE, GPIO_DIS, GPIO_SEL , GPIO_OUT_0}, /* GPIO4	EBC_ADDR(3)	DMA_ACK(3)	*/	\
{GPIO0_BASE, GPIO_DIS, GPIO_SEL , GPIO_OUT_0}, /* GPIO5	EBC_ADDR(2)	DMA_EOT/TC(3)	*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_0}, /* GPIO6	EBC_CS_N(1)			*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_0}, /* GPIO7	EBC_CS_N(2)			*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_0}, /* GPIO8	EBC_CS_N(3)			*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_0}, /* GPIO9	EBC_CS_N(4)			*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_0}, /* GPIO10 EBC_CS_N(5)			*/	\
{GPIO0_BASE, GPIO_IN , GPIO_ALT1, GPIO_OUT_0}, /* GPIO11 EBC_BUS_ERR			*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_0}, /* GPIO12				*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_1}, /* GPIO13				*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_1}, /* GPIO14				*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_0}, /* GPIO15				*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_1}, /* GPIO16 GMCTxD(4)			*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_1}, /* GPIO17 GMCTxD(5)			*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_1}, /* GPIO18 GMCTxD(6)			*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_1}, /* GPIO19 GMCTxD(7)			*/	\
{GPIO0_BASE, GPIO_IN , GPIO_ALT1, GPIO_OUT_0}, /* GPIO20 RejectPkt0			*/	\
{GPIO0_BASE, GPIO_IN , GPIO_ALT1, GPIO_OUT_0}, /* GPIO21 RejectPkt1			*/	\
{GPIO0_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO22				*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_0}, /* GPIO23 SCPD0				*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_1}, /* GPIO24 GMCTxD(2)			*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_1}, /* GPIO25 GMCTxD(3)			*/	\
{GPIO0_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO26				*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_0}, /* GPIO27 EXT_EBC_REQ	USB2D_RXERROR	*/	\
{GPIO0_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_0}, /* GPIO28		USB2D_TXVALID	*/	\
{GPIO0_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO29 EBC_EXT_HDLA	USB2D_PAD_SUSPNDM */	\
{GPIO0_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO30 EBC_EXT_ACK	USB2D_XCVRSELECT*/	\
{GPIO0_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO31 EBC_EXR_BUSREQ	USB2D_TERMSELECT*/	\
},											\
{											\
/* GPIO Core 1 */									\
{GPIO1_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_1}, /* GPIO32 USB2D_OPMODE0	EBC_DATA(2)	*/	\
{GPIO1_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_1}, /* GPIO33 USB2D_OPMODE1	EBC_DATA(3)	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_ALT2, GPIO_OUT_0}, /* GPIO34 UART0_DCD_N	UART1_DSR_CTS_N	UART2_SOUT*/ \
{GPIO1_BASE, GPIO_OUT, GPIO_ALT2, GPIO_OUT_1}, /* GPIO35 UART0_8PIN_DSR_N UART1_RTS_DTR_N UART2_SIN*/ \
{GPIO1_BASE, GPIO_IN , GPIO_ALT1, GPIO_OUT_0}, /* GPIO36 UART0_8PIN_CTS_N EBC_DATA(0)	UART3_SIN*/ \
{GPIO1_BASE, GPIO_OUT, GPIO_ALT1, GPIO_OUT_1}, /* GPIO37 UART0_RTS_N	EBC_DATA(1)	UART3_SOUT*/ \
{GPIO1_BASE, GPIO_OUT, GPIO_ALT2, GPIO_OUT_1}, /* GPIO38 UART0_DTR_N	UART1_SOUT	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_ALT2, GPIO_OUT_0}, /* GPIO39 UART0_RI_N	UART1_SIN	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_ALT1, GPIO_OUT_0}, /* GPIO40 UIC_IRQ(0)			*/	\
{GPIO1_BASE, GPIO_IN , GPIO_ALT1, GPIO_OUT_0}, /* GPIO41 UIC_IRQ(1)			*/	\
{GPIO1_BASE, GPIO_IN , GPIO_ALT1, GPIO_OUT_0}, /* GPIO42 UIC_IRQ(2)			*/	\
{GPIO1_BASE, GPIO_IN , GPIO_ALT1, GPIO_OUT_0}, /* GPIO43 UIC_IRQ(3)			*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO44 UIC_IRQ(4)	DMA_ACK(1)	*/	\
{GPIO1_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_0}, /* GPIO45 UIC_IRQ(6)	DMA_EOT/TC(1)	*/	\
{GPIO1_BASE, GPIO_OUT, GPIO_SEL , GPIO_OUT_0}, /* GPIO46 UIC_IRQ(7)	DMA_REQ(0)	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO47 UIC_IRQ(8)	DMA_ACK(0)	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO48 UIC_IRQ(9)	DMA_EOT/TC(0)	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO49  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO50  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO51  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO52  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO53  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO54  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO55  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO56  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO57  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO58  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO59  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO60  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO61  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO62  Unselect via TraceSelect Bit	*/	\
{GPIO1_BASE, GPIO_IN , GPIO_SEL , GPIO_OUT_0}, /* GPIO63  Unselect via TraceSelect Bit	*/	\
}											\
}

/*
 * Internal Definitions
 *
 * Boot Flags
 */
#define BOOTFLAG_COLD	0x01	/* Normal Power-On: Boot from FLASH	*/
#define BOOTFLAG_WARM	0x02	/* Software reboot			*/

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	230400 /* speed to run kgdb serial port	*/
#define CONFIG_KGDB_SER_INDEX	2	/* which serial port to use	*/
#endif

#endif /* __CONFIG_H */
