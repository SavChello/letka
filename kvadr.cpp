#include "init.h"

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

    printf(SAND "NeuroSlop understanding you. You want to solve quadratic equations. Let's goo..\n\n" DEFAULT);

    unit_random();
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
 * @brief A function that retrieves the three coefficients of a quadratic equation.
 * @details After processing, it passes their values back to the calling function via pointers.
 *
 * @param[out] a Coefficient {a}
 * @param[out] b Coefficient {b}
 * @param[out] c Coefficient {c}
 */
int roots_int(double *a, double *b, double *c) {
    printf(SAND "If you want to solve quadratic equations, such as ax^2 + bx + c = 0, type 3 numbers:\n\n" DEFAULT);

    *a = read_number('a');
    *b = read_number('b');
    *c = read_number('c');

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

    printf("Please, print {%c}: ", argue);

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


    if (!is_zero(a)) {
        double d = (b * b) - (a * c * 4);

        if (d > EPS) {
            double solve_sqrt = sqrt(d);
            *x1 = (-b - solve_sqrt) / (2 * a);
            *x2 = (-b + solve_sqrt) / (2 * a);
            return TWO_ROOTS;
            }

        else if (is_zero(d)) {
            *x1 = -b / (2 * a);
            return ONE_ROOT;
            }

        else
            return NO_ROOTS;
    }

    else {
        if (is_zero(b)) {
            if (is_zero(c))
                return UNLIMITED_ROOTS;
            else
                return NO_ROOTS;
        }

        else {
            *x1 = -c / b;
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
            printf("Look, your first root: " RED "%0.10lf" DEFAULT ", and second root: " RED "%0.10lf\n\n" DEFAULT, x1, x2);
            break;

        case ONE_ROOT:
            printf("Check, here your only one root: " RED "%0.10lf\n\n" DEFAULT, x1);
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
    fgets(type_str, sizeof(type_str), stdin);

    while (strncmp(type_str, "yes", 3) != 0 && strncmp(type_str, "no", 2) != 0) {
        printf(SAND "Write only " GREEN "\"yes\" " SAND "or " RED "\"no\" " DEFAULT);
        fgets(type_str, sizeof(type_str), stdin);
    }

    if (strncmp(type_str, "yes", 3) == 0) {
        neuroslop("start");
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
int sort_x(double *x1, double *x2) {
    double temp = 0.0;
    if (*x1 > *x2) {
        temp = *x1;
        *x1 = *x2;
        *x2 = temp;
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
