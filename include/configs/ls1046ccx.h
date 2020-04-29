/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2016 Freescale Semiconductor
 * Copyright 2019 NXP
 */

#ifndef __LS1046CCX_H__
#define __LS1046CCX_H__

#define SPL_NO_MISC
#include "ls1046a_common.h"

#define CONFIG_SYS_CLK_FREQ		100000000
#define CONFIG_DDR_CLK_FREQ		100000000


#define CONFIG_LAYERSCAPE_NS_ACCESS

#define CONFIG_DIMM_SLOTS_PER_CTLR	1
/* Physical Memory Map */
#define CONFIG_CHIP_SELECTS_PER_CTRL	4

#define CONFIG_DDR_SPD
#define SPD_EEPROM_ADDRESS		0x51
#define CONFIG_SYS_SPD_BUS_NUM		0

#define CONFIG_ENV_SIZE			0x2000
#define CONFIG_SYS_MMC_ENV_DEV		0

#ifndef SPL_NO_FMAN
#ifdef CONFIG_SYS_DPAA_FMAN

#define CONFIG_FMAN_ENET
#define RGMII_PHY1_ADDR			0x0
#define RGMII_PHY2_ADDR			0x3

#define SGMII_PHY1_ADDR			0xc
#define SGMII_PHY2_ADDR			0xf

#define SGMII_PHY3_ADDR			0x0
#define SGMII_PHY4_ADDR			0x1

#define FDT_SEQ_MACADDR_FROM_ENV

#define CONFIG_ETHPRIME			"FM1@DTSEC3"
#endif

#endif

/* QSPI device */
#ifndef SPL_NO_QSPI
#ifdef CONFIG_FSL_QSPI
#define CONFIG_SPI_FLASH_ISSI
#define FSL_QSPI_FLASH_SIZE		SZ_16M
#define FSL_QSPI_FLASH_NUM		1
#endif
#endif

