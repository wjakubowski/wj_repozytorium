#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include "plyty.h"

int DuzaPlyta::duzaPlytaX = 2800;
int DuzaPlyta::duzaPlytaY = 2070;

int DuzaPlyta::wymiarX() {
    return duzaPlytaX;
}

int DuzaPlyta::wymiarY() {
    return duzaPlytaY;
}

//##############################################################

vector<int> MalaPlyta::kolejnosc = vector<int>();
vector<vector<double> > MalaPlyta::wymiary = vector<vector<double> >();
vector<MalaPlyta> MalaPlyta::maleplyty = vector<MalaPlyta>();
int MalaPlyta::gruboscOchrony = 10;

int MalaPlyta::poleZachodzenia(Prostokat & p1, Prostokat & p2) {
    int del_x,del_y;
    kolejnosc.resize(0);
    kolejnosc.push_back(p1.x);
    kolejnosc.push_back(p2.x);
    kolejnosc.push_back(p1.x+p1.wymiarX());
    kolejnosc.push_back(p2.x+p2.wymiarX());
    sort(kolejnosc.begin(),kolejnosc.end());
    if ((kolejnosc[0]==p1.x && kolejnosc[1]==p1.x+p1.wymiarX()) || (kolejnosc[0]==p2.x && kolejnosc[1]==p2.x+p2.wymiarX())) {
        return 0;
    }
    else {
        del_x=kolejnosc[2]-kolejnosc[1];
    }
    kolejnosc.resize(0);
    kolejnosc.push_back(p1.y);
    kolejnosc.push_back(p2.y);
    kolejnosc.push_back(p1.y+p1.wymiarY());
    kolejnosc.push_back(p2.y+p2.wymiarY());
    sort(kolejnosc.begin(),kolejnosc.end());
    if ((kolejnosc[0]==p1.y && kolejnosc[1]==p1.y+p1.wymiarY()) || (kolejnosc[0]==p2.y && kolejnosc[1]==p2.y+p2.wymiarY())) {
        return 0;
    }
    else {
        del_y=kolejnosc[2]-kolejnosc[1];
    }
    return del_x*del_y;
}

int MalaPlyta::poleWystawania(Prostokat & duzaPlyta) {
    return this->wymiarX()*this->wymiarY() - poleZachodzenia(*this,duzaPlyta);
}

int MalaPlyta::poleZachodzeniaOchrona(Prostokat & p1, Prostokat & p2) {
    int del_x,del_y;
    kolejnosc.resize(0);
    kolejnosc.push_back(p1.x-gruboscOchrony);
    kolejnosc.push_back(p2.x-gruboscOchrony);
    kolejnosc.push_back(p1.x+p1.wymiarX()+gruboscOchrony);
    kolejnosc.push_back(p2.x+p2.wymiarX()+gruboscOchrony);
    sort(kolejnosc.begin(),kolejnosc.end());
    if ((kolejnosc[0]==p1.x && kolejnosc[1]==p1.x+p1.wymiarX()) || (kolejnosc[0]==p2.x && kolejnosc[1]==p2.x+p2.wymiarX())) {
        return 0;
    }
    else {
        del_x=(kolejnosc[2]+gruboscOchrony)-(kolejnosc[1]-gruboscOchrony);
    }
    kolejnosc.resize(0);
    kolejnosc.push_back(p1.y-gruboscOchrony);
    kolejnosc.push_back(p2.y-gruboscOchrony);
    kolejnosc.push_back(p1.y+p1.wymiarY()+gruboscOchrony);
    kolejnosc.push_back(p2.y+p2.wymiarY()+gruboscOchrony);
    sort(kolejnosc.begin(),kolejnosc.end());
    if ((kolejnosc[0]==p1.y-gruboscOchrony && kolejnosc[1]==p1.y+p1.wymiarY()+gruboscOchrony) ||
        (kolejnosc[0]==p2.y-gruboscOchrony && kolejnosc[1]==p2.y+p2.wymiarY()+gruboscOchrony)) {
        return 0;
    }
    else {
        del_y=(kolejnosc[2]+gruboscOchrony)-(kolejnosc[1]-gruboscOchrony);
    }
    return del_x*del_y;
}

int MalaPlyta::poleWystawaniaOchrona(Prostokat & duzaPlyta) {
    return (this->wymiarX()+2*gruboscOchrony)*(this->wymiarY()+2*gruboscOchrony) - poleZachodzeniaOchrona(*this,duzaPlyta);
}

int MalaPlyta::wymiarX() {
    if(!orientacjaPionowa)
        return wymiary[nr_plyty][0];
    else
        return wymiary[nr_plyty][1];
}

int MalaPlyta::wymiarY() {
    if(!orientacjaPionowa)
        return wymiary[nr_plyty][1];
    else
        return wymiary[nr_plyty][0];
}

void MalaPlyta::zaladujMalePlyty() {
    fstream plik;
    plik.open ("maleplyty.txt");
    string linia;
    stringstream strstr;
    int a,b;
    while(true) {
        linia = "";
        getline(plik,linia);
        if(!plik.good()) {
            break;
        }
        strstr.clear();
        strstr.str(linia);
        strstr >> a;
        strstr >> b;

        wymiary.push_back(vector<double>());
        wymiary.back().push_back(a);
        wymiary.back().push_back(b);
        //cout << linia << endl << "a\t" << wymiary.back()[0] << endl << "b\t" << wymiary.back()[1] << endl;
    }

    for(unsigned i=0; i<wymiary.size(); ++i){
        maleplyty.push_back(MalaPlyta(0,0,i,1));
    }
    plik.close();
}
