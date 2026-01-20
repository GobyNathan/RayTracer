#define CRITERION_DISABLE_LOGGING
#include <criterion/criterion.h>
#include <criterion/logging.h>

int main(int argc, char* argv[])
{
    // Initialize criterion and run the tests
    struct criterion_test_set* tests = criterion_initialize();

    int result = 0;
    if (criterion_handle_args(argc, argv, true))
        result = criterion_run_all_tests(tests);

    criterion_finalize(tests);
    return result;
}
