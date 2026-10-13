#include <iostream>
#include <memory>
#include <vector>

// There is a room full of balls and boxes with more balls/boxes
// You have to count them all and inflate them


class Inflateble
{
public:
    virtual int countBalls() = 0;
    virtual void inflate(int) = 0;
    virtual ~Inflateble() = default;
};

class Ball : public Inflateble    
{
    int pressure;
public:
    Ball(int p) : pressure(p) {};
    void inflate(int p) override  {pressure += p;}
    int countBalls() override {std::cout << "One more Ball\n"; return 1;}
};

class BallBox : public Inflateble
{
    std::vector<std::unique_ptr<Inflateble>> container; 
public:
    void add(std::unique_ptr<Inflateble> i)
    {
        container.push_back(std::move(i));
    }
    void inflate(int p) 
    {
        for(std::unique_ptr<Inflateble> & i : container)
            i->inflate(p);
    }
    int countBalls()
    {
        int res = 0;
        std::cout << "----There is a Box with:\n";
        for(std::unique_ptr<Inflateble> & i  : container)
            res += i->countBalls();
        return res;
    }
};

int main()
{
    // inner box: 3 balls
    auto inner = std::make_unique<BallBox>();
    inner->add(std::make_unique<Ball>(10));
    inner->add(std::make_unique<Ball>(10));
    inner->add(std::make_unique<Ball>(10));

    // outer box: 2 balls + the inner box
    auto outer = std::make_unique<BallBox>();
    outer->add(std::make_unique<Ball>(5));
    outer->add(std::make_unique<Ball>(5));
    outer->add(std::move(inner));

    // the room: a loose ball, the big box, and an empty box
    std::vector<std::unique_ptr<Inflateble>> room;
    room.push_back(std::make_unique<Ball>(0));
    room.push_back(std::move(outer));
    room.push_back(std::make_unique<BallBox>());

    int total = 0;
    for (auto& item : room)
        total += item->countBalls();
    std::cout << "Total balls: " << total << "\n\n";

    for (auto& item : room)
        item->inflate(5);
}