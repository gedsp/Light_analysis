TString cut_ntracks = "ntracks>=1";
float pedrms_cutval = 2.0;
float S1_amp_cutval = 1.95;
float S2_gaus_tau_cutval_min = 230.;
float S2_gaus_tau_cutval_max = 850.;

// calculated by fit_S1_tau.C
float S1_tau_fit_mean[N_PMT] = {229.125, 229.124, 229.187, 229.186, 229.130};
float S1_tau_fit_sigma[N_PMT] = {6.375e-03, 5.314e-03, 5.751e-03, 5.874e-03, 6.182e-03};

TString cut_nsamples = "nsamples==262144";
TString cut_track_crt = "crt_matchreco==1";
TString cut_track_fit_yx = "tpc_track_fitresult_yx==0";
TString cut_track_fit_zx = "tpc_track_fitresult_zx==0";

TString cut_track_fit = cut_track_fit_yx+" && "+cut_track_fit_zx;

TString cut_charge = cut_ntracks;

TString cut_S1_tau;
TString cut_pedrms;
TString cut_S1_amp;
TString cut_S2_gaus_tau;

TString cuts;

void build_cuts()
{
	TString cut_S1_tau_pmt[N_PMT];
	TString cut_pedrms_pmt[N_PMT];
	TString cut_S1_amp_pmt[N_PMT];
	TString cut_S2_gaus_tau_pmt[N_PMT];

	for (int k=0; k<N_PMT; k++) 
	{
		cut_S1_tau_pmt[k] = Form("(fabs(pmt_S1_tau[%d]-%f)<3.*%f)",k,S1_tau_fit_mean[k],S1_tau_fit_sigma[k]);
		cut_pedrms_pmt[k] = Form("pmt_pedrms[%d]<%f",k,pedrms_cutval);
		cut_S1_amp_pmt[k] = Form("pmt_S1_amp[%d]<%f",k,S1_amp_cutval);
		cut_S2_gaus_tau_pmt[k] = Form("pmt_S2_gaus_tau[%d]>%f && pmt_S2_gaus_tau[%d]<%f",k,S2_gaus_tau_cutval_min,k,S2_gaus_tau_cutval_max);
	}

	cut_S1_tau = cut_S1_tau_pmt[0];
	cut_pedrms = cut_pedrms_pmt[0];
	cut_S1_amp = cut_S1_amp_pmt[0];
	cut_S2_gaus_tau = cut_S2_gaus_tau_pmt[0];

	for (int k=1; k<N_PMT; k++) 
	{
		cut_S1_tau = cut_S1_tau+" && "+cut_S1_tau_pmt[k];
		cut_pedrms = cut_pedrms+" && "+cut_pedrms_pmt[k];
		cut_S1_amp = cut_S1_amp+" && "+cut_S1_amp_pmt[k];
		cut_S2_gaus_tau = cut_S2_gaus_tau+" && "+cut_S2_gaus_tau_pmt[k];
	}

	cuts = cut_nsamples+" && "+cut_S1_tau+" && "+cut_pedrms+" && "+cut_S1_amp+" && "+cut_S2_gaus_tau+" && "+cut_charge;
	
	cout << "cuts = " << cuts << endl;
}

