#include <iostream>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>
#include <utility>
#include <ctime>
#include <stdio.h>
#include <functional>
#include <stdlib.h>

using namespace std;

class Pole
{
public:
//pola
	int nx;
	int ny;
	double minx;
	double maxx;
	double miny;
	double maxy;
	double dx;
	double dy;
	double N;//ilosc zliczen w polu
//metody
	Pole(){}
	Pole(int nx,int ny,double minx,double maxx,double miny,double maxy,double dx,double dy);


};

Pole::Pole(int nx,int ny,double minx,double maxx,double miny,double maxy,double dx,double dy)
:nx(nx),ny(ny),minx(minx),maxx(maxx),miny(miny),maxy(maxy),dx(dx),dy(dy),N(0){}

class Histogram
{
public:
//pola
	int lx;
	int ly;
	double minx;
	double maxx;
	double miny;
	double maxy;
	double dx;
	double dy;
	vector<vector<Pole> > dane;
	int N_trafione;//liczebność danych które trafiły w histogram
	int N_ogolnie;//liczebność wszystkich danych

	


//metody	
	Histogram(int,int,double,double,double,double);
	void dodaj_dana(double,double);
	//friend ostream& operator<<(ostream& , Histogram& );

};

Histogram::Histogram(int lx,int ly,double minx,double maxx,double miny,double maxy):
lx(lx),
ly(ly),
minx((minx<=maxx)?minx:maxx),
maxx((minx<=maxx)?maxx:minx),
miny((miny<=maxy)?miny:maxy),
maxy((miny<=maxy)?maxy:miny),
dx((maxx-minx)/lx),
dy((maxy-miny)/ly),
N_trafione(0),
N_ogolnie(0)

{
	dane.resize(lx);
	for(int i=0;i<lx;++i)
	{
		dane[i].resize(ly);
		for(int j=0;j<ly;++j)
		{
			dane[i][j]=Pole(lx,ly,minx+dx*i,minx+dx*(i+1),miny+dy*j,miny+dy*(j+1),dx,dy);
		}
	}
}





void Histogram::dodaj_dana(double wx,double wy)
{
	++N_ogolnie;

	if(wx>=minx&&wx<=maxx&&wy>=miny&&wy<=maxy)
	{
		++N_trafione;
		/*cout<<wx<<" "<<wy<<endl;
		cout<<dane[0][0].minx<<endl;
		cout<<"xxxx"<<bool(wx>=dane[0][0].minx)<<endl;
		cout<<"xxxx"<<endl;*/
		int i=0,j=0;
		while(!(wx >= dane[i][j].minx && wx <= dane[i][j].maxx))
			++i;
		while(!(wy >= dane[i][j].miny && wy <= dane[i][j].maxy))
			++j;
		++dane[i][j].N;
		//cout<<(dane[i][j].N)<<'\t'<<i<<'\t'<<j<<endl;  
	}


}




//E[MeV]->lam[nm]
double LambdaOdE(double e)
{
	return (12.43125/e*pow(10,-4));
}

double wydajnosc(double l)//wydajnoc od lambda
	//{return 3*pow(10,-14)*pow(l,6)-pow(10,-10)*pow(l,5)+2*pow(10,-6)*pow(l,4)+0.066*pow(l,2)-13.56*l+1067;}
{
	if(l<250)return 0;
	else if(l<280)return 0.04;
	else if(l<300)return 0.08;
	else if(l<330)return 0.17;
	else if(l<350)return 0.2;
	else if(l<380)return 0.3;
	else if(l<400)return 0.35;
	else if(l<450)return 0.4;
	else if(l<550)return 0.3;
	else if(l<650)return 0.27;
	else if(l<700)return 0.12;
	else if(l<750)return 0.4;
	else return 0;


}



