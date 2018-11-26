#include <iostream>
#include "Riostream.h"
#include <sstream>
#include <string>

#include "../../../Light_reco/LIB/WaveformAnalysis.cc"

const string infolder = "/Volumes/scratch/downloads/datos/Tercera_Tanda";

static const int NMAX=1005000;

double fitPulse(TH1* h, TString cd, double& RC_err, float max=0)
{
	double pedrms;
	double ped = WaveformAnalysis::baseline(h,pedrms,20,220);
	
	double RC;
	
	if (cd=="c" || cd=="d")
	{
		double peak = (cd=="c")?h->GetMinimum():h->GetMaximum();
		int binpeak = (cd=="c")?h->GetMinimumBin():h->GetMaximumBin();
		double mint = hcenter(h,binpeak+10000);
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

TH1F* makePulseHisto(TString fn, TString loc, double& RC1, double &RC2)
{
	TString cd;
	if (fn.Contains("Carga")) cd="c";
	else if (fn.Contains("Descarga")) cd="d";
	else if (fn.Contains("C&D")) cd="cd";
	else cout << "Unable to initialize TString cd" << endl;
	
	bool hiRes = loc.Contains("250MSs");
		
	TString hname = loc+"_"+cd;
	
	TString infile = infolder+"/"+loc+"/"+fn+".csv";
	
	cout << "Creating histo " << hname << " from " << infile << endl;
	
	ifstream ifile;
	ifile.open(infile.Data());

	string aux;
	for (int i=0;i<21;i++) getline(ifile,aux);
	int nlines=0;
	float t, v;
	float tt[NMAX],vv[NMAX]={0};
	char cc;
	int counter=0;
	while(!ifile.eof() && nlines<NMAX)
	{
		counter++;
		getline(ifile,aux);
		//cout <<aux <<endl;
		stringstream aa(aux);
		aa >> t >> cc >> v;
		//cout << t << " " << v << endl;
		if (hiRes && counter<20000) continue;
		tt[nlines]=t*1.E6;
		vv[nlines]=v*1.E3;
		nlines++;
		if (nlines%10000==0) cout << nlines << endl;
		//if(nlines==30)	break;
	}
	//cout << "nlines = " << nlines << endl;
	
	ifile.close();
	
	float bw = tt[1]-tt[0];
	TH1F* h = new TH1F(hname.Data(),hname.Data(),nlines-2,tt[0]-bw/2.,tt[nlines-1]+bw/2.);
	
	h->GetXaxis()->SetTitle("[#mus]");
	h->GetYaxis()->SetTitle("[mV]");
	
	for (int i=0; i<nlines; i++) h->SetBinContent(i+1,vv[i]);
	
	double RC1_err,RC2_err=0.;
	if (cd=="c" || cd=="d") { RC1=fitPulse(h,cd,RC1_err); RC2=0.; }
	else if (cd=="cd") { RC1=fitPulse(h,"d",RC1_err); RC2=fitPulse(h,"c",RC2_err,hcenter(h,h->GetMaximumBin()-50000)); }

	h->Draw();
	
	TLatex l;
	l.SetNDC();
	float x=0.58;
	float y=0.68;
	l.DrawLatex(x,y,Form("RC = %0.1f #pm %0.1f #mus",RC1,RC1_err));
	
	if (cd=="cd") l.DrawLatex(0.34,0.2,Form("RC = %0.1f #pm %0.1f #mus",RC2,RC2_err));
	
	return h;
}

void makePulseRootFile4()
{
	TH1F* h;
	double RC1,RC2=0;
	
	gStyle->SetOptStat(0);
	
	TCanvas *c109_cable = new TCanvas("c109_cable","c109_cable",500,500);
	c109_cable->cd();
	h = makePulseHisto("AVER_C&D","250MSs/VLED109_2ms_Long_cable",RC1,RC2);
	c109_cable->Print(Form("%s.png",c109_cable->GetName()));
	
}
