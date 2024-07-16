#pragma once
#include "food.h"

class Drink : public Food
{
    Size drinksize;
    bool withIce;

public:
    Drink *next;
    Drink(string _name, int _kCal, int _price);
    void setIce(bool);
    void setSize(Size);
    Size getSize() const;
    void printData(ofstream*) const;
};
