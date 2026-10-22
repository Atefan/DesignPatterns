#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <map>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <optional>
#include <mutex>
#include <string>

class ISubscriber
{
public:
    virtual void notify() = 0;
    virtual ~ISubscriber() = default;
};

class RestaurantOrders
{
    using Time = std::chrono::steady_clock::time_point;

    struct Customer
    {
        ISubscriber& s;
        int number;
    };

    std::mutex mutex;
    std::map<Time, Customer> waitingClients;
    std::unordered_map<int, std::string> doneOrders;
    
    std::jthread worker;
    


public:
    RestaurantOrders()
    {
        worker = std::jthread([this](std::stop_token st)
        {
            while(!st.stop_requested())
            {
                // check every 1 second for new order done
                std::this_thread::sleep_for(std::chrono::seconds(1));

                const auto currTime = std::chrono::steady_clock::now();

                for(auto it = waitingClients.begin(); it != waitingClients.end();)
                {
                    std::lock_guard<std::mutex> lg(mutex);

                    // if enough time has passed for the pizza to be made, pass it to make it ready
                    if(it->first + std::chrono::seconds(2) <= currTime)
                    {
                        doneOrders.emplace(it->second.number, "MARGARITA");
                        it->second.s.notify();
                        it = waitingClients.erase(it);
                    }else
                        ++it;
                        
                }
            }
            std::cout << "THread Exiting\n";
        });
    }

    std::optional<std::string> getOrder(int num)
    {
        auto it = doneOrders.find(num);

        if(it == doneOrders.end())
            return std::nullopt;

        std::string res = it->second;
        doneOrders.erase(num);

        return res;
    }

    int makeAnOrder(ISubscriber & client)
    {
        std::lock_guard<std::mutex> lg(mutex);
        static int orderNumber = 0;
        waitingClients.emplace(std::chrono::steady_clock::now(), Customer{client, orderNumber});
        return orderNumber++;
    }
} RestaurantOrders;

class Waiter : public ISubscriber
{
    int orderNumber;
    bool notifiedFlag = false;
public:
    void notify() override 
    {
        std::cout<< "Waiter Notified\n";
        std::cout<< "Waiter getting order N:" << orderNumber << "\n";
        this->notifiedFlag = true;
    }
    void casualWork()
    {
        orderNumber = RestaurantOrders.makeAnOrder(*this);
        std::cout<< "Waiter Made an order with number "<< orderNumber << "\n";
        while(notifiedFlag == false)
        {
            
        }
        std::cout<< "Waiter received  "<< RestaurantOrders.getOrder(orderNumber).value_or("NOTHING!!!") << "\n";
        
    }
};

int main()
{
    Waiter waiter;
    waiter.casualWork();
}
