#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

// In your home there are different smart devices, each device depends in some way with other devices
// They communicate to the Mediator, which handesl the dependencies
// When the light sensor shows its night, notifies the mediator and the mediator sends a command to close the curtains
class IMediator
{
public:
    virtual void notify(std::string event) = 0;
    virtual ~IMediator() = default;
};

class ApartmentMotionSensor
{
    IMediator * m;
public:
    ApartmentMotionSensor() {}

    void setMediator(IMediator & medi) {m = &medi;}

    void enteredTheApartment()
    {
        std::cout<<"Sensors Detected Someone Entering the Apartment\n";
        m->notify("entered");
    }
    void leftTheApartment()
    {
        std::cout<<"Sensors Detected Someone Exiting the Apartment\n";
        m->notify("exited");
    }
};

class Lights
{
    IMediator * m;
public:
    Lights() {}

    void setMediator(IMediator & medi) {m = &medi;}

    void turnOff()
    {
        std::cout<<"Turning off lights\n";
    }

    void turnOn()
    {
        std::cout<<"Turning on lights\n";
    }
};

class VacuumCleaner
{
    IMediator * m;
    int currentRoom = 0;
public:
    VacuumCleaner() {}

    void setMediator(IMediator & medi) {m = &medi;}

    void startCleaning()
    {
        
        std::cout<<"Vacuum Cleaner Starting procedure\n";
        // requests lights on for the current room(for the camera)
        currentRoom = rand() % 3;
        m->notify("lights-on " + std::to_string(currentRoom));
    }
    
    void returnToCharger()
    {
        std::cout<<"Vacuum Returning to Charger\n";
        m->notify("lights-off " + std::to_string(currentRoom));
    }
};



class Mediator : public IMediator
{
    VacuumCleaner &vc;
    Lights & lightsRoom0;
    Lights & lightsRoom1;
    Lights & lightsRoom2;
    ApartmentMotionSensor & apartmentSensor;
public:
    Mediator(VacuumCleaner& vc, Lights& lights0, Lights& lights1, Lights& lights2, ApartmentMotionSensor& sensor)
            : vc(vc), lightsRoom0(lights0), lightsRoom1(lights1), lightsRoom2(lights2), apartmentSensor(sensor){}

    void notify(std::string request) override
    {
        std::cout << request << std::endl;

        auto space = request.find(' ');

        std::string command = request.substr(0, space);
        int room;
        if(space != std::string::npos)
            room = std::stoi(request.substr(space + 1));

        if (command == "lights-on")
        {
            switch (room)
            {
                case 0:
                    lightsRoom0.turnOn();
                    break;

                case 1:
                    lightsRoom1.turnOn();
                    break;

                case 2:
                    lightsRoom2.turnOn();
                    break;
            }
        }
        else if (command == "lights-off")
        {
            switch (room)
            {
                case 0:
                    lightsRoom0.turnOff();
                    break;

                case 1:
                    lightsRoom1.turnOff();
                    break;

                case 2:
                    lightsRoom2.turnOff();
                    break;
            }
        }
        else if(command == "entered")
        {
            lightsRoom0.turnOn();
            lightsRoom1.turnOn();
            lightsRoom2.turnOn();
            vc.returnToCharger();
        }
        else if(command == "exited")
        {
            lightsRoom0.turnOff();
            lightsRoom1.turnOff();
            lightsRoom2.turnOff();
            vc.startCleaning();
        }
        else
            std::cout<< "Invalid Request\n";

    }

    void testTriggerAppartmentSensor(bool state)
    {
        if(state)
            apartmentSensor.enteredTheApartment();
        else
            apartmentSensor.leftTheApartment();
    }
};

int main()
{
    VacuumCleaner vc;
    Lights lights0;
    Lights lights1;
    Lights lights2;
    ApartmentMotionSensor sensor;
    Mediator medi(vc,lights0, lights1, lights2, sensor);

    vc.setMediator(medi);
    lights0.setMediator(medi);
    lights1.setMediator(medi);
    lights2.setMediator(medi);
    sensor.setMediator(medi);

    std::cout<<"------USER ENTERING IN THE APARTMENT------\n";
    medi.testTriggerAppartmentSensor(1);
    
    std::cout<<"------USER EXITING THE APARTMENT------\n";
    medi.testTriggerAppartmentSensor(0);
    srand(time(0));
}
