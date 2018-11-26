#include "../config_analysis.h"

void draw_gains()
{
	SetMyStyle();
	
	TF1* myfits[N_PMT];

	for (int i=0; i<N_PMT; i++)  myfits[i] = new TF1(Form("%s %i","myfit_",i),"1.E-6*pow(10,[0])*pow(x,[1])",1110.,1290.); 
	
	// October 2017
	myfits[0]->SetParameter(0,-26.49);
	myfits[0]->SetParameter(1,10.54);
	myfits[1]->SetParameter(0,-27.96);
	myfits[1]->SetParameter(1,11.03);
	myfits[2]->SetParameter(0,-28.51);
	myfits[2]->SetParameter(1,11.34);	
	myfits[3]->SetParameter(0,-27.72);
	myfits[3]->SetParameter(1,11.12);
	myfits[4]->SetParameter(0,-27.41);
	myfits[4]->SetParameter(1,10.89);
	
	for (int i=0; i<N_PMT; i++) myfits[i]->SetLineColor(colors[i]);

	TCanvas *c1 = new TCanvas("c1","c1");
	
	float x = 0.24;
	float y = 0.5;
	
	TLegend *tleg = new TLegend(x,y,x+0.38,y+0.4);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
    tleg->SetHeader("October 2017 calib."); 
	for (int i=0; i<N_PMT; i++) tleg->AddEntry(myfits[i], Form("PMT %d (%s)",i+1,pmt_base[i].Data()), "l");
	
	c1->cd();
	TH1F* he = new TH1F("he","he",100,1110.,1290.);
	he->GetXaxis()->SetTitle("PMT voltage [V]");
	he->GetYaxis()->SetTitle("Gain (x10^{6})");
	he->SetMinimum(0.2);
	he->SetMaximum(7.5);
	he->Draw();
	tleg->Draw();
	for (int i=0; i<N_PMT; i++) myfits[i]->Draw("same");
	
	c1->RedrawAxis();
	c1->Modified();
	c1->Update();
	
}
	