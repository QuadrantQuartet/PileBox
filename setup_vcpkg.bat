@echo off
chcp 65001
echo 更新 vcpkg
echo ============================================================
git submodule update --init --recursive
if not exist .\vcpkg\vcpkg.exe (
    call .\vcpkg\bootstrap-vcpkg.bat
)

echo 设置 vcpkg 环境变量
echo ============================================================
set VCPKG_ROOT=
set VCPKG_DEFAULT_TRIPLET=x64-mingw-dynamic
set VCPKG_DEFAULT_HOST_TRIPLET=x64-mingw-dynamic

echo 下载 box2d
echo ============================================================
.\vcpkg\vcpkg install box2d
