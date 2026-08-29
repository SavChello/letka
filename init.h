#pragma once

#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Statuses for the number of equation roots.
 * Used when comparing double values or their differences to zero to account for floating-point inaccuracies.
 */
enum rootCount{
        INIT = -1,                ///< For initialization, for check program's mistakes
        NO_ROOTS = 0,             ///< No distinct roots
        ONE_ROOT = 1,             ///< One distinct root, D = 0
        TWO_ROOTS = 2,            ///< Two distinct root, D > 0
        UNLIMITED_ROOTS = 3       ///< Any root is solution
};


const double EPS = 1e-5;          ///< EPS - The recommended error margin for calculations
const double RAND_EPS = 2;     ///< Margin for random coefficients is 0.5%
const double MY_RAND_MAX = 1e14;
const double MY_RAND_MIN = -1e14;

#define MAXLINE 100
#define PRINT_WRONG_UNIT(unit_a, unit_b, unit_c, real_count_roots, x1ref, x2ref, x_1, x_2) (printf("[a] = [%lg], [b] = [%lg], [c] = [%lg]\nExpected %d roots: [x1ref] = [%lg], [x2ref] = [%lg]\nGott roots: [x1] = [%lg], [x2] = [%lg]\n\n", unit_a, unit_b, unit_c, real_count_roots, x1ref, x2ref, x_1, x_2))
#define TIME_SLEEP 20
#define COUNT_RANDOM_TESTS 500

/**
 * @brief Unit test case data
 */
struct ReadNumsUnits {
        double a, b, c;           ///< Three correctly provided arguments of type double
        int real_count_roots;     ///< The number of roots
        double x1ref, x2ref;      ///< The expected correct and ordered roots: two roots, or one if x2ref is NAN
};

struct ReadStrUnits {
        char a[MAXLINE];
        char b[MAXLINE];
        char c[MAXLINE];
        int right_test;
};

#define DEFAULT     "\x1b[0m"
#define RED         "\x1b[31m"
#define SAND        "\033[38;5;137m"
#define BACK_SAND   "\033[48;5;223m"
#define GREEN       "\e[0;32m"
#define MAGENTA     "\033[35m"
#define BLUE        "\e[0;36m"




//-----------------------------------------------------------------------------

int     unit_random();
double  randoms();
int     unit_solving_test();
int     unit_check_str_isnum();
bool    run_test_nums(struct ReadNumsUnits test);

int     roots_int(double *a, double *b, double *c);
double  read_number(char argue);
bool    is_str_is_num(char *str);
double  solve_one_root(double a, double b);
enum    rootCount calc_roots(double a, double b, double c, double *x1, double *x2);

int     roots_out(enum rootCount counter, double x1, double x2);
bool    check_to_go();

int     sort_x(double *x, double *y);
bool    is_zero(double num);
bool    is_equal(double x, double y);
bool    is_zero_rand(double num);
void    neuroslop(const char* c);

//-----------------------------------------------------------------------------
