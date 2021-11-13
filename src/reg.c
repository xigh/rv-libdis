#include <stdio.h>

static const char *regnames[32] = {
    /* 00 */ "zero",
    /* 01 */ "ra",
    /* 02 */ "sp",
    /* 03 */ "gp",
    /* 04 */ "tp",
    /* 05 */ "t0",
    /* 06 */ "t1",
    /* 07 */ "t2",

    /* 08 */ "s0",  // or fp
    /* 09 */ "s1",
    /* 10 */ "a0",
    /* 11 */ "a1",
    /* 12 */ "a2",
    /* 13 */ "a3",
    /* 14 */ "a4",
    /* 15 */ "a5",

    /* 16 */ "a6",
    /* 17 */ "a7",
    /* 18 */ "s2",
    /* 19 */ "s3",
    /* 20 */ "s4",
    /* 21 */ "s5",
    /* 22 */ "s6",
    /* 23 */ "s7",

    /* 24 */ "s8",
    /* 25 */ "s9",
    /* 26 */ "s10",
    /* 27 */ "s11",
    /* 28 */ "t3",
    /* 29 */ "t4",
    /* 30 */ "t5",
    /* 31 */ "t6",
};

static const char *fregnames[32] = {
    /* 00 */ "ft0",
    /* 01 */ "ft1",
    /* 02 */ "ft2",
    /* 03 */ "ft3",
    /* 04 */ "ft4",
    /* 05 */ "ft5",
    /* 06 */ "ft6",
    /* 07 */ "ft7",

    /* 08 */ "fs0",
    /* 09 */ "fs1",
    /* 10 */ "fa0",
    /* 11 */ "fa1",
    /* 12 */ "fa2",
    /* 13 */ "fa3",
    /* 14 */ "fa4",
    /* 15 */ "fa5",

    /* 16 */ "fa6",
    /* 17 */ "fa7",
    /* 18 */ "fs2",
    /* 19 */ "fs3",
    /* 20 */ "fs4",
    /* 21 */ "fs5",
    /* 22 */ "fs6",
    /* 23 */ "fs7",

    /* 24 */ "fs8",
    /* 25 */ "fs9",
    /* 26 */ "fs10",
    /* 27 */ "fs11",
    /* 28 */ "ft8",
    /* 29 */ "ft9",
    /* 30 */ "ft10",
    /* 31 */ "ft11",
};

int use_regnames = 1;

const char *rv_reg2name(int reg)
{
    const char *rn = regnames[reg];
    if (use_regnames)
        return rn;
    static char tmp_rn[10];
    snprintf(tmp_rn, sizeof tmp_rn, "x%d", reg);
    return tmp_rn;
}

const char *rv_freg2name(int reg)
{
    const char *rn = fregnames[reg];
    if (use_regnames)
        return rn;
    static char tmp_rn[10];
    snprintf(tmp_rn, sizeof tmp_rn, "f%d", reg);
    return tmp_rn;
}
