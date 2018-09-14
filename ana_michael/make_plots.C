#include "../config_analysis.h"

static const int nhist = 17;
static const int n_names = 24; // PMT plots

TString charge_cut = "ntracks>=1";

void plotS1S2(TTree* t, int ind, TString xname, TString yname, TString xtitle, TString ytitle, 
			  double scalex=1., double scaley=1., double xmin=0, double xmax=0, double ymin=0, double ymax=0)
{	
	TString cuts="";
	t->Draw(Form("%s/%e:%s/%e>>h2d[%d](100,%e,%e,100,%e,%e)",yname.Data(),scaley,xname.Data(),scalex,ind,xmin/scalex,xmax/scalex,ymin/scaley,ymax/scaley),cuts);
	
	TH2D* h2d = (TH2D*)gDirectory->Get(Form("h2d[%d]",ind));
	h2d->GetXaxis()->SetTitle(xtitle);
	h2d->GetYaxis()->SetTitle(ytitle);
	
	h2d->Draw("colz");
}
		
void plotPMT(TTree* t, int ind, TString name, TString xtitle, double scale=1., double xmin=0, double xmax=0)
{
	TH1D* hist[N_PMT+1];
	
	int nmax = N_PMT; // (name.Contains("charge") || name.Contains("npe")) ? N_PMT+1 : N_PMT;
	
	THStack* hs = new THStack(Form("hs%d",ind),Form("pmt%d",ind));
	hs->Clear();
	//hs->SetMinimum(1.E-8);
	
	float x = 0.68;
	float y = 0.58;
	if (ind==2 || ind==4) x=0.52;
	//else if (ind==9) {x=0.3;y=0.22;}
	else if (ind==9) {x=0.3;y=0.22;}
	else if (ind==n_names-2) {x=0.24;y=0.22;}
	else if (ind==15 || ind==20) {x=0.5;y=0.22;}
	//else if (ind==20) y=0.2;
	TLegend *tleg = new TLegend(x,y,x+0.33,y+0.32);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
    tleg->SetHeader(run_desc.Data()); 
	
	for (int i=0; i<nmax; i++)
	{
		TString pmt;
		if (ind==n_names-2) pmt=Form("4096.0*fabs(%s[%d])/pmt_pedrms[%d]",name.Data(),i,i);
		else if (ind==n_names-1) pmt=Form("(pmt_ped_end[%d]-pmt_ped[%d])/pmt_pedrms[%d]",i,i,i);
		else pmt = Form("%s[%d]",name.Data(),i);
		//TString pmt2 = Form("%s[%d]/%e",name.Data(),i,scale);
		//TString limits="";
		//if (!(xmin==0 && xmax==0) && (xmin<xmax)) limits=Form("(%s > %e) && (%s < %e)",pmt.Data(),xmin*0.96,pmt.Data(),xmax*1.04);
		float q = (2.0/4096.0)*4.*1e-9/(50.);
		//TString limits = (ind==n_names-1) ? "" : Form("(4096.0/2.0)*fabs(pmt_S1_amp[%d])>=12.*pmt_pedrms[%d]",i,i);
	  
		TString cut_trigger = Form("((nsamples==262144) && fabs(pmt_S1_tau[%d]-%f)<3.*%f)",i,S1_tau_fit_mean[i],S1_tau_fit_sigma[i]);
		//TString cut_trigger = Form("((nsamples==262144) && fabs(pmt_S1_tau[%d]-%f)<3.*%f) || ((nsamples==1000) && fabs(pmt_S1_tau[%d]-%f)<3.*%f)",
		//i,S1_tau_fit_mean_high[i],S1_tau_fit_sigma_high[i],i,S1_tau_fit_mean_low[i],S1_tau_fit_sigma_low[i]);
		TString cut_zero = Form("%s[%d]!=0",name.Data(),i);
		TString cut_pedrms = Form("pmt_pedrms[%d]<2.0",i); // Form("(4096.0/2.0)*fabs(pmt_S1_amp[%d])>=12.*pmt_pedrms[%d]",i,i);
		TString cut_S1_amp = Form("pmt_S1_amp[%d]<1.95",i);
		TString cut_S2_gaus_tau = Form("pmt_S2_gaus_tau[%d]>230. && pmt_S2_gaus_tau[%d]<850.",i,i);
		
		
		TString cuts = cut_trigger+" && "+cut_pedrms+" && "+cut_S1_amp+" && "+cut_S2_gaus_tau+" && "+charge_cut; 
		if (ind<n_names-2) cuts=cuts+" && "+cut_zero;
		/*
		cuts="";
		if (ind<n_names-2) cuts=cut_zero;
		else cuts="";
		*/

		//cout << "cuts = " << cuts << endl;
		t->Draw(Form("%s/%e>>hist[%d](100,%e,%e)",pmt.Data(),scale,ind*10+i,xmin/scale,xmax/scale),cuts);
		hist[i] = (TH1D*)gDirectory->Get(Form("hist[%d]",ind*10+i));
		hist[i]->SetLineColor(colors[i]);
		hs->Add(hist[i]);
		if (i<N_PMT) tleg->AddEntry(hist[i], Form("PMT %d (%s)",i+1,pmt_polar[i].Data()), "l");
		else tleg->AddEntry(hist[i], "PMT all", "l");
	}
	
	hs->Draw("hist.nostack");
	float ymax = hs->GetMaximum();
	if (ind!=2 && ind!=4 && ind!=n_names-1) { hs->SetMaximum(15*ymax); if (hs->GetMinimum()==0) hs->SetMinimum(0.11);}
	hs->Draw("hist.nostack");
	hs->GetYaxis()->SetTitle("Entries");
	//hs->GetXaxis()->SetRangeUser(xmin*scale,xmax*scale);
	hs->GetXaxis()->SetTitle(xtitle);
    tleg->Draw();
	hs->Draw("hist.nostack.same");
}
	
	
void plotHist(TTree* t, int ind, TString name, TString xtitle, double scale=1., double xmin=0, double xmax=0)
{	
	TString limits="";
	if (name.Contains("crt")) limits="crt_matchreco==1"; // Form("%s!=0",name.Data());
	else if (name.Contains("tpc"))
	{
		if (name.Contains("YX")) limits="tpc_track_fitresult_yx==0";
		else if (name.Contains("ZX")) limits="tpc_track_fitresult_zx==0";
	}
	if (limits=="") limits=charge_cut;
	else limits=limits+" && "+charge_cut;
	if (ind==0) t->Draw(Form("%s/%e>>h(%d,%e,%e)",name.Data(),scale,(int)(xmax/scale),xmin/scale,xmax/scale),limits); 
	else t->Draw(Form("%s/%e>>h(100,%e,%e)",name.Data(),scale,xmin/scale,xmax/scale),limits); 
	TH1D* h = (TH1D*)gDirectory->Get("h"); 
	h->SetLineColor(kBlack);
	h->Draw("hist");
	h->GetYaxis()->SetTitle("Entries");
	h->GetXaxis()->SetTitle(xtitle);
	h->Draw("hist.same");
	float x = 0.22;
	if (ind<4 || ind==5 || ind==14 || ind==15) x=0.65;
	myTextBold(x,0.8,kBlack,run_desc.Data()); 
	myTextBold(x,0.72,kBlack,Form("N_{entries} = %0.0f",h->GetEntries()));
}


