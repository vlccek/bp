# Návod pro překlad a spuštění projektu

V případě klonování repozitáře `git submodule update --init --recursive`

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`

> překlad nebyl testován na jiných systémech než linux

Tímto postupem je vyprodukována verze pro debugování. Pro produkční verzi je třeba přidat přepínač `-DCMAKE_BUILD_TYPE=Release` příkazu `cmake` v kroku 3.

## Spuštění a použití

Knihovnu je možné pod názve `hashoctree` získat standartním způsobem pomocí zahrnutí této složky jako závislosti projektu ve které je potřeba knihovnu použít.

Pro jednoduché otestování stačí vložit požadováný kod do souboru `main.cpp` a spustit `./main` v adresáři `build` (stačí přeložit pouze jeden cíl `main`).

## Závislosti

- cmake
- gcc 13 (nebo jiný C++ překladač pro c++20 s podporou std::format a std::ranges) s přislušnou std knihovnou. 
- openmp 4.5


