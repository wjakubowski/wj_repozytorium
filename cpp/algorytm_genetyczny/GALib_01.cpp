#include <cstdlib>
#include <iostream>
#include <conio.h>
#include <ga.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include "plyty/plyty.h"
#include "ga/GARealGenome.h"
#include "ga/GARealGenome.C"

using namespace std;

//licznik wykonań algorytmu, niektóre rzeczy wpipisują się co ileś wykonań licznika
int licznik=0;

//binarny fenotyp jest interpretowany jako: round(fenotyp_rzeczywistoliczbowy/dlogosc_genu_binarnego)
double dlogosc_genu_binarnego = 10.0;

//wsp zaawansowania symulacji ga.generation()/ngen;
double wsp_zaawansowania;

// Ustalamy wartosci parametrow algorytmu
int popsize  = 100;
int ngen     = 10000;
float pmut   = 0.1;
float pcross = 0.6;

// Informujemy kompilator, ze gdzies dalej bedzie funkcja celu
float objective(GAGenome & c);

//duża płyta reprezentująca płytę do wycinania
DuzaPlyta duzaPlyta;

//wypisywanie danych do pliku output.txt
void wypiszOutput(GAGenome & c);

//maksymalne warości człkowitej pow. płyt, zachodzących pow. płyt (liczona każda z każdą (N^2)/2),
//sumy odległości płyt od osi X i Y
double maxPowierzchniaPlyt=0;
double maxPowZachodzeniaPlyt=0;
double maxPowierzchniaPlytOchrona=0;
double maxPowZachodzeniaPlytOchrona=0;
double maxSumaOdleglosci=0;

//funkcja kary
double kara(double blad, double maxBlad ){

    double norma = blad/maxBlad;
    return pow(1.0-norma,2.0);
}

//twarda funkcja kary
double karaTwarda(double blad){

    if (blad != 0.0)
        return 0.0;
    else
        return 1.0;
}

float comparator(const GAGenome&, const GAGenome&);

int main(int argc, char *argv[])
{
    //wczytuje plik "maleplyty.txt" i statyczna tworze liste obiektow MalaPlyta::maleplyty
    MalaPlyta::zaladujMalePlyty();

    //zliczam maksymalne mozliwe wartosci powierzchni plyt, ich poktywania się
    //i sumy odległosci od osi X i Y (chcę dosuwać plyty do rogu)
    for(unsigned i=0; i<MalaPlyta::wymiary.size(); ++i) {
        maxPowierzchniaPlyt += MalaPlyta::wymiary[i][0] * MalaPlyta::wymiary[i][1];
        maxPowierzchniaPlytOchrona += ((MalaPlyta::wymiary[i][0]+2*MalaPlyta::gruboscOchrony) *
                                       (MalaPlyta::wymiary[i][1]+2*MalaPlyta::gruboscOchrony));
        maxSumaOdleglosci += 2080;
        maxSumaOdleglosci += 2070;
        for(unsigned j=i+1; j<MalaPlyta::maleplyty.size(); ++j) {
            maxPowZachodzeniaPlyt += MalaPlyta::poleZachodzenia(MalaPlyta::maleplyty[i], MalaPlyta::maleplyty[j]);
            maxPowZachodzeniaPlytOchrona += MalaPlyta::poleZachodzeniaOchrona(MalaPlyta::maleplyty[i], MalaPlyta::maleplyty[j]);
        }
    }

    GARealAlleleSetArray allele;
    for(unsigned i=0; i<MalaPlyta::wymiary.size(); ++i) {
        allele.add(0.0, 2800.0, GAAllele::INCLUSIVE,GAAllele::INCLUSIVE);
        allele.add(0.0, 2070.0, GAAllele::INCLUSIVE,GAAllele::INCLUSIVE);
        allele.add(0.0, dlogosc_genu_binarnego, GAAllele::INCLUSIVE,GAAllele::EXCLUSIVE);
        allele.add(0.0, dlogosc_genu_binarnego, GAAllele::INCLUSIVE,GAAllele::EXCLUSIVE);
    }
    GARealGenome genome(allele, objective);

    GASimpleGA ga(genome);

    //Inicjalizujemy algorytm ustalonymi wczesniej wartosciami
    ga.populationSize(popsize);
    ga.nGenerations(ngen);
    ga.pMutation(pmut);
    ga.pCrossover(pcross);
    genome.mutator(GARealSwapMutator);

    //wybór metody skalowania
    GASigmaTruncationScaling scaling;
    //GASharing scale(comparator);
    ga.scaling(scaling);

    ga.elitist(gaFalse);
    //wybór metody selekcji
    GARankSelector selekcja;
    ga.selector(selekcja);

    //wybór metody krzyżowania
    ga.crossover(GARealBlendCrossover);

    //wypisywanie statystyk
    ga.scoreFilename("zbieznosc.dat");
    ga.scoreFrequency(10);
    ga.flushFrequency(50);
    ga.selectScores(GAStatistics::Mean |
                    GAStatistics::Maximum |
                    GAStatistics::Minimum |
                    GAStatistics::Deviation |
                    GAStatistics::Diversity);
    ga.recordDiversity(gaFalse);
    ga.initialize((unsigned)time(0));

    //iteracja algorytmu
    while (!ga.done())
    {
        //modyfikacja współczynników tolerancji
        wsp_zaawansowania = 1.0*ga.generation()/ngen-0.5;
        if (wsp_zaawansowania < 0.0 )
            wsp_zaawansowania = 0.0;

        //zapis do pliku "output.txt" i rysowanie płyt

        //GARealGenome & best_genome = (GARealGenome &)ga.statistics().bestIndividual();
        GARealGenome & best_now = (GARealGenome &)ga.statistics().bestPopulation().individual(0);

        cout <<
            ga.statistics().generation() << '\t' <<
            ga.statistics().online() << '\t' <<
            ga.statistics().maxEver() << '\t' <<
            ga.statistics().offlineMax() << '\t' <<
            endl;

        if(0 == ga.generation()%200)
        {
            wypiszOutput(best_now);
            system("python ./rysuj.py");
        }

        ga.step();
    }

    GARealGenome & best_genome = (GARealGenome &)ga.statistics().bestIndividual();
    GARealGenome & best_now = (GARealGenome &)ga.statistics().bestPopulation().individual(0);
    wypiszOutput(best_genome);
    system("python ./rysuj.py");
    wypiszOutput(best_now);
    system("python ./rysuj.py");

    cout << "Algorytm zakonczony!" << endl;
    return EXIT_SUCCESS;
}

