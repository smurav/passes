# GCC

## Сборка
g++-14 rnd.cpp -o rnd_gcc

## Проверка нормальной сборки
./rnd_gcc
8 8 7 8 2 1 3 8 8 4 
5 2 5 1 2 4 9 6 9 9 
9 5 8 9 4 7 7 5 6 6 
3 5 9 4 6 3 2 5 2 9 
1 7 9 6 4 8 1 4 6 3 
5 8 7 2 8 6 8 5 2 3 
6 9 3 7 8 7 2 5 8 9 
6 7 8 6 3 6 6 8 1 6 
8 2 2 1 1 8 4 8 6 4 
9 3 2 4 7 7 6 1 7 1

## Просмотр всех проходов
g++-14 -O2 rnd.cpp -o rnd -fdump-passes

## Получение промежуточного представления кода
g++-14 -O1 -fdump-tree-original -fdump-tree-optimized rnd.cpp -o rnd

## Просмотр исходного промежуточного представления
cat rnd.cpp.005t.original
...
29394  {
    int j = 0;

    <<cleanup_point     int j = 0;>>;
    goto <D.94342>;
    <D.94341>:;
    <<cleanup_point <<< Unknown tree: expr_stmt
      std::linear_congruential_engine<long unsigned int, 16807, 0, 2147483647>::seed (&engine, (result_type) std::random_device::operator() (&rd)) >>>>>;
    <<cleanup_point <<< Unknown tree: expr_stmt
      (void) std::operator<< <std::char_traits<char> > (std::basic_ostream<char>::operator<< (&cout, std::uniform_int_distribution<int>::operator()<std::linear_congruential_engine<long unsigned int, 16807, 0, 2147483647> > (&val, (struct linear_congruential_engine &) &engine)), (const char *) " ") >>>>>;
    <<cleanup_point (void)  ++j>>;
    <D.94342>:;
    if (j <= 9) goto <D.94341>; else goto <D.94339>;
    <D.94339>:;
  }
...

## Просмотр оптимизированного промежуточного представления
cat rnd.cpp.265t.optimized
...
2413  <bb 14> [local count: 976138696]:
  # ivtmp_68 = PHI <ivtmp_67(14), 10(13)>
  _27 = std::random_device::_M_getval (&rd);
  _5 = (long unsigned int) _27;
  std::subtract_with_carry_engine<long unsigned int, 48, 5, 12>::seed (&engine._M_b, _5);
  engine._M_n = 0;
  _28 = std::uniform_int_distribution<int>::operator()<std::discard_block_engine<std::subtract_with_carry_engine<long unsigned int, 48, 5, 12>, 389, 11> > (&val, &engine, &val._M_param);
  _24 = std::basic_ostream<char>::operator<< (&cout, _28);
  std::__ostream_insert<char, std::char_traits<char> > (_24, " ", 1);
  ivtmp_67 = ivtmp_68 + 4294967295;
  if (ivtmp_67 != 0)
    goto <bb 14>; [90.00%]
  else
    goto <bb 15>; [10.00%]
...

## Сборка с применением вредоносного прохода оптимизации
## Библиотеку gcc_seedpass.so необходимо предварительно собрать из исходного 
## кода в каталоге ../gcc/seedpass

g++-14 -fdump-tree-optimized -fplugin=./plugins/gcc_seedpass.so rnd.cpp -o rnd_gcc_instr

## Просмотр оптимизированного промежуточного представления, полученного с 
## участием вредоносного прохода оптимизации
cat rnd_gcc_instr-rnd.cpp.265t.optimized
...
  <bb 10> :
  _43 = std::random_device::operator() (&rd);
  _5 = _43;
  _6 = (long unsigned int) _5;
  std::mersenne_twister_engine<long unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>::seed (&engine, 1234567);
  _46 = std::uniform_int_distribution<int>::operator()<std::mersenne_twister_engine<long unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253> > (&val, &engine);
  _7 = _46;
  _48 = std::basic_ostream<char>::operator<< (&cout, _7);
  _8 = _48;
  std::operator<< <std::char_traits<char> > (_8, " ");
  j_50 = j_9 + 1;
...

