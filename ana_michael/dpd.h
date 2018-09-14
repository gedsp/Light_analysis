//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Sep 14 15:45:48 2018 by ROOT version 6.08/06
// from TTree dpd/result tree
// found on file: dpd_goodRunList_v2_1117_20180914.root
//////////////////////////////////////////////////////////

#ifndef dpd_h
#define dpd_h

#include "../config_analysis.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class dpd {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           ev;
   Int_t           run;
   Int_t           subrun;
   Int_t           nsamples;
   Float_t         time_charge;
   Float_t         time_light;
   Int_t           crt_matchreco;
   Float_t         pmt_wvf_properties[5][2];
   Float_t         pmt_charge[6];
   Float_t         pmt_npe[6];
   Float_t         pmt_ped[5];
   Float_t         pmt_pedrms[5];
   Float_t         pmt_ped_end[5];
   Float_t         pmt_pedrms_end[5];
   Float_t         pmt_S1_charge[6];
   Float_t         pmt_S1_npe[6];
   Float_t         pmt_S1_width[5];
   Float_t         pmt_S1_amp[5];
   Float_t         pmt_S1_tau[5];
   Float_t         pmt_S2_charge[6];
   Float_t         pmt_S2_npe[6];
   Float_t         pmt_S2_width[5];
   Float_t         pmt_S2_amp[5];
   Float_t         pmt_S2_tau[5];
   Float_t         pmt_S2_gaus_charge[6];
   Float_t         pmt_S2_gaus_npe[6];
   Float_t         pmt_S2_gaus_width[5];
   Float_t         pmt_S2_gaus_amp[5];
   Float_t         pmt_S2_gaus_tau[5];
   Short_t         ntracks;
   Float_t         tpc_totcharge;
   Float_t         tpc_totrecocharge;
   Float_t         tpc_track_charge[142];   //[ntracks]
   Float_t         crt_mYX;
   Float_t         crt_mZX;
   Float_t         crt_nYX;
   Float_t         crt_nZX;
   Float_t         tpc_mYX[142];   //[ntracks]
   Float_t         tpc_mZX[142];   //[ntracks]
   Float_t         tpc_nYX[142];   //[ntracks]
   Float_t         tpc_nZX[142];   //[ntracks]
   Float_t         tpc_startX[142];   //[ntracks]
   Float_t         tpc_endX[142];   //[ntracks]
   Int_t           tpc_track_fitresult_yx[142];   //[ntracks]
   Int_t           tpc_track_fitresult_zx[142];   //[ntracks]
   Float_t         Track_Length[142];   //[ntracks]
   Float_t         tpc_drift_time_at_pmt_pos[5];

   // List of branches
   TBranch        *b_ev;   //!
   TBranch        *b_run;   //!
   TBranch        *b_subrun;   //!
   TBranch        *b_nsamples;   //!
   TBranch        *b_time_charge;   //!
   TBranch        *b_time_light;   //!
   TBranch        *b_crt_matchreco;   //!
   TBranch        *b_pmt_wvf_properties;   //!
   TBranch        *b_pmt_charge;   //!
   TBranch        *b_pmt_npe;   //!
   TBranch        *b_pmt_ped;   //!
   TBranch        *b_pmt_pedrms;   //!
   TBranch        *b_pmt_ped_end;   //!
   TBranch        *b_pmt_pedrms_end;   //!
   TBranch        *b_pmt_S1_charge;   //!
   TBranch        *b_pmt_S1_npe;   //!
   TBranch        *b_pmt_S1_width;   //!
   TBranch        *b_pmt_S1_amp;   //!
   TBranch        *b_pmt_S1_tau;   //!
   TBranch        *b_pmt_S2_charge;   //!
   TBranch        *b_pmt_S2_npe;   //!
   TBranch        *b_pmt_S2_width;   //!
   TBranch        *b_pmt_S2_amp;   //!
   TBranch        *b_pmt_S2_tau;   //!
   TBranch        *b_pmt_S2_gaus_charge;   //!
   TBranch        *b_pmt_S2_gaus_npe;   //!
   TBranch        *b_pmt_S2_gaus_width;   //!
   TBranch        *b_pmt_S2_gaus_amp;   //!
   TBranch        *b_pmt_S2_gaus_tau;   //!
   TBranch        *b_ntracks;   //!
   TBranch        *b_tpc_totcharge;   //!
   TBranch        *b_tpc_totrecocharge;   //!
   TBranch        *b_tpc_track_charge;   //!
   TBranch        *b_crt_mYX;   //!
   TBranch        *b_crt_mZX;   //!
   TBranch        *b_crt_nYX;   //!
   TBranch        *b_crt_nZX;   //!
   TBranch        *b_tpc_mYX;   //!
   TBranch        *b_tpc_mZX;   //!
   TBranch        *b_tpc_nYX;   //!
   TBranch        *b_tpc_nZX;   //!
   TBranch        *b_tpc_startX;   //!
   TBranch        *b_tpc_endX;   //!
   TBranch        *b_tpc_track_fitresult_yx;   //!
   TBranch        *b_tpc_track_fitresult_zx;   //!
   TBranch        *b_Track_Length;   //!
   TBranch        *b_tpc_drift_time_at_pmt_pos;   //!

   dpd(TTree *tree=0);
   virtual ~dpd();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef dpd_cxx
dpd::dpd(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(dpd_file.c_str());
      if (!f || !f->IsOpen()) {
         f = new TFile(dpd_file.c_str());
      }
      f->GetObject("dpd",tree);

   }
   Init(tree);
}

