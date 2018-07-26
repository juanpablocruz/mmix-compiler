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

int main(int argc, const char * argv[])
{
    mix_computer computer;
    init_computer(&computer);

    char* code_buffer[MAX_CODE_LINES];

    if (argc < 3) {
        printf("Usage mix -f [file]\n");
        return -1;
    }

    int i;
    unsigned int dump_memory = FALSE;
    const char *sourcefile;
    for (i = 0; i < argc; i++) {
        if ( strcmp(argv[i], "-f") == 0) {
            sourcefile = argv[i+1];
            i++;
        }
        if ( strcmp(argv[i], "-d") == 0) {
            dump_memory = TRUE;
        }
    }

    unsigned int lines = 0;
    read_code_file(sourcefile, code_buffer, &lines);

    compile_code(&computer, code_buffer, lines);
    execute_code(&computer);


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
