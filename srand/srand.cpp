#include <iostream>
#include <cstdlib> 

using namespace std;

int main() {
  for(int i{0}; i < 10; ++i) {

    unsigned int seed = i + 1;
    srand(seed);

    for(int j{0}; j < 10; ++j) {
      cout << rand() % 10 << " ";
    }

    cout << endl;
  }

  return 0; 
} 
