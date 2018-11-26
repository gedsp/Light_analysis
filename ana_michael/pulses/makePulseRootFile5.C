#include <iostream>
#include "Riostream.h"
#include <sstream>
#include <string>

#include "../../../Light_reco/LIB/WaveformAnalysis.cc"

const string infolder = "/Volumes/scratch/downloads/datos/";

static const int NMAX=100500;


TH1F* makePulseHisto(TCanvas *c, TString loc)
{
	TString cd = "cd";
		
	TString hname = loc+"_"+cd;
	
	TString infile = infolder+"/"+loc+".csv";
	
	cout << "Creating histo " << hname << " from " << infile << endl;
	
	ifstream ifile;
	ifile.open(infile.Data());
	
	string aux;
	for (int i=0;i<21;i++) getline(ifile,aux);
	int nlines=0;
	float t, gv, sv; 
	float tt[NMAX],gvv[NMAX],svv[NMAX]={0};
	char cc;
	int counter=0;
	while(!ifile.eof() && nlines<NMAX)
	{
		counter++;
		getline(ifile,aux);
		//cout <<aux <<endl;
		stringstream aa(aux);
		aa >> t >> cc >> gv >> cc >> sv;
		tt[nlines]=t*1.E6;
		gvv[nlines]=gv;
		svv[nlines]=sv; 
		nlines++;
		if (nlines%10000==0) cout << nlines << endl;
		//if(nlines==30)	break;
	}
	//cout << "nlines = " << nlines << endl;
	
	ifile.close();
	
	float bw = tt[1]-tt[0];
	cout << "bw = " << bw << endl;
	TH1F* hg = new TH1F(Form("%s_g",hname.Data()),Form("%s_g",hname.Data()),nlines-2,tt[0]-bw/2.,tt[nlines-1]+bw/2.);
	TH1F* hs = new TH1F(Form("%s_s",hname.Data()),Form("%s_s",hname.Data()),nlines-2,tt[0]-bw/2.,tt[nlines-1]+bw/2.);
	
	hg->GetXaxis()->SetTitle("[#mus]");
	hs->GetXaxis()->SetTitle("[#mus]");
	hg->GetYaxis()->SetTitle("[V]");
	hs->GetYaxis()->SetTitle("[V]");
	
	for (int i=0; i<nlines; i++)
	{ 
		hg->SetBinContent(i+1,gvv[i]); 
		hs->SetBinContent(i+1,svv[i]); 
	}
	
	// calculate pedestals
	float ped_start=-10;
	float ped_stop=-0.5;
	
	double pedrms_g=0;
	double pedrms_s=0;
	
	double ped_g = WaveformAnalysis::baseline(hg,pedrms_g,hg->GetXaxis()->FindBin(ped_start),hg->GetXaxis()->FindBin(ped_stop)-1); 
	double ped_s = WaveformAnalysis::baseline(hs,pedrms_s,hs->GetXaxis()->FindBin(ped_start),hs->GetXaxis()->FindBin(ped_stop)-1);
	
	printf("ped_g = %f +/- %f\n",ped_g,pedrms_g);
	printf("ped_s = %f +/- %f\n",ped_s,pedrms_s);
	
	for (int i=0; i<nlines; i++)
	{ 
		hg->SetBinContent(i+1,gvv[i]-ped_g);
		hs->SetBinContent(i+1,svv[i]-ped_s);
	}
	
	double sum_g=0.;
	double sum_s_Q1=0.;
	double sum_s_Q2=0.;
	double sum_s_Q3=0.;
	double sum_s_all=0.;
	
	for (int i=0; i<nlines; i++)  
	{
		double bc = hcenter(hg,i);
		//if (bc<-0.05 || bc>30) continue;
		//if (bc<-0.05) continue;
		sum_g-=hget(hg,i);
	}
	
	cout << "sum_g = " << sum_g << endl;
	
	int bmax = hs->GetMaximumBin();
	double vmax = hs->GetMaximum();
	double Q3 = vmax*200E-9*1.E6; // in uC
	double Q1=0.0;
	double Q2=0.0;
	
	for (int i=0; i<nlines; i++)
	{
		double bc = hcenter(hs,i);
		//if (bc<-0.05) continue;
		double con = hget(hs,i);
		sum_s_all+=con;
		
		
		//if (bc<=30 && con<0) sum_s_Q1-=con;
		//else if (bc>30 && con>0) sum_s_Q3+=con;
		
		//if (bc<=30) sum_s_Q1-=con;
		//else if (bc>30) sum_s_Q3+=con;
		
		if (bc<=30)
		{
			if (con>0) sum_s_Q2+=con;
			else sum_s_Q1+=fabs(con);
		}
		else if (bc>30) sum_s_Q3+=fabs(con);
		
		if (i<bmax && con<0) Q1+= bw*fabs(con)/50.;
		if (i<bmax && con>0) Q2+= bw*fabs(con)/50.;
	}
	
	double sum_s_tot = sum_s_Q1+sum_s_Q3;
	
	cout << "sum_s_tot = " << sum_s_tot << ": sum_s_Q1 = " << sum_s_Q1 << ", sum_S_Q2 = " << sum_s_Q2 << ", sum_S_Q3 = " << sum_s_Q3 << endl;
	cout << "percent err = " << 100.*(sum_s_tot-sum_g)/sum_g << endl;
	cout << "high V = " << hs->GetMaximum() << endl;
	cout << "sum_s_all = " << sum_s_all << endl;

	double Qtot = Q1+Q3;
	sum_g=sum_g*bw/50.;
	cout << "Q1 = " << Q1 << ", Q2 = " << Q2 << ", Q3 = " << Q3 << ", Qtot = " << Qtot << endl;
	cout << "percent err = " << 100.*(Qtot-sum_g)/sum_g << endl;

	TH1F* hc;
	//WaveformAnalysis::correct_wvf_histo(hs,hc,0.0,20.8,20.8); 
	WaveformAnalysis::correct_wvf_histo(hs,hc,0.0,256,313); 
	double sum_hc = -hc->Integral();
	cout << "sum hc = " << sum_hc << endl;
	cout << "percent err = " << 100.*(sum_hc-sum_g)/sum_g << endl;
		
	hg->SetLineColor(kCyan);
	hs->SetLineColor(kMagenta);
	hc->SetLineColor(kGreen+2);
	
	c->cd(1);
	hg->Draw();
	c->cd(2);
	hs->Draw();
	c->cd(3);
	hc->Draw();
	
	return hg;
}

void makePulseRootFile5()
{
	TH1F* h;
	double RC1,RC2=0;
	
	gStyle->SetOptStat(0);
	
	TCanvas *c1 = new TCanvas("Tren_pulsos_gen","Tren_pulsos_gen",1500,1000);
	c1->Divide(1,3);
	h = makePulseHisto(c1, "Tren_pulsos_gen");
	//c109_cable->Print(Form("%s.png",c109_cable->GetName()));
	
}
