# Сборка
cd ~/git/passes/llvm/seedpass
cmake -G Ninja -B build
cmake --build build --config release

# Установка собранной библиотеки llvm_srandpass.so в каталог ../../rnd/plugins/
cmake --install build --config release
