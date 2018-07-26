#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
#include "math.h"
#include "mix_compiler.h"

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

void store_address_into_index(mix_register *r, unsigned int address) {
    mix_register reg = convert_int_to_register(address);
    *r = reg;
}

void print_code_instruction(mix_code_instruction code_instruction) {
    int print = FALSE;
    if (code_instruction.hasLabel == TRUE) {
        printf("(label)%s ",
               code_instruction.label);
        print = TRUE;
    }
    if (code_instruction.hasInstruction == TRUE) {
        printf("(op:mod)%d:%d (v)%d ",
               code_instruction.instruction.operation,
               code_instruction.instruction.mod,
               code_instruction.instruction.v);
        print = TRUE;
    }
    if (code_instruction.hasArgument == TRUE) {
        printf("(arg)%s ",
               code_instruction.argument);
        print = TRUE;
    }
    if (print) {
        printf("\n");
    }
}

unsigned int get_memory_address_from_index(mix_register r) {
    return abs(convert_register_to_int(r));
}

unsigned int compute_address(mix_computer *computer,char *address) {
    unsigned int addr = 0;
    char *end;

    unsigned int is_int = (unsigned int) strtol(address, &end, 10);
    if (strcmp(end,"") != 0) {
        int i;
        for(i = 0; i < computer->labels_count; i++) {
            if (strcmp(computer->labels_computed[i].label, end) == 0) {
                addr = computer->labels_computed[i].address;
                break;
            }
        }
        if (addr == 0) {
            for(i = 0; i < computer->variables_count; i++) {
                if (strcmp(computer->computer_variables[i].name, end) == 0) {
                    addr = computer->computer_variables[i].value;
                    break;
                }
            }
        }
    } else {
        addr = is_int;
    }
    return addr;
}

unsigned int get_instruction_address(mix_computer *computer, mix_instruction instruction) {
    unsigned int indexData;
    mix_register r;
    switch (instruction.index.data) {
    case 1:
        r = computer->r1;
        break;
    case 2:
        r = computer->r2;
        break;
    case 3:
        r = computer->r3;
        break;
    case 4:
        r = computer->r4;
        break;
    case 5:
        r = computer->r5;
        break;
    case 6:
        r = computer->r6;
        break;
    }
    indexData = convert_register_to_int(r);
    return instruction.v + indexData;
}

void parse_instruction(mix_computer *computer, mix_code_instruction *instruction) {
    // First we process the argument to find bitfields
    if (strchr(instruction->argument, '(') != nullptr) {
        char *address = strtok(instruction->argument,"(:)");
        char *from = strtok(nullptr, "(:)");
        char *dest = strtok(nullptr, "(:)");

        instruction->instruction.v = compute_address(computer, address);

        if (from != nullptr) {
            unsigned int mod;
            if (instruction->instruction.operation >= JBUS &&
                    instruction->instruction.operation <=JRED) {
                // We are in an I/O instruction, we want to handle the mod as a device id
                char *end;
                mod = (unsigned int) strtol(from, &end, 10);
                if (strcmp(end, from) == 0) {
                    unsigned int i;
                    for(i = 0; i < computer->variables_count; i++) {
                        if (strcmp(computer->computer_variables[i].name, end) == 0) {
                            mod = computer->computer_variables[i].value;
                            break;
                        }
                    }
                }
            } else {
                mix_range range;
                if (dest != nullptr) {
                    snprintf(range.range, 6, "(%s:%s)", from, dest);
                    mod = convert_range_to_int(&range);
                } else {
                    if (instruction->instruction.operation == MOVE) {
                        mod = (unsigned int) strtol(from,NULL,10);
                    } else {
                        snprintf(range.range, 6, "(%s)", from);
                        mod = convert_range_to_int(&range);
                    }
                }
            }

            instruction->instruction.mod.data = mod;
        }
    } else {
        instruction->instruction.v = compute_address(computer, instruction->argument);
    }
}