## Проверка сборки, полученной с участием вредоносного прохода оптимизации
./rnd_gcc_instr
6 6 6 6 6 6 6 6 6 6 
7 7 7 7 7 7 7 7 7 7 
3 3 3 3 3 3 3 3 3 3 
9 9 9 9 9 9 9 9 9 9 
5 5 5 5 5 5 5 5 5 5 
6 6 6 6 6 6 6 6 6 6 
5 5 5 5 5 5 5 5 5 5 
6 6 6 6 6 6 6 6 6 6 
8 8 8 8 8 8 8 8 8 8 
6 6 6 6 6 6 6 6 6 6 

# LLVM

## Сборка
~/llvm20/bin/clang rnd.cpp -o rnd_llvm -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ -L/usr/lib/gcc/x86_64-linux-gnu/11 -lstdc++

## Проверка нормальной сборки
./rnd_llvm
1 8 6 6 7 2 3 2 3 4 
2 7 2 1 4 1 1 6 9 2 
8 9 6 1 2 8 8 7 3 2 
9 5 6 1 6 6 6 3 7 3 
7 8 5 3 6 1 3 4 1 4 
5 2 1 8 2 8 6 2 2 7 
7 3 8 9 5 8 4 7 1 7 
3 9 7 4 6 7 6 3 2 1 
6 7 4 5 1 3 6 2 1 8 
5 1 6 8 4 3 3 5 6 8

## Получение промежуточного представления кода
~/llvm20/bin/clang -S -emit-llvm -O1 rnd.cpp -o rnd.ll -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ 

## Просмотр промежуточного представления
cat rnd.ll
...
912 14:                                               ; preds = %3
  %15 = getelementptr inbounds i8, ptr %1, i64 10224
  store i64 624, ptr %15, align 8, !tbaa !12
  %16 = getelementptr inbounds i8, ptr %1, i64 10232
  store i64 1, ptr %16, align 8, !tbaa !14
  %17 = getelementptr inbounds i8, ptr %1, i64 10240
  store i64 1, ptr %17, align 8, !tbaa !16
  call void @_Z8generateILm0EJSt26linear_congruential_engineImLm16807ELm0ELm2147483647EES0_ImLm48271ELm0ELm2147483647EESt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EES3_ImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EESt26subtract_with_carry_engineImLm24ELm10ELm24EES6_ImLm48ELm5ELm12EESt20discard_block_engineIS7_Lm223ELm23EES9_IS8_Lm389ELm11EESt20shuffle_order_engineIS1_Lm256EES1_EENSt9enable_ifIXltT_sZT0_EvE4typeERSt5tupleIJDpT0_EE(ptr noundef nonnull align 8 dereferenceable(10248) %1)
  call void @llvm.lifetime.end.p0(i64 10248, ptr nonnull %1) #13
  ret i32 0
...

## Получение промежуточного представления кода с вредоносной оптимизацией
## Библиотеку llvm_seedpass.so необходимо предварительно собрать из исходного 
## кода в каталоге ../llvm/seedpass
~/llvm20/bin/opt --load-pass-plugin=./plugins/llvm_seedpass.so -passes="seedpass,default<O2>" rnd.ll -S -o rnd_opt.ll 

