#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "mix_computer.h"
#include "mix_instructions.h"

int get_from_range(int range) {
    return range / 8;
}

int get_to_range(int range) {
    return range % 8;
}

void convert_int_to_range(int range, mix_range *str_range) {
    int from, dest;

    from = get_from_range(range);
    dest = get_to_range(range);

    snprintf(str_range->range, 6,"(%d:%d)",from,dest);
}

unsigned int convert_range_to_int(mix_range *str_range) {
    char *token;
    token = strtok(str_range->range, "(:)");

    unsigned int range = 0;

    range = atoi(token) * 8;
    token = strtok(nullptr, "(:)");
    if (token != nullptr) {
        range += atoi(token);
    }

    return range;
}

mix_instruction parse_instruction_code(const char *function) {
    int opcode = -1;
    unsigned int mod = 0;

    mix_instruction instruction;

    if (strcmp(function, "LDA") == 0) {
        opcode = LDA;
    } else if(strcmp(function, "LD1") == 0) {
        opcode = LD1;
    } else if(strcmp(function, "LD2") == 0) {
        opcode = LD2;
    } else if(strcmp(function, "LD3") == 0) {
        opcode = LD3;
    } else if(strcmp(function, "LD4") == 0) {
        opcode = LD4;
    } else if(strcmp(function, "LD5") == 0) {
        opcode = LD5;
    } else if(strcmp(function, "LD6") == 0) {
        opcode = LD6;
    } else if(strcmp(function, "LDX") == 0) {
        opcode = LDX;
    } else if(strcmp(function, "LDAN") == 0) {
        opcode = LDAN;
    } else if(strcmp(function, "LD1N") == 0) {
        opcode = LD1N;
    } else if(strcmp(function, "LD2N") == 0) {
        opcode = LD2N;
    } else if(strcmp(function, "LD3N") == 0) {
        opcode = LD3N;
    } else if(strcmp(function, "LD4N") == 0) {
        opcode = LD4N;
    } else if(strcmp(function, "LD5N") == 0) {
        opcode = LD5N;
    } else if(strcmp(function, "LD6N") == 0) {
        opcode = LD6N;
    } else if(strcmp(function, "LDXN") == 0) {
        opcode = LDXN;
    } else if(strcmp(function, "ADD") == 0) {
        opcode = ADD;
    } else if(strcmp(function, "SUB") == 0) {
        opcode = SUB;
    } else if(strcmp(function, "MUL") == 0) {
        opcode = MUL;
    } else if(strcmp(function, "DIV") == 0) {
        opcode = DIV;
    } else if(strcmp(function, "STA") == 0) {
        opcode = STA;
    } else if(strcmp(function, "STX") == 0) {
        opcode = STX;
    } else if(strcmp(function, "ST1") == 0) {
        opcode = ST1;
    } else if(strcmp(function, "ST2") == 0) {
        opcode = ST2;
    } else if(strcmp(function, "ST3") == 0) {
        opcode = ST3;
    } else if(strcmp(function, "ST4") == 0) {
        opcode = ST4;
    } else if(strcmp(function, "ST5") == 0) {
        opcode = ST5;
    } else if(strcmp(function, "ST6") == 0) {
        opcode = ST6;
    } else if(strcmp(function, "ENTA") == 0) {
        opcode = ENTA;
        mod = 2;
    } else if(strcmp(function, "ENTX") == 0) {
        opcode = ENTX;
        mod = 2;
    } else if(strcmp(function, "ENT1") == 0) {
        opcode = ENT1;
        mod = 2;
    } else if(strcmp(function, "ENT2") == 0) {
        opcode = ENT2;
        mod = 2;
    } else if(strcmp(function, "ENT3") == 0) {
        opcode = ENT3;
        mod = 2;
    } else if(strcmp(function, "ENT4") == 0) {
        opcode = ENT4;
        mod = 2;
    } else if(strcmp(function, "ENT5") == 0) {
        opcode = ENT5;
        mod = 2;
    } else if(strcmp(function, "ENT6") == 0) {
        opcode = ENT6;
        mod = 2;
    } else if(strcmp(function, "ENNA") == 0) {
        opcode = ENNA;
        mod = 3;
    } else if(strcmp(function, "ENNX") == 0) {
        opcode = ENNX;
        mod = 3;
    } else if(strcmp(function, "ENN1") == 0) {
        opcode = ENN1;
        mod = 3;
    } else if(strcmp(function, "ENN2") == 0) {
        opcode = ENN2;
        mod = 3;
    } else if(strcmp(function, "ENN3") == 0) {
        opcode = ENN3;
        mod = 3;
    } else if(strcmp(function, "ENN4") == 0) {
        opcode = ENN4;
        mod = 3;
    } else if(strcmp(function, "ENN5") == 0) {
        opcode = ENN5;
        mod = 3;
    } else if(strcmp(function, "ENN6") == 0) {
        opcode = ENN6;
        mod = 3;
    } else if(strcmp(function, "INCA") == 0) {
        opcode = INCA;
        mod = 0;
    } else if(strcmp(function, "INCX") == 0) {
        opcode = INCX;
        mod = 0;
    } else if(strcmp(function, "INC1") == 0) {
        opcode = INC1;
        mod = 0;
    } else if(strcmp(function, "INC2") == 0) {
        opcode = INC2;
        mod = 0;
    } else if(strcmp(function, "INC3") == 0) {
        opcode = INC3;
        mod = 0;
    } else if(strcmp(function, "INC4") == 0) {
        opcode = INC4;
        mod = 0;
    } else if(strcmp(function, "INC5") == 0) {
        opcode = INC5;
        mod = 0;
    } else if(strcmp(function, "INC6") == 0) {
        opcode = INC6;
        mod = 0;
    } else if(strcmp(function, "DECA") == 0) {
        opcode = DECA;
        mod = 1;
    } else if(strcmp(function, "DECX") == 0) {
        opcode = DECX;
        mod = 1;
    } else if(strcmp(function, "DEC1") == 0) {
        opcode = DEC1;
        mod = 1;
    } else if(strcmp(function, "DEC2") == 0) {
        opcode = DEC2;
        mod = 1;
    } else if(strcmp(function, "DEC3") == 0) {
        opcode = DEC3;
        mod = 1;
    } else if(strcmp(function, "DEC4") == 0) {
        opcode = DEC4;
        mod = 1;
    } else if(strcmp(function, "DEC5") == 0) {
        opcode = DEC5;
        mod = 1;
    } else if(strcmp(function, "DEC6") == 0) {
        opcode = DEC6;
        mod = 1;
    } else if(strcmp(function, "CMPA") == 0) {
        opcode = CMPA;
    } else if(strcmp(function, "CMP1") == 0) {
        opcode = CMP1;
    } else if(strcmp(function, "CMP2") == 0) {
        opcode = CMP2;
    } else if(strcmp(function, "CMP3") == 0) {
        opcode = CMP3;
    } else if(strcmp(function, "CMP4") == 0) {
        opcode = CMP4;
    } else if(strcmp(function, "CMP5") == 0) {
        opcode = CMP5;
    } else if(strcmp(function, "CMP6") == 0) {
        opcode = CMP6;
    } else if(strcmp(function, "CMPX") == 0) {
        opcode = CMPX;
    } else if(strcmp(function, "JMP") == 0) {
        opcode = JMP;
        mod = 0;
    } else if(strcmp(function, "JSJ") == 0) {
        opcode = JSJ;
        mod = 1;
    } else if(strcmp(function, "JOV") == 0) {
        opcode = JOV;
        mod = 2;
    } else if(strcmp(function, "JNOV") == 0) {
        opcode = JNOV;
        mod = 3;
    } else if(strcmp(function, "JL") == 0) {
        opcode = JL;
        mod = 4;
    } else if(strcmp(function, "JE") == 0) {
        opcode = JE;
        mod = 5;
    } else if(strcmp(function, "JG") == 0) {
        opcode = JG;
        mod = 6;
    } else if(strcmp(function, "JGE") == 0) {
        opcode = JGE;
        mod = 7;
    } else if(strcmp(function, "JNE") == 0) {
        opcode = JNE;
        mod = 8;
    } else if(strcmp(function, "JLE") == 0) {
        opcode = JLE;
        mod = 9;
    } else if(strcmp(function, "JAN") == 0) {
        opcode = JAN;
        mod = 0;
    } else if(strcmp(function, "JAZ") == 0) {
        opcode = JAZ;
        mod = 1;
    } else if(strcmp(function, "JAP") == 0) {
        opcode = JAP;
        mod = 2;
    } else if(strcmp(function, "JANN") == 0) {
        opcode = JANN;
        mod = 3;
    } else if(strcmp(function, "JANZ") == 0) {
        opcode = JANZ;
        mod = 4;
    } else if(strcmp(function, "JANP") == 0) {
        opcode = JANP;
        mod = 5;
    } else if(strcmp(function, "JAE") == 0) {
        opcode = JAE;
        mod = 6;
    } else if(strcmp(function, "JAO") == 0) {
        opcode = JAO;
        mod = 7;
    } else if(strcmp(function, "JXN") == 0) {
        opcode = JXN;
        mod = 0;
    } else if(strcmp(function, "JXZ") == 0) {
        opcode = JXZ;
        mod = 1;
    } else if(strcmp(function, "JXP") == 0) {
        opcode = JXP;
        mod = 2;
    } else if(strcmp(function, "JXNN") == 0) {
        opcode = JXNN;
        mod = 3;
    } else if(strcmp(function, "JXNZ") == 0) {
        opcode = JXNZ;
        mod = 4;
    } else if(strcmp(function, "JXNP") == 0) {
        opcode = JXNP;
        mod = 5;
    } else if(strcmp(function, "JXE") == 0) {
        opcode = JXE;
        mod = 6;
    } else if(strcmp(function, "JXO") == 0) {
        opcode = JXO;
        mod = 7;
    } else if(strcmp(function, "J1N") == 0) {
        opcode = J1N;
        mod = 0;
    } else if(strcmp(function, "J1Z") == 0) {
        opcode = J1Z;
        mod = 1;
    } else if(strcmp(function, "J1P") == 0) {
        opcode = J1P;
        mod = 2;
    } else if(strcmp(function, "J1NN") == 0) {
        opcode = J1NN;
        mod = 3;
    } else if(strcmp(function, "J1NZ") == 0) {
        opcode = J1NZ;
        mod = 4;
    } else if(strcmp(function, "J1NP") == 0) {
        opcode = J1NP;
        mod = 5;
    } else if(strcmp(function, "J2N") == 0) {
        opcode = J2N;
        mod = 0;
    } else if(strcmp(function, "J2Z") == 0) {
        opcode = J2Z;
        mod = 1;
    } else if(strcmp(function, "J2P") == 0) {
        opcode = J2P;
        mod = 2;
    } else if(strcmp(function, "J2NN") == 0) {
        opcode = J2NN;
        mod = 3;
    } else if(strcmp(function, "J2NZ") == 0) {
        opcode = J2NZ;
        mod = 4;
    } else if(strcmp(function, "J2NP") == 0) {
        opcode = J2NP;
        mod = 5;
    } else if(strcmp(function, "J3N") == 0) {
        opcode = J3N;
        mod = 0;
    } else if(strcmp(function, "J3Z") == 0) {
        opcode = J3Z;
        mod = 1;
    } else if(strcmp(function, "J3P") == 0) {
        opcode = J3P;
        mod = 2;
    } else if(strcmp(function, "J3NN") == 0) {
        opcode = J3NN;
        mod = 3;
    } else if(strcmp(function, "J3NZ") == 0) {
        opcode = J3NZ;
        mod = 4;
    } else if(strcmp(function, "J3NP") == 0) {
        opcode = J3NP;
        mod = 5;
    } else if(strcmp(function, "J4N") == 0) {
        opcode = J4N;
        mod = 0;
    } else if(strcmp(function, "J4Z") == 0) {
        opcode = J4Z;
        mod = 1;
    } else if(strcmp(function, "J4P") == 0) {
        opcode = J4P;
        mod = 2;
    } else if(strcmp(function, "J4NN") == 0) {
        opcode = J4NN;
        mod = 3;
    } else if(strcmp(function, "J4NZ") == 0) {
        opcode = J4NZ;
        mod = 4;
    } else if(strcmp(function, "J4NP") == 0) {
        opcode = J4NP;
        mod = 5;
    } else if(strcmp(function, "J5N") == 0) {
        opcode = J5N;
        mod = 0;
    } else if(strcmp(function, "J5Z") == 0) {
        opcode = J5Z;
        mod = 1;
    } else if(strcmp(function, "J5P") == 0) {
        opcode = J5P;
        mod = 2;
    } else if(strcmp(function, "J5NN") == 0) {
        opcode = J5NN;
        mod = 3;
    } else if(strcmp(function, "J5NZ") == 0) {
        opcode = J5NZ;
        mod = 4;
    } else if(strcmp(function, "J5NP") == 0) {
        opcode = J5NP;
        mod = 5;
    } else if(strcmp(function, "J6N") == 0) {
        opcode = J6N;
        mod = 0;
    } else if(strcmp(function, "J6Z") == 0) {
        opcode = J6Z;
        mod = 1;
    } else if(strcmp(function, "J6P") == 0) {
        opcode = J6P;
        mod = 2;
    } else if(strcmp(function, "J6NN") == 0) {
        opcode = J6NN;
        mod = 3;
    } else if(strcmp(function, "J6NZ") == 0) {
        opcode = J6NZ;
        mod = 4;
    } else if(strcmp(function, "J6NP") == 0) {
        opcode = J6NP;
        mod = 5;
    } else if(strcmp(function, "JBUS") == 0) {
        opcode = JBUS;
    } else if(strcmp(function, "IOC") == 0) {
        opcode = IOC;
    } else if(strcmp(function, "IN") == 0) {
        opcode = IN;
    } else if(strcmp(function, "OUT") == 0) {
        opcode = OUT;
    } else if(strcmp(function, "JRED") == 0) {
        opcode = JRED;
    } else if(strcmp(function, "NUM") == 0) {
        opcode = NUM;
        mod = 0;
    } else if(strcmp(function, "CHAR") == 0) {
        opcode = CHAR;
        mod = 1;
    } else if(strcmp(function, "SLA") == 0) {
        opcode = SLA;
        mod = 0;
    } else if(strcmp(function, "SRA") == 0) {
        opcode = SRA;
        mod = 1;
    } else if(strcmp(function, "SLAX") == 0) {
        opcode = SLAX;
        mod = 2;
    } else if(strcmp(function, "SRAX") == 0) {
        opcode = SRAX;
        mod = 3;
    } else if(strcmp(function, "SLC") == 0) {
        opcode = SLC;
        mod = 4;
    } else if(strcmp(function, "SRC") == 0) {
        opcode = SRC;
        mod = 5;
    } else if(strcmp(function, "SLB") == 0) {
        opcode = SLB;
        mod = 0;
    } else if(strcmp(function, "SRB") == 0) {
        opcode = SRB;
        mod = 1;
    } else if(strcmp(function, "MOVE") == 0) {
        opcode = MOVE;
    } else if(strcmp(function, "NOP") == 0) {
        opcode = NOP;
        mod = 0;
    } else if(strcmp(function, "HLT") == 0) {
        opcode = HLT;
        mod = 2;
    } else if(strcmp(function, "ORIG") == 0) {
        opcode = ORIG;
        mod = 1;
    } else if(strcmp(function, "EQU") == 0) {
        opcode = EQU;
    } else {
        opcode = NO_OP_FOUND;
    }


    instruction.operation = opcode;
    instruction.mod.data = mod;

    return instruction;
}

