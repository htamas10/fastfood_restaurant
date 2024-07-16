#include <iostream>
#include <fstream>
#include <ostream>
#include <stdlib.h>
#include <math.h>
#include "food.h"
#include "drinks.h"
#include "burger.h"
#include "snacks.h"

using namespace std;

int Food::ORDERNUMBER = 1; //A program indulásakor 1, amíg fut minden rendelés után eggyel növekszik.

struct Products //Struktúrába foglaljuk bele a különböző ételeket tartalmazó láncolt listák "head" pointereit.
{
    Burger *blist;
    Drink *dlist;
    Snack *slist;
};

void file_write(Food **order, int kcalsum, double pricesum, int arraysize) //Fájlba írjuk ki a számlákat.
{
    ofstream newFile;
    newFile.open("/Users/horvathtamas/Codes/CPP/BME/Prog_1_2/nagyhazi_2/szamlak.txt", ios::app);

    if (!newFile)
    {
        cout << "Nem sikerült megnyitni a számlákat tartalmazó fájlt!" << endl;
        exit(1);
    }

    newFile << "///////////////////////////////////////////////" << endl
            << "Rendelés azonosítója: " << Food::ORDERNUMBER << endl;

    for (int i = 0; i < arraysize; i++)
    {
        order[i]->printData(&newFile); //A printData fv. gondoskodik arról, hogy kiírjuk az ételek adatait (tisztán virtuális).
    }                                  //Alapértelmezett paraméterként NULL pointert kap, így dönti el, hogy hova írjon.

    newFile << endl
            << "Kalóriatartalom: " << kcalsum << " kCal" << endl;
    newFile << "Ár (áfát tartalmazza): " << pricesum << " Ft" << endl;
    newFile << "///////////////////////////////////////////////" << endl
            << endl;

    Food::ORDERNUMBER++; //Minden rendelés után növeljük a rendelés azonosítóját.

    newFile.close();
    if (!newFile)
    {
        cout << "Nem sikerült bezárni a számlákat tartalmazó fájlt!" << endl;
        exit(1);
    }
}

void reciept(Food **order, int INBE, int arraysize, int is_menu) //A számlát összeállító fv.
{
    double pricesum = 0;
    int kcalsum = 0;

    cout << "///////////////////////////////////////////////" << endl
         << "Az ön rendelése a következő:" << endl;

    for (int i = 0; i < arraysize; i++)
    {
        order[i]->printData(); //Itt nem fájlba írunk, hanem a szabványos kimenetre.
        pricesum = pricesum + order[i]->getPrice(); //Összegezzük az árakat és a kalóriatartalmakat.
        kcalsum = kcalsum + order[i]->getKcal();
        cout << endl;
    }

    cout << "A rendelés kalóriatartalma: " << kcalsum << " kCal" << endl;

    if (is_menu) //Ha menüt rendelünk akkor belépünk, mivel változtatnunk kell az árat.
    {
        pricesum = round(pricesum * (1 - Food::DISCOUNT / 100));
        cout << "Fizetendő összeg (áfát tartalmazza): " << pricesum << " Ft"
             << " (-" << Food::DISCOUNT << "%)" << endl; //Mivel menüt rendeltünk így -15% kedvezmény jár.
    }

    else
    {
        cout << "Fizetendő összeg (áfát tartalmazza): " << pricesum << " Ft" << endl;
    }

    if (kcalsum > INBE) //Ha meghaladtuk az INBÉ értékünket akkor figyelmeztet minket a program, ezt figyelmen kívül hagyhatjuk.
    {
        cout << "\nFIGYELEM!" << endl;
        cout << "A rendelés kalóriatartalma meghaladja az ön által megadott INBÉ (" << INBE << " kCal) értéket." << endl;
        cout << "Biztos le szeretné adni a rendelést?" << endl
             << endl;
        cout << "IGEN <--> NEM" << endl;

        string choice; //Biztos felvesszük a rendelést?
        cin >> choice;

        if (choice == "IGEN")
        {
            cout << endl
                 << "Rendelését sikeresen felvettük!" << endl;
            cout << "Köszönjük, hogy éttermünket választotta!" << endl;
        }

        if (choice == "NEM")
            cout << "Rendelését töröltük!" << endl;
    }

    cout << "///////////////////////////////////////////////" << endl;
    file_write(order, kcalsum, pricesum, arraysize); //A rendelést kiírjuk a fájlba is.
}

