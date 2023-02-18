#ifndef COMPILE_ONLINE
#include "header.cpp"
#endif // !COMPILE_ONLINE

void Test1()
{
    int ans = Solution().lengthOfLastWord("Hello World");
    if (ans == 5)
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
    int ans = Solution().lengthOfLastWord("   fly me   to   the moon  ");
    if (ans == 4)
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