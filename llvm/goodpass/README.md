Пример безобидного прохода, который нужен для демонстрации подмены его вредоносным проходом

# Сборка
cd ~/git/passes/llvm/goodpass
cmake -G Ninja -B build
cmake --build build --config release

# Установка собранной библиотеки llvm_srandpass.so в каталог ../../srand/plugins/good/
cmake --install build --config release
