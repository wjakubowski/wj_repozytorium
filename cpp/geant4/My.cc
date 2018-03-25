#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyPrimaryGeneratorAction.hh"
#include "MyRunAction.hh"
#include "G4RunManager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4String.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif



#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv) {
  

  G4RunManager * runManager = new G4RunManager;
  runManager->SetUserInitialization(new MyDetectorConstruction);
  runManager->SetUserInitialization(new MyPhysicsList);
  runManager->SetUserAction(new MyPrimaryGeneratorAction());
  //##################
  runManager->SetUserAction(new MyRunAction());
  //##################
  runManager->Initialize();
  
  G4UImanager* UI = G4UImanager::GetUIpointer();  

  if (argc!=1)   
    {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UI->ApplyCommand(command+fileName);
    }
    
  else           
    { 
#ifdef G4VIS_USE
   G4VisManager* visManager = new G4VisExecutive;
   visManager->Initialize();
#endif    
     
#ifdef G4UI_USE
	G4UIExecutive * ui = new G4UIExecutive(argc,argv);    
	
	UI->ApplyCommand("/control/execute vis.mac"); 
	//################dodane przez Wojtka
	UI->ApplyCommand("/vis/open OGLI");
	UI->ApplyCommand("/control/execute run.mac");
	
      
	ui->SessionStart();
	delete ui;
#endif
     
#ifdef G4VIS_USE
     delete visManager;
#endif     
    }

  delete runManager;

  return 0;
  
  
}

