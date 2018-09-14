#define dpd_cxx
#include "dpd.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

int nbins_charge=90;
float min_charge=0.0;
float max_charge=9.0;
float scale_charge=1.E-6;

int nbins_recocharge=150;
float min_recocharge=0.0;
float max_recocharge=1.5;
float scale_recocharge=1.E-6;

int nbins_light=100;
float min_light=0.0;
float max_light=1.0;
float scale_light=1.E6;

int nbins_S2light=120;
float min_S2light=0.0;
float max_S2light=1.2;
float scale_S2light=1.E6;

int nbins_npe=70;
float min_npe=0.0;
float max_npe=7.0;
float scale_npe=1.E-6;

void dpd::Loop()
{
//   In a ROOT session, you can do:
//      root> .L dpd.C
//      root> dpd t
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

   Long64_t nentries = fChain->GetEntriesFast();
   
   
   
   TH2F* h_charge_light = new TH2F("h_charge_light","tot pmt light vs. tot charge;Charge [10^{6} ADC*ticks];Tot light [#muC]",nbins_charge,min_charge,max_charge,nbins_light,min_light,max_light);
   TH2F* h_recocharge_light = new TH2F("h_recocharge_light","tot pmt light vs. tot reco charge;Reco charge [10^{6} ADC*ticks];Tot light [#muC]",nbins_recocharge,min_recocharge,max_recocharge,nbins_light,min_light,max_light);
   TH2F* h_tracks_light = new TH2F("h_tracks_light","tot pmt light vs. n tracks;N;Tot light [#muC]",80,0,80,nbins_light,min_light,max_light); 
  
   TH2F* h_charge_S2light = new TH2F("h_charge_S2light","S2 pmt light vs. tot charge;Charge [10^{6} ADC*ticks];S2 light [#muC]",nbins_charge,min_charge,max_charge,nbins_S2light,min_S2light,max_S2light);
   TH2F* h_recocharge_S2light = new TH2F("h_recocharge_S2light","S2 pmt light vs. tot reco charge;Reco charge [10^{6} ADC*ticks];S2 light [#muC]",nbins_recocharge,min_recocharge,max_recocharge,nbins_S2light,min_S2light,max_S2light);
   TH2F* h_tracks_S2light = new TH2F("h_tracks_S2light","S2 pmt light vs. n tracks;N;S2 light [#muC]",80,0,80,nbins_S2light,min_S2light,max_S2light); 
  
   TH2F* h_charge_S2npe = new TH2F("h_charge_S2npe","S2 n.p.e. vs. tot charge;Charge [10^{6} ADC*ticks];PMT S2 n.p.e. (x10^{6})",nbins_charge,min_charge,max_charge,nbins_npe,min_npe,max_npe);
   TH2F* h_recocharge_S2npe = new TH2F("h_recocharge_S2npe","S2 n.p.e. vs. tot reco charge;Reco charge [10^{6} ADC*ticks];PMT S2 n.p.e. (x10^{6})",nbins_recocharge,min_recocharge,max_recocharge,nbins_npe,min_npe,max_npe);
  
   TH2F* h_S1S2_charge = new TH2F("h_S1S2_charge","S2 pmt_charge vs. S1 pmt_charge;S1 charge [nC];S2 charge [#muC]",100,0,5,80,0,0.4);
   TH2F* h_S1S2_npe = new TH2F("h_S1S2_npe","S2 n.p.e. vs. S1 n.p.e.;S1 n.p.e. (x10^{3});S2 n.p.e. (x10^{6})",100,0,20,100,0,2.0);
   TH2F* h_S1S2_width = new TH2F("h_S1S2_width","S2_gaus width vs. S1 width;S1 width [ns];S2_gaus width [#mus]",120,0,120,175,0,350);
   TH2F* h_S1S2_amp = new TH2F("h_S1S2_amp","S2 amp vs. S1 amp;S1 amp [V];S2 gaus amp [V]",100,0,2.01,90,0,0.09);
  
   /*
   TH2F* h_charge_light = new TH2F("h_charge_light","tot pmt light vs. tot charge;Charge [10^{6} ADC*ticks];Light [#muC]",250,0,5.,250,0,0.8);
   TH2F* h_tracks_light = new TH2F("h_tracks_light","tot pmt light vs. n tracks;N;Light [#muC]",80,0,80,160,0,0.8);
   TH2F* h_charge_S2light = new TH2F("h_charge_S2light","S2 pmt light vs. tot charge;Charge [10^{6} ADC*ticks];S2 light [#muC]",250,0,5.,250,0,0.3);
  
   TH2F* h_S1S2_charge = new TH2F("h_S1S2_charge","S2 pmt_charge vs. S1 pmt_charge;PMT S1 charge [nC];PMT S2 charge [#muC]",250,0,2.5,250,0,0.3);
   TH2F* h_S1S2_npe = new TH2F("h_S1S2_npe","S2 n.p.e. vs. S1 n.p.e.;PMT S1 n.p.e. (x10^{3});PMT S2 n.p.e. (x10^{6})",250,0,15,250,0,1.8);
   TH2F* h_S1S2_width = new TH2F("h_S1S2_width","S2_gaus width vs. S1 width;PMT S1 width [ns];PMT S2_gaus width [#mus]",100,0,100,175,0,350);
   TH2F* h_S1S2_amp = new TH2F("h_S1S2_amp","S2 amp vs. S1 amp;PMT S1 amp [V];PMT S2 amp [V]",100,0,2.01,100,0,0.8);
   */
 
  
   TH2F* h_charge_light_cut = (TH2F*)h_charge_light->Clone("h_charge_light_cut");
   TH2F* h_recocharge_light_cut = (TH2F*)h_recocharge_light->Clone("h_recocharge_light_cut");
   TH2F* h_tracks_light_cut = (TH2F*)h_tracks_light->Clone("h_tracks_light_cut");
   TH2F* h_charge_S2light_cut = (TH2F*)h_charge_S2light->Clone("h_charge_S2light_cut");
   TH2F* h_recocharge_S2light_cut = (TH2F*)h_recocharge_S2light->Clone("h_recocharge_S2light_cut");
   TH2F* h_tracks_S2light_cut = (TH2F*)h_tracks_S2light->Clone("h_tracks_S2light_cut");
   TH2F* h_charge_S2npe_cut = (TH2F*)h_charge_S2npe->Clone("h_charge_S2npe_cut");
   TH2F* h_recocharge_S2npe_cut = (TH2F*)h_recocharge_S2npe->Clone("h_recocharge_S2npe_cut");
   TH2F* h_S1S2_charge_cut = (TH2F*)h_S1S2_charge->Clone("h_S1S2_charge_cut");
   TH2F* h_S1S2_npe_cut = (TH2F*)h_S1S2_npe->Clone("h_S1S2_npe_cut");
   TH2F* h_S1S2_width_cut = (TH2F*)h_S1S2_width->Clone("h_S1S2_width_cut");
   TH2F* h_S1S2_amp_cut = (TH2F*)h_S1S2_amp->Clone("h_S1S2_amp_cut");
   

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

	  if (jentry%10000==0) cout << "event " << jentry << endl;

	  // event cuts
	  bool pmt_trig_cut[N_PMT]={false};
	  bool pmt_pedrms_cut[N_PMT]={false};
	  bool pmt_S1_amp_cut[N_PMT]={false};
	  bool pmt_S2_gaus_tau_cut[N_PMT]={false};
	  bool pmt_S2_gaus_width_cut[N_PMT]={false};
	  bool pmt_ped_diff_cut[N_PMT]={false};
	  bool charge_cut=false;
	  
	  for (int k=0; k<N_PMT; k++)
	  {
		  if (pmt_S2_charge[k]==0 && pmt_S2_npe[k]==0 && pmt_S2_width[k]==0 && pmt_S2_tau[k]==0) continue;
		  if (pmt_S2_gaus_charge[k]==0 && pmt_S2_gaus_npe[k]==0 && pmt_S2_gaus_width[k]==0 && pmt_S2_gaus_tau[k]==0) continue;
		  
		  if ((nsamples==262144 && fabs(pmt_S1_tau[k]-S1_tau_fit_mean[k])<3.*S1_tau_fit_sigma[k])) pmt_trig_cut[k]=true;
		  if (pmt_pedrms[k]<2.0) pmt_pedrms_cut[k]=true; // 2.0
		  if (pmt_S1_amp[k]<1.95) pmt_S1_amp_cut[k]=true; 
		  if (pmt_S2_gaus_tau[k]>230. && pmt_S2_gaus_tau[k]<850) pmt_S2_gaus_tau_cut[k]=true; // 300,850
		  //if (pmt_S2_gaus_width[k]<330.) pmt_S2_gaus_width_cut[k]=true;
		  //if (fabs(pmt_ped_end[k]-pmt_ped[k])/pmt_pedrms[k]<20.0) pmt_ped_diff_cut[k]=true;
		  //if ((4096.0/2.0)*fabs(pmt_S1_amp[k])>=12.*pmt_pedrms[k]) pmt_pedrms_cut[k]=true;
		  
		  if (ntracks>=1) charge_cut=true;
		  
		  h_S1S2_charge->Fill(pmt_S1_charge[k]*1.E9,pmt_S2_charge[k]*scale_light);
		  h_S1S2_npe->Fill(pmt_S1_npe[k]*1.E-3,pmt_S2_npe[k]*scale_npe);
		  h_S1S2_width->Fill(pmt_S1_width[k],pmt_S2_gaus_width[k]);
		  h_S1S2_amp->Fill(pmt_S1_amp[k],pmt_S2_gaus_amp[k]);
		  
		  if (!pmt_trig_cut[k]) continue;
		  if (!pmt_pedrms_cut[k]) continue;
		  if (!pmt_S1_amp_cut[k]) continue;
		  if (!pmt_S2_gaus_tau_cut[k]) continue;
		  //if (!pmt_S2_gaus_width_cut[k]) continue;
		  //if (!pmt_ped_diff_cut[k]) continue;
		  if (!charge_cut) continue;
		  
		  h_S1S2_charge_cut->Fill(pmt_S1_charge[k]*1.E9,pmt_S2_charge[k]*scale_light);
		  h_S1S2_npe_cut->Fill(pmt_S1_npe[k]*1.E-3,pmt_S2_npe[k]*scale_npe);
		  h_S1S2_width_cut->Fill(pmt_S1_width[k],pmt_S2_gaus_width[k]);
		  h_S1S2_amp_cut->Fill(pmt_S1_amp[k],pmt_S2_gaus_amp[k]);
		  
	  }
	  //printf("pmt_trig_cut = { %d, %d, %d, %d, %d }\n",pmt_trig_cut[0],pmt_trig_cut[1],pmt_trig_cut[2],pmt_trig_cut[3],pmt_trig_cut[4]);
	  //printf("pmt_pedrms_cut = { %d, %d, %d, %d, %d }\n",pmt_pedrms_cut[0],pmt_pedrms_cut[1],pmt_pedrms_cut[2],pmt_pedrms_cut[3],pmt_pedrms_cut[4]);
	
	  h_charge_light->Fill(tpc_totcharge*scale_charge,pmt_charge[5]*scale_light);
	  h_recocharge_light->Fill(tpc_totrecocharge*scale_recocharge,pmt_charge[5]*scale_light);
	  h_tracks_light->Fill(ntracks,pmt_charge[5]*scale_light);
	  h_charge_S2light->Fill(tpc_totcharge*scale_charge,pmt_S2_charge[5]*scale_light); 
	  h_recocharge_S2light->Fill(tpc_totrecocharge*scale_recocharge,pmt_S2_charge[5]*scale_light); 
	  h_tracks_S2light->Fill(ntracks,pmt_S2_charge[5]*scale_light);
	  h_charge_S2npe->Fill(tpc_totcharge*scale_charge,pmt_S2_npe[5]*scale_npe);
	  h_recocharge_S2npe->Fill(tpc_totrecocharge*scale_recocharge,pmt_S2_npe[5]*scale_npe);
	  
	  if (
		  pmt_trig_cut[0] && pmt_trig_cut[1] && pmt_trig_cut[2] && pmt_trig_cut[3] && pmt_trig_cut[4] &&
		  pmt_pedrms_cut[0] && pmt_pedrms_cut[1] && pmt_pedrms_cut[2] && pmt_pedrms_cut[3] && pmt_pedrms_cut[4] &&
		  pmt_S1_amp_cut[0] && pmt_S1_amp_cut[1] && pmt_S1_amp_cut[2] && pmt_S1_amp_cut[3] && pmt_S1_amp_cut[4] &&
		  pmt_S2_gaus_tau_cut[0] && pmt_S2_gaus_tau_cut[1] && pmt_S2_gaus_tau_cut[2] && pmt_S2_gaus_tau_cut[3] && pmt_S2_gaus_tau_cut[4] &&
		  //pmt_S2_gaus_width_cut[0] && pmt_S2_gaus_width_cut[1] && pmt_S2_gaus_width_cut[2] && pmt_S2_gaus_width_cut[3] && pmt_S2_gaus_width_cut[4] &&
		  //pmt_ped_diff_cut[0] && pmt_ped_diff_cut[1] && pmt_ped_diff_cut[2] && pmt_ped_diff_cut[3] && pmt_ped_diff_cut[4] &&
		  charge_cut)
	  {
		  h_charge_light_cut->Fill(tpc_totcharge*scale_charge,pmt_charge[5]*scale_light);
		  h_recocharge_light_cut->Fill(tpc_totrecocharge*scale_recocharge,pmt_charge[5]*scale_light);
		  h_tracks_light_cut->Fill(ntracks,pmt_charge[5]*scale_light);
		  h_charge_S2light_cut->Fill(tpc_totcharge*scale_charge,pmt_S2_charge[5]*scale_light); 
		  h_recocharge_S2light_cut->Fill(tpc_totrecocharge*scale_recocharge,pmt_S2_charge[5]*scale_light); 
		  h_tracks_S2light_cut->Fill(ntracks,pmt_S2_charge[5]*scale_light);
		  h_charge_S2npe_cut->Fill(tpc_totcharge*scale_charge,pmt_S2_npe[5]*scale_npe);
		  h_recocharge_S2npe_cut->Fill(tpc_totrecocharge*scale_recocharge,pmt_S2_npe[5]*scale_npe);
	  }
	
	  /*
	  for (int k=0; k<5; k++)
	  {
		  if (fabs(pmt_S1_width[k])<10.) 
			  printf("Charge = %0.3e, npe = %0.3e, pedrms = %0.3e, S1 charge = %0.3e, S1 width = %0.3e, S1 amp = %0.3e, S1 tau = %0.3e\n",
		  			pmt_charge[k],pmt_npe[k],pmt_pedrms[k],pmt_S1_charge[k],pmt_S1_width[k],pmt_S1_amp[k],pmt_S1_tau[k]);
		
	  }
	  */
   }
   
   TFile *f = new TFile("2D.root","RECREATE");
   h_charge_light->Write();
   h_charge_light_cut->Write();
   h_recocharge_light->Write();
   h_recocharge_light_cut->Write();
   h_tracks_light->Write();
   h_tracks_light_cut->Write();
   h_charge_S2npe->Write();
   h_charge_S2npe_cut->Write();
   h_recocharge_S2npe->Write();
   h_recocharge_S2npe_cut->Write();
   h_charge_S2light->Write();
   h_charge_S2light_cut->Write();
   h_recocharge_S2light->Write();
   h_recocharge_S2light_cut->Write();
   h_tracks_S2light->Write();
   h_tracks_S2light_cut->Write();
   h_S1S2_charge->Write();
   h_S1S2_charge_cut->Write();
   h_S1S2_npe->Write();
   h_S1S2_npe_cut->Write();
   h_S1S2_width->Write();
   h_S1S2_width_cut->Write();
   h_S1S2_amp->Write();
   h_S1S2_amp_cut->Write();
   
   f->Close();
}
	  
