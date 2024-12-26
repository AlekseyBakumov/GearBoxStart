#include <iostream>

// For tuning
#define RATIO_1 0.327
#define RATIO_2 0.484
#define RATIO_3 0.677
#define RATIO_4 0.926
#define RATIO_5 1.189
#define RATIO_R -0.269
#define RATIO_MAIN -0.206
#define HP 300
#define MAXRPM 2000

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN system("cls");
#else
#include <unistd.h>
#define CLEAR_SCREEN system("clear");
#endif

// Cross-platform sleep function
void sleepcp(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 10000);
#endif
}

#pragma region GearBox

class Gear
{
public:
    virtual double getRatio() = 0;
};

class Gear1 : public Gear
{
    virtual double getRatio() override
    {
        return RATIO_1;
    }
};

class Gear2 : public Gear
{
    virtual double getRatio() override
    {
        return RATIO_2;
    }
};

class Gear3 : public Gear
{
    virtual double getRatio() override
    {
        return RATIO_3;
    }
};

class Gear4 : public Gear
{
    virtual double getRatio() override
    {
        return RATIO_4;
    }
};

class Gear5 : public Gear
{
    virtual double getRatio() override
    {
        return RATIO_5;
    }
};

class GearR : public Gear
{
    virtual double getRatio() override
    {
        return RATIO_R;
    }
};

#pragma endregion

class Car
{
private:
    Gear* _gear;
    double _torque;
    double _torqueMax = 2000;
    Gear** _gearbox;
    int _selectedGear;

    void drawMeter(int number, int len, int max)
    {
        for (int i = 0; i < number; i += max/len)
            std::cout << "|";
        std::cout << std::endl;
    }

    bool nextGear()
    {
        if (_selectedGear == 5) return false;
        _selectedGear++;
        _gear = _gearbox[_selectedGear];
        return true;
    }

public:
    Car()
    {
        _gearbox = new Gear*[8];
        _gearbox[0] = new GearR();
        _gearbox[1] = new Gear1();
        _gearbox[2] = new Gear2();
        _gearbox[3] = new Gear3();
        _gearbox[4] = new Gear4();
        _gearbox[5] = new Gear5();
        _gear = _gearbox[0];
        _selectedGear = 0;
    }
    double GetSpeed()
    {
        return - _torque * RATIO_MAIN * _gear->getRatio();
    }
    double GetTorque()
    {
        return _torque;
    }

    void DrawStats()
    {
        std::cout << "SPEED------------" << std::endl;
        drawMeter((int)GetSpeed(), 10, 100);
        std::cout << (int)GetSpeed() << std::endl;
        std::cout << "TORQUE-----------" << std::endl;
        drawMeter((int)GetTorque(), 20, MAXRPM);
        std::cout << (int)GetTorque() << std::endl;
        std::cout << "GEAR-------------" << std::endl;
        std::cout << "|R|1|2|3|4|5|" << std::endl;
        for (int i = 0; i < _selectedGear; i++)
            std::cout << "  ";
        std::cout << " X" << std::endl;
    }

    bool Accelerate()
    {
        _torque += HP * 0.4;
        if (_torque >= _torqueMax)
        {
            if (!nextGear()) return false;
            _torque *= - _gear->getRatio() * RATIO_MAIN;
        }

        return true;
    }
};

int main()
{
    Car* car = new Car();

    while (car->Accelerate())
    {
        CLEAR_SCREEN
        car->DrawStats();
        sleepcp(20);
    }

    std::cout << "Top speed reached" << std::endl;

    return 0;
}

