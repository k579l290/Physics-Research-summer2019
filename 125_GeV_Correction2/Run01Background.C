#define Run01Background_cxx
#include "Run01Background.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

#include <TLorentzVector.h>
R__LOAD_LIBRARY(libTreePlayer)

void Run01Background::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Run01Background.C
//      root> Run01Background t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   
	//declare histogram
   TH1D *o_angle = new TH1D("","Opening Angle", 100, -2 * TMath::Pi(), 2 * TMath::Pi());
   
   //declare lorentzvectors
   TLorentzVector p1(0,0,0,0);
   TLorentzVector p2(0,0,0,0);

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
	  if (Photon_size >= 2) { //look at entries with >=2 photons
		//set lorentz vector values
			p1.SetPtEtaPhiE(Photon_PT[0],Photon_Eta[0],Photon_Phi[0],Photon_E[0]);
			p2.SetPtEtaPhiE(Photon_PT[1],Photon_Eta[1],Photon_Phi[1],Photon_E[1]);
	    }
		double p1_p = p1.Pt();
		double p2_p = p2.Pt();
		
		if (p1_p >= 40 && p2_p >= 30) { //fill opening angle histogram if photon1 pt > 40 and photon2 pt > 30
			double a = p1.Angle(p2.Vect());
			o_angle->Fill(a);
		}
   }
		//write histogram to root file
		TFile f("125_OAngle.root", "update");
		o_angle->Write("b1");
		
		
		//display histogram on canvas
		TCanvas *c1=new TCanvas();  
		o_angle->GetXaxis()->SetTitle("Angle (Rad)");
		o_angle->GetYaxis()->SetTitle("Number of Events");
		o_angle->Draw();
		
		
}
