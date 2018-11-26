#include "../config_analysis.h"
#include "../../Light_reco/LIB/pause.h"

TString plotdir = "plots/S1_tau";

TH1D* fitPMT(TTree* t, TString name, int p, float tau_nominal, bool do_end)
{	
	float width      = do_end?2.0:0.0065;
	float range_draw = do_end?10.0:0.05;
	float range_fit  = do_end?width*5:width*5.;
	
	TString pmt = Form("%s[%d]",name.Data(),p);
	TString limits = ""; //do_end?Form("(fabs(pmt_S1_tau[%d]-%f)<3.*%f)",p,S1_tau_fit_mean[p],S1_tau_fit_sigma[p]):"";

	//TString limits = (ind==n_names-1) ? "" : Form("(4096.0/2.0)*fabs(pmt_S1_amp[%d])>=12.*pmt_pedrms[%d]",i,i);
	t->Draw(Form("%s>>hist[%d](40,%e,%e)",pmt.Data(),p,tau_nominal-range_draw,tau_nominal+range_draw),limits);
	
	TH1D* hist = (TH1D*)gDirectory->Get(Form("hist[%d]",p));
	hist->Scale(1./1000);
	
	float max = hist->GetMaximum();
	float cen = hist->GetBinCenter(hist->GetMaximumBin());
	
	TF1* fit = new TF1("fit","gaus",cen-2.*range_fit,cen+2.*range_fit);
	fit->SetParameter(0,max);
	fit->SetParameter(1,cen);
	fit->SetParameter(2,width);
	if (!do_end) fit->SetParLimits(1,cen-0.1,cen+0.1);
	//fit->SetParLimits(2,width-0.0005,width+0.002);
	fit->SetLineColor(colors[1]);
	
	hist->Fit(fit,"MR+","",cen-range_fit,cen+range_fit);
	
	return hist;
}


TH1D* fitPMT_diff(TTree* t, int p)
{	
	float xmax = 20.;
	
	TString pmt = Form("pmt_S1_tau_end[%d]-pmt_S1_tau[%d]",p,p);
	TString limits = Form("(fabs(pmt_S1_tau[%d]-%f)<5.*%f)",p,S1_tau_fit_mean[p],S1_tau_fit_sigma[p]);
	//limits = limits+" && "+Form("pmt_pedrms[%d]<2.0",p);

	t->Draw(Form("%s>>hist2[%d](40,%e,%e)",pmt.Data(),p,0.,xmax),limits);

	
	TH1D* hist2 = (TH1D*)gDirectory->Get(Form("hist2[%d]",p));
	hist2->Scale(1./1000);
	
	float max = hist2->GetMaximum();
	float cen = hist2->GetBinCenter(hist2->GetMaximumBin());
	
	//TF1 *f1 = new TF1("f1","[0]*TMath::Poisson(x,[1])",0,xmax+1.);
	//f1->SetParameter(0,max);
	//f1->SetParameter(1,cen);
	
	TF1 *f1 = new TF1("f1","[0]*TMath::Landau(x,[1],[2])",0,xmax+1.);
	f1->SetParameter(0,max);
	f1->SetParameter(1,cen);
	f1->SetParameter(2,1);
	hist2->Fit(f1,"MR+","",0,xmax);
	f1->SetLineColor(colors[1]);

	return hist2;
}

TH1D* fitPMT_low(TTree* t, int p, float tau_nominal)
{	
	TString pmt = Form("pmt_S1_tau[%d]",p);
	TString limits="";
	//TString limits = (ind==n_names-1) ? "" : Form("(4096.0/2.0)*fabs(pmt_S1_amp[%d])>=12.*pmt_pedrms[%d]",i,i);
	t->Draw(Form("%s>>hist[%d](15,%e,%e)",pmt.Data(),p+10,tau_nominal-0.06,tau_nominal+0.06),limits);
	
	TH1D* hist = (TH1D*)gDirectory->Get(Form("hist[%d]",p+10));
	
	float max = hist->GetMaximum();
	float cen = hist->GetBinCenter(hist->GetMaximumBin());
	float width = 0.006;
	
	TF1* fit = new TF1("fit","gaus+[3]",cen-width*10,cen+width*10);
	fit->SetParameter(0,max);
	fit->SetParameter(1,cen);
	fit->SetParameter(2,width);
	fit->SetParLimits(1,cen-0.1,cen+0.1);
	fit->SetLineColor(colors[3]);
	
	hist->Fit(fit,"MR+","",cen-width*5,cen*width*5);
	
	return hist;
}


