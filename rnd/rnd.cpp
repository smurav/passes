#include <iostream>
#include <random>

using namespace std;

template <size_t Index = 0, typename... Types>
typename enable_if<Index == sizeof...(Types), void>::type
generate(tuple<Types...>&) {
}

template <size_t Index = 0, typename... Types>
typename enable_if<Index < sizeof...(Types), void>::type
generate(tuple<Types...>& t) {
    using EngineType = typename tuple_element<Index, tuple<Types...>>::type;
    
    EngineType engine{};    
    static random_device rd{};
    static uniform_int_distribution val{1, 9};
    
    for(int j{0}; j < 10; ++j) {
      engine.seed(rd());
      cout << val(engine) << " ";
    }    
    cout << endl;
    
    generate<Index + 1, Types...>(t);
}

int main() {
  tuple<minstd_rand0, minstd_rand, mt19937, mt19937_64, 
        ranlux24_base, ranlux48_base, ranlux24, ranlux48, 
        knuth_b, default_random_engine> engines;
  generate(engines);
  
  return 0; 
} 
