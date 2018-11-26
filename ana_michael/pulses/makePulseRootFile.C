#include <iostream>
#include "Riostream.h"
#include <sstream>
#include <string>

#include "../../../Light_reco/LIB/WaveformAnalysis.cc"

const string infolder = "/Volumes/scratch/downloads/datos/Segunda_Tanda";

static const int NMAX=12000;

void makeCorrHistos(const TH1* h, double RC_c, double RC_d, bool doPrint=false)
{
	double pedrms;
	double ped = WaveformAnalysis::baseline(h,pedrms,20,220);
	
	TString hName = h->GetName();
	
	TH1D* hR = (TH1D*)h->Clone("hR");
	for (int i=1; i<hR->GetNbinsX()+1; i++) hR->SetBinContent(i,hget(hR,i)-ped);
	
	TH1D* hPMT = (TH1D*)hR->Clone("hPMT");
	TH1D* hC   = (TH1D*)hR->Clone("hC");
	
	hC->SetBinContent(1,0.);
	hPMT->SetBinContent(1,0.);
	
	RC_c=285.;
	RC_d=255.; 
	
	double dt = hPMT->GetBinWidth(1);
	//cout << "dt = " << dt << endl;
	for (int i=2; i<hPMT->GetNbinsX()+1; i++)
	{
		double vri=hget(hR,i);
		double vri1=hget(hR,i-1);
		double vpmti1=hget(hPMT,i-1);
		//double vci1 = (i==2)?vpmti1-vri1:hget(hC,i-1);
		double vci1 = hget(hC,i-1);
		//double vci=vci1+dt/RC_c*(vpmti1-vci1);
		
		//double RC = 313.+gRandom->Gaus(0.,80.);
		//double vci=(vri<vri1)?vci1+dt/RC*(vpmti1-vci1):vci1+dt/RC*(vpmti1-vci1);
		
		
		double vci=(vri<vri1)?vci1+dt/RC_c*(vpmti1-vci1):vci1+dt/RC_d*(vpmti1-vci1); 
		//double vpmti=vpmti1+vri+vri1*(dt/RC_c-1.);
		double vpmti=vci+vri;
		//
		//if (fabs(vri)<0.1 && fabs(vri1)<0.1) hcorr->SetBinContent(i,0);
		//if (i==2) hC->SetBinContent(i-1,vci1);
		hC->SetBinContent(i,vci);
		hPMT->SetBinContent(i,vpmti);
	}
	
	THStack *hs = new THStack(Form("%s_corr",hName.Data()),Form("%s_corr",hName.Data()));
	
	//hR->SetLineColor(kBlue);
	hs->Add(hR);
	hC->SetLineColor(kRed);
	hs->Add(hC);
	hPMT->SetLineColor(kGreen+2);
	hs->Add(hPMT);
	
	hs->Draw("hist.nostack");
	
	double pedrms_end;
	double ped_end = WaveformAnalysis::baseline(hPMT,pedrms_end,hPMT->GetNbinsX()-212,hPMT->GetNbinsX()-10);
	
	if (doPrint)
	{
		TLatex l;
		l.SetNDC();
		l.DrawLatex(0.18,0.82,Form("PMT int = %0.3f",hPMT->Integral()));
		l.DrawLatex(0.18,0.76,Form("PMT ped bias = %0.3f #pm %0.3f mV",ped_end,pedrms_end));
	}
	
	hs->GetXaxis()->SetTitle("[#mus]");
	hs->GetYaxis()->SetTitle("[mV]");
	
	float x = 0.62;
	float y = 0.22;
	if (hName.Contains("2ms") || hName.Contains("3ms")) y=0.32;
	TLegend *tleg = new TLegend(x,y,x+0.26,y+0.16);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
	tleg->AddEntry(hR, "V_{R}", "l");
	tleg->AddEntry(hC, "V_{C}", "l");
	tleg->AddEntry(hPMT, "V_{PMT}", "l");
	tleg->Draw();
	
	hs->Draw("hist.nostack.same");
	
	return;
}

double fitPulse(TH1* h, TString cd, double& RC_err, float max=0)
{
	double pedrms;
	double ped = WaveformAnalysis::baseline(h,pedrms,20,220);
	
	double RC;
	
	if (cd=="c" || cd=="d")
	{
		double peak = (cd=="c")?h->GetMinimum():h->GetMaximum();
		int binpeak = (cd=="c")?h->GetMinimumBin():h->GetMaximumBin();
		double mint = hcenter(h,binpeak+10);
		double maxt = (max==0)?hcenter(h,h->GetNbinsX()):max;
		
		//printf("ped = %f, pedrms = %f, peak = %f, maxt = %f\n",ped,pedrms,peak,maxt);
		
		TF1* fit = new TF1("fit","[0]+[1]*exp(-(x-[2])/[3])",mint,maxt);
		fit->SetParameter(0,ped);
		if (cd=="c") fit->SetParameter(1,-fabs(peak-ped));
		else if (cd=="d") fit->SetParameter(1,fabs(peak-ped));
		fit->SetParameter(2,mint);
		fit->SetParameter(3,240.);
		
		h->Fit(fit,"MQR+","",mint,maxt);
		
		RC=fit->GetParameter(3);
		RC_err=fit->GetParError(3);
	}
	
	printf("hname = %s, RC = %f, RC_err = %f\n",h->GetName(),RC,RC_err);
	
	return RC;
}

