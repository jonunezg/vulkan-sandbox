# vulkan-sandbox
Playing around with Vulkan

* Build and run using VS generator

``` PowerShell
# From the repository root

# Release
conan install . --output-folder=build --build=missing
pushd build ; cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="build\generators\conan_toolchain.cmake" ; popd
pushd build ; cmake --build . --config Release ; popd
.\build\Release\vulkan.exe

#Debug
conan install . --output-folder=build --build=missing --settings=build_type=Debug
pushd build ; cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="build\generators\conan_toolchain.cmake" ; popd
pushd build ; cmake --build . --config Debug ; popd
.\build\Debug\vulkan.exe
```

* Build using init.ps1

``` PowerShell
. .\init.ps1
configure [-build_type Debug]
build [-build_type Debug]
run [-build_type Debug]

build_clean [-build_type Debug] # To rebuild
```

Once configured, project can be also built, run and debugged using VSCode controls