int load_to_register(mix_register *dest, mix_word orig) {
    return load_to_register(&dest->word, orig);
}

int load_to_register(mix_register *dest, mix_word orig, unsigned int mod) {
    return load_to_register(&dest->word, orig, mod);
}

int load_to_register(mix_word *dest, mix_register orig) {
    return load_to_register(dest, orig.word);
}

int load_to_register(mix_word *dest, mix_register orig, unsigned int mod) {
    return load_to_register(dest, orig.word, mod);
}

int load_to_register(mix_word *dest, mix_word orig) {
    dest->sign = orig.sign;
    dest->word.A1 = orig.word.A1;
    dest->word.A2 = orig.word.A2;
    dest->word.I = orig.word.I;
    dest->word.F = orig.word.F;
    dest->word.C = orig.word.C;
    return 0;
}

void copy_word(mix_word *from, mix_word *to) {
    from->word.A1 = to->word.A1;
    from->word.A2 = to->word.A2;
    from->word.I = to->word.I;
    from->word.F = to->word.F;
    from->word.C = to->word.C;

    from->sign = to->sign;
}

int load_to_register(mix_word *dest, mix_word orig, unsigned int mod) {
    copy_word(dest, &orig);
    return 0;
}

int add_to_register(mix_word *dest, mix_word orig) {
    int overflow = 0;

    int val_orig = convert_bytes_to_data(&orig);
    if (orig.sign == MIX_NEGATIVE) {
        val_orig *= -1;
    }

    int val_dest = convert_bytes_to_data(dest);
    if (dest->sign == MIX_NEGATIVE) {
        val_dest *= -1;
    }

    int sum = abs(val_dest + val_orig);
    if (sum > MAX_WORD_SIZE) {
        overflow = 1;
        sum = sum - (MAX_WORD_SIZE);
    }

    convert_data_to_bytes(dest, sum);

    if ((abs(val_dest)< abs(val_orig)) && (val_orig < 0)) {
        dest->sign = MIX_NEGATIVE;
    }
    return overflow;
}

