#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "rv-dis.h"

const char *rounding_mods[] = {
    "rne",
    "rtz",
    "rdn",
    "rup",
    "rmm",
    "inv1",
    "inv2",
    0,
};

static int emit_op(mnemonic_t *m, const char *op, ...)
{
    va_list ap;
    int i;

    bzero(m, sizeof *m);
    m->op = strdup(op);
    va_start(ap, op);
    for (i = 0; i < 6; i++)
    {
        char *arg = va_arg(ap, char *);
        if (arg == 0)
            break;
        m->args[i] = strdup(arg);
    }
    va_end(ap);
    return i;
}

static int emit_op_i(mnemonic_t *m, const char *name, rv_int64_t imm)
{
    char tmp_imm[64];
    snprintf(tmp_imm, sizeof tmp_imm, "%lld", imm);
    return emit_op(m, name, tmp_imm, 0);
}

static int emit_op_x(mnemonic_t *m, const char *name, rv_int64_t imm)
{
    char tmp_imm[64];
    snprintf(tmp_imm, sizeof tmp_imm, "0x%llx", imm);
    return emit_op(m, name, tmp_imm, 0);
}

static int emit_op_rr(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2)
{
    const char *tmp_r1 = rv_reg2name(r1);
    const char *tmp_r2 = rv_reg2name(r2);
    return emit_op(m, name, tmp_r1, tmp_r2, 0);
}

static int emit_op_ri(mnemonic_t *m, const char *name, rv_uint8_t r, rv_int64_t imm)
{
    const char *tmp_r = rv_reg2name(r);
    char tmp_imm[64];
    snprintf(tmp_imm, sizeof tmp_imm, "%lld", imm);
    return emit_op(m, name, tmp_r, tmp_imm, 0);
}

static int emit_op_rx(mnemonic_t *m, const char *name, rv_uint8_t r, rv_int64_t imm)
{
    const char *tmp_r = rv_reg2name(r);
    char tmp_imm[64];
    snprintf(tmp_imm, sizeof tmp_imm, "0x%llx", imm);
    return emit_op(m, name, tmp_r, tmp_imm, 0);
}

static int emit_op_rrr(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2, rv_uint8_t r3)
{
    const char *tmp_r1 = rv_reg2name(r1);
    const char *tmp_r2 = rv_reg2name(r2);
    const char *tmp_r3 = rv_reg2name(r3);
    return emit_op(m, name, tmp_r1, tmp_r2, tmp_r3, 0);
}

static int emit_op_ff(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2)
{
    const char *tmp_r1 = rv_freg2name(r1);
    const char *tmp_r2 = rv_freg2name(r2);
    return emit_op(m, name, tmp_r1, tmp_r2, 0);
}

static int emit_op_ff_rnd(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2, rv_uint8_t rnd)
{
    const char *tmp_r1 = rv_freg2name(r1);
    const char *tmp_r2 = rv_freg2name(r2);
    return emit_op(m, name, tmp_r1, tmp_r2, rounding_mods[rnd & 7], 0);
}

static int emit_op_rf_rnd(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2, rv_uint8_t rnd)
{
    const char *tmp_r1 = rv_reg2name(r1);
    const char *tmp_r2 = rv_freg2name(r2);
    return emit_op(m, name, tmp_r1, tmp_r2, rounding_mods[rnd & 7], 0);
}

static int emit_op_fr_rnd(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2, rv_uint8_t rnd)
{
    const char *tmp_r1 = rv_freg2name(r1);
    const char *tmp_r2 = rv_reg2name(r2);
    return emit_op(m, name, tmp_r1, tmp_r2, rounding_mods[rnd & 7], 0);
}

static int emit_op_rff(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2, rv_uint8_t r3)
{
    const char *tmp_r1 = rv_reg2name(r1);
    const char *tmp_r2 = rv_freg2name(r2);
    const char *tmp_r3 = rv_freg2name(r3);
    return emit_op(m, name, tmp_r1, tmp_r2, tmp_r3, 0);
}

static int emit_op_fff(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2, rv_uint8_t r3)
{
    const char *tmp_r1 = rv_freg2name(r1);
    const char *tmp_r2 = rv_freg2name(r2);
    const char *tmp_r3 = rv_freg2name(r3);
    return emit_op(m, name, tmp_r1, tmp_r2, tmp_r3, 0);
}

static int emit_op_rf(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2)
{
    const char *tmp_r1 = rv_reg2name(r1);
    const char *tmp_r2 = rv_freg2name(r2);
    return emit_op(m, name, tmp_r1, tmp_r2, 0);
}

static int emit_op_fr(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2)
{
    const char *tmp_r1 = rv_freg2name(r1);
    const char *tmp_r2 = rv_reg2name(r2);
    return emit_op(m, name, tmp_r1, tmp_r2, 0);
}

static int emit_op_fff_rnd(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2, rv_uint8_t r3, rv_uint8_t rnd)
{
    const char *tmp_r1 = rv_freg2name(r1);
    const char *tmp_r2 = rv_freg2name(r2);
    const char *tmp_r3 = rv_freg2name(r3);
    return emit_op(m, name, tmp_r1, tmp_r2, tmp_r3, rounding_mods[rnd & 7], 0);
}

static int emit_op_ffff_rnd(mnemonic_t *m, const char *name, rv_uint8_t r1, rv_uint8_t r2, rv_uint8_t r3, rv_uint8_t r4, rv_uint8_t rnd)
{
    const char *tmp_r1 = rv_freg2name(r1);
    const char *tmp_r2 = rv_freg2name(r2);
    const char *tmp_r3 = rv_freg2name(r3);
    const char *tmp_r4 = rv_freg2name(r4);
    return emit_op(m, name, tmp_r1, tmp_r2, tmp_r3, tmp_r4, rounding_mods[rnd & 7], 0);
}

