Program rozwi�zuje problem optymalizacji wycinania wybranych rozmiar�w prostok�t�w z du�ego prostok�ta tak by pozosta�a nie wyci�ta cz�� by�a jak najmniejsza.
Stosowana metodologia dzia�ania programu opiera si� o filozofi� algorytm�w genetycznych i korzysta z dedykowanej biblioteki "GALIB". Algorytm genetyczny obejmuje 
"hodowle" pewnej populacji rozwi�za� i promowanie, rozmna�anie najlepszych rozwi�za� w populacji. Poza tym rozwi�zania s� lekko modyfikowane ("mutowane") w kolejnych iteracjach,
a tak�e mieszane ze sob� - co przypomina zachowanie populacji �ywych organizm�w - st�d nazwa algorytmy genetyczne. W celu zachowania poprawno�ci rozwi�za�
b��dne rozwi�zania: zachodzenie na siebie prostok�t�w, wystawanie poza obr�b prostok�ta bazowego (z kt�rego wycinane s� pozosta�e), musz� by� karane, to znaczy usuwane z populacji.
	Do projektu do��czony jest plik plyty.cpp, kt�ry odpowiada za logik� zwi�zan� z rozmieszczeniem prostok�t�w. Za rysowanie rozmieszczenia wycinanych prostok�t�w odpowiada 
skrypt napisany w j�zyku Python. Opr�cz stosowanego pliku maleplyty.txt dodano zestaw innych zbior�w prostok�t�w w folderze "maleplyty".

Projekt jest gotowy do uruchomienia w CodeBlocks-ie

Szerszy opis zadania dost�pny jest pod linkiem:
https://drive.google.com/open?id=1ZW7dOumfXd1Q1cw_pM4k7SGf06GVWK3X