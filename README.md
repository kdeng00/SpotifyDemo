# SpotifyDemo

Experimenting with the Sptofiy API


## Built with

* C++
* cmake
* vcpkg
* nlohmann-json
* cpr
* cppcodec

## Getting Started

Install dependencies using `vcpkg`

```console
vcpkg install nlohmann-json cpr cppcodec
```

Bootstrap vcpkg

```
cd SpotifyDemo/3rdparty/vcpkg
./[bootstrap.bat || bootstrap.sh]
```

Build the software

```console
mkdir build
cd build
cmake ..
cmake --build .
```
