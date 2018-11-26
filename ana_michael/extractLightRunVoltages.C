#include <iostream>
#include "Riostream.h"
#include <sstream>
#include <string>

static const int NMAX=5000;

void extractLightRunVoltages(TString infile="/Volumes/data/Dropbox/physics/311/db/Light_runs_database_2.txt", const string outfile="dbVoltages_light.root")
{

	/*
Run 
Starttime 
Date 
Length(hours) 
trigger 
CRT_position 
nevents 
TimeSample(ns) 
nsamples 
rate(Hz) 
grid(V) 
cath(V) 
lemu[0](V) 
lemd[0](V) 
lemu[1](V) 
lemd[1](V) 
lemu[2](V) 
lemd[2](V) 
lemu[3](V) 
lemd[3](V) 
lemu[4](V) 
lemd[4](V) 
lemu[5](V) 
lemd[5](V) 
lemu[6](V) 
lemd[6](V) 
lemu[7](V) 
lemd[7](V) 
lemu[8](V) 
lemd[8](V) 
lemu[9](V) 
lemd[9](V) 
lemu[10](V) 
lemd[10](V) 
lemu[11](V) 
lemd[11](V) 
dl(mm) 
ext_field_l(kV/cm) 
ext_field_g(kV/cm) 
amp_field(kV/cm) 
drift_field(kV/cm) 
pmt_voltage_0(V) 
pmt_voltage_1(V) 
pmt_voltage_2(V) 
pmt_voltage_3(V) 
pmt_voltage_4(V)
	*/

    ifstream in;
	std::string tmp,tmp2,tmp3;
	int run,subrun,nevents;
	float nsamples,grid,cath,dl,ext_field_l,ext_field_g,amp_field,drift_field;
	float grid_err,cath_err,dl_err,ext_field_l_err,ext_field_g_err,amp_field_err,drift_field_err;
	float lemu[12],lemd[12],pmt[5],trash[2];
	float lemu_err[12],lemd_err[12],pmt_err[5];
	
    TTree *ntuple = new TTree("ntuple","ntuple");
	ntuple->Branch("run",&run,"run/I");
	ntuple->Branch("subrun",&subrun,"subrun/I");
	ntuple->Branch("nsamples",&nsamples,"nsamples/F");
	ntuple->Branch("grid",&grid,"grid/F");
	ntuple->Branch("grid_err",&grid_err,"grid_err/F");
	ntuple->Branch("cath",&cath,"cath/F");
	ntuple->Branch("cath_err",&cath_err,"cath_err/F");
	ntuple->Branch("dl",&dl,"dl/F");
	ntuple->Branch("dl_err",&dl_err,"dl_err/F");
	ntuple->Branch("ext_field_l",&ext_field_l,"ext_field_l/F");
	ntuple->Branch("ext_field_l_err",&ext_field_l_err,"ext_field_l_err/F");
	ntuple->Branch("ext_field_g",&ext_field_g,"ext_field_g/F");
	ntuple->Branch("ext_field_g_err",&ext_field_g_err,"ext_field_g_err/F");
	ntuple->Branch("amp_field",&amp_field,"amp_field/F");
	ntuple->Branch("amp_field_err",&amp_field_err,"amp_field_err/F");
	ntuple->Branch("drift_field",&drift_field,"drift_field/F");
	ntuple->Branch("drift_field_err",&drift_field_err,"drift_field_err/F");
	ntuple->Branch("pmt",&(pmt[0]),"pmt[5]/F");
	ntuple->Branch("pmt_err",&(pmt_err[0]),"pmt_err[5]/F");
	ntuple->Branch("lemu",&(lemu[0]),"lemu[12]/F");
	ntuple->Branch("lemu_err",&(lemu_err[0]),"lemu_err[12]/F");
	ntuple->Branch("lemd",&(lemd[0]),"lemd[12]/F");
	ntuple->Branch("lemd_err",&(lemd_err[0]),"lemd_err[12]/F");
	
	
    Int_t nlines = 0;
    in.open(infile);
  
  	char line[50];
    for (int rrrr=0; rrrr<82; rrrr++) {
       in >> line;
       cout<<rrrr<<"\t"<<line<<endl;
     }
		 
    while (1) {
  	  in >> run >> trash[0] >> tmp >> trash[1] >> tmp2 >> tmp3 >> trash[2] >> trash[3] >> 
		  nsamples >> trash[4] >> grid >> grid_err >> cath >> cath_err >> 
		    lemu[0] >> lemu_err[0] >> lemd[0] >> lemd_err[0] >> lemu[1] >> lemu_err[1] >> lemd[1] >> lemd_err[1] >>
				lemu[2] >> lemu_err[2] >> lemd[2] >> lemd_err[2] >> lemu[3] >> lemu_err[3] >> lemd[3] >> lemd_err[3] >>
					lemu[4] >> lemu_err[4] >> lemd[4] >> lemd_err[4] >> lemu[5] >> lemu_err[5] >> lemd[5] >> lemd_err[5] >>
						lemu[6] >> lemu_err[6] >> lemd[6] >> lemd_err[6] >> lemu[7] >> lemu_err[7] >> lemd[7] >> lemd_err[7] >>
							lemu[8] >> lemu_err[8] >> lemd[8] >> lemd_err[8] >> lemu[9] >> lemu_err[9] >> lemd[9] >> lemd_err[9] >>
								lemu[10] >> lemu_err[10] >> lemd[10] >> lemd_err[10] >> lemu[11] >> lemu_err[11] >> lemd[11] >> lemd_err[11] >>
									dl >> dl_err >> ext_field_l >> ext_field_l_err >> ext_field_g >> ext_field_g_err >> amp_field >> amp_field_err >>
										drift_field >> drift_field_err >> pmt[0] >> pmt_err[0] >> pmt[1] >> pmt_err[1] >> pmt[2] >> pmt_err[2] >> 
											pmt[3] >> pmt_err[3] >> pmt[4] >> pmt_err[4];
	  if (tmp[0] == '#') continue;
  	  if (!in.good()) break;
	  //a = std::strtod(tmp.c_str(),NULL);
	  
	  
	  if (1) {
	  //if (run==839 || run==840 || run==842 || (run>=984 && run<=1014) || run==1016 || run==1032 || (run>=1034 && run<=1038)) {
	  	if (fabs(grid_err/grid) > 0.005) printf("line %d: grid err/val = %0.2f%%\n",nlines,100.*grid_err/grid);
	  	if (fabs(cath_err/cath) > 0.005) printf("line %d: cath err/val = %0.2f%%\n",nlines,100.*cath_err/cath);
	  	if (fabs(dl_err/dl) > 0.005) printf("line %d: dl err/val = %0.2f%%\n",nlines,100.*dl_err/dl);
	  	if (fabs(ext_field_l_err/ext_field_l) > 0.005) printf("line %d: ext_field_l err/val = %0.2f%%\n",nlines,100.*ext_field_l_err/ext_field_l);
	  	if (fabs(ext_field_g_err/ext_field_g) > 0.005) printf("line %d: ext_field_g err/val = %0.2f%%\n",nlines,100.*ext_field_g_err/ext_field_g);
	  	if (fabs(amp_field_err/amp_field) > 0.005) printf("line %d: amp_field err/val = %0.2f%%\n",nlines,100.*amp_field_err/amp_field);
	  	if (fabs(drift_field_err/drift_field) > 0.005) printf("line %d: drift_field err/val = %0.2f%%\n",nlines,100.*drift_field_err/drift_field);
 	 }
	 
	 // correct non-sensical LEM voltages due to disconnected LEMs
	 for (int i=0; i<12; i++)
	 {
		 if (lemd[i]>1.E6)
		 {
			 lemd[i]=0.0;
			 lemd_err[i]=0.0;
		 }		
	 }

	  ntuple->Fill();
	  if (nlines<10) printf("line %d: %d %f %f %f %f %f %f\n",nlines,run,grid,cath,lemu[0],lemd[0],pmt[0],pmt[1]);
  	  nlines++;
    }
	
	cout << "total nlines = " << nlines << endl;
	in.close();
	
	TFile *fout = new TFile(outfile.c_str(),"RECREATE");
	ntuple->Write();
}
