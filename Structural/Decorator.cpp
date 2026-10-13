#include <iostream>
#include <memory>
// We have a library for balls, but we want to extend the functionality to return in inches
// After that and in different patternes

enum class Unit {cm, inch};
enum class SizeClass {small, medium, large};

struct Measure
{
    double value = 0;
    Unit unit = Unit::cm;
    SizeClass size = SizeClass::small;
};

const char* toString(SizeClass s)
{
    switch (s)
    {
        case SizeClass::small:  return "SIZE_SMALL";
        case SizeClass::medium: return "SIZE_MEDIUM";
        case SizeClass::large:  return "SIZE_LARGE";
        default:                return "UNKNOWN";
    }
}

class IBall
{
public:
    virtual Measure getDimensions() = 0;
    virtual ~IBall() = default; 
};

class TenisBall : public IBall
{
    double r = 15;
public:
    Measure getDimensions() override {return {r};}    
};

class BasketBall : public IBall
{
public:
    Measure getDimensions() override {return {40};}    
};

// ------------------ Decorators ------------------

class BallDecorator : public IBall
{
protected:
    std::unique_ptr<IBall> ball;
public:
    explicit BallDecorator(std::unique_ptr<IBall> b) : ball(std::move(b)) {}
    Measure getDimensions() override { return ball->getDimensions(); }
};

class InchesDecorator : public BallDecorator
{
public:
    using BallDecorator::BallDecorator;
    Measure getDimensions() override
    {
        Measure m = ball->getDimensions();
        if (m.unit == Unit::cm)
        {
            m.value /= 2.54;
            m.unit = Unit::inch;
        }
        return m;
    }
};

class SizeClassDecorator : public BallDecorator
{
public:
    using BallDecorator::BallDecorator;
    Measure getDimensions() override
    {
        Measure m = ball->getDimensions();
        double cm = (m.unit == Unit::inch) ? m.value * 2.54 : m.value;
        m.size = cm < 10 ? SizeClass::small
               : cm < 20 ? SizeClass::medium
                         : SizeClass::large;
        return m;
    }
};

int main()
{
    std::unique_ptr<IBall> basket =
        std::make_unique<InchesDecorator>(std::make_unique<BasketBall>());

    std::unique_ptr<IBall> tennis =
        std::make_unique<SizeClassDecorator>(
            std::make_unique<InchesDecorator>(std::make_unique<TenisBall>()));

    Measure b = basket->getDimensions();
    Measure t = tennis->getDimensions();
    std::cout << "Basketball in inches: " << b.value << "\n";
    std::cout << "Tennis ball: " << t.value << " in, " << toString(t.size) << "\n"; 
}
