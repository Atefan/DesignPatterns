#include <iostream>
#include <memory>
#include <memory>

class Ball
{
public:
    virtual void interact() const = 0;
    virtual void getSpecs() const = 0;
    virtual ~Ball() = default;
};

class TenisBall : public Ball
{
public:
    TenisBall(bool dirty, int radius) : dirty(dirty), radius(radius) {}

    void interact() const override
    {
        std::cout << "Creation: The Tenis Ball was interacted\n";
    }
    void getSpecs() const override
    {
        std::cout << "the ball is " << radius << " cm and " << ((!dirty) ? "is not dirty" : "is dirty") << "\n";
    }

private:
    bool dirty;
    int radius;
};

class BasketBall : public Ball
{
public:
    BasketBall(bool old) : old(old) {}

    void interact() const override
    {
        std::cout << "Creation: The BasketBall was interacted\n";
    }

    void getSpecs() const override
    {
        std::cout << "the ball " << ((!old) ? "is not old" : "is old") << "\n";
    }

private:
    bool old;
};


class BallCreator
{
public:
    virtual std::unique_ptr<Ball> createBall() const = 0;
    virtual ~BallCreator() = default;
};

class TenisBallCreator : public BallCreator
{
public:
    TenisBallCreator(bool dirty, int radius) : dirty(dirty), radius(radius) {}

    std::unique_ptr<Ball> createBall() const override
    {
        return std::make_unique<TenisBall>(dirty, radius);
    }

private:
    bool dirty;
    int radius;
};

class BasketBallCreator : public BallCreator
{
public:
    BasketBallCreator(bool old) : old(old) {}

    std::unique_ptr<Ball> createBall() const override
    {
        return std::make_unique<BasketBall>(old);
    }

private:
    bool old;
};

int main()
{
    int ballType = 2;
    std::unique_ptr<BallCreator> creator;

    switch (ballType)
    {
        // Tenis Ball
        case 1:
            creator = std::make_unique<TenisBallCreator>(false, 10);
            break;
        // Basket Ball
        case 2:
            creator = std::make_unique<BasketBallCreator>(false);
            break;
        default:
            std::cout << "Unknown ball\n";
            return 1;
    }

    
    std::unique_ptr<Ball> b = creator->createBall();
    b->interact();
    b->getSpecs();
}
