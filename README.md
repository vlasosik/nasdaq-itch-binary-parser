## Getting Started

### Prerequisites

- GCC or Clang with C++23 support
- CMake 3.22 or higher

### Building

The project uses [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
with [Ninja](https://ninja-build.org/) as the build system. The following presets are available:

| Preset       | Description                                   |
|--------------|-----------------------------------------------|
| `debug`      | Debug build                                   |
| `debug-asan` | Debug + AddressSanitizer (memory errors)      |
| `debug-tsan` | Debug + ThreadSanitizer (data races)          |
| `debug-msan` | Debug + MemorySanitizer (uninitialized reads) |
| `release`    | Optimized release build                       |

To configure and build, for example with the release preset:

```bash
cmake --preset release
cmake --build --preset release
```

### Cloning the repository

This project uses [vcpkg](https://github.com/microsoft/vcpkg) as a submodule to manage dependencies. Clone with the
`--recursive` flag to fetch it automatically:

```bash
git clone --recursive https://github.com/vlasosik/nasdaq-itch-binary-parser.git
```

If you already cloned without `--recursive`, run these commands inside the project directory:

```bash
git submodule init
git submodule update
```

### Data files

The parser expects the input file to be placed at the following path relative to the project root:

```
data/08302019.NASDAQ_ITCH50.gz
```

Sample ITCH 5.0 binary data files can be downloaded from
the [Nasdaq Historical Data](https://emi.nasdaq.com/ITCH/Nasdaq%20ITCH/) page.

For the ITCH 5.0 protocol specification, refer to
the [official Nasdaq documentation](https://www.nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHspecification.pdf).

A small sample file (`data/snapshot.bin`) is included in the repository for quick testing without downloading the full
dataset.