int mult_registers(mix_computer *computer, mix_word orig) {

    long result = convert_bytes_to_data(&computer->rA) * convert_bytes_to_data(&orig);

    unsigned int sign = MIX_POSITIVE;
    if (result < 0) {
        sign = MIX_NEGATIVE;
        result *= -1;
    }

    computer->rA.sign = sign;

    unsigned long max = (MAX_WORD_SIZE) - 1;
    unsigned long tmp1 = labs(result) & max;
    unsigned long tmp2 = labs(result) & ~max;

    convert_data_to_bytes(&computer->rA, tmp2);
    convert_data_to_bytes(&computer->rX, tmp1);

    return 0;
}

int div_registers(mix_computer *computer, mix_word orig) {

    int div = (convert_bytes_to_data(&computer->rA) << 30) + (convert_bytes_to_data(&computer->rX));

    int result = div / convert_bytes_to_data(&orig);


    if (computer->rA.sign == FALSE) {
        result *= -1;
    }
    if (orig.sign == FALSE) {
        result *= -1;
    }

    unsigned int sign = TRUE;
    if (result < 0) {
        sign = FALSE;
        result *= -1;
    }

    computer->rA.sign = sign;

    unsigned long max = (MAX_WORD_SIZE) - 1;
    unsigned long tmp1 = labs(result) & max;
    unsigned long tmp2 = labs(result) & ~max;

    convert_data_to_bytes(&computer->rA, tmp1);
    convert_data_to_bytes(&computer->rX, tmp2);

    return 0;
}

void reset_comparition_registers(mix_computer *computer) {
    computer->comparison.E = FALSE;
    computer->comparison.G = FALSE;
    computer->comparison.L = FALSE;
}

int compare_registers(mix_word a, mix_word b, mix_computer *computer) {
    int cmpRes = 0;

    int cmpDataA = convert_bytes_to_data(&a);
    int cmpDataB = convert_bytes_to_data(&b);

    if (cmpDataA > cmpDataB) {
        cmpRes = 1;
    } else if (cmpDataA <cmpDataB) {
        cmpRes = -1;
    } else {
        cmpRes = 0;
    }
    reset_comparition_registers(computer);
    switch (cmpRes) {
        case 0:
        computer->comparison.E = TRUE;
        break;
        case 1:
        computer->comparison.G = TRUE;
        break;
        case -1:
        computer->comparison.L = TRUE;
        break;
    }
    return cmpRes;
}

void conver_register_to_num(mix_word *r1, mix_word *r2) {
    char alfaNum[11];
    snprintf(alfaNum, 11, "%d%d%d%d%d%d%d%d%d%d",
             r1->word.A1.data,
             r1->word.A2.data,
             r1->word.C.data,
             r1->word.F.data,
             r1->word.I.data,
             r2->word.A1.data,
             r2->word.A2.data,
             r2->word.C.data,
             r2->word.F.data,
             r2->word.I.data);
    unsigned int i;
    unsigned int converted = 0;
    unsigned int convertedNumbers = 0;
    for (i = 0; i < 11; i+=2) {
        char tmp[3];
        unsigned int tmpNum;
        tmp[0] = alfaNum[i];
        tmp[1] = alfaNum[i+1];
        tmp[2] = '\0';
        char *end;
        tmpNum = (unsigned int) strtol(tmp, &end, 10);
        tmpNum = tmpNum % 10;
        converted = converted * pow(10, convertedNumbers) + tmpNum;
        convertedNumbers++;
    }
}

