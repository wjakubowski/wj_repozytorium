Rozszerzenie �rodowiska Geant4 (popularnego narz�dzia dla fizyk�w j�drowych i cz�stek elementarnych) do symulowania detektora czasu przelotu wykorzystuj�cego 
promieniowanie Czerenkowa. �rodowisko Geant4 to oprogramowanie napisane w j�zyku C++, do symulowania wielu zjawisk zachodz�cych w �wiecie cz�stek elementarnych, 
przy pomocy statystycznych metod Monte Carlo. Napisany przezemnie program rozszerza klasy �rodowiska Geant4 tak by dostosowa� ich dzia�anie do potrzeb projktu.
M�j program wykonuje kilka czynno�ci:
-definiuje geometri� detektora, w�a�ciwo�ci materia��w z kt�rych jest zrobiony, pliki src/MyPrimaryGeneratorAction,
-okre�la lec�ce cz�stki ich po�o�enia i pr�dko�ci, pliki src/MyPhotonSD, src/MyPrimaryGeneratorAction,
-definiuje jakie procesy fizyczne b�d� symulowane, pliki src/MyPhysicsList
-opisuje zliczane statystyki, powierzchnie czu�e detektora
-w pliku g��wnym My.cc, tworzony i uruchomiany jest obiekt klasy G4RunManager odpowiedzialny za sterowanie kolejnymi czynno�ciami wykonywanymi przez program, 
w tym graficzn� reprezentacj� przeprowadzanych symulacji.
Dodoatkowo w projecke znajduje si� plik histogramy.cpp, zadaniem kt�rego jest opracowywanie wykres�w histogram�w na podstawie danych z symulacji.

Linki:
#strona przedmiotu "�rodowisko programistyczne Geant4", opis wykonywanego projektu:
http://home.agh.edu.pl/~leszekad/dydaktyka/wfiis_geant4_2015/geant4_projekt_7.pdf
#strona projektu Geant4:
http://geant4.web.cern.ch/geant4/

Uwaga: ze wzgl�du na konieczno�� instalacji �rodowiska Geant4 program jest trudny do uruchomienia.

