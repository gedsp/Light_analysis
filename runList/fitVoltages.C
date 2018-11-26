#include "../config_analysis.h"

TString run_desc2 = "Runs 768-1199";

bool do_crosscheck=true;
float cut_val_err=0.01;

void fitHist(TTree* t, int ind, TString name, TString unit, int nbins, float xmin, float xmax, float& me, float& sig)
{
	t->Draw(Form("%s>>h[%d](%d)",name.Data(),ind,nbins)); // ,nbins)); 
	TH1D* h = (TH1D*)gDirectory->Get(Form("h[%d]",ind));
	h->GetXaxis()->SetTitle(Form("[%s]",unit.Data()));
	h->GetYaxis()->SetTitle("Entries");
	
	TF1* fit = new TF1("fit","gaus",h->GetBinLowEdge(1),h->GetBinLowEdge(h->GetNbinsX()+1)+h->GetBinWidth(h->GetNbinsX()+1));
	h->Fit(fit,"QMR+","",xmin,xmax);
	
	me=fit->GetParameter(1);
	sig=fit->GetParameter(2);
	
	printf("%s: mean = %0.3f, sigma = %0.3e\n",name.Data(),me,sig);
	myTextBold(0.15,0.8,kBlack,run_desc2.Data());
	myTextBold(0.15,0.72,kBlack,Form("Mean = %0.3f #pm %0.3f %s",me,sig,unit.Data()));
	
}

