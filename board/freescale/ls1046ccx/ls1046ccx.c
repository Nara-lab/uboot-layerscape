// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 */

#include <common.h>
#include <i2c.h>
#include <fdt_support.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/fsl_serdes.h>
#include <asm/arch/ppa.h>
#include <asm/arch/soc.h>
#include <asm/arch-fsl-layerscape/fsl_icid.h>
#include <hwconfig.h>
#include <ahci.h>
#include <mmc.h>
#include <scsi.h>
#include <fm_eth.h>
#include <fsl_csu.h>
#include <fsl_esdhc.h>
#include <power/mc34vr500_pmic.h>
#include <fsl_sec.h>

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_f(void)
{
	fsl_lsch2_early_init_f();

	return 0;
}

#ifndef CONFIG_SPL_BUILD
int checkboard(void)
{
	return 0;
}

int pld_enable_reset_req(void)
{
	int err;

	/* enable the reset_req driven reset by programming
	 * the GPIO Expander / PLD on IIC3 (Semtech SX1503) */

	err = i2c_set_bus_num(2);
	if (err < 0) {
		printf("Failed to set I2C Bus to IIC3.\n");
		return err;
	}

	i2c_reg_write(0x20, 0x05, 0xff);
	if (i2c_reg_read(0x20, 0x05) != 0xff) {
		printf("Failed to enable pull-ups on bank a.\n");
		return err;
	}

	i2c_reg_write(0x20, 0x23, 0x0a);
	if (i2c_reg_read(0x20, 0x23) != 0x0a) {
		printf("Failed to set PLD mode on bank a.\n");
		return err;
	}

	i2c_reg_write(0x20, 0x21, 0x01);
	if (i2c_reg_read(0x20, 0x21) != 0x01) {
		printf("Failed to enable PLD on bank a.\n");
		return err;
	}

	err = i2c_set_bus_num(0);
	if (err < 0) {
		printf("Failed to set I2C Bus to IIC1.\n");
		return err;
	}

	printf("Reset PLD: live\n");

	return 0;
}

int board_init(void)
{

#ifdef CONFIG_SECURE_BOOT
	/*
	 * In case of Secure Boot, the IBR configures the SMMU
	 * to allow only Secure transactions.
	 * SMMU must be reset in bypass mode.
	 * Set the ClientPD bit and Clear the USFCFG Bit
	 */
	u32 val;
	val = (in_le32(SMMU_SCR0) | SCR0_CLIENTPD_MASK) & ~(SCR0_USFCFG_MASK);
	out_le32(SMMU_SCR0, val);
	val = (in_le32(SMMU_NSCR0) | SCR0_CLIENTPD_MASK) & ~(SCR0_USFCFG_MASK);
	out_le32(SMMU_NSCR0, val);
#endif

#ifdef CONFIG_FSL_CAAM
	sec_init();
#endif

#ifdef CONFIG_FSL_LS_PPA
	ppa_init();
#endif

	return 0;
}

int board_setup_core_volt(u32 vdd)
{
	return 0;
}

int power_init_board(void)
{
	return 0;
}

void config_board_mux(void)
{
	struct ccsr_scfg *scfg = (struct ccsr_scfg *)CONFIG_SYS_FSL_SCFG_ADDR;

	/* enable IIC3 */
	out_be32(&scfg->rcwpmuxcr0, 0x0000);
}

#ifdef CONFIG_MISC_INIT_R
int misc_init_r(void)
{
	config_board_mux();

	pld_enable_reset_req();
	return 0;
}
#endif


int ft_board_setup(void *blob, bd_t *bd)
{
	int i;
	u64 base[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];

	/* fixup DT for the two DDR banks */
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		base[i] = gd->bd->bi_dram[i].start;
		size[i] = gd->bd->bi_dram[i].size;
	}

	fdt_fixup_memory_banks(blob, base, size, CONFIG_NR_DRAM_BANKS);
	ft_cpu_setup(blob, bd);

#ifdef CONFIG_SYS_DPAA_FMAN
	fdt_fixup_fman_ethernet(blob);
#endif

	fdt_fixup_icid(blob);

	return 0;
}
#endif
