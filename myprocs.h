//---------------------------------------------------------------------------
#ifndef myprocsH
#define myprocsH     
#include <math.h>
#include <stdlib.h> 
#include <complex.h>
#include <Classes.hpp>
#include "zvectbase.h"
//---------------------------------------------------------------------------
using namespace std;
typedef complex<double> comp;
typedef complex<long double> compl;
typedef complex<float> compf;
typedef unsigned __int8 byte;
const double Pi = M_PI;
const double Pi2 = 2*M_PI;
const double R2G = 180.0/M_PI;
const double G2R = M_PI/180.0;
const comp compj=comp(0.,1.);

inline double myrand() { return (double)rand()/(double)(RAND_MAX); }

inline double random(double a, double b)
{
   return a + (b-a)*myrand();
};

inline int random(int a, int b)
{
   return a + ((b-a)*rand())/RAND_MAX;
};

inline double sqr(double a) { return a*a; }
inline double sign(double a) { return (a>=0.) ? 1.0 : -1.0; }
inline int pow2(int pw) { return 1<<pw; }
inline int Mod(int a,int b) { div_t dt; dt=div(a,b); return dt.rem; }
inline int Div(int a,int b) { div_t dt; dt=div(a,b); return dt.quot; }
inline double round(double x) { return (x-floor(x))>=0.5 ? floor(x)+1:floor(x); }
inline void beep(int a)	{ MessageBeep(a);	}
double __fastcall gauss(double m,double sigma);
int __fastcall poisson(double m)
{
   int r = 0;
   double A = myrand();
   double Th = exp(-m);
   while(A>Th)
   {
      A*=myrand();
      r++;
   };
   return r;
};

