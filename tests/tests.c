#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include "./include/parserTests.h"

//COLORS
#define RED "\033[31;1m"
#define GREEN "\033[32;1m"
#define YELLOW "\033[33;1m"
#define RESET "\033[37;1m"


int main()
{
    static int (*parser_tests[])() = {
        parserTests,
    };

    //Starting timer
    clock_t begin = clock();

    //Statistics variables
    int failed = 0;
    int test_count = 0;

    printf(YELLOW "Running PARSER tests...\n" RESET);
    int test_parser_count = sizeof(parser_tests) / sizeof(parser_tests[0]);

    for (int i = 0; i < test_parser_count; i++)
    {
        if (parser_tests[i]() != 0)
        {
            failed++;
            printf(RED "Parser tests n°%d FAILED!\n" RESET, i);
        }
        else
            printf(GREEN "Parser tests n°%d passed\n" RESET, i);
        test_count++;
    }
    printf("\n");

    clock_t end = clock();
    double elapsed = ((double)(end - begin)) / CLOCKS_PER_SEC;
    
    printf(GREEN "--- ENDING TESTS SUITES ---\n" RESET);
    if (failed == 0)
        printf(GREEN "ALL TESTS PASSED (%d tests)!\n" RESET, test_count);
    else
    {
        printf(RED "FAILED %d TESTS!\n" RESET, failed);
    }
    printf("%d TESTS PASSED!\n", test_count);
    printf("Time taken: %f seconds\n", elapsed);

    if (failed)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
