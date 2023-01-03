# ulamspiral

<img src="/spiral.png" align="right" />

ulamspiral is a CLI tool for creating a PNG image of an [Ulam spiral](https://en.wikipedia.org/wiki/Ulam_spiral).

Conforms to POSIX.1-1990 and ISO C (1990).

## Dependencies

* make
* clang

## Installation

```shell
git clone https://github.com/toxeeec/ulamspiral
cd ulamspiral
make release
```

## Usage

`./spiral <FILE_NAME> <WIDTH>`

## Flags

`-f, --force` If a file exists, overwrite it.  
`-p color, --primary=color` A primary color of the image.  
`-s color, --secondary=color` A secondary color for the image.  

## Colors

A color format is either short or long.

A short format is RGB, each letter is a valid hex number between 0 and F.  
Each number gets replicated (e. g. F becomes FF).

A long format is RRGGBB, each value is a valid hex number between 00 and FF.

The default colors are white for primary and black for secondary.
