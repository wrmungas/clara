#ifndef CLAC_STRING
#define CLAC_STRING

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "clac_types.h"

// defines a string type and functions for working with strings
// these strings store their own length and are designed to be slightly more 
// flexible than straight char arrays.
//
// They also benefit from a kind of self-garbage collection, in that all strings 
// of this type are stored in a set of automatically managed blocks of memory that 
// are initialized once at the start of the program and freed once at the end.
// All string contents are pointers inside these blocks, making using strings faster 
// and safer than default C

// string struct 
typedef struct STRING {
    char *chars;
    uint len;
} string;

#define S(literal) string_from(literal)
#define C(str) string_toCString(str)


// string memory bookkeeping globals
// originally had an explicit memory free function to call at program end,
// but it resulted in a double free error and I'm not sure why
//
// now I'm just letting the OS handle that part on program finish
#define INITIAL_SIZE 1000
#define INITIAL_BLOCKS 1
static ulong block_size;
static string *blocks; // use the string struct to store blocks and size together
static ulong num_blocks;
static uint block_idx;
static ulong bytes_used;
static char *last_alloc;
static char *before_last_alloc;

/* STRING MEMORY FUNCTION DECLARATIONS */

// initialize the string memory blocks (CALL AT PROGRAM START)
void strings_init();
// get a pointer to space for `size` characters in the string memory blocks
static char *string_alloc(ulong size);
// creates a string of length `len` and allocates space for its characters
static string string_create(uint len);
// for debugging, print out memory bookkeeping
// if `contents` is true, prints out the contents of each memory block
void strings_print(bool contents);


/* STRING MANIPULATION FUNCTION DECLARATIONS - PUBLIC */

// allocates a string matching the given null-terminated char array in `str`
string string_from(const char *str);
// creates a string from the single character `c`
string string_fromChar(char c);
// allocates a string matching the first `n` characters from the given char array in `str`
// the string will be cut off before `n` characters if it encounters null-termination
//
// to slice the `str` c-string pass a pointer offset into it
string string_sizedFrom(const char *str, int n);
// returns a newly-allocated copy of the string `src`
// if you don't intend to modify either `src` or the copy, don't use this function;
// instead simply assign your new string to `src` directly
string string_copy(const string src);
// Checks if the `src1` string equals the `src2` string
bool string_equals(string src1, string src2);
// returns a new string formed by concatenating `src1` and `src2`
string string_concat(string src1, string src2);
// checks if the `src` string contains the character `c`
bool string_containsChar(string src, char c);
// Checks if the `src` string contains the sequence in `seq`
bool string_contains(string src, string seq);
// Similar to `contains()`, checks if the `src` string contains the sequence in `pattern`
// if it does, returns the index at which `pattern` first occurs
long string_match(string src, string pattern);
// returns a string slice from `src` 
// slices from the index `start` (inclusive) to the index `end` (exclusive)
// works if `start` is after `end`
// returns an empty string if `start` equals `end` or either is out of bounds of the string
string string_slice(string src, uint start, uint end);
// concatenates the integer `i` as a string to the end of `src`
string string_concatInt(string src, long i);
// concatenates the floating value `f` as string to the end of `src`, with the `frac_width` decimal places
string string_concatFloat(string src, double f, uint frac_width);
// prints the string `src` to stdout without newlines or formatting
void string_print(string src);
// gives a null-terminated char array from a string
char *string_toCString(string src);







/* STRING MEMORY FUNCTION DEFINITIONS */

void strings_init() {
    block_size = INITIAL_SIZE;
    num_blocks = INITIAL_BLOCKS;
    block_idx = num_blocks - 1;

    blocks = (string*)malloc(num_blocks * sizeof(string));
    blocks[block_idx].chars = (char*)malloc(block_size);
    blocks[block_idx].len = block_size;

    bytes_used = 0;
    last_alloc = NULL;
    before_last_alloc = NULL;
}

static char *string_alloc(ulong size) {
    if(bytes_used + size < block_size) {
        ulong idx = bytes_used;
        bytes_used += size;
        before_last_alloc = last_alloc;
        last_alloc = blocks[block_idx].chars + idx;
        return last_alloc;
    }
    // grow the array of pointers
    block_idx = num_blocks++;
    ulong blocks_size = num_blocks * sizeof(string*);
    blocks = (string*)realloc(blocks, blocks_size);

    // if the requested size is too big for a current block, resize blocks from here on out
    if(size > block_size) {
        block_size = size * 2;
    }
    blocks[block_idx].chars = (char*)malloc(block_size);
    blocks[block_idx].len = block_size;

    bytes_used = size;
    before_last_alloc = last_alloc;
    last_alloc = blocks[block_idx].chars;
    return last_alloc;
}

