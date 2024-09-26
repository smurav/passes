Пример вредоносного прохода

# Сборка
cd ~/git/passes/llvm/srandpass
cmake -G Ninja -B build
cmake --build build --config release

# Установка собранной библиотеки llvm_srandpass.so в каталог ../../srand/plugins
cmake --install build --config release
