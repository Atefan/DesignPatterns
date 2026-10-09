#include <iostream>
#include <memory>
#include <vector>

class Prototype
{
public:
    virtual std::unique_ptr<Prototype> clone() const = 0;
    virtual void doSmt() = 0;
    virtual ~Prototype() = default;
};

class Ball : public Prototype
{
    int weight;
    int type;
    int radius; 
    
public:
    Ball()
    {
        weight = 0;
        type = 0;
        radius = 0;
    }

    // for tests in main
    Ball(int w, int t, int r): weight(w), type(t), radius(r)
    {
        // ... extra work if needed
    }

    // for the clone function
    Ball(const Ball & b): weight(b.weight), type(b.type), radius(b.radius)
    {
        // ... extra work if needed
    }

    std::unique_ptr<Prototype> clone() const override
    {
        std::unique_ptr<Ball> n = std::make_unique<Ball>(*this);
        return std::move(n);
    }

    void doSmt()
    {
        this->radius += 5;
        std::cout<< "Ball Radius was "<<radius -5 << " now is " << radius << "\n";
    }

    ~Ball() = default;
};

class Cube : public Prototype
{
    int colors;
    int side;
    
public:
    Cube()
    {
        colors = 0;
        side = 0;
    }

    // for tests in main
    Cube(int c, int s): colors(c), side(s)
    {
        // ... extra work if needed
    }

    // for the clone function
    Cube(const Cube & c): colors(c.colors), side(c.side)
    {
        // ... extra work if needed
    }

    std::unique_ptr<Prototype> clone() const override
    {
        std::unique_ptr<Cube> n = std::make_unique<Cube>(*this);
        return std::move(n);
    }

    void doSmt()
    {
        this->side += 3;
        std::cout<< "Cube Side size was "<<side - 3 << " now is " << side << "\n";
    }

    ~Cube() = default;
};



int main()
{
    std::vector<std::unique_ptr<Prototype>> v;

    v.push_back(std::make_unique<Ball>(1,2,3));
    v.push_back(std::make_unique<Cube>(2,3));
    v.push_back(std::make_unique<Ball>(4,5,6));
    v.push_back(std::make_unique<Cube>(4,5));


    std::unique_ptr<Prototype> somethingNew = v[2]->clone();

    std::cout<< "\n\n---- Now for the new one ----\n";
    somethingNew->doSmt();
    somethingNew->doSmt();
    somethingNew->doSmt();

    std::cout<< "\n\n---- Now for the old once ----\n";
    v[2]->doSmt();


}