static int emit_op_rri(mnemonic_t *m, const char *name, rv_uint8_t r0, rv_uint8_t r1, rv_int64_t imm)
{
    const char *tmp_r0 = rv_reg2name(r0);
    const char *tmp_r1 = rv_reg2name(r1);
    char tmp_imm[64];
    snprintf(tmp_imm, sizeof tmp_imm, "%lld", imm);
    return emit_op(m, name, tmp_r0, tmp_r1, tmp_imm, 0);
}

static int emit_op_rrx(mnemonic_t *m, const char *name, rv_uint8_t r0, rv_uint8_t r1, rv_int64_t imm)
{
    const char *tmp_r0 = rv_reg2name(r0);
    const char *tmp_r1 = rv_reg2name(r1);
    char tmp_imm[64];
    snprintf(tmp_imm, sizeof tmp_imm, "0x%llx", imm);
    return emit_op(m, name, tmp_r0, tmp_r1, tmp_imm, 0);
}

static int emit_op_m(mnemonic_t *m, const char *name, rv_uint8_t r, rv_int64_t imm)
{
    const char *tmp_r = rv_reg2name(r);
    char tmp_mem[64];
    snprintf(tmp_mem, sizeof tmp_mem, "%lld(%s)", imm, tmp_r);
    return emit_op(m, name, tmp_mem, 0);
}

static int emit_op_rm(mnemonic_t *m, const char *name, rv_uint8_t r0, rv_uint8_t r1, rv_int64_t imm)
{
    const char *tmp_r1 = rv_reg2name(r1);
    char tmp_mem[64];
    snprintf(tmp_mem, sizeof tmp_mem, "%lld(%s)", imm, tmp_r1);
    const char *tmp_r0 = rv_reg2name(r0);
    return emit_op(m, name, tmp_r0, tmp_mem, 0);
}

static int emit_op_mr(mnemonic_t *m, const char *name, rv_uint8_t rs1, rv_int64_t imm, rv_uint8_t rs2)
{
    const char *tmp_rs1 = rv_reg2name(rs1);
    char tmp_mem[64];
    snprintf(tmp_mem, sizeof tmp_mem, "%lld(%s)", imm, tmp_rs1);
    const char *tmp_rs2 = rv_reg2name(rs2);
    return emit_op(m, name, tmp_rs2, tmp_mem, 0);
}

static int emit_op_fm(mnemonic_t *m, const char *name, rv_uint8_t r0, rv_uint8_t r1, rv_int64_t imm)
{
    const char *tmp_r1 = rv_reg2name(r1);
    char tmp_mem[64];
    snprintf(tmp_mem, sizeof tmp_mem, "%lld(%s)", imm, tmp_r1);
    const char *tmp_r0 = rv_freg2name(r0);
    return emit_op(m, name, tmp_r0, tmp_mem, 0);
}

static int emit_op_mf(mnemonic_t *m, const char *name, rv_uint8_t rs1, rv_int64_t imm, rv_uint8_t rs2)
{
    const char *tmp_rs1 = rv_reg2name(rs1);
    char tmp_mem[64];
    snprintf(tmp_mem, sizeof tmp_mem, "%lld(%s)", imm, tmp_rs1);
    const char *tmp_rs2 = rv_freg2name(rs2);
    return emit_op(m, name, tmp_mem, tmp_rs2, 0);
}

static int emit_op_cr(mnemonic_t *m, const char *name, rv_uint64_t csr, rv_uint8_t r0)
{
    const char *tmp_r0 = rv_reg2name(r0);
    return emit_op(m, name, rv_csr2name(csr), tmp_r0, 0);
}

static int emit_op_rc(mnemonic_t *m, const char *name, rv_uint8_t r0, rv_uint64_t csr)
{
    const char *tmp_r0 = rv_reg2name(r0);
    return emit_op(m, name, tmp_r0, rv_csr2name(csr), 0);
}

static int emit_op_rcr(mnemonic_t *m, const char *name, rv_uint8_t r0, rv_uint64_t csr, rv_uint8_t r1)
{
    const char *tmp_r0 = rv_reg2name(r0);
    const char *tmp_r1 = rv_reg2name(r1);
    return emit_op(m, name, tmp_r0, rv_csr2name(csr), tmp_r1, 0);
}

static int emit_op_rcu(mnemonic_t *m, const char *name, rv_uint8_t r0, rv_uint64_t csr, rv_uint64_t uimm)
{
    const char *tmp_r0 = rv_reg2name(r0);
    char tmp_uimm[64];
    snprintf(tmp_uimm, sizeof tmp_uimm, "0x%llx", uimm);
    return emit_op(m, name, tmp_r0, rv_csr2name(csr), tmp_uimm, 0);
}

static int emit_lr(mnemonic_t *m, const char *name, rv_uint8_t rd, rv_uint8_t rs1, rv_uint8_t mod)
{
    const char *tmp_rd = rv_reg2name(rd);
    const char *tmp_rs1 = rv_reg2name(rs1);
    char tmp_mem[32];
    snprintf(tmp_mem, sizeof tmp_mem, "(%s)", tmp_rs1);
    char tmp_name[64];
    switch (mod & 3)
    {
    case 0:
        return emit_op(m, name, tmp_rd, tmp_mem, 0);
    case 1:
        snprintf(tmp_name, sizeof tmp_name, "%s.rl", name);
        break;
    case 2:
        snprintf(tmp_name, sizeof tmp_name, "%s.aq", name);
        break;
    case 3:
        snprintf(tmp_name, sizeof tmp_name, "%s.aq.rl", name);
        break;
    }
    return emit_op(m, name, tmp_rd, tmp_mem, 0);
}

