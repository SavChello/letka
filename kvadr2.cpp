#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//git
//sscanf
// remove chars

struct ReadUnits {
    char *a, *b, *c;
    int real_count_roots;
    double x1ref, x2ref;
};

const double EPS = 1e-6;

enum rootCount{
        NO_ROOTS = 0,
        ONE_ROOT = 1,
        TWO_ROOTS = 2,
        UNLIMITED_ROOTS = 3
};

void    unit_test_txt(void);
void    unit_test_hards(void);
void    run_one_test(struct ReadUnits test);
void    roots_int(double* a, double* b, double* c);
void    sort_x(double *x, double *y);
enum    rootCount calc_roots(double a, double b, double c, double* x1, double* x2);
bool    check_to_go(void);
void    roots_out(enum rootCount counter, double x1, double x2);
double  solve_one_root(double a, double b);
bool    is_zero(double num);
bool    is_str_is_num(char *str);
double  new_read_number(char argue);
void    neuroslop(char *c);

#define MAXLINE 1000
#define PRINT_UNIT(unit_a, unit_b, unit_c, real_count_roots, x1ref, x2ref, x_1, x_2) (printf("\n[a] = [%lg], [b] = [%lg], [c] = [%lg]\nExpected %d roots: [x1ref] = [%lg], [x2ref] = [%lg]\nGott roots: [x1] = [%lg], [x2] = [%lg]\n\n", unit_a, unit_b, unit_c, real_count_roots, x1ref, x2ref, x_1, x_2))
#define PRINT_WRONG_UNIT(unit_a, unit_b, unit_c, real_count_roots, x1ref, x2ref, x_1, x_2) (printf("\n[a] = [%s], [b] = [%s], [c] = [%s]\nExpected %d roots: [x1ref] = [%lg], [x2ref] = [%lg]\nGott roots: [x1] = [%lg], [x2] = [%lg]\n\n", unit_a, unit_b, unit_c, real_count_roots, x1ref, x2ref, x_1, x_2))
#define TIME_SLEEP 250


//-----------------------------------------------------------------------------


#define DEFAULT     "\x1b[0m"
#define RED         "\x1b[31m"
#define SAND        "\033[38;5;137m"
#define BACK_SAND   "\033[48;5;223m"
#define GREEN       "\e[0;32m"
#define MAGENTA     "\033[35m"


//=============================================================================

//int main(int argc, char* argv[]) {
int main() {
    double x1 = 0.0, x2 = 0.0;
    double a = 0.0, b = 0.0, c = 0.0;
    rootCount count_roots = NO_ROOTS;

    printf(SAND "Ok, NeuroSlop understanding you. You want to solve quadratic equations. Let's goo..\n" DEFAULT);

    unit_test_hards();
    unit_test_txt();


    printf(SAND "If you want to solve ");
    printf("quadratic equations, such as ax^2 + bx + c = 0, type 3 numbers:\n\n" DEFAULT);

    while (1) {

        roots_int(&a, &b, &c);
        count_roots = calc_roots(a, b, c, &x1, &x2);
        roots_out(count_roots, x1, x2);

        if (check_to_go()) {
            break;
        }
    }

    printf(DEFAULT "Thanks for using our NeuroSlop");
    return 0;
}

