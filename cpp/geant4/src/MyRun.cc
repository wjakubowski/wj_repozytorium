#include "MyRun.hh"
#include "G4Event.hh"
#include "globals.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include <iomanip>
#include <iostream>
#include <fstream>
#include "G4SDManager.hh"

#include "G4THitsMap.hh"
#include "G4THitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "MyPhotonHit.hh"
#include "MyAnalysis.hh"

MyRun::MyRun()
{

	//G4SDManager* manager = G4SDManager::GetSDMpointer();
	//fMapId = manager->GetCollectionID("MyDetector/MyScorer");
	//G4cout << "MyLog:  MyRun constructor: index of photon scorer map: " << fMapId << G4endl;
	  std::fstream plik;
  	  plik.open("wyniki/czas_energia.txt");
	  plik.close();
}

MyRun::~MyRun()
{}

void MyRun::RecordEvent(const G4Event* evt)
{
 numberOfEvent++;
  G4HCofThisEvent* hce = evt->GetHCofThisEvent();

G4THitsCollection<MyPhotonHit>* photonCollection =
      dynamic_cast<G4THitsCollection<MyPhotonHit>*> (hce->GetHC(fCollectionId));

G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  std::fstream plik;
  plik.open("wyniki/czas_energia.txt",std::ios::app);
  
  for( G4int i = 0; i< photonCollection->entries(); i++) 
  {
     float e = (*photonCollection)[i]->GetEnergy()/MeV;
     float t = (*photonCollection)[i]->GetTime()/ns;
     std::cout<<t<<"  "<<e<<'\n';
     plik<<t<<"\t"<<e<<'\n';
     analysisManager->FillNtupleDColumn(0,t);
     analysisManager->FillNtupleDColumn(1,e);
     analysisManager->AddNtupleRow();

  }
  //plik.close();
}
/*
MyRun.cc
--------------
#include "MyAnalysis.hh"
void MyRun::RecordEvent(const G4Event* evt)
{
............................................................................
G4THitsCollection<MyPhotonHit>* photonCollection =
dynamic_cast<G4THitsCollection<MyPhotonHit>*> (hce->GetHC(fCollectionI
// Get analysis manager
G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
for( G4int
float e
float x
float y
float z
i
=
=
=
=
= 0; i< photonCollection->entries(); i++) {
(*photonCollection)[i]->GetEnergy()/MeV;
(*photonCollection)[i]->GetPosition().x()/cm;
(*photonCollection)[i]->GetPosition().y()/cm;
(*photonCollection)[i]->GetPosition().z()/cm;
analysisManager->FillNtupleDColumn(0,e);
analysisManager->FillNtupleDColumn(1,x);
analysisManager->FillNtupleDColumn(2,y);
analysisManager->FillNtupleDColumn(3,z);
analysisManager->AddNtupleRow();
}
*/


