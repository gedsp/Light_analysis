#ifndef _THREEEXPOGAUSFITSCINTUTILSCH_
#define _THREEEXPOGAUSFITSCINTUTILSCH_

#include<TFitResultPtr.h>
#include<TF1.h>
#include<TProfile.h>
#include<TH1F.h>
#include<TTree.h>
#include<TStyle.h>
#include<TPad.h>
#include<TMath.h>
#include<TMinuit.h>
#include<TMatrixDSym.h>

using namespace std;

double func(double *x, double *par, int index = 3);
double scint_time_1(double *x, double *par);
double scint_time_2(double *x, double *par);
double scint_time_3(double *x, double *par);
	
class ThreeExpoGausFitScintUtilsCh: public TObject {
	
public:
	
	ThreeExpoGausFitScintUtilsCh();
	~ThreeExpoGausFitScintUtilsCh(){};
	
	void Init(int ch);
	void Reset(int ch);

	void Fit(TH1 & h, int ch);

public:
	
	int _fit_n_comp;
	
	TF1 * _func;
	
	TFitResultPtr _fit_res;
		
	double _fit_x_min;
	double _fit_x_max;
	
	double _init_ped    ;
	double _init_t0     ;
	double _init_sigma	;
	double _init_A_fast ;
	double _init_t_fast ;
	double _init_A_int  ;
	double _init_t_int  ;
	double _init_A_slow ;
	double _init_t_slow ;

	double _ped    ;
	double _t0     ;
	double _sigma  ;
	double _A_fast ;
	double _t_fast ;
	double _A_int  ;
	double _t_int  ;
	double _A_slow ;
	double _t_slow ;
	
};

inline ThreeExpoGausFitScintUtilsCh::ThreeExpoGausFitScintUtilsCh(){

  	_fit_n_comp = 3;
	
	_func = 0 ;
	_fit_res = 0;
	
	_fit_x_min = -700;
	_fit_x_max = 3300;
		
	_init_t0      = -1 ;
	_init_sigma   = -1 ;
	_init_A_fast  = -1 ;
	_init_t_fast  = -1 ;
	_init_A_int   = -1 ;
	_init_t_int   = -1 ;
	_init_A_slow  = -1 ;
	_init_t_slow  = -1 ;
	
	_t0     = -1;
	_sigma  = -1;
	_A_fast = -1;
	_t_fast = -1;
	_A_int  = -1;
	_t_int  = -1;
	_A_slow = -1;
	_t_slow = -1;
	
}

inline void ThreeExpoGausFitScintUtilsCh::Init(int ch){
	
	if( _func ){
		delete _func;
	}
	
	if( _fit_n_comp == 3 ){
 
		_func = new TF1("scint_time", scint_time_3, _fit_x_min, _fit_x_max, 9);

		_func->SetParName(0, "Ped"         );		
		_func->SetParName(1, "t_{0}"       );
		_func->SetParName(2, "#sigma"      );
		_func->SetParName(3, "A_{fast}"    );
		_func->SetParName(4, "#tau_{fast}" );
		_func->SetParName(5, "A_{int}"     );
		_func->SetParName(6, "#tau_{int}"  );
		_func->SetParName(7, "A_{slow}"    );
		_func->SetParName(8, "#tau_{slow}" );

		_func->SetParameter(0 , _init_ped    );
		_func->SetParameter(1 , _init_t0     );
		_func->SetParameter(2 , _init_sigma  );
		_func->SetParameter(3 , _init_A_fast );
		_func->SetParameter(4 , _init_t_fast ); 
		_func->SetParameter(5 , _init_A_int  );
		_func->SetParameter(6 , _init_t_int  ); 
		_func->SetParameter(7 , _init_A_slow );
		_func->SetParameter(8 , _init_t_slow );	

		//my setting parameters
		_func->FixParameter(4, _init_t_fast); 
				
	} else {
		
		Fatal("ThreeExpoGausFitScintUtilsCh::Init", "_fit_n_comp = %i not implememnted", _fit_n_comp);

	}	
	
	_func->SetNpx(1000);
	
}

inline void ThreeExpoGausFitScintUtilsCh::Reset(int ch){
	
	delete _func;
	
}

