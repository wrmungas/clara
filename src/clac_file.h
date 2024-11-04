#include "clac_string.h"
#include <stdio.h>

#include "clac_utils.h"


// defines some functions for reading and writing file contents as strings


// Opens a file with the name `filename`
// Reads its contents to a string
// Closes the file
string read_file(string filename) {
    FILE *fp = fopen( C(filename), "r");
    if(!fp) {
        string message = string_concat( S("Failed to open file for read: "), filename);
        fatal(message);
    }
    
    uint size = 0;
    uint capacity = 1000;
    char *buf = (char*)malloc(capacity * sizeof(char));

    char c = fgetc(fp);
    while(c != EOF) {
        if(size == capacity) {
            capacity *= 2;
            buf = (char*)realloc(buf, capacity);
        }
        buf[size++] = c;
        c = fgetc(fp);
    }
    buf[size] = '\0';
    string result = S(buf);
    free(buf);
    fclose(fp);
    return result;
}

// Writes the string `contents` to a file with the name `filename`
// Opens and closes the file before writing
// Will create if it does not yet exist
void write_file(string filename, string contents) {
    FILE *fp = fopen( C(filename), "w");
    if(!fp) {
        string message = string_concat( S("Failed to open file for write: "), filename);
        fatal(message);
    }
    uint bytes = fwrite(contents.chars, sizeof(char), contents.len, fp);
    
    if(bytes < contents.len) {
        string message = string_concatInt( S("Only wrote ["), bytes);
        string_concat(message, S("] bytes ( of ["));
        string_concatInt(message, contents.len); 
        string_concat(message, S("] ) to file: "));
        string_concat(message, filename);
        string_concat(message, S("\n"));
        
        fatal(message);
    }
    fclose(fp);
}

