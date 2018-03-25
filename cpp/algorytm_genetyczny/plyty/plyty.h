#include <iostream>
#include <vector>

using namespace std;

class Prostokat {
public:
    int x;
    int y;
    virtual int wymiarX()=0;
    virtual int wymiarY()=0;
    Prostokat(int x, int y):
        x(x),y(y) {}
};

//##############################################################

class DuzaPlyta : public Prostokat {
public:
    static int duzaPlytaX;
    static int duzaPlytaY;
    int wymiarX();
    int wymiarY();
    DuzaPlyta():Prostokat(0,0) {}
};

//##############################################################

class MalaPlyta : public Prostokat {
public:
    int nr_plyty;
    bool orientacjaPionowa;
    bool czyWycinanie;
    int wymiarX();
    int wymiarY();
    //zmienna do obliczen
    static vector<int> kolejnosc;
	static vector<vector<double> > wymiary;
	static vector<MalaPlyta> maleplyty;
	static int gruboscOchrony;

    MalaPlyta(int x, int y, int nr_plyty, bool orientacjaPionowa):
        Prostokat(x,y),nr_plyty(nr_plyty),orientacjaPionowa(orientacjaPionowa) {czyWycinanie = true;}

	static void zaladujMalePlyty();
	static int poleZachodzenia(Prostokat & p1, Prostokat & p2);
    int poleWystawania(Prostokat & duzaPlyta);

    static int poleZachodzeniaOchrona(Prostokat & p1, Prostokat & p2);
    int poleWystawaniaOchrona(Prostokat & duzaPlyta);
};
