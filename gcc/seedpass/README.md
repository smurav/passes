# Сборка
cd ~/git/passes/gcc/seedpass
cmake -G Ninja -B build
cmake --build build --config release

# Установка собранной библиотеки gcc_srandpass.so в каталог ../../rnd/plugins/
cmake --install build --config release
