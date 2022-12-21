## Installation
```sh
git clone https://github.com/klmkyo/nBank
cd nBank
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh -disableMetrics
./vcpkg/vcpkg install sqlite3 ftxui sqlite-orm
```

## Building 
```sh
mkdir -p build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
```

## Roadmap
- [ ] UI, main loop - Marcin