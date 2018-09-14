#include "../config_analysis.h"

TH1D* fitPMT(TTree* t, int p, float tau_nominal)
{	
	TString pmt = Form("pmt_S1_tau[%d]",p);
	TString limits="";
	//TString limits = (ind==n_names-1) ? "" : Form("(4096.0/2.0)*fabs(pmt_S1_amp[%d])>=12.*pmt_pedrms[%d]",i,i);
	t->Draw(Form("%s>>hist[%d](40,%e,%e)",pmt.Data(),p,tau_nominal-0.05,tau_nominal+0.05),limits);
	
	TH1D* hist = (TH1D*)gDirectory->Get(Form("hist[%d]",p));
	hist->Scale(1./1000);
	
	float max = hist->GetMaximum();
	float cen = hist->GetBinCenter(hist->GetMaximumBin());
	float width = 0.0065;
	
	TF1* fit = new TF1("fit","gaus+[3]",cen-width*10,cen+width*10);
	fit->SetParameter(0,max);
	fit->SetParameter(1,cen);
	fit->SetParameter(2,width);
	fit->SetParLimits(1,cen-0.1,cen+0.1);
	//fit->SetParLimits(2,width-0.0005,width+0.002);
	fit->SetLineColor(colors[1]);
	
	hist->Fit(fit,"MR+","",cen-width*5,cen*width*5);
	
	/*
	float width2=0.01;
	TF1* fit2 = new TF1("fit2","gaus+[3]",cen+0.08-width2*10,cen+0.08+width2*10);
	fit2->SetParameter(0,0.04*max);
	fit2->SetParameter(1,cen+0.08);
	fit2->SetParameter(2,width2);
	fit2->SetLineColor(colors[0]);
	
	hist->Fit(fit2,"MR+","",cen+0.08-width2*5,cen+0.08+width2*5);
	*/
	
	return hist;
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
	gSystem->Exec("mkdir -p plots");
	
	SetMyStyle();
	
	TFile* f = TFile::Open(dpd_file.c_str());
	
	TTree* dpd = (TTree*)f->Get("dpd");
	
	TCanvas *c1 = new TCanvas;
	
	float mean[6]={0};
	float sigma[6]={0};
	float mean2[6]={0};
	float sigma2[6]={0};
	float mean3[6]={0};
	float sigma3[6]={0};
	
	// high S1_tau, nsamples = 262144
	for (int i=0; i<5; i++)
	{	
		TH1D* h = fitPMT(dpd,i,S1_tau_fit_mean[i]);
		
		TF1 *f1 = h->GetFunction("fit");
		mean[i] = f1->GetParameter(1);
		sigma[i] = f1->GetParameter(2);
		/*
		TF1 *f2 = h->GetFunction("fit2");
		mean2[i] = f2->GetParameter(1);
		sigma2[i] = f2->GetParameter(2);
		*/
		
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
		myTextBold(x,0.72,kBlack,Form("PMT %d (%s)",i+1,pmt_polar[i].Data()));
		myTextBold(x,0.64,kBlack,Form("#mu_{#tau} = %0.3f #mus",mean[i]));
		myTextBold(x,0.56,kBlack,Form("#sigma_{#tau} = %0.3f #mus",sigma[i]));
		
		//myTextBold(x,0.72,kBlack,Form("Mean = %0.3f #pm %0.3f #mus",mean[i],sigma[i]));
		
		//myTextBold(x,0.64,kBlack,Form("#sigma = %0.4f #mus",sigma[i]));
		/*
		myTextBold(x,0.56,colors[0],Form("Mean = %0.3f #mus",mean2[i]));
		myTextBold(x,0.48,colors[0],Form("#sigma = %0.4f #mus",sigma2[i]));
		*/
		c1->RedrawAxis();
		c1->Print(Form("plots/fit_S1_tau_%d.pdf",i));
		delete h;
		delete he;
	}
	
	/*
	// low S1_tau, nsamples = 1000
	for (int i=0; i<5; i++)
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
	
	for (int i=0; i<5; i++) printf("%0.3f, ", mean[i]);
	printf("\n");
	for (int i=0; i<5; i++) printf("%0.3e, ", sigma[i]);
	printf("\n");
	/*
	for (int i=0; i<5; i++) printf("%0.3f, ", mean3[i]);
	printf("\n");
	for (int i=0; i<5; i++) printf("%0.3e, ", sigma3[i]);
	printf("\n");
	*/
}