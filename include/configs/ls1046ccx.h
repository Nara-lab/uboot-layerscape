/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2016 Freescale Semiconductor
 * Copyright 2019 NXP
 */

#ifndef __LS1046CCX_H__
#define __LS1046CCX_H__

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

#define CONFIG_DDR_ECC
#define CONFIG_ECC_INIT_VIA_DDRCONTROLLER
#define CONFIG_MEM_INIT_VALUE           0xdeadbeef

#ifdef CONFIG_SD_BOOT
#define CONFIG_SYS_FSL_PBL_PBI board/freescale/ls1046ardb/ls1046ardb_pbi.cfg
#ifdef CONFIG_EMMC_BOOT
#define CONFIG_SYS_FSL_PBL_RCW \
	board/freescale/ls1046ardb/ls1046ardb_rcw_emmc.cfg
#else
#define CONFIG_SYS_FSL_PBL_RCW board/freescale/ls1046ardb/ls1046ardb_rcw_sd.cfg
#endif
#elif defined(CONFIG_QSPI_BOOT)
#define CONFIG_SYS_FSL_PBL_RCW \
	board/freescale/ls1046ardb/ls1046ardb_rcw_qspi.cfg
#define CONFIG_SYS_FSL_PBL_PBI \
	board/freescale/ls1046ardb/ls1046ardb_qspi_pbi.cfg
#define CONFIG_SYS_UBOOT_BASE		0x40100000
#define CONFIG_SYS_SPL_ARGS_ADDR	0x90000000
#endif

#ifndef SPL_NO_IFC
/* IFC */
#define CONFIG_FSL_IFC
#endif

/*
 * Environment
 */
#ifndef SPL_NO_ENV
#define CONFIG_ENV_OVERWRITE
#endif

#define CONFIG_SYS_MMC_ENV_DEV		0

#define CONFIG_ENV_SIZE			0x2000		/* 8KB */
#define CONFIG_ENV_OFFSET		0x500000	/* 5MB */
#define CONFIG_ENV_SECT_SIZE		0x40000		/* 256KB */

/* FMan */
#ifndef SPL_NO_FMAN

#ifdef CONFIG_NET
#define CONFIG_PHY_REALTEK
#endif

#ifdef CONFIG_SYS_DPAA_FMAN
#define CONFIG_FMAN_ENET
#define RGMII_PHY1_ADDR			0x1
#define RGMII_PHY2_ADDR			0x2

#define SGMII_PHY1_ADDR			0x3
#define SGMII_PHY2_ADDR			0x4

#define FM1_10GEC1_PHY_ADDR		0x0

#define FDT_SEQ_MACADDR_FROM_ENV

#define CONFIG_ETHPRIME			"FM1@DTSEC3"
#endif

#endif

/* QSPI device */
#ifndef SPL_NO_QSPI
#ifdef CONFIG_FSL_QSPI
#define CONFIG_SPI_FLASH_SPANSION
#define FSL_QSPI_FLASH_SIZE		SZ_8M
#define FSL_QSPI_FLASH_NUM		1
#endif
#endif

#ifndef SPL_NO_MISC
#undef CONFIG_BOOTCOMMAND
#ifdef CONFIG_TFABOOT
#define QSPI_NOR_BOOTCOMMAND "run distro_bootcmd; run qspi_bootcmd; "	\
			   "env exists secureboot && esbc_halt;;"
#define SD_BOOTCOMMAND "run distro_bootcmd;run sd_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"
#else
#if defined(CONFIG_QSPI_BOOT)
#define CONFIG_BOOTCOMMAND "run distro_bootcmd; run qspi_bootcmd; "	\
			   "env exists secureboot && esbc_halt;;"
#elif defined(CONFIG_SD_BOOT)
#define CONFIG_BOOTCOMMAND "run distro_bootcmd;run sd_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"
#endif
#endif
#endif

#include <asm/fsl_secure_boot.h>

#endif /* __LS1046CCX_H__ */
