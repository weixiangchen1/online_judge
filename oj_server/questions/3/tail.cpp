#ifndef COMPILE_ONLINE
#include "header.cpp"
#endif // !COMPILE_ONLINE

void Test1()
{
    vector<int> v {2, 7, 11, 15};
    int target = 9;
    vector<int> ans = Solution().twoSum(v, target);
    if (ans[0] = 0 && ans[1] == 1)
    {
        std::cout << "You pass the Example 1" << std::endl;
    }
    else
    {
        std::cout << "You can not pass the Example 1" << std::endl;
    }
}
    

int main()
{
    Test1();

    return 0;
}