# Turtle Development

To facilitate easier development, this branch separates turtle's modules into their own header and source files.

These modules can be recombined using `make singlefile` to produce a final `turtle.h` file.

## Build

The following build commands are available:

- `make` - The default build command for linux, this will package the turtle headers and source files into a single `turtle.h` file, create a linux static library binary `Linux/turtle.a`, and link that binary to create the example project `turtle.c` into a binary `turtle.o` which can be run.

- `make rel` - The release build command, this builds the example project with optimisations turned on (the library is always built with optimisations on)

- `make reltextures` - The release build command for linux including texture support.

- `make lib` - This command will package and build the `Linux/turtle.a` library.

- `make win` - The default build command for windows. Produces a library `Windows/turtle.lib` as well as the example project `turtle.exe`.

- `make winrel` - The release build command for windows.

- `make winreltextures` - The release build command for windows including texture support.

- `make singlefile` - This will package the turtle headers and source files into a single `turtle.h` file.

# To Do
- Experiment with wasm and webgl building into an html executable (insane potential)
- Write tests
- Vertical switches
- Fix flipped world coordinates
- Mouse interactions with textbox (highlight, index select)
- Text-only textbox
- 3D support
- Audio support
- Update wiki/documentation
- Linux COM support
- Reduce size of turtle.h, find a building solution that doesn't involve 26936 lines of duplicate code
- I think there is a way to increase performance for texture enabled turtle - I think it is possible to remove penpos entirely and just write straight to bufferList - this would probably only increase CPU performance which has not, in my experience, been the bottleneck for most machines
- increase performance by reverting the GPU always dying update (special handling for cases that would not otherwise mutate the penpos list)