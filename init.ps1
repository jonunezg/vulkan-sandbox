function clean_environment() {
    Remove-Item -Recurse $PSScriptRoot\build
    Remove-Item CMakeUserPresets.json
}

function configure() {
    param ([string]$build_type='Release')
    Push-Location $PSScriptRoot
        conan install . --output-folder=build --build=missing --settings=build_type=$build_type
        cd build
        cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="build\generators\conan_toolchain.cmake"
    Pop-Location
}

function build() {
    param ([string]$build_type='Release')
    Push-Location $PSScriptRoot\build
        cmake --build . --config $build_type
    Pop-Location
}

function build_clean() {
    param ([string]$build_type='Release')
    clean_environment
    configure($build_type)
    build($build_type)
}

function run() {
    param ([string]$build_type='Release')
    Push-Location $PSScriptRoot\build\$build_type
    . .\vulkan.exe
    Pop-Location
}