void plotNsamp(TTree* t, int ind, TString name, TString xtitle, double scale=1., double xmin=0, double xmax=0)
{	
	TString limits="";
	
	//TString cut_trigger_low = Form("((nsamples==1000) && fabs(pmt_S1_tau[%d]-%f)<3.*%f)",0,S1_tau_fit_mean_low[i],S1_tau_fit_sigma_low[i]);
	//TString cut_trigger_high = Form("((nsamples==262144) && fabs(pmt_S1_tau[%d]-%f)<3.*%f)",0,S1_tau_fit_mean_high[i],S1_tau_fit_sigma_high[i]);
	
	t->Draw(Form("%s/%e>>hns0(100,%e,%e)",name.Data(),scale,xmin/scale,xmax/scale),"nsamples==1000"); 
	TH1D* hns0 = (TH1D*)gDirectory->Get("hns0"); 
	t->Draw(Form("%s/%e>>hns1(100,%e,%e)",name.Data(),scale,xmin/scale,xmax/scale),"nsamples==262144"); 
	TH1D* hns1 = (TH1D*)gDirectory->Get("hns1"); 
	
	hns0->SetLineColor(kBlue);
	hns1->SetLineColor(kRed);
	
	THStack* hs = new THStack(Form("hs%d",ind*1000),Form("hs%d",ind*1000));
	hs->Clear();
	
	hs->Add(hns0);
	hs->Add(hns1);
	
	hs->Draw("hist.nostack");
	hs->GetXaxis()->SetTitle(xtitle);
	hs->GetYaxis()->SetTitle("Entries");

	float x = 0.58;
	float y = 0.74;
	TLegend *tleg = new TLegend(x,y,x+0.3,y+0.16);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
	tleg->AddEntry(hns1, "N_{samples} = 262144", "l");
	tleg->AddEntry(hns0, "N_{samples} = 1000", "l");
	tleg->Draw();
	
	/*
	x=0.62;
	myTextBold(x,0.76,kRed,Form("Entries = %0.f",hns1->GetEntries()));
	myTextBold(x,0.84,kBlue,Form("Entries = %0.f",hns0->GetEntries()));
	*/

}


