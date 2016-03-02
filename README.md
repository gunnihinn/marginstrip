# Marginpar stripper

Strip `\marginpar` and `\marginparhere` from LaTeX documents. The
output will always compile to the same document as the original source,
minus the marginpar notes.

## Use

`marginstrip [OPTION]... FILE...`

If no `FILE` is given, `marginstrip` reads from standard input.

Options:

    -h, --help      Print this help and exit
    -v, --version   Print version info and exit

## Installation

This is a `C` program and needs to be compiled to be run. Any UNIX-based
system should have a `C` compiler, so to compile and install the program
you should only need to run:

    make install

The program will be installed to `$HOME/bin` by default. If this
directory doesn't exist or isn't in your `$PATH`, either create it and
add to your path or edit the first line of the `Makefile` to install it
in a better place.

## Bugs

Maybe. If you find any, please report them to the author. The program
comes with a test suite you can run with:

    make test

If you find a bug, it would be greatly appreciated if you could make a
minimal example that exposes it. That way we can add it to the test
suite and be sure it never rises again.

The program tests live in the `test` folder. Each test is a pair
`inputN.tex` and `expectN.tex`, where `N` is a positive integer. The
file `inputN.tex` contains the input to the program, while `expectN.tex`
contains what we expect the program to return. If the program output
isn't equal to the contents of `expectN.tex`, the test runner will
complain.
        
## Author

    Gunnar Þór Magnússon <<gunnar@msp.org>>
