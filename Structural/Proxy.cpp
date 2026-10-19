#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>

// Here we implement a protection proxy
// Only moderators can render

enum class AccessToken {USER, MOD};

class IBall
{
public:
    virtual void render() = 0;
    virtual ~IBall() = default;
};

class BallModel : public IBall
{
    std::string file;
public:
    BallModel(const std::string &f) : file(f)
    {
        std::cout<< "Loading LARGE Model from file "<< file << "...\n"; //large init
    }
    void render() override
    {
        std::cout<< "Rendering... \n";
    }
};

// For storing the LARGE object and lazy initializing it
class BallModelCache
{
    std::unordered_map<std::string, std::shared_ptr<BallModel>> models;
public:
    std::shared_ptr<BallModel> get(const std::string &f)
    {
        auto it = models.find(f);
        if(it == models.end())
            return models.emplace(f, std::make_shared<BallModel>(f)).first->second;
        return it->second;
    }
};


class BallModelProxy : public IBall
{
    AccessToken token;
    std::string file;
    BallModelCache &cache;
public:
    BallModelProxy(const AccessToken& t,const std::string &f, BallModelCache &m) : token(t), file(f), cache(m) {}
    void render() override
    {
        if(token == AccessToken::MOD)
            cache.get(file)->render();
        else
            std::cout<< "Only MODs can print\n";
    }
};



int main()
{
    std::string file = "ball_4k.obj";
    BallModelCache ballDB;

    std::cout << "--- Creating two proxies (nothing should load yet) ---\n";
    BallModelProxy modBall(AccessToken::MOD, file, ballDB);
    BallModelProxy userBall(AccessToken::USER, file, ballDB);

    std::cout << "\n--- MOD renders twice (the model should load only once) ---\n";
    modBall.render();
    modBall.render();

    std::cout << "\n--- USER tries to render ---\n";
    userBall.render();

    std::cout << "\n--- The client only knows IBall, not that it is talking to a proxy ---\n";
    std::unique_ptr<IBall> ball = std::make_unique<BallModelProxy>(AccessToken::MOD, file, ballDB);
    ball->render();
}
