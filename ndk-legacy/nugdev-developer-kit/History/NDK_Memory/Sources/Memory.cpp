#include "Memory.h"

int main() {
  int a[5] = {1, 2, 3, 4, 5};

  ndk::memory::Memory<int> mem{a};
}