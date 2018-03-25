#ifndef MYRUN_HH
#define MYRUN_HH

#include "G4Run.hh"
#include "G4THitsMap.hh"

/////
#include "G4VHit.hh"
/////

class G4Event;

class MyRun : public G4Run {


public:

	G4int fCollectionId;
	G4THitsMap<G4double> frunHitsMap;

  // Constructor 
  MyRun();

  // Destructor
  virtual ~MyRun();

  //Method
  void RecordEvent(const G4Event*);
  

};











#endif
