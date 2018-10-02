#include <iostream>
#include "Riostream.h"
#include <sstream>
#include <string>

static const int NMAX=5000;

void extractAllVoltages(TString infile="/Volumes/data/Dropbox/physics/311/db/Charge_runs_database_2.txt", const string outfile="dbVoltages.root")
{

	/*
tmp: subRunFile	
a: Charge_run_number
b: subrun
c: Starttime
d: nevents	
e,f: grid(V)
g,h: cath(V)
i,j:lemu[0](V)
k,l:lemd[0](V)
m,n:lemu[1](V)
o,p:lemd[1](V)
q,r:lemu[2](V)
s,t:lemd[2](V)
u,v:lemu[3](V)
w,x:lemd[3](V)
y,z:lemu[4](V)
aa,bb:lemd[4](V)
cc,dd:lemu[5](V)
ee,ff:lemd[5](V)
gg,hh:lemu[6](V)
ii,jj:lemd[6](V)
kk.ll:lemu[7](V)
mm,nn:lemd[7](V)
oo,pp:lemu[8](V)
qq,rr:lemd[8](V)
ss,tt:lemu[9](V)
uu,vv:lemd[9](V)
ww,xx:lemu[10](V)
yy,zz:lemd[10](V)
aaa,bbb:lemu[11](V)
ccc,ddd:lemd[11](V)	
eee,fff:dl(mm)
ggg,hhh:ext_field_l(kV/cm)
iii,jjj:ext_field_g(kV/cm)
kkk.lll:amp_field(kV/cm)
mmm,nnn:drift_field(kV/cm)
ooo,ppp:pmt_voltage_0(V)
qqq,rrr:pmt_voltage_1(V)
sss,ttt:pmt_voltage_2(V)
uuu,vvv:pmt_voltage_3(V)
www,xxx:pmt_voltage_4(V)
*/

	
    ifstream in;
	std::string tmp;
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
    for (int rrrr=0; rrrr<77; rrrr++) {
       in >> line;
       cout<<rrrr<<"\t"<<line<<endl;
     }
	 
    while (1) {
  	  in >> tmp >> run >> subrun >> trash[0] >> trash[1] >> grid >> grid_err >> cath >> cath_err >> 
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
	  
	  
	  if (run>=997 && run<=1004) {
	  //if (run==839 || run==840 || run==842 || (run>=984 && run<=1014) || run==1016 || run==1032 || (run>=1034 && run<=1038)) {
	  	if (fabs(grid_err/grid) > 0.005) printf("line %d: grid err/val = %0.2f%%\n",nlines,100.*grid_err/grid);
	  	if (fabs(cath_err/cath) > 0.005) printf("line %d: cath err/val = %0.2f%%\n",nlines,100.*cath_err/cath);
	  	if (fabs(dl_err/dl) > 0.005) printf("line %d: dl err/val = %0.2f%%\n",nlines,100.*dl_err/dl);
	  	if (fabs(ext_field_l_err/ext_field_l) > 0.005) printf("line %d: ext_field_l err/val = %0.2f%%\n",nlines,100.*ext_field_l_err/ext_field_l);
	  	if (fabs(ext_field_g_err/ext_field_g) > 0.005) printf("line %d: ext_field_g err/val = %0.2f%%\n",nlines,100.*ext_field_g_err/ext_field_g);
	  	if (fabs(amp_field_err/amp_field) > 0.005) printf("line %d: amp_field err/val = %0.2f%%\n",nlines,100.*amp_field_err/amp_field);
	  	if (fabs(drift_field_err/drift_field) > 0.005) printf("line %d: drift_field err/val = %0.2f%%\n",nlines,100.*drift_field_err/drift_field);
 	 }
	 
	 if (run>=768 && run<=838) nsamples=1000;
	 else if (run>=839 && run<=1038) nsamples=262144;
	 else if (run>=1165 && run<=1199) nsamples=1000;
	 else { cout << "Nsamples unknown for run " << run << endl; nsamples=0; }

	  ntuple->Fill();
	  if (nlines<10) printf("line %d: %d %d %f %f %f %f %f, %f\n",nlines,run,subrun,grid,cath,lemu[0],lemd[0],pmt[0],pmt[1]);
  	  nlines++;
    }
	
	cout << "total nlines = " << nlines << endl;
	in.close();
	
	TFile *fout = new TFile(outfile.c_str(),"RECREATE");
	ntuple->Write();
}
