#include "clac_utils.h"
#include "clac_file.h"

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

bool strdebug = false;

string *source_files;
uint num_files;

void match_args(string arg);
void init_all();
void print_options();

// compiler for clara
int main(int argc, char **argv) {
    init_all();

    if(argc < 3) {
        fatal( S("Usage: clac <options> ... <infile> <outfile>\nDo clac -h to list options\n") );
    }
    
    string h = string_from("-h");
    string help = string_from("-help");

    for(int i = 1; i < argc - 2; i++) {
        string argvi = S(argv[i]);
        if(string_equals( argvi, h ) || string_equals(argvi, help)) {
            print_options();
        }
        match_args(argvi);
    }
    string infile = S(argv[argc - 2]);
    
    string_print(string_concat(S("Reading file: "), infile));
    string source_file = read_file(infile);

    string outfile = S(argv[argc - 1]);
    string_print(string_concat(S("Writing to file: "), outfile));
    write_file( outfile, source_file);

}

// helper functions

void match_args(string arg) {
    partial = string_equals(arg, S("-p")) || string_equals(arg, S("-part"));
    one_c_file = string_equals(arg, S("-m")) || string_equals(arg, S("-mono"));
    list_steps = string_equals(arg, S("-l")) || string_equals(arg, S("-loud"));

    list_parse = string_equals(arg, S("-parse"));
    list_tokens = string_equals(arg, S("-tokens"));

    list_types = string_equals(arg, S("-types"));
    list_functions = string_equals(arg, S("-funcs"));
    list_modules = string_equals(arg, S("-mods"));
    list_full = string_equals(arg, S("-full"));
    strdebug = string_equals(arg, S("-strdebug"));
}

void init_all() {
    source_files = NULL;
    num_files = 0;
    strings_init();
}

void print_options() {
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
    printf("-strdebug    (String debug):\n    - list debug info for clac string library");
    exit(0);
}

