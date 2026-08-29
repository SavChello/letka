#include "init.h"



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
