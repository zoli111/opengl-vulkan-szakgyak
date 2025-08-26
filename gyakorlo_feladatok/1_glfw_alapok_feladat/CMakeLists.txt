# Elsőnek a main.cpp-t kell működőképesre megcsinálni, utána érdemes
# ezt a fájlt nézegetni.

# Egy komplexebb, több forrásfájlból álló programot
# lefordítani, linkelni, több célkonfigurációt létrehozni, tesztelni,
# telepíteni, stb...
# bonyolult folyamat, ezért ennek az automatizálására build-rendszereket
# szokás használni. Ilyen build-rendszer például a Make. A projekt
# könyvtárában van egy Makefile nevű konfigurációs fájl, és a make
# parancs futtatásakor ez alapján épül fel a projekt.

# Viszont build-rendszerekből is több van, és lehet, hogy más operációs
# rendszeren más build-rendszert használnak. Ezért vannak meta build-rendszerek is,
# pl. a Cmake. Ezek build-rendszer konfigurációs állományokat hoznak létre
# automatikusan, és azokat felhasználva lehet lefordítani a programot.

# Cmake használata parancssorból:
# Tegyük fel, hogy a projekt könyvtárában vagyunk,
# Ahol a CMakeLists.txt is
# `mkdir build` # érdemes mindig külön könyvtárat használni a buildelésre
# `cd build` # belelépünk a build könyvtárba
# `cmake ..` # a projekt könyvtárát kell megadni (ahol a CMakeLists.txt van)
# Esetünkben ez a szülőkönyvtár
# `make` # a projekt buildelése
# `./program` # ha minden jól ment, kapunk egy futtatható fájlt, amit hát... futtathatunk

# Szerencsére ezeket a lépéseket nem kell mindig megcsinálni, elég
# a make-et futtatni, sőt, automatikusan érzékeli a megváltoztatott
# fájlokat, ezért csak azokat kell újra fordítani
# Így csökkenthető a build idő
# Na de lássunk is hogyan épül fel egy egyszerű CMake recept

# A minimális támogatott CMake verziót mindig meg kell adni
cmake_minimum_required(VERSION 3.5)
# Valamilyen projektnév
project(glfw_alapok)
# Első paraméter: futtatható fájl neve, többi paraméter: forrásfájlok
add_executable(program main.cpp)
# Első paraméter melyik futtatható fájlhoz linkeljünk
# Második paraméter: melyik függvénykönyvtárakat kell linkelni
# (Nem kell l prefix pl.: lGL helyett csak GL)
target_link_libraries(program glfw GL)
