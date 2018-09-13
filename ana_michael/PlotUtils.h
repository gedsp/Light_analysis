#ifndef __PLOTUTILS_H
#define __PLOTUTILS_H

#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

void WA105_LABEL(Double_t x,Double_t y,int font=73,Color_t color=1); 
void WA105_PRELIMINARY_LABEL(Double_t x,Double_t y,int font=73,Color_t color=1); 

TGraphErrors* myTGraphErrorsDivide(TGraphErrors* g1,TGraphErrors* g2);

TGraphAsymmErrors* myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2);

TGraphAsymmErrors* myMakeBand(TGraphErrors* g0, TGraphErrors* g1,TGraphErrors* g2);

void myAddtoBand(TGraphErrors* g1, TGraphAsymmErrors* g2);

TGraphErrors* TH1TOTGraph(TH1 *h1);

void myText(Double_t x,Double_t y,Color_t color,string text, Float_t tsize = -1);
void myTextBold(Double_t x,Double_t y,Color_t color,string text) ;

//void myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,char *text);
void myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor, string text, Int_t mstyle=1001) ;
void myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle,string text,Float_t msize=2.); 

void myLineText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,string text, int linestyle=1);


#endif // __ATLASUTILS_H
