# Сборка
cd ~/git/passes/gcc/srandpass
cmake -G Ninja -B build
cmake --build build --config release

# Установка собранной библиотеки gcc_srandpass.so в каталог ../../srand/plugins/
cmake --install build --config release