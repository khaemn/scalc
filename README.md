# scalc

The Scalc stands for SetCalculator - a tiny program, that could compute various algebraic operations on sets of positive integers.

Each set should be given as a text file, each line contains a single integer.
Up to 1M numbers per file allowed. May be more - I haven't tested it though.

### Usage

Presume you have files a.txt, b.txt, c.txt with some numbers of positive (or zero) integers in them,
a one number per line.

Running

```
$ ./scalc [ INT a.txt b.txt c.txt ]
```

would print an intersection of sets, described in the corresponding files, e.g. all elements, that are
available in every set.

The expression can include several operations:

```
$ ./scalc [ INT [ DIFF a.txt b.txt ] c.txt SUM [ a.txt c.txt ] ]
```

To print out more verbose output (useful for debugging the expressions) use an `l` key:

```
$ ./scalc l [ INT [ DIFF a.txt b.txt ] c.txt SUM [ a.txt c.txt ] ]
```

### Supported commands

`INT` - intersection, returns values that are present in all argument files / sets.
    
    INT [1, 3, 5] [ 2, 3, 4] == [3]
    
`DIF` - difference, returns only elements that are unique across the given sets (e.g. are present in only one of the sets)

    DIF [1, 3, 5] [ 2, 3, 4] == [1, 2, 4, 5]

`SUM` - sum, returns all elements that are present in any of the given sets

    SUM [1, 3, 5] [ 2, 3, 4] == [1, 2, 3, 4, 5]
   
`EQ n` (where `n` is a positive integer) - returns elements, that are found exactly in `n` of given sets.

    EQ 1 [1, 3, 5] [ 2, 3, 4] == [1, 2, 3, 4, 5] (actually, EQ 1 is the same as DIF)
    
    EQ 2 [1, 3, 5] [ 2, 3, 4] == [3] (actually, EQ n where n is the number of sets is the same as INT)
    
    EQ 3 [1, 3, 5] [ 2, 3, 4] == []
    
`GR n` (where `n` is a positive integer) - returns elements, that are found at least in `n` of given sets.

    GR 0 [1, 3, 5] [ 2, 3, 4] == [1, 2, 3, 4, 5] (actually, GR 0 is the same as SUM)
    
    GR 1 [1, 3, 5] [ 2, 3, 4] == [3]
    
    GR 2 [1, 3, 5] [ 2, 3, 4] == []
    
`LE n` (where `n` is a positive integer) - returns elements, that are found in less than `n` of given sets.

    LE 2 [1, 3, 5] [ 2, 3, 4] == [3]
    
    LE 100 [1, 3, 5] [ 2, 3, 4] == [1, 2, 3, 4, 5]
    
    LE 1 [1, 3, 5] [ 2, 3, 4] == []

### Expression syntax

* An expression is expected as a series of command line arguments when calling the `scalc` executable.
* Supported lexems: `[`, `]`, all commands, positive integers, valid filenames.
* All lexems are supposed to be separated with exactly one space ` ` character.
* An expression must start with `[` and end with `]`. Any opening bracket must have a corresponding closing one.
* Use `l` as the first command line argument to enable explicit logging.

### Build prerequisites

* CMake >3.0
* C++11 compiler
* (optional) Python3 for generating the test data

### Build

Run `build.sh`, observe a test output.
