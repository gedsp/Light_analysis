#include <iostream>
#include "Riostream.h"
#include <sstream>
#include <string>

//const string infolder = "/eos/user/j/jsotooto/root_files/2018Feb05/"; //set up the input folder where the light-charge matched data is stored.

const string infolder = "/Volumes/data/root_files/2018Feb05/";

#include "../../LIB/mylib.h"
//#include "attach_light_charge.C"
//#include "checks.C"
//#include "evt_display.C"
#include "ana_crt_2018Feb05.h"
#include "../../LIB/gains.h"

static const int NMAX=5000;

void extractPMTvoltages(TString infile="/Volumes/data/Dropbox/physics/311/db/Charge_runs_database.txt", const string outfile="PMTdb.root")
{


//subRunFile Charge_run_number subrun Starttime nevents pmt_voltage_0(V) Error pmt_voltage_1(V) Error pmt_voltage_2(V) Error pmt_voltage_3(V) Error pmt_voltage_4(V) Error
	
	
    ifstream in;
	std::string tmp;
	int a,b,d;
    double c,e,f,g,h,i,j,k,l,m,n;
	
    TNtuple *ntuple = new TNtuple("ntuple","ntuple","run:subrun:pmt0:pmt1:pmt2:pmt3:pmt4");
  
    Int_t nlines = 0;
    in.open(infile);
	int run[NMAX],subrun[NMAX];
    double pmt0[NMAX],pmt1[NMAX],pmt2[NMAX],pmt3[NMAX],pmt4[NMAX];
  
  	char line[50];
    for (int r=0; r<15; r++) {
       in >> line;
       cout<<r<<"\t"<<line<<endl;
     }
	 
    while (1) {
  	  in >> tmp >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> k >> l >> m >> n;
	  if (tmp[0] == '#') continue;
  	  if (!in.good()) break;
	  //a = std::strtod(tmp.c_str(),NULL);
	  run[nlines]=a;
	  subrun[nlines]=b;
  	  pmt0[nlines]=e;
  	  pmt1[nlines]=g;
  	  pmt2[nlines]=i;
  	  pmt3[nlines]=k;
  	  pmt4[nlines]=m;
	  ntuple->Fill(a,b,e,g,i,k,m);
	  if (nlines<10) printf("line %d: %d %d %f %f %f %f %f\n",nlines,a,b,e,g,i,k,m);
  	  nlines++;
    }
	
	in.close();
	
	TFile *fout = new TFile(outfile.c_str(),"RECREATE");
	ntuple->Write();
}
