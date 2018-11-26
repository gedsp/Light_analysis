#ifndef CUTS_H
#define CUTS_H

// USER-DEFINED CUTS

TString cut_time = "fabs(time_charge-time_light)<0.01";

TString cut_ntracks = "ntracks>=1";
//TString cut_ntracks = "_S2_tau_end < 900.";
 //n_sel_tracks==1 && n_tracks<=4
int   npeaks_cutval = 1;
float pedrms_cutval = 2.0;
//float S1_amp_cutval = 1.95; // 1.95;
float S2_tau_start_cutval_min = -900.; // 236.;
float S2_tau_start_cutval_max = 10000.; // 840.;
float S2_gaus_tau_cutval_min = 230.;
float S2_gaus_tau_cutval_max = 850.;
float S1_delta_tau_cutval = 15.;


// add cut on npeaks
// add cut on tau_S2_start and end

// calculated by fit_S1_tau.C

float S1_tau_fit_mean[N_PMT] = {229.125, 229.123, 229.187, 229.186, 229.130};
float S1_tau_fit_sigma[N_PMT] = {5.499e-03, 4.945e-03, 5.841e-03, 5.912e-03, 6.263e-03};

float S1_delta_tau_fit_mean[N_PMT] = {4.257, 4.424, 4.926, 3.722, 4.183};
float S1_delta_tau_fit_sigma[N_PMT] = {7.578e-01, 7.983e-01, 5.998e-01, 5.404e-01, 7.772e-01};

float S1_amp_min[N_PMT] = {0.125,0.125,0.125,0.125,0.125};
float S1_amp_max[N_PMT] = {0.6,0.6,1.2,1.2,0.6};


TString cut_charge = cut_ntracks;


// DO NOT EDIT BELOW

TString cut_nsamples = "nsamples==262144";
TString cut_track_crt = "crt_matchreco==1";
TString cut_track_fit_yx = "tpc_track_fitresult_yx==0";
TString cut_track_fit_zx = "tpc_track_fitresult_zx==0";

TString cut_track_fit = cut_track_fit_yx+" && "+cut_track_fit_zx;

TString cut_S1_tau;
TString cut_S1_delta_tau;
TString cut_npeaks;
TString cut_pedrms;
TString cut_S1_amp;
TString cut_S2_amp;
TString cut_S2_tau_start;
TString cut_S2_gaus_tau;
TString cut_wvf_ADC_sat;

TString cut_S1_tau_pmt[N_PMT];
TString cut_S1_delta_tau_pmt[N_PMT];
TString cut_npeaks_pmt[N_PMT];
TString cut_pedrms_pmt[N_PMT];
TString cut_S1_amp_pmt[N_PMT];
TString cut_S2_amp_pmt[N_PMT];
TString cut_S2_gaus_tau_pmt[N_PMT];
TString cut_S2_tau_start_pmt[N_PMT];
TString cut_wvf_ADC_sat_pmt[N_PMT];
TString cuts;

void build_cuts()
{
	for (int k=0; k<N_PMT; k++) 
	{
		cut_S1_tau_pmt[k] = Form("(fabs(pmt_S1_tau[%d]-%f)<5.*%f)",k,S1_tau_fit_mean[k],S1_tau_fit_sigma[k]);
		cut_S1_delta_tau_pmt[k] = Form("(pmt_S1_tau_end[%d]-pmt_S1_tau[%d])>%f && (pmt_S1_tau_end[%d]-pmt_S1_tau[%d])<%f",
		k,k,0.,k,k,S1_delta_tau_cutval);
		cut_S1_amp_pmt[k] = Form("pmt_S1_amp[%d]>%f && pmt_S1_amp[%d]<%f",k,S1_amp_min[k],k,S1_amp_max[k]);
		cut_S2_amp_pmt[k] = Form("pmt_S2_amp[%d]>%f && pmt_S2_amp[%d]<%f",k,S1_amp_min[k],k,S1_amp_max[k]);
		cut_npeaks_pmt[k] = Form("pmt_npeaks[%d]==%d",k,npeaks_cutval);
		cut_pedrms_pmt[k] = Form("pmt_pedrms[%d]<%f",k,pedrms_cutval);
		cut_S2_tau_start_pmt[k] = Form("pmt_S2_tau_start[%d]>%f && pmt_S2_tau_start[%d]<%f",k,S2_tau_start_cutval_min,k,S2_tau_start_cutval_max);
		cut_S2_gaus_tau_pmt[k] = Form("pmt_S2_gaus_tau[%d]>%f && pmt_S2_gaus_tau[%d]<%f",k,S2_gaus_tau_cutval_min,k,S2_gaus_tau_cutval_max);
		cut_wvf_ADC_sat_pmt[k] = Form("pmt_wvf_ADC_sat[%d]==0",k);
	}

	cut_S1_tau = cut_S1_tau_pmt[0];
	cut_S1_delta_tau = cut_S1_delta_tau_pmt[0];
	cut_npeaks = cut_npeaks_pmt[0];
	cut_pedrms = cut_pedrms_pmt[0];
	cut_S1_amp = cut_S1_amp_pmt[0];
	cut_S2_amp = cut_S2_amp_pmt[0];
	cut_S2_gaus_tau = cut_S2_gaus_tau_pmt[0];
	cut_S2_tau_start = cut_S2_tau_start_pmt[0];
	cut_wvf_ADC_sat = cut_wvf_ADC_sat_pmt[0];

	for (int k=1; k<N_PMT; k++) 
	{
		cut_S1_tau = cut_S1_tau+" && "+cut_S1_tau_pmt[k];
		cut_S1_delta_tau = cut_S1_delta_tau+" && "+cut_S1_delta_tau_pmt[k];
		cut_npeaks = cut_npeaks+" && "+cut_npeaks_pmt[k];
		cut_pedrms = cut_pedrms+" && "+cut_pedrms_pmt[k];
		cut_S1_amp = cut_S1_amp+" && "+cut_S1_amp_pmt[k];
		cut_S2_amp = cut_S2_amp+" && "+cut_S2_amp_pmt[k];
		cut_S2_tau_start = cut_S2_tau_start+" && "+cut_S2_tau_start_pmt[k];
		cut_S2_gaus_tau = cut_S2_gaus_tau+" && "+cut_S2_gaus_tau_pmt[k];
		cut_wvf_ADC_sat = cut_wvf_ADC_sat+" && "+cut_wvf_ADC_sat_pmt[k];
	}

	//cuts = cut_nsamples+" && "+cut_S1_tau+" && "+cut_pedrms+" && "+cut_S1_amp+" && "+cut_S2_gaus_tau+" && "+cut_charge;
	cuts = cut_nsamples+" && "+cut_S1_tau+" && "+cut_S1_delta_tau+" && "+cut_S1_amp+" && "+cut_npeaks+" && "+cut_pedrms+" && "+cut_wvf_ADC_sat+" && "+cut_charge;
	
	cout << "cuts = " << cuts << endl << endl;
}

#endif
