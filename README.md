# ReserveBottomMemory
Reserve all of the bottom memory to force allocations to start above the 4GB line.

This can easily expose compatibility issues when porting 32-bit code to 64 bit.

The original code was taken from: https://randomascii.wordpress.com/2012/02/14/64-bit-made-easy/

## How to use
Call `ReserveBottomMemory()` as early as possible in your project and `FreeBottomMemory()` at the end.

## Platforms supported
Currently only 64-bit Windows is supported.

## Integration into your project
You can either:
- Include the source file in your project.
- Create a shared library with the source and header file and define `RESERVEBOTTOMMEMORY_SOURCE`
to export the symbols.

## License
Apache License 2.0

http://www.apache.org/licenses/LICENSE-2.0
