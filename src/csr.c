#include <stdio.h>
#include <stdint.h>

typedef struct csr
{
    uint64_t num;
    const char *name;
} csr_t;

static const csr_t csrnames[] = {
    {.num = 0x000, .name = "ustatus"},
    {.num = 0x004, .name = "uie"},
    {.num = 0x005, .name = "utvec"},

    {.num = 0x040, .name = "uscratch"},
    {.num = 0x041, .name = "uepc"},
    {.num = 0x042, .name = "ucause"},
    {.num = 0x043, .name = "utval"},
    {.num = 0x044, .name = "uip"},

    {.num = 0x001, .name = "fflags"},
    {.num = 0x002, .name = "frm"},
    {.num = 0x003, .name = "fcsr"},

    {.num = 0xc00, .name = "cycle"},
    {.num = 0xc01, .name = "time"},
    {.num = 0xc02, .name = "instret"},
    {.num = 0xc03, .name = "hpmcounter3"},
    {.num = 0xc04, .name = "hpmcounter4"},
    {.num = 0xc05, .name = "hpmcounter5"},
    {.num = 0xc06, .name = "hpmcounter6"},
    {.num = 0xc07, .name = "hpmcounter7"},
    {.num = 0xc08, .name = "hpmcounter8"},
    {.num = 0xc09, .name = "hpmcounter9"},
    {.num = 0xc0a, .name = "hpmcounter10"},
    {.num = 0xc0b, .name = "hpmcounter11"},
    {.num = 0xc0c, .name = "hpmcounter12"},
    {.num = 0xc0d, .name = "hpmcounter13"},
    {.num = 0xc0e, .name = "hpmcounter14"},
    {.num = 0xc0f, .name = "hpmcounter15"},
    {.num = 0xc10, .name = "hpmcounter16"},
    {.num = 0xc11, .name = "hpmcounter17"},
    {.num = 0xc12, .name = "hpmcounter18"},
    {.num = 0xc13, .name = "hpmcounter19"},
    {.num = 0xc14, .name = "hpmcounter20"},
    {.num = 0xc15, .name = "hpmcounter21"},
    {.num = 0xc16, .name = "hpmcounter22"},
    {.num = 0xc17, .name = "hpmcounter23"},
    {.num = 0xc18, .name = "hpmcounter24"},
    {.num = 0xc19, .name = "hpmcounter25"},
    {.num = 0xc1a, .name = "hpmcounter26"},
    {.num = 0xc1b, .name = "hpmcounter27"},
    {.num = 0xc1c, .name = "hpmcounter28"},
    {.num = 0xc1d, .name = "hpmcounter29"},
    {.num = 0xc1e, .name = "hpmcounter30"},
    {.num = 0xc1f, .name = "hpmcounter31"},

    {.num = 0xc80, .name = "cycleh"},
    {.num = 0xc81, .name = "timeh"},
    {.num = 0xc82, .name = "instreth"},
    {.num = 0xc83, .name = "hpmcounter3h"},
    {.num = 0xc84, .name = "hpmcounter4h"},
    {.num = 0xc85, .name = "hpmcounter5h"},
    {.num = 0xc86, .name = "hpmcounter6h"},
    {.num = 0xc87, .name = "hpmcounter7h"},
    {.num = 0xc88, .name = "hpmcounter8h"},
    {.num = 0xc89, .name = "hpmcounter9h"},
    {.num = 0xc8a, .name = "hpmcounter10h"},
    {.num = 0xc8b, .name = "hpmcounter11h"},
    {.num = 0xc8c, .name = "hpmcounter12h"},
    {.num = 0xc8d, .name = "hpmcounter13h"},
    {.num = 0xc8e, .name = "hpmcounter14h"},
    {.num = 0xc8f, .name = "hpmcounter15h"},
    {.num = 0xc90, .name = "hpmcounter16h"},
    {.num = 0xc91, .name = "hpmcounter17h"},
    {.num = 0xc92, .name = "hpmcounter18h"},
    {.num = 0xc93, .name = "hpmcounter19h"},
    {.num = 0xc94, .name = "hpmcounter20h"},
    {.num = 0xc95, .name = "hpmcounter21h"},
    {.num = 0xc96, .name = "hpmcounter22h"},
    {.num = 0xc97, .name = "hpmcounter23h"},
    {.num = 0xc98, .name = "hpmcounter24h"},
    {.num = 0xc99, .name = "hpmcounter25h"},
    {.num = 0xc9a, .name = "hpmcounter26h"},
    {.num = 0xc9b, .name = "hpmcounter27h"},
    {.num = 0xc9c, .name = "hpmcounter28h"},
    {.num = 0xc9d, .name = "hpmcounter29h"},
    {.num = 0xc9e, .name = "hpmcounter30h"},
    {.num = 0xc9f, .name = "hpmcounter31h"},

    {.num = 0x100, .name = "sstatus"},
    {.num = 0x102, .name = "sedeleg"},
    {.num = 0x103, .name = "sideleg"},
    {.num = 0x104, .name = "sie"},
    {.num = 0x105, .name = "stvec"},
    {.num = 0x106, .name = "scounteren"},

    {.num = 0x140, .name = "sscratch"},
    {.num = 0x141, .name = "sepc"},
    {.num = 0x142, .name = "scause"},
    {.num = 0x143, .name = "stval"},
    {.num = 0x144, .name = "sip"},

    {.num = 0x180, .name = "satp"},

    {.num = 0xf11, .name = "mvendorid"},
    {.num = 0xf12, .name = "marchid"},
    {.num = 0xf13, .name = "mimpid"},
    {.num = 0xf14, .name = "mhartid"},

    {.num = 0x300, .name = "mstatus"},
    {.num = 0x301, .name = "misa"},
    {.num = 0x302, .name = "medeleg"},
    {.num = 0x303, .name = "mideleg"},
    {.num = 0x304, .name = "mie"},
    {.num = 0x305, .name = "mtvec"},
    {.num = 0x306, .name = "mtcounteren"},

    {.num = 0x340, .name = "mscratch"},
    {.num = 0x341, .name = "mepc"},
    {.num = 0x342, .name = "mcause"},
    {.num = 0x343, .name = "mtval"},
    {.num = 0x344, .name = "mip"},

    {.num = 0x3a0, .name = "pmpcfg0"},
    {.num = 0x3a1, .name = "pmpcfg1"},
    {.num = 0x3a2, .name = "pmpcfg2"},
    {.num = 0x3a3, .name = "pmpcfg3"},

    {.num = 0x3b0, .name = "pmpaddr0"},
    {.num = 0x3b1, .name = "pmpaddr1"},
    {.num = 0x3b2, .name = "pmpaddr2"},
    {.num = 0x3b3, .name = "pmpaddr3"},
    {.num = 0x3b4, .name = "pmpaddr4"},
    {.num = 0x3b5, .name = "pmpaddr5"},
    {.num = 0x3b6, .name = "pmpaddr6"},
    {.num = 0x3b7, .name = "pmpaddr7"},
    {.num = 0x3b8, .name = "pmpaddr8"},
    {.num = 0x3b9, .name = "pmpaddr9"},
    {.num = 0x3ba, .name = "pmpaddr10"},
    {.num = 0x3bb, .name = "pmpaddr11"},
    {.num = 0x3bc, .name = "pmpaddr12"},
    {.num = 0x3bd, .name = "pmpaddr13"},
    {.num = 0x3be, .name = "pmpaddr14"},
    {.num = 0x3bf, .name = "pmpaddr15"},

    {0, 0},
};

const char *rv_csr2name(uint64_t csr)
{
    static char tmp[32];
    for (const csr_t *c = csrnames; c->name; c++)
    {
        if (csr == c->num)
            return c->name;
    }
    snprintf(tmp, sizeof tmp, "0x%lx", csr);
    return tmp;
}
