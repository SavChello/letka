#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * @brief EPS - The recommended error margin for calculations.
 */
const double EPS = 1e-9;

/**
 * @brief Statuses for the number of equation roots.
 * Used when comparing double values or their differences to zero to account for floating-point inaccuracies.
 */
enum rootCount{
        INIT = -1,              ///< For initialization, for check program's mistakes
        NO_ROOTS = 0,           ///< No real roots, may D < 0
        ONE_ROOT = 1,           ///< One distinct root, D = 0
        TWO_ROOTS = 2,          ///< Two distinct root, D > 0
        UNLIMITED_ROOTS = 3     ///< Any real number is solution
};

/**
 * @brief Unit test case data
 */
struct ReadNumsUnits {
        double a, b, c;         ///< Three correctly provided arguments of type double
        int real_count_roots;   ///< The number of roots
        double x1ref, x2ref;    ///< The expected correct and ordered roots: two roots, or one if x2ref is NAN
};


int     unit_test_arrays();
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

/**
 * @brief The main function executes in the following sequence:
 *
 * 1. Initializes roots and coefficients with NAN to easily detect if the calculation fails.
 * 2. Runs three unit tests: testing with data from an array, verifying root calculations, and validating string-to-number input parsing.
 * 3. Enters a do-while loop that calls functions to: input the coefficients, calculate the roots and their count, and print the results.
 * 4. Waits for the user to input "yes" or "no" to determine whether to continue or exit the program.
 *
 */
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


/**
 * @brief A unit testing function that reads from an array and performs complex checks.
 * @details It allows you to easily copy-paste heavy, third-party tests to rigorously verify the root calculation logic.
 *
 */
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


/**
 * @brief Root Validation Unit Test Function
 * @details The execution flow is as follows:
 * Performs line-by-line reading from a file that contains the predetermined, ordered roots (or NAN).
 * Populates a test structure with the required parsed variables.
 * Executes the structure validation function: run_test_nums(test).
 * At the end, prints the total number of tests the program passed correctly.
 *
 */
int unit_solving_test() {
    char unit_str[MAXLINE] = {};
    int count_strs = 0, count_right_tests = 0;
    struct ReadNumsUnits test;
    test.x1ref = NAN;
    test.x2ref = NAN;

    neuroslop("units from UNIT_test.txt");

    FILE *file = fopen("UNIT_test.txt", "r");

    fgets(unit_str, sizeof(unit_str), file);
    while (sscanf(unit_str, "%lf %lf %lf %d %lf %lf",
                    &test.a, &test.b, &test.c, &test.real_count_roots, &test.x1ref, &test.x2ref) > 0) {
        count_strs++;
        if (run_test_nums(test)) {
            count_right_tests++;
        }
        else
            printf("Test {%d} was " RED "FAILED \n" DEFAULT, count_strs);

        test.x1ref = NAN;
        test.x2ref = NAN;
        fgets(unit_str, sizeof(unit_str), file);
    }

    printf("Unit tests " GREEN "right" DEFAULT " in "
           RED "{%d}" DEFAULT" from " RED"{%d} " DEFAULT "checks\n", count_right_tests, count_strs);
    return 0;
}


/**
 * @brief Second Unit Test: String Validation
 * @details Reads three potential input strings line-by-line.
 * Uses the is_str_is_num() function to check if each string evaluates to a valid number.
 * Prints the final count of correctly parsed strings.
 *
 */
int unit_check_str_isnum() {
    char input_a[MAXLINE] = {};
    char input_b[MAXLINE] = {};
    char input_c[MAXLINE] = {};
    int ans = 0;
    int right_tests = 0, all_tests = 0;
    char unit_str[MAXLINE] = {};

    neuroslop("unit test on checking STR is NUMBERS");

    FILE *file2 = fopen("UNIT_test_CHECK_NUM.txt", "r");
    fgets(unit_str, sizeof(unit_str), file2);

    while (sscanf(unit_str, "%s %s %s %d", input_a, input_b, input_c, &ans) > 0) {
        if (ans == (is_str_is_num(input_a) & is_str_is_num(input_b) & is_str_is_num(input_c)))
            right_tests++;
        all_tests++;
        fgets(unit_str, sizeof(unit_str), file2);
     }

    printf("Program was read " GREEN "right " RED "{%d}" DEFAULT" from "
            RED "{%d} " DEFAULT "unit test inputs\n", right_tests, all_tests);
    neuroslop("starting input");

    return 0;
}


/**
 * @brief Executes a single unit test for root calculation, following the same logic as the main program.
 * @details It suppresses output for successful tests, printing only the failed ones.
 * Returns an error status, or the correctly computed roots along with their count.
 *
 * @return bool
 */
