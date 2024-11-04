#include "clac_string.h"

// defines a scanner type for working with strings
// stores the position of the next character to be scanned
// allows peeking multiple characters ahead
typedef struct SCANNER {
    string src;
    uint pos;
    uint peek;
} scanner;




// SCANNER FUNCTION DECLARATIONS

// creates a scanner for the string `s`
scanner scan_create(string s);
// gets the next character from scanner `s`
// returns -1 if the scanner does not have a next (check for this case first)
char scan_next(scanner s);
// peeks ahead to get a single character from scanner `s`
// does not advance `s`, but does advance a peek position for repeat calls
char scan_peek(scanner s);
// advances the scanner by up to `n` characters
// stops early if the scan reaches the end of the source string
// returns the string scanned along the way
string scan_nNext(scanner s, uint n);
// peeks up to `n` characters ahead
// stops early if the scan reaches the end source string
// returns the string scanned along the way
string scan_nPeek(scanner s, uint n);
// gets the previous character scanned by `s`
// returns `-1` if the scanner does not have a previous (should check for this case first)
char scan_prev(scanner s);
// checks that `s` has more characters to scan
bool scan_checkNext(scanner s);
// checks that `s` can peek another character ahead
bool scan_checkPeek(scanner s);
// checks that `s` has a previously scanned character
bool scan_checkPrev(scanner s);
// gives the number of remaining characters in the scanner
uint scan_checkDist(scanner s);
// skips past any characters in `s` that match those in `set`
// `set` should be a minimal string without repeat characters, but the function will work with any string
// if `set` is empty, then the scanner will simply skip to the end
void scan_skip(scanner s, string set);
// advances `s` until any of the characters in `set` are reached
// returns the string scanned up to the first delimiter character
// if none of these characters remain in the string, will scan to the end
// advances the scanner to past the first instance of a delimiter
string scan_until(scanner s, string set);
// scans in `s` until a given sequence is reached
// returns the sequence scanned up to the first instances of `seq`
// if no such sequence remains in the string, scans to the end
// advances the scanner to past the first instance of `seq`
string scan_match(scanner s, string seq);




// FUNCTION DEFINITIONS

scanner scan_create(string s) {
    scanner sc = { s, 0, 0 };
    return sc;
}

char scan_next(scanner s) {
    s.peek = 0; // reset this
    if(!scan_checkNext(s)) return -1;
    return s.src.chars[s.pos++];
}

char scan_peek(scanner s) {
    if(!scan_checkPeek(s)) return -1;
    return s.src.chars[s.pos + s.peek++];
}

string scan_nNext(scanner s, uint n) {
    uint start = s.pos;
    s.pos = scan_checkDist(s) < n ? s.pos + n : s.src.len;
    return string_slice(s.src, start, s.pos);
}

string scan_nPeek(scanner s, uint n) {
    uint start = s.pos + s.peek;
    s.peek = s.pos + s.peek + n < s.src.len ? s.peek + n : s.src.len - s.pos;
    return string_slice(s.src, start, s.pos + s.peek);
}

char scan_prev(scanner s) {
    if(!scan_checkPrev(s)) return -1;
    return s.src.chars[s.pos - 1];
}

bool scan_checkNext(scanner s) {
    return s.pos < s.src.len;
}

bool scan_checkPeek(scanner s) {
    return s.pos + s.peek < s.src.len;
}

bool scan_checkPrev(scanner s) {
    return s.pos > 0;
}

uint scan_checkDist(scanner s) {
    return s.pos - s.src.len;
}

void scan_skip(scanner s, string set) {
    while(scan_checkNext(s) && string_containsChar(set, scan_next(s))) {
        // advance
    }
}


string scan_until(scanner s, string set) {
    uint start = s.pos;
    while(scan_checkNext(s) && !string_containsChar(set, scan_next(s))) {
        // advance
    }
    return string_slice(s.src, start, s.pos);
}

string scan_match(scanner s, string seq) {
    long idx = string_match(s.src, seq);
    uint prior_pos = s.pos;   

    s.pos = idx == -1 ? s.src.len : idx + seq.len;

    return string_slice(s.src, prior_pos, s.pos);
}