static int emit_amo(mnemonic_t *m, const char *name, rv_uint8_t rd, rv_uint8_t rs1, rv_uint8_t rs2, rv_uint8_t mod)
{
    const char *tmp_rd = rv_reg2name(rd);
    const char *tmp_rs1 = rv_reg2name(rs1);
    const char *tmp_rs2 = rv_reg2name(rs2);
    char tmp_mem[32];
    char tmp_name[64];
    snprintf(tmp_mem, sizeof tmp_mem, "(%s)", tmp_rs1);
    switch (mod & 3)
    {
    case 0:
        return emit_op(m, name, tmp_rd, tmp_rs2, tmp_mem, 0);
    case 1:
        snprintf(tmp_name, sizeof tmp_name, "%s.rl", name);
        break;
    case 2:
        snprintf(tmp_name, sizeof tmp_name, "%s.aq", name);
        break;
    case 3:
        snprintf(tmp_name, sizeof tmp_name, "%s.aq.rl", name);
        break;
    }
    return emit_op(m, name, tmp_rd, tmp_mem, tmp_rs2, 0);
}

int rv_disasm(inst_t *ir, mnemonic_t *m)
{
    switch (ir->op)
    {
    default:
        break;

        // -------------- load

    case RV_LB:
        return emit_op_rm(m, "lb", ir->rd, ir->rs1, ir->imm.i);

    case RV_LH:
        return emit_op_rm(m, "lh", ir->rd, ir->rs1, ir->imm.i);

    case RV_LW:
        return emit_op_rm(m, "lw", ir->rd, ir->rs1, ir->imm.i);

    case RV_LD:
        return emit_op_rm(m, "ld", ir->rd, ir->rs1, ir->imm.i);

    case RV_LBU:
        return emit_op_rm(m, "lbu", ir->rd, ir->rs1, ir->imm.i);

    case RV_LHU:
        return emit_op_rm(m, "lhu", ir->rd, ir->rs1, ir->imm.i);

    case RV_LWU:
        return emit_op_rm(m, "lwu", ir->rd, ir->rs1, ir->imm.i);

        // -------------- store

    case RV_SB:
        return emit_op_mr(m, "sb", ir->rs1, ir->imm.i, ir->rs2);

    case RV_SH:
        return emit_op_mr(m, "sh", ir->rs1, ir->imm.i, ir->rs2);

    case RV_SW:
        return emit_op_mr(m, "sw", ir->rs1, ir->imm.i, ir->rs2);

    case RV_SD:
        return emit_op_mr(m, "sd", ir->rs1, ir->imm.i, ir->rs2);

        // -------------- auipc, lui

    case RV_LUI:
        return emit_op_rx(m, "lui", ir->rd, ir->imm.i);

    case RV_AUIPC:
        return emit_op_rx(m, "auipc", ir->rd, ir->imm.i);

        // -------------- jump

    case RV_JAL:
        if (ir->rd == 0)
            return emit_op_x(m, "j", ir->imm.i + (rv_int64_t)ir->pc);
        if (ir->rd == 1)
            return emit_op_x(m, "jal", ir->imm.i + (rv_int64_t)ir->pc);
        return emit_op_rx(m, "jal", ir->rd, ir->imm.i + (rv_int64_t)ir->pc);

    case RV_JALR:
        if (ir->rd == 0 || ir->rs1 == 1 || ir->imm.i == 0)
            return emit_op(m, "ret", 0);
        if (ir->rd == 0)
            return emit_op_m(m, "jr", ir->rs1, ir->imm.i);
        if (ir->rd == 1)
            return emit_op_m(m, "jalr", ir->rs1, ir->imm.i);
        return emit_op_rm(m, "jalr", ir->rd, ir->rs1, ir->imm.i);

        // -------------- branch

    case RV_BEQ:
        if (ir->rs2 == 0)
            return emit_op_rx(m, "beqz", ir->rs1, ir->imm.i + (rv_int64_t)ir->pc);
        return emit_op_rrx(m, "beq", ir->rs1, ir->rs2, ir->imm.i + (rv_int64_t)ir->pc);

    case RV_BGE:
        if (ir->rs1 == 0)
            return emit_op_rx(m, "blez", ir->rs2, ir->imm.i + (rv_int64_t)ir->pc);
        if (ir->rs2 == 0)
            return emit_op_rx(m, "bgez", ir->rs1, ir->imm.i + (rv_int64_t)ir->pc);
        return emit_op_rrx(m, "bge", ir->rs1, ir->rs2, ir->imm.i + (rv_int64_t)ir->pc);

    case RV_BLT:
        if (ir->rs2 == 0)
            return emit_op_rx(m, "bltz", ir->rs1, ir->imm.i + (rv_int64_t)ir->pc);
        if (ir->rs1 == 0)
            return emit_op_rx(m, "bgtz", ir->rs2, ir->imm.i + (rv_int64_t)ir->pc);
        return emit_op_rrx(m, "blt", ir->rs1, ir->rs2, ir->imm.i + (rv_int64_t)ir->pc);

    case RV_BNE:
        if (ir->rs2 == 0)
            return emit_op_rx(m, "bnez", ir->rs1, ir->imm.i + (rv_int64_t)ir->pc);
        return emit_op_rrx(m, "bne", ir->rs1, ir->rs2, ir->imm.i + (rv_int64_t)ir->pc);

    case RV_BGEU:
        return emit_op_rrx(m, "bgeu", ir->rs1, ir->rs2, ir->imm.i + (rv_int64_t)ir->pc);

    case RV_BLTU:
        return emit_op_rrx(m, "bltu", ir->rs1, ir->rs2, ir->imm.i + (rv_int64_t)ir->pc);

        // -------------- op

    case RV_ADD:
        return emit_op_rrr(m, "add", ir->rd, ir->rs1, ir->rs2);

    case RV_SUB:
        if (ir->rs1 == 0)
            return emit_op_rr(m, "neg", ir->rd, ir->rs2);
        return emit_op_rrr(m, "sub", ir->rd, ir->rs1, ir->rs2);

    case RV_AND:
        return emit_op_rrr(m, "and", ir->rd, ir->rs1, ir->rs2);

    case RV_OR:
        return emit_op_rrr(m, "or", ir->rd, ir->rs1, ir->rs2);

    case RV_XOR:
        return emit_op_rrr(m, "xor", ir->rd, ir->rs1, ir->rs2);

    case RV_SLT:
        if (ir->rs2 == 0)
            return emit_op_rr(m, "sltz", ir->rd, ir->rs1);
        if (ir->rs1 == 0)
            return emit_op_rr(m, "sgtz", ir->rd, ir->rs2);
        return emit_op_rrr(m, "slt", ir->rd, ir->rs1, ir->rs2);

    case RV_SLTU:
        if (ir->rs1 == 0)
            return emit_op_rr(m, "snez", ir->rd, ir->rs2);
        return emit_op_rrr(m, "sltu", ir->rd, ir->rs1, ir->rs2);

    case RV_SLL:
        return emit_op_rrr(m, "sll", ir->rd, ir->rs1, ir->rs2);

    case RV_SRL:
        return emit_op_rrr(m, "srl", ir->rd, ir->rs1, ir->rs2);

    case RV_SRA:
        return emit_op_rrr(m, "sra", ir->rd, ir->rs1, ir->rs2);

        // -------------- mul exts

    case RV_MUL:
        return emit_op_rrr(m, "mul", ir->rd, ir->rs1, ir->rs2);

    case RV_MULH:
        return emit_op_rrr(m, "mulh", ir->rd, ir->rs1, ir->rs2);

    case RV_MULHSU:
        return emit_op_rrr(m, "mulhsu", ir->rd, ir->rs1, ir->rs2);

    case RV_MULHU:
        return emit_op_rrr(m, "mulhu", ir->rd, ir->rs1, ir->rs2);

    case RV_DIV:
        return emit_op_rrr(m, "div", ir->rd, ir->rs1, ir->rs2);

    case RV_DIVU:
        return emit_op_rrr(m, "divu", ir->rd, ir->rs1, ir->rs2);

    case RV_REM:
        return emit_op_rrr(m, "rem", ir->rd, ir->rs1, ir->rs2);

    case RV_REMU:
        return emit_op_rrr(m, "remu", ir->rd, ir->rs1, ir->rs2);

        // -------------- opimm

    case RV_ADDI:
        if (ir->rd == 0 && ir->rs1 == 0 && ir->imm.i == 0)
            return emit_op(m, "nop", 0);
        if (ir->rs1 == 0)
            return emit_op_ri(m, "li", ir->rd, ir->imm.i);
        if (ir->imm.i == 0)
            return emit_op_rr(m, "mv", ir->rd, ir->rs1);
        return emit_op_rri(m, "addi", ir->rd, ir->rs1, ir->imm.i);

    case RV_SLLI:
        return emit_op_rri(m, "slli", ir->rd, ir->rs1, ir->imm.i);

    case RV_SLTI:
        return emit_op_rri(m, "slti", ir->rd, ir->rs1, ir->imm.i);

    case RV_SLTIU:
        if (ir->imm.i == 1)
            return emit_op_rr(m, "seqz", ir->rd, ir->rs1);
        return emit_op_rri(m, "sltiu", ir->rd, ir->rs1, ir->imm.i);

    case RV_XORI:
        if (ir->imm.i == -1)
            return emit_op_rr(m, "not", ir->rd, ir->rs1);
        return emit_op_rri(m, "xori", ir->rd, ir->rs1, ir->imm.i);

    case RV_SRLI:
        return emit_op_rri(m, "srli", ir->rd, ir->rs1, ir->imm.i);

    case RV_SRAI:
        return emit_op_rri(m, "srai", ir->rd, ir->rs1, ir->imm.i);

    case RV_ORI:
        return emit_op_rri(m, "ori", ir->rd, ir->rs1, ir->imm.i);

    case RV_ANDI:
        return emit_op_rri(m, "andi", ir->rd, ir->rs1, ir->imm.i);

        // -------------- csr

    case RV_CSRRC:
        if (ir->rd == 0)
            return emit_op_cr(m, "csrc", ir->imm.u, ir->rs1);
        return emit_op_rcr(m, "csrrc", ir->rd, ir->imm.u, ir->rs1);

    case RV_CSRRS:
        if (ir->rs1 == 0)
            return emit_op_rc(m, "csrr", ir->rd, ir->imm.u);
        if (ir->rd == 0)
            return emit_op_cr(m, "csrs", ir->imm.u, ir->rs1);
        return emit_op_rcr(m, "csrrs", ir->rd, ir->imm.u, ir->rs1);

    case RV_CSRRW:
        if (ir->rd == 0)
            return emit_op_cr(m, "csrw", ir->imm.u, ir->rs1);
        return emit_op_rcr(m, "csrrw", ir->rd, ir->imm.u, ir->rs1);

    case RV_CSRRCI:
        return emit_op_rcu(m, "csrrci", ir->rd, ir->imm.u, ir->uimm);

    case RV_CSRRSI:
        return emit_op_rcu(m, "csrrsi", ir->rd, ir->imm.u, ir->uimm);

    case RV_CSRRWI:
        return emit_op_rcu(m, "csrrwi", ir->rd, ir->imm.u, ir->uimm);

        // -------------- 64 bits

    case RV_ADDW:
        return emit_op_rrr(m, "addw", ir->rd, ir->rs1, ir->rs2);

    case RV_SUBW:
        if (ir->rs1 == 0)
            return emit_op_rr(m, "negw", ir->rd, ir->rs2);
        return emit_op_rrr(m, "subw", ir->rd, ir->rs1, ir->rs2);

    case RV_SLLW:
        return emit_op_rrr(m, "sllw", ir->rd, ir->rs1, ir->rs2);

    case RV_SRAW:
        return emit_op_rrr(m, "sraw", ir->rd, ir->rs1, ir->rs2);

    case RV_SRLW:
        return emit_op_rrr(m, "srlw", ir->rd, ir->rs1, ir->rs2);

    case RV_ADDIW:
        if (ir->imm.i == 0)
            return emit_op_rr(m, "sext.w", ir->rd, ir->rs1);
        return emit_op_rri(m, "addiw", ir->rd, ir->rs1, ir->imm.i);

    case RV_SLLIW:
        return emit_op_rri(m, "slliw", ir->rd, ir->rs1, ir->imm.i);

    case RV_SRAIW:
        return emit_op_rri(m, "sraiw", ir->rd, ir->rs1, ir->imm.i);

    case RV_SRLIW:
        return emit_op_rri(m, "srliw", ir->rd, ir->rs1, ir->imm.i);

        // -------------- compressed

    case RV_C_UNIMP:
        return emit_op(m, "c.undef", 0);

    case RV_C_ADDI4SPN:
        // emit_op_rri(m, "c.addi", ir->rd, "sp", ir->imm.i);
        return emit_op_ri(m, "c.addi4spn", ir->rd, ir->imm.i);

    case RV_C_FLD:
        return emit_op_fm(m, "c.fld", ir->rd, ir->rs1, ir->imm.i);

    case RV_C_LQ:
        return emit_op_rm(m, "c.lq", ir->rd, ir->rs1, ir->imm.i);

    case RV_C_LW:
        return emit_op_rm(m, "c.lw", ir->rd, ir->rs1, ir->imm.i);

    case RV_C_FLW:
        return emit_op_fm(m, "c.flw", ir->rd, ir->rs1, ir->imm.i);

    case RV_C_LD:
        return emit_op_rm(m, "c.ld", ir->rd, ir->rs1, ir->imm.i);

    case RV_C_FSD:
        return emit_op_fm(m, "c.fsd", ir->rs2, ir->rs1, ir->imm.i);

    case RV_C_SQ:
        return emit_op_rm(m, "c.sq", ir->rs2, ir->rs1, ir->imm.i);

    case RV_C_SW:
        return emit_op_rm(m, "c.sw", ir->rs2, ir->rs1, ir->imm.i);

    case RV_C_FSW:
        return emit_op_fm(m, "c.fsw", ir->rs2, ir->rs1, ir->imm.i);

    case RV_C_SD:
        return emit_op_rm(m, "c.sd", ir->rs2, ir->rs1, ir->imm.i);

    case RV_C_NOP:
        if (ir->imm.u == 0)
            return emit_op(m, "c.nop", 0);
        return emit_op_i(m, "c.nop", ir->imm.i);

    case RV_C_ADDI:
        return emit_op_rri(m, "c.addi", ir->rd, ir->rd, ir->imm.i);

    case RV_C_JAL:
        return emit_op_x(m, "c_jal", ir->imm.i + (rv_int64_t)ir->pc);

    case RV_C_ADDIW:
        return emit_op(m, "c_addiw", 0); // TODO

    case RV_C_LI:
        return emit_op_ri(m, "c_li", ir->rd, ir->imm.i);

    case RV_C_ADDI16SP:
        return emit_op(m, "c_addi16sp", 0);

    case RV_C_LUI:
        return emit_op(m, "c_lui", 0);

    case RV_C_SRLI:
        return emit_op(m, "c_srli", 0);

    case RV_C_SRLI64:
        return emit_op(m, "c_srli64", 0);

    case RV_C_SRAI:
        return emit_op(m, "c_srai", 0);

    case RV_C_SRAI64:
        return emit_op(m, "c_srai64", 0);

    case RV_C_ANDI:
        return emit_op(m, "c_andi", 0);

    case RV_C_SUB:
        return emit_op(m, "c_sub", 0);

    case RV_C_XOR:
        return emit_op(m, "c_xor", 0);

    case RV_C_OR:
        return emit_op(m, "c_or", 0);

    case RV_C_AND:
        return emit_op(m, "c_and", 0);

    case RV_C_SUBW:
        return emit_op(m, "c_subw", 0);

    case RV_C_ADDW:
        return emit_op(m, "c_addw", 0);

    case RV_C_J:
        return emit_op(m, "c_j", 0);

    case RV_C_BEQZ:
        return emit_op(m, "c_beqz", 0);

    case RV_C_BNEZ:
        return emit_op(m, "c_bnez", 0);

    case RV_C_SLLI:
        return emit_op(m, "c_slli", 0);

    case RV_C_SLLI64:
        return emit_op(m, "c_slli64", 0);

    case RV_C_FLDSP:
        return emit_op(m, "c_fldsp", 0);

    case RV_C_LQSP:
        return emit_op(m, "c_lqsp", 0);

    case RV_C_LWSP:
        return emit_op(m, "c_lwsp", 0);

    case RV_C_FLWSP:
        return emit_op(m, "c_flwsp", 0);

    case RV_C_LDSP:
        return emit_op(m, "c_ldsp", 0);

    case RV_C_JR:
        return emit_op(m, "c_jr", 0);

    case RV_C_MV:
        return emit_op(m, "c_mv", 0);

    case RV_C_EBREAK:
        return emit_op(m, "c_ebreak", 0);

    case RV_C_JALR:
        return emit_op(m, "c_jalr", 0);

    case RV_C_ADD:
        return emit_op(m, "c_add", 0);

    case RV_C_FSDSP:
        return emit_op(m, "c_fsdsp", 0);

    case RV_C_SQSP:
        return emit_op(m, "c_sqsp", 0);

    case RV_C_SWSP:
        return emit_op(m, "c_swsp", 0);

    case RV_C_FSWSP:
        return emit_op(m, "c_fswsp", 0);

    case RV_C_SDSP:
        return emit_op(m, "c_sdsp", 0);

        // -------------- Atomic/LR-SC

    case RV_LR_W:
        return emit_lr(m, "lr.w", ir->rd, ir->rs1, ir->mod);

    case RV_LR_D:
        return emit_lr(m, "lr.d", ir->rd, ir->rs1, ir->mod);

    case RV_SC_W:
        return emit_amo(m, "sc.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_SC_D:
        return emit_amo(m, "sc.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

        // -------------- Atomic/AMO

    case RV_AMOADD_W:
        return emit_amo(m, "amoadd.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOAND_W:
        return emit_amo(m, "amoand.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOMAXU_W:
        return emit_amo(m, "amomaxu.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOMAX_W:
        return emit_amo(m, "amomax.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOMINU_W:
        return emit_amo(m, "amominu.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOMIN_W:
        return emit_amo(m, "amomin.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOOR_W:
        return emit_amo(m, "amoor.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOSWAP_W:
        return emit_amo(m, "amoswap.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOXOR_W:
        return emit_amo(m, "amoxor.w", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOADD_D:
        return emit_amo(m, "amoadd.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOAND_D:
        return emit_amo(m, "amoand.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOMAX_D:
        return emit_amo(m, "amomax.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOMAXU_D:
        return emit_amo(m, "amomaxu.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOMIN_D:
        return emit_amo(m, "amomin.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOMINU_D:
        return emit_amo(m, "amominu.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOOR_D:
        return emit_amo(m, "amoor.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOSWAP_D:
        return emit_amo(m, "amoswap.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_AMOXOR_D:
        return emit_amo(m, "amoxor.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

        // system

    case RV_FENCE:
        return emit_op_fff(m, "fence", ir->rd, ir->rs1, ir->rs2);

    case RV_FENCE_I:
        return emit_op_fff(m, "fence.i", ir->rd, ir->rs1, ir->rs2);

    case RV_EBREAK:
        return emit_op(m, "ebreak", 0);

    case RV_ECALL:
        return emit_op(m, "ecall", 0);

    case RV_MRET:
        return emit_op(m, "mret", 0);

    case RV_SFENCE_VMA:
        return emit_op(m, "sfence.vma", 0);

    case RV_SRET:
        return emit_op(m, "sret", 0);

    case RV_URET:
        return emit_op(m, "uret", 0);

    case RV_WFI:
        return emit_op(m, "wfi", 0);

    case RV_HFENCE_BVMA:
        return emit_op(m, "hfence.bvma", 0);

    case RV_HFENCE_GVMA:
        return emit_op(m, "hfence.gvma", 0);

    case RV_DRET:
        return emit_op(m, "dret", 0);

        // float memory

    case RV_FLH:
        return emit_op_fm(m, "flh", ir->rd, ir->rs1, ir->imm.i);

    case RV_FLW:
        return emit_op_fm(m, "flw", ir->rd, ir->rs1, ir->imm.i);

    case RV_FLD:
        return emit_op_fm(m, "fld", ir->rd, ir->rs1, ir->imm.i);

    case RV_FLQ:
        return emit_op_fm(m, "flq", ir->rd, ir->rs1, ir->imm.i);

    case RV_FSH:
        return emit_op_mf(m, "fsh", ir->rs1, ir->imm.i, ir->rs2);

    case RV_FSW:
        return emit_op_mf(m, "fsw", ir->rs1, ir->imm.i, ir->rs2);

    case RV_FSD:
        return emit_op_mf(m, "fsd", ir->rs1, ir->imm.i, ir->rs2);

    case RV_FSQ:
        return emit_op_mf(m, "fsq", ir->rs1, ir->imm.i, ir->rs2);

        // fma

    case RV_FMADD_S:
        return emit_op_ffff_rnd(m, "fmadd.s", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FMADD_D:
        return emit_op_ffff_rnd(m, "fmadd.d", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FMADD_Q:
        return emit_op_ffff_rnd(m, "fmadd.q", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FMSUB_S:
        return emit_op_ffff_rnd(m, "fmsub.s", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FMSUB_D:
        return emit_op_ffff_rnd(m, "fmsub.d", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FMSUB_Q:
        return emit_op_ffff_rnd(m, "fmsub.q", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FNMSUB_S:
        return emit_op_ffff_rnd(m, "fnmsub.s", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FNMSUB_D:
        return emit_op_ffff_rnd(m, "fnmsub.d", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FNMSUB_Q:
        return emit_op_ffff_rnd(m, "fnmsub.q", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FNMADD_S:
        return emit_op_ffff_rnd(m, "fnmadd.s", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FNMADD_D:
        return emit_op_ffff_rnd(m, "fnmadd.d", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

    case RV_FNMADD_Q:
        return emit_op_ffff_rnd(m, "fnmadd.q", ir->rd, ir->rs1, ir->rs2, ir->rs3, ir->mod);

        // float

    case RV_FADD_S:
        return emit_op_fff_rnd(m, "fadd.s", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FADD_D:
        return emit_op_fff_rnd(m, "fadd.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FADD_H:
        return emit_op_fff_rnd(m, "fadd.h", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FADD_Q:
        return emit_op_fff_rnd(m, "fadd.q", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FSUB_S:
        return emit_op_fff_rnd(m, "fsub.s", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FSUB_D:
        return emit_op_fff_rnd(m, "fsub.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FSUB_H:
        return emit_op_fff_rnd(m, "fsub.h", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FSUB_Q:
        return emit_op_fff_rnd(m, "fsub.q", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMUL_S:
        return emit_op_fff_rnd(m, "fmul.s", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMUL_D:
        return emit_op_fff_rnd(m, "fmul.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMUL_H:
        return emit_op_fff_rnd(m, "fmul.h", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMUL_Q:
        return emit_op_fff_rnd(m, "fmul.q", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FDIV_S:
        return emit_op_fff_rnd(m, "fdiv.s", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FDIV_D:
        return emit_op_fff_rnd(m, "fdiv.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FDIV_H:
        return emit_op_fff_rnd(m, "fdiv.h", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FDIV_Q:
        return emit_op_fff_rnd(m, "fdiv.q", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FSQRT_S:
        return emit_op_fff_rnd(m, "fsqrt.s", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FSQRT_D:
        return emit_op_fff_rnd(m, "fsqrt.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FSQRT_H:
        return emit_op_fff_rnd(m, "fsqrt.h", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FSQRT_Q:
        return emit_op_fff_rnd(m, "fsqrt.q", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMIN_S:
        return emit_op_fff_rnd(m, "fmin.s", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMIN_D:
        return emit_op_fff_rnd(m, "fmin.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMIN_H:
        return emit_op_fff_rnd(m, "fmin.h", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMIN_Q:
        return emit_op_fff_rnd(m, "fmin.q", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMAX_S:
        return emit_op_fff_rnd(m, "fmax.s", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMAX_D:
        return emit_op_fff_rnd(m, "fmax.d", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMAX_H:
        return emit_op_fff_rnd(m, "fmax.h", ir->rd, ir->rs1, ir->rs2, ir->mod);

    case RV_FMAX_Q:
        return emit_op_fff_rnd(m, "fmax.q", ir->rd, ir->rs1, ir->rs2, ir->mod);

        // fclass

    case RV_FCLASS_S:
        return emit_op_ff(m, "fclass.s", ir->rd, ir->rs1);

    case RV_FCLASS_D:
        return emit_op_ff(m, "fclass.d", ir->rd, ir->rs1);

    case RV_FCLASS_H:
        return emit_op_ff(m, "fclass.h", ir->rd, ir->rs1);

    case RV_FCLASS_Q:
        return emit_op_ff(m, "fclass.q", ir->rd, ir->rs1);

        // fsgn

    case RV_FSGNJ_S:
        return emit_op_fff(m, "fsgnj.s", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJ_D:
        return emit_op_fff(m, "fsgnj.d", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJ_H:
        return emit_op_fff(m, "fsgnj.h", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJ_Q:
        return emit_op_fff(m, "fsgnj.q", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJN_S:
        return emit_op_fff(m, "fsgnjn.s", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJN_D:
        return emit_op_fff(m, "fsgnjn.d", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJN_H:
        return emit_op_fff(m, "fsgnjn.h", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJN_Q:
        return emit_op_fff(m, "fsgnjn.q", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJX_S:
        return emit_op_fff(m, "fsgnjx.s", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJX_D:
        return emit_op_fff(m, "fsgnjx.d", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJX_H:
        return emit_op_fff(m, "fsgnjx.h", ir->rd, ir->rs1, ir->rs2);

    case RV_FSGNJX_Q:
        return emit_op_fff(m, "fsgnjx.q", ir->rd, ir->rs1, ir->rs2);

        // fcvt float,float

    case RV_FCVT_S_D:
        return emit_op_ff_rnd(m, "fcvt.s.d", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_S_H:
        return emit_op_ff_rnd(m, "fcvt.s.h", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_S_Q:
        return emit_op_ff_rnd(m, "fcvt.s.q", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_D_S:
        return emit_op_ff_rnd(m, "fcvt.d.s", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_D_H:
        return emit_op_ff_rnd(m, "fcvt.d.h", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_D_Q:
        return emit_op_ff_rnd(m, "fcvt.d.q", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_H_S:
        return emit_op_ff_rnd(m, "fcvt.h.s", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_H_D:
        return emit_op_ff_rnd(m, "fcvt.h.d", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_H_Q:
        return emit_op_ff_rnd(m, "fcvt.h.q", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_Q_S:
        return emit_op_ff_rnd(m, "fcvt.q.s", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_Q_D:
        return emit_op_ff_rnd(m, "fcvt.q.d", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_Q_H:
        return emit_op_ff_rnd(m, "fcvt.q.h", ir->rd, ir->rs1, ir->mod);

        // fcvt int,fmt

    case RV_FCVT_W_S:
        return emit_op_rf_rnd(m, "fcvt.w.s", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_WU_S:
        return emit_op_rf_rnd(m, "fcvt.wu.s", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_L_S:
        return emit_op_rf_rnd(m, "fcvt.l.s", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_LU_S:
        return emit_op_rf_rnd(m, "fcvt.lu.s", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_W_D:
        return emit_op_rf_rnd(m, "fcvt.w.d", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_WU_D:
        return emit_op_rf_rnd(m, "fcvt.wu.d", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_L_D:
        return emit_op_rf_rnd(m, "fcvt.l.d", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_LU_D:
        return emit_op_rf_rnd(m, "fcvt.lu.d", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_W_H:
        return emit_op_rf_rnd(m, "fcvt.w.h", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_WU_H:
        return emit_op_rf_rnd(m, "fcvt.wu.h", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_L_H:
        return emit_op_rf_rnd(m, "fcvt.l.h", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_LU_H:
        return emit_op_rf_rnd(m, "fcvt.lu.h", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_W_Q:
        return emit_op_rf_rnd(m, "fcvt.w.q", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_WU_Q:
        return emit_op_rf_rnd(m, "fcvt.wu.q", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_L_Q:
        return emit_op_rf_rnd(m, "fcvt.l.q", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_LU_Q:
        return emit_op_rf_rnd(m, "fcvt.lu.q", ir->rd, ir->rs1, ir->mod);

        // fcvt fmt,int

    case RV_FCVT_S_W:
        return emit_op_fr_rnd(m, "fcvt.s.w", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_S_WU:
        return emit_op_fr_rnd(m, "fcvt.s.wu", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_S_L:
        return emit_op_fr_rnd(m, "fcvt.s.l", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_S_LU:
        return emit_op_fr_rnd(m, "fcvt.s.lu", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_D_W:
        return emit_op_fr_rnd(m, "fcvt.d.w", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_D_WU:
        return emit_op_fr_rnd(m, "fcvt.d.wu", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_D_L:
        return emit_op_fr_rnd(m, "fcvt.d.l", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_D_LU:
        return emit_op_fr_rnd(m, "fcvt.d.lu", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_H_W:
        return emit_op_fr_rnd(m, "fcvt.h.w", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_H_WU:
        return emit_op_fr_rnd(m, "fcvt.h.wu", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_H_L:
        return emit_op_fr_rnd(m, "fcvt.h.l", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_H_LU:
        return emit_op_fr_rnd(m, "fcvt.h.lu", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_Q_W:
        return emit_op_fr_rnd(m, "fcvt.q.w", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_Q_WU:
        return emit_op_fr_rnd(m, "fcvt.q.wu", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_Q_L:
        return emit_op_fr_rnd(m, "fcvt.q.l", ir->rd, ir->rs1, ir->mod);

    case RV_FCVT_Q_LU:
        return emit_op_fr_rnd(m, "fcvt.q.lu", ir->rd, ir->rs1, ir->mod);

        // fmv int,float

    case RV_FMV_X_W:
        return emit_op_fr(m, "fmv.x.w", ir->rd, ir->rs1);

    case RV_FMV_X_D:
        return emit_op_fr(m, "fmv.x.d", ir->rd, ir->rs1);

    case RV_FMV_X_H:
        return emit_op_fr(m, "fmv.x.h", ir->rd, ir->rs1);

        // fmv float,int

    case RV_FMV_W_X:
        return emit_op_rf(m, "fmv.w.x", ir->rd, ir->rs1);

    case RV_FMV_D_X:
        return emit_op_rf(m, "fmv.d.x", ir->rd, ir->rs1);

    case RV_FMV_H_X:
        return emit_op_rf(m, "fmv.h.x", ir->rd, ir->rs1);

        // fcmp

    case RV_FLE_S:
        return emit_op_rff(m, "fle.s", ir->rd, ir->rs1, ir->rs2);

    case RV_FLE_D:
        return emit_op_rff(m, "fle.d", ir->rd, ir->rs1, ir->rs2);

    case RV_FLE_H:
        return emit_op_rff(m, "fle.h", ir->rd, ir->rs1, ir->rs2);

    case RV_FLE_Q:
        return emit_op_rff(m, "fle.q", ir->rd, ir->rs1, ir->rs2);

    case RV_FLT_S:
        return emit_op_rff(m, "flt.s", ir->rd, ir->rs1, ir->rs2);

    case RV_FLT_D:
        return emit_op_rff(m, "flt.d", ir->rd, ir->rs1, ir->rs2);

    case RV_FLT_H:
        return emit_op_rff(m, "flt.h", ir->rd, ir->rs1, ir->rs2);

    case RV_FLT_Q:
        return emit_op_rff(m, "flt.q", ir->rd, ir->rs1, ir->rs2);

    case RV_FEQ_S:
        return emit_op_rff(m, "feq.s", ir->rd, ir->rs1, ir->rs2);

    case RV_FEQ_D:
        return emit_op_rff(m, "feq.d", ir->rd, ir->rs1, ir->rs2);

    case RV_FEQ_H:
        return emit_op_rff(m, "feq.h", ir->rd, ir->rs1, ir->rs2);

    case RV_FEQ_Q:
        return emit_op_rff(m, "feq.q", ir->rd, ir->rs1, ir->rs2);

        // --------------------- bit

    case RV_BMATFLIP:
        // ['rd', 'rs1']
        break;

    case RV_CRC32_D:
        // ['rd', 'rs1']
        break;

    case RV_CRC32C_D:
        // ['rd', 'rs1']
        break;

    case RV_BMATOR:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_BMATXOR:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SLLI_UW:
        // ['rd', 'rs1', 'shamt']
        break;

    case RV_ADD_UW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SLOW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SROW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_ROLW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_RORW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SBCLRW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SBSETW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SBINVW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SBEXTW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_GORCW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_GREVW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SLOIW:
        // ['rd', 'rs1', 'shamtw']
        break;

    case RV_SROIW:
        // ['rd', 'rs1', 'shamtw']
        break;

    case RV_RORIW:
        // ['rd', 'rs1', 'shamtw']
        break;

    case RV_SBCLRIW:
        // ['rd', 'rs1', 'shamtw']
        break;

    case RV_SBSETIW:
        // ['rd', 'rs1', 'shamtw']
        break;

    case RV_SBINVIW:
        // ['rd', 'rs1', 'shamtw']
        break;

    case RV_GORCIW:
        // ['rd', 'rs1', 'shamtw']
        break;

    case RV_GREVIW:
        // ['rd', 'rs1', 'shamtw']
        break;

    case RV_FSLW:
        // ['rd', 'rs1', 'rs2', 'rs3']
        break;

    case RV_FSRW:
        // ['rd', 'rs1', 'rs2', 'rs3']
        break;

    case RV_FSRIW:
        // ['rd', 'rs1', 'shamtw', 'rs3']
        break;

    case RV_CLZW:
        // ['rd', 'rs1']
        break;

    case RV_CTZW:
        // ['rd', 'rs1']
        break;

    case RV_CPOPW:
        // ['rd', 'rs1']
        break;

    case RV_SH1ADD_UW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SH2ADD_UW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SH3ADD_UW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_SHFLW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_UNSHFLW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_BCOMPRESSW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_BDECOMPRESSW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_PACKW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_PACKUW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_BFPW:
        // ['rd', 'rs1', 'rs2']
        break;

    case RV_XPERM_W:
        // ['rd', 'rs1', 'rs2']
        break;

        // --------------------- vector: TODO
    }

    return emit_op(m, "undef", 0);
}
