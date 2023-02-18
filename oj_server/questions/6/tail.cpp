#ifndef COMPILE_ONLINE
#include "header.cpp"
#endif // !COMPILE_ONLINE

void Test1()
{
    double ans = Solution().myPow(2.00000, 10);
    if (ans == 1024.00000)
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
    double ans = Solution().myPow(2.10000, 3);
    if (ans == 9.26100)
    {
        std::cout << "You pass the Example 2" << std::endl;
    }
    else
    {
        std::cout << "You can not pass the Example 2" << std::endl;
    }
}
    

int main()
{
    Test1();
    Test2();

    return 0;
}