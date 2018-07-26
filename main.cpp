#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "memory.h"
#include "ctype.h"
#include "math.h"
#include "mix_computer.h"
#include "mix_instructions.h"
#include "mix_compiler.h"

int read_code_file(const char* file, char ** code_buffer, unsigned int *lines) {
    int result = 0;


    FILE* fp;

    if ((fp = fopen(file, "r")) == NULL) { /* Open source file. */
        perror("fopen source-file");
        return 1;
    }

    int line = 0;
    code_buffer[line] = (char*)malloc(BUFSIZE);
    while (fgets(code_buffer[line], BUFSIZE, fp) != NULL) {
        line++;
        code_buffer[line] = (char*)malloc(BUFSIZE);
    }
    *lines = line;

    fclose(fp);

    return result;
}

int main()
{
    mix_computer computer;
    init_computer(&computer);

    char* code_buffer[MAX_CODE_LINES];
    unsigned int lines = 0;
    read_code_file("C:\\Users\\pablo.cruz\\Documents\\mix\\code.txt", code_buffer, &lines);

    compile_code(&computer, code_buffer, lines);
    execute_code(&computer);

    int i;
    for (i = 0; i < 27; i++) {
        print_memory(computer.memory, i);
    }
    printf("      .\n");
    printf("      .\n");
    print_memory(computer.memory, 2000);
    print_memory(computer.memory, 2001);
    print_memory(computer.memory, 2002);
    print_memory(computer.memory, 2003);

    print_computer(&computer);

    return 0;
}
