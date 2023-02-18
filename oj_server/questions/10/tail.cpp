#ifndef COMPILE_ONLINE
#include "header.cpp"
#endif // !COMPILE_ONLINE

void Test1()
{
    int ans = Solution().romanToInt("IV");
    if (ans == 4)
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
    int ans = Solution().romanToInt("LVIII");
    if (ans == 58)
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
    int ans = Solution().romanToInt("MCMXCIV");
    if (ans == 1994)
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