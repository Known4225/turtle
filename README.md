# Turtle Development

To facilitate easier development, this branch separates turtle's modules into their own header and source files.

These modules can be recombined using `make singlefile` to produce a final `turtle.h` file.

## Build

The following build commands are available:

- `make` - The default build command for linux, this will package the turtle headers and source files into a single `turtle.h` file, create a linux static library binary `Linux/turtle.a`, and link that binary to create the example project `turtle.c` into a binary `turtle.o` which can be run.

- `make rel` - The release build command, this builds the example project with optimisations turned on (the library is always built with optimisations on)

- `make lib` - Packages and builds the `Linux/turtle.a` library.

- `make source` - Used for development for faster binary production by not compiling the library.

- `make win` - The default build command for windows. Produces a library `Windows/turtle.lib` as well as the example project `turtle.exe`.

- `make winrel` - The release build command for windows.

- `make winlib` - Packages and builds the `Windows/turtle.lib` library.

- `make singlefile` - This will package the turtle headers and source files into a single `turtle.h` file.

- `make winsource` - Used for development for faster binary production by not compiling the library.

- `make html` - The build command for standalone html file

- `make htmlserver` - The build command for three-file html (.wasm, .js, .html, for servers)

- `make runserver` - For testing the server build on browser

# To Do
- Double click and drag highlights words in normal textboxes
- In normal textboxes you can drag highlighted text out of the text box and into another textbox or program like a pseudo-copy-and-paste (but it doesn't actually use the clipboard)
- Fix textboxes and dropdown priority system by emulating readers "always on top" behaviour (but first fix reader flickering)
- Allow textbox selecting off the ends of the visible content
- Bug: When deploying turtle.html to firebase it did not work on the internet when using three file server setup (even though emrun worked). Standalone singlefile version did work - perhaps emscripten needs to be updated?
- Handrolled linear resizing is not only at least 2x slower than stbir, it is also worse (box size is only what box takes up instead of larger)
- Build some support for osTools on OS_BROWSER
- Add some emojis to font
- Write tests
- Fix flipped world coordinates
- Text-only textbox
- 3D support
- Audio support (PortAudio?)
- SVG support (NanoSVG?)
- Update wiki/documentation
- Linux COM support
- I think there is a way to increase performance for texture enabled turtle - I think it is possible to remove penpos entirely and just write straight to bufferList

# Limitations of web deployment
- Must osTools features will not work
- Must compile with TURTLE_ENABLE_TEXTURES (fixed function pipeline like `glBegin` are not supported in WebGL)
- Cannot compile using static library (must use TURTLE_IMPLEMENTATION macro in source)
- Textures must be loaded with GL_RGBA (this is a bug that I can't figure out how to fix)
- pthreads don't appear to work (despite [emcc saying they can](https://emscripten.org/docs/porting/pthreads.html), I have not been able to get them to work)