//szcytuje małe płyty z genomu
void wczytajMalePlytyZGenomu(GAGenome & c) {
    GARealGenome & genome = (GARealGenome &) c;
    for(unsigned i=0; i<MalaPlyta::maleplyty.size(); ++i) {
        MalaPlyta::maleplyty[i].x=genome.gene(4*i);
        MalaPlyta::maleplyty[i].y=genome.gene(4*i+1);
        MalaPlyta::maleplyty[i].orientacjaPionowa=round(genome.gene(4*i+2)/dlogosc_genu_binarnego);
        MalaPlyta::maleplyty[i].czyWycinanie=round(genome.gene(4*i+3)/dlogosc_genu_binarnego);
        /*cout << MalaPlyta::maleplyty[i].x << "\t" << MalaPlyta::maleplyty[i].y
        << "\t" << MalaPlyta::maleplyty[i].orientacjaPionowa << "\t" <<
        MalaPlyta::maleplyty[i].czyWycinanie << endl;*/
    }
}

//wypisuje plik "output.txt"
void wypiszOutput(GAGenome & c) {
    //szcytuje małe płyty z genomu
    wczytajMalePlytyZGenomu(c);

    //czyszcze plik "output.txt"
    std::ofstream ofs;
    ofs.open("output.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    //zliczam łączną powierzchnię małych płyt, powierzchnię ich zachodzenia na siebie,
    //sumę odległości płyt od osi X i Y
    fstream plik;
    plik.open("output.txt", std::ofstream::out);
    int obszarPlyt=0;
    for(unsigned i=0; i<MalaPlyta::maleplyty.size(); ++i) {
        if(MalaPlyta::maleplyty[i].czyWycinanie == false)
            continue;
        obszarPlyt += MalaPlyta::wymiary[i][0] * MalaPlyta::wymiary[i][1];
    }
    plik << obszarPlyt << endl;
    for(unsigned i=0; i<MalaPlyta::maleplyty.size(); ++i) {
        plik << MalaPlyta::wymiary[i][0] << " ";
        plik << MalaPlyta::wymiary[i][1] << " ";

        if(MalaPlyta::maleplyty[i].czyWycinanie == false) {
            plik << -1 << " ";
            plik << -1 << " ";
        }
        else {
            plik << MalaPlyta::maleplyty[i].x << " ";
            plik << MalaPlyta::maleplyty[i].y << " ";
        }
        plik << MalaPlyta::maleplyty[i].orientacjaPionowa << endl;
    }
    plik.close();
}

//funkcja dostosowania
float objective(GAGenome & c) {

    GARealGenome & genome = (GARealGenome &) c;
    wczytajMalePlytyZGenomu(c);

    double taPlytaZachodzenie, taPlytaWystawanie, taPlytaPowierzchnia;
    double wszystkiePlytyZachozenie = 0;
    double wszystkiePlytyWystawanie = 0;
    double wszystkiePlytyPowierzchnia = 0;

    /*double taPlytaZachodzenieOchrona, taPlytaWystawanieOchrona, taPlytaPowierzchniaOchrona;
    double wszystkiePlytyZachozenieOchrona = 0;
    double wszystkiePlytyWystawanieOchrona = 0;
    double wszystkiePlytyPowierzchniaOchrona = 0;*/

    double wszystkiePlytyOdleglosc = 0;

    /*for(unsigned i=0; i<MalaPlyta::wymiary.size(); ++i) {
        if(MalaPlyta::maleplyty[i].czyWycinanie == false)
            continue;
        maxPowierzchniaPlyt += MalaPlyta::wymiary[i][0] * MalaPlyta::wymiary[i][1];
        maxPowierzchniaPlytOchrona += ((MalaPlyta::wymiary[i][0]+2*MalaPlyta::gruboscOchrony) *
                                       (MalaPlyta::wymiary[i][1]+2*MalaPlyta::gruboscOchrony));
        maxSumaOdleglosci += 2080;
        maxSumaOdleglosci += 2070;
        for(unsigned j=i+1; j<MalaPlyta::maleplyty.size(); ++j) {
            maxPowZachodzeniaPlyt += MalaPlyta::poleZachodzenia(MalaPlyta::maleplyty[i], MalaPlyta::maleplyty[j]);
            maxPowZachodzeniaPlytOchrona += MalaPlyta::poleZachodzeniaOchrona(MalaPlyta::maleplyty[i], MalaPlyta::maleplyty[j]);
        }
    }*/
    int n=0;
    for(unsigned i=0; i<MalaPlyta::maleplyty.size(); ++i) {
        if(MalaPlyta::maleplyty[i].czyWycinanie == false)
            continue;
        n++;

        taPlytaWystawanie = MalaPlyta::maleplyty[i].poleWystawania(duzaPlyta);
        taPlytaPowierzchnia = MalaPlyta::wymiary[i][0] * MalaPlyta::wymiary[i][1];
        wszystkiePlytyWystawanie += taPlytaWystawanie;
        wszystkiePlytyPowierzchnia += taPlytaPowierzchnia;

        /*taPlytaWystawanieOchrona = MalaPlyta::maleplyty[i].poleWystawaniaOchrona(duzaPlyta);
        taPlytaPowierzchniaOchrona =
            ((MalaPlyta::wymiary[i][0]+2*MalaPlyta::gruboscOchrony) *
             (MalaPlyta::wymiary[i][1]+2*MalaPlyta::gruboscOchrony));
        wszystkiePlytyWystawanieOchrona += taPlytaWystawanieOchrona;
        wszystkiePlytyPowierzchniaOchrona += taPlytaPowierzchniaOchrona;*/

        wszystkiePlytyOdleglosc +=  MalaPlyta::maleplyty[i].x*MalaPlyta::maleplyty[i].x;
        wszystkiePlytyOdleglosc +=  MalaPlyta::maleplyty[i].y*MalaPlyta::maleplyty[i].y;

        taPlytaZachodzenie = 0;
        //taPlytaZachodzenieOchrona = 0;

        for(unsigned j=i+1; j<MalaPlyta::maleplyty.size(); ++j) {
            if(MalaPlyta::maleplyty[j].czyWycinanie == false)
                continue;

            taPlytaZachodzenie += MalaPlyta::poleZachodzenia(MalaPlyta::maleplyty[i], MalaPlyta::maleplyty[j]);
            //taPlytaZachodzenieOchrona += MalaPlyta::poleZachodzeniaOchrona(MalaPlyta::maleplyty[i], MalaPlyta::maleplyty[j]);

            ///KARANIE INDYWIDUALNE

            //double pow_i=MalaPlyta::wymiary[i][0]*MalaPlyta::wymiary[i][1];
            //double pow_j=MalaPlyta::wymiary[j][0]*MalaPlyta::wymiary[j][1];
            if(0.0 != taPlytaZachodzenie) {
                if(GARandomInt(0,1)) {
                    genome.gene(4*i+3,0.3);

                    wczytajMalePlytyZGenomu(genome);
                    wszystkiePlytyWystawanie -= taPlytaWystawanie;
                    wszystkiePlytyPowierzchnia -= taPlytaPowierzchnia;
                    wszystkiePlytyOdleglosc -=  MalaPlyta::maleplyty[i].x*MalaPlyta::maleplyty[i].x;
                    wszystkiePlytyOdleglosc -=  MalaPlyta::maleplyty[i].y*MalaPlyta::maleplyty[i].y;
                    wszystkiePlytyZachozenie -= taPlytaZachodzenie;

                    /*wszystkiePlytyWystawanieOchrona -= taPlytaWystawanieOchrona;
                    wszystkiePlytyPowierzchniaOchrona -= taPlytaPowierzchniaOchrona;
                    wszystkiePlytyZachozenieOchrona -= taPlytaZachodzenieOchrona;*/
                    continue;
                }
                else {
                    genome.gene(4*i+3,0.3);
                    //MalaPlyta::gruboscOchrony+=1;
                    //cout << "grubosc" << MalaPlyta::gruboscOchrony << endl;
                    wczytajMalePlytyZGenomu(genome);
                    wszystkiePlytyWystawanie -= MalaPlyta::maleplyty[j].poleWystawania(duzaPlyta);
                    wszystkiePlytyPowierzchnia -=  MalaPlyta::wymiary[i][0] * MalaPlyta::wymiary[i][1];
                    wszystkiePlytyOdleglosc -=  MalaPlyta::maleplyty[j].x*MalaPlyta::maleplyty[i].x;
                    wszystkiePlytyOdleglosc -=  MalaPlyta::maleplyty[j].y*MalaPlyta::maleplyty[i].y;
                    wszystkiePlytyZachozenie -= taPlytaZachodzenie;


                    /*wszystkiePlytyWystawanieOchrona -= MalaPlyta::maleplyty[j].poleWystawaniaOchrona(duzaPlyta);
                    wszystkiePlytyPowierzchniaOchrona -= ((MalaPlyta::wymiary[j][0]+2*MalaPlyta::gruboscOchrony) *
                                                          (MalaPlyta::wymiary[j][1]+2*MalaPlyta::gruboscOchrony));
                    wszystkiePlytyZachozenieOchrona -= taPlytaZachodzenieOchrona;*/
                }
            }
        }

        wszystkiePlytyZachozenie += taPlytaZachodzenie;
        //wszystkiePlytyZachozenieOchrona += taPlytaZachodzenieOchrona;


        /// KARANIE INDYWIDUALNE

        if(0.0 != taPlytaWystawanie ) {
            genome.gene(4*i+3,0.3);
            wczytajMalePlytyZGenomu(genome);

            wszystkiePlytyWystawanie -= taPlytaWystawanie;
            wszystkiePlytyPowierzchnia -= taPlytaPowierzchnia;
            wszystkiePlytyOdleglosc -=  MalaPlyta::maleplyty[i].x*MalaPlyta::maleplyty[i].x;
            wszystkiePlytyOdleglosc -=  MalaPlyta::maleplyty[i].y*MalaPlyta::maleplyty[i].y;
            wszystkiePlytyZachozenie -= taPlytaZachodzenie;

            /*wszystkiePlytyWystawanieOchrona -= taPlytaWystawanieOchrona;
            wszystkiePlytyPowierzchniaOchrona -= taPlytaPowierzchniaOchrona;
            wszystkiePlytyZachozenieOchrona -= taPlytaZachodzenieOchrona;*/
        }

    }

    ++licznik;

        double zwr= wszystkiePlytyPowierzchnia/* - 0.001*wszystkiePlytyOdleglosc*wszystkiePlytyOdleglosc/n/n*/;

    if(!licznik%1000)
        cout << zwr  << "\t" << MalaPlyta::gruboscOchrony << endl;

    return zwr >= 0.0 ? zwr : 0.0;

}

float comparator(const GAGenome& g1, const GAGenome& g2) {
  GARealGenome& a = (GARealGenome &)g1;
  GARealGenome& b = (GARealGenome &)g2;

  float odleglosc = 0;
    for(unsigned i=0; i<MalaPlyta::maleplyty.size(); ++i) {
        /*if (round(a.gene(4*i+3)/dlogosc_genu_binarnego) != round(b.gene(4*i+3)/dlogosc_genu_binarnego))
            return 1;
        if(a.gene(4*i+3) == false&&b.gene(4*i+3) == false)
            continue;*/
        odleglosc += (pow(a.gene(4*i)-b.gene(4*i),2)+ pow(a.gene(4*i)-b.gene(4*i),2)+0.01);
    }
    float miara = 1 - odleglosc/1000;
    return odleglosc>0 ? miara : 0.0;
}


