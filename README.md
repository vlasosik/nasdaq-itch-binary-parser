# NASDAQ ITCH 5.0 Binary Parser

A zero-copy parser for NASDAQ TotalView-ITCH 5.0 market data. Memory-maps
raw ITCH files and decodes messages directly from the mapping no
intermediate copies.

## Getting Started

### Prerequisites

- GCC or Clang with C++23 support
- CMake 3.22 or higher
- ~10 GB of free disk space for a day file (see Data files)

### Cloning the repository

This project uses [vcpkg](https://github.com/microsoft/vcpkg) as a submodule
to manage dependencies. Clone with the `--recursive` flag to fetch it
automatically:

```bash
git clone --recursive https://github.com/vlasosik/nasdaq-itch-binary-parser.git
```

If you already cloned without `--recursive`, run these commands inside the
project directory:

```bash
git submodule init
git submodule update
```

### Building

The project uses [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
with [Ninja](https://ninja-build.org/) as the build system. The following
presets are available:

| Preset       | Description                                               |
|--------------|-----------------------------------------------------------|
| `debug`      | Debug build                                               |
| `debug-asan` | Debug + AddressSanitizer (memory errors)                  |
| `debug-tsan` | Debug + ThreadSanitizer (data races)                      |
| `debug-msan` | Debug + MemorySanitizer (uninitialized reads, Clang only) |
| `release`    | Optimized release build (`-O3`, LTO, native CPU tuning)   |

To configure and build, for example with the release preset:

```bash
cmake --preset release
cmake --build --preset release
```

### Data files

Sample ITCH 5.0 binary data files can be downloaded from the
[Nasdaq Historical Data](https://emi.nasdaq.com/ITCH/Nasdaq%20ITCH/) page.
A compressed day file is ~1 GB and expands to ~9 GB, so make sure you have
enough disk space.

The parser works with the **uncompressed** file. Download and unpack:

```bash
# download 08302019.NASDAQ_ITCH50.gz into data/, then:
gunzip -k data/08302019.NASDAQ_ITCH50.gz
```

The `-k` flag keeps the original `.gz` file. The parser expects the
uncompressed file at: