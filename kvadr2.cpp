#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


const double EPS = 1e-9;

enum rootCount{
        INIT = -1,
        NO_ROOTS = 0,
        ONE_ROOT = 1,
        TWO_ROOTS = 2,
        UNLIMITED_ROOTS = 3
};

struct ReadNumsUnits {
        double a, b, c;
        int real_count_roots;
        double x1ref, x2ref;
};



int     unit_test_arrays();
int     unit_solving_test();
int     unit_check_str_isnum();
bool    run_test_nums(struct ReadNumsUnits test);

int     roots_int(double *a, double *b, double *c);
double  new_read_number(char argue);
bool    is_str_is_num(char *str);
double  solve_one_root(double a, double b);
enum    rootCount calc_roots(double a, double b, double c, double *x1, double *x2);

int     roots_out(enum rootCount counter, double x1, double x2);
bool    check_to_go();

int     sort_x(double *x, double *y);
bool    is_zero(double num);
void    neuroslop(const char* c);



#define MAXLINE 100
#define PRINT_WRONG_UNIT(unit_a, unit_b, unit_c, real_count_roots, x1ref, x2ref, x_1, x_2) (printf("\n[a] = [%lg], [b] = [%lg], [c] = [%lg]\nExpected %d roots: [x1ref] = [%lg], [x2ref] = [%lg]\nGott roots: [x1] = [%lg], [x2] = [%lg]\n\n", unit_a, unit_b, unit_c, real_count_roots, x1ref, x2ref, x_1, x_2))
#define TIME_SLEEP 0


//-----------------------------------------------------------------------------


#define DEFAULT     "\x1b[0m"
#define RED         "\x1b[31m"
#define SAND        "\033[38;5;137m"
#define BACK_SAND   "\033[48;5;223m"
#define GREEN       "\e[0;32m"
#define MAGENTA     "\033[35m"
#define BLUE        "\e[0;36m"


//=============================================================================

//int main(int argc, char* argv[]) {
int main() {
    double x1 = NAN, x2 = NAN;
    double a = NAN, b = NAN, c = NAN;
    rootCount count_roots = INIT;

    printf(SAND "NeuroSlop understanding you. You want to solve quadratic equations. Let's goo..\n" DEFAULT);

    unit_test_arrays();
    unit_solving_test();
    unit_check_str_isnum();


    do {
        roots_int(&a, &b, &c);
        count_roots = calc_roots(a, b, c, &x1, &x2);
        roots_out(count_roots, x1, x2);

    } while(!check_to_go());

    printf(DEFAULT "Thanks for using our NeuroSlop");
    return 0;
}


int unit_test_arrays() {
    printf(SAND "\nOk, here your UNIT tests! Lets check it:\n" DEFAULT);
    neuroslop("units from Array");

    int count = 0;
    struct ReadNumsUnits test[] = {{.a = 0, .b = 1, .c = 0, .real_count_roots = 1, .x1ref = 0,  .x2ref = NAN},
                                   {.a = 1, .b = 2, .c = 1, .real_count_roots = 1, .x1ref = -1, .x2ref = NAN}};
    int count_tests = sizeof(test) / sizeof(test[0]);

    for (int i = 0; i < count_tests; i++) {
        if (run_test_nums(test[i]))
            count++;
    }
    printf("From" RED " arrays " DEFAULT
           "Program check " GREEN "right " RED "{%d} " DEFAULT
           "from " RED "{%d} " DEFAULT "inputs\n", count, count_tests);
    return 0;
}


// /**
int unit_solving_test() {
    char unit_str[MAXLINE] = {};
    int i = 0, count_right_tests = 0;
    struct ReadNumsUnits test;
    test.x1ref = NAN;
    test.x2ref = NAN;

    neuroslop("units from UNIT_test.txt");

    FILE *fpp = fopen("UNIT_test.txt", "r");

    fgets(unit_str, sizeof(unit_str), fpp);
    while (sscanf(unit_str, "%lf %lf %lf %d %lf %lf",
                    &test.a, &test.b, &test.c, &test.real_count_roots, &test.x1ref, &test.x2ref) > 0) {
        i++;
        if (run_test_nums(test)) {
            count_right_tests++;
        }
        else
            printf("Test {%d} was " RED "FAILED \n" DEFAULT, i);

        test.x1ref = NAN;
        test.x2ref = NAN;
        fgets(unit_str, sizeof(unit_str), fpp);
    }

    printf("Unit tests " GREEN "right" DEFAULT " in "
           RED "{%d}" DEFAULT" from " RED"{%d} " DEFAULT "checks\n", count_right_tests, i);
    return 0;
}


int unit_check_str_isnum() {
    char input_a[MAXLINE] = {};
    char input_b[MAXLINE] = {};
    char input_c[MAXLINE] = {};
    int ans = 0;
    int right_tests = 0, all_tests = 0;
    char unit_str[MAXLINE] = {};

    neuroslop("unit test on checking STR is NUMBERS");

    FILE *fpp = fopen("UNIT_test_CHECK_NUM.txt", "r");
    fgets(unit_str, sizeof(unit_str), fpp);

    while (sscanf(unit_str, "%s %s %s %d", input_a, input_b, input_c, &ans) > 0) {
        if (ans == (is_str_is_num(input_a) & is_str_is_num(input_b) & is_str_is_num(input_c)))
            right_tests++;
        all_tests++;
        fgets(unit_str, sizeof(unit_str), fpp);
     }

    printf("Program was read " GREEN "right " RED "{%d}" DEFAULT" from "
            RED "{%d} " DEFAULT "unit test inputs\n", right_tests, all_tests);
    neuroslop("starting input");

    return 0;
}

bool run_test_nums(struct ReadNumsUnits test) {
    double counted_x1 = NAN, counted_x2 = NAN;
    int count_roots = 0;
    bool check_right = 0;

    count_roots = calc_roots(test.a, test.b, test.c, &counted_x1, &counted_x2);

    switch (test.real_count_roots) {
        case 2:
            sort_x(&counted_x1, &counted_x2);
            if (count_roots == test.real_count_roots && is_zero(counted_x1 - test.x1ref) && is_zero(counted_x2 - test.x2ref))
                check_right++;
            break;

        case 1:
            if (count_roots == test.real_count_roots && is_zero(counted_x1 - test.x1ref) && isnan(counted_x2))
                check_right++;
            break;

        case 3:
        case 0:
            if (count_roots == test.real_count_roots && isnan(counted_x1) && isnan(counted_x2))
                check_right++;
            break;

        default:
            break;
        }
    if (!check_right)
        PRINT_WRONG_UNIT(test.a, test.b, test.c, test.real_count_roots, test.x1ref, test.x2ref, counted_x1, counted_x2);

    return check_right;
}


int roots_int(double *a, double *b, double *c) {
    double internal_a = 0.0, internal_b = 0.0, internal_c = 0.0;

    printf(SAND "If you want to solve ");
    printf("quadratic equations, such as ax^2 + bx + c = 0, type 3 numbers:\n\n" DEFAULT);

    printf("Please, print {a}: ");
    internal_a = new_read_number('a');
    printf("Please, print {b}: ");
    internal_b = new_read_number('b');
    printf("Please, print {c}: ");
    internal_c = new_read_number('c');

    *a = internal_a;
    *b = internal_b;
    *c = internal_c;

    return 0;
}

double new_read_number(char argue) {
    bool check_num = 0;
    double num = 0.0;
    char str[MAXLINE] = {};

    fgets(str, sizeof(str), stdin);
    check_num = is_str_is_num(str);

    while (!check_num) {
        neuroslop("input");
        printf("Sorry, your input is not ONLY number, please, write {%c} again: ", argue);

        fgets(str, sizeof(str), stdin);
        check_num = is_str_is_num(str);

    }
//scanf
    sscanf(str, "%lg", &num);

    return num;
}


bool is_str_is_num(char *str) {

    char *end_of_str = {};
    strtod(str, &end_of_str);

    if (*str == '\n' || (*end_of_str != '\n' && *end_of_str != '\0') || str == NULL || str == end_of_str)
        return false;
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

        if (d > EPS) {
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


int roots_out(enum rootCount counter, double x1, double x2) {
    neuroslop("solving");

    switch (counter) {
        case UNLIMITED_ROOTS:
            printf("You see, any number works; the number of roots is " RED "infinite!\n\n" DEFAULT);
            break;

        case TWO_ROOTS:
            printf("Look, your first root: " RED "%0.10lg" DEFAULT ", and second root: " RED "%0.10lg\n\n" DEFAULT, x1, x2);
            break;

        case ONE_ROOT:
            printf("Check, here your only one root: " RED "%0.10lg\n\n" DEFAULT, x1);
            break;

        case NO_ROOTS:
            printf("Unfortunately, the equation has " RED "0 roots" DEFAULT". Try different arguments.\n\n");
            break;
        case INIT:
            printf("Oops, Neuroslop was solving nothing..\n\n");
            break;

        default:
            break;
    }

    return 0;
}


bool check_to_go() {
    char type_str[MAXLINE] = {};
    printf(SAND "If you want to go another solving, type " GREEN "\"yes\" " SAND "or " RED "\"no\": " DEFAULT);

    while (!strcmp(type_str, "yes") || !strcmp(type_str, "no")) {
        fgets(type_str, sizeof(type_str), stdin);
        printf(SAND "Write only " GREEN "\"yes\" " SAND "or " RED "\"no\" " DEFAULT);

    }

    neuroslop("start");
    if (strcmp(type_str, "yes")) {
        return false;
    }
    return true;
}


int sort_x(double *x, double *y) {
    double temp = 0.0;
    if (*x > *y) {
        temp = *x;
        *x = *y;
        *y = temp;
    }
    return 0;
}


bool is_zero(double num) {

    num = fabs(num);
    if (num <= EPS) {
        return true;
    }
    return false;
}


void neuroslop(const char *c) {
    printf(BLUE "\n  *");
    txSleep(TIME_SLEEP);
    printf("  *");
    txSleep(TIME_SLEEP);
    printf("  *  <thinking on %s>....", c);
    txSleep(TIME_SLEEP);
    printf("  *");
    txSleep(TIME_SLEEP);
    printf("  *  ");
    txSleep(TIME_SLEEP);
    printf("*  \n\n" DEFAULT);
}
