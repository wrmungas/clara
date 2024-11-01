#ifndef CLAC_STRING
#define CLAC_STRING

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "clac_types.h"

// string struct 
typedef struct {
    char *chars;
    uint len;
} string;


// string memory bookkeeping globals
#define BLOCK_SIZE 10000
#define INITIAL_BLOCKS 1
static char **blocks;
static ulong num_blocks;
static char *current_block;
static ulong current_used;

/* STRING MEMORY FUNCTION DECLARATIONS */

// initialize the string memory blocks
void strings_init();
// free all string blocks (CALL AT END OF PROGRAM)
void strings_free(bool print, bool fullprint);
// get a pointer to space for `size` characters in the string memory blocks
static char *string_alloc(ulong size);
// creates a string of length `len` and allocates space for its characters
static string string_create(uint len);
// for debugging, print out memory bookkeeping
// if `contents` is true, prints out the contents of each memory block
static void strings_print(bool contents);


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
// returns a copy of the string `src`
string string_copy(const string src);
// Checks if the `src1` string equals the `src2` string
bool string_equals(string src1, string src2);
// returns a new string formed by concatenating `src1` and `src2`
string string_concat(string src1, string src2);
// Checks if the `src` string contains the sequence in `seq`
bool string_contains(string src, string seq);
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





/* STRING MEMORY FUNCTION DEFINITIONS */

void strings_init() {
    blocks = (char**)malloc(INITIAL_BLOCKS * sizeof(char*));
    num_blocks = INITIAL_BLOCKS;
    blocks[0] = (char*)malloc(BLOCK_SIZE);

    current_block = blocks[num_blocks - 1];
    current_used = 0;
}

static char *string_alloc(ulong size) {
    if(current_used + size < BLOCK_SIZE) {
        ulong idx = current_used;
        current_used += size;
        return &(current_block[idx]);
    }
    num_blocks++;
    ulong blocks_size = num_blocks * sizeof(char*);
    blocks = (char**)realloc(blocks, blocks_size);
    blocks[num_blocks - 1] = (char*)malloc(BLOCK_SIZE);
    current_block = blocks[num_blocks - 1];
    current_used = 0;
    return &(current_block[0]);
}

static string string_create(uint len) {
    string s = {string_alloc(len), len};
    return s;
}

void strings_free(bool print, bool fullprint) {
    if(print) strings_print(fullprint);

    for(int i = 0; i < num_blocks; i++) {
        free(blocks[i]);
    }
    free(blocks);
}

static void strings_print(bool contents) {
    printf("%lu blocks:\n", num_blocks);
    for(int i = 0; i < num_blocks; i++) {
        printf("[%d]: [%d] bytes\n", i, BLOCK_SIZE);
        if(contents) printf(":\n{\n%*s\n}\n", BLOCK_SIZE, blocks[i]);
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
    string res = string_create(len);

    for(int i = 0; i < len; i++) {
        res.chars[i] = i < src1.len ? src1.chars[i] : src2.chars[i - src1.len];
    }

    return res;
}

bool string_contains(string src, string seq) {
    if(src.len > seq.len) {
        return false;
    }

    for(int i = 0; i < src.len - seq.len + 1; i++) {
        int j = i;
        int k = 0;

        while(src.chars[j] == seq.chars[k] && j < src.len && k < seq.len) {
            j++;
            k++;
        }
        if(k == seq.len) return true;
    }

    return false;
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
        int len = end - start;
        string result = string_create(len);
        for(int i = 0; i < len; i++) {
            result.chars[i] = src.chars[start + i];
        }
        return result;
    }
    // allow users to slice backwards too
    int len = start - end;
    string result = string_create(len);
    for(int i = 0; i < len; i++) {
        result.chars[i] = src.chars[start - i];
    }
    return result;
}

string string_concatInt(string src, long i) {
    uint size = (int)ceil(log10(i));
    size = i < 0 ? size + 1 : size;

    char *intstr = (char*)malloc(size);
    sprintf(intstr, "%ld", i);
    
    string result = string_concat(src, string_from(intstr));
    free(intstr);
    return result;
}

string string_concatFloat(string src, double f, uint frac_width) {
    int total_width = frac_width + (int)ceil(log10(f)) + (f < 0) ? 2 : 1;
    char *floatstr = (char*)malloc( total_width );
    sprintf(floatstr, "%*.*f", total_width, frac_width, f);

    string result = string_concat(src, string_from(floatstr));
    free(floatstr);
    return result;
}

void string_print(string src) {
    printf("%*s", src.len, src.chars);
}

#endif