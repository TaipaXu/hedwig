# Hedwig

`Hedwig` is a command-line program used to prevent the screen from sleeping, keeping the display always on.

Hedwig hasn't been out for a long time; your space looks good—let her fly freely!

English | [中文](./README_ZH.md)

## Compiling

Before starting the compilation, please make sure you have installed [vcpkg](https://github.com/microsoft/vcpkg). Set the `VCPKG_ROOT` environment variable to point to the directory where `vcpkg` is installed. Add the `vcpkg` path to the system's `PATH` environment variable.

### Linux & macOS

```sh
git clone https://github.com/TaipaXu/hedwig.git && cd hedwig
vcpkg install
cmake --preset=unix-release
cmake --build build/release -j $(nproc)
```

### Windows

```cmd
git clone https://github.com/TaipaXu/hedwig.git && cd hedwig
vcpkg install
cmake --preset=win-release
cmake --build build --config Release -j ([Environment]::ProcessorCount)
```

# License

[GPL-3.0](LICENSE)
