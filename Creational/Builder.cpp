#include <iostream>
#include <memory>

class Ball
{
    int diameter;
    int texture;
    int color;
    int matchesPlayed;
    std::string extraData;
    
    const int avrgHitsPerMatch = 123;
    
public:
    Ball() : diameter(0), texture(0), color(0), matchesPlayed(0)
    {
        extraData = "";
    }

    friend class BallBuilder;
};

class BallBuilder
{
    std::unique_ptr<Ball> ball;

public:

    BallBuilder()
    {
        this->ball = std::make_unique<Ball>();
    }

    BallBuilder& setDiameter(int d)                  { ball.get()->diameter = d; return *this;}
    BallBuilder& setTexture(int t)                   { ball.get()->texture = t; return *this; }
    BallBuilder& setColor(int c)                     { ball.get()->color = c; return *this; }
    BallBuilder& setMatchesPlayed(int m)             { ball.get()->matchesPlayed = m; return *this; }
    BallBuilder& setExtraData(std::string data)      { ball.get()->extraData = std::move(data); return *this; }
    
    void reset()
    {
        ball.reset(new Ball);
    }

    std::unique_ptr<Ball> build()
    {
        auto result = std::move(this->ball);
        this->reset();
        return result;
    }
};

int main()
{
    BallBuilder builder;
    builder.setDiameter(5).setColor(4).setExtraData("Tennis Ball");

    std::unique_ptr<Ball> resultBall = builder.build();

}
