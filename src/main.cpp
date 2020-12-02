#include "../include/test.h"


// main function, testing components
int main()
{
    testUtility();
    testMemory();
    testVector();
    testList();
    std::cout << test_pass << "/" << test_count 
        << " (passed " << test_pass * 100.0 / test_count << "%)" << std::endl;

    return 0;
}