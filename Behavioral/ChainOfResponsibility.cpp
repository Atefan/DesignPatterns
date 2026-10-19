#include <iostream>
#include <memory>
#include <random>
#include <chrono>

// You are making a pizza machine
// The pizza machine makes different types of pizza
// The pizzas are made step by step (first tomatoes, then cheese, then topping, then oven, then optional sauses)

enum class PizzaTypes {PEPERONI, MARGARITA, VEGITABLES};
enum class DoughType {WHITE, DARK};
enum class ToppingType {NONE, PEPERONI, MUSHROOMS};

struct Order
{
    PizzaTypes pizzaType;
};

struct Pizza
{
    DoughType doughType;
    ToppingType toppingType;
};

int roll()
{
    static std::mt19937 rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    static std::uniform_int_distribution<int> dist(0, 6);
    return dist(rng);
}

class IHandler
{
public:
    virtual void setNext(std::unique_ptr<IHandler> h) = 0;
    virtual bool proccess(Order &o, Pizza &p) = 0;
    
    virtual ~IHandler() = default;
};

class Handler : public IHandler
{
protected:
    std::unique_ptr<IHandler> nextHandler;
public:
    void setNext(std::unique_ptr<IHandler> h) override {nextHandler = std::move(h);}
    bool proccess(Order &o, Pizza &p) override
    {
        if(nextHandler)
            return nextHandler->proccess(o, p);
        return true; // end of the chain, everything succeeded
    }
};

class DoughHandler : public Handler
{
public:
    bool proccess(Order &o, Pizza &p) override
    {
        int r = roll();
        if(r)
        {
            std::cout<<"Cooking Dough... (roll " << r << ")\n";
            if(o.pizzaType == PizzaTypes::PEPERONI || o.pizzaType == PizzaTypes::MARGARITA)
                p.doughType = DoughType::WHITE;
            else
                p.doughType = DoughType::DARK;

            return Handler::proccess(o, p);
        }
        else
        {
            std::cout<<"Dough burned!\n";
            return false;
        }
    }
};

class SauceHandler : public Handler
{
public:
    bool proccess(Order &o, Pizza &p) override
    {
        int r = roll();
        if(r)
        {
            std::cout<<"Cooking Tomato Sause... (roll " << r << ")\n";
            return Handler::proccess(o, p);
        }
        else
        {
            std::cout<<"Out of tomatoes!\n";
            return false;
        }
    }
};

class ToppingHandler : public Handler
{
public:
    bool proccess(Order &o, Pizza &p) override
    {
        int r = roll();
        if(r)
        {
            std::cout<<"Placing Topping... (roll " << r << ")\n";
            if(o.pizzaType == PizzaTypes::PEPERONI)
                p.toppingType = ToppingType::PEPERONI;
            else if(o.pizzaType == PizzaTypes::VEGITABLES)
                p.toppingType = ToppingType::MUSHROOMS;
            else // Margarita
                p.toppingType = ToppingType::NONE;

            return Handler::proccess(o, p);
        }
        else
        {
            std::cout<<"Topping fell on the floor!\n";
            return false;
        }
    }

};

const char* toString(PizzaTypes t)
{
    switch(t)
    {
        case PizzaTypes::PEPERONI:   return "Peperoni";
        case PizzaTypes::MARGARITA:  return "Margarita";
        case PizzaTypes::VEGITABLES: return "Vegitables";
    }
    return "?";
}

const char* toString(DoughType t)
{
    return t == DoughType::WHITE ? "white" : "dark";
}

const char* toString(ToppingType t)
{
    switch(t)
    {
        case ToppingType::NONE:      return "none";
        case ToppingType::PEPERONI:  return "peperoni";
        case ToppingType::MUSHROOMS: return "mushrooms";
    }
    return "?";
}

int main()
{
    // dough -> sauce -> topping
    std::unique_ptr<IHandler> topping = std::make_unique<ToppingHandler>();
    std::unique_ptr<IHandler> sauce = std::make_unique<SauceHandler>();
    sauce->setNext(std::move(topping));
    std::unique_ptr<IHandler> chain = std::make_unique<DoughHandler>();
    chain->setNext(std::move(sauce));

    Order orders[] = { {PizzaTypes::PEPERONI}, {PizzaTypes::MARGARITA}, {PizzaTypes::VEGITABLES} };

    for(Order &o : orders)
    {
        std::cout << "=== Order: " << toString(o.pizzaType) << " ===\n";
        Pizza p{};
        if(chain->proccess(o, p))
            std::cout << "-> Ready: dough=" << toString(p.doughType)
                      << ", topping=" << toString(p.toppingType) << "\n\n";
        else
            std::cout << "-> Order failed, pizza thrown away\n\n";
    }
}