void fitVoltages()
{
	gSystem->Exec("mkdir -p voltages");
	
	TFile *f = TFile::Open(db_charge_file.c_str());
	TTree *ntup = (TTree*)f->Get("ntuple");
	
	gStyle->SetOptStat(0);
	
	TCanvas *c1 = new TCanvas();

	static const int nhist = 7;
	
	TString names[nhist] = {"grid","cath","dl","ext_field_l","ext_field_g","amp_field","drift_field"};
	TString units[nhist] = {"V","V","mm","kV/cm","kV/cm","kV/cm","kV/cm"};
	int bins[nhist] = {100,175,500,100,200,100,150};
	float xmin[nhist] = {4800,50000,9,1.8,2.8,26,0.45};
	float xmax[nhist] = {5200,56000,10,2.3,3.1,29,0.51};
	
	float mean[nhist] = {0};
	float sigma[nhist] = {0};
	
	for (int i=0; i<nhist; i++)
	{
		fitHist(ntup,i,names[i],units[i],bins[i],xmin[i],xmax[i],mean[i],sigma[i]);
		c1->Modified();
		c1->Update();
		c1->Print(Form("voltages/%s_fit.pdf",(names[i]).Data()));
	}
	
	// 3sigma cut
	TString cuts = Form("fabs(%s-%f)<3.*%f && fabs(%s-%f)<3.*%f && fabs(%s-%f)<3.*%f && fabs(%s-%f)<3.*%f && fabs(%s-%f)<3.*%f",
	names[0].Data(),mean[0],sigma[0],names[1].Data(),mean[1],sigma[1],names[2].Data(),mean[2],sigma[2],
	names[3].Data(),mean[3],sigma[3],//names[4].Data(),mean[4],sigma[4],
	//names[5].Data(),mean[5],sigma[5],
	names[6].Data(),mean[6],sigma[6]);
	
	// err cut
	cuts=cuts+Form(" && fabs(%s_err/%s)<%f && fabs(%s_err/%s)<%f && fabs(%s_err/%s)<%f && fabs(%s_err/%s)<%f && fabs(%s_err/%s)<%f && fabs(%s_err/%s)<%f && fabs(%s_err/%s)<%f",
	names[0].Data(),names[0].Data(),cut_val_err,names[1].Data(),names[1].Data(),cut_val_err,
	names[2].Data(),names[2].Data(),cut_val_err,names[3].Data(),names[3].Data(),cut_val_err,
	names[4].Data(),names[4].Data(),cut_val_err,names[5].Data(),names[5].Data(),cut_val_err,
	names[6].Data(),names[6].Data(),cut_val_err);
	
	// remove nsamples=1000
	cuts=cuts+" && run!=789 && run!=790 && run!=791 && run!=800 && run!=801 && run!=802 ";
	// remove runs where PMT1 voltage is off
	cuts=cuts+" && run!=984 && run!=985 ";
	// remove missing runs
	cuts=cuts+" && (run*1000+subrun!=999020) && (run*1000+subrun!=1007003) && (run*1000+subrun!=998053) && (run*1000+subrun!=998055) && (run*1000+subrun!=840077) && (run*1000+subrun!=842010) && (run*1000+subrun!=981000) && (run*1000+subrun!=982000) && (run*1000+subrun!=983000) && (run*1000+subrun!=984000) && (run*1000+subrun!=984001) && (run*1000+subrun!=986000) && (run*1000+subrun!=989009) && (run*1000+subrun!=989010) && (run*1000+subrun!=989011) && (run*1000+subrun!=990000) && (run*1000+subrun!=994009) && (run*1000+subrun!=995000) && (run*1000+subrun!=995003) && (run*1000+subrun!=995004) && (run*1000+subrun!=995005) && (run*1000+subrun!=995006) && (run*1000+subrun!=995007) && (run*1000+subrun!=995008) && (run*1000+subrun!=996000) && (run*1000+subrun!=996001) && (run*1000+subrun!=996002) && (run*1000+subrun!=996003) && (run*1000+subrun!=996004) && (run*1000+subrun!=996005) && (run*1000+subrun!=996006) && (run*1000+subrun!=1038020) && (run*1000+subrun!=1038021) && (run*1000+subrun!=1039000) && (run*1000+subrun!=1040000) && (run*1000+subrun!=1040001) && (run*1000+subrun!=1040002)";
	
	//cuts=cuts+" && fabs(pmt[2]-1100)<1. && fabs(pmt[3]-1100)<1.";
	
	TH1F* hh[nhist];
	for (int i=0; i<nhist; i++)
	{
		ntup->Draw(Form("%s>>hh[%d]",names[i].Data(),i),cuts);
		hh[i] = (TH1F*)gDirectory->Get(Form("hh[%d]",i));
		hh[i]->GetXaxis()->SetTitle(Form("%s [%s]",names[i].Data(),units[i].Data()));
		hh[i]->GetYaxis()->SetTitle("Entries");
		c1->Modified();
		c1->Update();
		c1->Print(Form("voltages/%s_cuts.pdf",names[i].Data()));
	}

	if (do_crosscheck)
	{
		// LEMu cross check 
		for (int i=0; i<N_LEM; i++) 
		{ 
			//cout << "lemu[" << i << "]" << endl;
			ntup->Draw(Form("lemu[%d]",i),cuts);
			c1->Modified();
			c1->Update();
			c1->Print(Form("voltages/lemu%d_cuts.pdf",i));
			//lets_pause();
		}
		// LEMd cross check 
		for (int i=0; i<N_LEM; i++) 
		{ 
			//cout << "lemd[" << i << "]" << endl;
			ntup->Draw(Form("lemd[%d]",i),cuts);
			c1->Modified();
			c1->Update();
			c1->Print(Form("voltages/lemd%d_cuts.pdf",i));
			//lets_pause();
		}
		// LEMd cross check 
		for (int i=0; i<N_LEM; i++) 
		{ 
			//cout << "lemd-u[" << i << "]" << endl;
			ntup->Draw(Form("lemd[%d]-lemu[%d]",i,i),cuts);
			c1->Modified();
			c1->Update();
			c1->Print(Form("voltages/lemd-u%d_cuts.pdf",i));
			//lets_pause();
		}
		TH1F* hhh[3];
		// Sum LEMu cross check
		ntup->Draw("lemu[0]+lemu[1]+lemu[2]+lemu[3]+lemu[4]+lemu[5]+lemu[6]+lemu[7]+lemu[8]+lemu[9]+lemu[10]+lemu[11]>>hhh[0]",cuts);
		hhh[0] = (TH1F*)gDirectory->Get("hhh[0]");
		hhh[0]->GetXaxis()->SetTitle("#Sum lemu [V]");
		hhh[0]->GetYaxis()->SetTitle("Entries");
		c1->Modified();
		c1->Update();
		c1->Print("voltages/sum_lemu_cuts.pdf");
		
		// Sum LEMd cross check
		ntup->Draw("lemd[0]+lemd[1]+lemd[2]+lemd[3]+lemd[4]+lemd[5]+lemd[6]+lemd[7]+lemd[8]+lemd[9]+lemd[10]+lemd[11]>>hhh[1]",cuts);
		hhh[1] = (TH1F*)gDirectory->Get("hhh[1]");
		hhh[1]->GetXaxis()->SetTitle("#Sum lemd [V]");
		hhh[1]->GetYaxis()->SetTitle("Entries");
		c1->Modified();
		c1->Update();
		c1->Print("voltages/sum_lemd_cuts.pdf");
		
		// Sum LEMd-u cross check
		ntup->Draw("lemd[0]+lemd[1]+lemd[2]+lemd[3]+lemd[4]+lemd[5]+lemd[6]+lemd[7]+lemd[8]+lemd[9]+lemd[10]+lemd[11]-(lemu[0]+lemu[1]+lemu[2]+lemu[3]+lemu[4]+lemu[5]+lemu[6]+lemu[7]+lemu[8]+lemu[9]+lemu[10]+lemu[11])>>hhh[2]",cuts);
		hhh[2] = (TH1F*)gDirectory->Get("hhh[2]");
		hhh[2]->GetXaxis()->SetTitle("#Sum (lemd-lemu)[V]");
		hhh[2]->GetYaxis()->SetTitle("Entries");
		c1->Modified();
		c1->Update();
		c1->Print("voltages/sum_lemd-u_cuts.pdf");
		
		// PMT cross check
		for (int i=0; i<N_PMT; i++) 
		{ 
			//cout << "pmt[" << i << "]" << endl;
			ntup->Draw(Form("pmt[%d]",i),cuts);
			c1->Modified();
			c1->Update();
			c1->Print(Form("voltages/pmt%d_cuts.pdf",i));
			//lets_pause();
		}
	}
	
	cout << endl;
	for (int i=0; i<nhist; i++) printf("%s & %0.3f & %0.3f & %s \\\\ \n",(names[i]).Data(),mean[i],sigma[i],units[i].Data());
	cout << endl;
	
	// write run list
	int nbins=500000;
	ntup->Draw(Form("1000*run+subrun>>runhist(%d,%d,%d)",nbins,749500,1249500),cuts);
	TH1I* runh = (TH1I*)gDirectory->Get("runhist");
	
	// format hadd command
	cout << endl << "hadd runs.root ";
	int n_runs_selected=0;
	for (int i=0; i<500000; i++)
	{
		if (runh->GetBinContent(i)==1) { cout << "dpd-matched-" << (int)(runh->GetBinCenter(i)/1000) << "-" << ((int)runh->GetBinCenter(i))%1000 << ".root "; n_runs_selected++; }
		if (runh->GetBinContent(i)>1) cout << endl << "WARNING: more than 1 entry in bin " << i << ": " << runh->GetBinCenter(i) << endl;
	}
	cout << endl;
	
	cout << "total runs/subruns selected = " << n_runs_selected << endl;

	/*
	// format hadd command
	cout << endl << "hadd ";
	for (int i=0; i<nbins; i++) if (runh->GetBinContent(i)>=1) cout << "files/run" << runh->GetBinCenter(i) <<"-*.root ";
	cout << endl;	
	*/
	
	// display easy-to-read ranges
	runh->Rebin(1000);
	bool selec=false;
	for (int i=1; i<runh->GetNbinsX()+1; i++)
	{
		if (runh->GetBinContent(i)>=1) 
		{
			if (!selec) { selec=true; cout << (int)(runh->GetBinCenter(i)/1000.) << "--"; }
		}
		else if (selec) { selec=false; cout << (int)(runh->GetBinCenter(i)/1000.)-1 << ", "; }
	}
	cout << endl;
}