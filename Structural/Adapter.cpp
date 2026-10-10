#include <iostream>
// Class Hoop takes balls, but people what to shoot toys in there

class IBall
{
public:

    virtual int getRadius() const = 0;
    virtual ~IBall() = default;
};

class BallObj : public IBall
{
    int radius;
public:
    BallObj() = default;
    BallObj(int r):radius(r) {}
    int getRadius() const {return this->radius;}
};

class ToyObj
{
    int maxDiameter;
public:
    ToyObj(int mD) : maxDiameter(mD) {}
    int getMaxDiameter() const {return maxDiameter;}
};

class ToyAdapter : public IBall
{
    ToyObj toy;
public:
    int getRadius() const
    {
        return toy.getMaxDiameter() / 2;
    }
    ToyAdapter(const ToyObj &t): toy(t) {}
};



class Hoop
{
public:
    const int radius = 20;

    bool canPassTheHoop(const IBall &b)
    {
        return b.getRadius() <= this->radius;
    }
};


int main()
{
    Hoop hoop;

    BallObj b1(15), b2(20), b3(25);
    std::cout << hoop.canPassTheHoop(b1) << "\n";   // true
    std::cout << hoop.canPassTheHoop(b2) << "\n";   // true
    std::cout << hoop.canPassTheHoop(b3) << "\n";   // false

    ToyAdapter ta1(ToyObj(15)), ta2(ToyObj(20)), ta3(ToyObj(25));
    std::cout << hoop.canPassTheHoop(ta1) << "\n";  // true  (radius 7)
    std::cout << hoop.canPassTheHoop(ta2) << "\n";  // true  (radius 10)
    std::cout << hoop.canPassTheHoop(ta3) << "\n";  // true  (radius 12)
}
