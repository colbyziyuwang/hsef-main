/**
 * Code for doing proper floating point comparisons.
 *
 * @file floating_point_utils.h
 */

#ifndef FLOATING_POINT_COMPARE_H_
#define FLOATING_POINT_COMPARE_H_

static const double TOLERANCE = 0.000001;  ///< The amount of tolerance to allow for floating point comparisons.
static const double ROOT_TWO = 1.414213562;  ///< Precomputed static value for square root 2.

/**
 * Returns if a is less than b, given some tolerance due to floating point arithmetic.
 *
 * @param num1 The left-hand operator of "<"
 * @param num2 The right-hand operator of "<"
 * @return If the first number is less than the second.
 */
bool fpLess(double num1, double num2);

/**
 * Returns if a is greater than b, given some tolerance due to floating point arithmetic.
 *
 * @param num1 The left-hand operator of ">"
 * @param num2 The right-hand operator of ">"
 * @return If the first number is greater than the second.
 */
bool fpGreater(double num1, double num2);

/**
 * Returns if a is equal to b, given some tolerance due to floating point arithmetic.
 *
 * @param num1 The left-hand operator of "=="
 * @param num2 The right-hand operator of "=="
 * @return If the first number is equal to the second.
 */
bool fpEqual(double num1, double num2);

/**
 * Returns the minimum of a and b, given some tolerance due to floating point arithmetic.
 *
 * @param num1 The first value to take the minimum over.
 * @param num2 The second value to take the minimum over.
 * @return The minimum of the two numbers
 */
double fpMin(double num1, double num2);

/**
 * Returns the maximum of a and b, given some tolerance due to floating point arithmetic.
 *
 * @param num1 The first value to take the maximum over.
 * @param num2 The second value to take the maximum over.
 * @return The maximum of the two numbers
 */
double fpMax(double num1, double num2);

#endif /* FLOATING_POINT_COMPARE_H_ */