TH1D* makePulseHisto(TString vled, TString len, TString fn, double& RC1, double &RC2)
{
	TString cd;
	if (fn.Contains("Carga")) cd="c";
	else if (fn.Contains("Descarga")) cd="d";
	else if (fn.Contains("C&D")) cd="cd";
	else cout << "Unable to initialize TString cd" << endl;
	
	TString hname = vled+"_"+len+"_"+cd;
	
	TString infile = infolder+"/"+vled+"/"+"Pulso_"+len+"/"+fn+".csv";
	
	//cout << "Creating histo " << hname << " from " << infile << endl;
	
	ifstream ifile;
	ifile.open(infile.Data());

	string aux;
	for (int i=0;i<21;i++) getline(ifile,aux);
	int nlines=0;
	double t, v;
	double tt[NMAX],vv[NMAX]={0};
	char cc;
	while(!ifile.eof())
	{
		getline(ifile,aux);
		//	cout <<aux <<endl;
		stringstream aa(aux);
		aa >> t >> cc >> v;
		//cout << t << " " << v << endl;
		tt[nlines]=t*1.E6;
		vv[nlines]=v*1.E3;
		nlines++;
		//if(nlines==30)	break;
	}
	//cout << "nlines = " << nlines << endl;
	
	ifile.close();
	
	double bw = tt[1]-tt[0];
	TH1D* h = new TH1D(hname.Data(),hname.Data(),nlines-2,tt[0]-bw/2.,tt[nlines-1]+bw/2.);
	
	h->GetXaxis()->SetTitle("[#mus]");
	h->GetYaxis()->SetTitle("[mV]");
	
	for (int i=0; i<nlines; i++) h->SetBinContent(i+1,vv[i]);
	
	double RC1_err,RC2_err=0.;
	if (cd=="c" || cd=="d") { RC1=fitPulse(h,cd,RC1_err); RC2=0.; }
	else if (cd=="cd") { RC1=fitPulse(h,"d",RC1_err); RC2=fitPulse(h,"c",RC2_err,hcenter(h,h->GetMaximumBin()-10)); }

	h->Draw();
	
	TLatex l;
	l.SetNDC();
	float x=0.58;
	float y=0.68;
	l.DrawLatex(x,y,Form("RC = %0.1f #pm %0.1f #mus",RC1,RC1_err));
	
	if (cd=="cd") l.DrawLatex(0.34,0.2,Form("RC = %0.1f #pm %0.1f #mus",RC2,RC2_err));
	
	return h;
}

void makePulseRootFile()
{
	static const int NHIST=8;
	
	TH1D* h[NHIST];
	double RC1[NHIST],RC2[NHIST]={0};
	
	TString LT[] = {"500us","1ms","2ms","2ms","500us","1ms","3ms","3ms"};
	TString NA[] = {"AVER_C&D","AVER_C&D","AVER_Carga","AVER_Descarga"};
	
	gStyle->SetOptStat(0);
	
	TCanvas *c108 = new TCanvas("c108","c108",1000,1000);
	c108->Divide(2,2);

	for (int i=0; i<4; i++)
	{
		c108->cd(i+1);
		h[i] = makePulseHisto("VLED108",LT[i],NA[i],RC1[i],RC2[i]);
		c108->RedrawAxis();
	}
	c108->Print(Form("%s.png",c108->GetName()));
	
	TCanvas *c108_corr = new TCanvas("c108_corr","c108_corr",1000,1000);
	c108_corr->Divide(2,2);
	
	for (int i=0; i<4; i++)
	{
		c108_corr->cd(i+1);
		if (i<2) makeCorrHistos(h[i],RC1[i],RC2[i],true); 
		else if (i==2) makeCorrHistos(h[i],RC1[i],RC1[i+1]); 
		else if (i==3) makeCorrHistos(h[i],RC1[i-1],RC1[i]); // i-1
		c108_corr->RedrawAxis();
	}
	c108_corr->Print(Form("%s.png",c108_corr->GetName()));
	
	TCanvas *c109 = new TCanvas("c109","c109",1000,1000);
	c109->Divide(2,2);
	
	for (int i=0; i<4; i++)
	{
		c109->cd(i+1);
		h[4+i] = makePulseHisto("VLED109",LT[4+i],NA[i],RC1[i+4],RC2[i+4]);
		c109->RedrawAxis();
	}
	c109->Print(Form("%s.png",c109->GetName()));
	
	TCanvas *c109_corr = new TCanvas("c109_corr","c109_corr",1000,1000);
	c109_corr->Divide(2,2);
	
	for (int i=0; i<4; i++)
	{
		c109_corr->cd(i+1);
		if (i<2) makeCorrHistos(h[i+4],RC1[i+4],RC2[i+4],true);
		else if (i==2) makeCorrHistos(h[i+4],RC1[i+4],RC1[i+5]); 
		else if (i==3) makeCorrHistos(h[i+4],RC1[i+3],RC1[i+4]);
		c109_corr->RedrawAxis();
	}
	c109_corr->Print(Form("%s.png",c109_corr->GetName()));
	
	TCanvas *c_corr = new TCanvas("c_corr","c_corr",1000,1000);
	c_corr->Divide(2,2);
	
	for (int i=0; i<4; i++)
	{
		c_corr->cd(i+1);
		if (i<2) makeCorrHistos(h[i],RC1[i],RC2[i],true);
		else makeCorrHistos(h[i+2],RC1[i+2],RC2[i+2],true);
		c_corr->RedrawAxis();
	}
	c_corr->Print(Form("%s.png",c_corr->GetName()));
	
	TFile *fout = new TFile("pulses.root","RECREATE");
	for (int i=0; i<NHIST; i++) h[i]->Write();
	c108->Write();
	c108_corr->Write();
	c109->Write();
	c109_corr->Write();
	fout->Close();
}
