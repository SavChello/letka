#include "init.h"


int unit_random() {
    int count_right_solves = 0, count_tests_withroots = 0;
    double x1 = NAN, x2 = NAN, rand_a = NAN, rand_b = NAN, rand_c = NAN;
    double solve1 = NAN, solve2 = NAN;
    enum rootCount count_roots = INIT;

    printf(SAND "Ok, here your UNIT tests! Lets check it:\n" DEFAULT);
    neuroslop("making random unit tests");
    srand(time(NULL));

    for (int i = 1; i <= COUNT_RANDOM_TESTS; i++) {
        x1 = NAN, x2 = NAN;
        solve1 = NAN, solve2 = NAN;

        rand_a = randoms(); //[-1e14, 1e14]
        rand_b = randoms();
        rand_c = randoms();
        //printf("%0.10lg %0.10lg %0.10lg\n", rand_a, rand_b, rand_c);
        count_roots = calc_roots(rand_a, rand_b, rand_c, &x1, &x2);

        switch (count_roots) {
            case TWO_ROOTS:
                count_tests_withroots++;
                solve1 = rand_a * x1 * x1 + rand_b * x1 + rand_c;
                solve2 = rand_a * x2 * x2 + rand_b * x2 + rand_c;

                if (is_zero_rand(solve1) && is_zero_rand(solve2))
                    count_right_solves++;
                else
                    printf("test <%d/%d> wrong: first solving "
                    "give [%lf], second solving give [%lf]\n", i, COUNT_RANDOM_TESTS, solve1, solve2);
                break;

            case ONE_ROOT:
                count_tests_withroots++;
                solve1 = rand_a * x1 * x1 + rand_b * x1 + rand_c;

                if (is_zero_rand(solve1))
                    count_right_solves++;
                else
                    printf("test <%d/%d> wrong: solving give [%lf]\n", i, COUNT_RANDOM_TESTS, solve1);
                break;

            case UNLIMITED_ROOTS:
            case NO_ROOTS:
            case INIT:
                break;

            default:
                break;
        }

    }

    printf("Random units " GREEN "right " DEFAULT "in "
            RED "{%d} " DEFAULT "from " RED "{%d} " DEFAULT
            "solves, where exists roots\n", count_right_solves, count_tests_withroots);
    return 0;
}


double randoms() {
    double k = (double)rand() / RAND_MAX;
    return k * k * k * (MY_RAND_MAX - MY_RAND_MIN) + MY_RAND_MIN;
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
    struct ReadNumsUnits test[MAXLINE] = {{0, 0, 0, 0, 0, 0}};
    int count_right_tests = 0;
    int i = 0;

    neuroslop("units from UNIT_test.txt");

    FILE *file = fopen("UNIT_test.txt", "rb");
    while (fgets(unit_str, sizeof(unit_str), file) != NULL) {
        test[i].x1ref = NAN;
        test[i].x2ref = NAN;
        sscanf(unit_str, "%lg %lg %lg %d %lg "
            "%lg", &test[i].a, &test[i].b, &test[i].c, &test[i].real_count_roots, &test[i].x1ref, &test[i].x2ref);
        i++;
    }
    fclose(file);

    i--;
    for (int j = 0; j < i; j++) {
        if (run_test_nums(test[j]))
            count_right_tests++;
        else
            printf("Test {%d} was " RED "FAILED \n" DEFAULT, j);
    }

    printf("Unit tests " GREEN "right" DEFAULT " in "
           RED "{%d}" DEFAULT" from " RED"{%d} " DEFAULT "checks\n", count_right_tests, i);
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
    struct ReadStrUnits test[MAXLINE] = {{"0", "0", "0", 0}};
    int right_tests = 0;
    int i = 0;
    char unit_str[MAXLINE] = {};

    neuroslop("unit test on checking STR is NUMBERS");

    FILE *file2 = fopen("UNIT_test_CHECK_NUM.txt", "r");

    while (fgets(unit_str, sizeof(unit_str), file2) != NULL) {
        sscanf(unit_str, "%s %s %s %d", test[i].a, test[i].b, test[i].c, &test[i].right_test);
        i++;
    }
    fclose(file2);

    i--;
    for (int j = 0; j < i; j++) {
        if (test[j].right_test == (is_str_is_num(test[j].a) && is_str_is_num(test[j].b) && is_str_is_num(test[j].c)))
            right_tests++;
    }

    printf("Program was read " GREEN "right " RED "{%d}" DEFAULT" from "
            RED "{%d} " DEFAULT "unit test inputs\n\n", right_tests, i);
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
            if (count_roots == test.real_count_roots &&
                    is_equal(counted_x1, test.x1ref) && is_equal(counted_x2, test.x2ref))
                check_right++;
            break;

        case ONE_ROOT:
            if (count_roots == test.real_count_roots && is_equal(counted_x1, test.x1ref) && isnan(counted_x2))
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


bool is_equal(double x, double y) {
    double num = x - y;
    num = fabs(num);
    if (num <= EPS) {
        return true;
    }
    return false;
}


bool is_zero_rand(double num) {

    num = fabs(num);
    if (num <= RAND_EPS) {
        return true;
    }
    return false;
}
