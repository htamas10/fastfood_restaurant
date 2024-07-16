#include "burger.h"
#include "food.h"
#include <iostream>
#include <fstream>
#include <ostream>

Burger::Burger(string _name, int _kCal, int _price, Ingredient _ingredient) : Food(_name, _kCal, _price)
{
    if (_ingredient == CSIRKEHUS || _ingredient == MARHAHUS || _ingredient == SERTESHUS)
        ingredient = _ingredient;
    else
    {
        cout << "Hibás hústípus (burger) a beolvasás közben!" << endl;
        return;
    }
    next = nullptr;
}

void Burger::set_hotness(bool x)
{
    hotness = x;
}

void Burger::printData(ofstream *newFile = nullptr) const
{
    if (newFile == nullptr)
    {
        string type;
        switch (ingredient)
        {
        case CSIRKEHUS:
            type = "CSIRKEHÚS";
            break;
        case MARHAHUS:
            type = "MARHAHÚS";
            break;
        case SERTESHUS:
            type = "SERTÉSHÚS";
            break;
        }

        cout << "BURGER: " << endl
             << name << " // "
             << price << " Ft"
             << " // "
             << kCal << " kCal"
             << " // "
             << type << " // ";
        if (hotness == false)
            cout << "NEM CSÍPŐS" << endl;
        else
            cout << "CSÍPŐS" << endl;
    }
    
    if (newFile != nullptr)
    {
        string type;
        switch (ingredient)
        {
        case CSIRKEHUS:
            type = "CSIRKEHÚS";
            break;
        case MARHAHUS:
            type = "MARHAHÚS";
            break;
        case SERTESHUS:
            type = "SERTÉSHÚS";
            break;
        default:
            cout << "Méret nem található (burger)!" << endl;
        }

        *newFile << "BURGER: " << endl
             << name << " // "
             << price << " Ft"
             << " // "
             << kCal << " kCal"
             << " // "
             << type << " // ";
        if (hotness == false)
            *newFile << "NEM CSÍPŐS" << endl;
        else
            *newFile << "CSÍPŐS" << endl;
    }
}