void make_plots(void)
{
	gSystem->Exec("mkdir -p voltages");
	
	SetMyStyle();
	
	TFile* f = TFile::Open(dpd_file.c_str());
	
	TTree* dpd = (TTree*)f->Get("dpd");
	
	TCanvas *c1 = new TCanvas;
	
	// 1D HIST
	
	TString hist_names[nhist] = {"ntracks","tpc_totcharge","tpc_totrecocharge","tpc_track_charge",
								 "crt_mYX","crt_mZX","crt_nYX","crt_nZX",
							 	 "tpc_mYX","tpc_mZX","tpc_nYX","tpc_nZX",
							 	 "tpc_startX","tpc_endX",
							     "Track_Length","Track_Length[0]","nsamples"};
	TString hist_xtit[nhist] = {"N_{tracks}", "TPC tot charge [10^{6} ADC*ticks]", "TPC tot reco charge [10^{6} ADC*ticks]", "TPC track charge [10^{6} ADC*ticks]",
							 	"CRT m_{YX}", "CRT m_{ZX}", "CRT n_{YX}", "CRT n_{ZX}",
								"TPC m_{YX}", "TPC m_{ZX}", "TPC n_{YX}", "TPC n_{ZX}",
							 	"TPC x_{start} [cm]", "TPC x_{end} [cm]",
								"Track length [cm]","Leading track length [cm]", "N_{samples} (x10^{3})"};
	double minh[nhist] = {0, 0, 0, 0,
						 -10., -0.4, -300., -120., 
						 -10, -10, -300, -300, -60, -60, 0,0}; 
	double maxh[nhist] = {140, 9.E6, 9.E6, 0.7E5,
						 10., 0.2, 300., 120., 
						 10, 10, 300, 300, 55, 55, 300,300,3.E5};
	
	for (int i=0; i<nhist; i++) // nhist; i++)
	{
		c1->Clear();
		c1->cd();
		double sc=1.;
		if (i==1 || i==2 || i==3) sc=1.E6;
		else if (i==nhist-1) sc=1.E3;
		plotHist(dpd,i,hist_names[i],hist_xtit[i],sc,minh[i],maxh[i]);
		if (i<4 || i==14 || i==15) c1->SetLogy(1);
		else c1->SetLogy(0);
		c1->RedrawAxis();
		c1->Print("plots/"+hist_names[i]+".pdf");
	}
	

	/*
	static const int nhist2=3;
	TString hist2_names[nhist2] = {"ntracks","pmt_charge[5]","tpc_totcharge"};
	TString hist2_xtit[nhist2] = {"N_{tracks}","Total PMT charge [#muC]","TPC tot charge [10^{6} ADC*ticks]"};
	double minh2[nhist2] = {0,-0.001E-6,0.};
	double maxh2[nhist2] = {100,0.34E-6,5.E6};
	
	for (int i=0; i<nhist2; i++)
	{
		c1->Clear();
		c1->cd();
		double sc=1.;
		if (i==1) sc=1.E-6;
		else if (i==2) sc=1.E6;
		plotNsamp(dpd,i,hist2_names[i],hist2_xtit[i],sc,minh2[i],maxh2[i]);
		c1->SetLogy(1);
		c1->RedrawAxis();
		c1->Print("plots/"+hist2_names[i]+"_nsamp.pdf");
	}
	*/
	
	// 1D HIST VS. PMT
	TString names[n_names] = {"pmt_charge","pmt_npe","pmt_ped","pmt_pedrms","pmt_ped_end","pmt_pedrms_end",
							  "pmt_S1_charge","pmt_S1_npe","pmt_S1_width","pmt_S1_amp","pmt_S1_tau",
							  "pmt_S2_charge","pmt_S2_npe","pmt_S2_width","pmt_S2_amp","pmt_S2_tau",
							  "pmt_S2_gaus_charge","pmt_S2_gaus_npe","pmt_S2_gaus_width","pmt_S2_gaus_amp","pmt_S2_gaus_tau",
						  	  "tpc_drift_time_at_pmt_pos","0.5*pmt_S1_amp","pmt_ped_end_diff"};
	
	double scale[n_names] = {1.E-6,1.E6,1.,1.,1.,1.,
		1.E-9,1.E3,1.,1.,1.,
		1.E-6,1.E6,1.,1.,1.,
		1.E-6,1.E6,1.,1.,1.,
		1.,1.E3,1.};
	
	TString xtit[n_names] = {"PMT charge [#muC]", "PMT n.p.e. (x10^{6})", "PMT ped [ADU]", "PMT #sigma_{ped} [ADU]", "PMT ped_end [ADU]", "PMT #sigma_{ped_end} [ADU]",
							 "PMT S1 charge [nC]", "PMT S1 n.p.e. (x10^{3})", "PMT S1 width [ns]", "PMT S1 amp [V]", "PMT S1 #tau [#mus]",
						 	 "PMT S2 charge [#muC]", "PMT S2 n.p.e. (x10^{6})", "PMT S2 width [#mus]", "PMT S2 amp [V]", "PMT S2 #tau [#mus]",
							 "PMT S2_gaus charge [#muC]", "PMT S2_gaus n.p.e. (x10^{6})", "PMT S2_gaus width [#mus]", "PMT S2_gaus amp [V]", "PMT S2_gaus #tau [#mus]",
						 	 "TPC drift time at PMT pos [#mus]", "PMT S1 amp/#sigma_{ped}/1000", "(PMT ped_end-pmt_ped)/#sigma_{ped}"};
	double min[n_names] = {-0.001E-6,-0.01E6,3980.,0.,3980.,0., -0.01E-9,-0.01E3,0.,-0.01,0., -0.001E-6,-0.001E6,-0.001,0.,0., -0.001E-6,-0.001E6,-0.0001,0.,0., -5.1,0.,-10.};
	double max[n_names] = {0.34E-6,2.E6,4070,10.,4100,10., 5.E-9,30.E3,100.,2.05,400., 0.4E-6,2.5E6,0.04,0.6,1200., 0.5E-6,2.5E6,350,0.12,1200., 700.1,5000.,20.};
							  
	for (int i=0; i<n_names; i++)
	{
		//if (i>=4) continue;
		//if (i!=14) continue;
		c1->Clear();
		c1->cd();
		plotPMT(dpd,i,names[i],xtit[i],scale[i],min[i],max[i]);
		if (i==2 || i==4 || i==n_names-1) c1->SetLogy(0);
		else c1->SetLogy(1);
		c1->RedrawAxis();
		c1->Modified();
		c1->Update();
		c1->Print("plots/"+names[i]+".pdf");				  	
	}
	
}