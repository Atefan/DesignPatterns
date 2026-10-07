#include <iostream>
#include <memory>


// Base
class Ball
{
public:
    Ball(int radius) : r(radius){}
    int r;
    virtual ~Ball() {}
};

class LidlBall : public Ball
{
public:
    LidlBall(int r) : Ball(r) {std::cout << "Created LidlBall\n";}
    ~LidlBall() {}
};

class DecathBall : public Ball
{
public:
    DecathBall(int r) : Ball(r) {std::cout << "Created DecathBall\n";}
    ~DecathBall() { }
};

// Base
class Tshirt
{
public:
    virtual void fold() = 0;
    virtual void clean() = 0;
    virtual ~Tshirt() {}
};

class LidlTshirt : public Tshirt
{
public:
    void fold()
    {
        std::cout << "Folding in a Lidl Way\n";
    }
    void clean()
    {
        std::cout << "Cleanign in a Lidl Way\n";
    }
    ~LidlTshirt() { }
};

class DecathTshirt : public Tshirt
{
public:
    void fold()
    {
        std::cout << "Folding in a Decath Way\n";
    }
    void clean()
    {
        std::cout << "Cleanign in a Decath Way\n";
    }
    ~DecathTshirt() { }
};


// ----------------Factories----------------

class AbstractFactory
{
public:
    virtual std::unique_ptr<Tshirt> createTshirt() = 0;
    virtual std::unique_ptr<Ball> createBall(int ) = 0;

    virtual ~AbstractFactory() { }
};

class LidlFactory : public AbstractFactory
{
public:
    std::unique_ptr<Ball> createBall(int radius)
    {
        return std::make_unique<LidlBall>(radius);
    }   
    std::unique_ptr<Tshirt> createTshirt()
    {
        return std::make_unique<LidlTshirt>();
    }   
    ~LidlFactory() { }
};

// The Decath items have year while the Lidl contain ID
class DecathFactory : public AbstractFactory
{
public:
    std::unique_ptr<Ball> createBall(int radius)
    {
        return std::make_unique<DecathBall>(radius);
    }   
    std::unique_ptr<Tshirt> createTshirt()
    {
        return std::make_unique<DecathTshirt>();
    }   
    ~DecathFactory() { }
};

int main()
{
    // Decath section
    std::unique_ptr<AbstractFactory> factory = std::make_unique<DecathFactory>();

    // Lidl section
    // std::unique_ptr<AbstractFactory> factory = std::make_unique<DecathFactory>();


    std::unique_ptr<Ball> b= factory->createBall(5);
    b->r;

    std::unique_ptr<Tshirt> ts = factory->createTshirt();
    ts->clean();
    ts->fold();


}
