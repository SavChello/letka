#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
//#include <conio.h>

const double EPS = 1e-6;
#define MAXLINE 1000

enum rootCount{
    NO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    UNLIMITED_ROOTS = 3
};


// print color

void roots_int(double* a, double* b, double* c);
enum rootCount calc_roots(double a, double b, double c, double* x1, double* x2);
bool check_to_go(void);
void roots_out(enum rootCount counter, double x1, double x2);
double solve_one_root(double a, double b);
bool is_zero(double num);
bool is_str_is_num(char *str);
double read_number(void);


int main(void) {
    double x1 = 0.0, x2 = 0.0;
    double a = 0.0, b = 0.0, c = 0.0;
    rootCount count_roots = NO_ROOTS;
    printf("Hello, this is a program for solving ");
    printf("quadratic equations, such as ax^2 + bx + c = 0\n");

    while (1) {
        roots_int(&a, &b, &c);
        count_roots = calc_roots(a, b, c, &x1, &x2);
        roots_out(count_roots, x1, x2);

        if (check_to_go()) {
            break;
        }
    }

    printf("Thanks for using program");
    return 0;
}

void roots_int(double *a, double *b, double *c) {
    double internal_a = 0.0, internal_b = 0.0, internal_c = 0.0;

    printf("Enter a: ");
    internal_a = read_number();
    printf("Enter b: ");
    internal_b = read_number();
    printf("Enter c: ");
    internal_c = read_number();

    *a = internal_a;
    *b = internal_b;
    *c = internal_c;

}

double read_number(void) {
    int check_num = 0;
    double num = 0.0;
    char str[MAXLINE] = {};

    fgets(str, sizeof(str), stdin);
    check_num = is_str_is_num(str);

    while (!check_num) {

        printf("Wrong type, its not ONLY number, write again: ");
        fgets(str, sizeof(str), stdin);
        check_num = is_str_is_num(str);

    }

    sscanf(str, "%lg", &num);
    return num;
}

bool is_str_is_num(char *str) {
    int count_minus = 0, count_dots = 0;
    bool if_otriz = 0;

    if (str[0] == '-')
        if_otriz++;
    if (*str == '\n' || str[0] == '.')
        return false;

    for (int i = 1; str[i] != '\0'; i++) {
        if (str[i - 1] == '.') {
            count_dots++;
            continue;
        }
        else if (str[i - 1] == '-') {
            count_minus++;
            continue;
        }
        else if ((isdigit(str[i - 1]) == 0)) {
            return false;
        }
    }

    if (count_dots > 1 || (if_otriz && count_minus > 1) || (!if_otriz && count_minus > 0))
        return false;

    return true;
}


bool check_to_go(void) {
    int type_letter;
    printf("If you want to go another solving, type \"yes\" or \"no\": ");
    type_letter = getchar();

    while (type_letter != 'y' and type_letter != 'n') {

        printf("Write only \"yes\" or \"no\" ");
        while (getchar() != '\n') {;}
        type_letter = getchar();

    }
    if (type_letter == 'y') {
        while (getchar() != '\n') {;}
        return false;
    }

    while (getchar() != '\n') {;}
    return true;
}

double solve_one_root(double a, double b) {
    return -b / a;
}

enum rootCount calc_roots(double a, double b, double c, double *x1, double *x2) {
    double d = (b * b) - (a * c * 4);

    if (!is_zero(a)) {

        if (is_zero(c)) {
            *x1 = solve_one_root(a, b);
            return TWO_ROOTS;
            }

        if (d > 0) {
            *x1 = (-b + sqrt(d)) / (2 * a);
            *x2 = (-b - sqrt(d)) / (2 * a);
            return TWO_ROOTS;
            }

        else if (is_zero(d)) {
            *x1 = -b / (2 * a);
            return ONE_ROOT;
            }

        else {
            return NO_ROOTS;
            }
    }

    else {
        if (is_zero(b)) {
            if (is_zero(c)) {
                return UNLIMITED_ROOTS;
                }
            else  {
                return NO_ROOTS;
                }
        }

        else {
            *x1 = solve_one_root(b, c);
            return ONE_ROOT;
            }
        }
}

bool is_zero(double num) {

    num = fabs(num);
    if (num <= EPS) {
        return true;
    }
    return false;
}


void roots_out(enum rootCount counter, double x1, double x2) {
    switch (counter) {
        case UNLIMITED_ROOTS:
            printf("Unlimited roots\n");
            break;

        case TWO_ROOTS:
            printf("Your first root: %0.10lg, and second root: %0.10lg\n", x1, x2);
            break;

        case ONE_ROOT:
            printf("Your one root: %0.10lg\n", x1);
            break;

        case NO_ROOTS:
            printf("0 roots\n");
            break;

        default:
            break;
    }
}
