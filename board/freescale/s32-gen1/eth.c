// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2019 NXP
 */

#include <common.h>
#include <dm.h>
#include <asm/io.h>
#include <net.h>
#include <netdev.h>
#include <phy.h>
#include <malloc.h>
#include <asm/types.h>
#include <asm/arch/soc.h>
#if CONFIG_IS_ENABLED(DWC_ETH_QOS_S32CC)
#include <dm/platform_data/dwc_eth_qos_dm.h>
#endif

/* GMAC driver for common chassis */
#if CONFIG_IS_ENABLED(DWC_ETH_QOS_S32CC)

/* driver platform data (TODO: remove when switching to DT) */
static struct eqos_pdata dwmac_pdata = {
	.eth = {
		/* registers base address */
		.iobase = (phys_addr_t)ETHERNET_0_BASE_ADDR,
		/* default phy mode */
		.phy_interface = PHY_INTERFACE_MODE_RGMII,
		/* generic fake HW addr */
		.enetaddr = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 },
		/* max 1 Gbps */
		.max_speed = 1000,
	},
	/* vendor specific driver config */
	.config = &eqos_s32cc_config,
};

U_BOOT_DEVICE(dwmac_s32cc) = {
	.name = "eth_eqos",
	.platdata = &dwmac_pdata,
};

/* GMAC platform specific setup */

void setup_iomux_enet_gmac(int intf)
{
	/* configure interface specific pins */
	/* Note: this is valid for NXP S32G EVB only */
	switch (intf) {
	case PHY_INTERFACE_MODE_SGMII:
		/* TODO: pinmuxing for SGMII  */
		break;

	case PHY_INTERFACE_MODE_RGMII:
		/* set PE2 - MSCR[66] - for TX CLK */
		writel(SIUL2_MSCR_S32_G1_ENET_TX_CLK,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_GMAC0_TX_CLK));
		/* set PE3 - MSCR[67] - for TX_EN */
		writel(SIUL2_MSCR_S32_G1_ENET_TX_EN,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE3));
		/* set PE4 - MSCR[68] - for TX_D0 */
		writel(SIUL2_MSCR_S32_G1_ENET_TX_D0,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE4));
		/* set PE5 - MSCR[69] - for TX_D1 */
		writel(SIUL2_MSCR_S32_G1_ENET_TX_D1,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE5));
		/* set PE6 - MSCR[70] - for TX_D2 */
		writel(SIUL2_MSCR_S32_G1_ENET_TX_D2,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE6));
		/* set PE7 - MSCR[71] - for TX_D3 */
		writel(SIUL2_MSCR_S32_G1_ENET_TX_D3,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE7));
		/* set PE8 - MSCR[72] - for RX_CLK */
		writel(SIUL2_MSCR_S32_G1_ENET_RX_CLK,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE8));
		writel(SIUL2_MSCR_S32_G1_ENET_RX_CLK_IN,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_GMAC0_RX_CLK_IN));
		/* set PD9 - MSCR[73] - for RX_DV */
		writel(SIUL2_MSCR_S32_G1_ENET_RX_DV,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE9));
		writel(SIUL2_MSCR_S32_G1_ENET_RX_DV_IN,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_GMAC0_RXDV_IN));
		/* set PE10 - MSCR[74] - for RX_D0 */
		writel(SIUL2_MSCR_S32_G1_ENET_RX_D0,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE10));
		writel(SIUL2_MSCR_S32_G1_ENET_RX_D0_IN,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_GMAC0_RXD0_IN));
		/* set PE11 - MSCR[75] - for RX_D1 */
		writel(SIUL2_MSCR_S32_G1_ENET_RX_D1,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE11));
		writel(SIUL2_MSCR_S32_G1_ENET_RX_D1_IN,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_GMAC0_RXD1_IN));
		/* set PE12 - MSCR[76] - for RX_D2 */
		writel(SIUL2_MSCR_S32_G1_ENET_RX_D2,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE12));
		writel(SIUL2_MSCR_S32_G1_ENET_RX_D2_IN,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_GMAC0_RXD2_IN));
		/* set PE13 - MSCR[77] - for RX_D3 */
		writel(SIUL2_MSCR_S32_G1_ENET_RX_D3,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PE13));
		writel(SIUL2_MSCR_S32_G1_ENET_RX_D3_IN,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_GMAC0_RXD3_IN));
		/* GMAC_MDC */
		writel(SIUL2_MSCR_S32_G1_ENET_MDC,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PD12));
		/* GMAC_MDIO */
		writel(SIUL2_MSCR_S32_G1_ENET_MDIO,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_PD13));
		writel(SIUL2_MSCR_S32_G1_ENET_RX_D3_IN,
		       SIUL2_0_MSCRn(SIUL2_MSCR_S32_G1_GMAC0_MDI_IN));
		break;

	case PHY_INTERFACE_MODE_RMII:
		/* TODO: pinmuxing for RMII  */
		break;

	case PHY_INTERFACE_MODE_MII:
		/* TODO: pinmuxing for MII  */
		break;

	default:
		break;
	}
}

static u32 gmac_calc_link_speed_divider(u32 idx)
{
	u32 div;

	switch (idx) {
	case 0:
		div = 50 - 1; /* 2.5MHz */
		break;
	case 1:
		div = 5 - 1; /* 25MHz */
		break;
	default:
	case 2:
		div = 1 - 1; /* 125MHz */
		break;
	}

	return div;
}

int set_tx_clk_enet_gmac(u32 idx)
{
	mux_div_clk_config(MC_CGM0_BASE_ADDR, 10, 0,
			   gmac_calc_link_speed_divider(idx));

	return 0;
}

void setup_clocks_enet_gmac(int intf)
{
	/* setup the mux clock divider for GMAC_TS_CLK (200 MHz) */
	mux_source_clk_config(MC_CGM0_BASE_ADDR, 9,
			      MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI4);

	/* configure interface specific clocks */
	switch (intf) {
	case PHY_INTERFACE_MODE_SGMII:
		/* TODO: clocks cfg for SGMII */
		break;

	case PHY_INTERFACE_MODE_RGMII:
		/* setup the mux clock divider for GMAC_0_TX_CLK
		 * (125/25/2.5 MHz)
		 */
		mux_source_clk_config(MC_CGM0_BASE_ADDR, 10,
				      MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI5);
		set_tx_clk_enet_gmac(dwmac_pdata.eth.max_speed);

		/* setup the mux clock divider for GMAC_0_RX_CLK
		 * (Ext source from PHY - RX)
		 */
		mux_source_clk_config(MC_CGM0_BASE_ADDR, 11,
				      MC_CGM_MUXn_CSC_SEL_GMAC_RX_CLK);
		break;

	case PHY_INTERFACE_MODE_RMII:
		/* TODO: clocks cfg for RMII */
		break;

	case PHY_INTERFACE_MODE_MII:
		/* TODO: clocks cfg for MII */
		break;

	default:
		break;
	}
}

#endif