int main (int argc,char ** argv)
{
	srand(time(NULL));
	double skalax=1.;
	double skalay=1.;

	string nazwa_pliku_czyt = argv[3];	

	double minx=pow(10,100),maxx=-pow(10,100),miny=pow(10,100),maxy=-pow(10,100);
	fstream plik;	
	double t,e,l;
	plik.open(nazwa_pliku_czyt.c_str());
	//cout<<plik.good()<<endl;
	while(plik.good())
	{
		plik>>t;plik>>e;
		l=LambdaOdE(e);
		t*=skalax;
		l*=skalay;
		
		if(t<minx)minx=t;
		if(t>maxx)maxx=t;
		if(l<miny)miny=l;
		if(l>maxy)maxy=l;

	}
	plik.close();
	cout<<"inicjalizacja histogramu wartościami od do: "<<minx<<" - "<<maxx<<" , "<<miny<<" - "<<maxy<<endl;
	Histogram hist(atof(argv[1]),atof(argv[2]),minx,maxx,miny,maxy);
	plik.open(nazwa_pliku_czyt.c_str());
	

	//cout<<plik.good()<<endl;
	while(plik.good())
	{
		plik>>t;plik>>e;
		l=LambdaOdE(e);
		cout<<t<<" "<<l<<endl;
		t*=skalax;
		l*=skalay;		
		hist.dodaj_dana(t,l);

	}
	
	plik.close();		
	//cout<<hist.dane[8][8].N<<endl;

	/*minx*=skalax;
	maxx*=skalax;
	miny*=skalay;
	maxy*=skalay;
	hist.minx*=skalax;
	hist.maxx*=skalax;
	hist.miny*=skalay;
	hist.maxy*=skalay;
	hist.dx*=skalax;
	hist.dy*=skalay;*/
	//cout<<minx<<" "<<maxx<<" "<<miny<<" "<<maxy<<endl;
/*
	for(int i=0;i<hist.lx;++i)
	{
		for(int j=0;j<hist.ly;++j)
		{
			hist.dane[i][j].minx*=skalax;
			hist.dane[i][j].maxx*=skalax;
			hist.dane[i][j].miny*=skalay;
			hist.dane[i][j].maxy*=skalay;

		}
	
	}
*/

	ofstream oplik;
	oplik.open("hist2D.txt",ofstream::trunc);
	for(int i=0;i<hist.lx;++i)
	{
		for(int j=0;j<hist.ly;++j)
		{
			oplik<<hist.dane[i][j].minx<<'\t'<<hist.dane[i][j].miny<<'\t'<<hist.dane[i][j].N<<endl;
		}
		oplik<<endl;
	}
	
	oplik.close();

	oplik.open("hist2Dr.txt",ofstream::trunc);
	for(int i=0;i<hist.lx;++i)
	{
		for(int j=0;j<hist.ly;++j)
		{
			oplik<<hist.dane[i][j].minx<<'\t'<<hist.dane[i][j].miny<<'\t'<<
			hist.dane[i][j].N*wydajnosc(hist.dane[i][j].miny)<<endl;
		}
		oplik<<endl;
	}
	
	oplik.close();
	// x i czas t
	// y j lambda l
	double suma=0;
	oplik.open("histl.txt",ofstream::trunc);
	for(int j=0;j<hist.ly;++j)
	{
		for(int i=0;i<hist.lx;++i)
		{
			suma+=hist.dane[i][j].N;
		}
		oplik<<hist.dane[0][j].miny<<'\t'<<suma<<endl;
		suma=0;
	}
	
	oplik.close();

	suma=0;
	oplik.open("histlr.txt",ofstream::trunc);
	for(int j=0;j<hist.ly;++j)
	{
		for(int i=0;i<hist.lx;++i)
		{
			suma+=hist.dane[i][j].N*wydajnosc(hist.dane[i][j].miny);
		}
		oplik<<hist.dane[0][j].miny<<'\t'<<suma<<endl;
		suma=0;
	}
	
	oplik.close();

	suma=0;
	oplik.open("histt.txt",ofstream::trunc);
	for(int i=0;i<hist.lx;++i)
	{
		for(int j=0;j<hist.ly;++j)
		{
			suma+=hist.dane[i][j].N;
		}
		oplik<<hist.dane[i][0].minx<<'\t'<<suma<<endl;
		suma=0;
	}
	
	oplik.close();

	suma=0;
	oplik.open("histtr.txt",ofstream::trunc);
	for(int i=0;i<hist.lx;++i)
	{
		for(int j=0;j<hist.ly;++j)
		{
			suma+=hist.dane[i][j].N*wydajnosc(hist.dane[i][j].miny);
		}
		oplik<<hist.dane[i][0].minx<<'\t'<<suma<<endl;
		suma=0;
	}
	
	oplik.close();



	/*string nazwa2D,nazwa2Dr,nazwaL,nazwaLr,nazwat,nazwatr; 
	nazwa2D=nazwa2Dr=nazwaL=nazwaLr=nazwat=nazwatr = argv[1];
	nazwa2D+="2D.png";
	nazwa2Dr+="2Dr.png";
	nazwaL+="L.png";
	nazwaLr+="Lr.png";
	nazwat+="t.png";
	nazwatr+="tr.png";

	system("gnuplot ./rysuj.plt "+=nazwa2D);*/
}














