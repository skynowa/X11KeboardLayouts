#!/usr/bin/env bash
set -Eeuo pipefail

main() {
    if [[ "${1:-}" == "--help" || "${1:-}" == "-h" ]]; then
        printf '%s\n' \
            'Usage: ./deploy.sh [CMake configure arguments...]' \
            'Build a Release AppImage using the shared CMakeLib QtAppImage module.' \
            'Environment: BUILD_DIR, QT_PREFIX, CMAKELIB_DIR, LINUXDEPLOY, BUILD_JOBS.' \
            'Defaults: build-release, newest ~/Qt/*/gcc_64 SDK, ~/Projects/CMakeLib.' \
            'linuxdeploy is located on PATH or in build/appimage-tools; no tools are downloaded.'
        return 0
    fi
    local project_dir build_dir qt_prefix linuxdeploy
    project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
    build_dir="${BUILD_DIR:-${project_dir}/build-release}"
    build_dir="$(realpath -m -- "${build_dir}")"
    qt_prefix="${QT_PREFIX:-}"
    if [[ -z "${qt_prefix}" && -z "${CMAKE_PREFIX_PATH:-}" ]]; then
        shopt -s nullglob
        local qt_sdks=("${HOME}"/Qt/*/gcc_64)
        if (( ${#qt_sdks[@]} > 0 )); then
            qt_prefix="$(printf '%s\n' "${qt_sdks[@]}" | sort -V | tail -n 1)"
        fi
    fi
    linuxdeploy="${LINUXDEPLOY:-}"
    if [[ -z "${linuxdeploy}" ]]; then
        if command -v linuxdeploy > /dev/null 2>&1; then
            linuxdeploy="$(command -v linuxdeploy)"
        else
            linuxdeploy="${project_dir}/build/appimage-tools/linuxdeploy-$(uname -m).AppImage"
        fi
    fi
    if ! linuxdeploy="$(command -v -- "${linuxdeploy}")"; then
        printf 'linuxdeploy not found; set LINUXDEPLOY and place its executable Qt plugin next to it.\n' >&2
        return 1
    fi
    export LINUXDEPLOY
    LINUXDEPLOY="$(realpath -- "${linuxdeploy}")"
    local configure_args=(-S "${project_dir}" -B "${build_dir}" -G Ninja -DCMAKE_BUILD_TYPE=Release)
    configure_args+=("-DX11KeyboardLayouts_CMAKELIB_DIR=${CMAKELIB_DIR:-${HOME}/Projects/CMakeLib}")
    if [[ -n "${qt_prefix}" ]]; then
        configure_args+=("-DCMAKE_PREFIX_PATH=${qt_prefix}")
    fi
    cmake "${configure_args[@]}" "$@"
    local build_args=(--build "${build_dir}" --config Release --target appimage)
    if [[ -n "${BUILD_JOBS:-}" ]]; then
        build_args+=(--parallel "${BUILD_JOBS}")
    fi
    cmake "${build_args[@]}"
    printf 'Deployment package: %s/appimage/\n' "${build_dir}"
}

main "$@"
