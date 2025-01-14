#include "starbugs_coffee.hpp"
#include <memory>

void client(std::unique_ptr<Coffee> coffee)
{
    std::cout << "Description: " << coffee->get_description() << "; Price: " << coffee->get_total_price() << std::endl;
    coffee->prepare();
}

int main()
{
    std::unique_ptr<Coffee> cf = std::make_unique<Cappuccino>();
    std::unique_ptr<Coffee> cf1 = std::make_unique<Whisky>(std::move(cf));
    std::unique_ptr<Coffee> cf2 = std::make_unique<Whisky>(std::move(cf1));
    std::unique_ptr<Coffee> cf3 = std::make_unique<Whisky>(std::move(cf2));
    std::unique_ptr<Coffee> cf4 = std::make_unique<Whipped>(std::move(cf3));
    client(std::move(cf4));
}
