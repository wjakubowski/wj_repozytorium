	Przedstawione tu programy stanowi� tylko ilustracj� u�ytecznych skrypt�w napasanych w celu usprawnienia codziennej pracy.
	Dwa pliki importAndPlotData.py oraz runManager.py stanowi� prosty system obs�ugi zada� na klastrze obliczeniowym, dostosowany 
do potrzeb u�ytkownika wykorzystuj�cego pakiet obliczeniowy OOMMF.
	Program runManager odpowiada za wywo�anie symulacji (uruchamiany z poziomu klastra obliczeniowego) z parametrami zadanymi 
	na odpowiedniej siatce (zadawane s� warto�ci min i max oraz krok, mo�na zadawa� wi�cej ni� jeden parametr do modyfikowania -
- wtedy siatka parametr�w ma wi�cej ni� jeden wymiar).
	Program "importAndPlotData" pobiera dane wynikowe i rysuje odpowiednie wykresy (uruchamiany na komputerze osobistym).
	Zamieszczono r�wnie� przyk�adowe pliki "skrypt.txt" - szkielet dla generowanych plik�w "skrypt.sh" do wywo�ania symulacji oraz 
przyk�adowy plik ".mif" konfiguracyjnego pliku symulacji o bardzo nie por�cznym i przestarza�ym formatowaniu 
(wynik d�ugowieczno�ci pakietu OOMMF napisanego pod koniec lat 90-tych).