template <typename T>
T *get_item(T *head, string name) //Különböző termékeket tartalmazó listákat kap (burger, üdítő, snack).
{                                 //Megkeresi az általunk választottakat a listában.
    while (head != nullptr)
    {
        if (head->getName() == name)
        {
            return head;
        }
        head = head->next;
    }
    return nullptr;
}

template <typename U> //A fájlolvasás után felépítjük a láncolt listákat (az üdítőket és a snackeket).
U *data_to_list(U *head, int counter, string name, int kCal, int price)
{
    if (counter == 0) //Ha az első termék jött, akkor ő lesz a head.
    {
        U *newElement = new U(name, kCal, price);
        head = newElement;

        head->next = nullptr;
        newElement->next = nullptr;
    }

    else
    {
        U *newElement = new U(name, kCal, price);
        U *p = NULL;
        for (p = head; p->next != NULL; p = p->next)
            ;
        p->next = newElement;
        p->next->next = nullptr;
    }

    return head;
}

Burger *data_to_list(Burger *head, int counter, string category, string name, int kCal, int price, Ingredient ingredient)
{
    if (counter == 0)                       //Ugyan az, mint az előző fv. csak ez Burgerre működik csak.
    {
        Burger *newElement = new Burger(name, kCal, price, ingredient);
        head = newElement;
        head->next = nullptr;
        newElement->next = nullptr;
    }

    else
    {
        Burger *newElement = new Burger(name, kCal, price, ingredient);
        Burger *p = NULL;
        for (p = head; p->next != NULL; p = p->next)
            ;
        p->next = newElement;
        p->next->next = NULL;
    }

    return head;
}

Ingredient enum_converter(string ingredient)  //Visszaadja az enumot, ami megfelel a beolvasott stringnek.
{
    if (ingredient == "MARHAHUS")
        return MARHAHUS;
    if (ingredient == "CSIRKEHUS")
        return CSIRKEHUS;
    if (ingredient == "SERTESHUS")
        return SERTESHUS;
    else
    {
        cout << "Hibás beolvasás a hústípusoknál!";
        exit(1);
    }
}

Products *file_read(Products &p, int &x, int &y, int &z) //A fájlból olvasás függvénye.
{                                                        //x,y,z a különböző burgerek, üdítők és snackek száma.
    int i = 0, j = 0, k = 0;

    ifstream newFile;
    newFile.open("/Users/horvathtamas/Codes/CPP/BME/Prog_1_2/nagyhazi_2/ajanlatok.txt");
    if (!newFile)
    {
        cout << "Nem sikerült megnyitni az ajánlatokat tartalmazó fájlt!" << endl;
        exit(1);
    }

    string category;                    //A category string segítségével tudjuk, hogy épp melyik terméktípust olvassuk.
    Burger *bhead = nullptr;
    Drink *dhead = nullptr;
    Snack *shead = nullptr;

    while (!newFile.eof())   //Elkezdjük olvasni a fájlt.
    {
        newFile >> category;
        string name, ingredient, buffer;
        int kCal, price;
        if (category == "BURGEREK") //Elsőként a burgereket stb.
        {
            while (!newFile.eof())
            {
                newFile >> name;
                if (name == "//////////////////////////")
                {
                    break;
                }
                newFile >> kCal;
                newFile >> price;
                newFile >> ingredient;
                bhead = data_to_list(bhead, i, category, name, kCal, price, enum_converter(ingredient));
                i++;
                x++;
            }
        }

        if (category == "UDITOK")
        {
            while (!newFile.eof())
            {
                newFile >> name;
                if (name == "//////////////////////////")
                    break;

                newFile >> kCal;
                newFile >> price;
                dhead = data_to_list(dhead, j, name, kCal, price);
                j++;
                y++;
            }
        }

        if (category == "SNACKS")
        {
            while (!newFile.eof())
            {
                newFile >> name;
                if (name == "//////////////////////////")
                    break;

                newFile >> kCal;
                newFile >> price;
                shead = data_to_list(shead, k, name, kCal, price);
                k++;
                z++;
            }
        }

        if (name == "//////////////////////////" && category == "SNACKS") //Ha az utolsó kategória a snack volt,
            break;                                                        //és látjuk az ajánlatok végét, befejezzük az olvasást.
    }  

    p.blist = bhead; //Elmentjük a struktúrákba a lista "head" pointereit, mivel így egyszerre mindhármat visszaadhatjuk.
    p.dlist = dhead;
    p.slist = shead;

    string buffer;
    double rate;

    while (getline(newFile, buffer)) //Beolvassuk a szorzókat (mennyivel drágább a közepes/nagy termék).
    {
        if (buffer == "Szorzók:")
        {
            newFile >> buffer >> rate >> buffer;
            Food::setMediumpricerate(rate); //Be is állítjuk őket.
            newFile >> buffer >> rate;
            Food::setBigpricerate(rate);
            break;
        }
    }

    newFile.close();
    if (!newFile)
    {
        cout << "Nem sikerült bezárni az ajánlatokat tartalmazó fájlt!" << endl;
        exit(1);
    }
    return &p;
}

