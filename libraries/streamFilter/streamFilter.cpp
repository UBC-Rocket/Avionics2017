#include "streamFilter.h"

int streamFilter::begin(int aFraction, int bFraction) {
  int sum = aFraction + bFraction;
  aPercent = aFraction / sum;
  bPercent = bFraction / sum;

  return 0;
}
