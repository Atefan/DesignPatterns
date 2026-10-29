#include <iostream>
#include <memory>
#include <string>


class Order;
class CreatedState;
class PaidState;
class DeliveredState;
class CanceledState;

class IState
{
public:
    virtual void orderPaidHook(Order &) = 0;
    virtual void orderDelevered(Order &) = 0;
    virtual void orderCanceled(Order &) = 0;
    virtual std::string getStatus() = 0;
    virtual ~IState() = default;
};

class Order 
{
    friend class CreatedState;
    friend class PaidState;
    friend class DeliveredState;
    friend class CanceledState;

    std::unique_ptr<IState> state;
    void changeState(std::unique_ptr<IState> s);
public:
    Order();
    void eventPaid() {state->orderPaidHook(*this);}
    void eventDelivered() {state->orderDelevered(*this);}
    void eventCanceled() {state->orderCanceled(*this);}

    void printState() {std::cout << state->getStatus(); }
    
};

class CanceledState : public IState
{
public:
    void orderPaidHook(Order &o) override {std::cout<<"Ignored!\n";}
    void orderDelevered(Order &o) override {std::cout<<"Ignored!\n";}
    void orderCanceled(Order &o) override {std::cout<<"Ignored!\n";}
    std::string getStatus(){ return "CanceledState\n";}
};

class CreatedState : public IState
{
public:
    void orderPaidHook(Order &o) override;
    void orderDelevered(Order &o) override {std::cout<<"Ignored!\n";}
    void orderCanceled(Order &o) override {std::cout<<"No Change!\n";}
    std::string getStatus(){ return "CreatedState\n";}
};

class PaidState : public IState
{
public:
    void orderPaidHook(Order &o) override{std::cout<<"Ignored!\n";}
    void orderDelevered(Order &o) override;
    void orderCanceled(Order &o) override;
    std::string getStatus(){ return "PaidState\n";}
};

class DeliveredState : public IState
{
public:
    void orderPaidHook(Order &o) override{std::cout<<"Ignored!\n";}
    void orderDelevered(Order &o) override {std::cout<<"Ignored!\n";}
    void orderCanceled(Order &o) override {std::cout<<"Cant Cancel a Delevered Order!\n";}
    std::string getStatus(){ return "DeliveredState\n";}
};

Order::Order() : state(std::make_unique<CreatedState>()) {}

void Order::changeState(std::unique_ptr<IState> s)  {state = std::move(s);}


void CreatedState::orderPaidHook(Order &o)
{
    std::cout<<"Paid!\n";
    o.changeState(std::make_unique<PaidState>());
}

void PaidState::orderDelevered(Order &o) 
{
    std::cout<<"Delivered Order!\n";
    o.changeState(std::make_unique<DeliveredState>());
}

void PaidState::orderCanceled(Order &o)
{
    std::cout<<"Canceled Returning Money!\n";
    o.changeState(std::make_unique<CanceledState>());
}

int main()
{
    Order c;
    c.eventCanceled(); c.printState();
    c.eventPaid(); c.printState();
    c.eventPaid(); c.printState();
    c.eventDelivered(); c.printState();
    c.eventCanceled(); c.printState();
}
