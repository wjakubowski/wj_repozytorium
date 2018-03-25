Rozszerzenie œrodowiska Geant4 (popularnego narzêdzia dla fizyków j¹drowych i cz¹stek elementarnych) do symulowania detektora czasu przelotu wykorzystuj¹cego 
promieniowanie Czerenkowa. Œrodowisko Geant4 to oprogramowanie napisane w jêzyku C++, do symulowania wielu zjawisk zachodz¹cych w œwiecie cz¹stek elementarnych, 
przy pomocy statystycznych metod Monte Carlo. Napisany przezemnie program rozszerza klasy œrodowiska Geant4 tak by dostosowaæ ich dzia³anie do potrzeb projktu.
Mój program wykonuje kilka czynnoœci:
-definiuje geometriê detektora, w³aœciwoœci materia³ów z których jest zrobiony, pliki src/MyPrimaryGeneratorAction,
-okreœla lec¹ce cz¹stki ich po³o¿enia i prêdkoœci, pliki src/MyPhotonSD, src/MyPrimaryGeneratorAction,
-definiuje jakie procesy fizyczne bêd¹ symulowane, pliki src/MyPhysicsList
-opisuje zliczane statystyki, powierzchnie czu³e detektora
-w pliku g³ównym My.cc, tworzony i uruchomiany jest obiekt klasy G4RunManager odpowiedzialny za sterowanie kolejnymi czynnoœciami wykonywanymi przez program, 
w tym graficzn¹ reprezentacj¹ przeprowadzanych symulacji.
Dodoatkowo w projecke znajduje siê plik histogramy.cpp, zadaniem którego jest opracowywanie wykresów histogramów na podstawie danych z symulacji.

Linki:
#strona przedmiotu "Œrodowisko programistyczne Geant4", opis wykonywanego projektu:
http://home.agh.edu.pl/~leszekad/dydaktyka/wfiis_geant4_2015/geant4_projekt_7.pdf
#strona projektu Geant4:
http://geant4.web.cern.ch/geant4/

Uwaga: ze wzglêdu na koniecznoœæ instalacji œrodowiska Geant4 program jest trudny do uruchomienia.

