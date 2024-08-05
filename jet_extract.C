#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

#include <fstream>

//------------------------------------------------------------------------------

void jet_extract(const char *inputFile)
{
  gSystem->Load("/home/kousik/Tools/Delphes-3.5.0/libDelphes.so");

  TChain chain("Delphes");
  chain.Add(inputFile);

  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  TClonesArray *branchJet = treeReader->UseBranch("Jet");

  std::ofstream outFile("jet_extract.csv");
  outFile << "Event_Number,Jet_No,Px,Py,Pz,Energy,Mass,Eta,Phi\n";

  for (Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    treeReader->ReadEntry(entry);
    Int_t numberOfJets = branchJet->GetEntries();
    
    if (numberOfJets > 0)
    {
      for (Int_t jetIndex = 0; jetIndex < numberOfJets; ++jetIndex)
      {
        Jet *jet = (Jet*) branchJet->At(jetIndex);
        double px = jet->PT * cos(jet->Phi);
        double py = jet->PT * sin(jet->Phi);
        double pz = jet->PT * sinh(jet->Eta);
        double E = sqrt(px * px + py * py + pz * pz + jet->Mass * jet->Mass);
        
        // Print to console
        cout << "Event No: " << entry + 1 
             << " , Jet No: " << jetIndex + 1 
             << " , Px = " << px 
             << " , Py = " << py 
             << " , Pz = " << pz 
             << " , E = " << E 
             << " , Mass = " << jet->Mass 
             << " , Eta = " << jet->Eta 
             << " , Phi = " << jet->Phi 
             << endl;
        
        // Write to CSV file
        outFile << entry + 1 << "," 
                << jetIndex + 1 << "," 
                << px << "," 
                << py << "," 
                << pz << "," 
                << E << "," 
                << jet->Mass << "," 
                << jet->Eta << "," 
                << jet->Phi << "\n";
      }
    }
  }

  outFile.close();
}