double __fastcall stdvar(double *X,double avr,int N);
double __fastcall avr(double *X,int N);
AnsiString __fastcall C2SExp(comp X,int num);
AnsiString __fastcall F2S(double X,int num);
AnsiString __fastcall I2S(int X);
AnsiString __fastcall B2S(bool X);
int __fastcall S2I(AnsiString X);
double __fastcall S2F(AnsiString X);
void __fastcall SaveCompMatrixToFile(AnsiString fname, AnsiString Comment, int Fidelity, comp **A, int a1, int b1, int a2, int b2);
void __fastcall SaveDoubleMatrixToFile(AnsiString fname, AnsiString Comment, int Fidelity, double **A, int a1, int b1, int a2, int b2);
void __fastcall SaveFloatMatrixToFile(AnsiString fname, AnsiString Comment, int Fidelity, float **A, int a1, int b1, int a2, int b2);
void __fastcall SaveIntMatrixToFile(AnsiString fname, AnsiString Comment, int **A, int a1, int b1, int a2, int b2);
int __fastcall ReadFileStr(int H, AnsiString &str);
int __fastcall WriteFileStr(int H, AnsiString str);
AnsiString FloatStrComma2Point(AnsiString str);
AnsiString FloatStrPoint2Comma(AnsiString str);
AnsiString RemoveSpaces(AnsiString &str);
AnsiString RemoveSemiColons(AnsiString &str);
AnsiString RemoveAfterSemiColons(AnsiString &str);
void __fastcall Normalize360(double &A);
void __fastcall Normalize180(double &A);
//---------------------------------------------------------------------------
template <class T>
T **  __fastcall matrix(T **&A,int a1,int b1,int a2,int b2)
{
   T **tmp = (new T*[b1-a1+1]) - a1;
   for (int i=a1; i<=b1; i++) tmp[i] = new T[b2-a2+1] - a2;
   return tmp;
};
//---------------------------------------------------------------------------
template <class T>
void  __fastcall free(T **&A,int a1,int b1,int a2,int b2)
{
   for (int i=a1; i<=b1; i++) delete[] (A[i] + a2);
   delete[] (A + a1);
   A = NULL;
};
//---------------------------------------------------------------------------
template <class T>
T *  __fastcall vector(T *&vector,int a1,int b1)
{
   T *tmp = (new T[b1-a1+1]) - a1;
   return tmp;
};
//---------------------------------------------------------------------------
template <class T>
void __fastcall free(T *&vector,int a1,int b1)
{
   delete[] (vector + a1);
   vector = NULL;
};
//---------------------------------------------------------------------------
template <class T>
T __fastcall ArrayMax(T *array,int a1,int b1)
{
   T max;
   max = array[a1];
   for (int i=a1+1; i<=b1; i++) if (max < array[i]) max = array[i];
   return max;
};
//---------------------------------------------------------------------------
template <class T>
T __fastcall ArrayMin(T *array,int a1,int b1)
{
   T min;
   min = array[a1];
   for (int i=a1+1; i<=b1; i++) if (min > array[i]) min = array[i];
   return min;
};
//---------------------------------------------------------------------------
template <class T>
void __fastcall ArrayIndexMax(T *array,int a1,int b1,T &max,int &Index)
{
   max = array[a1];
   Index = a1;
   for (int i=a1+1; i<=b1; i++)
   if (max<array[i])
   {
      Index = i;
      max = array[i];
   };
};
//---------------------------------------------------------------------------
template <class T>
void __fastcall ArrayIndexMin(T *array,int a1,int b1,T &min,int &Index)
{
   min = array[a1];
   Index = a1;
   for (int i=a1+1; i<=b1; i++)
   if (min>array[i])
   {
      Index = i;
      min = array[i];
   };
};
//---------------------------------------------------------------------------
template <class T>
inline void __fastcall DeleteAndClear(T *pointer)
{
   if (pointer) delete pointer;
   pointer = 0;
};
//---------------------------------------------------------------------------
template <class T>
ArrayIndexMin(T *array,int a1,int b1,T &min,int &Index)
{
   min = array[a1];
   Index = a1;
   for (int i=a1+1; i<=b1; i++)
   if (min>array[i])
   {
      Index = i;
      min = array[i];
   };
};
//---------------------------------------------------------------------------
class TDigitalFilter
{
  private:
    double *A;
    double *B;
    int NA;
    int NB;
  protected:
  public:
    __fastcall TDigitalFilter(AnsiString AFileName);
    ~TDigitalFilter();
    void __fastcall Filtering(int N,double *X,double *Y);
};
//---------------------------------------------------------------------------
class TDoplerPelengator
{
  public:
    double *Data1;
    double *Data2;
    int N;
    __fastcall TDoplerPelengator(AnsiString FName1,AnsiString FName2);
    ~TDoplerPelengator();
};
//---------------------------------------------------------------------------
//void __fastcall QDemodulation(double *InSignal,int N,double F0,double Fd,AnsiString FilterName,double *ACos,double *ASin);
//---------------------------------------------------------------------------
//void __fastcall QCompDemodulation(double *InSignal,int N,double F0,double Fd,AnsiString FilterName,comp *CompSignal);
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline double __fastcall cmod(double R,double I)     // вычисление модуля комплексного числа без переполнений
{
  double AR,AI;
  AR = fabs(R);
  AI = fabs(I);
  if (AR<AI) return AI*sqrt(1.0 + (AR/AI)*(AR/AI));
  else if (AR>AI) return AR*sqrt(1.0 + (AI/AR)*(AI/AR));
    else return AR*sqrt(2.0);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline float __fastcall cmodf(float R,float I)     // вычисление модуля комплексного числа без переполнений
{
  float AR,AI;
  AR = fabs(R);
  AI = fabs(I);
  if (AR<AI) return AI*sqrt(1.0 + (AR/AI)*(AR/AI));
  else if (AR>AI) return AR*sqrt(1.0 + (AI/AR)*(AI/AR));
    else return AR*sqrt(2.0);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
template <class T>
void __fastcall StraightSort(T *a, int L, int R)
{
   int i,j,k;
   T x;
   for (i=L; i<R; i++)
   {
      k = i;
      x = a[i];
      for (j=i+1; j<=R; j++)
      {
         if (a[j] < x)
         {
            k = j;
            x = a[k];
         };
      };
      a[k] = a[i];
      a[i] = x;
   };
   return;
};
//---------------------------------------------------------------------------
class TPrmFile
{
   private:
      int H;
      AnsiString Separator;
      TStringList *strarr;
      int Error;
   public:
   __fastcall TPrmFile(AnsiString FileName, AnsiString aSeparator);
   __fastcall ~TPrmFile();
   int __fastcall GetPrm(AnsiString PrmName, AnsiString &Prm);
   int __fastcall GetPrm(AnsiString PrmName, int &Prm);
   int __fastcall GetPrm(AnsiString PrmName, double &Prm);
   int __fastcall GetPrm(AnsiString PrmName, bool &Prm);
   bool __fastcall CheckError();
};
//---------------------------------------------------------------------------
void __fastcall Brenzehem(int x1, int y1, int x2, int y2, TCanvas *Canvas);
//---------------------------------------------------------------------------
class TDataStat
{
 private:
   int fN;
   double fD;
   double fM;
 public:
 
 __property int N = {read = fN};

   double __fastcall GetMean() {return fM/double(fN);};
   double __fastcall GetDisp() {return fD/double(fN);};
   double __fastcall GetRMS()
   {
      double A = fM/double(fN);
      A = fD/double(fN) - A*A;
      if (A<1e-15) return 0;
      else return sqrt(A);
   };
   void __fastcall Add(double data)
   {
      fD += data*data;
      fM += data;
      fN++;
   };
 __fastcall TDataStat() {fD = 0.0; fM = 0.0; fN = 0; }
};
//---------------------------------------------------------------------------
#endif
