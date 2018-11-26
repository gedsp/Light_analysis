#include "../config_analysis.h"

TString plotdir = "plots/S2_charge";

TString cut_zero[N_PMT];
TString myCuts[N_PMT];

void plotPed(TTree*t, TCanvas *c, int ind)
{
	THStack* hsped= new THStack(Form("hsped%d",ind),Form("pmt%d",ind));
	hsped->Clear();
	
	TH1D *hped, *hpedC;
	
	double scaley = 1.E3;
	float xmin=-20.;
	float xmax=15.;

	t->Draw(Form("pmt_ped_end_corr[%d]>>hped%d(120,%f,%f)",ind,ind,xmin,xmax),cut_zero[ind]);
	t->Draw(Form("pmt_ped_end_corr[%d]>>hpedC%d(120,%f,%f)",ind,ind,xmin,xmax),myCuts[ind]);
	
	hped = (TH1D*)gDirectory->Get(Form("hped%d",ind)); 
	hpedC = (TH1D*)gDirectory->Get(Form("hpedC%d",ind)); 
	
	hped->Scale(1./scaley);
	hpedC->Scale(1./scaley);
	
	TF1* fit = new TF1("fit","gaus",-30,30);
	hpedC->Fit(fit);
	fit->SetLineColor(kRed);
	fit->SetLineStyle(2);
	
	hped->SetLineColor(kGreen+2);
	hpedC->SetLineColor(kGreen+2);
	hpedC->SetLineStyle(2);
	
	hsped->Add(hped);
	hsped->Add(hpedC);
	
	c->Clear();
	c->cd();
	c->SetLogy(0);
	
	hsped->Draw("hist.nostack");
	
	float x = 0.18;
	float y = 0.60;
	TLegend *tleg = new TLegend(x,y,x+0.43,y+0.3);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
    tleg->SetHeader(Form("%s, PMT %d",run_desc.Data(),ind+1)); 
	tleg->AddEntry(hped,"corrected, no cuts","l");
	tleg->AddEntry(hpedC,"corrected, with cuts","l");
	
	hsped->GetYaxis()->SetTitle("Entries (x1000)");
	hsped->GetXaxis()->SetTitle("#delta ped_end [ADC]"); 
    tleg->Draw();
	c->RedrawAxis();
	hsped->Draw("hist.nostack.same");
	fit->Draw("same");
	
	c->Modified();
	c->Update();
	
	c->Print(plotdir+Form("/ped_end_corr%d.pdf",ind));	
	
}

void plotPMT(TTree* t, TCanvas *c, int ind)
{
	THStack* hs = new THStack(Form("hs%d",ind),Form("pmt%d",ind));
	hs->Clear();
	
	TH1D *h,*hC,*hP,*hM;
	
	double scale = 1.E-6;
	double scaley = 1.E3;
	
	t->Draw(Form("pmt_S2_charge[%d]/%f>>h%d",ind,scale,ind),cut_zero[ind]);
	t->Draw(Form("pmt_S2_charge_corr[%d]/%f>>hC%d",ind,scale,ind),cut_zero[ind]);
	t->Draw(Form("pmt_S2_charge_corr_p[%d]/%f>>hP%d",ind,scale,ind),cut_zero[ind]);
	t->Draw(Form("pmt_S2_charge_corr_m[%d]/%f>>hM%d",ind,scale,ind),cut_zero[ind]);
	
	h = (TH1D*)gDirectory->Get(Form("h%d",ind));
	hC = (TH1D*)gDirectory->Get(Form("hC%d",ind));
	hP = (TH1D*)gDirectory->Get(Form("hP%d",ind));
	hM = (TH1D*)gDirectory->Get(Form("hM%d",ind));
	
	h->Scale(1./scaley);
	hC->Scale(1./scaley);
	hP->Scale(1./scaley);
	hM->Scale(1./scaley);
	
	h->SetLineColor(kBlack);
	hC->SetLineColor(kGreen+2);
	hP->SetLineColor(kRed);
	hM->SetLineColor(kBlue);
	
	hs->Add(h);
	hs->Add(hC);
	hs->Add(hP);
	hs->Add(hM);
	
	c->Clear();
	c->cd();
	
	hs->Draw("hist.nostack");
	c->SetLogy(1);
	
	float x = 0.60;
	float y = 0.48;
	TLegend *tleg = new TLegend(x,y,x+0.43,y+0.42);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
    tleg->SetHeader(Form("%s, PMT %d",run_desc.Data(),ind+1)); 
	tleg->AddEntry(h,"uncorrected","l");
	tleg->AddEntry(hC,"corrected","l");
	tleg->AddEntry(hP,"corr_p","l");
	tleg->AddEntry(hM,"corr_m","l");
	
	hs->GetYaxis()->SetTitle("Entries (x1000)");
	hs->GetXaxis()->SetTitle("PMT S2 charge [#muC]"); 
    tleg->Draw();
	c->RedrawAxis();
	hs->Draw("hist.nostack.same");
	
	c->Modified();
	c->Update();
	
	c->Print(plotdir+Form("/S2_charge_%d.pdf",ind));
	
}

