#include "detours.hpp"
int main() {
  int min = 256, max = 6144;
  Experiments exps(min, max);
  exps.action();
  return 0;
}
