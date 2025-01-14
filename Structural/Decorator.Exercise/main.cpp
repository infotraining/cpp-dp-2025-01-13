#include "starbugs_coffee.hpp"

#include <memory>
#include <cassert>

void client(std::unique_ptr<Coffee> coffee)
{
    std::cout << "Description: " << coffee->get_description() << "; Price: " << coffee->get_total_price() << std::endl;
    coffee->prepare();
}

class CoffeeBuilder
{
    std::unique_ptr<Coffee> coffee_;

public:
    template <typename BaseCoffee>
    CoffeeBuilder& create_base()
    {
        coffee_.reset(new BaseCoffee);

        return *this;
    }

    template <typename... Condiments>
    CoffeeBuilder& add()
    {
        assert(coffee_);

        (..., (coffee_ = std::make_unique<Condiments>(std::move(coffee_)))); // fold expression

        return *this;
    }

    std::unique_ptr<Coffee> get_coffee()
    {
        return std::move(coffee_);
    }
};

int main()
{
    std::unique_ptr<Coffee> cf = std::make_unique<Cappuccino>();
    std::unique_ptr<Coffee> cf1 = std::make_unique<Whisky>(std::move(cf));
    std::unique_ptr<Coffee> cf2 = std::make_unique<Whisky>(std::move(cf1));
    std::unique_ptr<Coffee> cf3 = std::make_unique<Whisky>(std::move(cf2));
    std::unique_ptr<Coffee> cf4 = std::make_unique<Whipped>(std::move(cf3));
    client(std::move(cf4));

    std::cout << "\n\n";

    CoffeeBuilder coffee_builder;
    coffee_builder
        .create_base<Espresso>()
        .add<Whisky, Whisky, ExtraEspresso>()
        .add<Whipped>();

    client(coffee_builder.get_coffee());
}
