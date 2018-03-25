	Przedstawione tu programy stanowi¹ tylko ilustracjê u¿ytecznych skryptów napasanych w celu usprawnienia codziennej pracy.
	Dwa pliki importAndPlotData.py oraz runManager.py stanowi¹ prosty system obs³ugi zadañ na klastrze obliczeniowym, dostosowany 
do potrzeb u¿ytkownika wykorzystuj¹cego pakiet obliczeniowy OOMMF.
	Program runManager odpowiada za wywo³anie symulacji (uruchamiany z poziomu klastra obliczeniowego) z parametrami zadanymi 
	na odpowiedniej siatce (zadawane s¹ wartoœci min i max oraz krok, mo¿na zadawaæ wiêcej ni¿ jeden parametr do modyfikowania -
- wtedy siatka parametrów ma wiêcej ni¿ jeden wymiar).
	Program "importAndPlotData" pobiera dane wynikowe i rysuje odpowiednie wykresy (uruchamiany na komputerze osobistym).
	Zamieszczono równie¿ przyk³adowe pliki "skrypt.txt" - szkielet dla generowanych plików "skrypt.sh" do wywo³ania symulacji oraz 
przyk³adowy plik ".mif" konfiguracyjnego pliku symulacji o bardzo nie porêcznym i przestarza³ym formatowaniu 
(wynik d³ugowiecznoœci pakietu OOMMF napisanego pod koniec lat 90-tych).