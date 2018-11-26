#include "../config_analysis.h"

TString run_desc2 = "Runs 768-1199";

vector<int> makeRunList(TString name, TTree* t, int ch, int run_min, int run_max, float volt=1100, float tol=5)
{
	Int_t run;
	Float_t pmt[N_PMT]={0};
	Float_t lemu[N_LEM]={0};
	Float_t lemd[N_LEM]={0};
	
	vector<int> r;
	
	t->SetBranchAddress("run",&run);
	t->SetBranchAddress("pmt",&pmt); 
	t->SetBranchAddress("lemu",&lemu);
	t->SetBranchAddress("lemd",&lemd);
	
	Int_t nentries = (Int_t)t->GetEntries();
	Int_t i=0;
	while (i<nentries)
	{
		t->GetEntry(i);
		if (run>=run_min && run<=run_max && fabs(pmt[ch]-volt)<tol) r.push_back(run);
		i++;
	}

	//printf("%s size = %d\n",name.Data(),(int)r.size());
	
	if (r.size()>0)
	{
		printf("hadd dpd-20181114-%s.root ",name.Data());
		for (int j=0; j<r.size(); j++) printf("light/dpd-light-%d.root ",r.at(j));
		printf("\n");
	}
	
	return r;
}

void pmtVoltages()
{
	TFile *f = TFile::Open(db_light_file.c_str());
	TTree *t = (TTree*)f->Get("ntuple");
	
	vector<int> r2A = makeRunList("light-2A",t,2,0,608);
	vector<int> r3A = makeRunList("light-3A",t,3,0,608);
	
	vector<int> r2B = makeRunList("light-2B",t,2,609,934);
	vector<int> r3B = makeRunList("light-3B",t,3,609,934);
	
	vector<int> r2C = makeRunList("light-2C",t,2,935,1063);
	vector<int> r3C = makeRunList("light-3C",t,3,935,1063);
	
	vector<int> r2D = makeRunList("light-2D",t,2,1064,1716);
	vector<int> r3D = makeRunList("light-3D",t,3,1064,1716);
	
	vector<int> r2E = makeRunList("light-2E",t,2,1717,9999);
	vector<int> r3E = makeRunList("light-3E",t,3,1717,9999);
	
	return;
}
