#include <iostream>
#include <memory>

// How does a ball bounce on different surfaces?

class Surface
{
public:
    // Between 0 (doesnt bounce) - 1 (bounces with the full high)
    virtual float restitution() = 0;
    virtual ~Surface() = default;
};

class Ball
{
protected:
    // A biliard ball bounces differently than a basketball
    virtual float getBallBounciness() = 0;
    std::unique_ptr<Surface> s;
public:
    virtual ~Ball() = default;
    Ball(std::unique_ptr<Surface> s1) {s = std::move(s1);}
    //  returns the hight after the bounce
    float drop(float hight)
    {
        return hight * s->restitution() * getBallBounciness();
    }
};

class TennisBall : public Ball
{
    float getBallBounciness()
    {
        return 0.8;
    }
public:
    TennisBall(std::unique_ptr<Surface> s1) : Ball(std::move(s1)) {}
};

class BiliardBall : public Ball
{
    float getBallBounciness()
    {
        return 0.1;
    }
public:
    BiliardBall(std::unique_ptr<Surface> s1) : Ball(std::move(s1)) {}
};


class ConcreteSurface : public Surface
{
public:
    float restitution() {return 0.9;}
};

class GlassSurface : public Surface
{
public:
    float restitution() {return 0.2;}
};

int main()
{
    TennisBall  t(std::make_unique<ConcreteSurface>());
    BiliardBall b(std::make_unique<GlassSurface>());
    std::cout << t.drop(100) << "\n";   // 72  (100 * 0.9 * 0.8)
    std::cout << b.drop(100) << "\n";   // 2   (100 * 0.2 * 0.1)
}