void plotPMT_delta(TTree* t, TCanvas *c, int ind)
{
	THStack* hsdelta = new THStack(Form("hsdelta%d",ind),Form("pmt%d",ind));
	hsdelta->Clear();
	
	double scaley = 1.E3;
	
	TH1D *hdeltaP, *hdeltaM;
	
	t->Draw(Form("(pmt_S2_charge_corr[%d]-pmt_S2_charge[%d])/pmt_S2_charge[%d]>>hdelta%d",ind,ind,ind,ind),cut_zero[ind]);
	t->Draw(Form("(pmt_S2_charge_corr_m[%d]-pmt_S2_charge[%d])/pmt_S2_charge[%d]>>hdeltaC%d",ind,ind,ind,ind),myCuts[ind]);
	
	hdelta = (TH1D*)gDirectory->Get(Form("hdelta%d",ind)); 
	hdeltaC = (TH1D*)gDirectory->Get(Form("hdeltaC%d",ind)); 
	
	hdelta->Scale(1./scaley);
	hdeltaC->Scale(1./scaley);
	
	hdelta->SetLineColor(kGreen+2);
	hdeltaC->SetLineColor(kGreen+2);
	hdeltaC->SetLineStyle(2);
	
	hsdelta->Add(hdelta);
	hsdelta->Add(hdeltaC);
	
	c->Clear();
	c->cd();
	
	hsdelta->Draw("hist.nostack");
	
	float x = 0.55;
	float y = 0.6;
	TLegend *tleg = new TLegend(x,y,x+0.43,y+0.32);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
    tleg->SetHeader(Form("%s, PMT %d",run_desc.Data(),ind+1)); 
	tleg->AddEntry(hdelta,"corrected, no cuts","l");
	tleg->AddEntry(hdeltaC,"corrected, with cuts","l");
	
	hsdelta->GetYaxis()->SetTitle("Entries (x1000)");
	hsdelta->GetXaxis()->SetTitle("#delta S2 charge / S2 charge"); 
    tleg->Draw();
	c->RedrawAxis();
	hsdelta->Draw("hist.nostack.same");
	
	c->Modified();
	c->Update();
	
	c->Print(plotdir+Form("/S2_delta_charge_%d.pdf",ind));	
	
}

void plotPMT_delta2(TTree* t, TCanvas *c, int ind)
{
	THStack* hsdelta2 = new THStack(Form("hsdelta2%d",ind),Form("pmt%d",ind));
	hsdelta2->Clear();
	
	double scaley = 1.E3;
	
	TH1D *hdelta2P, *hdelta2M, *hdelta2Pc, *hdelta2Mc;
	
	t->Draw(Form("(pmt_S2_charge_corr_p[%d]-pmt_S2_charge_corr[%d])/pmt_S2_charge_corr[%d]>>hdelta2P%d",ind,ind,ind,ind),cut_zero[ind]);
	t->Draw(Form("(pmt_S2_charge_corr_m[%d]-pmt_S2_charge_corr[%d])/pmt_S2_charge_corr[%d]>>hdelta2M%d",ind,ind,ind,ind),cut_zero[ind]);
	t->Draw(Form("(pmt_S2_charge_corr_p[%d]-pmt_S2_charge_corr[%d])/pmt_S2_charge_corr[%d]>>hdelta2Pc%d",ind,ind,ind,ind),myCuts[ind]);
	t->Draw(Form("(pmt_S2_charge_corr_m[%d]-pmt_S2_charge_corr[%d])/pmt_S2_charge_corr[%d]>>hdelta2Mc%d",ind,ind,ind,ind),myCuts[ind]);
	
	hdelta2P = (TH1D*)gDirectory->Get(Form("hdelta2P%d",ind)); 
	hdelta2M = (TH1D*)gDirectory->Get(Form("hdelta2M%d",ind)); 
	hdelta2Pc = (TH1D*)gDirectory->Get(Form("hdelta2Pc%d",ind)); 
	hdelta2Mc = (TH1D*)gDirectory->Get(Form("hdelta2Mc%d",ind)); 
	
	hdelta2P->Scale(1./scaley);
	hdelta2M->Scale(1./scaley);
	hdelta2Pc->Scale(1./scaley);
	hdelta2Mc->Scale(1./scaley);
	
	hdelta2P->SetLineColor(kRed);
	hdelta2M->SetLineColor(kBlue);
	hdelta2Pc->SetLineColor(kRed);
	hdelta2Mc->SetLineColor(kBlue);
	hdelta2Pc->SetLineStyle(2);
	hdelta2Mc->SetLineStyle(2);
	
	hdelta2P->GetXaxis()->SetRangeUser(-0.3,0.8);
	
	hsdelta2->Add(hdelta2P);
	hsdelta2->Add(hdelta2M);
	hsdelta2->Add(hdelta2Pc);
	hsdelta2->Add(hdelta2Mc);
	
	c->Clear();
	c->cd();
	c->SetLogy(0);
	
	hsdelta2->Draw("hist.nostack");
	
	hsdelta2->GetXaxis()->SetLimits(-0.3,0.08);
	hsdelta2->Draw("hist.nostack");
	
	float x = 0.21;
	float y = 0.5;
	TLegend *tleg = new TLegend(x,y,x+0.38,y+0.42);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
    tleg->SetHeader(Form("%s, PMT %d",run_desc.Data(),ind+1)); 
	tleg->AddEntry(hdelta2P,"corr P, no cuts","l");
	tleg->AddEntry(hdelta2Pc,"corr P, with cuts","l");
	tleg->AddEntry(hdelta2M,"corr M, no cuts","l");
	tleg->AddEntry(hdelta2Mc,"corr M, with cuts","l");
	
	hsdelta2->GetYaxis()->SetTitle("Entries (x1000)");
	hsdelta2->GetXaxis()->SetTitle("#delta2 S2 corrected charge / S2 corrected charge"); 
    tleg->Draw();
	c->RedrawAxis();
	hsdelta2->Draw("hist.nostack.same");
	
	c->Modified();
	c->Update();
	
	c->Print(plotdir+Form("/S2_delta_corr_charge_%d.pdf",ind));
	
}

