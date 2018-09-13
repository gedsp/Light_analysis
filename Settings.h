#ifndef SETTINGS_H
#define SETTINGS_H

static const int N_PMT = 5;
static const int N_LEM = 12;

//set up the input folder where the light-charge matched data is stored.
const string matched_data_dir = "/Volumes/data/root_files/2018Feb05/"; // local 
//const string matched_data_dir = "/eos/user/j/jsotooto/root_files/2018Feb05/"; // on lxplus

// full path to Light_analysis directory                        
const string ana_dir = "/Volumes/data/Dropbox/physics/311/git/Light_analysis"; // local
//const string ana_dir = "/afs/cern.ch/user/l/leyton/Light_analysis"; // on lxplus 


// full path to input/output folder where dpds are stored
const string dpd_dir = ana_dir+"/dpdMaker/dpd"; 

// full path to db file
const string db_file = ana_dir+"/runList/dbVoltages.root";
// full path to analysis input dpd file
const string dpd_file = ana_dir+"/goodRuns_v2_1117.root";

TString run_desc = "goodRunList v2";


// PLOTTING

TString pmt_polar[N_PMT] = {"+","+","--","--","+"};
int colors[N_PMT+1]={kBlue,kRed,kGreen-7,kViolet,kCyan,kBlack};

// CUTS

// calculated by fit_S1_tau.C
float S1_tau_fit_mean[N_PMT] = {229.125, 229.124, 229.187, 229.186, 229.130};
float S1_tau_fit_sigma[N_PMT] = {6.375e-03, 5.314e-03, 5.751e-03, 5.874e-03, 6.182e-03};

#endif