#ifndef __DDR_H
#define __DDR_H

/* DDR frequency presets */
#ifdef CONFIG_DDR_HIGH_SPEED
#define DDR_FREQ_DEFAULT    3200
#else
#define DDR_FREQ_DEFAULT    2400
#endif

#define TIMING_AUTO     0
#define TIMING_MANUAL   1

struct ddr_config {
    unsigned int frequency;     /* MHz */
    int timing_mode;            /* TIMING_AUTO or TIMING_MANUAL */
    unsigned int rank_count;
    void *platform_data;        /* Opaque, platform-specific */
};

/* Function pointer table - platform fills these in */
struct ddr_ops {
    int (*init)(struct ddr_config *cfg);
    int (*train)(struct ddr_config *cfg);
    void (*shutdown)(void);
};

/* Weak symbol - platform must override */
struct ddr_ops *get_ddr_ops(void);

#endif /* __DDR_H */
