#include <iostream>

class BallJury
{
private:
    BallJury() = default;
    BallJury(const BallJury & t) = delete;
    BallJury & operator=(BallJury & t) = delete;

    int sharedItem;
public:
    // sets the static 
    static BallJury& returnBallJury()
    {
        static BallJury sharedInstance;
        return sharedInstance;
    }


    int readSharedItem()
    {
        return sharedItem;
    }

    void write(int value)
    {
        sharedItem = value;
    }
};


int main()
{
    BallJury::returnBallJury().write(5);
    std::cout<< "Result is: " << BallJury::returnBallJury().readSharedItem() << "\n";
}