template <typename Y>
string num_to_string(Y *list, int num, int counter) //A counter segítségével tudjuk épp melyik terméktípust tartalmazó listában
{                                                   //keresünk, a num pedig a burgerek/üdítők/snackek számát jelenti.
    if (counter == 0)
    {
        for (int i = 1; i < num; i++)
            list = list->next;

        return list->getName();
    }

    if (counter == 1)
    {
        for (int i = 1; i < num; i++)
            list = list->next;

        return list->getName();
    }

    else
    {
        for (int i = 1; i < num; i++)
            list = list->next;

        return list->getName();
    }
}

template <typename W>
void print_menu(int j, W list) //Kiíratjuk a menüt, név/ár/kalória.
{
    for (int i = 1; i <= j; i++)
    {
        cout << "(" << i << ") " << list->getName() << " // " << list->getPrice() << " Ft // " << list->getKcal() << " kCal" << endl;
        if (list->next != nullptr)
            list = list->next;
    }
}

template <typename Z>
Size size_chooser(int num, Z &item) //Num a felhasználói interface-nek megfelelő méretet jelöli.
{                                   //A méretnek megfeleően megváltoztatja az árat.
    switch (num)
    {
    case 1:
        return KICSI;
    case 2:
        item->setPrice(round(item->getPrice() * (1 + Food::getMediumpricerate() / 100)));
        return KOZEPES;
    case 3:
        item->setPrice(round(item->getPrice() * (1 + Food::getBigpricerate() / 100)));
        return NAGY;
    default:
        cout << "Nem létező mező, kérjük próbálja újra!" << endl;
        exit(1);
    }
}

template <typename X>
void size_interface(X *item)//Kiírja a lehetséges méreteket, illetve azok felárait (a szorzók segítségével).
{                            
    int choicenum;

    cout << "Válasszon méretet:" << endl
         << "(1) KICSI"
         << "  +0 Ft" << endl
         << "(2) KÖZEPES"
         << "  +" << round(item->getPrice() * Food::getMediumpricerate() / 100) << " Ft" << endl
         << "(3) NAGY"
         << "  +" << round(item->getPrice() * Food::getBigpricerate() / 100) << " Ft" << endl;

    cin >> choicenum;
    item->setSize(size_chooser(choicenum, item)); //Ha megvan a méret a választás után meghívjuk az előző fv.-t.
}

