#include "MyDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4String.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSSphereSurfaceCurrent.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4SubtractionSolid.hh"
#include "MyPhotonSD.hh"



MyDetectorConstruction::MyDetectorConstruction()
	  :fpWorldLogical(0)
	  ,fpWorldPhysical(0)
	  //wymiary preta i katody
	  ,szerokosc_pret(0.006*m)
	  ,dlugosc_pret(0.006*m)
	  ,wysokosc_pret_Si(0.012*m)
	  ,wysokosc_pret_powietrze(0.078*m)
	  ,grubosc_folia_Al(0.05*mm)
	  ,szerokosc_katoda(0.006*m)
	  ,dlugosc_katoda(0.006*m)
	  ,wysokosc_katoda(0.001*m)
	  ,fi(48.3*deg)
	  //,fi(0.0*deg)
{}

MyDetectorConstruction::~MyDetectorConstruction() {}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	  // Material Definition
	  DefineMaterials();  

	  // Geometry Definition
	  SetupGeometry();   

	  // Return world volume
	  return fpWorldPhysical;  
}

void MyDetectorConstruction::DefineMaterials()
{

}

void MyDetectorConstruction::SetupGeometry()
{


	  G4NistManager* man = G4NistManager::Instance();
	  G4Material* air = man->FindOrBuildMaterial("G4_AIR");
	  G4Material* krzemionka = man->FindOrBuildMaterial("G4_SILICON_DIOXIDE"); 
	  G4Material* Al= man->FindOrBuildMaterial("G4_Al");
	
	//optyczne właściwości materiałów
	const G4int NUMENTRIES = 6;

	G4double ppckov[NUMENTRIES] = 
	{
				4.14375*eV,
				3.1078125*eV,
				2.48625*eV,
				2.071875*eV,
				1.77589285714286*eV,
				1.55390625*eV,
	};



	G4double rindexx[NUMENTRIES] = {1.49,1.47,1.464,1.458,1.456,1.455};
	G4double rindexxAIR[NUMENTRIES] = {1, 1, 1, 1, 1, 1};
	G4double rindexxAL[NUMENTRIES] = {1.5, 1.5, 1.5, 1.5,  1.5, 1.5};
	G4double absorption[NUMENTRIES] = {2000*cm,2000*cm,2000*cm,2000*cm,2000*cm,2000*cm};

	G4MaterialPropertiesTable *MPTSi = new G4MaterialPropertiesTable();
	MPTSi -> AddProperty("RINDEX",ppckov,rindexx,NUMENTRIES);
	MPTSi -> AddProperty("ABSLENGTH",ppckov,absorption,NUMENTRIES);

	G4MaterialPropertiesTable *MPTAIR = new G4MaterialPropertiesTable();
	MPTAIR -> AddProperty("RINDEX",ppckov,rindexxAIR,NUMENTRIES);
	MPTAIR -> AddProperty("ABSLENGTH",ppckov,absorption,NUMENTRIES);

	G4MaterialPropertiesTable *MPTAL = new G4MaterialPropertiesTable();
	MPTAL -> AddProperty("RINDEX",ppckov,rindexxAL,NUMENTRIES);
	MPTAL -> AddProperty("ABSLENGTH",ppckov,absorption,NUMENTRIES);

	krzemionka -> SetMaterialPropertiesTable(MPTSi);
	air -> SetMaterialPropertiesTable(MPTAIR);
	Al -> SetMaterialPropertiesTable(MPTAL);





   

	  // World volume
	  G4Box* worldSolid = new G4Box("World_Solid",2.0*m, 2.0*m, 2.0*m);    
	  
	  fpWorldLogical = new G4LogicalVolume(worldSolid,air,"World_Logical"); 
	  
	  fpWorldPhysical = new G4PVPlacement(0,G4ThreeVector(), fpWorldLogical,"World_Physical",0,false,0);		 


	//moj detektor
	//wymiary

	//G4Box* pret= new G4Box("Pret",szerokosc_pret/2,dlugosc_pret/2,wysokosc_pret/2);

	//pręt powietrze

	G4Box* pret_powietrze= new G4Box("Pret powietrze",wysokosc_pret_powietrze/2,dlugosc_pret/2,szerokosc_pret/2);
	G4Box* pret_powietrze_srodek= new G4Box 
	("Pret powietrze",wysokosc_pret_powietrze/2,dlugosc_pret/2-grubosc_folia_Al,szerokosc_pret/2-grubosc_folia_Al);
	G4SubtractionSolid * fpRoznicaBryl = new G4SubtractionSolid ("Folia Al",pret_powietrze,pret_powietrze_srodek);
	G4LogicalVolume* fpFoliaAl = new G4LogicalVolume(fpRoznicaBryl,Al,"Folia Al");
	G4LogicalVolume* fppret_powietrze_srodek = new G4LogicalVolume(pret_powietrze_srodek,air,"Pret powietrze srodek");	



	//pręt reszta
	G4Box* pret_Si= new G4Box("Pret Si",wysokosc_pret_Si/2,dlugosc_pret/2,szerokosc_pret/2);
	G4Box* katoda= new G4Box("Fotokatoda",wysokosc_katoda/2,dlugosc_katoda/2,szerokosc_katoda/2);
	G4LogicalVolume* fppret_Si = new G4LogicalVolume(pret_Si,krzemionka,"Pret Si");
	G4LogicalVolume* fpkatoda = new G4LogicalVolume(katoda,air,"Fotokatoda");
	

	//fotokatoda przypisanie jako detektora
	SetupScoring(fpkatoda); 
	

	//translacja
	G4ThreeVector translacja_Si = G4ThreeVector(0,0,0);
	G4ThreeVector translacja_powietrze = G4ThreeVector(wysokosc_pret_powietrze/2+wysokosc_pret_Si/2,0,0);
	G4ThreeVector translacja_katoda = G4ThreeVector(wysokosc_katoda/2+wysokosc_pret_powietrze+wysokosc_pret_Si/2,0,0);



	// obrot
	
	G4RotationMatrix * rotacja = new G4RotationMatrix(0,0,0);
	rotacja->rotateY(-fi);

	// obrot wektorow translacji

	// os obrotu
	G4ThreeVector * osY =new G4ThreeVector(0,1,0);

	translacja_powietrze.rotate(fi,*osY);
	translacja_katoda.rotate(fi,*osY);

	//obiekty fizyczne pręt powietrze
	G4VPhysicalVolume * fpPhysicalPowietrze = new G4PVPlacement(rotacja,translacja_powietrze,fppret_powietrze_srodek, 
	"pret powietrze",fpWorldLogical,false,0,true);
	G4VPhysicalVolume * fpPhysicalFoliaAl = new G4PVPlacement
	(rotacja,translacja_powietrze,fpFoliaAl,"FoliaAl",fpWorldLogical,false,0,true);


	//obiekty fizyczne reszta
	

	G4VPhysicalVolume * fpPhysicalSi = new G4PVPlacement(rotacja,translacja_Si,fppret_Si,"pret Si",fpWorldLogical,false,0,true);
	/*G4VPhysicalVolume * fpPhysicalKatoda = */
	new G4PVPlacement (rotacja,translacja_katoda,fpkatoda,"fotokatoda",fpWorldLogical,false,0,true);

	//płaszczyzny granicy ośrodków optycznych
		
	G4OpticalSurface* GranicaSiPowietrze = new G4OpticalSurface("GranicaSiPowietrze");
	G4OpticalSurface* GranicaPowietrzeSi = new G4OpticalSurface("GranicaPowietrzeSi");
	G4OpticalSurface* GranicaPowietrzeAl = new G4OpticalSurface("GranicaPowietrzeAl");

	G4double sigma_alpha = 0.1;

	GranicaSiPowietrze -> SetType(dielectric_dielectric);
	GranicaSiPowietrze -> SetModel(glisur);
	GranicaSiPowietrze -> SetFinish(polished);
	GranicaSiPowietrze -> SetSigmaAlpha(sigma_alpha);

	GranicaPowietrzeSi -> SetType(dielectric_dielectric);
	GranicaPowietrzeSi -> SetModel(glisur);
	GranicaPowietrzeSi -> SetFinish(polished);
	GranicaPowietrzeSi -> SetSigmaAlpha(sigma_alpha);

	GranicaPowietrzeAl -> SetType(dielectric_metal);
	GranicaPowietrzeAl -> SetModel(glisur);
	GranicaPowietrzeAl -> SetFinish(polished);
	GranicaPowietrzeAl -> SetSigmaAlpha(sigma_alpha);



	const G4int NUM = 6;

	G4double pp[NUM] = 
	{
				4.14375*eV,
				3.1078125*eV,
				2.48625*eV,
				2.071875*eV,
				1.77589285714286*eV,
				1.55390625*eV,
	};
	
	G4double rindexSiPow[NUM] = {1.49,1.47,1.464,1.458,1.456,1.455};
	G4double reflectivitySiPow[NUM] = {0.5, 0.5, 0.5, 0.5,0.5, 0.5};
	G4double efficiencySiPow[NUM] = {0.5, 0.5, 0.5, 0.5,0.5, 0.5};


	G4double rindexPowSi[NUM] = {0.5, 0.5, 0.5, 0.5,0.5, 0.5};
	G4double reflectivityPowSi[NUM] = {0.5, 0.5, 0.5, 0.5,0.5, 0.5};
	G4double efficiencyPowSi[NUM] = {0.5, 0.5, 0.5, 0.5,0.5, 0.5};

	G4double rindexPowAl[NUM] = {0.5, 0.5, 0.5, 0.5,0.5, 0.5};
	G4double reflectivityPowAl[NUM] = {1, 1, 1, 1, 1, 1};
	G4double efficiencyPowAl[NUM] = {1, 1, 1, 1, 1, 1};
	

	G4MaterialPropertiesTable* SMPTSiPow = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable* SMPTPowSi = new G4MaterialPropertiesTable();
	G4MaterialPropertiesTable* SMPTPowAl = new G4MaterialPropertiesTable();

	SMPTSiPow -> AddProperty("RINDEX",pp,rindexSiPow,NUM);
	SMPTSiPow -> AddProperty("REFLECTIVITY",pp,reflectivitySiPow,NUM);
	SMPTSiPow -> AddProperty("EFFICIENCY",pp,efficiencySiPow,NUM);
	
	SMPTPowSi -> AddProperty("RINDEX",pp,rindexPowSi,NUM);
	SMPTPowSi -> AddProperty("REFLECTIVITY",pp,reflectivityPowSi,NUM);
	SMPTPowSi -> AddProperty("EFFICIENCY",pp,efficiencyPowSi,NUM);

	SMPTPowAl -> AddProperty("RINDEX",pp,rindexPowAl,NUM);
	SMPTPowAl -> AddProperty("REFLECTIVITY",pp,reflectivityPowAl,NUM);
	SMPTPowAl -> AddProperty("EFFICIENCY",pp,efficiencyPowAl,NUM);

	
	GranicaSiPowietrze -> SetMaterialPropertiesTable(SMPTSiPow);
	GranicaSiPowietrze -> SetPolish(0.5);

	GranicaPowietrzeSi -> SetMaterialPropertiesTable(SMPTPowSi);
	GranicaPowietrzeSi -> SetPolish(0.5);
	
	GranicaPowietrzeAl -> SetMaterialPropertiesTable(SMPTPowAl);
	GranicaPowietrzeAl -> SetPolish(1);
	
	//fizyczne granice ostodkow optycznych
	new G4LogicalBorderSurface("name",fpPhysicalSi,fpPhysicalPowietrze,GranicaSiPowietrze);
	new G4LogicalBorderSurface("name",fpPhysicalPowietrze,fpPhysicalSi,GranicaPowietrzeSi);
	new G4LogicalBorderSurface("name",fpPhysicalPowietrze,fpPhysicalFoliaAl,GranicaPowietrzeAl);
	new G4LogicalBorderSurface("name",fpPhysicalFoliaAl,fpWorldPhysical,GranicaPowietrzeAl);
	new G4LogicalBorderSurface("name",fpPhysicalSi,fpWorldPhysical,GranicaSiPowietrze);

	//ATRYBUTY WIZUALIZACJI

	// Invisible world volume.
	  fpWorldLogical->SetVisAttributes(G4VisAttributes::Invisible);


	// pret Si atrybuty
	  G4VisAttributes* pret_atrybuty = new G4VisAttributes(G4Colour(1.0,0.0,0.0,1.0));
	  pret_atrybuty->SetForceSolid(true);
	  fppret_Si->SetVisAttributes(pret_atrybuty);


	// pret powietrze atrybuty
	  G4VisAttributes* pret_atrybuty_powietrze = new G4VisAttributes(G4Colour(0.0,0.0,1.0,1.0));
	  pret_atrybuty_powietrze->SetForceSolid(true);
	  fppret_powietrze_srodek->SetVisAttributes(pret_atrybuty_powietrze);


	// folia Al
	  
	  fpFoliaAl->SetVisAttributes(G4VisAttributes::Invisible);


	// fotokatoda atrybuty
	  G4VisAttributes* katoda_atrybuty = new G4VisAttributes(G4Colour(0.0,1.0,0.0,1.0));
	  katoda_atrybuty->SetForceSolid(true);
	  fpkatoda->SetVisAttributes(katoda_atrybuty);

  
}



