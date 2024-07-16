#pragma once
#include "food.h"
class Burger : public Food
{
    bool hotness;
    Ingredient ingredient;

public:
    Burger *next;
    Burger(string _name, int _kCal, int _price, Ingredient _ingredient);
    void set_hotness(bool);
    void printData(ofstream*) const;
};