## Прямое получение промежуточного представления кода с вредоносной оптимизацией
~/llvm20/bin/clang -S -emit-llvm -O1 -fpass-plugin=./plugins/llvm_seedpass.so rnd.cpp -o rnd_instr.ll -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ 
_ZNSt26linear_congruential_engineImLm16807ELm0ELm2147483647EE4seedEm => std::linear_congruential_engine<unsigned long, 16807ul, 0ul, 2147483647ul>::seed(unsigned long)
_ZNSt26linear_congruential_engineImLm16807ELm0ELm2147483647EE4seedEm => std::linear_congruential_engine<unsigned long, 16807ul, 0ul, 2147483647ul>::seed(unsigned long)
_ZNSt26subtract_with_carry_engineImLm48ELm5ELm12EE4seedEm => std::subtract_with_carry_engine<unsigned long, 48ul, 5ul, 12ul>::seed(unsigned long)
_ZNSt26subtract_with_carry_engineImLm24ELm10ELm24EE4seedEm => std::subtract_with_carry_engine<unsigned long, 24ul, 10ul, 24ul>::seed(unsigned long)
_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EE4seedEm => std::mersenne_twister_engine<unsigned long, 64ul, 312ul, 156ul, 31ul, 13043109905998158313ul, 29ul, 6148914691236517205ul, 17ul, 8202884508482404352ul, 37ul, 18444473444759240704ul, 43ul, 6364136223846793005ul>::seed(unsigned long)
_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE4seedEm => std::mersenne_twister_engine<unsigned long, 32ul, 624ul, 397ul, 31ul, 2567483615ul, 11ul, 4294967295ul, 7ul, 2636928640ul, 15ul, 4022730752ul, 18ul, 1812433253ul>::seed(unsigned long)
_ZNSt26linear_congruential_engineImLm48271ELm0ELm2147483647EE4seedEm => std::linear_congruential_engine<unsigned long, 48271ul, 0ul, 2147483647ul>::seed(unsigned long)
_ZNSt26linear_congruential_engineImLm48271ELm0ELm2147483647EE4seedEm => std::linear_congruential_engine<unsigned long, 48271ul, 0ul, 2147483647ul>::seed(unsigned long)
_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE4seedEm => std::mersenne_twister_engine<unsigned long, 32ul, 624ul, 397ul, 31ul, 2567483615ul, 11ul, 4294967295ul, 7ul, 2636928640ul, 15ul, 4022730752ul, 18ul, 1812433253ul>::seed(unsigned long)
_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EE4seedEm => std::mersenne_twister_engine<unsigned long, 64ul, 312ul, 156ul, 31ul, 13043109905998158313ul, 29ul, 6148914691236517205ul, 17ul, 8202884508482404352ul, 37ul, 18444473444759240704ul, 43ul, 6364136223846793005ul>::seed(unsigned long)
_ZNSt26subtract_with_carry_engineImLm24ELm10ELm24EE4seedEm => std::subtract_with_carry_engine<unsigned long, 24ul, 10ul, 24ul>::seed(unsigned long)
_ZNSt26subtract_with_carry_engineImLm48ELm5ELm12EE4seedEm => std::subtract_with_carry_engine<unsigned long, 48ul, 5ul, 12ul>::seed(unsigned long)
_ZNSt20discard_block_engineISt26subtract_with_carry_engineImLm24ELm10ELm24EELm223ELm23EE4seedEm => std::discard_block_engine<std::subtract_with_carry_engine<unsigned long, 24ul, 10ul, 24ul>, 223ul, 23ul>::seed(unsigned long)
_ZNSt26subtract_with_carry_engineImLm24ELm10ELm24EE4seedEm => std::subtract_with_carry_engine<unsigned long, 24ul, 10ul, 24ul>::seed(unsigned long)
_ZNSt20discard_block_engineISt26subtract_with_carry_engineImLm48ELm5ELm12EELm389ELm11EE4seedEm => std::discard_block_engine<std::subtract_with_carry_engine<unsigned long, 48ul, 5ul, 12ul>, 389ul, 11ul>::seed(unsigned long)
_ZNSt26subtract_with_carry_engineImLm48ELm5ELm12EE4seedEm => std::subtract_with_carry_engine<unsigned long, 48ul, 5ul, 12ul>::seed(unsigned long)
_ZNSt20shuffle_order_engineISt26linear_congruential_engineImLm16807ELm0ELm2147483647EELm256EE4seedEm => std::shuffle_order_engine<std::linear_congruential_engine<unsigned long, 16807ul, 0ul, 2147483647ul>, 256ul>::seed(unsigned long)
_ZNSt26linear_congruential_engineImLm16807ELm0ELm2147483647EE4seedEm => std::linear_congruential_engine<unsigned long, 16807ul, 0ul, 2147483647ul>::seed(unsigned long)
_ZNSt26linear_congruential_engineImLm16807ELm0ELm2147483647EE4seedEm => std::linear_congruential_engine<unsigned long, 16807ul, 0ul, 2147483647ul>::seed(unsigned long)

