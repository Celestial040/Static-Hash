#include "sort/median.h"

unsigned short median_of_three(unsigned short a, unsigned short b, unsigned short c) {
    if ((c > a && a > b) || (b > a && a > c)) {
        return a;
    }
    if ((a > b && b > c) || (c > b && b > a)) {
        return b;
    }
    if ((a > c && c > b) || (b > c && c > a)) {
        return c;
    }
    if ((a < b && b == c) || (a > b && b == c)) {
        return b;
    }
    if ((a < b && a == c) || (a > b && a == c)) {
        return a;
    }
    return b;
}
