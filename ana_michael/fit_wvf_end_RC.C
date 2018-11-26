#include "../config_analysis.h"
#include "../../Light_reco/LIB/pause.h"

TString plotdir = "plots/wvf_end_RC";

TH1D* fitPMT(TTree* t, TString name, int p)
{	
	float xmin=10.;
	float xmax=1000;
	
	TString pmt = Form("%s[%d]",name.Data(),p);
	TString limits = cut_pedrms_pmt[p]+" && "+cut_S1_tau_pmt[p]+" && "+cut_S1_amp_pmt[p]+" && "+
		Form("pmt_wvf_end_fit_chi2[%d]>0. && pmt_wvf_end_fit_chi2[%d]<1.0",p,p)+" && "+
		Form("pmt_wvf_ADC_sat[%d]==0",p); // +" && "+Form("pmt_S2_amp[%d]<1.1",p);

	t->Draw(Form("%s>>hist(100,%e,%e)",pmt.Data(),xmin,xmax),limits);
	
	TH1D* hist = (TH1D*)gDirectory->Get("hist");
	
	float max = hist->GetMaximum();
	float cen = hist->GetBinCenter(hist->GetMaximumBin());
	
	/*
	TF1 *f1 = new TF1("f1","[0]*TMath::Landau(x,[1],[2])",0,xmax+1.);
	f1->SetParameter(0,max);
	f1->SetParameter(1,cen);
	f1->SetParameter(2,1);
	hist->Fit(f1,"MR+","",0,xmax);
	f1->SetLineColor(colors[1]);
	*/
	
	TF1 *f1 = new TF1("f1","[0]+gaus(1)",0,xmax+1.);
	f1->SetParameter(1,max);
	f1->SetParameter(2,cen);
	f1->SetParameter(3,1);
	hist->Fit(f1,"MR+","",0,xmax);
	f1->SetLineColor(colors[1]);
	
	return hist;
}


void fit_wvf_end_RC(const string fname)
{
	gSystem->Exec(Form("mkdir -p %s",plotdir.Data()));
	
	build_cuts();
	
	SetMyStyle();
	
	TCanvas *c1 = new TCanvas;
	
	TFile* f = TFile::Open(fname.c_str());
	TTree* dpd = (TTree*)f->Get("dpd");

	float mean[N_PMT]={0};
	float sigma[N_PMT]={0};
	
	for (int i=2; i<=3; i++)
	{	
		TH1D* h = fitPMT(dpd,"pmt_wvf_end_fit_c2",i);
		
		TF1 *f1 = h->GetFunction("f1");
		mean[i] = f1->GetParameter(2);
		sigma[i] = f1->GetParameter(3);
		f1->SetLineColor(kRed);

		c1->Clear();
		c1->cd();

		h->GetXaxis()->SetTitle("RC_{eff} [#mus]");
		h->GetYaxis()->SetTitle("Entries");
		
		h->Draw();
		f1->Draw("same");
		
		float x = 0.62; // (i==2 || i==3) ? 0.22 : 0.6;
		myTextBold(x,0.8,kBlack,Form("%s",run_desc.Data()));
		myTextBold(x,0.72,kBlack,Form("PMT %d (%s) @ 1100 V",i+1,pmt_base[i].Data()));
		myTextBold(x,0.64,kBlack,Form("#mu_{RC_{eff}} = %0.3f #mus",mean[i]));
		myTextBold(x,0.56,kBlack,Form("#sigma_{RC_{eff}} = %0.3f #mus",sigma[i]));
		
		c1->Modified();
		c1->Update();
		c1->RedrawAxis();
		c1->Print(plotdir+Form("/fit_wvf_end_RC_%d.pdf",i));
		delete h;
	}
}