# ReserveBottomMemory
Reserve all of the bottom memory to force allocations to start above the 4GB line.

This can easily expose compatibility issues when porting 32-bit code to 64 bit.

The original code was taken from: https://randomascii.wordpress.com/2012/02/14/64-bit-made-easy/

## How to use
Call `ReserveBottomMemory()` as early as possible in your process and
`FreeBottomMemory()` at the end.

## How to build
Running cmake will create a VisualStudio solution with a shared library
called `ReserveBottomMemory` and its tests.  
You can build them and run the tests as usual with cmake solutions.

## Integration into your project
You can either:
- Build the shared library with cmake as described above and link to it in your
  project.
- Include the source file directly in your project.
- If you want to use some other build system just create a shared library
  with the source and header file and define `RESERVEBOTTOMMEMORY_SOURCE`
  to export the symbols.

## Tests
Tests have been added to check that after calling `ReserveBottomMemory()`
allocations return high memory addresses.  
See the `tests` folder.

## Platforms supported
Currently only 64-bit Windows is supported.

## License
Apache License 2.0  
http://www.apache.org/licenses/LICENSE-2.0
