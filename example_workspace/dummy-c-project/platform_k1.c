/*
 * Platform-specific DDR implementation for K1
 * Demonstrates: __weak override, #ifdef branches, ops binding
 */
#include "ddr.h"

/* Forward declarations */
static int k1_ddr_init(struct ddr_config *cfg);
static int k1_ddr_train(struct ddr_config *cfg);

/* Platform ops - overrides __weak get_ddr_ops() in main.c */
static struct ddr_ops k1_ops = {
    .init  = k1_ddr_init,
    .train = k1_ddr_train,
    .shutdown = NULL,   /* Not implemented yet */
};

struct ddr_ops *get_ddr_ops(void)
{
    return &k1_ops;
}

static int k1_ddr_init(struct ddr_config *cfg)
{
    if (!cfg)
        return -1;

    /* Configure PHY registers based on frequency */
#ifdef CONFIG_DDR_HIGH_SPEED
    /* High-speed path: additional voltage tuning */
    volatile unsigned int *phy_reg = (void *)0x20000000;
    *phy_reg = cfg->frequency | 0x80000000;  /* Enable high-speed flag */
#else
    volatile unsigned int *phy_reg = (void *)0x20000000;
    *phy_reg = cfg->frequency;
#endif

    cfg->rank_count = 2;  /* K1 always dual-rank */
    return 0;
}

static int k1_ddr_train(struct ddr_config *cfg)
{
    if (cfg->timing_mode == TIMING_AUTO) {
        /* Auto-training sequence */
        volatile unsigned int *train_reg = (void *)0x20001000;
        *train_reg = 0x1;   /* Start training */

        /* Spin wait - real code would have timeout */
        while (*train_reg & 0x1)
            ;

        return (*train_reg & 0x100) ? 0 : -1;  /* Check pass/fail bit */
    }

    return 0;  /* Manual mode: assume configured */
}
