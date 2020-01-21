#define Run05Signal_cxx
#include "Run05Signal.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

#include <TLorentzVector.h>
R__LOAD_LIBRARY(libTreePlayer)

void Run05Signal::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Run05Signal.C
//      root> Run05Signal t
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

	TH1D *HBmass = new TH1D("", "Mass", 200,0,200);
	TH1D *photon_p = new TH1D("", "Momentum", 100,0,1000);
	TH1D *eta_t = new TH1D("","Eta", 100, -2 * TMath::Pi(), 2 * TMath::Pi());

	
	TLorentzVector p1(0,0,0,0);
	TLorentzVector p2(0,0,0,0);
	TLorentzVector ptotal(0,0,0,0);
   
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
	  if (Photon_size >= 2) { //look at entries with >=2 photons
		//fill histogram
			p1.SetPtEtaPhiE(Photon_PT[0],Photon_Eta[0],Photon_Phi[0],Photon_E[0]);
			p2.SetPtEtaPhiE(Photon_PT[1],Photon_Eta[1],Photon_Phi[1],Photon_E[1]);
	    }
		
		ptotal = p1 + p2;
		double ptt = ptotal.Pt();
		photon_p->Fill(ptt);
		double m = ptotal.M();
		HBmass->Fill(m);
		double eta_total = ptotal.Eta();
		eta_t->Fill(eta_total);
   }
   
		/*TFile f("125_Mass.root", "update");
		HBmass->Write("s5");
		
		TFile f2("125_Pt.root" , "update");
		photon_p->Write("s5");*/
		
		
		TCanvas *c1=new TCanvas();  
		HBmass->GetXaxis()->SetTitle("Mass (GeV)");
		HBmass->GetYaxis()->SetTitle("Number of Events");
		HBmass->Draw();
		
		TCanvas *c5=new TCanvas(); 
		photon_p->GetXaxis()->SetTitle("p1+p2 Momentum (GeV/c)");
		photon_p->GetYaxis()->SetTitle("Number of Events");
		photon_p->Draw();
		
		TCanvas *c9=new TCanvas(); 
		eta_t->GetXaxis()->SetTitle("Total Eta");
		eta_t->GetYaxis()->SetTitle("Number of Events");
		eta_t->Draw();
}
