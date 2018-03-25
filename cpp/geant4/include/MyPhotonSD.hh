/*
#ifndef MOJDETEKTOR_HH
#define MOJDETEKTOR_HH
      #include "G4PSDoseDeposit.hh"
      #include "G4THitsMap.hh"
      #include <string>
      #include <fstream>
      #include <vector>
      #include "MyPhotonHit.hh"

      class MojDetektor : public G4PSDoseDeposit
      {
       public: // with description
            MojDetektor(G4String name, G4int depth=0);

        protected: // with description
            virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
       
        public: 
            virtual void Initialize(G4HCofThisEvent*);
		
	//moja pamiec
		std::vector<Moja_Klasa_Pamieci*> fotony;
		void zapisz_dane_do_pliku(std::string);
		
	 private:
		G4int HCID;
   		G4THitsMap<G4double>* EvtMap;

      };
      #endif*/


#ifndef MYPHOTONSD_HH
#define MYPHOTONSD_HH

#include "G4VSensitiveDetector.hh"
#include "MyPhotonHit.hh"
#include "G4THitsCollection.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class MyPhotonSD : public G4VSensitiveDetector {

public:

  // Constructor
  MyPhotonSD(const G4String& name);

  // Destructor
  virtual ~MyPhotonSD();
  // Methods
  void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);  
  G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);
  void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  //inline int getfPhotonCollectionID(){return fPhotonCollectionID;}
  //inline G4String getSensitiveDetectorName(){return SensitiveDetectorName;}
private:
  
  // Data members
  G4THitsCollection<MyPhotonHit>* fPhotonCollection;
  G4int fPhotonCollectionID;

};




#endif




	




