#include "food.h"

Food::Food(string _name, int _kCal, int _price)
{
    if (_kCal < 0)
        kCal = -_kCal;
    else
        kCal = _kCal;
    if (_price < 0)
        price = -_price;
    else
        price = _price;
    name = _name;
}

int Food::getKcal() const
{
    return kCal;
}

int Food::getPrice() const
{
    return price;
}

string Food::getName() const
{
    return name;
}

void Food::setPrice(int newPrice)
{
    price = newPrice;
}

double Food::getMediumpricerate()
{
    return MEDIUMPRICE;
}

double Food::getBigpricerate()
{
    return BIGPRICE;
}

void Food::setMediumpricerate(double x)
{
    if (x > 1)
        Food::MEDIUMPRICE = x;

    else
        return;
}

void Food::setBigpricerate(double x)
{
    if (x > 1)
        Food::BIGPRICE = x;

    else
        return;
}

const double Food::DISCOUNT = 15; //-15% jár a menüt vásárlóknak.
double Food::BIGPRICE = 1;
double Food::MEDIUMPRICE = 1;