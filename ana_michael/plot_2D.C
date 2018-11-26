#include "TFile.h"
#include "TROOT.h"

#include "../config_analysis.h"

TString plotdir="plots/2D";

bool remake_histos=true;

const string fname_2D = "2D.root";

int nbins_charge=70;
float min_charge=0.0;
float max_charge=7.0;
float scale_charge=1.E-6;
TString tit_charge="Tot charge";
TString axis_charge="Charge [10^{6} ADC*ticks]";

int nbins_recocharge=100;
float min_recocharge=0.0;
float max_recocharge=1.0;
float scale_recocharge=1.E-6;
TString tit_recocharge="Tot reco charge";
TString axis_recocharge="Reco charge [10^{6} ADC*ticks]";

int nbins_N=81;
float min_N=-0.5;
float max_N=80.5;
float scale_N=1.;
TString tit_N="N tracks";
TString axis_N="N_{tracks}";

int nbins_light=100;
float min_light=0.0;
float max_light=1.0;
float scale_light=1.E6;
TString tit_light="Tot PMT light";
TString axis_light="Tot light [#muC]";

int nbins_S2light=120;
float min_S2light=0.0;
float max_S2light=1.2;
float scale_S2light=1.E6;
TString tit_S2light="S2 PMT light";
TString axis_S2light="S2 corrected light [#muC]";

int nbins_npe=60;
float min_npe=0.0;
float max_npe=6.0;
float scale_npe=1.E-6;
TString tit_npe="S2 n.p.e.";
TString axis_npe="PMT S2 corrected n.p.e. (x10^{6})";