void plotPedCharge(TTree* t, TCanvas *c, int ind, bool doCuts)
{
	TH2D* h2d;
	
	double scale = 1.E-6;
	
	TString myCuts2d = cut_zero[ind]+" && "+Form("fabs(pmt_ped_end_corr[%d])<50.",ind);
	if (doCuts) myCuts2d=myCuts2d+" && "+myCuts[ind];
	
	t->Draw(Form("pmt_ped_end_corr[%d]:pmt_S2_charge_corr[%d]/%f>>h2d%d",ind,ind,scale,ind),myCuts2d,"colz");
	
	h2d = (TH2D*)gDirectory->Get(Form("h2d%d",ind)); 

	c->Clear();
	c->cd();
	
	h2d->GetXaxis()->SetTitle("S2 corrected charge [#muC]");
	h2d->GetYaxis()->SetTitle("Corrected ped_end [ADC]");
	
	h2d->Draw("colz");
	c->SetLogz(1);
	
  	TLatex l;
  	l.SetNDC();
	l.DrawLatex(0.5,0.86,Form("%s, PMT %d",run_desc.Data(),ind+1)); 
  	l.DrawLatex(0.5,0.78,Form("N_{entries} = %0.0f",h2d->GetEntries()));
	
	if (doCuts) c->Print(plotdir+Form("/ped_end_vs_S2_corr_charge_cuts_%d.pdf",ind));
	else c->Print(plotdir+Form("/ped_end_vs_S2_corr_charge_%d.pdf",ind));
}


void plot_S2_charge_corr(void)
{
	gSystem->Exec(Form("mkdir -p %s",plotdir.Data()));
	
	SetMyStyle();
	
	build_cuts();
	
	TFile* f = TFile::Open(dpd_file.c_str());
	
	TTree* dpd = (TTree*)f->Get("dpd");
	
	TCanvas *c1 = new TCanvas;
	
	for (int i=2; i<=3; i++)
	{
		cut_zero[i] = Form("pmt_S2_charge[%d]!=0",i);
		myCuts[i] = cut_zero[i]+" && "+cut_npeaks_pmt[i]+" && "+cut_pedrms_pmt[i]+" && "+cut_S1_tau_pmt[i]+" && "+cut_S1_amp_pmt[i]+" && "+Form("pmt_wvf_ADC_sat[%d]==0",i);
	}
	
	for (int i=2; i<=3; i++) // positive-base PMTs only
	{		
		plotPed(dpd,c1,i);
		plotPMT(dpd,c1,i); 
		plotPMT_delta(dpd,c1,i); 
		plotPMT_delta2(dpd,c1,i); 
	}
	
	SetMyStyle(true);
	TCanvas *c2 = new TCanvas;

	for (int i=2; i<=3; i++)
	{
		plotPedCharge(dpd,c2,i,false);
		plotPedCharge(dpd,c2,i,true);
	}
		
}