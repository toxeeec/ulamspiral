<h1 align="center">ulamspiral</h1>

A C program to create a PNG file of an [Ulam spiral](https://en.wikipedia.org/wiki/Ulam_spiral).

Conforms to POSIX.1-1990 and ISO C (1990).

## Usage

`spiral file_name width`

`file_name` is a name of a file that will be created.  
`width` is a width of an image

## Options

`-f, --force`
If a file exists, overwrite it.

`-p color, --primary=color`
A primary color of the image.

`-s color, --secondary=color`
A secondary color for the image.

## Colors

A color format is either short or long.

A short format is RGB, each letter is a valid hex number between 0 and F.  
Each number gets replicated (e. g. F becomes FF).

A long format is RRGGBB, each value is a valid hex number between 00 and FF.

The default colors are white for primary and black for secondary.

## License

[MIT](LICENSE)
