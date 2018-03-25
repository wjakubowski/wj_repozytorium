
#ifndef MYDETECTORCONSTRUCTION_HH
#define MYDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"


class G4LogicalVolume;
class G4VPhysicalVolume;


class MyDetectorConstruction : public G4VUserDetectorConstruction {

public:
  
  // Constructor
  MyDetectorConstruction();
  
  // Destructor
  virtual ~MyDetectorConstruction();
  
  // Method
  virtual G4VPhysicalVolume* Construct();
  
private:

  // Helper methods
  void DefineMaterials();
  void SetupGeometry();
  
  // World logical and physical volumes
  G4LogicalVolume*   fpWorldLogical;
  G4VPhysicalVolume* fpWorldPhysical;
///////////////////////////////////////////////////
  void SetupScoring(G4LogicalVolume*);
///////////////////////////////////////////////////




//#########################################################
//moje parametry


//wymiary pret
G4double szerokosc_pret;
G4double dlugosc_pret;
G4double wysokosc_pret_Si;
G4double wysokosc_pret_powietrze;
G4double grubosc_folia_Al;

//wymiarykatoda
G4double szerokosc_katoda;
G4double dlugosc_katoda;
G4double wysokosc_katoda;

//kat przechylenia
G4double fi;





};

#endif

