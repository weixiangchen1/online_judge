#ifndef COMPILE_ONLINE
#include "header.cpp"
#endif // !COMPILE_ONLINE

void Test1()
{
    int ans = Solution().reverse(120);
    if (ans == 21)
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
    int ans = Solution().reverse(-123);
    if (ans == -321)
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