## Просмотр промежуточного представления кода с вредоносной оптимизацией
cat rnd_instr.ll
...
2981 46:                                               ; preds = %14, %46
  %47 = phi i32 [ %52, %46 ], [ 0, %14 ]
  %48 = call noundef i32 @_ZNSt13random_device9_M_getvalEv(ptr noundef nonnull align 8 dereferenceable(5000) @_ZZ8generateILm9EJSt26linear_congruential_engineImLm16807ELm0ELm2147483647EES0_ImLm48271ELm0ELm2147483647EESt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EES3_ImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EESt26subtract_with_carry_engineImLm24ELm10ELm24EES6_ImLm48ELm5ELm12EESt20discard_block_engineIS7_Lm223ELm23EES9_IS8_Lm389ELm11EESt20shuffle_order_engineIS1_Lm256EES1_EENSt9enable_ifIXltT_sZT0_EvE4typeERSt5tupleIJDpT0_EEE2rd)
  store i64 1234567, ptr %2, align 8, !tbaa !16
  %49 = call noundef i32 @_ZNSt24uniform_int_distributionIiEclISt26linear_congruential_engineImLm16807ELm0ELm2147483647EEEEiRT_RKNS0_10param_typeE(ptr noundef nonnull align 4 dereferenceable(8) @_ZZ8generateILm9EJSt26linear_congruential_engineImLm16807ELm0ELm2147483647EES0_ImLm48271ELm0ELm2147483647EESt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EES3_ImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EESt26subtract_with_carry_engineImLm24ELm10ELm24EES6_ImLm48ELm5ELm12EESt20discard_block_engineIS7_Lm223ELm23EES9_IS8_Lm389ELm11EESt20shuffle_order_engineIS1_Lm256EES1_EENSt9enable_ifIXltT_sZT0_EvE4typeERSt5tupleIJDpT0_EEE3val, ptr noundef nonnull align 8 dereferenceable(8) %2, ptr noundef nonnull align 4 dereferenceable(8) @_ZZ8generateILm9EJSt26linear_congruential_engineImLm16807ELm0ELm2147483647EES0_ImLm48271ELm0ELm2147483647EESt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EES3_ImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EESt26subtract_with_carry_engineImLm24ELm10ELm24EES6_ImLm48ELm5ELm12EESt20discard_block_engineIS7_Lm223ELm23EES9_IS8_Lm389ELm11EESt20shuffle_order_engineIS1_Lm256EES1_EENSt9enable_ifIXltT_sZT0_EvE4typeERSt5tupleIJDpT0_EEE3val)
  %50 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, i32 noundef %49)
  %51 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l(ptr noundef nonnull align 8 dereferenceable(8) %50, ptr noundef nonnull @.str, i64 noundef 1)
  %52 = add nuw nsw i32 %47, 1
  %53 = icmp eq i32 %52, 10
  br i1 %53, label %19, label %46, !llvm.loop !92
...

## Cборка из промежуточного представления с применением вредоносного прохода оптимизации
~/llvm20/bin/clang rnd_instr.ll -o rnd_llvm_instr -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ -L/usr/lib/gcc/x86_64-linux-gnu/11 -lstdc++

## Прямая сборка с применением вредоносного прохода оптимизации
~/llvm20/bin/clang rnd.cpp -o rnd_llvm_instr -fpass-plugin=./plugins/llvm_seedpass.so -I /usr/include/c++/11/ -I /usr/include/x86_64-linux-gnu/c++/11/ -L/usr/lib/gcc/x86_64-linux-gnu/11 -lstdc++

## Проверка сборки, полученной с участием вредоносного прохода оптимизации
./rnd_llvm_instr
6 6 6 6 6 6 6 6 6 6 
7 7 7 7 7 7 7 7 7 7 
3 3 3 3 3 3 3 3 3 3 
9 9 9 9 9 9 9 9 9 9 
5 5 5 5 5 5 5 5 5 5 
6 6 6 6 6 6 6 6 6 6 
5 5 5 5 5 5 5 5 5 5 
6 6 6 6 6 6 6 6 6 6 
8 8 8 8 8 8 8 8 8 8 
6 6 6 6 6 6 6 6 6 6 

## Сборка с генерацией базы данных компиляции для GCC
## База данных формируется в файле ./build/compile_commands.json
cmake -G Ninja -B build
cmake --build build --config release

## Сборка с генерацией базы данных компиляции для LLVM
~/llvm20/bin/clang rnd.cpp -o rnd -MJ compile_db.json ...

