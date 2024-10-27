# Clara
files and code relevant to my project 'clara'

This is an overview of the project; look to the wiki for greater detail.

## Project Overview

### What is Clara?
Clara is a project based on some ideas of mine, in two parts. The primary portion of the project is a set of extensions to C code. The secondary portion of this project is the specification of a distinct language that carries these extensions to a logical extreme, codifying them into features.

The features are created around the following principles:
- C-derived: maintain some of the best qualities of C (directness, readability, simplicity) and the qualities of procedural, machine and data oriented styles
- clarity: code should be readable and understandable. The name of this project comes from this principle and the fact that it builds off of C
- adaptability: code should tend towards some robustness while making consistent guarantees
- modularity: code should be able to be organized into modular sets of functions and datatypes, without being overly verbose

### Part I: Extensions to the C programming language
Provide a set of simple extensions to C language. These should be simple enough to be implemented as a separate program (a kind of pre-preprocessor) that will translate the Clara extensions directly into C code. These extensions should include:

- Namespaces: support for including functions and structs within a namespace to improve code modularity. If possible, support namespace resolution when the namespace is not explicitly included. Additional extension includes a 'using' block that brings a namespace into the local scope to simplify namespace resolution. When resolving to C code, a namespaced type or function should be annotated to create a unique name
- Advanced function naming: allow functions with different arguments/return types but the same name. When resolving to C code, annotate function names with their arguments to create unique names. The exact function is picked based on evaluation of the return and argument types


### Part II: A Distinct Language
This portion would be a longer-term project adapting more extensive changes and features into a distinct language entirely. Some difficulty of compilation could be avoided by retaining use of C as an intermediate language: compiling Clara to C, then compiling the C code. Features for consideration include:

- Modules: higher-level version of namespaces. May not have a particular use case with namespaces existing, but I like the name 'modules'. It may be that 'module' is term I settle on for the feature, and 'namespace' is the internal term for it's included functions / types
- dot syntax for namespace resolution e.g. `Vec3.normalize()`
- Improved 'use' or 'using' keyword that creates a sub-scope that uses the given module namespace
- Function behavior: Functions that take a single anonymous struct / tuple (args) and return another (results)
- Type interfaces / traits: similar to what I think Rust does, although I don't know Rust very well. A type is considered to implement an interface/trait if the functions it requires are defined, and if the struct matches the required member names
- Memory-focused type building: build all types off the premise of two extremely simple primitives: bytes and blocks of bytes. Build other functionality off of these types. All types are structs!
- Explicit arrays and tuples: support for static arrays as a kind of struct by default rather than a pointer, and tuples as anonymous structs (unnamed groupings of arbitrary data). Static arrays have size known by the compiler and can be type-checked more strongly
- Functional types: clean syntax for a function pointer and function type. Some support for functional programming like lambdas. e.g. `function name(args):(return) {}`
- Struct member aliasing: allow the same data block in a struct type to be accessed via different names (and perhaps different types)
- References: like in C++, cleaner syntax than pointers

Additional features I'm toying with the idea of:
- Modules: functions can have restrictable visibility through a keyword like 'internal' or perhaps 'helper' (maybe replaces 'function')
- Shorthand keywords: 'fun' for function, 'mod' for module, 'dec' for declare. In fact, let types have explicit full names and shorthands too, why not? 'int' for integer32, 'short' for integer16, 'long' for integer64, 'float' for floating32, 'double' for floating64, 'bool' for boolean, 'uint' for unsigned32, etc
- File names: .clara, .cl, .cla 
- Module files: .clmod, .clmodule, .clam (I like clam)
- Pointer syntax: cleaner, use <type>@ for pointer types, `$()` for address-of and `@()` for dereference operations ( perhaps shorthand for `addressof()` and `dereference()`, and parentheses may ultimately be optional with these as keywords)

## Who I am
I am currently a junior in college learning computer science. My motivations for this project are to push myself as a programmer and create a C extension or unique language that provides additional features I want with a syntax I like. I do not think there is anything wrong with C, or with a number of other languages that may do these features better than I will (in fact I suspect I'll end up creating some subset of C++ with cleaner syntax and some Rust features). I am simply interested in trying to expand from its limitations and have fun learning and exploring how language design and compilation work. I am not trying to be particularly serious (at least at the time of writing this); I am a creative person who enjoys optimization puzzles, and I just want to have fun and learn something new.

