# Zint WASM Bindings

Simple infrastructure and bindings to get the excellent [Zint](https://zint.org.uk/) barcode generation library running in the browser via WebAssembly compiled with [Emscripten](https://emscripten.org/).

Most features should be exposed by the bindings to get most barcode types and configurations to display, but not everything is exposed. The wrapper is farily thin though, so it should be straightforward to see what's going on. Pull requests welcome if you find something necesssary is missing.

## Building

To build, run build.sh on a somewhat recent Linux distribution. wasmzint.js should be the result if everything worked.

build_dependencies.sh (called by build.sh) handles passing proper arguments to the dependency projects and building dependencies. Dependencies are pulled in via Git submodules in the /deps directory.

## Using

The main idea is to wait for the WASM to init, create a new instance of the ZintWrapper module with whatever parameters you need, call render with scaling information, and read the bytes out into something useful in Javascript.

See the example drawing to a canvas in demo.html.

### Relocating .wasm file
When instantiating the module, provide a dictionary with a function that returns a uri to a file.

Example:

    wasmzint(
        { locateFile: f => `some_directory_prefix_here/${f}` }
    ).then( ...