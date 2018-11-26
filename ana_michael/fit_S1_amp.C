#include "TFile.h"
#include "TROOT.h"

#include "../config_analysis.h"

TString plotdir = "plots/S1_amp";

int nbins_amp=100;
float min_amp=0.0;
float max_amp=2.0;
float scale_npe=1.E-3;
TString axis_npe="PMT S1 n_{PE} (x10^{3})";
TString axis_amp="PMT S1 amplitude [V]";

float hmax[N_PMT]={1.2,1.3,2.0,2.0,1.3};
float ymax[N_PMT]={30,31,40,28,36};

void make_histos(TTree* dpd, TCanvas *c1, int p)
{
	TProfile *hp1  = new TProfile("hp1","hp1",nbins_amp,min_amp,max_amp);
	TProfile *hp4  = (TProfile*)hp1->Clone("hp4");
	TProfile *hpm  = (TProfile*)hp1->Clone("hpm");
	TProfile *hp80 = (TProfile*)hp1->Clone("hp80");
	
	TString myCuts = cut_S1_tau_pmt[p]+" && "+Form("pmt_pedrms[%d]<2.0",p);
	TString myCuts_m2 = myCuts+" && "+cut_S1_delta_tau_pmt[p];
	
	dpd->Draw(Form("%f*pmt_S1_npe_1us[%d]:pmt_S1_amp[%d]>>hp1",scale_npe,p,p),myCuts);
	dpd->Draw(Form("%f*pmt_S1_npe_4us_corr[%d]:pmt_S1_amp[%d]>>hp4",scale_npe,p,p),myCuts);
	dpd->Draw(Form("%f*pmt_S1_npe_m2[%d]:pmt_S1_amp[%d]>>hpm",scale_npe,p,p),myCuts_m2);
	dpd->Draw(Form("%f*pmt_S1_npe_80ns[%d]:pmt_S1_amp[%d]>>hp80",scale_npe,p,p),myCuts);
	
	float fit_range_min=0.2;
	float fit_range_max=(p==2||p==3)?1.0:0.6;
	
	TF1* f1  = new TF1("f1" ,"pol1",min_amp,max_amp);
	TF1* f4  = new TF1("f4" ,"pol1",min_amp,max_amp);
	TF1* fm  = new TF1("fm" ,"pol1",min_amp,max_amp);
	TF1* f80 = new TF1("f80","pol1",min_amp,max_amp);
	
	hp1->Fit(f1,"RMN","",fit_range_min,fit_range_max);
	hp4->Fit(f4,"RMN","",fit_range_min,fit_range_max);
	hpm->Fit(fm,"RMN","",fit_range_min,fit_range_max);
	hp80->Fit(f80,"RMN","",fit_range_min,fit_range_max);
	
	f1->SetLineStyle(2);
	f4->SetLineStyle(2);
	fm->SetLineStyle(2);
	f80->SetLineStyle(2);
	
	f1->SetLineWidth(1);
	f4->SetLineWidth(1);
	fm->SetLineWidth(1);
	f80->SetLineWidth(1);
	
	
	f1->SetLineColor(colors[0]);
	f4->SetLineColor(colors[1]);
	fm->SetLineColor(colors[2]);
	f80->SetLineColor(colors[3]);
	
	hp1->SetMarkerColor(colors[0]);
	hp4->SetMarkerColor(colors[1]);
	hpm->SetMarkerColor(colors[2]);
	hp80->SetMarkerColor(colors[3]);
	
	hp1->SetMarkerStyle(20);
	hp4->SetMarkerStyle(21);
	hpm->SetMarkerStyle(31);
	hp80->SetMarkerStyle(22);
	
	hp1->SetLineColor(colors[0]);
	hp4->SetLineColor(colors[1]);
	hpm->SetLineColor(colors[2]);
	hp80->SetLineColor(colors[3]);
	
	hp1->SetMarkerSize(0.6);
	hp4->SetMarkerSize(0.6);
	hpm->SetMarkerSize(0.6);
	hp80->SetMarkerSize(0.6);

	c1->Clear();
  	c1->cd();
	
	TH1F* he = new TH1F("he","he",10,min_amp,hmax[p]);
	he->GetXaxis()->SetTitle(axis_amp);
	he->GetYaxis()->SetTitle(axis_npe);
	he->SetMaximum(ymax[p]);
	he->Draw();

	float x = 0.26;
	float y = 0.64;
	TLegend *tleg = new TLegend(x,y,x+0.26,y+0.26);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
    tleg->SetHeader(Form("PMT %d (%s)",p+1,pmt_base[p].Data())); 
	tleg->AddEntry(hp1, "1 #mus","lp");
	tleg->AddEntry(hp4, "4 #mus","lp");
	tleg->AddEntry(hpm, "m2", "lp");
	tleg->AddEntry(hp80,"80 ns", "lp");
	tleg->Draw();

	hp4->Draw("same");
	hp1->Draw("same");
	hpm->Draw("same");
	hp80->Draw("same");
	
	f1->Draw("same");
	fm->Draw("same");
	f4->Draw("same");
	f80->Draw("same");
	

	c1->Modified();
	c1->Update();
	
	TLine *l1=new TLine(0.122,gPad->GetUymin(),0.122,gPad->GetUymax());
	l1->SetLineStyle(2);
	l1->Draw("same");
	
	c1->RedrawAxis();
	c1->Print(plotdir+Form("/profile_S1_amp_npe_ch%d.pdf",p)); 	
	
	delete hp1;
	delete hp4;
	delete hpm;
	delete hp80;
	delete he;		
}

void fit_S1_amp()
{
	gSystem->Exec(Form("mkdir -p %s", plotdir.Data()));
	
	build_cuts();
	
	SetMyStyle();
	
	TCanvas *c1 = new TCanvas;
	
	TFile* f = TFile::Open(dpd_file.c_str());
	TTree* dpd = (TTree*)f->Get("dpd");
	
	for (int i=0; i<N_PMT; i++)
	{
		make_histos(dpd,c1,i);
	}	
}