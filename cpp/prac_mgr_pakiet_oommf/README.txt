W tym folderze zamieœci³em now¹ opracowywan¹ przeze mnie w ramach pracy magisterskiej wersjê rozszerzenia "MF_CurrentFlowEvolver" do œrodowiska obliczeniowego OOMMF 
(obiect oriented micro magnetic framework). 
Rozszerzenie to by³o czêœci¹ pracy doktorskiej mojego opiekuna dr Marka Frankowskiego. 
Tematem mojej pracy magisterskiej jest:
"Model mikromagnetyczny pracy magnetycznego z³¹cza tunelowego z wielokrotn¹ barier¹ izolatora". Temat zwi¹zany jest z magnetycznymi pamiêciami RAM, tzw. MRAM, lub STT-MRAM.
Pierwotna wersja rozszerzenia umo¿liwia³a symulowanie jednokrotnej warstwy izolatora w uk³adzie, jednak wspó³czesne pamiêci MRAM,
wymagaj¹ zastosowania dwóch barier izolatora - st¹d potrzeba uogólnienia. Oryginalna wersja rozszerzenia dostêpna jest na stronie: 
http://layer.uci.agh.edu.pl/M.Frankowski/download.html
Edytowane przeze mnie fragmenty kodu to g³ównie:
dodanie klas:
MagneticAtlas, Boundary, LinksBetweenTwoInterfaces,
edycja lub dodanie metod:
CheckBoundaryParameters, RegisterAtlas, getMagneticAtlasByRegion, getBoundaryByName, RegisterBoundary, LinkTwoSurfaces, FillLinkList 
(nie zachowano konwencji mówi¹cej o nazywaniu metod zaczynaj¹c od ma³ej litery - zrobiono tak by nie zmieniaæ istniej¹cego porz¹dku nazewniczego).