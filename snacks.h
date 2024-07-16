#pragma once
#include "food.h"
#include "drinks.h"

class Snack : public Food
{
    Size snacksize;

public:
    Snack *next;
    Snack(string _name, int _kCal, int _price) : Food(_name, _kCal, _price) { next = nullptr; }
    void setSize(Size x) { snacksize = x; }
    Size getSize() const { return snacksize; }
    void printData(ofstream *newFile = nullptr) const
    {
        if (newFile == nullptr)
        {
            string size;
            switch (snacksize)
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
            cout << "SNACK: " << endl
                 << name << " // "
                 << price << " Ft"
                 << " // "
                 << kCal << " kCal"
                 << " // "
                 << size << endl;
        }
        if (newFile != nullptr)
        {
            string size;
            switch (snacksize)
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
                cout << "Méret nem található (snack)!" << endl;
                exit(1);
            }
            *newFile << "SNACK: " << endl
                     << name << " // "
                     << price << " Ft"
                     << " // "
                     << kCal << " kCal"
                     << " // "
                     << size << endl;
        }
    }
};