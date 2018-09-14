#ifndef CONFIG_ANALYSIS_H
#define CONFIG_ANALYSIS_H

#include "ana_michael/MyStyle.C"
#include "ana_michael/PlotUtils.h"
#include "ana_michael/PlotUtils.C"

static const int N_PMT = 5; // also in config_reco.h 
static const int N_LEM = 12;

// full path to Light_analysis directory                        
const string ana_dir = "/Volumes/data/Dropbox/physics/311/git/Light_analysis"; // local
//const string ana_dir = "/afs/cern.ch/user/l/leyton/Light_analysis"; // on lxplus 

// full path to DPD storage
const string dpd_dir = "/Volumes/data/Dropbox/physics/311/git/Light_reco/dpdMaker/dpd";

// full path to db file
const string db_file = "/Volumes/data/Dropbox/physics/311/git/Light_reco/dbVoltages.root";
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