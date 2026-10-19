#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

struct Pos{int x, y;};

//intrinsic class, immutable, shared by many balls
class BallIntrinsic
{
    std::string texture;
    std::string manifacturer;
    std::string color;

public:
    BallIntrinsic(std::string &tex, std::string &man, std::string &col): texture(tex), manifacturer(man), color(col) {}
    void draw(Pos p) const
    {
        std::cout << "  (" << p.x << ", " << p.y << ")  " << color << " " << texture << " ball by " << manifacturer
                  << "  [type @ " << this << "]\n";
    }
};

class Ball
{
    Pos pos;
    std::shared_ptr<const BallIntrinsic> type;
public:
    Ball( Pos x, std::shared_ptr<const BallIntrinsic> p):  pos(x), type(p) {}
    void draw() const { type->draw(pos); }
};

class IntrinsicHolder
{
    std::unordered_map<std::string, std::shared_ptr< const BallIntrinsic>> holder;

public:
    std::shared_ptr<const BallIntrinsic> get(std::string tex, std::string man, std::string col)
    {
        std::string key = tex + "|" + man+ "|" + col;
        
        auto it = holder.find(key);
        if (it != holder.end())
            return it->second;
        return holder.emplace(key, std::make_shared<const BallIntrinsic>(tex, man, col)).first->second;
    }

    std::size_t size() const { return holder.size(); }

    void print() const
    {
        std::cout << "Shared ball types:\n";
        // The holder keeps one reference itself, the rest are balls using this type
        for(const auto & i : holder)
            std::cout << "  " << i.first << "  -> used by " << i.second.use_count() - 1 << " ball(s)\n";
    }
};

int main()
{
    IntrinsicHolder h;
    std::vector<Ball> field;
    field.push_back({{1,2} , h.get("text1", "man1", "col1")});
    field.push_back({{1,2} , h.get("text2", "man2", "col2")});
    field.push_back({{1,2} , h.get("text1", "man1", "col1")});
    field.push_back({{1,2} , h.get("text3", "man3", "col3")});
    field.push_back({{1,2} , h.get("text1", "man1", "col1")});
    field.push_back({{1,2} , h.get("text1", "man1", "col1")});

    std::cout << "Balls on the field:\n";
    for(const auto&a : field)
        a.draw();

    std::cout << "\n";
    h.print();

    std::cout << "\n" << field.size() << " balls, but only " << h.size() << " ball types in memory\n";
}
