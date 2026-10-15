#include <iostream>
#include <array>
#include <string>


// Each TT player has the ability to serve 4 different serves(backhand top spin, forhand top spin, ...back spin)
// The player serves and the balls state changes in a complex way that is of no interest to the payer
// (he thinks, i wll serve a backhand bottom spin, not a 150RPM 30degrees 12kmph serve in that direction)

// The design->
//  We Have a complex BallStateCalculator Library that contains a lot of functions
//  From there we are going to

class BallState
{
public:
    std::array<int, 3> possition = {0, 0, 0};
    std::array<int, 3> direction = {0, 0, 0};
    std::array<int, 2> spinDirection = {0, 0};

    BallState() = default;

    BallState(BallState && other)
    {
        possition = other.possition;
        direction = other.direction;
        spinDirection = other.spinDirection;
    }

    BallState& operator=(BallState && other)
    {
        if(this != &other)
        {
            possition = other.possition;
            direction = other.direction;
            spinDirection = other.spinDirection;

        }
        return *this;
    }

    void print(const std::string& label) const
    {
        std::cout << "  [" << label << "] pos(" << possition[0] << ", " << possition[1] << ", " << possition[2] << ")"
                  << " dir(" << direction[0] << ", " << direction[1] << ", " << direction[2] << ")"
                  << " spin(" << spinDirection[0] << ", " << spinDirection[1] << ")\n";
    }
};

class WindCalculation
{
public:
    std::array<int, 3> calculateWind(const BallState &prevState)    
    {
        // A light side wind that gets weaker the further the ball has travelled
        int wind = (prevState.possition[0] < 100) ? 1 : 0;
        return std::array<int, 3>{0, wind, 0};
    }
};

class SpinCalculation
{
public:
    std::array<int, 3> calculateSpinFromContact(const BallState &prevState, BallState &newState)
    {
        // spinDirection[0]: + topspin / - backspin, spinDirection[1]: sidespin
        // Topspin makes the ball dip and speed up, backspin makes it float and slow down
        int spinChange = newState.spinDirection[0] - prevState.spinDirection[0];
        return std::array<int, 3>{spinChange / 10, newState.spinDirection[1] / 10, -spinChange / 20};
    }
};

class GravityCalculation
{
public:
    std::array<int, 3> calculateGravity(const BallState &prevState)
    {
        // Gravity only pulls the ball down while it is above the table
        int pull = (prevState.possition[2] > 0) ? -2 : 0;
        return std::array<int, 3>{0, 0, pull};
    }
};

enum class ServeType {    ForehandBackspin, ForehandTopspin, BackhandBackspin, BackhandTopspin};

class BallFacade
{
    BallState ball;
        
    WindCalculation windCalc;
    SpinCalculation spinCalc;
    GravityCalculation gravCalc;

    // Starts the next state from the current ball (BallState can't be copied)
    BallState startNext()
    {
        BallState next;
        next.possition = ball.possition;
        next.direction = ball.direction;
        next.spinDirection = ball.spinDirection;
        return next;
    }

    // Applies gravity + wind (based on the current ball), moves one step and makes "next" the ball
    void fly(BallState &next)
    {
        std::array<int, 3> gravity = gravCalc.calculateGravity(ball);
        std::array<int, 3> wind = windCalc.calculateWind(ball);

        for (std::size_t i = 0; i < 3; ++i)
        {
            next.direction[i] += gravity[i] + wind[i];
            next.possition[i] = ball.possition[i] + next.direction[i];
        }
        if (next.possition[2] < 0)
            next.possition[2] = 0;

        ball = std::move(next);
    }

    void redirect(std::array<int, 3> hitDirection)
    {
        BallState next = startNext();
        next.direction = hitDirection;
        fly(next);
    }

    void addSpin(std::array<int, 2> spin)
    {
        BallState next = startNext();
        next.spinDirection = spin;

        std::array<int, 3> spinEffect = spinCalc.calculateSpinFromContact(ball, next);
        for (std::size_t i = 0; i < 3; ++i)
            next.direction[i] += spinEffect[i];

        fly(next);
    }

    void toss()
    {
        ball = BallState();
        ball.possition = {0, 0, 20};
        ball.print("toss");
    }
    
    // ---------- Facade ----------
public:


    void serve(ServeType type)
    {
        std::string name;
        std::array<int, 3> hitDirection;
        std::array<int, 2> spin;
        
        switch (type)
        {
        case ServeType::ForehandBackspin:
        {
            name = "Forehand backspin serve";
            hitDirection = {10, -3, 2};
            spin = {-150, -20};
            break;
        }
        case ServeType::ForehandTopspin:
        {
            name = "Forehand topspin serve";
            hitDirection = {15, -3, 3};
            spin = {150, -20};
            break;
        }
        case ServeType::BackhandBackspin:
        {
            name = "Backhand backspin serve";
            hitDirection = {10, 3, 2};
            spin = {-150, 20};
            break;
        }
        case ServeType::BackhandTopspin:
        {
            name = "Backhand topspin serve";
            hitDirection = {15, 3, 3};
            spin = {150, 20};
            break;
        }
        default:
            std::cout << "Unknown serve\n";
            return;
        }

        std::cout << name << ":\n";
        toss();
        redirect(hitDirection);
        ball.print("hit");
        
        addSpin(spin);
        ball.print("spin");

    }

    
    bool returnPass()
    {
       // Some use of the classes
       return false;
    }

}bf;

class Player
{

public:
    Player() {}

    void playGame(ServeType type)
    {
        bf.serve(type);
        while(bf.returnPass());
            
    }


};

int main()
{
    Player player;
    player.playGame(ServeType::ForehandBackspin);

}
