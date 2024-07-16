#include "drinks.h"
#include "food.h"
#include <fstream>

Drink::Drink(string _name, int _kCal, int _price) : Food(_name, _kCal, _price)
{
    drinksize = KICSI;
    withIce = true;
}


Size Drink::getSize() const
{
    return drinksize;
}

void Drink::setIce(bool x)
{
    withIce = x;
}

void Drink::setSize(Size x)
{
    drinksize = x;
}

void Drink::printData(ofstream *newFile = nullptr) const
{
    if (newFile == nullptr)
    {
        string size;
        switch (drinksize)
        {
        case 0:
            size = "KICSI";
            break;
        case 1:
            size = "KÖZEPES";
            break;
        case 2:
            size = "NAGY";
            break;
        default:
            cout << "Méret nem található (ital)!" << endl;
            exit(1);
        }
        cout << "ÜDÍTŐ: " << endl
             << name << " // "
             << price << " Ft"
             << " // "
             << kCal << " kCal"
             << " // "
             << size << " // ";

        if (withIce == 0)
            cout << "JÉG NÉLKÜL" << endl;
        else
            cout << "JÉGGEL" << endl;
    }

    if (newFile != nullptr)
    {
        string size;
        switch (drinksize)
        {
        case 0:
            size = "KICSI";
            break;
        case 1:
            size = "KÖZEPES";
            break;
        case 2:
            size = "NAGY";
            break;
        default:
            cout << "Méret nem található!" << endl;
            exit(1);
        }
        *newFile << "ÜDÍTŐ: " << endl
                 << name << " // "
                 << price << " Ft"
                 << " // "
                 << kCal << " kCal"
                 << " // "
                 << size << " // ";

        if (withIce == 0)
            *newFile << "JÉG NÉLKÜL" << endl;
        else
            *newFile << "JÉGGEL" << endl;
    }
}