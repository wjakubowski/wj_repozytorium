W tym folderze zamie�ci�em now� opracowywan� przeze mnie w ramach pracy magisterskiej wersj� rozszerzenia "MF_CurrentFlowEvolver" do �rodowiska obliczeniowego OOMMF 
(obiect oriented micro magnetic framework). 
Rozszerzenie to by�o cz�ci� pracy doktorskiej mojego opiekuna dr Marka Frankowskiego. 
Tematem mojej pracy magisterskiej jest:
"Model mikromagnetyczny pracy magnetycznego z��cza tunelowego z wielokrotn� barier� izolatora". Temat zwi�zany jest z magnetycznymi pami�ciami RAM, tzw. MRAM, lub STT-MRAM.
Pierwotna wersja rozszerzenia umo�liwia�a symulowanie jednokrotnej warstwy izolatora w uk�adzie, jednak wsp�czesne pami�ci MRAM,
wymagaj� zastosowania dw�ch barier izolatora - st�d potrzeba uog�lnienia. Oryginalna wersja rozszerzenia dost�pna jest na stronie: 
http://layer.uci.agh.edu.pl/M.Frankowski/download.html
Edytowane przeze mnie fragmenty kodu to g��wnie:
dodanie klas:
MagneticAtlas, Boundary, LinksBetweenTwoInterfaces,
edycja lub dodanie metod:
CheckBoundaryParameters, RegisterAtlas, getMagneticAtlasByRegion, getBoundaryByName, RegisterBoundary, LinkTwoSurfaces, FillLinkList 
(nie zachowano konwencji m�wi�cej o nazywaniu metod zaczynaj�c od ma�ej litery - zrobiono tak by nie zmienia� istniej�cego porz�dku nazewniczego).