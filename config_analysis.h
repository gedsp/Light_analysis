#ifndef CONFIG_ANALYSIS_H
#define CONFIG_ANALYSIS_H

#include "ana_michael/MyStyle.C"
#include "ana_michael/PlotUtils.h"
#include "ana_michael/PlotUtils.C"

static const int N_PMT = 5; // also in config_reco.h 
static const int N_LEM = 12; // also in config_reco.h

#include "ana_michael/cuts.h"

// full path to Light_analysis directory                        
const string ana_dir = "/Volumes/data/Dropbox/physics/311/git/Light_analysis"; // local
//const string ana_dir = "/afs/cern.ch/user/l/leyton/git/Light_analysis"; // on lxplus 

// full path to DPD storage
const string dpd_dir = "/Volumes/data/Dropbox/physics/311/git/Light_reco/dpdMaker/dpd"; // local
//const string dpd_dir = "/afs/cern.ch/user/l/leyton/public/dpd_20181003";

// full path to db files
const string db_charge_file = "/Volumes/data/Dropbox/physics/311/git/Light_reco/dbVoltages_charge.root";
const string db_light_file = "/Volumes/data/Dropbox/physics/311/git/Light_reco/dbVoltages_light.root";

// full path to analysis input dpd file
const string dpd_file = dpd_dir+"/dpd_goodRunList_v3_1107_20181106.root"; // local
const string dpd_file2 = dpd_dir+"/dpd_goodRunList_v3_PMT1100_931_20181018.root"; // local
//const string dpd_file = "/afs/cern.ch/user/l/leyton/public/dpd_goodRunList_v3_1107_20181003.root"; // on lxplus


TString run_desc = "goodRunList v3";


// PLOTTING

TString pmt_base[N_PMT] = {"--","--","+","+","--"};
int colors[N_PMT+1]={kBlue,kRed,kGreen+2,kViolet,kCyan,kBlack};

#endif
