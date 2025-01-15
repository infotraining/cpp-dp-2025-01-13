#include "strategy.hpp"

int main()
{
    DataContext data{UpperCaseFormatter{}};
    data.pretty_print();

    auto lcf = lower_case_formatter;
    data.reset_formatter(lcf);
    data.pretty_print();

    data.reset_formatter(capitalize_formatter);
    data.pretty_print();
}