void MyDetectorConstruction::SetupScoring(G4LogicalVolume* scoringVolume)
{

	
	G4SDManager* moj_manager = G4SDManager::GetSDMpointer();	
	MyPhotonSD* photonSD =  new MyPhotonSD("MyScorer");
	moj_manager->AddNewDetector(photonSD);
	scoringVolume->SetSensitiveDetector(photonSD);

	//###nie wiem czy 4 poniższe linijki są potrzebne?
	//G4MultiFunctionalDetector * moj_detektor=new G4MultiFunctionalDetector("MyDetector");
	//moj_manager->AddNewDetector(moj_detektor);
	//G4PSSphereSurfaceCurrent* scorer = new G4PSSphereSurfaceCurrent("MyScorer",fCurrent_In);
	//moj_detektor->RegisterPrimitive(scorer);




//###################tak ma być 
/*
  // Create a new sensitive detector named "MyDetector"
   G4MultiFunctionalDetector* detector =
    new G4MultiFunctionalDetector("MyDetector");

  // Get pointer to detector manager
  G4SDManager* manager = G4SDManager::GetSDMpointer();  

  // Register detector with manager
  manager->AddNewDetector(detector);

  // Attach detector to scoring volume
  //##############skomentowane
  //scoringVolume->SetSensitiveDetector(detector);
  //################dodane
  // Create a new sensitive detector named "Photon"
  MyPhotonSD* photonSD =  new MyPhotonSD("Photon");
  manager->AddNewDetector(photonSD);
  scoringVolume->SetSensitiveDetector(photonSD);
  //#############################
  // Create a primitive Scorer named MyScorer
  G4PSSphereSurfaceCurrent* scorer = 
    new G4PSSphereSurfaceCurrent("MyScorer",fCurrent_In);

  // Register scorer with detector
  detector->RegisterPrimitive(scorer);  
*/
}