bool run_test_nums(struct ReadNumsUnits test) {
    double counted_x1 = NAN, counted_x2 = NAN;
    int count_roots = 0;
    bool check_right = 0;

    count_roots = calc_roots(test.a, test.b, test.c, &counted_x1, &counted_x2);

    switch (test.real_count_roots) {
        case TWO_ROOTS:
            sort_x(&counted_x1, &counted_x2);
            if (count_roots == test.real_count_roots && is_zero(counted_x1 - test.x1ref) && is_zero(counted_x2 - test.x2ref))
                check_right++;
            break;

        case ONE_ROOT:
            if (count_roots == test.real_count_roots && is_zero(counted_x1 - test.x1ref) && isnan(counted_x2))
                check_right++;
            break;

        case UNLIMITED_ROOTS:
        case NO_ROOTS:
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


/**
 * @brief A function that retrieves the three coefficients of a quadratic equation.
 * @details After processing, it passes their values back to the calling function via pointers.
 *
 * @param[out] a Coefficient {a}
 * @param[out] b Coefficient {b}
 * @param[out] c Coefficient {c}
 */
int roots_int(double *a, double *b, double *c) {
    double internal_a = 0.0, internal_b = 0.0, internal_c = 0.0;

    printf(SAND "If you want to solve ");
    printf("quadratic equations, such as ax^2 + bx + c = 0, type 3 numbers:\n\n" DEFAULT);
//закинуть принт в функцию
    printf("Please, print {a}: ");
    internal_a = read_number('a');
    printf("Please, print {b}: ");
    internal_b = read_number('b');
    printf("Please, print {c}: ");
    internal_c = read_number('c');

    *a = internal_a;
    *b = internal_b;
    *c = internal_c;

    return 0;
}

/**
 * @brief Prompts the user for valid numeric input.
 * @details If the first attempt contains anything other than a double, a continuous loop is triggered,
 * prompting the user until a valid number is successfully parsed.
 *
 * @param[in] argue Input coefficient name
 * @return double Required coefficient value
 */
double read_number(char argue) {
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

/**
 * @brief String-to-double validation function
 *
 * @param[in] str An input string read from the console or a file for a unit test
 * @return true The string is double
 * @return false The string is not double
 */
bool is_str_is_num(char *str) {

    char *end_of_str = {};
    strtod(str, &end_of_str);

    if (*str == '\n' || (*end_of_str != '\n' && *end_of_str != '\0') || str == NULL || str == end_of_str)
        return false;
    return true;
}

/**
 * @brief A function that returns the solution to a linear equation of the form ax + b = 0
 *
 * @param[in] a Coefficient a
 * @param[in] b Coefficient b
 * @return double The single solution: root x
 */
double solve_one_root(double a, double b) {
    return -b / a;
}

/**
 * @brief A root calculation function that returns a value of type enum rootCount
 * @details After calculating the discriminant, a check is performed within a nested loop.
 * If there are two roots, they are passed back to main via pointers x1 and x2.
 * If there is only one root, it is passed via the x1 pointer alone.
 *
 * @param[in] a Coefficient a
 * @param[in] b Coefficient b
 * @param[in] c Coefficient c
 * @param[out] x1 Root 1
 * @param[out] x2 Root 2
 * @return enum rootCount The number of roots
 */
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


/**
 * @brief Root output function
 * @details A switch statement evaluates the enum rootCount to output the calculated roots and their total count.
 *
 * @param[in] counter Count roots
 * @param[in] x1 Root 1
 * @param[in] x2 Root 2
 */
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

/**
 * @brief A function that checks if the user wants to continue solving equations.
 * @details If the input is anything other than 'yes' or 'no',
 * the user is continuously prompted to enter strictly one of these two strings.
 *
 * @return true If 'yes' is entered, the loop in main restarts.
 * @return false If 'no' is entered, the loop terminates.
 */
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

/**
 * @brief A utility function that sorts two numbers.
 * @details It is used in unit tests to prevent the test from failing if the program returns the roots unordered.
 *
 * @param[in, out] x Pointer to the first value.
 * @param[in, out] y Pointer to the second value.
 * @return int
 */
int sort_x(double *x, double *y) {
    double temp = 0.0;
    if (*x > *y) {
        temp = *x;
        *x = *y;
        *y = temp;
    }
    return 0;
}

/**
 * @brief A function that determines whether a number, or the difference between two numbers,
 * is within the recommended precision tolerance (EPS).
 *
 * @param[in] num А number, or the difference between two numbers
 * @return true If the absolute value of the number is less than EPS, it can be considered zero.
 * @return false This value cannot be considered approximately zero.
 */
bool is_zero(double num) {

    num = fabs(num);
    if (num <= EPS) {
        return true;
    }
    return false;
}

/**
 * @brief A function that elegantly displays text using a simulated AI response effect.
 *
 * @param[in] c The response string to be displayed.
 */
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