dpd::~dpd()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t dpd::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t dpd::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void dpd::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ev", &ev, &b_ev);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("subrun", &subrun, &b_subrun);
   fChain->SetBranchAddress("nsamples", &nsamples, &b_nsamples);
   fChain->SetBranchAddress("time_charge", &time_charge, &b_time_charge);
   fChain->SetBranchAddress("time_light", &time_light, &b_time_light);
   fChain->SetBranchAddress("crt_matchreco", &crt_matchreco, &b_crt_matchreco);
   fChain->SetBranchAddress("pmt_wvf_properties", pmt_wvf_properties, &b_pmt_wvf_properties);
   fChain->SetBranchAddress("pmt_charge", pmt_charge, &b_pmt_charge);
   fChain->SetBranchAddress("pmt_npe", pmt_npe, &b_pmt_npe);
   fChain->SetBranchAddress("pmt_ped", pmt_ped, &b_pmt_ped);
   fChain->SetBranchAddress("pmt_pedrms", pmt_pedrms, &b_pmt_pedrms);
   fChain->SetBranchAddress("pmt_ped_end", pmt_ped_end, &b_pmt_ped_end);
   fChain->SetBranchAddress("pmt_pedrms_end", pmt_pedrms_end, &b_pmt_pedrms_end);
   fChain->SetBranchAddress("pmt_S1_charge", pmt_S1_charge, &b_pmt_S1_charge);
   fChain->SetBranchAddress("pmt_S1_npe", pmt_S1_npe, &b_pmt_S1_npe);
   fChain->SetBranchAddress("pmt_S1_width", pmt_S1_width, &b_pmt_S1_width);
   fChain->SetBranchAddress("pmt_S1_amp", pmt_S1_amp, &b_pmt_S1_amp);
   fChain->SetBranchAddress("pmt_S1_tau", pmt_S1_tau, &b_pmt_S1_tau);
   fChain->SetBranchAddress("pmt_S2_charge", pmt_S2_charge, &b_pmt_S2_charge);
   fChain->SetBranchAddress("pmt_S2_npe", pmt_S2_npe, &b_pmt_S2_npe);
   fChain->SetBranchAddress("pmt_S2_width", pmt_S2_width, &b_pmt_S2_width);
   fChain->SetBranchAddress("pmt_S2_amp", pmt_S2_amp, &b_pmt_S2_amp);
   fChain->SetBranchAddress("pmt_S2_tau", pmt_S2_tau, &b_pmt_S2_tau);
   fChain->SetBranchAddress("pmt_S2_gaus_charge", pmt_S2_gaus_charge, &b_pmt_S2_gaus_charge);
   fChain->SetBranchAddress("pmt_S2_gaus_npe", pmt_S2_gaus_npe, &b_pmt_S2_gaus_npe);
   fChain->SetBranchAddress("pmt_S2_gaus_width", pmt_S2_gaus_width, &b_pmt_S2_gaus_width);
   fChain->SetBranchAddress("pmt_S2_gaus_amp", pmt_S2_gaus_amp, &b_pmt_S2_gaus_amp);
   fChain->SetBranchAddress("pmt_S2_gaus_tau", pmt_S2_gaus_tau, &b_pmt_S2_gaus_tau);
   fChain->SetBranchAddress("ntracks", &ntracks, &b_ntracks);
   fChain->SetBranchAddress("tpc_totcharge", &tpc_totcharge, &b_tpc_totcharge);
   fChain->SetBranchAddress("tpc_totrecocharge", &tpc_totrecocharge, &b_tpc_totrecocharge);
   fChain->SetBranchAddress("tpc_track_charge", tpc_track_charge, &b_tpc_track_charge);
   fChain->SetBranchAddress("crt_mYX", &crt_mYX, &b_crt_mYX);
   fChain->SetBranchAddress("crt_mZX", &crt_mZX, &b_crt_mZX);
   fChain->SetBranchAddress("crt_nYX", &crt_nYX, &b_crt_nYX);
   fChain->SetBranchAddress("crt_nZX", &crt_nZX, &b_crt_nZX);
   fChain->SetBranchAddress("tpc_mYX", tpc_mYX, &b_tpc_mYX);
   fChain->SetBranchAddress("tpc_mZX", tpc_mZX, &b_tpc_mZX);
   fChain->SetBranchAddress("tpc_nYX", tpc_nYX, &b_tpc_nYX);
   fChain->SetBranchAddress("tpc_nZX", tpc_nZX, &b_tpc_nZX);
   fChain->SetBranchAddress("tpc_startX", tpc_startX, &b_tpc_startX);
   fChain->SetBranchAddress("tpc_endX", tpc_endX, &b_tpc_endX);
   fChain->SetBranchAddress("tpc_track_fitresult_yx", tpc_track_fitresult_yx, &b_tpc_track_fitresult_yx);
   fChain->SetBranchAddress("tpc_track_fitresult_zx", tpc_track_fitresult_zx, &b_tpc_track_fitresult_zx);
   fChain->SetBranchAddress("Track_Length", Track_Length, &b_Track_Length);
   fChain->SetBranchAddress("tpc_drift_time_at_pmt_pos", tpc_drift_time_at_pmt_pos, &b_tpc_drift_time_at_pmt_pos);
   Notify();
}

Bool_t dpd::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void dpd::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t dpd::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef dpd_cxx
