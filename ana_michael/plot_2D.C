#include "TFile.h"
#include "TROOT.h"

#include "../config_analysis.h"

const string fname_2d = "2D.root";

void plot_2D()
{
	TFile f_2d(fname_2d.c_str());
	
	gStyle->SetOptStat(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
	
    Int_t icol=0; // WHITE
    gStyle->SetFrameBorderMode(icol);
    gStyle->SetFrameFillColor(icol);
    gStyle->SetCanvasBorderMode(icol);
    gStyle->SetCanvasColor(icol);
    gStyle->SetPadBorderMode(icol);
    gStyle->SetPadColor(icol);
    gStyle->SetStatColor(icol);
    //gStyle->SetFillColor(icol); // don't use: white fill color floa *all* objects

    // set the paper & margin sizes
    gStyle->SetPaperSize(20,26);

    // set margin sizes
    //gStyle->SetPadTopMargin(0.05);
    //gStyle->SetPadRightMargin(0.05);
    //gStyle->SetPadBottomMargin(0.16);
    //gStyle->SetPadLeftMargin(0.16);

    // set title offsets (for axis label)
    gStyle->SetTitleXOffset(1.25);
    gStyle->SetTitleYOffset(1.25); // was 1.25

    // use large fonts
    //Int_t font=72; // Helvetica italics
    Int_t font=42; // Helvetica
    Double_t tsize=0.05;
    gStyle->SetTextFont(font);

    gStyle->SetTextSize(tsize);
    gStyle->SetLabelFont(font,"x");
    gStyle->SetTitleFont(font,"x");
    gStyle->SetLabelFont(font,"y");
    gStyle->SetTitleFont(font,"y");
    gStyle->SetLabelFont(font,"z");
    gStyle->SetTitleFont(font,"z");
  
    gStyle->SetLabelSize(tsize,"x");
    gStyle->SetTitleSize(tsize,"x");
    gStyle->SetLabelSize(tsize,"y");
    gStyle->SetTitleSize(tsize,"y");
    gStyle->SetLabelSize(tsize,"z");
    gStyle->SetTitleSize(tsize,"z");
	
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	
	TCanvas *c1 = new TCanvas();
	
    TH2 *h;
    TKey *key;
    TIter nextkey(gDirectory->GetListOfKeys());
    while ((key=(TKey*)nextkey())) 
    {
      TObject *obj=key->ReadObj();
      if (obj->IsA()->InheritsFrom("TH2")) 
  	{
  		h = (TH2*)obj;
  		TString name=h->GetName();
		  
		  c1->Clear();
		  c1->cd();
		  h->Draw("colz");
		  
		  TLatex l;
		  l.SetNDC();
		  l.DrawLatex(0.14,0.82,Form("N_{entries} = %0.0f",h->GetEntries()));
		  
		  //TF2* fit = new TF2("fit","xygaus");
		  //h->Fit(fit);
		  c1->SetLogz();
		  c1->RedrawAxis();
		  c1->Print("plots/2D_"+name+".pdf");				  	
	  }
  }
}