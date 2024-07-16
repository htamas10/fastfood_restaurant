#pragma once
#include <iostream>
using namespace std;

enum Ingredient
{
    MARHAHUS,
    CSIRKEHUS,
    SERTESHUS
};

enum Size
{
    KICSI,
    KOZEPES,
    NAGY
};

class Food
{
protected:
    int kCal;
    int price;
    string name;
    static double MEDIUMPRICE; //A közepes méretű termékek árszorzója.
    static double BIGPRICE; //A nagy méretű termékek árszorzója.

public:
    Food(string _name, int _kCal, int _price);
    virtual void printData(ofstream* = nullptr) const = 0;
    static double getMediumpricerate();
    static double getBigpricerate();
    static void setMediumpricerate(double);
    static void setBigpricerate(double);
    int getKcal() const;
    int getPrice() const;
    string getName() const;
    void setPrice(int);
    static const double DISCOUNT; //A menükre vonatkozó árkedvezmény.
    static int ORDERNUMBER; //A rendelés száma, amíg fut a program 1-től indul addig amíg rendelünk, aztán újraindul.
};