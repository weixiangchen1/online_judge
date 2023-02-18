#ifndef COMPILE_ONLINE
#include "header.cpp"
#endif // !COMPILE_ONLINE

void Test1()
{
    vector<string> ans = Solution().generateParenthesis(1);
    if (ans[0] == "()")
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
    vector<string> ans = Solution().generateParenthesis(2);
    if (ans[0] == "(())" && ans[1] == "()()")
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