inline void ThreeExpoGausFitScintUtilsCh::Fit(TH1 & h, int ch){

        TString status_fit;
    
	// Pedestal subtraction
	TF1 * _func_pedestal = new TF1("pedestal", "pol0", _fit_x_min ,_fit_x_max);
	TFitResultPtr r = h.Fit(_func_pedestal, "S+", "",  -550, -150);
	_init_ped = _func_pedestal->GetParameter(0);

	int i_max = h.GetMaximumBin();
	double x_max = h.GetBinCenter(i_max);
	double max = h.GetMaximum();
 
	// T0 estimation
	_init_t0 = x_max;

	// Amplitude estimation
	int i_l_max = i_max;
	while( h.GetBinContent(i_l_max) > _init_ped ){
		i_l_max--;
	}

	int i_r_max = i_max;
	while( h.GetBinContent(i_r_max) > max/3. ){
		i_r_max++;
	}

	TF1 * _func_fast = new TF1("around_fast", "gaus", _fit_x_min ,_fit_x_max );
	h.Fit(_func_fast, "LMESR+", "", h.GetBinCenter(i_l_max), h.GetBinCenter(i_r_max));
	_init_A_fast = _func_fast->Integral( _func_fast->GetParameter(1) - 5*_func_fast->GetParameter(2) , _func_fast->GetParameter(1) + 5*_func_fast->GetParameter(2));
	_init_sigma = _func_fast->GetParameter(2);


	// Int component estimation
	int min_expo = _init_t0 + 1*_init_sigma; 
	int max_expo = _init_t0 + 7*_init_sigma;
	TF1 * _func_int = new TF1("int", "expo", min_expo, max_expo);

	h.Fit(_func_int, "LMESR+");

	_init_A_int = _init_A_fast/10.;
	_init_t_int =  - 1 / _func_int->GetParameter(1);
	
	// Slow component estimation
	TF1 * _func_slow = new TF1("slow", "expo", 100, 3000);//3300);//500,1000 ,4000);
	
	h.Fit(_func_slow, "LMESR+");

	_func_pedestal->Draw("same");
	_func_fast->Draw("same");
	_func_int->Draw("same");	
	_func_slow->Draw("same");
	
	_init_t_fast = 6;

	_init_A_slow = _init_A_fast/100.;
	_init_t_slow =  - 1 / _func_slow->GetParameter(1);
	
	cout << "           ---------   " << endl;		
	cout << "       ----> Ch: "<< ch <<" <----" << endl;
	cout << "           ---------   " << endl;
	cout << "Init Ped      : " << _init_ped << endl;
	cout << "Init t0       : " << _init_t0 << endl;
	cout << "Init sigma    : " << _init_sigma << endl;
	cout << "Init A_{Fast} : " << _init_A_fast << endl;
	cout << "Init t_{Fast} : " << _init_t_fast << endl;	
	cout << "Init A_{Int}  : " << _init_A_int << endl;
	cout << "Init t_{Int}  : " << _init_t_int << endl;	
	cout << "Init A_{Slow} : " << _init_A_slow << endl;
	cout << "Init t_{Slow} : " << _init_t_slow << endl;	
	
	Init(ch);

	_fit_res = h.Fit(_func, "S0WLEMR");
	h.Draw("same");
	
	cout << "           ---------   " << endl;		
	cout << "     ----> I'm here!   " << endl;
	cout << "           ---------   " << endl;

}

// Fit functions
double func(double *x, double *par, int index) {
	
	double P = par[0]; // Pedestal

	double TT  = par[1]; // PMT Transit Time
	double TTS = par[2]; // PMT Transit Time Spread (Jitter)
	
	double A   = par[index]; // Normalisation
	double tau = par[index+1]; // Time constant 

	double t = x[0]; // Variable
	
	double c = A*4.;//*1000;// if scale the histo
	c/=(2.*tau);

	double t1 = (TTS *TTS) / (2 * tau * tau);
		
	double t2 = t - TT; 
	t2 /= tau;		

	double t3 = (TTS * TTS) - (tau * (t-TT));
	t3 /= (TMath::Sqrt(2) * TTS * tau);
		
	return P + c * TMath::Exp( t1 - t2 ) * ( 1 - TMath::Erf(t3) );

}

double scint_time_1(double *x, double *par){
	return func(x, par, 3);
}

double scint_time_2(double *x, double *par){	
	return func(x, par, 3) + func(x, par, 5);
}

double scint_time_3(double *x, double *par){	
	return func(x, par, 3) + func(x, par, 5) + func(x, par, 7);
}

#endif