void neuroslop(char *c) {
    printf("\e[0;36m\n  *");
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


void unit_test_hards(void) {
    neuroslop("units from Array");

    struct ReadUnits test[] = {{.a = "0", .b = "1", .c = "0", .real_count_roots = 1, .x1ref = 0, .x2ref = NAN},
                {.a = "1", .b = "2", .c = "1", .real_count_roots = 1, .x1ref = -1, .x2ref = NAN}};
    int size = sizeof(test) / sizeof(test[0]);

    for (int i = 0; i < size; i++) {
        printf(MAGENTA "Test from array {%d} was " DEFAULT, i + 1);
        run_one_test(test[i]);
    }

}


void unit_test_txt(void) {
    char a[MAXLINE] = {};
    char b[MAXLINE] = {};
    char c[MAXLINE] = {};
    char x1[MAXLINE] = {};
    char x2[MAXLINE] = {};
    int n_roots = 0, i = 1;
    double return_x1 = 0.0, return_x2 = 0.0;

    neuroslop("units from UNIT_test.txt");
    printf(SAND "Ok, here your UNIT tests! Lets check it:\n\n" DEFAULT);

    FILE *fpp = fopen("UNIT_test.txt", "r");

    while (fscanf(fpp, "%s %s %s %d %s %s", a, b, c, &n_roots, x1, x2) == 6) {

        if (!strcmp(x1, "NAN"))
            return_x1 = NAN;
        else
            sscanf(x1, "%lf", &return_x1);

        if (!strcmp(x2, "NAN"))
            return_x2 = NAN;
        else
            sscanf(x2, "%lf", &return_x2);

        struct ReadUnits test = {
            .a = a, .b = b, .c = c, .real_count_roots = n_roots,
            .x1ref = return_x1, .x2ref = return_x2};

        printf("Test {%d} was ", i);
        i++;
        run_one_test(test);

     }

     neuroslop("starting input");
}


void run_one_test(struct ReadUnits test) {
    double x_1 = NAN, x_2 = NAN;
    double unit_a = 0.0, unit_b = 0.0, unit_c = 0.0;
    enum rootCount count_roots = NO_ROOTS;
    bool check_right = 0;

    if (is_str_is_num(test.a) && is_str_is_num(test.b) && is_str_is_num(test.c)) {
        sscanf(test.a, "%lg", &unit_a);
        sscanf(test.b, "%lg", &unit_b);
        sscanf(test.c, "%lg", &unit_c);

        count_roots = calc_roots(unit_a, unit_b, unit_c, &x_1, &x_2);

        switch (test.real_count_roots) {
            case 2:
                sort_x(&x_1, &x_2);
                if (count_roots == test.real_count_roots && (x_1 - test.x1ref) < EPS && (x_2 - test.x2ref) < EPS)
                    check_right++;
                break;

            case 1:
                if (count_roots == test.real_count_roots && (x_1 - test.x1ref) < EPS && isnan(x_2))
                    check_right++;
                break;

            case 0:
                if (count_roots == test.real_count_roots && isnan(x_1) && isnan(x_2))
                    check_right++;
                break;

            default:
                break;
        }

        if (check_right)
            printf(RED "RIGHT " DEFAULT);

        else
            printf(RED "FAILED" DEFAULT ", but type is Right ");

        PRINT_UNIT(unit_a, unit_b, unit_c, test.real_count_roots, test.x1ref, test.x2ref, x_1, x_2);
    }

    else {
        printf(RED "FAILED" DEFAULT ", type is WRONG ");
        PRINT_WRONG_UNIT(test.a, test.b, test.c, test.real_count_roots, test.x1ref, test.x2ref, x_1, x_2);
    }
}

void sort_x(double *x, double *y) {
    double temp = 0.0;
    if (*x > *y) {
        temp = *x;
        *x = *y;
        *y = temp;
    }
}

void roots_int(double *a, double *b, double *c) {
    double internal_a = 0.0, internal_b = 0.0, internal_c = 0.0;

    printf("Please, print {a}: ");
    internal_a = new_read_number('a');
    printf("Please, print {b}: ");
    internal_b = new_read_number('b');
    printf("Please, print {c}: ");
    internal_c = new_read_number('c');

    *a = internal_a;
    *b = internal_b;
    *c = internal_c;

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


bool check_to_go(void) {
    int type_letter;
    printf(SAND "If you want to go another solving, type " GREEN "\"yes\" " SAND "or " RED "\"no\": " DEFAULT);
    type_letter = getchar();

    while (type_letter != 'y' and type_letter != 'n') {

        printf(SAND "Write only " GREEN "\"yes\" " SAND "or " RED "\"no\" " DEFAULT);
        while (getchar() != '\n') {;}
        type_letter = getchar();

    }
    neuroslop("start");
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


bool is_zero(double num) {

    num = fabs(num);
    if (num <= EPS) {
        return true;
    }
    return false;
}


void roots_out(enum rootCount counter, double x1, double x2) {
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

        default:
            break;
    }
}
