# X11KeyboardLayouts

## AppImage deployment (Linux)

Packaging uses `QtAppImage.cmake` from `$HOME/Projects/CMakeLib` and its shared
`Functions/QtAppImage.sh` helper. Install linuxdeploy and linuxdeploy-plugin-qt for
your architecture, make both executable, and place the plugin next to linuxdeploy.
No packaging tools are downloaded by the build.

```bash
LINUXDEPLOY=/absolute/path/to/linuxdeploy-x86_64.AppImage ./deploy.sh
```

`deploy.sh` configures a Release build and builds the `appimage` target. Packages
are written to `build-release/appimage/`. It selects the newest SDK under
`$HOME/Qt/*/gcc_64` unless `QT_PREFIX` or `CMAKE_PREFIX_PATH` is set.
`QT_PREFIX`, `CMAKELIB_DIR`, `BUILD_DIR`, and `BUILD_JOBS` override the defaults;
additional arguments are forwarded to CMake configuration. Use `./deploy.sh --help`.
linuxdeploy can also be found on PATH or in `build/appimage-tools/`.

For an already configured build, use `cmake --build build-release --target appimage`
with `LINUXDEPLOY` set. The CMakeLib path can be overridden with
`-DX11KeyboardLayouts_CMAKELIB_DIR=/path/to/CMakeLib`.
Run the generated executable AppImage directly; on systems without FUSE, use
`APPIMAGE_EXTRACT_AND_RUN=1`. Build on the oldest supported Linux distribution
and create a separate package for each architecture.
