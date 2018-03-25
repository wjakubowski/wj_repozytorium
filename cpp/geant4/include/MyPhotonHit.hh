#ifndef MYPHOTONHIT_HH
#define MYPHOTONHIT_HH

#include "G4VHit.hh"
#include "G4ThreeVector.hh"

class MyPhotonHit : public G4VHit {

public:

  // Constructor
  MyPhotonHit();

  // Destructor
  virtual ~MyPhotonHit();

  // Methods 
  // void Draw()
   //void Print();

  // Photon energy
  inline void SetEnergy(G4double energy) {fEnergy = energy;}
  inline G4double GetEnergy() const {return fEnergy;}

  // Photon position
  inline void SetTime(G4double time) {fTime = time;}
  inline G4double GetTime() const {return fTime;}
  inline void Draw(){std::cout<<" "<<fTime<<"  "<<fEnergy<<" ";}
private:
  //Data Members
 G4double fEnergy;
 G4double fTime;
};

#endif