mix_word apply_modifier_to_register(mix_word *reg, unsigned int mod) {
    mix_word result;

    if (mod == 0) {
        return *reg;
    }

    int from = get_from_range(mod) - 1;
    int to = 5 - get_to_range(mod);
    copy_word(&result, reg);
    if (from < 0) {
        from = 0;
    } else if (reg->sign == MIX_NEGATIVE) {
        // We only want to copy de sign if the range is from 0
        result.sign = MIX_POSITIVE;
    }

    unsigned int i;
    for (i = 0; i < from; i++) {
        shift_register_left(&result);
    }

    for (i = 0; i < to; i++) {
        shift_register_right(&result);
    }

    return result;
}

int execute_instruction(mix_computer *computer, mix_code_instruction instruction) {

    unsigned int registerIndex = get_instruction_address(computer, instruction.instruction);

    mix_word registerDest = apply_modifier_to_register(&computer->memory[registerIndex],
                                                       instruction.instruction.mod.data);

    switch (instruction.instruction.operation) {
    case LDA: {
        load_to_register(&computer->rA, registerDest);
    } break;
    case LD1: {
        load_to_register(&computer->r1, registerDest);
    } break;
    case LD2: {
        load_to_register(&computer->r2, registerDest);
    } break;
    case LD3: {
        load_to_register(&computer->r3, registerDest);
    } break;
    case LD4: {
        load_to_register(&computer->r4, registerDest);
    } break;
    case LD5: {
        load_to_register(&computer->r5, registerDest);
    } break;
    case LD6: {
        load_to_register(&computer->r6, registerDest);
    } break;
    case LDX: {
        load_to_register(&computer->rX, registerDest);
    } break;
    case LDAN: {
        mix_word tmp = registerDest;
        if (tmp.sign == TRUE) {
            tmp.sign = FALSE;
        } else {
            tmp.sign = TRUE;
        }
        load_to_register(&computer->rA, tmp);
    } break;
    case LD1N: {
        mix_word tmp = registerDest;
        if (tmp.sign == TRUE) {
            tmp.sign = FALSE;
        } else {
            tmp.sign = TRUE;
        }
        load_to_register(&computer->r1, tmp);
    } break;
    case LD2N: {
        mix_word tmp = registerDest;
        if (tmp.sign == TRUE) {
            tmp.sign = FALSE;
        } else {
            tmp.sign = TRUE;
        }
        load_to_register(&computer->r2, tmp);
    } break;
    case LD3N: {
        mix_word tmp = registerDest;
        if (tmp.sign == TRUE) {
            tmp.sign = FALSE;
        } else {
            tmp.sign = TRUE;
        }
        load_to_register(&computer->r3, tmp);
    } break;
    case LD4N: {
        mix_word tmp = registerDest;
        if (tmp.sign == TRUE) {
            tmp.sign = FALSE;
        } else {
            tmp.sign = TRUE;
        }
        load_to_register(&computer->r4, tmp);
    } break;
    case LD5N: {
        mix_word tmp = registerDest;
        if (tmp.sign == TRUE) {
            tmp.sign = FALSE;
        } else {
            tmp.sign = TRUE;
        }
        load_to_register(&computer->r5, tmp);
    } break;
    case LD6N: {
        mix_word tmp = registerDest;
        if (tmp.sign == TRUE) {
            tmp.sign = FALSE;
        } else {
            tmp.sign = TRUE;
        }
        load_to_register(&computer->r6, tmp);
    } break;
    case LDXN: {
        mix_word tmp = registerDest;
        if (tmp.sign == TRUE) {
            tmp.sign = FALSE;
        } else {
            tmp.sign = TRUE;
        }
        load_to_register(&computer->rX, tmp);
    } break;

    case ADD: {
        int overflow = add_to_register(&computer->rA, registerDest);
        if (overflow) {
            computer->overflow = 1;
        }
    } break;
    case SUB: {
        mix_word tmp;
        load_to_register(&tmp, registerDest);
        if (tmp.sign == TRUE) {
            tmp.sign = FALSE;
        } else {
            tmp.sign = TRUE;
        }
        int overflow = add_to_register(&computer->rA, tmp);
        if (overflow) {
            computer->overflow = 1;
        }
    } break;
    case MUL: {
        mult_registers(computer, registerDest);
    } break;
    case DIV: {
        div_registers(computer, registerDest);
    } break;

    case STA: {
        mix_word ra = apply_modifier_to_register(&computer->rA, instruction.instruction.mod.data);
        load_to_register(&computer->memory[registerIndex], ra);

    }break;
    case STX: {
        mix_word rX = apply_modifier_to_register(&computer->rX, instruction.instruction.mod.data);
        load_to_register(&computer->memory[registerIndex], rX);

    }break;
    case ST1: {
        mix_word r1 = apply_modifier_to_register(&computer->r1.word, instruction.instruction.mod.data);
        load_to_register(&computer->memory[registerIndex], r1);
    }break;
    case ST2: {
        mix_word r2 = apply_modifier_to_register(&computer->r2.word, instruction.instruction.mod.data);
        load_to_register(&computer->memory[registerIndex], r2);
    }break;
    case ST3: {
        mix_word r3 = apply_modifier_to_register(&computer->r3.word, instruction.instruction.mod.data);
        load_to_register(&computer->memory[registerIndex], r3);
    }break;
    case ST4: {
        mix_word r4 = apply_modifier_to_register(&computer->r4.word, instruction.instruction.mod.data);
        load_to_register(&computer->memory[registerIndex], r4);
    }break;
    case ST5: {
        mix_word r5 = apply_modifier_to_register(&computer->r5.word, instruction.instruction.mod.data);
        load_to_register(&computer->memory[registerIndex], r5);
    }break;
    case ST6: {
        mix_word r6 = apply_modifier_to_register(&computer->r6.word, instruction.instruction.mod.data);
        load_to_register(&computer->memory[registerIndex], r6);
    }break;

    case CMPA: {
        compare_registers(computer->rA, registerDest,computer);
    }break;
    case CMPX: {
        compare_registers(computer->rX, registerDest,computer);
    }break;
    case CMP1: {
        compare_registers(computer->r1.word, registerDest,computer);
    }break;
    case CMP2: {
        compare_registers(computer->r2.word, registerDest,computer);
    }break;
    case CMP3: {
        compare_registers(computer->r3.word, registerDest,computer);
    }break;
    case CMP4: {
        compare_registers(computer->r4.word, registerDest,computer);
    }break;
    case CMP5: {
        compare_registers(computer->r5.word, registerDest,computer);
    }break;
    case CMP6: {
        compare_registers(computer->r6.word, registerDest,computer);
    }break;

    case JMP:
    {
        switch (instruction.instruction.mod.data) {
        case 0: {
            convert_data_to_bytes(&computer->rj.word, registerIndex);
            computer->instructionIndex = registerIndex - 1;
        }break;
        case 1:{
            computer->instructionIndex = registerIndex;
        }break; // JSJ
        case 2: { // JOV
            if (computer->overflow == 1) {
                convert_data_to_bytes(&computer->rj.word, registerIndex);
                computer->instructionIndex = registerIndex - 1;
            }
        }break;
        case 3: { // JNOV
            if (computer->overflow == 0) {
                convert_data_to_bytes(&computer->rj.word, registerIndex);
                computer->instructionIndex = registerIndex - 1;
            }
        }break;
        case 4: { // JL
            if (computer->comparison.L == 1) {
                convert_data_to_bytes(&computer->rj.word, registerIndex);
                computer->instructionIndex = registerIndex - 1;
            }
        }break;
        case 5: { // JE
            if (computer->comparison.E == 1) {
                convert_data_to_bytes(&computer->rj.word, registerIndex);
                computer->instructionIndex = registerIndex - 1;
            }
        }break;
        case 6: { // JG
            if (computer->comparison.G == 1) {
                convert_data_to_bytes(&computer->rj.word, registerIndex);
                computer->instructionIndex = registerIndex - 1;
            }
        }break;
        case 7: { // JGE
            if (computer->comparison.L == 0) {
                convert_data_to_bytes(&computer->rj.word, registerIndex);
                computer->instructionIndex = registerIndex - 1;
            }
        }break;
        case 8: { // JNE
            if (computer->comparison.E == 0) {
                convert_data_to_bytes(&computer->rj.word, registerIndex);
                computer->instructionIndex = registerIndex - 1;
            }
        }break;
        case 9: { // JLE
            if (computer->comparison.G == 0) {
                convert_data_to_bytes(&computer->rj.word, registerIndex);
                computer->instructionIndex = registerIndex - 1;
            }
        }break;
        }
    }break;
    case JAN:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // JAN
            jum_if_word_negative(computer, &computer->rA, registerIndex);
        }break;
        case 1:{ // JAZ
            jum_if_word_zero(computer, &computer->r3.word, registerIndex);
        }break;
        case 2:{ // JAP
            jum_if_word_positive(computer, &computer->rA, registerIndex);
        }break;
        case 3:{ // JANN
            jum_if_word_no_negative(computer, &computer->rA, registerIndex);
        }break;
        case 4:{ // JANZ
            jum_if_word_no_zero(computer, &computer->rA, registerIndex);
        }break;
        case 5:{ // JANP
            jum_if_word_no_positive(computer, &computer->rA, registerIndex);
        }break;
        case 6:{ // JAE
            jum_if_word_even(computer, &computer->rA, registerIndex);
        }break;
        case 7:{ // JAO
            jum_if_word_odd(computer, &computer->rA, registerIndex);
        }break;
        }
    }break;
    case JXN:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // JXN
            jum_if_word_negative(computer, &computer->rX, registerIndex);
        }break;
        case 1:{ // JXZ
            jum_if_word_zero(computer, &computer->rX, registerIndex);
        }break;
        case 2:{ // JXP
            jum_if_word_positive(computer, &computer->rX, registerIndex);
        }break;
        case 3:{ // JXNN
            jum_if_word_no_negative(computer, &computer->rX, registerIndex);
        }break;
        case 4:{ // JXNZ
            jum_if_word_no_zero(computer, &computer->rX, registerIndex);
        }break;
        case 5:{ // JXNP
            jum_if_word_no_positive(computer, &computer->rX, registerIndex);
        }break;
        case 6:{ // JXE
            jum_if_word_even(computer, &computer->rX, registerIndex);
        }break;
        case 7:{ // JXO
            jum_if_word_odd(computer, &computer->rX, registerIndex);
        }break;
        }
    }break;
    case J1N:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // J1N
            jum_if_word_negative(computer, &computer->r1.word, registerIndex);
        }break;
        case 1:{ // J1Z
            jum_if_word_zero(computer, &computer->r1.word, registerIndex);
        }break;
        case 2:{ // J1P
            jum_if_word_positive(computer, &computer->r1.word, registerIndex);
        }break;
        case 3:{ // J1NN
            jum_if_word_no_negative(computer, &computer->r1.word, registerIndex);
        }break;
        case 4:{ // J1NZ
            jum_if_word_no_zero(computer, &computer->r1.word, registerIndex);
        }break;
        case 5:{ // J1NP
            jum_if_word_no_positive(computer, &computer->r1.word, registerIndex);
        }break;
        }
    }break;
    case J2N:{
        switch (instruction.instruction.mod.data) {
            case 0:{ // J2N
                jum_if_word_negative(computer, &computer->r2.word, registerIndex);
            }break;
            case 1:{ // J2Z
                jum_if_word_zero(computer, &computer->r2.word, registerIndex);
            }break;
            case 2:{ // J2P
                jum_if_word_positive(computer, &computer->r2.word, registerIndex);
            }break;
            case 3:{ // J2NN
                jum_if_word_no_negative(computer, &computer->r2.word, registerIndex);
            }break;
            case 4:{ // J2NZ
                jum_if_word_no_zero(computer, &computer->r2.word, registerIndex);
            }break;
            case 5:{ // J2NP
                jum_if_word_no_positive(computer, &computer->r2.word, registerIndex);
            }break;
            }
    }break;
    case J3N:{
            switch (instruction.instruction.mod.data) {
            case 0:{ // J3N
                jum_if_word_negative(computer, &computer->r3.word, registerIndex);
            }break;
            case 1:{ // J3Z
                jum_if_word_zero(computer, &computer->r3.word, registerIndex);
            }break;
            case 2:{ // J3P
                jum_if_word_positive(computer, &computer->r3.word, registerIndex);
            }break;
            case 3:{ // J3NN
                jum_if_word_no_negative(computer, &computer->r3.word, registerIndex);
            }break;
            case 4:{ // J3NZ
                jum_if_word_no_zero(computer, &computer->r3.word, registerIndex);
            }break;
            case 5:{ // J3NP
                jum_if_word_no_positive(computer, &computer->r3.word, registerIndex);
            }break;
            }
    }break;
    case J4N:{
            switch (instruction.instruction.mod.data) {
            case 0:{ // J4N
                jum_if_word_negative(computer, &computer->r4.word, registerIndex);
            }break;
            case 1:{ // J4Z
                jum_if_word_zero(computer, &computer->r4.word, registerIndex);
            }break;
            case 2:{ // J4P
                jum_if_word_positive(computer, &computer->r4.word, registerIndex);
            }break;
            case 3:{ // J4NN
                jum_if_word_no_negative(computer, &computer->r4.word, registerIndex);
            }break;
            case 4:{ // J4NZ
                jum_if_word_no_zero(computer, &computer->r4.word, registerIndex);
            }break;
            case 5:{ // J4NP
                jum_if_word_no_positive(computer, &computer->r4.word, registerIndex);
            }break;
            }
    }break;
    case J5N:{
            switch (instruction.instruction.mod.data) {
            case 0:{ // J5N
                jum_if_word_negative(computer, &computer->r5.word, registerIndex);
            }break;
            case 1:{ // J5Z
                jum_if_word_zero(computer, &computer->r5.word, registerIndex);
            }break;
            case 2:{ // J5P
                jum_if_word_positive(computer, &computer->r5.word, registerIndex);
            }break;
            case 3:{ // J5NN
                jum_if_word_no_negative(computer, &computer->r5.word, registerIndex);
            }break;
            case 4:{ // J5NZ
                jum_if_word_no_zero(computer, &computer->r5.word, registerIndex);
            }break;
            case 5:{ // J5NP
                jum_if_word_no_positive(computer, &computer->r5.word, registerIndex);
            }break;
            }
    }break;
    case J6N:{
            switch (instruction.instruction.mod.data) {
            case 0:{ // J6N
                jum_if_word_negative(computer, &computer->r6.word, registerIndex);
            }break;
            case 1:{ // J6Z
                jum_if_word_zero(computer, &computer->r6.word, registerIndex);
            }break;
            case 2:{ // J6P
                jum_if_word_positive(computer, &computer->r6.word, registerIndex);
            }break;
            case 3:{ // J6NN
                jum_if_word_no_negative(computer, &computer->r6.word, registerIndex);
            }break;
            case 4:{ // J6NZ
                jum_if_word_no_zero(computer, &computer->r6.word, registerIndex);
            }break;
            case 5:{ // J6NP
                jum_if_word_no_positive(computer, &computer->r6.word, registerIndex);
            }break;
            }
    }break;

    case ENTA:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // INCA
            unsigned int overflow  = increase_word(&computer->rA, instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 1:{ // DECA
            unsigned int overflow  = increase_word(&computer->rA, -instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 2: // ENTA
        convert_data_to_bytes(&computer->rA, instruction.instruction.v);
        break;
        case 3: // ENNA
        convert_data_to_bytes(&computer->rA, instruction.instruction.v);
        computer->rA.sign = !computer->rA.sign;
        break;
        }
    }break;
    case ENTX:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // INCX
            unsigned int overflow  = increase_word(&computer->rX, instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 1:{ // DECX
            unsigned int overflow  = increase_word(&computer->rX, -instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 2:
        convert_data_to_bytes(&computer->rX, instruction.instruction.v);
        break;
        case 3:
        convert_data_to_bytes(&computer->rX, instruction.instruction.v);
        computer->rX.sign = !computer->rX.sign;
        break;
        }
    }break;
    case ENT1:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // INC1
            unsigned int overflow = increase_register(&computer->r1, instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 1:{ // DEC1
            unsigned int overflow = increase_register(&computer->r1, -instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 2: // ENT1
            store_address_into_index(&computer->r1,instruction.instruction.v);
        break;
        case 3: // ENNT1
            store_address_into_index(&computer->r1,instruction.instruction.v);
            computer->r1.bytes.sign = !computer->r1.bytes.sign;
        break;
        }
    }break;
    case ENT2:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // INC2
            unsigned int overflow = increase_register(&computer->r2, instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 1:{ // DEC2
            unsigned int overflow = increase_register(&computer->r2, -instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 2:
            store_address_into_index(&computer->r2,instruction.instruction.v);
        break;
        case 3:
            store_address_into_index(&computer->r2,instruction.instruction.v);
            computer->r2.bytes.sign = !computer->r2.bytes.sign;
        break;
        }
    }break;
    case ENT3:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // INC3
            unsigned int overflow = increase_register(&computer->r3, instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 1:{ // DEC3
            unsigned int overflow = increase_register(&computer->r3, -instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 2:
            store_address_into_index(&computer->r3,instruction.instruction.v);
        break;
        case 3:
            store_address_into_index(&computer->r3,instruction.instruction.v);
            computer->r3.bytes.sign = !computer->r3.bytes.sign;
        break;
        }
    }break;
    case ENT4:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // INC4
            unsigned int overflow = increase_register(&computer->r4, instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 1:{ // DEC4
            unsigned int overflow = increase_register(&computer->r4, -instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 2:
            store_address_into_index(&computer->r4,instruction.instruction.v);
        break;
        case 3:
            store_address_into_index(&computer->r4,instruction.instruction.v);
            computer->r4.bytes.sign = !computer->r4.bytes.sign;
        break;
        }
    }break;
    case ENT5:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // INC5
            unsigned int overflow = increase_register(&computer->r5, instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 1:{ // DEC5
            unsigned int overflow = increase_register(&computer->r5, -instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 2:
            store_address_into_index(&computer->r4,instruction.instruction.v);
        break;
        case 3:
            store_address_into_index(&computer->r4,instruction.instruction.v);
            computer->r4.bytes.sign = !computer->r4.bytes.sign;
        break;
        }
    }break;
    case ENT6:{
        switch (instruction.instruction.mod.data) {
        case 0:{ // INC6
            unsigned int overflow = increase_register(&computer->r6, instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 1:{ // DEC6
            unsigned int overflow = increase_register(&computer->r6, -instruction.instruction.v);
            computer->overflow = overflow;
        }break;
        case 2:
            store_address_into_index(&computer->r4,instruction.instruction.v);
        break;
        case 3:
            store_address_into_index(&computer->r4,instruction.instruction.v);
            computer->r4.bytes.sign = !computer->r4.bytes.sign;
        break;
        }
    }break;

    case IN:{
        if (instruction.instruction.mod.data < 8) {
            // TAPE
        } else if (instruction.instruction.mod.data < 16) {
            // DISKs
        } else if (instruction.instruction.mod.data == 16) {
            // Card-reader
        } else if (instruction.instruction.mod.data == 17) {
            // Card-writer
        } else if (instruction.instruction.mod.data == 18) {
            // line printer
        } else if (instruction.instruction.mod.data == 19) {
            // Terminal
            int input;
            printf("> ");
            scanf("%d", &input);
            convert_data_to_bytes(&computer->memory[instruction.instruction.v], input);
        } else if (instruction.instruction.mod.data == 20) {
            // Paper tape
        }
    }break;
    case OUT:{
        if (instruction.instruction.mod.data < 0) {
            printf("The device %d does not exists", instruction.instruction.mod.data);
        }
        else if (instruction.instruction.mod.data < 8) {
            // TAPE
        } else if (instruction.instruction.mod.data < 16) {
            // DISKs
        } else if (instruction.instruction.mod.data == 16) {
            // Card-reader
        } else if (instruction.instruction.mod.data == 17) {
            // Card-writer
        } else if (instruction.instruction.mod.data == 18) {
            // line printer
        } else if (instruction.instruction.mod.data == 19) {
            // Terminal
            printf("%d\n", convert_bytes_to_data(&computer->memory[instruction.instruction.v]));
        } else if (instruction.instruction.mod.data == 20) {
            // Paper tape
        }
    }break;
    case IOC:{

    }break;
    case JRED:{

    }break;
    case JBUS:{

    }break;

    case NUM: {
        switch (instruction.instruction.mod.data) {
        case 0: // NUM
        {
            conver_register_to_num(&computer->rA, &computer->rX);
        }break;
        case 1: // CHAR
        {
            convert_registers_to_char(&computer->rA, &computer->rX);
        }break;
        }
    }break;
    case SLA: {
        switch (instruction.instruction.mod.data) {
        case 0:{ // SLA
            unsigned int i;
            for (i = 0; i < instruction.instruction.v; i++) {
                shift_register_left(&computer->rA);
            }
        }break;
        case 1: { // SRA
            unsigned int i;
            for (i = 0; i < instruction.instruction.v; i++) {
                shift_register_right(&computer->rA);
            }
        }break;
        case 2: { // SLAX
            unsigned int i;
            for (i = 0; i < instruction.instruction.v; i++) {
                shift_two_registers_left(&computer->rA, &computer->rX);
            }
        }break;
        case 3: { // SRAX
            unsigned int i;
            for (i = 0; i < instruction.instruction.v; i++) {

            }
        }break;
        case 4: { // SLC
            unsigned int i;
            for (i = 0; i < instruction.instruction.v; i++) {
                shift_register_left_circular(&computer->rA);
            }
        }break;
        case 5: { // SRC
            unsigned int i;
            for (i = 0; i < instruction.instruction.v; i++) {
                shift_register_right_circular(&computer->rA);
            }
        }break;
        }
    }break;
    case MOVE: {
        unsigned int address = get_memory_address_from_index(computer->r1);
        if (address != 0) {
            mix_word reg = computer->memory[address];
            mix_word from = computer->memory[instruction.instruction.v];
            move_n_words(&reg, from, instruction.instruction.mod.data);

            computer->memory[address] = reg;
        }
    }break;
    default:
        break;
    }
    return 0;
}

int execute_code(mix_computer *computer) {
    int endOfCode = FALSE;
    while(!endOfCode) {
        mix_word instruction = computer->memory[computer->instructionIndex];

        if (instruction.word.A1.data == 0 ) {
            endOfCode = TRUE;
        } else {
            mix_code_instruction tmp;
            get_instruction_from_memory(instruction, &tmp);
            if (tmp.instruction.operation == HLT && tmp.instruction.mod.data == 2) {
                endOfCode = TRUE;
            } else {
                execute_instruction(computer,tmp);
            }
            computer->instructionIndex++;
        }
    }

    return 0;
}

void process_code_line(mix_computer *computer, char** code, mix_code_instruction *code_instruction, unsigned int address) {
    int i;
    int hasInstruction = FALSE;
    char *operationName = nullptr;
    mix_instruction instruction;
    int instruction_index = 0;
    char *label = nullptr;
    int arguments = 0;

    code_instruction->hasArgument = FALSE;
    code_instruction->hasLabel = FALSE;
    code_instruction->hasInstruction = FALSE;

    code_instruction->argument = "";

    for (i = 0; i < 3; i++) {
        if (hasInstruction == FALSE) {
            instruction = parse_instruction_code(code[i]);
            if (instruction.operation != -1) {
                hasInstruction = TRUE;
                instruction_index = i;
                operationName = code[i];
            } else { // It may be a label
                label = code[i];
            }
        } else {
            if (strcmp(code[i], "\n") != 0) {
                arguments = i;
            }
        }
    }

    if (instruction.operation != -1) {
        if (label != nullptr) {
            code_instruction->label = label;
            code_instruction->hasLabel = TRUE;

            if (instruction.operation == EQU) {

                mix_variable* newVariableList = (mix_variable*)
                        malloc((computer->variables_count + 1) * sizeof(mix_variable));
                unsigned int i;
                for (i = 0; i < computer->variables_count; i++) {
                    newVariableList[i] = computer->computer_variables[i];
                }

                computer->computer_variables = newVariableList;

                unsigned int intArg;
                char *end;
                intArg = (unsigned int) strtol(code[arguments], &end, 10);

                mix_variable var;
                var.name = label;
                var.value = intArg;
                computer->computer_variables[computer->variables_count] = var;

                computer->variables_count++;

            } else {
                label_address* newComputed = (label_address*)
                        malloc((computer->labels_count + 1) * sizeof(label_address));
                unsigned int i;
                for (i = 0; i < computer->labels_count; i++) {
                    newComputed[i] = computer->labels_computed[i];
                }

                computer->labels_computed = newComputed;

                label_address newAddress;
                newAddress.address = address + 1;
                newAddress.label = label;

                computer->labels_computed[computer->labels_count] = newAddress;
                computer->labels_count++;
            }


        }

        code_instruction->instruction = instruction;
        code_instruction->hasInstruction = TRUE;

        if (arguments != 0) {
            code_instruction->argument = code[arguments];
            code_instruction->hasArgument = TRUE;
        }
    }
}

mix_code_instruction* parse_code(mix_computer *computer, char ** code_buffer, unsigned int *lines) {

    mix_code_instruction* processed_code = (mix_code_instruction*)malloc(*lines * sizeof (mix_code_instruction));

    unsigned int i;
    unsigned int whiteLines = 0;
    for (i = 0; i < *lines; i++) {
        char *tmp =  trimwhitespace(code_buffer[i]);
        if (tmp[0] == '*' && tmp[1] == '*') {
            whiteLines++;
        } else if (tmp[0] == 0) {
            whiteLines++;
        }else {
            char *token;
            token = strtok(tmp, "\t ");
            char *parts[3];
            int partIndex = 0;
            while(token != nullptr) {
                parts[partIndex++] = token;
                token = strtok(nullptr, "\t ");
            }
            // If there are less than 3 parts
            if (partIndex < 4) {
                int i = 0;
                for (i = partIndex; i < 4; i++) {
                    parts[i] = "\n";
                }
            }
            process_code_line(computer, parts, &processed_code[i - whiteLines], i - whiteLines);
        }
    }
    *lines = *lines - whiteLines;
    return processed_code;
}

void asign_index_to_instruction(mix_computer *computer, mix_instruction *instruction) {
    instruction->index.data = 3;
}

void store_instruction_in_memory(mix_computer *computer,mix_word *r, mix_code_instruction instruction) {
    /*
     *       ------------------------------------------------
     *       |   0   |   1   |   2   |   3   |   4   |   5    |
     *        ------------------------------------------------
     *       |        ADDRESS        | INDEX |  MOD  | OPCODE |
     *        ------------------------------------------------
    */

    mix_register address = convert_int_to_register(instruction.instruction.v);

    asign_index_to_instruction(computer, &instruction.instruction);

    r->word.A1 = address.bytes.first;
    r->word.A2 =  address.bytes.second;

    r->word.I = instruction.instruction.index;
    r->word.F = instruction.instruction.mod;
    r->word.C.data = instruction.instruction.operation;

}

void get_instruction_from_memory(mix_word r, mix_code_instruction *instruction) {

    /*
     *       ------------------------------------------------
     *       |   0   |   1   |   2   |   3   |   4   |   5    |
     *        ------------------------------------------------
     *       |        ADDRESS        | INDEX |  MOD  | OPCODE |
     *        ------------------------------------------------
    */

    instruction->instruction.operation = r.word.C.data;
    instruction->instruction.mod = r.word.F;

    mix_register address;
    address.bytes.first = r.word.A1;
    address.bytes.second = r.word.A2;
    instruction->instruction.v = abs(convert_register_to_int(address));
    instruction->instruction.index = r.word.I;
}

void compile_code(mix_computer *computer, char **code_buffer, unsigned int lines) {

    computer->labels_count = 0;
    computer->variables_count = 0;
    mix_code_instruction* processed_code = parse_code(computer, code_buffer, &lines);

    computer->instructionIndex = 1; // lets leave the 0 for null

    unsigned int i;
    for (i = computer->instructionIndex; i <= lines; i++) {
        parse_instruction(computer, &processed_code[i - 1]);
        store_instruction_in_memory(computer,&computer->memory[i], processed_code[i - 1]);
    }
}
