#include "floating_point_utils.h"

bool fpLess(double num1, double num2) {
    return (num1 < num2 - TOLERANCE);
}

bool fpGreater(double num1, double num2) {
    return (num1 > num2 + TOLERANCE);
}

bool fpEqual(double num1, double num2) {
    return !fpLess(num1, num2) && !fpGreater(num1, num2);
}

double fpMin(double num1, double num2) {
    if (fpGreater(num1, num2)) {
        return num2;
    }
    return num1;
}

double fpMax(double num1, double num2) {
    if (fpGreater(num1, num2)) {
        return num1;
    }
    return num2;
}
