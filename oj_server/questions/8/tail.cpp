#ifndef COMPILE_ONLINE
#include "header.cpp"
#endif // !COMPILE_ONLINE

void Test1()
{
    string ans = Solution().minWindow("ADOBECODEBANC", "ABC");
    if (ans == "BANC")
    {
        std::cout << "You pass the Example 1" << std::endl;
    }
    else
    {
        std::cout << "You can not pass the Example 1" << std::endl;
    }
}

void Test2()
{
    string ans = Solution().minWindow("a", "a");
    if (ans == "a")
    {
        std::cout << "You pass the Example 2" << std::endl;
    }
    else
    {
        std::cout << "You can not pass the Example 2" << std::endl;
    }
}

void Test3()
{
    string ans = Solution().minWindow("a", "aa");
    if (ans == "")
    {
        std::cout << "You pass the Example 3" << std::endl;
    }
    else
    {
        std::cout << "You can not pass the Example 3" << std::endl;
    }
}   

int main()
{
    Test1();
    Test2();
    Test3();

    return 0;
}