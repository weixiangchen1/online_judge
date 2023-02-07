#include "compiler.hpp"

using namespace ns_compiler;

int main()
{
    std::string file_name = "code";
    compiler::compile(file_name);

    return 0;
}