void convert_registers_to_char(mix_word *r1, mix_word *r2) {
    r1->word.A1.data += 30;
    r1->word.A2.data += 30;

    r1->word.C.data += 30;
    r1->word.F.data += 30;
    r1->word.I.data += 30;

    r2->word.A1.data += 30;
    r2->word.A2.data += 30;

    r2->word.C.data += 30;
    r2->word.F.data += 30;
    r2->word.I.data += 30;
}

void shift_register_left(mix_word *r) {
    mix_word rA;
    copy_word(&rA, r);
    rA.word.A1 = rA.word.A2;
    rA.word.A2 = rA.word.I;
    rA.word.I = rA.word.F;
    rA.word.F = rA.word.C;
    mix_byte C;
    C.data = 0;
    rA.word.C = C;
    *r = rA;
}

void shift_register_right(mix_word *r) {
    mix_word rA;
    copy_word(&rA, r);
    rA.word.C = r->word.F;
    rA.word.F = r->word.I;
    rA.word.I = r->word.A2;
    rA.word.A2 = r->word.A1;
    mix_byte A1;
    A1.data = 0;
    rA.word.A1 = A1;
    *r = rA;
}

void shift_two_registers_left(mix_word *r1, mix_word *r2) {
    mix_word rA;
    copy_word(&rA, r1);
    mix_word rX;
    copy_word(&rX, r2);
    rA.word.A1 = rA.word.A2;
    rA.word.A2 = rA.word.I;
    rA.word.I = rA.word.F;
    rA.word.F = rA.word.C;
    rA.word.C = rX.word.A1;

    rX.word.A1 = rX.word.A2;
    rX.word.A2 = rX.word.I;
    rX.word.I = rX.word.F;
    rX.word.F = rX.word.C;
    mix_byte C;
    C.data = 0;
    rX.word.C = C;
    *r1 = rA;
    *r2 = rX;
}

