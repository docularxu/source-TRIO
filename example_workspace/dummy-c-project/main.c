/*
 * Minimal DDR initialization example
 * Demonstrates: function pointers, #ifdef branches, __weak overrides
 */
#include "ddr.h"

/* Global DDR configuration - shared across modules */
struct ddr_config g_ddr_cfg;

int main(void)
{
    struct ddr_ops *ops;

    /* Phase 1: Get platform-specific ops */
    ops = get_ddr_ops();
    if (!ops || !ops->init || !ops->train) {
        return -1;
    }

    /* Phase 2: Initialize DDR controller */
    g_ddr_cfg.frequency = DDR_FREQ_DEFAULT;
    g_ddr_cfg.timing_mode = TIMING_AUTO;
    int ret = ops->init(&g_ddr_cfg);
    if (ret)
        return ret;

    /* Phase 3: Run training */
#ifdef CONFIG_DDR_TRAINING
    ret = ops->train(&g_ddr_cfg);
    if (ret)
        return ret;
#endif

    return 0;
}

/* __weak: platform can override this */
__attribute__((weak))
struct ddr_ops *get_ddr_ops(void)
{
    return NULL;  /* Must be overridden by platform */
}
