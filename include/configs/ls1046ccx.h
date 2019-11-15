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

/* Move up so can fit bootloader in 8MB FLASH */
#undef CONFIG_SYS_FMAN_FW_ADDR
#define CONFIG_SYS_FMAN_FW_ADDR                0x400000

#define CONFIG_LAYERSCAPE_NS_ACCESS

#define CONFIG_DIMM_SLOTS_PER_CTLR	1
/* Physical Memory Map */
#define CONFIG_CHIP_SELECTS_PER_CTRL	4

#define CONFIG_DDR_SPD
#define SPD_EEPROM_ADDRESS		0x51
#define CONFIG_SYS_SPD_BUS_NUM		0

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

#define SPL_NO_MISC
#undef CONFIG_BOOTCOMMAND
#define QSPI_NOR_BOOTCOMMAND "run qspi_bootcmd"
#define SD_BOOTCOMMAND "run sd_bootcmd;"

#include <asm/fsl_secure_boot.h>

#endif /* __LS1046CCX_H__ */
