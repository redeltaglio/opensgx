#ifndef HW_PPC_H
#define HW_PPC_H 1

void ppc_set_irq(PowerPCCPU *cpu, int n_IRQ, int level);

/* PowerPC hardware exceptions management helpers */
typedef void (*clk_setup_cb)(void *opaque, uint32_t freq);
typedef struct clk_setup_t clk_setup_t;
struct clk_setup_t {
    clk_setup_cb cb;
    void *opaque;
};
static inline void clk_setup (clk_setup_t *clk, uint32_t freq)
{
    if (clk->cb != NULL)
        (*clk->cb)(clk->opaque, freq);
}

struct ppc_tb_t {
    /* Time base management */
    int64_t  tb_offset;    /* Compensation                    */
    int64_t  atb_offset;   /* Compensation                    */
    uint32_t tb_freq;      /* TB frequency                    */
    /* Decrementer management */
    uint64_t decr_next;    /* Tick for next decr interrupt    */
    uint32_t decr_freq;    /* decrementer frequency           */
    QEMUTimer *decr_timer;
    /* Hypervisor decrementer management */
    uint64_t hdecr_next;    /* Tick for next hdecr interrupt  */
    QEMUTimer *hdecr_timer;
    uint64_t purr_load;
    uint64_t purr_start;
    void *opaque;
    uint32_t flags;
};

/* PPC Timers flags */
#define PPC_TIMER_BOOKE              (1 << 0) /* Enable Booke support */
#define PPC_TIMER_E500               (1 << 1) /* Enable e500 support */
#define PPC_DECR_UNDERFLOW_TRIGGERED (1 << 2) /* Decr interrupt triggered when
                                               * the most significant bit
                                               * changes from 0 to 1.
                                               */
#define PPC_DECR_ZERO_TRIGGERED      (1 << 3) /* Decr interrupt triggered when
                                               * the decrementer reaches zero.
                                               */
#define PPC_DECR_UNDERFLOW_LEVEL     (1 << 4) /* Decr interrupt active when
                                               * the most significant bit is 1.
                                               */

uint64_t cpu_ppc_get_tb(ppc_tb_t *tb_env, uint64_t vmclk, int64_t tb_offset);
clk_setup_cb cpu_ppc_tb_init (CPUPPCState *env, uint32_t freq);
/* Embedded PowerPC DCR management */
typedef uint32_t (*dcr_read_cb)(void *opaque, int dcrn);
typedef void (*dcr_write_cb)(void *opaque, int dcrn, uint32_t val);
int ppc_dcr_init (CPUPPCState *env, int (*dcr_read_error)(int dcrn),
                  int (*dcr_write_error)(int dcrn));
int ppc_dcr_register (CPUPPCState *env, int dcrn, void *opaque,
                      dcr_read_cb drc_read, dcr_write_cb dcr_write);
clk_setup_cb ppc_40x_timers_init (CPUPPCState *env, uint32_t freq,
                                  unsigned int decr_excp);

/* Embedded PowerPC reset */
void ppc40x_core_reset(PowerPCCPU *cpu);
void ppc40x_chip_reset(PowerPCCPU *cpu);
void ppc40x_system_reset(PowerPCCPU *cpu);
void PREP_debug_write (void *opaque, uint32_t addr, uint32_t val);

extern CPUWriteMemoryFunc * const PPC_io_write[];
extern CPUReadMemoryFunc * const PPC_io_read[];
void PPC_debug_write (void *opaque, uint32_t addr, uint32_t val);

void ppc40x_irq_init (CPUPPCState *env);
void ppce500_irq_init (CPUPPCState *env);
void ppc6xx_irq_init (CPUPPCState *env);
void ppc970_irq_init (CPUPPCState *env);
void ppcPOWER7_irq_init (CPUPPCState *env);

/* PPC machines for OpenBIOS */
enum {
    ARCH_PREP = 0,
    ARCH_MAC99,
    ARCH_HEATHROW,
    ARCH_MAC99_U3,
};

#define FW_CFG_PPC_WIDTH	(FW_CFG_ARCH_LOCAL + 0x00)
#define FW_CFG_PPC_HEIGHT	(FW_CFG_ARCH_LOCAL + 0x01)
#define FW_CFG_PPC_DEPTH	(FW_CFG_ARCH_LOCAL + 0x02)
#define FW_CFG_PPC_TBFREQ	(FW_CFG_ARCH_LOCAL + 0x03)
#define FW_CFG_PPC_CLOCKFREQ	(FW_CFG_ARCH_LOCAL + 0x04)
#define FW_CFG_PPC_IS_KVM       (FW_CFG_ARCH_LOCAL + 0x05)
#define FW_CFG_PPC_KVM_HC       (FW_CFG_ARCH_LOCAL + 0x06)
#define FW_CFG_PPC_KVM_PID      (FW_CFG_ARCH_LOCAL + 0x07)
/* OpenBIOS has FW_CFG_PPC_NVRAM_ADDR as +0x08 */
#define FW_CFG_PPC_BUSFREQ      (FW_CFG_ARCH_LOCAL + 0x09)

#define PPC_SERIAL_MM_BAUDBASE 399193

/* ppc_booke.c */
void ppc_booke_timers_init(PowerPCCPU *cpu, uint32_t freq, uint32_t flags);

#endif
