#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include "plyty.h"
using namespace std;


int main(int argc, char *argv[])
{
	MalaPlyta::zaladujWymiary();
	DuzaPlyta duzaPlyta;
	MalaPlyta p1(10,0,0,0),p2(0,0,0,1);
	cout << "zachodzenie 1,2: " << MalaPlyta::poleZachodzenia(p1,p2) << endl;
}


