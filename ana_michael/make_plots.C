#include "../config_analysis.h"

static const int nhist = 23;
static const int n_names = 37; // PMT plots

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
		
void plotPMT(TTree* t, TCanvas* c1, int ind, TString name, TString xtitle, double scale=1., double xmin=0, double xmax=0)
{
	TH1D* hist[N_PMT+1];
	
	THStack* hs = new THStack(Form("hs%d",ind),Form("pmt%d",ind));
	hs->Clear();
	//hs->SetMinimum(1.E-8);
	
	float x = 0.68;
	float y = 0.58;
	if (name.Contains("pmt_ped")) x=0.52;
	//else if (ind==9) {x=0.3;y=0.22;}
	else if (name.Contains("pmt_S1_amp")) {x=0.3;y=0.22;}
	else if (name.Contains("0.5*pmt_S1_amp")) {x=0.24;y=0.22;}
	else if (name.Contains("pmt_S2_tau")) {x=0.5;y=0.22;}
	//else if (ind==20) y=0.2;
	TLegend *tleg = new TLegend(x,y,x+0.33,y+0.32);
    tleg->SetFillColor(0);
    tleg->SetBorderSize(0);
    tleg->SetHeader(run_desc.Data()); 
	
	for (int i=0; i<N_PMT; i++)
	{
		TString pmt;
		if      (name.Contains("0.5*pmt_S1_amp")) 		pmt=Form("4096.0*fabs(%s[%d])/pmt_pedrms[%d]",name.Data(),i,i);
		else if (name.Contains("pmt_ped_end_diff")) 	pmt=Form("(pmt_ped_end[%d]-pmt_ped[%d])/pmt_pedrms[%d]",i,i,i);
		else if (name.Contains("pmt_S1overS2_charge")) 	pmt=Form("pmt_S1_charge_1us[%d]/pmt_S2_charge[%d]",i,i);
		else if (name.Contains("pmt_S1overS2_npe")) 	pmt=Form("pmt_S1_npe_1us[%d]/pmt_S2_npe[%d]",i,i);
		else if (name.Contains("pmt_S1overS2_amp")) 	pmt=Form("pmt_S1_amp[%d]/pmt_S2_amp[%d]",i,i);
		else if (name.Contains("pmt_S1overS2_width")) 	pmt=Form("pmt_S1_width[%d]/pmt_S2_width[%d]",i,i);
		else pmt = Form("%s[%d]",name.Data(),i);
		
		TString cut_zero = Form("%s[%d]!=0",name.Data(),i);
		TString myCuts;
		
		if (ind<(n_names-6)) myCuts=cut_S1_tau_pmt[i]+" && "+cut_zero; // cuts+" && "+cut_zero;

		//cout << "cuts = " << cuts << endl;
		t->Draw(Form("%s/%e>>hist[%d](100,%e,%e)",pmt.Data(),scale,ind*10+i,xmin/scale,xmax/scale),cuts);
		hist[i] = (TH1D*)gDirectory->Get(Form("hist[%d]",ind*10+i));
		hist[i]->SetLineColor(colors[i]);
		hs->Add(hist[i]);
		if (i<N_PMT) tleg->AddEntry(hist[i], Form("PMT %d (%s)",i+1,pmt_base[i].Data()), "l");
		else tleg->AddEntry(hist[i], "PMT all", "l");
	}
	
	c1->Clear();
	c1->cd();
	
	hs->Draw("hist.nostack");
	float ymax = hs->GetMaximum();
	if (name.Contains("pmt_pedrms") || !(name.Contains("pmt_ped")))
	{ 
		hs->SetMaximum(15*ymax); 
		if (hs->GetMinimum()==0) hs->SetMinimum(0.11);
	}
	hs->Draw("hist.nostack");
	hs->GetYaxis()->SetTitle("Entries");
	hs->GetXaxis()->SetTitle(xtitle); 
    tleg->Draw();
	hs->Draw("hist.nostack.same");
	
	if (name.Contains("pmt_pedrms")) c1->SetLogy(1);
	else if (name.Contains("pmt_ped")) c1->SetLogy(0);
	else c1->SetLogy(1);
	c1->RedrawAxis();
	c1->Modified();
	c1->Update();
	c1->Print("plots/"+name+".pdf");	
	
	for (int k=0; k<N_PMT; k++) delete hist[k];
	delete hs;
}
	
	
void plotHist(TTree* t, TCanvas* c1, int ind, TString name, TString xtitle, double scale=1., double xmin=0, double xmax=0)
{	
	TString limits="";
	if (name.Contains("crt")) limits=cut_track_crt+" && "+Form("%s!=0",name.Data());
	else if (name.Contains("tpc")) 
	{
		if (name.Contains("YX")) limits=cut_track_fit_yx; 
		else if (name.Contains("ZX")) limits=cut_track_fit_zx;
	}
	if (limits=="") limits=cut_charge;
	else limits=limits+" && "+cut_charge;
	if (name.Contains("tracks")) t->Draw(Form("%s/%e>>h(%d,%e,%e)",name.Data(),scale,(int)(xmax/scale),xmin/scale,xmax/scale),limits); 
	else t->Draw(Form("%s/%e>>h(100,%e,%e)",name.Data(),scale,xmin/scale,xmax/scale),limits); 
	TH1D* h = (TH1D*)gDirectory->Get("h"); 
	h->SetLineColor(kBlack);
	h->Draw("hist");
	h->GetYaxis()->SetTitle("Entries");
	h->GetXaxis()->SetTitle(xtitle);
	h->Draw("hist.same");
	float x = 0.22;
	if (ind<5 || ind==6 || ind==15 || ind==16 || ind==20) x=0.65;
	myTextBold(x,0.8,kBlack,run_desc.Data()); 
	myTextBold(x,0.72,kBlack,Form("N_{entries} = %0.0f",h->GetEntries()));
	
	if (ind<5 || ind==15 || ind==16 || ind==18 || ind==19) c1->SetLogy(1);
	else c1->SetLogy(0);
	c1->RedrawAxis();
	c1->Print("plots/"+name+".pdf");
	
	delete h;
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
	gSystem->Exec("mkdir -p plots");
	
	SetMyStyle();
	
	build_cuts();
	
	TFile* f = TFile::Open(dpd_file.c_str());
	
	TTree* dpd = (TTree*)f->Get("dpd");
	
	TCanvas *c1 = new TCanvas;
	
	// 1D HIST
	
	TString hist_names[nhist] = {"ntracks","n_sel_tracks","tpc_totcharge","tpc_totrecocharge","tpc_track_charge", 
		 						 "crt_mYX","crt_mZX","crt_nYX","crt_nZX",
							 	 "tpc_track_mYX","tpc_track_mZX","tpc_track_nYX","tpc_track_nZX", //12
							 	 "tpc_track_startX","tpc_track_endX",
							     "tpc_track_length","tpc_track_length[0]","nsamples", //17
								 "crt_ToF", "crt_track_lenFV", "crt_pmt_dist",
								 "tpc_track_start_theta", "tpc_track_start_phi"};
								 
	TString hist_xtit[nhist] = {"N_{tracks}", "N_{sel. tracks}", "TPC tot charge [10^{6} ADC*ticks]", "TPC tot reco charge [10^{6} ADC*ticks]", "TPC track charge [10^{6} ADC*ticks]",
							 	"CRT m_{YX}", "CRT m_{ZX}", "CRT n_{YX}", "CRT n_{ZX}",
								"TPC m_{YX}", "TPC m_{ZX}", "TPC n_{YX}", "TPC n_{ZX}",
							 	"TPC x_{start} [cm]", "TPC x_{end} [cm]",
								"Track length [cm]","Leading track length [cm]", "N_{samples} (x10^{3})",
								"CRT ToF", "CRT track length FV [mm]", "CRT Track-PMT distance [mm]",
								"TPC track #theta", "TPC track #phi"};
								
	double minh[nhist] = {0, 0, 0, 0, 0,
						 -10., -0.4, -300., -120., 
						 -10, -10, -300, -300, 
						 -60, -60,
						 0,0,0,
						 -180,0,0,
						 0,-181};

	double maxh[nhist] = {140, 8, 11.E6, 10.E6, 1.1E6,
						 10., 0.2, 300., 120., 
						 10, 10, 300, 300, 
						 55, 55, 
						 400,400,3.E5,
					 	 70,3500,1700,
						 181,181};
	
	for (int i=0; i<nhist; i++) 
	{
		double sc=1.;
		if (hist_names[i].Contains("charge")) sc=1.E6;
		else if (hist_names[i].Contains("nsamples")) sc=1.E3;
		//if (i==2 || i==3 || i==4) sc=1.E6;
		//else if (i==17) sc=1.E3;
		plotHist(dpd,c1,i,hist_names[i],hist_xtit[i],sc,minh[i],maxh[i]);
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
	
	
							  /*
	double scale[n_names] = {1.E-6,1.E6,1.,1.,1.,1.,
		1.E-9,1.E3,1.,1.,1.,
		1.E-6,1.E6,1.,1.,1.,
		1.E-6,1.E6,1.,1.,1.,
		1.,1.E3,1.,
		1.,1.,1.,1.};
							  */
	
	TString names[n_names] = {"pmt_charge","pmt_npe",
							  "pmt_ped","pmt_pedrms","pmt_ped2","pmt_pedrms2","pmt_ped_end","pmt_pedrms_end", //7
							  "pmt_S1_charge_1us","pmt_S1_charge_4us","pmt_S1_charge_80ns","pmt_S1_charge_m2",
							  "pmt_S1_npe_1us","pmt_S1_npe_4us","pmt_S1_npe_80ns","pmt_S1_npe_m2",
							  "pmt_S1_width","pmt_S1_amp","pmt_S1_tau","pmt_S1_tau_end", //19
							  "pmt_S2_charge","pmt_S2_charge_m2","pmt_S2_npe","pmt_S2_npe_m2",
							  "pmt_S2_width","pmt_S2_amp",
							  "pmt_S2_tau","pmt_S2_tau_avg","pmt_S2_tau_start","pmt_S2_tau_end", //29
						  	  "tpc_drift_time_at_pmt_pos","0.5*pmt_S1_amp","pmt_ped_end_diff",
							  "pmt_S1overS2_charge", "pmt_S1overS2_npe", "pmt_S1overS2_amp", "pmt_S1overS2_width"};
							  
	TString xtit[n_names] = {"PMT charge [#muC]", "PMT n_{PE} (x10^{6})", 
							 "PMT ped [ADU]", "PMT #sigma_{ped} [ADU]", "PMT ped2 [ADU]", "PMT #sigma_{ped2} [ADU]", "PMT ped_end [ADU]", "PMT #sigma_{ped_end} [ADU]",
							 "PMT S1 charge 1 #mus [nC]", "PMT S1 charge 4 #mus [nC]","PMT S1 charge 80 ns [nC]","PMT S1 charge m2 [nC]",
							 "PMT S1 n_{PE} 1 #mus (x10^{3})", "PMT S1 n_{PE} 4 #mus (x10^{3})","PMT S1 n_{PE} 80 ns (x10^{3})","PMT S1 n_{PE} m2 (x10^{3})",
							 "PMT S1 width [ns]", "PMT S1 amp [V]", "PMT S1 #tau_{0} [#mus]", "PMT S1 #tau_{end} [#mus]",
						 	 "PMT S2 charge [#muC]", "PMT S2 charge m2 [#muC]", "PMT S2 n_{PE} (x10^{6})", "PMT S2 n_{PE} m2 (x10^{6})",
							 "PMT S2 width [#mus]", "PMT S2 amp [V]", 
							 "PMT S2 #tau [#mus]","PMT S2 #tau_{avg} [#mus]","PMT S2 #tau_{start} [#mus]","PMT S2 #tau_{end} [#mus]",
						 	 "TPC drift time at PMT pos [#mus]", "PMT S1 amp/#sigma_{ped}/1000", "(PMT ped_end-pmt_ped)/#sigma_{ped}",
						 	 "PMT S1/S2 charge", "PMT S1/S2 n_{PE}", "PMT S1/S2 amp", "PMT S1/S2 width"};
							 
	double min[n_names] = {-0.001E-6,-0.01E6,
							3980.,0.,3980,0.,3980.,0., 
							-0.01E-9,-0.01E-9,-0.01E-9,-0.01E-9,
							-0.01E3,-0.01E3,-0.01E3,-0.01E3,
							10.,-0.01,229.08,200., 
							-0.01E-6,-0.01E-6,-0.001E6,-0.001E6,
							0.,0.,
							200.,250.,0.,0.,
							-5.1,0.,-10.,
							0,0,0,0};
							
	double max[n_names] = {0.34E-6,2.E6,
							4070,10.,4070,10.,4100,10., 
							6.E-9,1.1E-8,4.E-9,1.E-8,
							45.E3,60.E3,17.E3,55.E3,
							140.,2.05,229.3,1100., 
							0.35E-6,0.35E-6,2.1E6,2.1E6,
							910,0.8,
							910.,910.,910.,910.,
							700.1,5000.,20.,
							0.15,0.18,80,20.0};
							  
	for (int i=0; i<n_names; i++)
	{
		double sc=1.;
		if (names[i].Contains("S1_charge")) sc=1.E-9;
		else if (names[i].Contains("charge")) sc=1.E-6;
		else if (names[i].Contains("S1_npe")) sc=1.E3;
		else if (names[i].Contains("npe")) sc=1.E6;
		else if (names[i].Contains("0.5*pmt_S1_amp")) sc=1.E3;
 
		c1->Clear();
		c1->cd();
		plotPMT(dpd,c1,i,names[i],xtit[i],sc,min[i],max[i]);
		/*
		if (names[i].Contains("pmt_pedrms")) c1->SetLogy(1);
		else if (names[i].Contains("pmt_ped")) c1->SetLogy(0);
		else c1->SetLogy(1);
		c1->RedrawAxis();
		c1->Modified();
		c1->Update();
		c1->Print("plots/"+names[i]+".pdf");	
		*/			  	
	}
	
}