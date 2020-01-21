#define Run03Signal_cxx
#include "Run03Signal.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

#include <TLorentzVector.h>
R__LOAD_LIBRARY(libTreePlayer)

void Run03Signal::Loop()
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
   
   //TH1D *HBmass = new TH1D("", "Mass", 200,0,200);
  // TH1D *photon_p = new TH1D("", "Momentum", 100,0,1000);
  // TH1D *eta_1 = new TH1D("","Photon 1 Eta", 100, -2 * TMath::Pi(), 2 * TMath::Pi());
  // TH1D *eta_2 = new TH1D("","Photon 2 Eta", 100, -2 * TMath::Pi(), 2 * TMath::Pi());
   TH1D *o_angle = new TH1D("","Opening Angle", 100, -2 * TMath::Pi(), 2 * TMath::Pi());
   
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
		//ptotal = p1 + p2;
		//double ptt = ptotal.Pt();
		//photon_p->Fill(ptt);
		double p1_p = p1.Pt();
		double p2_p = p2.Pt();
		
		if (p1_p >= 40 && p2_p >= 30) {
			//double m = ptotal.M();
			double a = p1.Angle(p2.Vect());
			o_angle->Fill(a);
		}
		
		/*double eta_p1= p1.Eta();
		eta_1->Fill(eta_p1);
		double eta_p2 = p2.Eta();
		eta_2->Fill(eta_p2);*/
   }
		
		TFile f("125_OAngle.root", "update");
		o_angle->Write("s3");
		
		/*TFile f2("125_Pt.root" , "update");
		photon_p->Write("b1");*/
		
		/*TFile f3("Background_eta.root", "update");
		eta_1->Write("1_p1");
		eta_2->Write("1_p2");*/
		
		
		
		TCanvas *c1=new TCanvas();  
		o_angle->GetXaxis()->SetTitle("Angle (Rad)");
		o_angle->GetYaxis()->SetTitle("Number of Events");
		o_angle->Draw();
		
		/*TCanvas *c5=new TCanvas(); 
		photon_p->GetXaxis()->SetTitle("p1+p2 Momentum (GeV/c)");
		photon_p->GetYaxis()->SetTitle("Number of Events");
		photon_p->Draw();
		
		TCanvas *c6=new TCanvas(); 
		eta_1->GetXaxis()->SetTitle("Photon 1 Eta");
		eta_1->GetYaxis()->SetTitle("Number of Events");
		eta_1->Draw();
		
		TCanvas *c7=new TCanvas(); 
		eta_2->GetXaxis()->SetTitle("Photon 2 Eta");
		eta_2->GetYaxis()->SetTitle("Number of Events");
		eta_2->Draw();*/
		
		
}