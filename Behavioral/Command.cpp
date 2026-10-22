#include <iostream>
#include <memory>
#include <queue>

enum class SteerDir { Straight, Left, Right };
enum class SpeedOp  { Stop, Increase, Decrease };

const char* name(SteerDir d)
{
    switch (d)
    {
        case SteerDir::Left:  return "left";
        case SteerDir::Right: return "right";
        default:              return "straight";
    }
}

class Backend
{
    int speed = 0;
public:
    bool steer(SteerDir dir, int angle)
    {
        std::cout << "steer " << name(dir) << " by " << angle << "\n";
        return true;
    }
    bool changeSpeed(SpeedOp op, int amount)
    {
        if (op == SpeedOp::Stop)
            speed = 0;
        else if (op == SpeedOp::Increase)
            speed += amount;
        else
            speed = (speed > amount) ? speed - amount : 0;

        std::cout << "speed now " << speed << "\n";
        return true;
    }
    bool reverse()
    {
        if (speed > 0)
        {
            std::cout << "reverse rejected (speed is " << speed << ")\n";
            return false;
        }
        std::cout << "reversing\n";
        return true;
    }
};

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual bool execute() = 0;
};

class SteerCommand : public ICommand
{
    Backend& b;
    SteerDir dir;
    int angle;
public:
    SteerCommand(Backend& be, SteerDir d, int a) : b(be), dir(d), angle(a) {}
    bool execute() override { return b.steer(dir, angle); }
};

class ChangeSpeedCommand : public ICommand
{
    Backend& b;
    SpeedOp op;
    int amount;
public:
    ChangeSpeedCommand(Backend& be, SpeedOp o, int a) : b(be), op(o), amount(a) {}
    bool execute() override { return b.changeSpeed(op, amount); }
};

class ReverseCommand : public ICommand
{
    Backend& b;
public:
    explicit ReverseCommand(Backend& be) : b(be) {}
    bool execute() override 
    { 
        return b.reverse();
    }
};

int main()
{
    Backend backend;
    std::queue<std::unique_ptr<ICommand>> queue;

    queue.push(std::make_unique<SteerCommand>(backend, SteerDir::Left, 15));
    queue.push(std::make_unique<ChangeSpeedCommand>(backend, SpeedOp::Increase, 30));
    queue.push(std::make_unique<ReverseCommand>(backend));

    while (!queue.empty())
    {
        if (!queue.front()->execute())
            std::cout << "command rejected\n";
        queue.pop();
    }
}