Program rozwi¹zuje problem optymalizacji wycinania wybranych rozmiarów prostok¹tów z du¿ego prostok¹ta tak by pozosta³a nie wyciêta czêœæ by³a jak najmniejsza.
Stosowana metodologia dzia³ania programu opiera siê o filozofiê algorytmów genetycznych i korzysta z dedykowanej biblioteki "GALIB". Algorytm genetyczny obejmuje 
"hodowle" pewnej populacji rozwi¹zañ i promowanie, rozmna¿anie najlepszych rozwi¹zañ w populacji. Poza tym rozwi¹zania s¹ lekko modyfikowane ("mutowane") w kolejnych iteracjach,
a tak¿e mieszane ze sob¹ - co przypomina zachowanie populacji ¿ywych organizmów - st¹d nazwa algorytmy genetyczne. W celu zachowania poprawnoœci rozwi¹zañ
b³êdne rozwi¹zania: zachodzenie na siebie prostok¹tów, wystawanie poza obrêb prostok¹ta bazowego (z którego wycinane s¹ pozosta³e), musz¹ byæ karane, to znaczy usuwane z populacji.
	Do projektu do³¹czony jest plik plyty.cpp, który odpowiada za logikê zwi¹zan¹ z rozmieszczeniem prostok¹tów. Za rysowanie rozmieszczenia wycinanych prostok¹tów odpowiada 
skrypt napisany w jêzyku Python. Oprócz stosowanego pliku maleplyty.txt dodano zestaw innych zbiorów prostok¹tów w folderze "maleplyty".

Projekt jest gotowy do uruchomienia w CodeBlocks-ie

Szerszy opis zadania dostêpny jest pod linkiem:
https://drive.google.com/open?id=1ZW7dOumfXd1Q1cw_pM4k7SGf06GVWK3X