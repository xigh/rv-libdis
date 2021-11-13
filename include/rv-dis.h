#ifndef __RV_DIS_H
#define __RV_DIS_H

#include "rv-dec.h"

typedef struct
{
    char *op;
    char *args[5];
} mnemonic_t;

extern int rv_disasm(inst_t *ir, mnemonic_t *m);
extern const char *rv_reg2name(rv_uint8_t reg);
extern const char *rv_freg2name(rv_uint8_t reg);
extern const char *rv_csr2name(rv_uint64_t csr);

#endif // __RV_DIS_H