void single_order_interface(Burger *blist, Drink *dlist, Snack *slist, int b_amount, int d_amount, int s_amount, int INBE)
{                       //A sima rendelést megvalósítő felhasználói interface.
    int choicenum;      //A választásaink számát tartalmazó int.
    int counter = 0;    //Miután kiválasztottuk a burgert, megnöveljük, hogy a következő fv. tudja mely terméken fog dolgozni.
    int i = 0;

    Food *order[50] = {0}; //Feltesszük, hogy 50-nél több rendelést a rendszer nem fogad.
                           //Eltároljuk ebbe a tömbbe a termékeket, mint heterogén kollekció.
    cout << "BURGEREK:" << endl;
    print_menu(b_amount, blist);
    cout << "\n(" << b_amount + 1 << ")"
         << " ÜDÍTŐK>>" << endl;

    while (1)
    {
        cin >> choicenum;
        if (choicenum == b_amount + 1)
            break;

        if (choicenum <= 0 || choicenum > b_amount)
        {
            cout << "Nem létező mező, kérjük próbálja újra!" << endl;
            exit(1);
        }

        Burger *burger = get_item(blist, num_to_string(blist, choicenum, counter));

        cout << "Csípős szósszal?" << endl
             << "(1) IGEN" << endl
             << "(2) NEM" << endl;
        cin >> choicenum;

        if (choicenum != 1 && choicenum != 2)
        {
            cout << "Nem létező mező, kérjük próbálja újra!" << endl;
            exit(1);
        }

        if (choicenum == 1)
        {
            burger->set_hotness(true);
        }

        cout << "Sikeres felvétel!" << endl;
        order[i] = burger; //Először a burgerek foglalják el a tömb első elemeit, i jelöli azok számát kezdetben.
        i++;
    }
    counter++;

    cout << "ÜDÍTŐK:" << endl;
    print_menu(d_amount, dlist);
    cout << "\n(" << d_amount + 1 << ")"
         << " SNACKEK>>" << endl;

    while (1)
    {
        cin >> choicenum;
        if (choicenum == d_amount + 1)
            break;

        if (choicenum <= 0 || choicenum > d_amount)
        {
            cout << "Nem létező mező, kérjük próbálja újra!" << endl;
            exit(1);
        }

        Drink *drink = get_item(dlist, num_to_string(dlist, choicenum, counter));
        cout << "Jéggel?" << endl
             << "(1) IGEN" << endl
             << "(2) NEM" << endl;
        cin >> choicenum;

        if (choicenum != 1 && choicenum != 2)
        {

            cout << "Nem létező mező, kérjük próbálja újra!" << endl;
            exit(1);
        }

        if (choicenum == 2)
        {
            drink->setIce(false);
        }

        size_interface(drink);
        cout << "Sikeres felvétel!" << endl;

        order[i] = drink;
        i++;
    }
    counter++;

    cout << "SNACKEK:" << endl;
    print_menu(s_amount, slist);
    cout << "\n(" << s_amount + 1 << ")"
         << " RENDELÉS VÉGE>>" << endl;

    while (1)
    {
        cin >> choicenum;
        if (choicenum == s_amount + 1)
            break;

        if (choicenum <= 0 || choicenum > b_amount)
        {
            cout << "Nem létező mező, kérjük próbálja újra!" << endl;
            exit(1);
        }

        Snack *snack = get_item(slist, num_to_string(slist, choicenum, counter));
        size_interface(snack);

        order[i] = snack; 
        i++;
        cout << "Sikeres felvétel!" << endl;
    }
    reciept(order, INBE, i, 0); //Miután feltöltöttük a tömbböt elküldjük a számlát előállító fv.-nek, ahol i a rendelt
}                            //termékek számát jelöli.

void menu_interface(Burger *blist, Drink *dlist, Snack *slist, int b_amount, int d_amount, int s_amount, int INBE)
{                   //A menü felhasználói interface-ét megvalósító fv., hasonló a sima rendelés függvényéhez.
    int choicenum;
    int counter = 0; // Num_to_string melyik termékcsoporton dolgozzon (0=burgerek, 1=üdítők, 2=snackek)
    int newprice;    // Módosított ár

    cout << "BURGEREK:" << endl;
    print_menu(b_amount, blist);
    cin >> choicenum;

    if (choicenum <= 0 || choicenum > b_amount)
    {
        cout << "Nem létező mező, kérjük próbálja újra!" << endl;
        exit(1);
    }

    Burger *burger = get_item(blist, num_to_string(blist, choicenum, counter));

    cout << "Csípős szósszal?" << endl //Csípős legyen-e a burger vagy ne? Alapértelmezetten nem csípős.
         << "(1) IGEN" << endl
         << "(2) NEM" << endl;
    cin >> choicenum;

    if (choicenum != 1 && choicenum != 2)
    {
        cout << "Nem létező mező, kérjük próbálja újra!" << endl;
        exit(1);
    }

    if (choicenum == 1)
    {
        burger->set_hotness(true);
    }

    counter++;
    cout << "Sikeres felvétel!" << endl;

    cout << "ÜDÍTŐK:" << endl;
    print_menu(d_amount, dlist);
    cin >> choicenum;

    if (choicenum <= 0 || choicenum > d_amount)
    {
        cout << "Nem létező mező, kérjük próbálja újra!" << endl;
        exit(1);
    }

    Drink *drink = get_item(dlist, num_to_string(dlist, choicenum, counter));

    cout << "Jéggel?" << endl   //Jéggel kérjük az italt vagy sem? Alapértelmezetten jéggel.
         << "(1) IGEN" << endl
         << "(2) NEM" << endl;
    cin >> choicenum;

    if (choicenum != 1 && choicenum != 2)
    {
        cout << "Nem létező mező, kérjük próbálja újra!" << endl;
        exit(1);
    }

    if (choicenum == 2)
    {
        drink->setIce(false);
    }
    size_interface(drink);

    counter++;
    cout << "Sikeres felvétel!" << endl;

    cout << "SNACKEK:" << endl;
    print_menu(s_amount, slist);
    cin >> choicenum;

    if (choicenum <= 0 || choicenum > b_amount)
    {
        cout << "Nem létező mező, kérjük próbálja újra!" << endl;
        exit(1);
    }

    Snack *snack = get_item(slist, num_to_string(slist, choicenum, counter));
    size_interface(snack);
    cout << "Sikeres felvétel!" << endl;

    Food *menu[3];  //Elmentjük a rendelést (3 termék) egy 3 elemű tömbbe, ami egy heterogén kollekció.
    menu[0] = burger;
    menu[1] = drink;
    menu[2] = snack;
    reciept(menu, INBE, 3, 1);
}