void make_histos(TTree* dpd)
{
	
    TH2F* h_charge_light = new TH2F("h_charge_light",Form("%s vs. %s;%s;%s",tit_light.Data(),tit_charge.Data(),axis_charge.Data(),axis_light.Data()),nbins_charge,min_charge,max_charge,nbins_light,min_light,max_light);
    TH2F* h_recocharge_light = new TH2F("h_recocharge_light",Form("%s vs. %s;%s;%s",tit_light.Data(),tit_recocharge.Data(),axis_recocharge.Data(),axis_light.Data()),nbins_recocharge,min_recocharge,max_recocharge,nbins_light,min_light,max_light);
    TH2F* h_tracks_light = new TH2F("h_tracks_light",Form("%s vs. %s;%s;%s",tit_light.Data(),tit_N.Data(),axis_N.Data(),axis_light.Data()),nbins_N,min_N,max_N,nbins_light,min_light,max_light); 
  
    TH2F* h_charge_S2light = new TH2F("h_charge_S2light",Form("%s vs. %s;%s;%s",tit_S2light.Data(),tit_charge.Data(),axis_charge.Data(),axis_S2light.Data()),nbins_charge,min_charge,max_charge,nbins_S2light,min_S2light,max_S2light);
    TH2F* h_recocharge_S2light = new TH2F("h_recocharge_S2light",Form("%s vs. %s;%s;%s",tit_S2light.Data(),tit_recocharge.Data(),axis_recocharge.Data(),axis_S2light.Data()),nbins_recocharge,min_recocharge,max_recocharge,nbins_S2light,min_S2light,max_S2light);
    TH2F* h_tracks_S2light = new TH2F("h_tracks_S2light",Form("%s vs. %s;%s;%s",tit_S2light.Data(),tit_N.Data(),axis_N.Data(),axis_S2light.Data()),nbins_N,min_N,max_N,nbins_S2light,min_S2light,max_S2light); 
  
    TH2F* h_charge_S2npe = new TH2F("h_charge_S2npe",Form("%s vs. %s;%s;%s",tit_npe.Data(),tit_charge.Data(),axis_charge.Data(),axis_npe.Data()),nbins_charge,min_charge,max_charge,nbins_npe,min_npe,max_npe);
    TH2F* h_recocharge_S2npe = new TH2F("h_recocharge_S2npe",Form("%s vs. %s;%s;%s",tit_npe.Data(),tit_recocharge.Data(),axis_recocharge.Data(),axis_npe.Data()),nbins_recocharge,min_recocharge,max_recocharge,nbins_npe,min_npe,max_npe);
  
    TH2F* h_S1S2_charge = new TH2F("h_S1S2_charge","S2 pmt_charge vs. S1 pmt_charge;S1 charge [nC];S2 charge [#muC]",100,0,5,80,0,0.4);
    TH2F* h_S1S2_npe = new TH2F("h_S1S2_npe","S2 n.p.e. vs. S1 n.p.e.;S1 n.p.e. (x10^{3});S2 n.p.e. (x10^{6})",100,0,20,100,0,2.0);
    TH2F* h_S1S2_width = new TH2F("h_S1S2_width","S2 width vs. S1 width;S1 width [ns];S2 width [#mus]",120,0,120,175,0,350);
    TH2F* h_S1S2_amp = new TH2F("h_S1S2_amp","S2 amp vs. S1 amp;S1 amp [V];S2 amp [V]",100,0,2.01,90,0,0.09);
  
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
	
	TString cut_sum = Form("Iteration$!=%d",N_PMT);
	TString myCuts = cuts+" && "+cut_sum;
	
	/*
	dpd->Draw(Form("pmt_S2_charge*%f:pmt_S1_charge_4us*1.E9>>h_S1S2_charge",scale_light),cut_sum,"colz");
	dpd->Draw(Form("pmt_S2_npe*%f:pmt_S1_npe_4us*1.E-3>>h_S1S2_npe",scale_npe),cut_sum,"colz");
	dpd->Draw("pmt_S2_width:pmt_S1_width>>h_S1S2_width","","colz");
	dpd->Draw("pmt_S2_amp:pmt_S1_amp>>h_S1S2_amp","","colz");
	
	dpd->Draw(Form("pmt_charge[5]*%f:tpc_totcharge*%f>>h_charge_light",scale_light,scale_charge),"","colz");
	dpd->Draw(Form("pmt_charge[5]*%f:tpc_totrecocharge*%f>>h_recocharge_light",scale_light,scale_recocharge),"","colz");
	dpd->Draw(Form("pmt_charge[5]*%f:ntracks*%f>>h_tracks_light",scale_light,scale_N),"","colz");
	dpd->Draw(Form("pmt_S2_charge[5]*%f:tpc_totcharge*%f>>h_charge_S2light",scale_light,scale_charge),"","colz");
	dpd->Draw(Form("pmt_S2_charge[5]*%f:tpc_totrecocharge*%f>>h_recocharge_S2light",scale_light,scale_recocharge),"","colz");
	dpd->Draw(Form("pmt_S2_charge[5]*%f:ntracks*%f>>h_tracks_S2light",scale_S2light,scale_N),"","colz");
	dpd->Draw(Form("pmt_S2_npe[5]*%f:tpc_totcharge*%f>>h_charge_S2npe",scale_npe,scale_charge),"","colz");
	dpd->Draw(Form("pmt_S2_npe[5]*%f:tpc_totrecocharge*%f>>h_recocharge_S2npe",scale_npe,scale_recocharge),"","colz");
	
	dpd->Draw(Form("pmt_S2_charge*%f:pmt_S1_charge_4us*1.E9>>h_S1S2_charge_cut",scale_light),myCuts,"colz");
	dpd->Draw(Form("pmt_S2_npe*%f:pmt_S1_npe_4us*1.E-3>>h_S1S2_npe_cut",scale_npe),myCuts,"colz");
	dpd->Draw("pmt_S2_width:pmt_S1_width>>h_S1S2_width_cut",cuts,"colz");
	dpd->Draw("pmt_S2_amp:pmt_S1_amp>>h_S1S2_amp_cut",cuts,"colz");
	
	dpd->Draw(Form("pmt_charge[5]*%f:tpc_totcharge*%f>>h_charge_light_cut",scale_light,scale_charge),cuts,"colz");
	dpd->Draw(Form("pmt_charge[5]*%f:tpc_totrecocharge*%f>>h_recocharge_light_cut",scale_light,scale_recocharge),cuts,"colz");
	dpd->Draw(Form("pmt_charge[5]*%f:ntracks*%f>>h_tracks_light_cut",scale_light,scale_N),cuts,"colz");
	dpd->Draw(Form("pmt_S2_charge[5]*%f:tpc_totcharge*%f>>h_charge_S2light_cut",scale_light,scale_charge),cuts,"colz");
	cout << "cuts 2 = " << cuts << endl;
	dpd->Draw(Form("pmt_S2_charge[5]*%f:tpc_totrecocharge*%f>>h_recocharge_S2light_cut",scale_light,scale_recocharge),cuts,"colz");
	dpd->Draw(Form("pmt_S2_charge[5]*%f:ntracks*%f>>h_tracks_S2light_cut",scale_S2light,scale_N),cuts,"colz");
	dpd->Draw(Form("pmt_S2_npe[5]*%f:tpc_totcharge*%f>>h_charge_S2npe_cut",scale_npe,scale_charge),cuts,"colz");
	dpd->Draw(Form("pmt_S2_npe[5]*%f:tpc_totrecocharge*%f>>h_recocharge_S2npe_cut",scale_npe,scale_recocharge),cuts,"colz");
	*/
	
	dpd->Draw(Form("pmt_S2_charge*%f:pmt_S1_charge_4us*1.E9>>h_S1S2_charge",scale_light),cut_sum,"colz");
	dpd->Draw(Form("pmt_S2_npe*%f:pmt_S1_npe_4us*1.E-3>>h_S1S2_npe",scale_npe),cut_sum,"colz");
	dpd->Draw("pmt_S2_width:pmt_S1_width>>h_S1S2_width","","colz");
	dpd->Draw("pmt_S2_amp:pmt_S1_amp>>h_S1S2_amp","","colz");
	
	dpd->Draw(Form("(pmt_charge[0]+pmt_charge[1]+pmt_charge[4])*%f:tpc_totcharge*%f>>h_charge_light",scale_light,scale_charge),"","colz");
	dpd->Draw(Form("(pmt_charge[0]+pmt_charge[1]+pmt_charge[4])*%f:tpc_totrecocharge*%f>>h_recocharge_light",scale_light,scale_recocharge),"","colz");
	dpd->Draw(Form("(pmt_charge[0]+pmt_charge[1]+pmt_charge[4])*%f:ntracks*%f>>h_tracks_light",scale_light,scale_N),"","colz");
	dpd->Draw(Form("pmt_S2_charge_corr[5]*%f:tpc_totcharge*%f>>h_charge_S2light",scale_light,scale_charge),"","colz");
	dpd->Draw(Form("pmt_S2_charge[5]*%f:tpc_totrecocharge*%f>>h_recocharge_S2light",scale_light,scale_recocharge),"","colz");
	dpd->Draw(Form("pmt_S2_charge[5]*%f:ntracks*%f>>h_tracks_S2light",scale_S2light,scale_N),"","colz");
	dpd->Draw(Form("pmt_S2_npe_corr[5]*%f:tpc_totcharge*%f>>h_charge_S2npe",scale_npe,scale_charge),"","colz");
	dpd->Draw(Form("pmt_S2_npe_corr[5]*%f:tpc_totrecocharge*%f>>h_recocharge_S2npe",scale_npe,scale_recocharge),"","colz");
	
	dpd->Draw(Form("pmt_S2_charge*%f:pmt_S1_charge_4us*1.E9>>h_S1S2_charge_cut",scale_light),myCuts,"colz");
	dpd->Draw(Form("pmt_S2_npe*%f:pmt_S1_npe_4us*1.E-3>>h_S1S2_npe_cut",scale_npe),myCuts,"colz");
	dpd->Draw("pmt_S2_width:pmt_S1_width>>h_S1S2_width_cut",cuts,"colz");
	dpd->Draw("pmt_S2_amp:pmt_S1_amp>>h_S1S2_amp_cut",cuts,"colz");
	
	dpd->Draw(Form("pmt_S2_charge_corr[5]*%f:tpc_totcharge*%f>>h_charge_light_cut",scale_light,scale_charge),cuts,"colz");
	dpd->Draw(Form("pmt_S2_charge_corr[5]*%f:tpc_totrecocharge*%f>>h_recocharge_light_cut",scale_light,scale_recocharge),cuts,"colz");
	dpd->Draw(Form("pmt_S2_charge_corr[5]*%f:ntracks*%f>>h_tracks_light_cut",scale_light,scale_N),cuts,"colz");
	dpd->Draw(Form("pmt_S2_charge_corr[5]*%f:tpc_totcharge*%f>>h_charge_S2light_cut",scale_light,scale_charge),cuts,"colz");
	cout << "cuts 2 = " << cuts << endl;
	dpd->Draw(Form("pmt_S2_charge_corr[5]*%f:tpc_totrecocharge*%f>>h_recocharge_S2light_cut",scale_light,scale_recocharge),cuts,"colz");
	dpd->Draw(Form("pmt_S2_charge_corr[5]*%f:ntracks*%f>>h_tracks_S2light_cut",scale_S2light,scale_N),cuts,"colz");
	dpd->Draw(Form("pmt_S2_npe_corr[5]*%f:tpc_totcharge*%f>>h_charge_S2npe_cut",scale_npe,scale_charge),cuts,"colz");
	dpd->Draw(Form("pmt_S2_npe_corr[5]*%f:tpc_totrecocharge*%f>>h_recocharge_S2npe_cut",scale_npe,scale_recocharge),cuts,"colz");
	
  	
	TFile *f = new TFile(fname_2D.c_str(),"RECREATE");
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

void plot_2D()
{
	gSystem->Exec(Form("mkdir -p %s",plotdir.Data()));
	
	build_cuts();
	
	SetMyStyle(true);
	
	TCanvas *c1 = new TCanvas;
	
	if (remake_histos)
	{
		TFile* f = TFile::Open(dpd_file.c_str());
		TTree* dpd = (TTree*)f->Get("dpd");
		
		make_histos(dpd);
	}
	else cout << "WARNING: remake_histos set to FALSE" << endl << endl;
	
	TFile f_2d(fname_2D.c_str());
	
    TH2 *h;
    TKey *key;
    TIter nextkey(gDirectory->GetListOfKeys());
    while ((key=(TKey*)nextkey())) 
    {
      TObject *obj=key->ReadObj();
      if (obj->IsA()->InheritsFrom("TH2")) 
  	{
  		h = (TH2*)obj;
  		TString name=h->GetName();
		  
		  c1->Clear();
		  c1->cd();
		  h->Draw("colz");
		  
		  TLatex l;
		  l.SetNDC();
		  l.DrawLatex(0.19,0.88,Form("N_{entries} = %0.0f",h->GetEntries()));
		  
		  if (0 && name=="h_recocharge_S2light")
		  {
			  TF2* fit = new TF2("fit","xygaus");
			  h->Fit(fit);
		  }
		  c1->SetLogz();
		  c1->RedrawAxis();
		  c1->Print(plotdir+"/2D_"+name+".pdf");				  	
	  }
  }
}