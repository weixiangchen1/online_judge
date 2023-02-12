#ifndef COMPILE_ONLINE
#include "header.cpp"
#endif // !COMPILE_ONLINE

void Test1()
{
    bool ret = Solution().isPalindrome(212);
    if (ret) 
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
    bool ret = Solution().isPalindrome(12);
    if (!ret) 
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