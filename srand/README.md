# GCC

## Сборка
g++-14 srand.cpp -o srand

## Проверка нормальной сборки
./srand
3 6 7 5 3 5 6 2 9 1 
0 9 8 5 1 8 4 7 5 7 
6 5 8 0 5 0 2 6 1 4 
1 3 4 6 3 7 5 3 8 5 
5 5 0 2 6 2 0 9 3 1 
1 5 2 5 8 5 6 3 2 8 
7 9 9 1 5 3 6 7 0 3 
6 4 2 9 1 3 2 1 7 3 
5 4 5 7 0 3 0 8 2 2 
5 8 8 5 8 7 5 1 7 7

## Получение промежуточного представления кода
g++-14 -O1 -fdump-tree-original -fdump-tree-optimized srand.cpp -o srand

## Просмотр исходного промежуточного представления
cat srand.cpp.005t.original
...
    <D.50993>:;
    {
      unsigned int seed = (unsigned int) (i + 1);

            unsigned int seed = (unsigned int) (i + 1);
      <<cleanup_point <<< Unknown tree: expr_stmt
        srand (seed) >>>>>;
...

## Просмотр оптимизированного промежуточного представления
cat srand.cpp.265t.optimized
...
  <bb 3> [local count: 97603128]:
  # i_34 = PHI <_1(10), 0(2)>
  _1 = i_34 + 1;
  _51 = (unsigned int) _1;
  srand (_51);
...

## Сборка с применением вредоносного прохода оптимизации
## Библиотеку gcc_srandpass.so необходимо предварительно собрать из исходного 
## кода в каталоге ../gcc/srandpass

g++-14 -fdump-tree-optimized -fplugin=./plugins/gcc_srandpass.so srand.cpp -o srand_gcc_instr

## Просмотр оптимизированного промежуточного представления, полученного с 
## участием вредоносного прохода оптимизации
cat srand_gcc_instr-srand.cpp.265t.optimized
...
  <bb 3> :
  _1 = i_5 + 1;
  seed_12 = (unsigned int) _1;
  srand (1234567);
  j_14 = 0;
  goto <bb 5>; [INV]
...

## Проверка сборки, полученной с участием вредоносного прохода оптимизации
./srand_gcc_instr
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 

# LLVM

## Сборка
~/llvm20/bin/clang srand.cpp -o srand -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ -L/usr/lib/gcc/x86_64-linux-gnu/11 -lstdc++

## Проверка нормальной сборки
./srand
3 6 7 5 3 5 6 2 9 1 
0 9 8 5 1 8 4 7 5 7 
6 5 8 0 5 0 2 6 1 4 
1 3 4 6 3 7 5 3 8 5 
5 5 0 2 6 2 0 9 3 1 
1 5 2 5 8 5 6 3 2 8 
7 9 9 1 5 3 6 7 0 3 
6 4 2 9 1 3 2 1 7 3 
5 4 5 7 0 3 0 8 2 2 
5 8 8 5 8 7 5 1 7 7

## Получение промежуточного представления кода
~/llvm20/bin/clang -S -emit-llvm -O1 srand.cpp -o srand.ll -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ 

## Просмотр промежуточного представления
cat srand.ll
...
2:                                                ; preds = %0, %25
  %3 = phi i32 [ 0, %0 ], [ %4, %25 ]
  %4 = add nuw nsw i32 %3, 1
  tail call void @srand(i32 noundef %4) #4
  br label %30
...

## Получение промежуточного представления кода с вредоносной оптимизацией
## Библиотеку llvm_srandpass.so необходимо предварительно собрать из исходного 
## кода в каталоге ../llvm/srandpass
~/llvm20/bin/opt --load-pass-plugin=./plugins/llvm_srandpass.so -passes="srandpass,default<O2>" srand.ll -S -o srand_opt.ll 

## Прямое получение промежуточного представления кода с вредоносной оптимизацией
~/llvm20/bin/clang -S -emit-llvm -O1 -fpass-plugin=./plugins/llvm_srandpass.so srand.cpp -o srand_instr.ll -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ 

## Просмотр промежуточного представления кода с вредоносной оптимизацией
cat srand_instr.ll
...
2:                                                ; preds = %0, %24
  %3 = phi i32 [ 0, %0 ], [ %28, %24 ]
  tail call void @srand(i32 noundef 1234567) #4
  br label %30
...

## Cборка из промежуточного представления с применением вредоносного прохода оптимизации
~/llvm20/bin/clang srand_instr.ll -o srand_llvm_instr -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ -L/usr/lib/gcc/x86_64-linux-gnu/11 -lstdc++

## Прямая сборка с применением вредоносного прохода оптимизации
~/llvm20/bin/clang srand.cpp -o srand_llvm_instr -fpass-plugin=./plugins/llvm_srandpass.so -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ -L/usr/lib/gcc/x86_64-linux-gnu/11 -lstdc++

## Проверка сборки, полученной с участием вредоносного прохода оптимизации
./srand_llvm_instr
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 
4 2 9 0 0 8 3 5 4 6 

## Сборка с генерацией базы данных компиляции
~/llvm20/bin/clang srand.cpp -o srand -MJ compile.db ...

