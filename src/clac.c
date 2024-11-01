#include "clac_utils.h"

bool partial = false;
bool one_c_file = false;
bool list_steps = false;
// readouts for parsing stages
bool list_parse = false;
bool list_tokens = false;
// readouts for type structures
bool list_types = false;
bool list_functions = false;
bool list_modules = false;
bool list_full = false;

void match_args(string arg);

// compiler for clara
int main(int argc, char **argv) {
    init_all();

    if(argc < 2) {
        fatal("Usage: clac <options> ... <file>\nDo clac -h to list options\n");
    }
    
    string h = string_from("-h");
    string help = string_from("-help");

    for(int i = 1; i < argc; i++) {
        string argvi = string_from(argv[i]);
        if(string_equals( argvi, h ) || string_equals(argvi, help)) {
            printf("Options:\n-----------------------\n");
            printf("-h, -help    (Help):\n   - print these options\n\n");
            printf("-p, -part    (Partial compilation):\n   - compile to C code (do not remove temporary files)\n\n");
            printf("-m, -mono    (Unary compilation):\n   - compile all source code to a single file\n\n");
            printf("-l, -loud    (Loud compilation):\n   - compile with progress printouts, list generated files\n\n");
            printf("Listing options:\n------------------------\n");
            printf("-parse       (Parse step):\n   - list parsing step results\n\n");
            printf("-tokens      (Token step):\n   - list tokenization step results\n\n");
            printf("-types       (Type listing):\n   - list all user types\n\n");
            printf("-funcs       (Function listing):\n   - list all user functions\n\n");
            printf("-mods        (Module listing):\n   - list all user modules and namespaces/sub-namespaces\n\n");
            printf("-full        (Full module listing):\n   - list complete user types, functions, modules, and namespaces\n\n");
            exit(0);
        }
        match_args(argvi);
    }
    

    free_all(false);
}

void match_args(string arg) {
    partial = string_equals(arg, string_from("-p")) || string_equals(arg, string_from("-part")) ? true : partial;
    one_c_file = string_equals(arg, string_from("-m")) || string_equals(arg, string_from("-mono")) ? true : one_c_file;
    list_steps = string_equals(arg, string_from("-l")) || string_equals(arg, string_from("-loud")) ? true : list_steps;

    list_parse = string_equals(arg, string_from("-parse")) ? true : list_parse;
    list_tokens = string_equals(arg, string_from("-tokens")) ? true : list_tokens;

    list_types = string_equals(arg, string_from("-types")) ? true : list_types;
    list_functions = string_equals(arg, string_from("-funcs")) ? true : list_functions;
    list_modules = string_equals(arg, string_from("-mods")) ? true : list_modules;
    list_full = string_equals(arg, string_from("-full")) ? true : list_full;
}

void init_all() {
    strings_init();
}

void free_all(bool info) {
    strings_free(info, false);
}