//###########
//zadania do wykonania
/*
– rozkład widmowy (tzn. w funkcji długości fali) fotonów padających na fotokatodę,
– rozkład liczby fotonów w zależności od czasu ich przybycia do fotokatody,
– rozkład liczby wybitych fotoelektronów w zależności od czasu, zakładając, że wydaj-
ność fotopowielacza dana jest formułą przedstawioną na poniższym rysunku:
– rozkład liczby fotoelektronów w zależnosci od długości fali fotonu,
– dwuwymiarowy rozkład czasu przybycia fotonów do fotokatody w zależności od ich
długości fali,
*/

/*
	1)widmo fotonów (lambda)
	2)N fotonów od t
	3)N fotonów od t z założeniem wydajności z rysuku (N_miezone=N_real/wydajnosc(lambda))
	-rozklad 2D N(t,lambda)
	-calka od 0 do inf z rozklad / wydajnosc(lambda) * d_lambda -> rzeczywiste N(t)
	4)rzeczywiste N(lambda) - fotoelektronów
	5)rozklad 2D N(t,lambda)
*/

/*
plan działania:
-rozkład 2D N(t,lambda)
-rozkład 2D N_real(t,lambda)/wydajność -> rozkład rzeczywisty
5)OK
-całka N(t,lambda) po lambda -> N(t)
-całka N_real(t,lambda) po lambda -> N_real(t)
2)3)OK
-całka N(t,lambda) po t -> N(lambda)
-całka N_real(t,lambda) po t -> N_real(lambda)
4)OK

*/