void shift_two_registers_right(mix_word *r1, mix_word *r2) {
    mix_word rA;
    copy_word(&rA, r1);
    mix_word rX;
    copy_word(&rX, r2);

    rX.word.C = rX.word.F;
    rX.word.F = rX.word.I;
    rX.word.I = rX.word.A2;
    rX.word.A2 = rX.word.A1;
    rX.word.A1 = rA.word.C;

    rA.word.C = rA.word.F;
    rA.word.F = rA.word.I;
    rA.word.I = rA.word.A2;
    rA.word.A2 = rA.word.A1;

    mix_byte A1;
    A1.data = 0;
    rA.word.A1 = A1;
    *r1 = rA;
    *r2 = rX;
}

void shift_register_left_circular(mix_word *r) {
    mix_word rA;
    copy_word(&rA, r);
    rA.word.A1 = rA.word.A2;
    rA.word.A2 = rA.word.I;
    rA.word.I = rA.word.F;
    rA.word.F = rA.word.C;
    rA.word.C = r->word.A1;
    *r = rA;
}

void shift_register_right_circular(mix_word *r) {
    mix_word rA;
    copy_word(&rA, r);
    rA.word.C = r->word.F;
    rA.word.F = r->word.I;
    rA.word.I = r->word.A2;
    rA.word.A2 = r->word.A1;
    rA.word.A1 = r->word.I;
    *r = rA;
}

