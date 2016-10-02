//---------------------------------------------------------------------------
#ifndef mymathH
#define mymathH
#include "myprocs.h"

enum {wRect,wTriangle,wHann,wHemming,wNattol,wGauss};

void __fastcall cpoly(double *OPR,double *OPI,int DEGREE,double *ZEROR,double *ZEROI,bool &FAIL);


__fastcall FFT(comp * fft,double * sampl,int L,comp *coef);
__fastcall FFT(comp * fft,comp * sampl,int L,comp *coef);
__fastcall IFFT(comp * ifft,comp * sampl,int L,comp *coef);
comp * __fastcall initFFTcoeff(int L);
comp __fastcall GetCorrelationViaSpectralDomain(comp *X1, comp *X2, int N);
comp __fastcall GetCorrelationViaSpectralDomain(double *X1, double *X2, int N);




void __fastcall svd(double **a, int m, int n, double *w, double **v);
void __fastcall cevd(int N,comp **AA,double *EV,comp **VV,int &IERR);
void __fastcall cevd(int N, float **AARe, float **AAIm, float *EV, float **VVRe, float **VVIm, int &IERR);
void __fastcall cevd(int N, double **AARe, double **AAIm, double *EV, double **VVRe, double **VVIm, int &IERR);
void __fastcall MatrixInv(comp **A,const int N,int *S);
void __fastcall MatrixInv(compf **A,const int N,int *S);
void __fastcall MatrixInv(float **Re, float **Im, const int N,int *S);
void __fastcall MatrixInv(compl **A,const int N,int *S);
void __fastcall MatrixInv(double **Re, double **Im, const int N,int *S);
bool __fastcall ModCovar(double Norm,double Td,int N,int NPSD,int IP,double * X1,double * &FR,double * &PSD);
bool __fastcall BURG(double Norm,double Td,int N,int NPSD,int IP,double * X1,double * &FR,double * &PSD);
bool __fastcall Geometric(double Norm,double Td,int N,int NPSD,int IP,double * X1,double * &FR,double * &PSD);
void __fastcall Correlation(int N,int Lag,int Mode, double *X,double *Y,double &R0,double *R);
bool __fastcall Levinson(int M,double T0,double *T,double &P,double *A);
bool __fastcall YuleWalker(double Norm,double Td,int N,int NPSD,int IP,int Shift,double * X1,double * &FR,double * &PSD);
bool __fastcall MUSIC_EV(double Norm,double Td,int Method,int N,int NPSD,int IP,int &NSIG,double * X1,double * &FR,double * &PSD);
bool __fastcall cModCovar(int N,int IP,comp *X ,double &Rp0,comp *A);
void __fastcall csvd(comp **A,int M,int N,int IP,int NU,int NV,double *S,comp **U,comp **V);
bool __fastcall cMUSIC_EV(double Norm,double Td,int Method,int M,int N,int NPSD,int &NSIG,comp **X,double * &FR,double * &PSD);
bool __fastcall cLARMUSIC(double d2l,int M,int N,int NPSD,int &NSIG,comp **X,double * &FR,double * &PSD);
bool __fastcall Classic(double Norm,double Td,int N,int NPSD,int wKind,double * X1,double * &FR,double * &PSD);
bool __fastcall Danyell(double Norm,double Td,int N,int NPSD,int wKind,int PD,double * X1,double * &FR,double * &PSD);
bool __fastcall Welch(double Norm,double Td,int N,int NPSD,int wKind,int PW,double AW,double * X1,double * &FR,double * &PSD);
double __fastcall Wnd(int n,int kind,int N2);
AnsiString __fastcall WndName(int wKind);
//---------------------------------------------------------------------------
#endif