void interface(Burger *blist, Drink *dlist, Snack *slist, int x, int y, int z)
{                           //Az üdvözlő interface-t megvalósító fv.
    int choicenum;
    int INBE;

    cout << "Köszöntjük éttermünkben!" << endl;
    cout << "Kérjük adja meg az INBÉ értékét:" << endl; //Megadhatjuk az INBÉ értékünket 0 és 10000 között.
    cin >> INBE;                                        //Ennél több kalóriát tartalmazó rendelés esetén figyelmeztet a program.

    if (INBE <= 0 || INBE > 10000)
    {
        cout << "Hibás INBÉ érték, kérjük próbálja újra!" << endl;
        return;
    }

    cout << "Menü vagy sima rendelés?" << endl;   //Eldönthetjük, hogy milyen rendeléstípust választunk, menü vagy sima.
    cout << "(1) Termékek >>\n(2) Menük >>" << endl;
    cin >> choicenum;

    if (choicenum != 1 && choicenum != 2)
    {
        cout << "Hibás mező, kérjük próbálja újra!" << endl;
        exit(1);
    }

    if (choicenum == 1)
    {
        single_order_interface(blist, dlist, slist, x, y, z, INBE); //Ha (1), akkor sima rendelés.
    }

    if (choicenum == 2)
    {
        menu_interface(blist, dlist, slist, x, y, z, INBE); //Ha (2), akkor menüs rendelés.
    }
}

template <typename M>
void dispose_list(M *head) //Felszabadítja a burgereket, üdítőket és snackeket tartalmazó láncolt listákat.
{
    M *current = head;
    M *next = NULL;

    while (current != NULL)
    {
        next = current->next;
        delete current;
        current = next;
    }

    head = nullptr;
}

int main() //A main függvény a főbb függvények hívásához szolgál.
{
    int bamount = 0, damount = 0, samount = 0; //Rendre a burgerek/üdítők/snackek számát tároló intek.
    Products products;
    file_read(products, bamount, damount, samount); //A products struktúrában megkapjuk mindhárom lista "head" pointerét.

    Burger *burger_list = products.blist;
    Drink *drink_list = products.dlist;
    Snack *snack_list = products.slist;

    string choice = "IGEN";     //Az első választás mindenképp IGEN lesz, hiszen először rendelünk.
    while (choice == "IGEN")    //Eldönthetjük, hogy rendelünk-e még az utóbbi rendelésünk után.
    {
        interface(burger_list, drink_list, snack_list, bamount, damount, samount);
        cout << "Szeretne új rendelést felvenni?" << endl
             << endl;
        cout << "IGEN <--> NEM" << endl;
        cin >> choice;
    }

    dispose_list(burger_list); //Lebontjuk a listákat.
    dispose_list(drink_list);
    dispose_list(snack_list);

    return 0;
}