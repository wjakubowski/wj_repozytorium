#include "MyRunAction.hh"
#include "G4Run.hh"
#include "globals.hh"
#include "MyRun.hh"
#include "MyAnalysis.hh"


MyRunAction::MyRunAction() 
{}

MyRunAction::~MyRunAction() 
{}

G4Run* MyRunAction::GenerateRun()
{
 return new MyRun();
}

/*
void MyRunAction::BeginOfRunAction(const G4Run* aRun)
{

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstNtupleId(1);
  analysisManager->OpenFile("tree.root");
  analysisManager->CreateNtuple("PhotonCollection","Kolekcja fotonow");
  analysisManager->CreateNtupleDColumn("t");
  analysisManager->CreateNtupleDColumn("E");
  analysisManager->FinishNtuple();

  

}

void MyRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();


}


*/


void MyRunAction::BeginOfRunAction(const G4Run* aRun)
{
// Create analysis manager
G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
analysisManager->SetVerboseLevel(1);
analysisManager->SetFirstNtupleId(1);
// Open an output file
analysisManager->OpenFile("tree.root");
// Creating ntuple
analysisManager->CreateNtuple("PhotonCollection","Kolekcja fotonów");
analysisManager->CreateNtupleDColumn("t");
analysisManager->CreateNtupleDColumn("E");

analysisManager->FinishNtuple();

}

void MyRunAction::EndOfRunAction(const G4Run* aRun)
{
// Get analysis manager
G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
// Save ntuple and close file
analysisManager->Write();
analysisManager->CloseFile();
}

