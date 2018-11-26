#include "../config_analysis.h"

int msty[] = {20,21,22,23,33};
	
void plotTimeProfile(void)
{
	gSystem->Exec("mkdir -p plots");
	
	SetMyStyle();
	
	build_cuts();
	
	TFile* f = TFile::Open(dpd_file.c_str());
	
	TTree* dpd = (TTree*)f->Get("dpd");
	
	double scale_S1=1.E9;
	double scale_time=1./60./60.; // convert to hours
	int timemax = 72000/60/60;
	int ntimebins = timemax*2; // half-hour bins
	
	TProfile *hp[N_PMT];	
	for (int p=0; p<N_PMT; p++)
	{
		hp[p] = new TProfile(Form("hp%d",p),Form("hp%d",p),ntimebins,0,timemax,"s");

		TString myCuts = "(run_charge==840 || run_charge==842) && "+cut_npeaks_pmt[p]+" && "+cut_pedrms_pmt[p]+
			" && "+cut_S1_tau_pmt[p]+" && "+cut_S1_amp_pmt[p]+" && "+Form("pmt_wvf_ADC_sat[%d]==0",p);
		
		dpd->Draw(Form("%f*pmt_S1_charge_4us[%d]:%f*(time_charge-1501091406)>>hp%d",scale_S1,p,scale_time,p),myCuts);
		
		hp[p]->SetLineColor(colors[p]);
		hp[p]->SetMarkerColor(colors[p]);
		hp[p]->SetMarkerStyle(msty[p]);
		hp[p]->SetMarkerSize(0.6);
	}
	
	TCanvas *c1 = new TCanvas;
	
	c1->Clear();
  	c1->cd();
	
	TH1F* he = new TH1F("he","he",10,0,20);
	he->GetXaxis()->SetTitle("Time [hr]");
	he->GetYaxis()->SetTitle("PMT S1 charge 4 us [#muC]");
	he->SetMinimum(0.2); // 0.8
	he->SetMaximum(2.2); // 3.5
	he->Draw();

	float x = 0.5;
	float y = 0.55;
	TLegend *tleg = new TLegend(x,y,x+0.3,y+0.32);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
	tleg->SetHeader("Charge runs 840, 842"); 
    //tleg->SetHeader(Form("PMT %d (%s)",p+1,pmt_base[p].Data())); 
	for (int p=0; p<N_PMT; p++) 
	{
		hp[p]->Draw("same");
		tleg->AddEntry(hp[p], Form("PMT %d (%s)",p+1,pmt_base[p].Data()), "lp");
	}
	tleg->Draw();

	c1->Modified();
	c1->Update();
	
}