void fit_S1_tau(void)
{
	gSystem->Exec(Form("mkdir -p %s",plotdir.Data()));
	
	SetMyStyle();
	
	TFile* f = TFile::Open(dpd_file.c_str());
	
	TTree* dpd = (TTree*)f->Get("dpd");
	
	TCanvas *c1 = new TCanvas;
	
	float mean[N_PMT]={0};
	float sigma[N_PMT]={0};
	float mean2[N_PMT]={0};
	float sigma2[N_PMT]={0};
	float mean3[N_PMT]={0};
	float sigma3[N_PMT]={0};
	
	// high S1_tau, nsamples = 262144
	for (int i=0; i<N_PMT; i++)
	{	
		TH1D* h = fitPMT(dpd,"pmt_S1_tau",i,S1_tau_fit_mean[i],false);
		
		TF1 *f1 = h->GetFunction("fit");
		mean[i] = f1->GetParameter(1);
		sigma[i] = f1->GetParameter(2);

		c1->Clear();
		c1->cd();
		TH1D* he = new TH1D("he","he",10,mean[i]-0.06,mean[i]+0.08);
		he->SetMaximum(1.2*h->GetMaximum());
		he->GetXaxis()->SetTitle("PMT S1 #tau [#mus]");
		he->GetYaxis()->SetTitle("Entries (x1000)");
		he->Draw();
		h->Draw("same");
		float x = 0.62; // (i==2 || i==3) ? 0.22 : 0.6;
		myTextBold(x,0.8,kBlack,Form("%s",run_desc.Data()));
		myTextBold(x,0.72,kBlack,Form("PMT %d (%s)",i+1,pmt_base[i].Data()));
		myTextBold(x,0.64,kBlack,Form("#mu_{#tau} = %0.3f #mus",mean[i]));
		myTextBold(x,0.56,kBlack,Form("#sigma_{#tau} = %0.3f #mus",sigma[i]));
		
		//myTextBold(x,0.72,kBlack,Form("Mean = %0.3f #pm %0.3f #mus",mean[i],sigma[i]));
		
		//myTextBold(x,0.64,kBlack,Form("#sigma = %0.4f #mus",sigma[i]));
		/*
		myTextBold(x,0.56,colors[0],Form("Mean = %0.3f #mus",mean2[i]));
		myTextBold(x,0.48,colors[0],Form("#sigma = %0.4f #mus",sigma2[i]));
		*/
		c1->RedrawAxis();
		c1->Print(plotdir+Form("/fit_S1_tau_%d.pdf",i));
		delete h;
		delete he;
	}
	
	
	// S1 delta tau, nsamples = 262144
	for (int i=0; i<N_PMT; i++)
	{	
		TH1D* h = fitPMT_diff(dpd,i);
	
		TF1 *f2 = h->GetFunction("f1");
		mean2[i] = f2->GetParameter(1);
		sigma2[i] = f2->GetParameter(2);
		f2->SetLineColor(colors[1]);
		
		c1->Clear();
		c1->cd();

		h->GetXaxis()->SetTitle("PMT S1 #Delta#tau [#mus]");
		h->GetYaxis()->SetTitle("Entries (x1000)");
		h->Draw();
		float x = 0.62; // (i==2 || i==3) ? 0.22 : 0.6;
		myTextBold(x,0.8,kBlack,Form("%s",run_desc.Data()));
		myTextBold(x,0.72,kBlack,Form("PMT %d (%s)",i+1,pmt_base[i].Data()));
		myTextBold(x,0.64,kBlack,Form("#mu_{#Delta#tau} = %0.3f #mus",mean2[i]));
		myTextBold(x,0.56,kBlack,Form("#sigma_{#Delta#tau} = %0.3f #mus",sigma2[i]));

		c1->RedrawAxis();
		c1->Modified();
		c1->Update();
		c1->Print(plotdir+Form("/fit_S1_delta_tau_%d.pdf",i));
		delete h;
		//delete he;
	}
	
	
	/*
	// low S1_tau, nsamples = 1000
	for (int i=0; i<N_PMT; i++)
	{	
		TH1D* h = fitPMT_low(dpd,i,S1_tau_fit_mean_low[i]);
		
		TF1 *f1 = h->GetFunction("fit");
		mean3[i] = f1->GetParameter(1);
		sigma3[i] = f1->GetParameter(2);
		
		c1->Clear();
		c1->cd();
		TH1D* he = new TH1D("he","he",10,mean3[i]-0.06,mean3[i]+0.1);
		he->SetMaximum(1.2*h->GetMaximum());
		he->GetXaxis()->SetTitle("PMT S1 #tau [#mus]");
		he->GetYaxis()->SetTitle("Entries");
		he->Draw();
		h->Draw("same");
		float x = 0.6; 
		myTextBold(x+0.06,0.8,kBlack,Form("PMT %d (%s)",i,pmt_polar[i].Data()));
		myTextBold(x,0.72,colors[3],Form("Mean = %0.3f #mus",mean3[i]));
		myTextBold(x,0.64,colors[3],Form("#sigma = %0.4f #mus",sigma3[i]));
		c1->RedrawAxis();
		c1->Print(Form("plots/fit_S1_tau_low_%d.pdf",i));
		delete h;
		delete he;
	}
	*/
	
	cout << "tau: " << endl;
	for (int i=0; i<N_PMT; i++) printf("%0.3f, ", mean[i]);
	printf("\n");
	for (int i=0; i<N_PMT; i++) printf("%0.3e, ", sigma[i]);
	printf("\n");
	cout << "delta tau: " << endl;
	for (int i=0; i<N_PMT; i++) printf("%0.3f, ", mean2[i]);
	printf("\n");
	for (int i=0; i<N_PMT; i++) printf("%0.3e, ", sigma2[i]);
	printf("\n");
}