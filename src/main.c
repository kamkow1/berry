#include "berry.h"
#include "be_repl.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(__linux) || defined(__unix) || defined(__APPLE__)
#include <readline/readline.h>
#include <readline/history.h>
#endif

static void dofile(bvm *vm, const char *fname)
{
    char buffer[4096];
    FILE *fp = fopen(fname, "r");
    if (fp) {
        int res;
        size_t len = fread(buffer, 1, sizeof(buffer), fp);
        
        buffer[len] = '\0';
        fclose(fp);
        res = be_loadstring(vm, buffer) || be_pcall(vm, 0);
        if (res) {
            printf("%s\n", be_tostring(vm, -1));
        }
    } else {
        printf("error: can not open file '%s'.\n", fname);
    }
}

static const char* get_line(const char *prompt)
{
#if defined(__linux) || defined(__unix) || defined(__APPLE__)
    const char *line = readline(prompt);
    add_history(line);
    return line;
#else
    static char buffer[1000];
    printf("> ");
    if (fgets(buffer, sizeof(buffer), stdin)) {
        return buffer;
    }
    return NULL;
#endif
}

int main(int argc, char *argv[])
{
    bvm *vm = be_newvm(32);
    be_loadlibs(vm);
    srand((unsigned)time(NULL));
    if (argc >= 2) {
        dofile(vm, argv[1]);
    } else {
        be_repl(vm, get_line);
    }
    return 0;
}