#define SD_BOOTCOMMAND "run system_load"
#define QSPI_NOR_BOOTCOMMAND "run system_boot"
#define CONFIG_EXTRA_ENV_SETTINGS \
	"ethaddr=ce:ce:ce:ce:ce:01\0" \
	"eth1addr=ce:ce:ce:ce:ce:02\0" \
	"eth2addr=ce:ce:ce:ce:ce:03\0" \
	"eth3addr=ce:ce:ce:ce:ce:04\0" \
	"eth4addr=ce:ce:ce:ce:ce:05\0" \
	"eth5addr=ce:ce:ce:ce:ce:06\0" \
	"hwconfig=fsl_ddr:bank_intlv=auto\0" \
	"bootargs=earlycon=uart8250,mmio,0x21c0500 console=ttyS0,115200\0" \
	"bootargs_enable_loader=setenv bootargs ${bootargs} loader\0" \
	"bootargs_set_rootfs=setenv bootargs ${bootargs} root=/dev/sda${bootarg_rootpart} ro\0" \
	"bootargs_set_console=setenv bootargs ${bootargs} console=ttyS0,${baudrate} earlycon=uart8250,mmio,0x21c0500\0" \
	"bootarg_rootpart=2\0" \
	"loadaddr_ram=0xa0000000\0" \
	"loadaddr_flash=0x0\0" \
	"loadaddr_ram_kernel=0xa0800000\0" \
	"loadaddr_ram_dtb=0xa0000000\0" \
	"filename_onetimeenv=uEnv.onetime.txt\0" \
	"ram_to_flash=" \
		"sf probe && " \
		"sf erase ${loadaddr_flash} +${filesize} && " \
		"sf write ${loadaddr_ram} ${loadaddr_flash} ${filesize}\0" \
	"ram_to_sata=" \
		"scsi rescan && " \
		"ext4write scsi 0:1 ${loadaddr_ram} /${filename} ${filesize}\0" \
	"sdcard_to_ram=ext4load mmc 0:1 ${loadaddr_ram} /${filename}\0" \
	"sata_to_ram=ext4load scsi 0:1 ${loadaddr_ram} /${filename}\0" \
	"sdcard_to_flash=" \
		"run sdcard_to_ram && " \
		"run ram_to_flash\0" \
	"sdcard_to_flash_pbl=" \
		"setenv filename bl2.pbl && " \
		"setenv loadaddr_flash 0x000000 && " \
		"run sdcard_to_flash\0" \
	"sdcard_to_flash_fib=" \
		"setenv filename fip.bin && " \
		"setenv loadaddr_flash 0x100000 && " \
		"run sdcard_to_flash\0" \
	"sdcard_to_ram_dtb=" \
		"setenv filename linux.dtb && " \
		"setenv loadaddr_ram ${loadaddr_ram_dtb} && " \
		"run sdcard_to_ram\0" \
	"sdcard_to_ram_kernel=" \
		"setenv filename Image && " \
		"setenv loadaddr_ram ${loadaddr_ram_kernel} && " \
		"run sdcard_to_ram\0" \
	"sata_to_ram_dtb=" \
		"setenv filename boot-${bootarg_rootpart}/linux.dtb && " \
		"setenv loadaddr_ram ${loadaddr_ram_dtb} && " \
		"run sata_to_ram\0" \
	"sata_to_ram_kernel=" \
		"setenv filename boot-${bootarg_rootpart}/Image && " \
		"setenv loadaddr_ram ${loadaddr_ram_kernel} && " \
		"run sata_to_ram\0" \
	"sata_to_env_rootpart=" \
		"setenv filename ${filename_onetimeenv} && " \
		"if run sata_to_ram; then " \
			"env import -t ${loadaddr_ram} ${filesize} lastboot defaultrootpart onetimerootpart; " \
		"else " \
			"true; " \
		"fi && " \
		"if env exists defaultrootpart; then "\
			"setenv bootarg_rootpart ${defaultrootpart}; " \
		"else " \
			"setenv defaultrootpart ${bootarg_rootpart}; " \
		"fi && " \
		"if env exists onetimerootpart; then " \
			"setenv bootarg_rootpart ${onetimerootpart}; " \
		"else " \
			"true; " \
		"fi\0" \
	"env_to_sata_rootpart=" \
		"if env exists onetimerootpart || test \"${lastboot}\" != \"${bootarg_rootpart}\"; then " \
			"setenv lastboot ${bootarg_rootpart} && " \
			"env export -t ${loadaddr_ram} lastboot defaultrootpart && " \
			"setenv filename ${filename_onetimeenv} && " \
			"run ram_to_sata; " \
		"else " \
			"true; " \
		"fi\0" \
	"boot_kernel_loader=" \
		"run bootargs_enable_loader && " \
		"run sdcard_to_ram_dtb && " \
		"run sdcard_to_ram_kernel && " \
		"booti ${loadaddr_ram_kernel} - ${loadaddr_ram_dtb}\0" \
	"boot_kernel_normal=" \
		"run bootargs_set_rootfs && " \
		"run bootargs_set_console && " \
		"run sata_to_ram_dtb && " \
		"run sata_to_ram_kernel && " \
		"booti ${loadaddr_ram_kernel} - ${loadaddr_ram_dtb}\0" \
	"system_load=" \
		"if run sdcard_to_flash_pbl && run sdcard_to_flash_fib; then " \
			"run boot_kernel_loader; esbc_halt; " \
		"else " \
			"echo Failed to find file firmware ${filename} && esbc_halt; " \
		"fi\0" \
	"system_boot=" \
		"if scsi rescan && run sata_to_env_rootpart && run env_to_sata_rootpart; then " \
			"run boot_kernel_normal; esbc_halt; " \
		"else " \
			"echo Failed to setup environment && esbc_halt; " \
		"fi\0" \

#include <asm/fsl_secure_boot.h>

#endif /* __LS1046CCX_H__ */
