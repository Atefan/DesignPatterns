#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Memento
{
public:
    virtual std::string getName() const = 0;
    virtual ~Memento() = default;
};

class WeightNetwork
{
    std::vector<double> network;
    int cnt;

    class SavedSnapshot : public Memento
    {
        std::vector<double> savednetwork;
        int nameNum = 0;
        friend class WeightNetwork;
    public:
        std::string getName() const override {return "snapshot " + std::to_string(nameNum);}
        
        SavedSnapshot(std::vector<double>& s, int num) : savednetwork(s), nameNum(num) {}
    };

public:
    WeightNetwork() : network(100, 0.0), cnt(0){}

    void setWeight(std::size_t i, double w) { network.at(i) = w; }
    double getWeight(std::size_t i) const   { return network.at(i); }

    std::unique_ptr<Memento> createSnapshot()
    {
        return std::make_unique<SavedSnapshot> (network, cnt++);
    }
    void loadSnapshot(const Memento & mm)
    {
        const SavedSnapshot& s = dynamic_cast<const SavedSnapshot&>(mm);
        network = s.savednetwork;
    }

};


int main()
{
    WeightNetwork net;
    std::vector<std::unique_ptr<Memento>> history;

    net.setWeight(0, 1.5);
    history.push_back(net.createSnapshot());
    net.setWeight(0, 9.9);
    history.push_back(net.createSnapshot());
    net.setWeight(0, -3.0);

    std::cout << net.getWeight(0) << "\n";
    net.loadSnapshot(*history[0]);
    std::cout << history[0]->getName() << ": " << net.getWeight(0) << "\n";

}