static string string_create(uint len) {
    string s = {string_alloc(len), len};
    return s;
}


void strings_print(bool contents) {
    printf("%lu block%s:\n", num_blocks, num_blocks > 1 ? "s" : "");
    for(int i = 0; i < num_blocks; i++) {
        printf("[%d]: [%d] bytes\n", i, blocks[i].len);
        if(contents) {
            printf(":\n{\n");
            string_print(blocks[i]);
            printf("\n}\n");
        }
    }
}




/* STRING MANIPULATION FUNCTION DEFINITIONS */

string string_from(const char *str) {
    uint len = strlen(str);

    string s = string_create(len);
    for(int i = 0; i < len; i++) {
        s.chars[i] = str[i];
    }
    return s;
}

string string_fromChar(char c) {
    uint len = 1;
    string s = string_create(len);
    s.chars[0] = c;
    return s;
}

string string_sizedFrom(const char *str, int n) { 
    int len = 0;
    int i = 0;
    while(i < len && str[i]) {
        len++;
        i++;
    }
    string s = string_create(len);
    for(i = 0; i < len; i++) {
        s.chars[i] = str[i];
    }
    return s;
}

string string_copy(const string src) {
    string result = string_create(src.len);
    for(int i = 0; i < src.len; i++) {
        result.chars[i] = src.chars[i];
    }
    return result;
}

bool string_equals(string src1, string src2) {
    if(src1.len != src2.len) {
        return false;
    }
    for(int i = 0; i < src1.len; i++) {
        if(src1.chars[i] != src2.chars[i]) return false;
    }
    return true;
}

string string_concat(string src1, string src2) {
    int len = src1.len + src2.len;

    if(src1.chars == before_last_alloc && src2.chars == last_alloc) {
        string res = { src1.chars, len };
        return res;
    }

    string res = string_create(len);

    for(int i = 0; i < len; i++) {
        res.chars[i] = i < src1.len ? src1.chars[i] : src2.chars[i - src1.len];
    }

    return res;
}

string string_concatC(string src, const char *str) {
    int len = src.len + strlen(str);
    if(src.chars == last_alloc && bytes_used + len < block_size) {
        string_from(str);
        string result = { src.chars, len };
        return result;
    }
    return string_concat(src, string_from(str));
}

bool string_contains(string src, string seq) {
    return string_match(src, seq) > -1; 
}

long string_match(string src, string seq) {
    if(src.len < seq.len) {
        return -1;
    }

    for(int i = 0; i < src.len - seq.len + 1; i++) {
        int j = i;
        int k = 0;

        while(src.chars[j] == seq.chars[k] && j < src.len && k < seq.len) {
            j++;
            k++;
        }
        if(k == seq.len) return i;
    }

    return -1;


}

string string_slice(string src, uint start, uint end) {
    if(start >= src.len || start < 0) {
        return string_from("");
    }
    if(end > src.len || end < -1) {
        return string_from("");
    }
    if(start == end) {
        return string_from("");
    }
    if(start < end) {
        // if it is a forward slice of an existing string just reuse that space instead of allocating new
        int len = end - start;
        string result = { src.chars + start , len };
        return result;
    }
    // allow users to slice backwards too, requires allocation
    int len = start - end;
    string result = string_create(len);
    for(int i = 0; i < len; i++) {
        result.chars[i] = src.chars[start - i];
    }
    return result;
}

string string_concatInt(string src, long i) {
    uint size = (int)ceil(log10(i));
    size += i < 0 ? 1 : 0; // account for negative sign

    char *intstr = (char*)malloc(size);
    sprintf(intstr, "%ld", i);
    
    string result = string_concat(src, string_from(intstr));
    free(intstr);
    return result;
}

string string_concatFloat(string src, double f, uint frac_width) {
    int total_width = frac_width + (int)ceil(log10(f));
    total_width += (f < 0) ? 2 : 1; // account for decimal point and negative sign
    
    char *floatstr = (char*)malloc( total_width );
    sprintf(floatstr, "%*.*f", total_width, frac_width, f);

    string result = string_concat(src, string_from(floatstr));
    free(floatstr);
    return result;
}

void string_print(string src) {
    for(uint i = 0; i < src.len; i++) {
        putc(src.chars[i], stdout);   
    }
    putc('\n', stdout);
    fflush(stdout);
}

char *string_toCString(string src) {
    char *str = string_alloc(src.len + 1);
    uint i = 0;
    while(i < src.len) {
        str[i] = src.chars[i];
        i++;
    }
    str[i] = '\0';
    return str;
}

#endif