void move_n_words(mix_word *dest, mix_word from, unsigned int n) {
    if (n > 0) {
        dest->word.C = from.word.A1;
        n--;
    }
    if (n > 0) {
        dest->word.C = from.word.A2;
        dest->word.F = from.word.A1;
        n--;
    }
    if (n > 0) {
        dest->word.C = from.word.I;
        dest->word.F = from.word.A2;
        dest->word.I = from.word.A1;
        n--;
    }
    if (n > 0) {
        dest->word.A2 = from.word.F;
        dest->word.C = from.word.I;
        dest->word.F = from.word.A2;
        dest->word.I = from.word.A1;
        n--;
    }
    if (n > 0) {
        dest->word.A1 = from.word.C;
        dest->word.A2 = from.word.F;
        dest->word.C = from.word.I;
        dest->word.F = from.word.A2;
        dest->word.I = from.word.A1;
    }
}

unsigned int increase_word(mix_word *w, int c) {
    unsigned int overflow = FALSE;
    unsigned int sign = MIX_POSITIVE;
    int sum = convert_bytes_to_data(w) + c;
    if (sum > MAX_WORD_SIZE) {
        sum = 0;
        overflow = TRUE;
    }
    if (sum < 0) {
        sign = MIX_NEGATIVE;
    }

    convert_data_to_bytes(w, abs(sum));
    w->sign = sign;
    return overflow;
}

unsigned int increase_register(mix_register *r, int c) {
    return increase_word(&r->word, c);;
}

void jum_if_word_negative(mix_computer *computer, mix_word *w, unsigned int registerIndex) {
    if (w->sign == MIX_NEGATIVE && convert_bytes_to_data(w) > 0) {
        convert_data_to_bytes(&computer->rj.word,registerIndex);
        computer->instructionIndex = registerIndex;
    }
}

void jum_if_word_positive(mix_computer *computer, mix_word *w, unsigned int registerIndex) {
    if (w->sign == MIX_POSITIVE && convert_bytes_to_data(w) > 0) {
        convert_data_to_bytes(&computer->rj.word, registerIndex);
        computer->instructionIndex = registerIndex;
    }
}

void jum_if_word_zero(mix_computer *computer, mix_word *w, unsigned int registerIndex) {
    if (convert_bytes_to_data(w) == 0) {
        convert_data_to_bytes(&computer->rj.word, registerIndex);
        computer->instructionIndex = registerIndex;
    }
}

void jum_if_word_no_negative(mix_computer *computer, mix_word *w, unsigned int registerIndex) {
    if (w->sign == MIX_POSITIVE) {
        convert_data_to_bytes(&computer->rj.word, registerIndex);
        computer->instructionIndex = registerIndex;
    }
}

void jum_if_word_no_positive(mix_computer *computer, mix_word *w, unsigned int registerIndex) {
    if (w->sign == MIX_NEGATIVE) {
        convert_data_to_bytes(&computer->rj.word, registerIndex);
        computer->instructionIndex = registerIndex;
    }
}

void jum_if_word_no_zero(mix_computer *computer, mix_word *w, unsigned int registerIndex) {
    if (w->sign != 0) {
        convert_data_to_bytes(&computer->rj.word, registerIndex);
        computer->instructionIndex = registerIndex;
    }
}

void jum_if_word_even(mix_computer *computer, mix_word *w, unsigned int registerIndex) {
    if ((convert_bytes_to_data(w) % 2) == 0) {
        convert_data_to_bytes(&computer->rj.word, registerIndex);
        computer->instructionIndex = registerIndex;
    }
}

void jum_if_word_odd(mix_computer *computer, mix_word *w, unsigned int registerIndex) {
    if ((convert_bytes_to_data(w) % 2) != 0) {
        convert_data_to_bytes(&computer->rj.word, registerIndex);
        computer->instructionIndex = registerIndex;
    }
}

