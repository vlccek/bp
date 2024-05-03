# Návod na překlad a spuštění projektu

0. V případě klonování repozitáře `git submodule update --init --recursive`
1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`

## závislosti

- cmake
- gcc 13 (nebo jiný C++ překladač pro c++20 s podporou std::format a std::ranges) s přislušnou std knihovnou. 
- openmp 3.0


