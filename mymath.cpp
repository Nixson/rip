//---------------------------------------------------------------------------
#pragma hdrstop
#include "mymath.h"
double SR,SI,TR,TI,PVR,PVI,ARE,MRE,ETA,INFIN,SMALNO,BASE;
double *PR,*PI,*HR,*HI,*QPR,*QPI,*QHR,*QHI,*SHR,*SHI;
int NN;
//---------------------------------------------------------------------------
inline int ISIGN(const int &a, const int &b)
{
  return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}
//---------------------------------------------------------------------------
double __fastcall scale(int NN,double *PT,double ETA,double INFIN,double SMALNO,double BASE)
{
  double HI,LO,MAX,MIN,X,SC,L;
  HI=sqrt(INFIN);
  LO=SMALNO/ETA;
  MAX=0.0;
  MIN=INFIN;
  for (int i=1; i<=NN; i++)
  {
    X=PT[i];
    if (X>MAX) MAX=X;
    if ((X!=0.0)&&(X<MIN)) MIN=X;
  };
  if ((MIN>=LO)&&(MAX<=HI)) return 1.0;
  X=LO/MIN;
  if (X<=1.0) SC=1.0/(sqrt(MAX)*sqrt(MIN));
  else
  {
    SC=X;
    if (INFIN/SC > MAX) SC=1.0;
  }
  L=log(SC)/log(BASE)+0.5;
  return pow(BASE,L);
};
//---------------------------------------------------------------------------
void __fastcall mcon()
{
	int M=1023,N=-1023,T=52;
	BASE=2.0;
	ETA=pow(BASE,(double)1-T);
	INFIN=BASE*(1.0-pow(BASE,(double)-T))*pow(BASE,(double)M-1);
	SMALNO=pow(BASE,(double)N+3)/pow(BASE,(double)3);
};
//---------------------------------------------------------------------------
void __fastcall cdivid(double AR,double AI,double BR,double BI,double &CR,double &CI)
{
  double R,D,T;
  if ((BR==0)&&(BI==0))
  {
    CR=INFIN;
    CI=INFIN;
    return;
  };
  if (fabs(BR)<fabs(BI))
  {
    R=BR/BI;
    D=BI+R*BR;
    CR=(AR*R+AI)/D;
    CI=(AI*R-AR)/D;
    return;
  };
  R=BI/BR;
  D=BR+R*BI;
  CR=(AR+AI*R)/D;
  CI=(AI-AR*R)/D;
  return;
};
//---------------------------------------------------------------------------
double __fastcall cauchy(int NN,double *PT,double *Q)
{
  double X,XM,F,DX,DF;
  int N;
  PT[NN]=-PT[NN];
  N=NN-1;
  X=exp(log(-PT[NN])-log(PT[1]))/double(N);
  if (PT[N]==0.0) goto _20;
  XM=-PT[NN]/PT[N];
  if (XM<X) X=XM;
_20:
  XM=X*0.1;
  F=PT[1];
  for (int i=2; i<=NN; i++) F=F*XM+PT[i];
  if (F<=0.0) goto _40;
  X=XM;
  goto _20;
_40:
  DX=X;
_50:
  if (fabs(DX/X)<=0.005) goto _70;
  Q[1]=PT[1];
  for (int i=2; i<=NN; i++) Q[i]=Q[i-1]*X+PT[i];
_60:
  F=Q[NN];
  DF=Q[1];
  for (int i=2; i<=N; i++) DF=DF*X+Q[i];
_65:
  DX=F/DF;
  X=X-DX;
  goto _50;
_70:
  return X;
};
//---------------------------------------------------------------------------
void __fastcall noshft(int L1)
{
  double XNI,T1,T2;
  int i,JJ,J;
  int N=NN-1;
  int NM1=N-1;
  for (i=1; i<=N; i++)
  {
    int XNI=NN-i;
    HR[i]=XNI*PR[i]/double(N);
    HI[i]=XNI*PI[i]/double(N);
  };
  for (JJ=1; JJ<=L1; JJ++)
  {
    if (cmod(HR[N],HI[N])<=ETA*10.0*cmod(PR[N],PI[N])) goto _30;
    cdivid(-PR[NN],-PI[NN],HR[N],HI[N],TR,TI);
    for (i=1; i<=NM1; i++)
    {
      J=NN-i;
      T1=HR[J-1];
      T2=HI[J-1];
      HR[J]=TR*T1-TI*T2+PR[J];
      HI[J]=TR*T2+TI*T1+PI[J];
    };
_20:
    HR[1]=PR[1];
    HI[1]=PI[1];
    continue;
_30:
    for (i=1; i<=NM1; i++)
    {
      J=NN-i;
      HR[J]=HR[J-1];
      HI[J]=HI[J-1];
    };
_40:
  HR[1]=0.0;
  HI[1]=0.0;
  };
_50:
};
//---------------------------------------------------------------------------
void __fastcall polyev(int NN,double SR,double SI,double *PR,double *PI,double *QR,double *QI,double &PVR,double &PVI)
{
  double T;
  int I;
  QR[1]=PR[1];
  QI[1]=PI[1];
  PVR=QR[1];
  PVI=QI[1];
  for (I=2; I<=NN; I++)
  {
    T=PVR*SR-PVI*SI+PR[I];
    PVI=PVR*SI+PVI*SR+PI[I];
    PVR=T;
    QR[I]=PVR;
    QI[I]=PVI;
  };
  return;
};
//---------------------------------------------------------------------------
void __fastcall calct(bool &BOOL)
{
  double HVR,HVI;
  int N=NN-1;
  polyev(N,SR,SI,HR,HI,QHR,QHI,HVR,HVI);
  BOOL=(cmod(HVR,HVI)<=ARE*10.0*cmod(HR[N],HI[N]));
  if (BOOL) goto _10;
  cdivid(-PVR,-PVI,HVR,HVI,TR,TI);
  return;
_10:
  TR=0.0;
  TI=0.0;
  return;
};
//---------------------------------------------------------------------------
void __fastcall nexth(bool BOOL)
{
  double T1,T2;
  int N=NN-1;
  if (BOOL) goto _20;
  for (int J=2; J<=N; J++)
  {
    T1=QHR[J-1];
    T2=QHI[J-1];
    HR[J]=TR*T1-TI*T2+QPR[J];
    HI[J]=TR*T2+TI*T1+QPI[J];
  };
_10:
  HR[1]=QPR[1];
  HI[1]=QPI[1];
  return;
_20:
  for (int J=2; J<=N; J++)
  {
    HR[J]=QHR[J-1];
    HI[J]=QHI[J-1];
  };
_30:
  HR[1]=0.0;
  HI[1]=0.0;
  return;
};
//---------------------------------------------------------------------------
double __fastcall errev(int NN,double *QR,double *QI,double MS,double MP,double ARE,double MRE)
{
  double E;
  E=cmod(QR[1],QI[1])*MRE/(ARE+MRE);
  for (int I=1; I<=NN; I++) E=E*MS+cmod(QR[I],QI[I]);
  return (E*(ARE+MRE)-MP*MRE);
};
//---------------------------------------------------------------------------
void __fastcall vrshft(int L3,double &ZR,double &ZI,bool &CONV)
{
  double MP,MS,OMP,RELSTP,R1,R2,TP;
  bool B,BOOL;
  int I,J;
  CONV=false;
  B=false;
  SR=ZR;
  SI=ZI;
  for (I=1; I<=L3; I++)
  {
    polyev(NN,SR,SI,PR,PI,QPR,QPI,PVR,PVI);
    MP=cmod(PVR,PVI);
    MS=cmod(SR,SI);
    if (MP>20.0*errev(NN,QPR,QPI,MS,MP,ARE,MRE)) goto _10;
    CONV=true;
    ZR=SR;
    ZI=SI;
    return;
_10:
    if (I==1) goto _40;
    if ((B)||(MP<OMP)||(RELSTP>=0.05)) goto _30;
    TP=RELSTP;
    B=true;
    if (RELSTP<ETA) TP=ETA;
    R1=sqrt(TP);
    R2=SR*(1.0+R1)-SI*R1;
    SI=SR*R1+SI*(1.0+R1);
    SR=R2;
    polyev(NN,SR,SI,PR,PI,QPR,QPI,PVR,PVI);
    for (J=1; J<=5; J++)
    {
      calct(BOOL);
      nexth(BOOL);
    };
_20:
    OMP=INFIN;
    goto _50;
_30:
    if (0.1*MP>OMP) return;
_40:
    OMP=MP;
_50:
    calct(BOOL);
    nexth(BOOL);
    calct(BOOL);
    if (BOOL) continue;
    RELSTP=cmod(TR,TI)/cmod(SR,SI);
    SR+=TR;
    SI+=TI;
  };
_60:
  return;
};
//---------------------------------------------------------------------------
void __fastcall fxshft(int L2,double &ZR,double &ZI,bool &CONV)
{
  double OTR,OTI,SVSR,SVSI;
  bool TEST,PASD,BOOL;
  int J,I;
  int N=NN-1;
  polyev(NN,SR,SI,PR,PI,QPR,QPI,PVR,PVI);
  TEST=true;
  PASD=false;
  calct(BOOL);
  for (J=1; J<=L2; J++)
  {
    OTR=TR;
    OTI=TI;
    nexth(BOOL);
    calct(BOOL);
    ZR=SR+TR;
    ZI=SI+TI;
    if ((BOOL)||(! TEST)||(J==L2)) continue;
    if (cmod(TR-OTR,TI-OTI)>=0.5*cmod(ZR,ZI)) goto _40;
    if (!PASD)goto _30;
    for (I=1; I<=N; I++)
    {
      SHR[I]=HR[I];
      SHI[I]=HI[I];
    }
_10:
    SVSR=SR;
    SVSI=SI;
    vrshft(10,ZR,ZI,CONV);
    if (CONV) return;
    TEST=false;
    for (I=1; I<=N; I++)
    {
      HR[I]=SHR[I];
      HI[I]=SHI[I];
    };
_20:
    SR=SVSR;
    SI=SVSI;
    polyev(NN,SR,SI,PR,PI,QPR,QPI,PVR,PVI);
    calct(BOOL);
    continue;
_30:
    PASD=true;
    continue;
_40:
    PASD=false;
  };
_50:
  vrshft(10,ZR,ZI,CONV);
  return;
};
//---------------------------------------------------------------------------
void __fastcall cpoly(double *OPR,double *OPI,int DEGREE,double *ZEROR,double *ZEROI,bool &FAIL)
{
  double XX,YY,COSR,SINR,XXX,ZR,ZI,BND;
  int CNT1,CNT2,IDNN2,i;
	bool CONV;

	PR=new double[DEGREE+4];
	PI=new double[DEGREE+4];
	HR=new double[DEGREE+4];
	HI=new double[DEGREE+4];
	QPR=new double[DEGREE+4];
	QPI=new double[DEGREE+4];
	QHR=new double[DEGREE+4];
	QHI=new double[DEGREE+4];
	SHR=new double[DEGREE+4];
	SHI=new double[DEGREE+4];

	mcon();
	ARE=ETA;
	MRE=2.0*sqrt(2.0)*ETA;
	XX=0.70710678;
	YY=-XX;
	COSR=-0.069756474;
	SINR=0.99756405;
	FAIL=false;
	NN=DEGREE+1;
	if ((OPR[1]==0.)&&(OPI[1]==0.))
	{
    FAIL=true;
    delete[] PR; delete[] PI; delete[] HR; delete[] HI; delete[] QPR;
    delete[] QHR; delete[] QHI; delete[] SHR; delete[] SHI; delete[] QPI;
    return;
	};
_10:
  if ((OPR[NN]==0.)&&(OPI[NN]==0.))
  {
    IDNN2=DEGREE-NN+2;
    ZEROR[IDNN2]=0.0;
    ZEROI[IDNN2]=0.0;
    NN=NN-1;
    goto _10;
  };
_20:
  for (i=1;i<=NN;i++)
  {
    PR[i]=OPR[i];
    PI[i]=OPI[i];
    SHR[i]=cmod(PR[i],PI[i]);
  };
_30:
  BND=scale(NN,SHR,ETA,INFIN,SMALNO,BASE);
  if (BND!=1.0)
  {
    for (i=1; i<=NN; i++)
    {
      PR[i]*=BND;
      PI[i]*=BND;
    };
  };
_40:
  if (NN <=2)
  {
    if (NN==1)
    {
      delete[] PR; delete[] PI; delete[] HR; delete[] HI; delete[] QPR;
      delete[] QHR; delete[] QHI; delete[] SHR; delete[] SHI; delete[] QPI;
      return;
    }
    cdivid(-PR[2],-PI[2],PR[1],PI[1],ZEROR[DEGREE],ZEROI[DEGREE]);
    delete[] PR; delete[] PI; delete[] HR; delete[] HI; delete[] QPR;
    delete[] QHR; delete[] QHI; delete[] SHR; delete[] SHI; delete[] QPI;
    return;
  }
_50:
  for (i=1; i<=NN; i++) SHR[i]=cmod(PR[i],PI[i]);
_60:
  BND=cauchy(NN,SHR,SHI);
  for (CNT1=1; CNT1<=2; CNT1++)
  {
    noshft(5);
    for (CNT2=1; CNT2<=9; CNT2++)
    {
      XXX=COSR*XX-SINR*YY;
      YY=SINR*XX+COSR*YY;
      XX=XXX;
      SR=BND*XX;
      SI=BND*YY;
      fxshft(10*CNT2,ZR,ZI,CONV);
      if (!CONV) continue;
      IDNN2=DEGREE-NN+2;
      ZEROR[IDNN2]=ZR;
      ZEROI[IDNN2]=ZI;
      NN--;
      for (i=1;i<=NN;i++)
      {
        PR[i]=QPR[i];
        PI[i]=QPI[i];
      }
_70:
      goto _40;
    };
_90:
  };
_100:
  FAIL=true;
  delete[] PR; delete[] PI; delete[] HR; delete[] HI; delete[] QPR;
  delete[] QHR; delete[] QHI; delete[] SHR; delete[] SHI; delete[] QPI;
  return;
};
//---------------------------------------------------------------------------
/*int __fastcall exchange(int &x,int &L)
{
  int a,b,i;
  int denom=1;
  int bits[30];
  b=0;
  for (i=0; i<L; i++)
  {
    a=x&denom;
    a>>=i;
    denom<<=1;
    bits[i]=a;
  };
  for (i=0; i<L; i++)
  {
    a=bits[L-1-i];
    a<<=i;
    b+=a;
  };
  return b;
}                  */
//---------------------------------------------------------------------------
__fastcall FFT(comp * fft,double * sampl,int L,comp *coef)
{
  bool initcoeff=false;
  if (coef==NULL)
  {
    coef=initFFTcoeff(L);
    initcoeff=true;
  };
  int N=pow2(L);
  comp z1,z2;
  int pow2L1J,pow2J1,pow2J,C1,C2,C3,i,j,k;
  int denom;
  int bits[30];
  for (i=0; i<N; i++)
  {
    denom=1;
    for (j=0; j<L; j++)
    {
      C1=i&denom;
      C1>>=j;
      denom<<=1;
      bits[j]=C1;
    };
    C2=0;
    for (j=0; j<L; j++)
    {
      C1=bits[L-1-j];
      C1<<=j;
      C2+=C1;
    };
    fft[C2]=(comp)sampl[i];
  };
  for (j=0; j<L; j++)
  {
    pow2L1J=1<<(L-1-j);
    pow2J=1<<j;
    pow2J1=pow2J<<1;
    C1=0;
    for (k=0; k<pow2L1J; k++)
    {
      for (i=0; i<pow2J; i++)
      {
        C2=C1+i;
        C3=C2+pow2J;
        z1=fft[C2];
        z2=coef[i*pow2L1J]*fft[C3];
        fft[C2]=z1+z2;
        fft[C3]=z1-z2;
      };
      C1+=pow2J1;
    };
  };
  if (initcoeff) delete[] coef;
  return 0;
};
//---------------------------------------------------------------------------
__fastcall FFT(comp * fft,comp * sampl,int L,comp *coef)
{
  bool initcoeff=false;
  if (coef==NULL)
  {
    coef=initFFTcoeff(L);
    initcoeff=true;
  };
  int N=pow2(L);
  comp z1,z2;
  int pow2L1J,pow2J1,pow2J,C1,C2,C3,i,j,k;
  int denom;
  int bits[30];
  for (i=0; i<N; i++)
  {
    denom=1;
    for (j=0; j<L; j++)
    {
      C1=i&denom;
      C1>>=j;
      denom<<=1;
      bits[j]=C1;
    };
    C2=0;
    for (j=0; j<L; j++)
    {
      C1=bits[L-1-j];
      C1<<=j;
      C2+=C1;
    };
    fft[C2]=sampl[i];
  };
  for (j=0; j<L; j++)
  {
    pow2L1J=1<<(L-1-j);
    pow2J=1<<j;
    pow2J1=pow2J<<1;
    C1=0;
    for (k=0; k<pow2L1J; k++)
    {
      for (i=0; i<pow2J; i++)
      {
        C2=C1+i;
        C3=C2+pow2J;
        z1=fft[C2];
        z2=coef[i*pow2L1J]*fft[C3];
        fft[C2]=z1+z2;
        fft[C3]=z1-z2;
      };
      C1+=pow2J1;
    };
  };
  if (initcoeff) delete[] coef;
  return 0;
};
//---------------------------------------------------------------------------
__fastcall IFFT(comp * ifft,comp * sampl,int L,comp *coef)
{
  bool initcoeff=false;
  if (coef==NULL)
  {
    coef=initFFTcoeff(L);
    initcoeff=true;
  };
  int N=pow2(L);
  comp z1,z2;
  int pow2L1J,pow2J1,pow2J,C1,C2,C3,i,j,k;
  int denom;
  int bits[30];
  for (i=0; i<N; i++)
  {
    denom=1;
    for (j=0; j<L; j++)
    {
      C1=i&denom;
      C1>>=j;
      denom<<=1;
      bits[j]=C1;
    };
    C2=0;
    for (j=0; j<L; j++)
    {
      C1=bits[L-1-j];
      C1<<=j;
      C2+=C1;
    };
    ifft[C2]=conj(sampl[i])/(double)N;
  };
  for (j=0; j<L; j++)
  {
    pow2L1J=1<<(L-1-j);
    pow2J=1<<j;
    pow2J1=pow2J<<1;
    C1=0;
    for (k=0; k<pow2L1J; k++)
    {
      for (i=0; i<pow2J; i++)
      {
        C2=C1+i;
        C3=C2+pow2J;
        z1=ifft[C2];
        z2=coef[i*pow2L1J]*ifft[C3];
        ifft[C2]=z1+z2;
        ifft[C3]=z1-z2;
      };
      C1+=pow2J1;
    };
  };
  if (initcoeff) delete[] coef;
  return 0;
};
//---------------------------------------------------------------------------
comp * __fastcall initFFTcoeff(int L)
{
  comp * coef,C1,C2;
  double S;
  int N=pow2(L-1);
  coef=new comp[N];
  S=M_PI/(double)N;
  C1=comp(cos(S),-sin(S));
  C2=comp(1,0);
  for (int i=0; i<N; i++)
  {
    coef[i]=C2;
    C2*=C1;
  };
  return coef;
};
//---------------------------------------------------------------------------
comp __fastcall GetCorrelationViaSpectralDomain(comp *X1, comp *X2, int N)
{
   int L = (int)round(log(N)/log(2));
   if (N != pow2(L)) throw "error";
   comp *fft1 = new comp[N];
   comp *fft2 = new comp[N];
   FFT(fft1, X1, L, NULL);
   FFT(fft2, X2, L, NULL);
   comp S = 0;
   for (int i=1; i<N; i++)
   {
      S+= fft1[i]*conj(fft2[i]);
   };
   S = S/comp(N*N);
   delete[] fft1;
   delete[] fft2;
   return S;
};
//---------------------------------------------------------------------------
comp __fastcall GetCorrelationViaSpectralDomain(double *X1, double *X2, int N)
{
   int L = (int)round(log(N)/log(2));
   if (N != pow2(L)) throw "error";
   comp *fft1 = new comp[N];
   comp *fft2 = new comp[N];
   FFT(fft1, X1, L, NULL);
   FFT(fft2, X2, L, NULL);
   comp S = 0;
   for (int i=1; i<N/2; i++)
   {
      S+= fft1[i]*conj(fft2[i]);
   };
   delete[] fft1;
   delete[] fft2;
   return S;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall MatrixInv(comp **A,const int N,int *S)
{
  int F,G,O,P,Q,BA,J,K,L,M,I;
  double U,Y,ZERO;
  comp V,W,BH,BB;
  ZERO=0.0;
  if (N==0) return;
  if (N==1) goto _14;
  F=1;
  G=1;
_1:
  J=G;
  Q=F;
  U=ZERO;
  if (F>N) goto _30;
  for (I=F; I<=N; I++)
  {
    Y=abs(A[I][J]);
    if (U>=Y) continue;
    U=Y;
    Q=I;
  };
_30:
  for (J=1; J<=N; J++)
  {
    BH=A[Q][J];
    A[Q][J]=A[F][J];
    A[F][J]=BH;
  };
  S[F]=Q;
  J=G;
  V=comp(1.,0.E0)/A[F][F];
  for (I=1; I<=N; I++) A[I][F]*=-V;
  A[F][F]=V;
  M=0;
_5:
  M=M+1;
  if (M==F) goto _5;
  if (N<M) goto _8;
  BB=A[F][M];
  BA=F-1;
  if (BA==0) goto _33;
  for (O=1; O<=BA; O++) A[O][M]+=A[O][F]*BB;
_33:

  BA=F+1;
  if (BA>N) goto _5;
  for (O=BA; O<=N; O++) A[O][M]+=A[O][F]*BB;
  goto _5;
_8:
  L=J+1;
  if (L>N) goto _32;
  for (M=L; M<=N; M++) A[F][M]*=A[F][G];
_32:
  L=J-1;
  if (L==0) goto _31;
  for (M=1; M<=L; M++) A[F][M]*=A[F][G];
_31:

  F=F+1;
  G=G+1;
  if (F>N) goto _11;
  if (G<=N) goto _1;
_11:
  I=1;
_12:
  P=ISIGN(1,1-N);
  K=N-P;
_13:
  K=K+P;
  J=S[K];
  W=A[I][J];
  A[I][J]=A[I][K];
  A[I][K]=W;
  if (K!=1) goto _13;
  I++;
  if (I<=N) goto _12;
  goto _15;
_14:
  A[1][1]=comp(1.,0.E0)/A[1][1];
_15:
  return;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall MatrixInv(compf **A,const int N,int *S)
{
  int F,G,O,P,Q,BA,J,K,L,M,I;
  float U,Y,ZERO;
  compf V,W,BH,BB;
  ZERO=0.0;
  if (N==0) return;
  if (N==1) goto _14;
  F=1;
  G=1;
_1:
  J=G;
  Q=F;
  U=ZERO;
  if (F>N) goto _30;
  for (I=F; I<=N; I++)
  {
    Y=abs(A[I][J]);
    if (U>=Y) continue;
    U=Y;
    Q=I;
  };
_30:
  for (J=1; J<=N; J++)
  {
    BH=A[Q][J];
    A[Q][J]=A[F][J];
    A[F][J]=BH;
  };
  S[F]=Q;
  J=G;
  V=compf(1.,0.E0)/A[F][F];
  for (I=1; I<=N; I++) A[I][F]*=-V;
  A[F][F]=V;
  M=0;
_5:
  M=M+1;
  if (M==F) goto _5;
  if (N<M) goto _8;
  BB=A[F][M];
  BA=F-1;
  if (BA==0) goto _33;
  for (O=1; O<=BA; O++) A[O][M]+=A[O][F]*BB;
_33:

  BA=F+1;
  if (BA>N) goto _5;
  for (O=BA; O<=N; O++) A[O][M]+=A[O][F]*BB;
  goto _5;
_8:
  L=J+1;
  if (L>N) goto _32;
  for (M=L; M<=N; M++) A[F][M]*=A[F][G];
_32:
  L=J-1;
  if (L==0) goto _31;
  for (M=1; M<=L; M++) A[F][M]*=A[F][G];
_31:

  F=F+1;
  G=G+1;
  if (F>N) goto _11;
  if (G<=N) goto _1;
_11:
  I=1;
_12:
  P=ISIGN(1,1-N);
  K=N-P;
_13:
  K=K+P;
  J=S[K];
  W=A[I][J];
  A[I][J]=A[I][K];
  A[I][K]=W;
  if (K!=1) goto _13;
  I++;
  if (I<=N) goto _12;
  goto _15;
_14:
  A[1][1]=compf(1.,0.E0)/A[1][1];
_15:
  return;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall MatrixInv(float **Re, float **Im, const int N,int *S)
{
   int F,G,O,P,Q,BA,J,K,L,M,I;
   float U, Y, ZERO, tmp;
   float VRe, VIm, WRe, WIm, BHRe, BHIm, BBRe, BBIm;
   ZERO = 0.0;
   if (N == 0) return;
   if (N == 1) goto _14;
   F = 1;
   G = 1;
_1:
   J = G;
   Q = F;
   U = ZERO;
   if (F > N) goto _30;
   for (I=F; I<=N; I++)
   {
      Y = cmodf(Re[I][J], Im[I][J]);
      if (U>=Y) continue;
      U = Y;
      Q = I;
   };
_30:
   for (J=1; J<=N; J++)
   {
      BHRe = Re[Q][J]; BHIm = Im[Q][J];
      Re[Q][J] = Re[F][J]; Im[Q][J] = Im[F][J];
      Re[F][J] = BHRe; Im[F][J] = BHIm;
   };
   S[F] = Q;
   J = G;
   tmp = Re[F][F]*Re[F][F] + Im[F][F]*Im[F][F];
   VRe = Re[F][F]/tmp; VIm = -Im[F][F]/tmp;

   for (I=1; I<=N; I++)
   {
      tmp = -Re[I][F]*VRe + Im[I][F]*VIm;
      Im[I][F] = -Im[I][F]*VRe - Re[I][F]*VIm;
      Re[I][F] = tmp;
   };
   Re[F][F] = VRe; Im[F][F] = VIm;
   M = 0;
_5:
   M = M + 1;
   if (M == F) goto _5;
   if (N<M) goto _8;
   BBRe = Re[F][M]; BBIm = Im[F][M];
   BA = F-1;
   if (BA == 0) goto _33;
   for (O=1; O<=BA; O++)
   {
      Re[O][M] += Re[O][F]*BBRe - Im[O][F]*BBIm;
      Im[O][M] += Im[O][F]*BBRe + Re[O][F]*BBIm;
   };
_33:
   BA = F + 1;
   if (BA > N) goto _5;
   for (O=BA; O<=N; O++)
   {
      Re[O][M] += Re[O][F]*BBRe - Im[O][F]*BBIm;
      Im[O][M] += Im[O][F]*BBRe + Re[O][F]*BBIm;
   };
   goto _5;
_8:
   L = J + 1;
   if (L > N) goto _32;
   for (M=L; M<=N; M++)
   {
      tmp = Re[F][M]*Re[F][G] - Im[F][M]*Im[F][G];
      Im[F][M] = Im[F][M]*Re[F][G] + Re[F][M]*Im[F][G];
      Re[F][M] = tmp;
   };
_32:
   L = J - 1;
   if (L == 0) goto _31;
   for (M=1; M<=L; M++)
   {
      tmp = Re[F][M]*Re[F][G] - Im[F][M]*Im[F][G];
      Im[F][M] = Im[F][M]*Re[F][G] + Re[F][M]*Im[F][G];
      Re[F][M] = tmp;
   };
_31:
   F = F + 1;
   G = G + 1;
   if (F > N) goto _11;
   if (G <= N) goto _1;
_11:
   I = 1;
_12:
   P = ISIGN(1,1-N);
   K = N - P;
_13:
   K = K + P;
   J = S[K];
   WRe = Re[I][J]; WIm = Im[I][J];
   Re[I][J] = Re[I][K]; Im[I][J] = Im[I][K];
   Re[I][K] = WRe; Im[I][K] = WIm;
   if (K!=1) goto _13;
   I++;
   if (I<=N) goto _12;
   goto _15;
_14:
   tmp = Re[1][1]*Re[1][1] + Im[1][1]*Im[1][1];
   Re[1][1] = Re[1][1]/tmp; Im[1][1] = -Im[1][1]/tmp;
_15:
   return;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall MatrixInv(double **Re, double **Im, const int N,int *S)
{
   int F,G,O,P,Q,BA,J,K,L,M,I;
   double U, Y, ZERO, tmp;
   double VRe, VIm, WRe, WIm, BHRe, BHIm, BBRe, BBIm;
   ZERO = 0.0;
   if (N == 0) return;
   if (N == 1) goto _14;
   F = 1;
   G = 1;
_1:
   J = G;
   Q = F;
   U = ZERO;
   if (F > N) goto _30;
   for (I=F; I<=N; I++)
   {
      Y = cmod(Re[I][J], Im[I][J]);
      if (U>=Y) continue;
      U = Y;
      Q = I;
   };
_30:
   for (J=1; J<=N; J++)
   {
      BHRe = Re[Q][J]; BHIm = Im[Q][J];
      Re[Q][J] = Re[F][J]; Im[Q][J] = Im[F][J];
      Re[F][J] = BHRe; Im[F][J] = BHIm;
   };
   S[F] = Q;
   J = G;
   tmp = Re[F][F]*Re[F][F] + Im[F][F]*Im[F][F];
   VRe = Re[F][F]/tmp; VIm = -Im[F][F]/tmp;

   for (I=1; I<=N; I++)
   {
      tmp = -Re[I][F]*VRe + Im[I][F]*VIm;
      Im[I][F] = -Im[I][F]*VRe - Re[I][F]*VIm;
      Re[I][F] = tmp;
   };
   Re[F][F] = VRe; Im[F][F] = VIm;
   M = 0;
_5:
   M = M + 1;
   if (M == F) goto _5;
   if (N<M) goto _8;
   BBRe = Re[F][M]; BBIm = Im[F][M];
   BA = F-1;
   if (BA == 0) goto _33;
   for (O=1; O<=BA; O++)
   {
      Re[O][M] += Re[O][F]*BBRe - Im[O][F]*BBIm;
      Im[O][M] += Im[O][F]*BBRe + Re[O][F]*BBIm;
   };
_33:
   BA = F + 1;
   if (BA > N) goto _5;
   for (O=BA; O<=N; O++)
   {
      Re[O][M] += Re[O][F]*BBRe - Im[O][F]*BBIm;
      Im[O][M] += Im[O][F]*BBRe + Re[O][F]*BBIm;
   };
   goto _5;
_8:
   L = J + 1;
   if (L > N) goto _32;
   for (M=L; M<=N; M++)
   {
      tmp = Re[F][M]*Re[F][G] - Im[F][M]*Im[F][G];
      Im[F][M] = Im[F][M]*Re[F][G] + Re[F][M]*Im[F][G];
      Re[F][M] = tmp;
   };
_32:
   L = J - 1;
   if (L == 0) goto _31;
   for (M=1; M<=L; M++)
   {
      tmp = Re[F][M]*Re[F][G] - Im[F][M]*Im[F][G];
      Im[F][M] = Im[F][M]*Re[F][G] + Re[F][M]*Im[F][G];
      Re[F][M] = tmp;
   };
_31:
   F = F + 1;
   G = G + 1;
   if (F > N) goto _11;
   if (G <= N) goto _1;
_11:
   I = 1;
_12:
   P = ISIGN(1,1-N);
   K = N - P;
_13:
   K = K + P;
   J = S[K];
   WRe = Re[I][J]; WIm = Im[I][J];
   Re[I][J] = Re[I][K]; Im[I][J] = Im[I][K];
   Re[I][K] = WRe; Im[I][K] = WIm;
   if (K!=1) goto _13;
   I++;
   if (I<=N) goto _12;
   goto _15;
_14:
   tmp = Re[1][1]*Re[1][1] + Im[1][1]*Im[1][1];
   Re[1][1] = Re[1][1]/tmp; Im[1][1] = -Im[1][1]/tmp;
_15:
   return;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall MatrixInv(compl **A,const int N,int *S)
{
  int F,G,O,P,Q,BA,J,K,L,M,I;
  long double U,Y,ZERO;
  compl V,W,BH,BB;
  ZERO=0.0;
  if (N==0) return;
  if (N==1) goto _14;
  F=1;
  G=1;
_1:
  J=G;
  Q=F;
  U=ZERO;
  if (F>N) goto _30;
  for (I=F; I<=N; I++)
  {
    Y=abs(A[I][J]);
    if (U>=Y) continue;
    U=Y;
    Q=I;
  };
_30:
  for (J=1; J<=N; J++)
  {
    BH=A[Q][J];
    A[Q][J]=A[F][J];
    A[F][J]=BH;
  };
  S[F]=Q;
  J=G;
  V=compl(1.,0.E0)/A[F][F];
  for (I=1; I<=N; I++) A[I][F]*=-V;
  A[F][F]=V;
  M=0;
_5:
  M=M+1;
  if (M==F) goto _5;
  if (N<M) goto _8;
  BB=A[F][M];
  BA=F-1;
  if (BA==0) goto _33;
  for (O=1; O<=BA; O++) A[O][M]+=A[O][F]*BB;
_33:

  BA=F+1;
  if (BA>N) goto _5;
  for (O=BA; O<=N; O++) A[O][M]+=A[O][F]*BB;
  goto _5;
_8:
  L=J+1;
  if (L>N) goto _32;
  for (M=L; M<=N; M++) A[F][M]*=A[F][G];
_32:
  L=J-1;
  if (L==0) goto _31;
  for (M=1; M<=L; M++) A[F][M]*=A[F][G];
_31:

  F=F+1;
  G=G+1;
  if (F>N) goto _11;
  if (G<=N) goto _1;
_11:
  I=1;
_12:
  P=ISIGN(1,1-N);
  K=N-P;
_13:
  K=K+P;
  J=S[K];
  W=A[I][J];
  A[I][J]=A[I][K];
  A[I][K]=W;
  if (K!=1) goto _13;
  I++;
  if (I<=N) goto _12;
  goto _15;
_14:
  A[1][1]=compl(1.,0.E0)/A[1][1];
_15:
  return;
};
//---------------------------------------------------------------------------
void __fastcall csvd(comp **A,int M,int N,int IP,int NU,int NV,double *S,comp **U,comp **V)
{
  double *B=vector(B,0,N+1);
  double *C=vector(C,0,N+1);
  double *T=vector(T,0,N+1);
  double ETA=1.2e-7;
  double TOL=2.4e-32;
  int NP=N+IP;
  int N1=N+1;
  double EPS,Z,W,CS,SN,F,X,Y,H,G;
  comp R,Q;
  int K,K1,I,J,L,L1,KK,LL;

// Редукция(понижение порядка) Хаусхолдера
  C[1]=0.;
  for (K=1; K<=N; K++)
  {
    K1=K+1;
// Исключение элементов A[I][K], I=K+1,...,M
    Z=0.;
    for(I=K; I<=M; I++) Z+=norm(A[I][K]);
    B[K]=0.;
    if (Z>TOL)
    {
      Z=sqrt(Z);
      B[K]=Z;
      W=abs(A[K][K]);
      Q=comp(1.,0.);
      if (W !=0) Q=A[K][K]/W;
      A[K][K]=Q*(Z+W);
      if (K!=NP)
      {
        for (J=K1; J<=NP; J++)
        {
          Q=comp(0.,0.);
          for (I=K; I<=M; I++) Q+=conj(A[I][K])*A[I][J];
          Q=Q/(Z*(Z+W));
          for (I=K; I<=M; I++) A[I][J]-=Q*A[I][K];
        };
// Преобразование фазы
        Q=-conj(A[K][K])/abs(A[K][K]);
        for (J=K1; J<=NP; J++) A[K][J]*=Q;
      };
    };
// Исключение элементов A[K][J], J=K+2,...,N
    if (K==N) break;
    Z=0.;
    for (J=K1; J<=N; J++) Z+=norm(A[K][J]);
    C[K1]=0.;
    if (Z>TOL)
    {
      Z=sqrt(Z);
      C[K1]=Z;
      W=abs(A[K][K1]);
      Q=comp(1.,0.);
      if (W!=0.) Q=A[K][K1]/W;
      A[K][K1]=Q*(Z+W);
      for (I=K1; I<=M; I++)
      {
        Q=comp(0.,0.);
        for (J=K1; J<=N; J++) Q+=conj(A[K][J])*A[I][J];
        Q/=Z*(Z+W);
        for (J=K1; J<=N; J++) A[I][J]-=Q*A[K][J];
      };
// Преобразование фазы
      Q=-conj(A[K][K1])/abs(A[K][K1]);
      for (I=K1; I<=M; I++) A[I][K1]*=Q;
    }
  };

// Допуск для пренебрежимо малых элементов
  EPS=0.;
  for (K=1; K<=N; K++)
  {
    S[K]=B[K];
    T[K]=C[K];
    EPS=max(EPS,S[K]+T[K]);
  };
  EPS*=ETA;
// Инициализация вычислений U и V
  if (NU!=0)
  {
    for (J=1; J<=NU; J++)
    {
      for (I=1; I<=M; I++) U[I][J]=comp(0.,0.);
      U[J][J]=comp(1.,0.);
    };
  };
  if (NV!=0)
  {
    for (J=1; J<=NV; J++)
    {
      for (I=1; I<=N; I++) V[I][J]=comp(0.,0.);
      V[J][J]=comp(1.,0.);
    };
  };
// QR - диагонализация

  for (KK=1; KK<=N; KK++)
  {
    K=N1-KK;
// Проверка на ращепление
_220:
    for (LL=1; LL<=K; LL++)
    {
      L=K+1-LL;
      if (fabs(T[L])<=EPS) goto _290;
      if (fabs(S[L-1])<=EPS) goto _240;
    };
// Сокращение B[L]
_240:
    CS=0.;
    SN=1.;
    L1=L-1;
    for (I=L; I<=K; I++)
    {
      F=SN*T[I];
      T[I]*=CS;
      if (fabs(F)<=EPS) goto _290;
      H=S[I];
      W=sqrt(F*F+H*H);
      S[I]=W;
      CS=H/W;
      SN=-F/W;
      if (NU==0) goto _260;
      for (J=1; J<=N; J++)
      {
        X=real(U[J][L1]);
        Y=real(U[J][I]);
        U[J][L1]=comp(X*CS+Y*SN,0.);
        U[J][I]=comp(Y*CS-X*SN,0.);
      };
_260:
      if (NP==N) continue;
      for (J=N1; J<=NP; J++)
      {
        Q=A[L1][J];
        R=A[I][J];
        A[L1][J]=Q*CS+R*SN;
        A[I][J]=R*CS-Q*SN;
      };
    };
// Проверка сходимости
_290:
    W=S[K];
    if (L==K) goto _360;
// Сдвиг начала координат
    X=S[L];
    Y=S[K-1];
    G=T[K-1];
    H=T[K];
    F=((Y-W)*(Y+W)+(G-H)*(G+H))/(2.*H*Y);
    G=sqrt(F*F+1.);
    if (F<0) G=-G;
    F=((X-W)*(X+W)+(Y/(F+G)-H)*H)/X;
// QR - шаг
    CS=1.;
    SN=1.;
    L1=L+1;
    for (I=L1; I<=K; I++)
    {
      G=T[I];
      Y=S[I];
      H=SN*G;
      G*=CS;
      W=sqrt(H*H+F*F);
      T[I-1]=W;
      CS=F/W;
      SN=H/W;
      F=X*CS+G*SN;
      G=G*CS-X*SN;
      H=Y*SN;
      Y*=CS;
      if (NV==0) goto _310;
      for (J=1; J<=N; J++)
      {
        X=real(V[J][I-1]);
        W=real(V[J][I]);
        V[J][I-1]=comp(X*CS+W*SN,0.);
        V[J][I]=comp(W*CS-X*SN,0.);
      };
_310:
      W=sqrt(H*H+F*F);
      S[I-1]=W;
      CS=F/W;
      SN=H/W;
      F=CS*G+SN*Y;
      X=CS*Y-SN*G;
      if (NU!=0)
      {
        for (J=1; J<=N; J++)
        {
          Y=real(U[J][I-1]);
          W=real(U[J][I]);
          U[J][I-1]=comp(Y*CS+W*SN,0.);
          U[J][I]=comp(W*CS-Y*SN,0.);
        };
      };
      if (N==NP) continue;
      for (J=N1; J<=NP; J++)
      {
        Q=A[I-1][J];
        R=A[I][J];
        A[I-1][J]=Q*CS+R*SN;
        A[I][J]=R*CS-Q*SN;
      };
    };
    T[L]=0.;
    T[K]=F;
    S[K]=X;
    goto _220;
// Сходимость
_360:
    if (W>=0.) continue;
    S[K]=-W;
    if (NV==0) continue;
    for (J=1; J<=N; J++) V[J][K]=-V[J][K];
    continue;
  };
// Упорядочение сингулярных чисел
  for (K=1; K<=N; K++)
  {
    G=-1;
    J=K;
    for (I=K; I<=N; I++)
    {
      if (S[I]<=G) continue;
      G=S[I];
      J=I;
    };
    if (J==K) continue;
    S[J]=S[K];
    S[K]=G;
    if (NV!=0)
    for (I=1; I<=N; I++)
    {
      Q=V[I][J];
      V[I][J]=V[I][K];
      V[I][K]=Q;
    };
    if (NU!=0)
    for (I=1; I<=N; I++)
    {
      Q=U[I][J];
      U[I][J]=U[I][K];
      U[I][K]=Q;
    };
    if (N!=NP)
    for (I=N1; I<=NP; I++)
    {
      Q=A[J][I];
      A[J][I]=A[K][I];
      A[K][I]=Q;
    };
  };
// Обратное преобразование
  if (NU!=0)
  {
    for (KK=1; KK<=N; KK++)
    {
      K=N1-KK;
      if (B[K]==0) continue;
      Q=-A[K][K]/abs(A[K][K]);
      for (J=1; J<=NU; J++) U[K][J]*=Q;
      for (J=1; J<=NU; J++)
      {
        Q=comp(0.,0.);
        for (I=K; I<=M; I++) Q+=conj(A[I][K])*U[I][J];
        Q/=abs(A[K][K])*B[K];
        for (I=K; I<=M; I++) U[I][J]-=Q*A[I][K];
      };
    };
  };
  if ((NV==0)||(N<2))
  {
    free(B);
    free(C);
    free(T);
    return;
  };
  for (KK=2; KK<=N; KK++)
  {
    K=N1-KK;
    K1=K+1;
    if (C[K1]==0) continue;
    Q=-conj(A[K][K1])/abs(A[K][K1]);
    for (J=1; J<=NV; J++) V[K1][J]*=Q;
    for (J=1; J<=NV; J++)
    {
      Q=comp(0.,0.);
      for (I=K1; I<=N; I++) Q+=A[K][I]*V[I][J];
      Q/=abs(A[K][K1])*C[K1];
      for (I=K1; I<=N; I++) V[I][J]-=Q*conj(A[K][I]);
    };
  };
free(B);
free(C);
free(T);
return;
};
//---------------------------------------------------------------------------
inline double FMAX(double a,double b)
{
	return a>=b ? a:b;
};
//---------------------------------------------------------------------------
inline int IMIN(int a,int b)
{
	return a<=b ? a:b;
};
//---------------------------------------------------------------------------
inline double SIGN(const double &a, const double &b)
{
  return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}
//---------------------------------------------------------------------------
void __fastcall svd(double **a, int m, int n, double *w, double **v) 	//Given a matrix a[1..m][1..n], this routine computes its singular value decomposition,	//A=U*W*VT. ThematrixUreplaces a on output. The diagonal matrix of singular values W is out- //put as a vector w[1..n]. The matrix V(not the transpose VT ) is output as v[1..n][1..n].
{
	int flag,i,its,j,jj,k,l,nm;
	double anorm,c,f,g,h,s,scale,x,y,z,*rv1;
	rv1=vector(rv1,1,n);
	g=scale=anorm=0.0;											                //Householder reduction to bidiagonal form.
	for (i=1;i<=n;i++)
  {
    l=i+1;
		rv1[i]=scale*g;
		g=s=scale=0.0;
		if (i <= m)
    {
			for (k=i;k<=m;k++) scale += fabs(a[k][i]);
			if (scale)
      {
				for (k=i;k<=m;k++)
        {
					a[k][i] /= scale;
					s += a[k][i]*a[k][i];
				}
				f=a[i][i];
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][i]=f-g;
				for (j=l;j<=n;j++)
        {
					for (s=0.0,k=i;k<=m;k++) s += a[k][i]*a[k][j];
					f=s/h;
					for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
				}
				for (k=i;k<=m;k++) a[k][i] *= scale;
			}
		}
		w[i]=scale *g;
		g=s=scale=0.0;
		if(i<= m && i != n)
    {
			for (k=l;k<=n;k++) scale += fabs(a[i][k]);
			if (scale)
      {
        for (k=l;k<=n;k++)
        {
          a[i][k] /= scale;
					s += a[i][k]*a[i][k];
				}
				f=a[i][l];
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][l]=f-g;
				for (k=l;k<=n;k++) rv1[k]=a[i][k]/h;
				for (j=l;j<=m;j++)
        {
					for (s=0.0,k=l;k<=n;k++) s += a[j][k]*a[i][k];
					for (k=l;k<=n;k++) a[j][k] += s*rv1[k];
				}
				for (k=l;k<=n;k++) a[i][k] *= scale;
			}
		}
		anorm=FMAX(anorm,(fabs(w[i])+fabs(rv1[i])));
  };
	for (i=n;i>=1;i--)
  { 					//Accumulation of right-hand transformations.
		if(i < n)
    {
			if (g)
      {
				for (j=l;j<=n;j++) //Double division to avoid possible under ow.
				v[j][i]=(a[i][j]/a[i][l])/g;
				for (j=l;j<=n;j++)
        {
					for (s=0.0,k=l;k<=n;k++) s += a[i][k]*v[k][j];
					for (k=l;k<=n;k++) v[k][j] += s*v[k][i];
				}
			}
			for (j=l;j<=n;j++) v[i][j]=v[j][i]=0.0;
		}
		v[i][i]=1.0;
		g=rv1[i];
		l=i;
	}
	for (i=IMIN(m,n);i>=1;i--)
  { 								//Accumulation of left-hand transformations.
		l=i+1;
		g=w[i];
		for (j=l;j<=n;j++) a[i][j]=0.0;
		if (g)
    {
			g=1.0/g;
			for (j=l;j<=n;j++)
      {
				for (s=0.0,k=l;k<=m;k++) s += a[k][i]*a[k][j];
				f=(s/a[i][i])*g;
				for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
			}
      for (j=i;j<=m;j++) a[j][i] *= g;
		}
    else for (j=i;j<=m;j++) a[j][i]=0.0;
		++a[i][i];
	}

	for (k=n;k>=1;k--)
  { 							//Diagonalization of the bidiagonal form: Loop over
								//singular values, and over allowed iterations.
    for (its=1;its<=30;its++)
    {
			flag=1;
			for (l=k;l>=1;l--)
      { 					//Test for splitting.
				nm=l-1; 		//Note that rv1[1] is always zero.
				if ((double)(fabs(rv1[l])+anorm) == anorm)
        {
					flag=0;
					break;
				}
				if ((double)(fabs(w[nm])+anorm) == anorm) break;
			}
			if (flag)
      {
        c=0.0;					//Cancellation of rv1[l], ifl>1.
				s=1.0;
				for (i=l;i<=k;i++)
        {
					f=s*rv1[i];
					rv1[i]=c*rv1[i];
					if ((double)(fabs(f)+anorm) == anorm) break;
					g=w[i];
					h=cmod(f,g);
					w[i]=h;
					h=1.0/h;
					c=g*h;
					s = -f*h;
					for (j=1;j<=m;j++)
          {
						y=a[j][nm];
						z=a[j][i];
						a[j][nm]=y*c+z*s;
						a[j][i]=z*c-y*s;
					}
				}
			}
			z=w[k];
			if (l == k)
      { 									//Convergence.
				if (z < 0.0)
        { 								//Singular value is made nonnegative.
					w[k] = -z;
					for (j=1;j<=n;j++) v[j][k] = -v[j][k];
				}
				break;
			}
			if (its == 30) throw "no convergence in 30 svdcmp iterations";
			x=w[l]; 							//Shiftfrom bottom 2-by-2minor.
			nm=k-1;
			y=w[nm];
			g=rv1[nm];
			h=rv1[k];
			f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
			g=cmod(f,1.0);
			f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
			c=s=1.0; 							//Next QR transformation:
			for (j=l;j<=nm;j++)
      {
				i=j+1;
				g=rv1[i];
				y=w[i];
				h=s*g;
				g=c*g;
				z=cmod(f,h);
				rv1[j]=z;
				c=f/z;
				s=h/z;
				f=x*c+g*s;
				g = g*c-x*s;
				h=y*s;
				y *= c;
				for (jj=1;jj<=n;jj++)
        {
					x=v[jj][j];
					z=v[jj][i];
					v[jj][j]=x*c+z*s;
					v[jj][i]=z*c-x*s;
				}
				z=cmod(f,h);
				w[j]=z; 					//Rotation can be arbitrary if z = 0.
				if (z)
        {
					z=1.0/z;
					c=f*z;
					s=h*z;
				}
        f=c*g+s*y;
				x=c*y-s*g;
				for (jj=1;jj<=m;jj++)
        {
					y=a[jj][j];
					z=a[jj][i];
					a[jj][j]=y*c+z*s;
					a[jj][i]=z*c-y*s;
				}
			}
			rv1[l]=0.0;
			rv1[k]=f;
			w[k]=x;
		}
	}
	free(rv1,1,n);
/******* Ordering of the Singular Values by absolute magnitude *********/
  for (k=1; k<=n; k++)
  {
    j=k;
    scale=-1;
    for (i=k; i<=n; i++)            // finding maximum in w[k]...w[n]
    {
			if (w[i]<=scale) continue;
      scale=w[i];
      j=i;
    };
    if (j==k) continue;
    w[j]=w[k];
    w[k]=scale;
    for (i=1; i<=n; i++)
    {
      scale=v[i][j];
      v[i][j]=v[i][k];
      v[i][k]=scale;
    };

    for (i=1; i<=m; i++)
    {
      scale=a[i][j];
      a[i][j]=a[i][k];
      a[i][k]=scale;
    };
	};
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall cevd(int N,comp **AA,double *EV,comp **VV,int &IERR)
{
      //DATA  SYS051/1.1107652D-16/
  int I,J,A,B,C,D,E,F,R,S,T,U,V,W,X,Y,BI,BJ,BK,BL,N2,NA,ND,NBK,NA2,NB2,NBK2,ND2,NC2;
  double G,H,K,L,M,O,P,Q,Z,BA,BB,BC,BD,BE,BF,BG,SYS051,BM,BN,BO;
  double *RAB=new double[3*N+5];
  SYS051=1.0e-15;
  N2=2*N;
  RAB[N2]=1.0;
  RAB[3*N]=0.0;
  for (A=1; A<=N; A++)
  {
    EV[A]=real(AA[A][A]);
  };
  for (E=1; E<=N; E++)
  {
    A=N+1-E;
    D=A-1;
    M=0.0;
    Q=0.0;
    if (D<1) goto _3;
    for (C=1; C<=D; C++)
    {
      Q=Q+fabs(real(AA[A][C]))+fabs(imag(AA[A][C]));
    };
    ND=N+D;
    NA=N+A;
    NA2=N2+A;
    ND2=N2+D;
    if (Q!=0.0) goto _4;
    RAB[ND]=1.0;
    RAB[ND2]=0.0;
_3: RAB[A]=0.0;
    goto _15;
_4: for (C=1; C<=D; C++)
    {
      AA[A][C]=AA[A][C]/comp(Q);
      M=M+norm(AA[A][C]);
    };
    K=sqrt(M);
    RAB[A]=Q*K;
    G=abs(AA[A][D]);
    if (G==0.0) goto _6;
    RAB[ND]=(imag(AA[A][D])*RAB[NA2]-real(AA[A][D])*RAB[NA])/G;
    P=(real(AA[A][D])*RAB[NA2]+imag(AA[A][D])*RAB[NA])/G;
    M=M+G*K;
    K=1.0+K/G;
    AA[A][D]=AA[A][D]*comp(K);

    if (D==1) goto _13;
    goto _7;
_6: RAB[ND]=-RAB[NA];
    P=RAB[NA2];
    (AA[A][D])._M_re=K;
_7: G=0.0;
    for (B=1; B<=D; B++)
    {
      NB2=N2+B;
      K=0.0;
      L=0.0;
      for (C=1; C<=B; C++)
      {
        K=K+AA[B][C]._M_re*AA[A][C]._M_re+AA[B][C]._M_im*AA[A][C]._M_im;
        L=L-AA[B][C]._M_re*AA[A][C]._M_im+AA[B][C]._M_im*AA[A][C]._M_re;
      };
      F=B+1;
      if (D<F) goto _10;
      for (C=F; C<=D; C++)
      {
        K=K+AA[C][B]._M_re*AA[A][C]._M_re-AA[C][B]._M_im*AA[A][C]._M_im;
        L=L-AA[C][B]._M_re*AA[A][C]._M_im-AA[C][B]._M_im*AA[A][C]._M_re;
_9:   };
_10:  RAB[B]=K/M;
      RAB[NB2]=L/M;
      G=G+RAB[B]*AA[A][B]._M_re-RAB[NB2]*AA[A][B]._M_im;
    };
    O=G/(M+M);
    for (B=1; B<=D; B++)
    {
      NB2=N2+B;
      G=AA[A][B]._M_re;
      K=RAB[B]-O*G;
      RAB[B]=K;
      H=-AA[A][B]._M_im;
      L=RAB[NB2]-O*H;
      RAB[NB2]=-L;
      for (C=1; C<=B; C++)
      {
        NC2=N2+C;
        AA[B][C]._M_re+=-G*RAB[C]-K*AA[A][C]._M_re+H*RAB[NC2]+L*AA[A][C]._M_im;
        AA[B][C]._M_im+=-G*RAB[NC2]-K*AA[A][C]._M_im-H*RAB[C]-L*AA[A][C]._M_re;
      };
    };
_13:
    for (C=1; C<=D; C++)
    {
      AA[A][C]._M_re*=Q;
      AA[A][C]._M_im*=Q;
    };
    RAB[ND2]=-P;
_15:
    O=EV[A];
    EV[A]=AA[A][A]._M_re;
    AA[A][A]._M_re=O;
    AA[A][A]._M_im=Q*sqrt(M);
  };

  for (I=1; I<=N; I++)
  {
    for (J=1; J<=N; J++)
    {
      VV[I][J]._M_re=0.0;
    };
    VV[I][I]._M_re=1.0;
  };
  IERR=0;
  if (N==1) goto _34;
  for (R=2; R<=N; R++) RAB[R-1]=RAB[R];
  BB=0.0;
  Z=0.0;
  RAB[N]=0.0;
  for (U=1; U<=N; U++)
  {
    S=0;
    BD=SYS051*(fabs(EV[U])+fabs(RAB[U]));
    if (Z<BD) Z=BD;
    for (V=U; V<=N; V++) if (fabs(RAB[V])<=Z) break;
_21:
    if (V==U) goto _28;
_22:
    if (S==30) goto _33;
    S=S+1;
    X=U+1;
    BC=EV[U];
    BE=(EV[X]-BC)/(2.0*RAB[U]);
    if (fabs(BE)>1.0) BF=fabs(BE)*sqrt(1.0+1/(BE*BE));
    if (fabs(BE)<=1.0) BF=sqrt(BE*BE+1.0);
    EV[U]=RAB[U]/(BE+SIGN(BF,BE));
    BD=BC-EV[U];
    for (R=X; R<=N; R++) EV[R]=EV[R]-BD;
    BB=BB+BD;
    BE=EV[V];
    BA=1.0;
    BG=0.0;
    Y=V-U;
    for (W=1; W<=Y; W++)
    {
      R=V-W;
      BC=BA*RAB[R];
      BD=BA*BE;
      if (fabs(BE)<fabs(RAB[R])) goto _24;
      BA=RAB[R]/BE;
      BF=sqrt(BA*BA+1.0);
      RAB[R+1]=BG*BE*BF;
      BG=BA/BF;
      BA=1.0/BF;
      goto _25;
_24:
      BA=BE/RAB[R];
      BF=sqrt(BA*BA+1.0);
      RAB[R+1]=BG*RAB[R]*BF;
      BG=1.0/BF;
      BA=BA*BG;
_25:
      BE=BA*EV[R]-BG*BC;
      EV[R+1]=BD+BG*(BA*BC+BG*EV[R]);
      for (T=1; T<=N; T++)
      {
        BD=VV[T][R+1]._M_re;
        VV[T][R+1]._M_re=BG*VV[T][R]._M_re+BA*BD;
        VV[T][R]._M_re=BA*VV[T][R]._M_re-BG*BD;
      };
    };
    RAB[U]=BG*BE;
    EV[U]=BA*BE;
    if (fabs(RAB[U])>Z) goto _22;
_28:
    EV[U]=EV[U]+BB;
  };
  for (W=2; W<=N; W++)
  {
    R=W-1;
    T=R;
    BE=EV[R];
    for (S=W; S<=N; S++)
    {
      if (EV[S]>=BE) continue;
      T=S;
      BE=EV[S];
    };
    if (T==R) continue;
    EV[T]=EV[R];
    EV[R]=BE;
    for (S=1; S<=N; S++)
    {
      BE=VV[S][R]._M_re;
      VV[S][R]._M_re=VV[S][T]._M_re;
      VV[S][T]._M_re=BE;
    };
  };
  goto _34;
_33:
  IERR=U;
_34:
  if (IERR!=0) goto _40;
  if (N==0) goto _40;
  for (BK=1; BK<=N; BK++)
  {
    NBK=N+BK;
    NBK2=N2+BK;
    for (BJ=1; BJ<=N; BJ++)
    {
      VV[BK][BJ]._M_im=-VV[BK][BJ]._M_re*RAB[NBK2];
      VV[BK][BJ]._M_re=VV[BK][BJ]._M_re*RAB[NBK];
    };
  };

  if (N==1) goto _40;
  for (BI=2; BI<=N; BI++)
  {
    BL=BI-1;
    BM=AA[BI][BI]._M_im;
    if (BM==0.0) continue;
    for (BJ=1; BJ<=N; BJ++)
    {
      BN=0.0;
      BO=0.0;
      for (BK=1; BK<=BL; BK++)
      {
        BN=BN+AA[BI][BK]._M_re*VV[BK][BJ]._M_re-AA[BI][BK]._M_im*VV[BK][BJ]._M_im;
        BO=BO+AA[BI][BK]._M_re*VV[BK][BJ]._M_im+AA[BI][BK]._M_im*VV[BK][BJ]._M_re;
      };
      BN=(BN/BM)/BM;
      BO=(BO/BM)/BM;
      for (BK=1; BK<=BL; BK++)
      {
        VV[BK][BJ]._M_re=VV[BK][BJ]._M_re-BN*AA[BI][BK]._M_re-BO*AA[BI][BK]._M_im;
        VV[BK][BJ]._M_im=VV[BK][BJ]._M_im-BO*AA[BI][BK]._M_re+BN*AA[BI][BK]._M_im;
      };
    };
  };
_40:
  if (IERR!=0) MessageBeep(64);
  delete[] RAB;
  return;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall cevd(int N, float **AARe, float **AAIm, float *EV, float **VVRe, float **VVIm, int &IERR)
{
   int I,J,A,B,C,D,E,F,R,S,T,U,V,W,X,Y,BI,BJ,BK,BL,N2,NA,ND,NBK,NA2,NB2,NBK2,ND2,NC2;
   float G,H,K,L,M,O,P,Q,Z,BA,BB,BC,BD,BE,BF,BG,SYS051,BM,BN,BO;
   float *RAB = new float[3*N + 5];
   SYS051 = 1.0e-7;
   N2 = 2*N;
   RAB[N2] = 1.0;
   RAB[3*N] = 0.0;
   for (A=1; A<=N; A++)
   {
      EV[A] = AARe[A][A];
   };
   for (E=1; E<=N; E++)
   {
      A = N + 1 - E;
      D = A - 1;
      M = 0.0;
      Q = 0.0;
      if (D<1) goto _3;
      for (C=1; C<=D; C++)
      {
         Q = Q + fabs(AARe[A][C]) + fabs(AAIm[A][C]);
      };
      ND = N + D;
      NA = N + A;
      NA2 = N2 + A;
      ND2 = N2 + D;
      if (Q!=0.0) goto _4;
      RAB[ND] = 1.0;
      RAB[ND2] = 0.0;
_3:   RAB[A] = 0.0;
      goto _15;                                      
_4:   for (C=1; C<=D; C++)
      {
         AARe[A][C] = AARe[A][C]/Q;
         AAIm[A][C] = AAIm[A][C]/Q;
         M = M + AARe[A][C]*AARe[A][C] + AAIm[A][C]*AAIm[A][C];
      };
      K = sqrt(M);
      RAB[A] = Q*K;
      G = sqrt(AARe[A][D]*AARe[A][D] + AAIm[A][D]*AAIm[A][D]);
      if (G == 0.0) goto _6;
      RAB[ND] = (AAIm[A][D]*RAB[NA2] - AARe[A][D]*RAB[NA])/G;
      P = (AARe[A][D]*RAB[NA2] + AAIm[A][D]*RAB[NA])/G;
      M = M + G*K;
      K = 1.0 + K/G;
      AARe[A][D]*=K;
      AAIm[A][D]*=K;
      if (D == 1) goto _13;
      goto _7;
_6:   RAB[ND] = -RAB[NA];
      P = RAB[NA2];
      AARe[A][D] = K;
_7:   G = 0.0;
      for (B=1; B<=D; B++)
      {
         NB2 = N2 + B;
         K = 0.0;
         L = 0.0;
         for (C=1; C<=B; C++)
         {
            K = K + AARe[B][C]*AARe[A][C] + AAIm[B][C]*AAIm[A][C];
            L = L - AARe[B][C]*AAIm[A][C] + AAIm[B][C]*AARe[A][C];
         };
         F = B + 1;
         if (D<F) goto _10;
         for (C=F; C<=D; C++)
         {
            K = K + AARe[C][B]*AARe[A][C] - AAIm[C][B]*AAIm[A][C];
            L = L - AARe[C][B]*AAIm[A][C] - AAIm[C][B]*AARe[A][C];
_9:      };
_10:     RAB[B] = K/M;
         RAB[NB2] = L/M;
         G = G + RAB[B]*AARe[A][B] - RAB[NB2]*AAIm[A][B];
      };
      O = G/(M+M);
      for (B=1; B<=D; B++)
      {
         NB2 = N2 + B;
         G = AARe[A][B];
         K = RAB[B] - O*G;
         RAB[B] = K;
         H = -AAIm[A][B];
         L = RAB[NB2] - O*H;
         RAB[NB2] = -L;
         for (C=1; C<=B; C++)
         {
            NC2 = N2 + C;
            AARe[B][C] += -G*RAB[C] - K*AARe[A][C] + H*RAB[NC2] + L*AAIm[A][C];
            AAIm[B][C] += -G*RAB[NC2] - K*AAIm[A][C] - H*RAB[C] - L*AARe[A][C];
         };
      };
_13:
      for (C=1; C<=D; C++)
      {
         AARe[A][C]*=Q;
         AAIm[A][C]*=Q;
      };
      RAB[ND2] = -P;
_15:
      O = EV[A];
      EV[A] = AARe[A][A];
      AARe[A][A] = O;
      AAIm[A][A] = Q*sqrt(M);
   };

   for (I=1; I<=N; I++)
   {
      for (J=1; J<=N; J++)
      {
         VVRe[I][J] = 0.0;
      };
      VVRe[I][I] = 1.0;
   };
   IERR = 0;
   if (N == 1) goto _34;
   for (R=2; R<=N; R++) RAB[R-1] = RAB[R];
   BB = 0.0;
   Z = 0.0;
   RAB[N] = 0.0;
   for (U=1; U<=N; U++)
   {
      S = 0;
      BD = SYS051*(fabs(EV[U]) + fabs(RAB[U]));
      if (Z<BD) Z = BD;
      for (V=U; V<=N; V++) if (fabs(RAB[V])<=Z) break;
_21:
      if (V == U) goto _28;
_22:
      if (S == 30) goto _33;
      S = S + 1;
      X = U + 1;
      BC = EV[U];
      BE = (EV[X]-BC)/(2.0*RAB[U]);
      if (fabs(BE)>1.0) BF = fabs(BE)*sqrt(1.0 + 1/(BE*BE));
      if (fabs(BE)<=1.0) BF = sqrt(BE*BE + 1.0);
      EV[U] = RAB[U]/(BE + SIGN(BF,BE));
      BD = BC - EV[U];
      for (R=X; R<=N; R++) EV[R] = EV[R] - BD;
      BB = BB + BD;
      BE = EV[V];
      BA = 1.0;
      BG = 0.0;
      Y = V - U;
      for (W=1; W<=Y; W++)
      {
         R = V - W;
         BC = BA*RAB[R];
         BD = BA*BE;
         if (fabs(BE)<fabs(RAB[R])) goto _24;
         BA = RAB[R]/BE;
         BF = sqrt(BA*BA + 1.0);
         RAB[R+1] = BG*BE*BF;
         BG = BA/BF;
         BA = 1.0/BF;
         goto _25;
_24:
         BA = BE/RAB[R];
         BF = sqrt(BA*BA + 1.0);
         RAB[R+1] = BG*RAB[R]*BF;
         BG = 1.0/BF;
         BA = BA*BG;
_25:
         BE = BA*EV[R] - BG*BC;
         EV[R+1] = BD+BG*(BA*BC + BG*EV[R]);
         for (T=1; T<=N; T++)
         {
            BD = VVRe[T][R+1];
            VVRe[T][R+1] = BG*VVRe[T][R] + BA*BD;
            VVRe[T][R] = BA*VVRe[T][R] - BG*BD;
         };
      };
      RAB[U] = BG*BE;
      EV[U] = BA*BE;
      if (fabs(RAB[U])>Z) goto _22;
_28:
      EV[U] = EV[U] + BB;
   };
   for (W=2; W<=N; W++)
   {
      R = W-1;
      T = R;
      BE = EV[R];
      for (S=W; S<=N; S++)
      {
         if (EV[S]>=BE) continue;
         T = S;
         BE = EV[S];
      };
      if (T == R) continue;
      EV[T] = EV[R];
      EV[R] = BE;
      for (S=1; S<=N; S++)
      {
         BE = VVRe[S][R];
         VVRe[S][R] = VVRe[S][T];
         VVRe[S][T] = BE;
      };
   };
   goto _34;
_33:
   IERR = U;
_34:
   if (IERR!=0) goto _40;
   if (N == 0) goto _40;
   for (BK=1; BK<=N; BK++)
   {
      NBK = N + BK;
      NBK2 = N2 + BK;
      for (BJ=1; BJ<=N; BJ++)
      {
         VVIm[BK][BJ] = -VVRe[BK][BJ]*RAB[NBK2];
         VVRe[BK][BJ] = VVRe[BK][BJ]*RAB[NBK];
      };
   };

   if (N == 1) goto _40;
   for (BI=2; BI<=N; BI++)
   {
      BL = BI - 1;
      BM = AAIm[BI][BI];
      if (BM == 0.0) continue;
      for (BJ=1; BJ<=N; BJ++)
      {
         BN = 0.0;
         BO = 0.0;
         for (BK=1; BK<=BL; BK++)
         {
            BN = BN + AARe[BI][BK]*VVRe[BK][BJ] - AAIm[BI][BK]*VVIm[BK][BJ];
            BO = BO + AARe[BI][BK]*VVIm[BK][BJ] + AAIm[BI][BK]*VVRe[BK][BJ];
         };
         BN = (BN/BM)/BM;
         BO = (BO/BM)/BM;
         for (BK=1; BK<=BL; BK++)
         {
            VVRe[BK][BJ] = VVRe[BK][BJ] - BN*AARe[BI][BK] - BO*AAIm[BI][BK];
            VVIm[BK][BJ] = VVIm[BK][BJ] - BO*AARe[BI][BK] + BN*AAIm[BI][BK];
         };
      };
   };
_40:
   if (IERR!=0) MessageBeep(64);
   delete[] RAB;
   return;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall cevd(int N, double **AARe, double **AAIm, double *EV, double **VVRe, double **VVIm, int &IERR)
{
   int I,J,A,B,C,D,E,F,R,S,T,U,V,W,X,Y,BI,BJ,BK,BL,N2,NA,ND,NBK,NA2,NB2,NBK2,ND2,NC2;
   double G,H,K,L,M,O,P,Q,Z,BA,BB,BC,BD,BE,BF,BG,SYS051,BM,BN,BO;
   double *RAB = new double[3*N + 5];
   SYS051 = 1.0e-7;
   N2 = 2*N;
   RAB[N2] = 1.0;
   RAB[3*N] = 0.0;
   for (A=1; A<=N; A++)
   {
      EV[A] = AARe[A][A];
   };
   for (E=1; E<=N; E++)
   {
      A = N + 1 - E;
      D = A - 1;
      M = 0.0;
      Q = 0.0;
      if (D<1) goto _3;
      for (C=1; C<=D; C++)
      {
         Q = Q + fabs(AARe[A][C]) + fabs(AAIm[A][C]);
      };
      ND = N + D;
      NA = N + A;
      NA2 = N2 + A;
      ND2 = N2 + D;
      if (Q!=0.0) goto _4;
      RAB[ND] = 1.0;
      RAB[ND2] = 0.0;
_3:   RAB[A] = 0.0;
      goto _15;
_4:   for (C=1; C<=D; C++)
      {
         AARe[A][C] = AARe[A][C]/Q;
         AAIm[A][C] = AAIm[A][C]/Q;
         M = M + AARe[A][C]*AARe[A][C] + AAIm[A][C]*AAIm[A][C];
      };
      K = sqrt(M);
      RAB[A] = Q*K;
      G = sqrt(AARe[A][D]*AARe[A][D] + AAIm[A][D]*AAIm[A][D]);
      if (G == 0.0) goto _6;
      RAB[ND] = (AAIm[A][D]*RAB[NA2] - AARe[A][D]*RAB[NA])/G;
      P = (AARe[A][D]*RAB[NA2] + AAIm[A][D]*RAB[NA])/G;
      M = M + G*K;
      K = 1.0 + K/G;
      AARe[A][D]*=K;
      AAIm[A][D]*=K;
      if (D == 1) goto _13;
      goto _7;
_6:   RAB[ND] = -RAB[NA];
      P = RAB[NA2];
      AARe[A][D] = K;
_7:   G = 0.0;
      for (B=1; B<=D; B++)
      {
         NB2 = N2 + B;
         K = 0.0;
         L = 0.0;
         for (C=1; C<=B; C++)
         {
            K = K + AARe[B][C]*AARe[A][C] + AAIm[B][C]*AAIm[A][C];
            L = L - AARe[B][C]*AAIm[A][C] + AAIm[B][C]*AARe[A][C];
         };
         F = B + 1;
         if (D<F) goto _10;
         for (C=F; C<=D; C++)
         {
            K = K + AARe[C][B]*AARe[A][C] - AAIm[C][B]*AAIm[A][C];
            L = L - AARe[C][B]*AAIm[A][C] - AAIm[C][B]*AARe[A][C];
_9:      };
_10:     RAB[B] = K/M;
         RAB[NB2] = L/M;
         G = G + RAB[B]*AARe[A][B] - RAB[NB2]*AAIm[A][B];
      };
      O = G/(M+M);
      for (B=1; B<=D; B++)
      {
         NB2 = N2 + B;
         G = AARe[A][B];
         K = RAB[B] - O*G;
         RAB[B] = K;
         H = -AAIm[A][B];
         L = RAB[NB2] - O*H;
         RAB[NB2] = -L;
         for (C=1; C<=B; C++)
         {
            NC2 = N2 + C;
            AARe[B][C] += -G*RAB[C] - K*AARe[A][C] + H*RAB[NC2] + L*AAIm[A][C];
            AAIm[B][C] += -G*RAB[NC2] - K*AAIm[A][C] - H*RAB[C] - L*AARe[A][C];
         };
      };
_13:
      for (C=1; C<=D; C++)
      {
         AARe[A][C]*=Q;
         AAIm[A][C]*=Q;
      };
      RAB[ND2] = -P;
_15:
      O = EV[A];
      EV[A] = AARe[A][A];
      AARe[A][A] = O;
      AAIm[A][A] = Q*sqrt(M);
   };

   for (I=1; I<=N; I++)
   {
      for (J=1; J<=N; J++)
      {
         VVRe[I][J] = 0.0;
      };
      VVRe[I][I] = 1.0;
   };
   IERR = 0;
   if (N == 1) goto _34;
   for (R=2; R<=N; R++) RAB[R-1] = RAB[R];
   BB = 0.0;
   Z = 0.0;
   RAB[N] = 0.0;
   for (U=1; U<=N; U++)
   {
      S = 0;
      BD = SYS051*(fabs(EV[U]) + fabs(RAB[U]));
      if (Z<BD) Z = BD;
      for (V=U; V<=N; V++) if (fabs(RAB[V])<=Z) break;
_21:
      if (V == U) goto _28;
_22:
      if (S == 30) goto _33;
      S = S + 1;
      X = U + 1;
      BC = EV[U];
      BE = (EV[X]-BC)/(2.0*RAB[U]);
      if (fabs(BE)>1.0) BF = fabs(BE)*sqrt(1.0 + 1/(BE*BE));
      if (fabs(BE)<=1.0) BF = sqrt(BE*BE + 1.0);
      EV[U] = RAB[U]/(BE + SIGN(BF,BE));
      BD = BC - EV[U];
      for (R=X; R<=N; R++) EV[R] = EV[R] - BD;
      BB = BB + BD;
      BE = EV[V];
      BA = 1.0;
      BG = 0.0;
      Y = V - U;
      for (W=1; W<=Y; W++)
      {
         R = V - W;
         BC = BA*RAB[R];
         BD = BA*BE;
         if (fabs(BE)<fabs(RAB[R])) goto _24;
         BA = RAB[R]/BE;
         BF = sqrt(BA*BA + 1.0);
         RAB[R+1] = BG*BE*BF;
         BG = BA/BF;
         BA = 1.0/BF;
         goto _25;
_24:
         BA = BE/RAB[R];
         BF = sqrt(BA*BA + 1.0);
         RAB[R+1] = BG*RAB[R]*BF;
         BG = 1.0/BF;
         BA = BA*BG;
_25:
         BE = BA*EV[R] - BG*BC;
         EV[R+1] = BD+BG*(BA*BC + BG*EV[R]);
         for (T=1; T<=N; T++)
         {
            BD = VVRe[T][R+1];
            VVRe[T][R+1] = BG*VVRe[T][R] + BA*BD;
            VVRe[T][R] = BA*VVRe[T][R] - BG*BD;
         };
      };
      RAB[U] = BG*BE;
      EV[U] = BA*BE;
      if (fabs(RAB[U])>Z) goto _22;
_28:
      EV[U] = EV[U] + BB;
   };
   for (W=2; W<=N; W++)
   {
      R = W-1;
      T = R;
      BE = EV[R];
      for (S=W; S<=N; S++)
      {
         if (EV[S]>=BE) continue;
         T = S;
         BE = EV[S];
      };
      if (T == R) continue;
      EV[T] = EV[R];
      EV[R] = BE;
      for (S=1; S<=N; S++)
      {
         BE = VVRe[S][R];
         VVRe[S][R] = VVRe[S][T];
         VVRe[S][T] = BE;
      };
   };
   goto _34;
_33:
   IERR = U;
_34:
   if (IERR!=0) goto _40;
   if (N == 0) goto _40;
   for (BK=1; BK<=N; BK++)
   {
      NBK = N + BK;
      NBK2 = N2 + BK;
      for (BJ=1; BJ<=N; BJ++)
      {
         VVIm[BK][BJ] = -VVRe[BK][BJ]*RAB[NBK2];
         VVRe[BK][BJ] = VVRe[BK][BJ]*RAB[NBK];
      };
   };

   if (N == 1) goto _40;
   for (BI=2; BI<=N; BI++)
   {
      BL = BI - 1;
      BM = AAIm[BI][BI];
      if (BM == 0.0) continue;
      for (BJ=1; BJ<=N; BJ++)
      {
         BN = 0.0;
         BO = 0.0;
         for (BK=1; BK<=BL; BK++)
         {
            BN = BN + AARe[BI][BK]*VVRe[BK][BJ] - AAIm[BI][BK]*VVIm[BK][BJ];
            BO = BO + AARe[BI][BK]*VVIm[BK][BJ] + AAIm[BI][BK]*VVRe[BK][BJ];
         };
         BN = (BN/BM)/BM;
         BO = (BO/BM)/BM;
         for (BK=1; BK<=BL; BK++)
         {
            VVRe[BK][BJ] = VVRe[BK][BJ] - BN*AARe[BI][BK] - BO*AAIm[BI][BK];
            VVIm[BK][BJ] = VVIm[BK][BJ] - BO*AARe[BI][BK] + BN*AAIm[BI][BK];
         };
      };
   };
_40:
   if (IERR!=0) MessageBeep(64);
   delete[] RAB;
   return;
};
//---------------------------------------------------------------------------
void __fastcall Correlation(int N,int Lag,int Mode, double *X,double *Y,double &R0,double *R)
{
	double SUM;
  int K,NK,J;
	for (K=0; K<Lag; K++)
  {
    NK=N-K;
    SUM=0;
    for (J=0; J<NK; J++) 	SUM+=X[J+K]*Y[J];
    if (Mode==0) R[K]=SUM/(16);
    else R[K]=SUM/N;
  };
  SUM=0;
  for (J=0; J<N; J++) SUM+=X[J+K]*Y[J];
  R0=SUM/N;
  return;
};
//---------------------------------------------------------------------------
bool __fastcall Levinson(int M,double T0,double *T,double &P,double *A)
{
  int K,J,KHALF,KJ;
  double SAVE,TEMP;
	P=T0;
  if (M==0) return true;
  for (K=1; K<=M; K++)
  {
    SAVE=T[K];
    if (K!=1)	for (J=1; J<=(K-1); J++) SAVE+=A[J]*T[K-J];
    TEMP=-SAVE/P;
    P*=(1-TEMP*TEMP);
    if (P<0) return false;
    A[K]=TEMP;
    if (K!=1)
    {
      KHALF=K/2;
      for (J=1; J<=KHALF; J++)
      {
        KJ=K-J;
        SAVE=A[J];
        A[J]=SAVE+TEMP*A[KJ];
        if (J==KJ) continue;
        A[KJ]=A[KJ]+TEMP*SAVE;
      };
    };
  };
  return true;
};
//---------------------------------------------------------------------------
bool __fastcall YuleWalker(double Norm,double Td,int N,int NPSD,int IP,int Shift,double * X1,double * &FR,double * &PSD)
{
  double Rp0;
	double *X=X1-1;
	double *A=vector(A,1,IP+1);
	double *R=vector(R,1,IP+1);
  double R0;
  bool result;
  int i;
  Correlation(N,IP,Shift,X,X,R0,R);
  result=Levinson(IP,R0,R,Rp0,A);
  free(R,1,IP+1);
  if (!result) return false;
PSD_DRAW:
	double * XA=vector(XA,0,NPSD);
  int Nhalf=NPSD/2;
  PSD=vector(PSD,0,Nhalf);
  FR=vector(FR,0,Nhalf);
	if (Rp0<0) return false;
  int L=(int)floor(log(NPSD)/log(2));
  XA[0]=1;
  for (i=1; i<=IP; i++) XA[i]=A[i];
  for (i=IP+1; i<NPSD; i++) XA[i]=0;
  comp *fft = new comp[NPSD];
  FFT(fft, XA, L, NULL);
  double tmp1=Norm/((double)(NPSD)*Td);
  double tmp2=0;
  double tmp3=Td*Rp0;
  for (i=0; i<Nhalf; i++) {PSD[i]=tmp3/::norm(fft[i]);};
  for (i=0; i<Nhalf; i++)
  {
    FR[i]=tmp2;
    tmp2+=tmp1;
  };
  free(XA,0,NPSD);
  delete[] fft;
  free(A,1,IP+1);
  return true;
};
//---------------------------------------------------------------------------
bool __fastcall ModCovar(double Norm,double Td,int N,int NPSD,int IP,double * X1,double * &FR,double * &PSD)
{
	double Rp0;
	double *X=X1-1;
	double *A=vector(A,1,IP+1);
	double *C=vector(C,1,IP+1);
	double *D=vector(D,1,IP+1);
	double *R=vector(R,1,IP+1);
	double Lambda,Theta,Psi,XI,EF,EB,C1,C2,C3,C4,Save1,Save2,Save3,Save4;
	double R1,R2,R3,R4,R5,Delta,Gamma;
	int M,MK,k,i,K;
	R1=0;
	for (k=2; k<=(N-1); k++) R1+=2*X[k]*X[k];
	R2=X[1]*X[1];
	R3=X[N]*X[N];
	R4=1/(R1+2*(R2+R3));
	Rp0=R1+R2+R3;
	Delta=1-R2*R4;
	Gamma=1-R3*R4;
	Lambda=X[1]*X[N]*R4;
	C[1]=X[N]*R4;
	D[1]=X[1]*R4;
	if (IP==0)
	{
		Rp0=(0.5*R1+R2+R3)/N;
		free(C,1,IP+1);
		free(D,1,IP+1);
		free(R,1,IP+1);
		goto PSD_DRAW;
	}
	for (M=1; M<=IP; M++)
	{
		Save1=0;
		for (k=M+1; k<=N; k++) Save1+=X[k]*X[k-M];
		Save1*=2;
		R[M]=Save1;
		Theta=X[N]*D[1];
		Psi=X[N]*C[1];
		XI=X[1]*D[1];
		for (k=1; k<=(M-1); k++)
		{
      Theta+=X[N-k]*D[k+1];
      Psi+=X[N-k]*C[k+1];
      XI+=X[k+1]*D[k+1];
      R[k]=R[k]-(X[N+1-M]*X[N+1-M+k]+X[M]*X[M-k]);
      Save1+=R[k]*A[M-k];
		};
		C1=-Save1/Rp0;
		A[M]=C1;
		Rp0*=(1-C1*C1);
		for (K=1; K<=M/2; K++)
		{
			MK=M-K;
			Save1=A[K];
			A[K]=Save1+C1*A[MK];
			if (K==MK) break;
			A[MK]+=C1*Save1;
		};
		if (M==IP)
		{
			Rp0=0.5*Rp0/(N-M);
			break;
		};
		R1=1/(Delta*Gamma-Lambda*Lambda);
		C1=(Theta*Lambda+Psi*Delta)*R1;
		C2=(Psi*Lambda+Theta*Gamma)*R1;
		C3=(XI*Lambda+Theta*Delta)*R1;
		C4=(Theta*Lambda+XI*Gamma)*R1;
		for (K=1; K<=(M+1)/2; K++)
		{
			MK=M+1-K;
			Save1=C[K];
      Save2=D[K];
      Save3=C[MK];
      Save4=D[MK];
      C[K]+=C1*Save3+C2*Save4;
      D[K]+=C3*Save3+C4*Save4;
      if (K==MK) break;
      C[MK]+=C1*Save1+C2*Save2;
      D[MK]+=C3*Save1+C4*Save2;
    };
    R2=Psi*Psi;
    R3=Theta*Theta;
    R4=XI*XI;
    R5=Gamma-(R2*Delta+R3*Gamma+2*Psi*Lambda*Theta)*R1;
    R2=Delta-(R3*Delta+R4*Gamma+2*Theta*Lambda*XI)*R1;
    Gamma=R5;
    Delta=R2;
    Lambda+=C3*Psi+C4*Theta;
    if (Rp0<=0)
    {
      free(C,1,IP+1);
      free(D,1,IP+1);
      free(R,1,IP+1);
      return false;
    };
    if ((Delta<=0)||(Delta>1)||(Gamma<=0)||(Gamma>1))
    {
      free(C,1,IP+1);
      free(D,1,IP+1);
      free(R,1,IP+1);
      return false;
    };

    R1=1/Rp0;
    R2=1/(Delta*Gamma-Lambda*Lambda);
    EF=X[M+1];
    EB=X[N-M];
    for (K=1; K<=M; K++)
    {
      EF+=A[K]*X[M+1-K];
      EB+=A[K]*X[N-M+K];
    };
    C1=EB*R1;
    C2=EF*R1;
    C3=(EB*Delta+EF*Lambda)*R2;
    C4=(EF*Gamma+EB*Lambda)*R2;
    for (K=M; K>=1; K--)
    {
      Save1=A[K];
      A[K]=Save1+C3*C[K]+C4*D[K];
      C[K+1]=C[K]+C1*Save1;
      D[K+1]=D[K]+C2*Save1;
    };
    C[1]=C1;
    D[1]=C2;
    R3=EB*EB;
    R4=EF*EF;
    Rp0=Rp0-(R3*Delta+R4*Gamma+2*EF*EB*Lambda)*R2;
    Delta-=R4*R1;
    Gamma-=R3*R1;
    Lambda+=EF*EB*R1;
    if(Rp0<=0)
    {
      free(C,1,IP+1);
      free(D,1,IP+1);
      free(R,1,IP+1);
      return false;
    };
    if ((Delta<=0)||(Delta>1)||(Gamma<=0)||(Gamma>1))
    {
      free(C,1,IP+1);
      free(D,1,IP+1);
      free(R,1,IP+1);
      return false;
    };
  };
  free(C,1,IP+1);
  free(D,1,IP+1);
  free(R,1,IP+1);
PSD_DRAW:
  double * XA=vector(XA,0,NPSD);
  int Nhalf=NPSD/2;
  PSD=vector(PSD,0,Nhalf);
  FR=vector(FR,0,Nhalf);
	if (Rp0<0) return false;
  int L=(int)round(log(NPSD)/log(2));
  XA[0]=1;
  for (i=1; i<=IP; i++) XA[i]=A[i];
  for (i=IP+1; i<NPSD; i++) XA[i]=0;
  comp *fft = new comp[NPSD];
  FFT(fft, XA, L, NULL);
  double tmp1=Norm/((double)(NPSD)*Td);
  double tmp2=0;
  double tmp3=Td*Rp0;
  for (i=0; i<Nhalf; i++) {PSD[i]=tmp3/::norm(fft[i]);};
  for (i=0; i<Nhalf; i++)
  {
    FR[i]=tmp2;
    tmp2+=tmp1;
  };
	free(XA,0,NPSD);
   delete[] fft;
   free(A,1,IP+1);
   return true;
};
//---------------------------------------------------------------------------
bool __fastcall BURG(double Norm,double Td,int N,int NPSD,int IP,double * X1,double * &FR,double * &PSD)
{
	double Rp0;
	double *X=X1-1;
	double *A=vector(A,1,IP+1);
	double * EF=vector(EF,1,N);
	double * EB=vector(EB,1,N);
	double NUM, SAVE1, SAVE2, DEN , TEMP;
	int J,K,KHALF,KJ,i;
	Rp0=0;
	for (J=1; J<=N; J++) Rp0+=X[J]*X[J];
	DEN=2*Rp0;
	Rp0/=(double)N;
	if (IP==0)
	{
		free(EF,1,N);
    free(EB,1,N);
    goto PSD_DRAW;
  };
  for (J=1; J<=N; J++)
  {
    EF[J]=X[J];
    EB[J]=X[J];
  };
  TEMP=1;

    // Главная рекурсия
	for (K=1; K<=IP; K++)
  {
    NUM=0;
    for (J=K+1; J<=N; J++) NUM+=EF[J]*EB[J-1];
    DEN=TEMP*DEN-EF[K]*EF[K]-EB[N]*EB[N];
    SAVE1=-2*NUM/DEN;
    TEMP=1-SAVE1*SAVE1;
    Rp0*=TEMP;
    if (TEMP<=0)
    {
      free(EF,1,N);
      free(EB,1,N);
      return false;
    };
    A[K]=SAVE1;
    if (K>1)
    {
      KHALF=K/2;
      for (J=1; J<=KHALF; J++)
      {
        KJ=K-J;
        SAVE2=A[J];
        A[J]=SAVE2+SAVE1*A[KJ];
        if (J==KJ) continue;
        A[KJ]+=SAVE1*SAVE2;
      };
		};
    if (K==IP) break;
    for (J=N; J>=K+1; J--)
    {
      SAVE2=EF[J];
      EF[J]=SAVE2+SAVE1*EB[J-1];
      EB[J]=EB[J-1]+SAVE1*SAVE2;
    };
  };
  free(EF,1,N);
  free(EB,1,N);
PSD_DRAW:
	double * XA=vector(XA,0,NPSD);
  int Nhalf=NPSD/2;
  PSD=vector(PSD,0,Nhalf);
  FR=vector(FR,0,Nhalf);
	if (Rp0<0) return false;
  int L=(int)round(log(NPSD)/log(2));
  XA[0]=1;
  for (i=1; i<=IP; i++) XA[i]=A[i];
  for (i=IP+1; i<NPSD; i++) XA[i]=0;
  comp *fft = new comp[NPSD];
  FFT(fft, XA, L, NULL);
  double tmp1=Norm/((double)(NPSD)*Td);
  double tmp2=0;
  double tmp3=Td*Rp0;
  for (i=0; i<Nhalf; i++) {PSD[i]=tmp3/::norm(fft[i]);};
  for (i=0; i<Nhalf; i++)
  {
    FR[i]=tmp2;
    tmp2+=tmp1;
  };
	free(XA,0,NPSD);
	delete[] fft;
   free(A,1,IP+1);
  return true;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool __fastcall Geometric(double Norm,double Td,int N,int NPSD,int IP,double * X1,double * &FR,double * &PSD)
{
  int p,n,half,pn,i;
  double Kp,Rp0;
  double * epf,* epb,tmp1,tmp2,tmp3;
  double * ap=vector(ap,1,IP+1);
  epf=(double *)malloc(sizeof(double)*(N+1));
  epb=(double *)malloc(sizeof(double)*(N+1));

  Rp0=0;
  for (n=0; n<N; n++)
  {
    Rp0+=X1[n]*X1[n];
    epf[n+1]=X1[n];
    epb[n+1]=X1[n];
  };
  Rp0/=(double)N;
  for (p=1; p<=IP; p++)
  {
    tmp1=0;tmp2=0;tmp3=0;
    for (n=p+1; n<=N; n++)
    {
      tmp1+=epf[n]*epb[n-1];
      tmp2+=epf[n]*epf[n];
      tmp3+=epb[n-1]*epb[n-1];
    };
    Kp=-tmp1/(sqrt(tmp2)*sqrt(tmp3));

    half=p/2;
    for (n=1; n<=half; n++)
    {
      pn=p-n;
      tmp1=ap[n];
      ap[n]=tmp1+Kp*ap[pn];
      if (n==pn) continue;
      ap[pn]+=Kp*tmp1;
    };
    ap[p]=Kp;
    Rp0=Rp0*(1-Kp*Kp);
 		if (Rp0<0)
    {
      free(epf);
      free(epb);
      return false;
    };
    for (n=N; n>=p+1; n--)
    {
      tmp1=epf[n];
      epf[n]=tmp1+Kp*epb[n-1];
      epb[n]=epb[n-1]+Kp*tmp1;
    };
  };
  free(epf);
  free(epb);

	double * XA=vector(XA,0,NPSD);
  int Nhalf=NPSD/2;
  PSD=vector(PSD,0,Nhalf);
  FR=vector(FR,0,Nhalf);
	if (Rp0<0) return false;
  int L=(int)round(log(NPSD)/log(2));
  XA[0]=1;
  for (i=1; i<=IP; i++) XA[i]=ap[i];
  for (i=IP+1; i<NPSD; i++) XA[i]=0;
  comp *fft = new comp[NPSD];
  FFT(fft, XA, L, NULL);
  tmp1=Norm/((double)(NPSD)*Td);
  tmp2=0;
  tmp3=Td*Rp0;
  for (i=0; i<Nhalf; i++) {PSD[i]=tmp3/::norm(fft[i]);};
  for (i=0; i<Nhalf; i++)
  {
    FR[i]=tmp2;
    tmp2+=tmp1;
  };
	free(XA,0,NPSD);
	delete[] fft;
  free(ap,1,IP+1);
	return true;
};
//---------------------------------------------------------------------------
bool __fastcall MUSIC_EV(double Norm,double Td,int Method,int N,int NPSD,int IP,int &NSIG,double * X1,double * &FR,double * &PSD)
{
  int NP,i,k,NP2;
  int L=(int)round(log(double(NPSD))/log(2));
  double *X=X1-1;
  double **a,*w,**v,*z;
  comp *fft,*coeff;
  double awr;
  NP=N-IP;
  NP2=2*NP;
  v=matrix(v,1,IP,1,IP);
  a=matrix(a,1,NP2,1,IP);
  w=vector(w,1,IP);
  z=vector(z,0,NPSD-1);
  int Nhalf=NPSD/2;
  PSD=vector(PSD,0,Nhalf);
  FR=vector(FR,0,Nhalf);
  if (NSIG>IP) return false;
  for (i=1; i<=NP; i++)
  for (k=1; k<=IP; k++)
  {
    a[i][k]=X[i-k+IP];
    a[i+NP][k]=X[i+k];
  };
  svd(a,NP2,IP,w,v);
  if (NSIG<0)  // AutoDetect NSIG
  {
    awr=0;
    for (i=1; i<=IP; i++) awr+=w[i];
    awr/=IP;
    NSIG=0;
    for (i=1; i<=IP; i++)
    if (w[i]<awr)
    {
      NSIG=i-1;
      break;
    };
  };
  coeff = initFFTcoeff(L);
  fft = new comp[NPSD];
  for (k=0; k<Nhalf; k++) PSD[k]=0;
  for (i=NSIG+1; i<=IP; i++)
  {
    for (k=0; k<IP; k++) z[k]=v[k+1][i];
    for (k=IP; k<NPSD; k++) z[k]=0;
    FFT(fft, z, L, coeff);
    if (Method==0) for (k=0; k<Nhalf; k++) PSD[k]+=norm(fft[k]);
    else for (k=0; k<Nhalf; k++) PSD[k]+=norm(fft[k])/w[i];
  };
  for (k=0; k<Nhalf; k++) PSD[k]=1/PSD[k];
  free(z);
  free(v,1,IP,1,IP);
  free(a,1,NP2,1,IP);
  free(w,1,IP);
  delete[] coeff;
  delete[] fft;
  double tmp1=Norm/((double)(NPSD)*Td);
  double tmp2=0;
  for (i=0; i<Nhalf; i++)
  {
    FR[i]=tmp2;
    tmp2+=tmp1;
  };
  return true;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool __fastcall cModCovar(int N,int IP,comp *X ,double &Rp0,comp *A)
{
  comp *C, *D, *R;
  C=(comp *)malloc(sizeof(comp)*(IP+2));
  D=(comp *)malloc(sizeof(comp)*(IP+2));
  R=(comp *)malloc(sizeof(comp)*(IP+2));
  comp Lambda,Theta,Psi,XI,EF,EB,C1,C2,C3,C4,Save1,Save2,Save3,Save4;
  long double R1,R2,R3,R4,R5,Delta,Gamma;
  int M,MK,k;
  // Инициализация
  R1=0;
  for (k=2; k<=(N-1); k++) R1+=2*norm(X[k]);
  R2=norm(X[1]);
  R3=norm(X[N]);
  R4=1/(R1+2*(R2+R3));
  Rp0=R1+R2+R3;
  Delta=1-R2*R4;
  Gamma=1-R3*R4;
  Lambda=conj(X[1]*X[N])*(comp)R4;
  C[1]=X[N]*(comp)R4;
  D[1]=conj(X[1])*(comp)R4;
  if (IP==0)
  {
    Rp0=(0.5*R1+R2+R3)/N;
    free(C);
    free(D);
    free(R);
    return true;
  }
    // Основной цикл
  for (M=1; M<=IP; M++)
  {
    Save1=comp(0,0);
    for (int k=M+1; k<=N; k++) Save1+=X[k]*conj(X[k-M]);
    Save1*=2;
    R[M]=conj(Save1);
    Theta=X[N]*D[1];
    Psi=X[N]*C[1];
    XI=conj(X[1])*D[1];
    for (int k=1; k<=(M-1); k++)
    {
      Theta+=X[N-k]*D[k+1];
      Psi+=X[N-k]*C[k+1];
      XI+=conj(X[k+1])*D[k+1];
      R[k]=R[k]-(X[N+1-M]*conj(X[N+1-M+k])+conj(X[M])*X[M-k]);
      Save1+=conj(R[k])*A[M-k];
    };
        // Обновление порядка вектора А
    C1=-Save1/Rp0;
    A[M]=C1;
    Rp0*=(1-norm(C1));
    for (int K=1; K<=M/2; K++)
    {
      MK=M-K;
      Save1=A[K];
      A[K]=Save1+C1*conj(A[MK]);
      if (K==MK) break;
      A[MK]+=C1*conj(Save1);
    };
    if (M==IP)
    {
      Rp0=0.5*Rp0/(N-M);
      free(C);
      free(D);
      free(R);
      return true;
    };
    R1=1/(Delta*Gamma-norm(Lambda));
    C1=(Theta*conj(Lambda)+Psi*(comp)Delta)*(comp)R1;
    C2=(Psi*Lambda+Theta*(comp)Gamma)*(comp)R1;
    C3=(XI*conj(Lambda)+Theta*(comp)Delta)*(comp)R1;
    C4=(Theta*Lambda+XI*(comp)Gamma)*(comp)R1;
    for (int K=1; K<=(M+1)/2; K++)
    {
      MK=M+1-K;
      Save1=conj(C[K]);
      Save2=conj(D[K]);
      Save3=conj(C[MK]);
      Save4=conj(D[MK]);
      C[K]+=C1*Save3+C2*Save4;
      D[K]+=C3*Save3+C4*Save4;
      if (K==MK) break;
      C[MK]+=C1*Save1+C2*Save2;
      D[MK]+=C3*Save1+C4*Save2;
    };
    R2=norm(Psi);
    R3=norm(Theta);
    R4=norm(XI);
    R5=Gamma-(R2*Delta+R3*Gamma+2*real(Psi*Lambda*conj(Theta)))*R1;
    R2=Delta-(R3*Delta+R4*Gamma+2*real(Theta*Lambda*conj(XI)))*R1;
    Gamma=R5;
    Delta=R2;
    Lambda+=C3*conj(Psi)+C4*conj(Theta);
    if (Rp0<=0)
    {
      free(C);
      free(D);
      free(R);
      return false;
    };
    if ((Delta<=0)||(Delta>1)||(Gamma<=0)||(Gamma>1))
    {
      free(C);
      free(D);
      free(R);
      return false;
    };
    // Обновление во времени вектора А; обновление порядка векторов С,D
    // и скаляров Gamma, Delta, Lambda

    R1=1/Rp0;
    R2=1/(Delta*Gamma-norm(Lambda));
    EF=X[M+1];
    EB=X[N-M];
    for (int K=1; K<=M; K++)
    {
      EF+=A[K]*X[M+1-K];
      EB+=conj(A[K])*X[N-M+K];
    };
    C1=EB*(comp)R1;
    C2=conj(EF)*(comp)R1;
		C3=(conj(EB)*(comp)Delta+EF*Lambda)*(comp)R2;
    C4=(EF*(comp)Gamma+conj(EB*Lambda))*(comp)R2;
    for (int K=M; K>=1; K--)
    {
      Save1=A[K];
      A[K]=Save1+C3*C[K]+C4*D[K];
      C[K+1]=C[K]+C1*Save1;
      D[K+1]=D[K]+C2*Save1;
    };
    C[1]=C1;
    D[1]=C2;
    R3=norm(EB);
    R4=norm(EF);
    Rp0=Rp0-(R3*Delta+R4*Gamma+2*real(EF*EB*Lambda))*R2;
    Delta-=R4*R1;
    Gamma-=R3*R1;
    Lambda+=conj(EF*EB)*(comp)R1;
    if(Rp0<=0)
    {
      free(C);
      free(D);
      free(R);
      return false;
    };
    if ((Delta<=0)||(Delta>1)||(Gamma<=0)||(Gamma>1))
    {
      free(C);
      free(D);
      free(R);
      return false;
    };
  };
  free(C);
  free(D);
  free(R);
  return true;
};
//---------------------------------------------------------------------------
bool __fastcall cMUSIC_EV(double Norm,double Td,int Method,int M,int N,int NPSD,int &NSIG,comp **X,double * &FR,double * &PSD)
{
	int L=(int)round(log(double(NPSD))/log(2));
  int i,k;
  comp **u=matrix(u,1,M,1,N);
  comp **v=matrix(v,1,N,1,N);
  double *w=vector(w,1,N);
  comp *z=vector(z,0,NPSD-1);
  comp *fft,*coeff;
  double awr;
  PSD=vector(PSD,0,NPSD);
  FR=vector(FR,0,NPSD);
  if (NSIG>N) return false;
  csvd(X,M,N,0,N,N,w,u,v);
  if (NSIG<0)  // AutoDetect NSIG
  {
    awr=0;
    for (i=1; i<=N; i++) awr+=w[i];
    awr/=N;
    NSIG=0;
    for (i=1; i<=N; i++)
    if (w[i]<awr)
    {
      NSIG=i-1;
      break;
    };
  };
  coeff = initFFTcoeff(L);
  fft = new comp[NPSD];
  for (k=0; k<NPSD; k++) PSD[k]=0;
  for (i=NSIG+1; i<=N; i++)
  {
    for (k=0; k<N; k++) z[k]=v[k+1][i];
    for (k=N; k<NPSD; k++) z[k]=0;
    FFT(fft, z, L, coeff);
    if (Method==0) for (k=0; k<NPSD; k++) PSD[k]+=norm(fft[k]);
    else for (k=0; k<NPSD; k++) PSD[k]+=norm(fft[k])/w[i];
  };
  for (k=0; k<NPSD; k++) PSD[k]=1/PSD[k];
  delete[] coeff;
  delete[] fft;
  free(u,1,M,1,N);
  free(v,1,N,1,N);
  free(w,1,N);
  free(z,0,NPSD-1);
  double tmp1=Norm/((double)(NPSD)*Td);
  double tmp2=0;
  double tmp3;
  int Nhalf=NPSD/2;
  for (i=0; i<Nhalf; i++)
  {
    tmp3=PSD[i];
    PSD[i]=PSD[Nhalf+i];
    PSD[Nhalf+i]=tmp3;
  };
  for (i=0; i<Nhalf; i++)
  {
    FR[Nhalf+i]=tmp2;
    FR[Nhalf-i]=-tmp2;
    tmp2+=tmp1;
  };
  FR[0]=-tmp2;
  return true;
};
//---------------------------------------------------------------------------
bool __fastcall cLARMUSIC(double d2l,int M,int N,int NPSD,int &NSIG,comp **X,double * &FR,double * &PSD)
{
  int i,k,j;
  comp **u=matrix(u,1,M,1,N);
  comp **v=matrix(v,1,N,1,N);
  double *w=vector(w,1,N);
  double awr;
  PSD=vector(PSD,0,NPSD);
  FR=vector(FR,0,NPSD);
  if (NSIG>N) return false;
  csvd(X,M,N,0,N,N,w,u,v);
  if (NSIG<0)  // AutoDetect NSIG
  {
    awr=0;
    for (i=1; i<=N; i++) awr+=w[i];
    awr/=N;
    NSIG=0;
    for (i=1; i<=N; i++)
    if (w[i]<awr)
    {
      NSIG=i-1;
      break;
    };
  };
  setmem(PSD,0,NPSD*sizeof(double));
  double T,dT;
  comp sum;
  double sum1;
  dT=Pi/NPSD;
  T=-Pi/2;
  for (j=0; j<NPSD; j++)
  {
    sum1=0;
    for (k=NSIG+1; k<=N; k++)
    {
      sum=0;
      for (int i=0; i<N; i++) sum+=(v[i+1][k])*exp(-compj*comp(Pi2*d2l*i*sin(T)));
      sum1+=norm(sum);
    };
    FR[j]=T*180.0/Pi;
    PSD[j]=1/sum1;
    T+=dT;
  };
  free(u,1,M,1,N);
  free(v,1,N,1,N);
  free(w,1,N);
  return true;
};
//------------------------------------------------------------------------------
bool __fastcall Classic(double Norm,double Td,int N,int NPSD,int wKind,double * X1,double * &FR,double * &PSD)
{
	int i;
  int Nhalf=NPSD/2;
  double *X=vector(X,0,NPSD-1);
  comp *dpf;			// массив коэффицентов ДПФ
  int L=(int)round(log(NPSD)/log(2));
  double WndP;			// мощность окна
  double tmp;
  double tmp1=Norm/((double)(NPSD)*Td);
  double tmp2=0;
  PSD=vector(PSD,0,Nhalf);
  FR=vector(FR,0,Nhalf);
  WndP=0;
  for (i=0; i<N; i++)
  {
    tmp=Wnd(i,wKind,N);
    WndP+=tmp*tmp;
  };
  for (i=0; i<N; i++) X[i]=X1[i]*Wnd(i,wKind,N);
  for (i=N; i<NPSD; i++) X[i]=0;
  dpf = new comp[NPSD];
  FFT(dpf, X, L, NULL);
  tmp=Td/WndP;
  for (i=0; i<Nhalf; i++) PSD[i]=tmp*norm(dpf[i]);
  for (i=0; i<Nhalf; i++)
  {
    FR[i]=tmp2;
    tmp2+=tmp1;
  };
  free(X);
  delete[] dpf;
  return true;
};
//---------------------------------------------------------------------------
bool __fastcall Danyell(double Norm,double Td,int N,int NPSD,int wKind,int PD,double * X1,double * &FR,double * &PSD)
{
	int i,j;
  int Nhalf=NPSD/2;
  double *X=vector(X,0,NPSD-1);
  comp *dpf;
  int L=(int)round(log(NPSD)/log(2));
  double WndP,tmp;
  double tmp1=Norm/((double)(NPSD)*Td);
  double tmp2=0;
  PSD=vector(PSD,0,Nhalf);
  FR=vector(FR,0,Nhalf);
  WndP=0;
  for (i=0; i<N; i++)
  {
    tmp=Wnd(i,wKind,N);
    WndP+=tmp*tmp;
  };
  for (i=0; i<N; i++) X[i]=X1[i]*Wnd(i,wKind,N);
  for (i=N; i<NPSD; i++) X[i]=0;
  dpf = new comp[NPSD];
  FFT(dpf, X, L, NULL);
  for (i=0; i<Nhalf; i++) PSD[i]=0;
  tmp=Td/(WndP*(2*PD+1));
  for (i=PD; i<(Nhalf-PD); i++)
  {
    for (j=-PD; j<=PD; j++) PSD[i-PD]+=tmp*norm(dpf[i+j]);
  };
  for (i=0; i<Nhalf; i++)
  {
    if (i>=PD) FR[i-PD]=tmp2;
    tmp2+=tmp1;
  };
  free(X);
  delete[] dpf;
  return true;
};
//---------------------------------------------------------------------------
bool __fastcall Welch(double Norm,double Td,int N,int NPSD,int wKind,int PW,double AW,double * X1,double * &FR,double * &PSD)
{
	int i,j;
  int D;                     	// Число отсчётов в сегменте
  int S;                      // Величина сдвига между сегментами
  int Nhalf=NPSD/2;
  double *X=vector(X,0,NPSD-1);
  comp *dpf = new comp[NPSD];			// массив коэффицентов ДПФ
  int L=(int)round(log(NPSD)/log(2));
  double WndP;			// мощность окна
  double tmp;
  double tmp1=Norm/((double)(NPSD)*Td);
  double tmp2=0;
  PSD=vector(PSD,0,Nhalf);
  FR=vector(FR,0,Nhalf);
  D=floor((double)N/(double)(1+(1-AW)*(PW-1)));
  S=floor(((double)N-D)/(double)(PW-1));
  WndP=0;
  for (i=0; i<D; i++)
  {
    tmp=Wnd(i,wKind,D);
    WndP+=tmp*tmp;
  };
  for (i=0; i<Nhalf; i++) PSD[i]=0;
  tmp=Td/(PW*WndP);

  for (j=0; j<PW; j++)
  {
    for (i=0; i<N; i++) X[i]=X1[i]*Wnd(i-j*S,wKind,D);
    for (i=N; i<NPSD; i++) X[i]=0;
    FFT(dpf, X, L, NULL);
    for (i=0; i<Nhalf; i++) PSD[i]+=tmp*norm(dpf[i]);
  };
  for (i=0; i<Nhalf; i++)
  {
    FR[i]=tmp2;
    tmp2+=tmp1;
  };
  free(X);
  delete[] dpf;
  return true;
};
//---------------------------------------------------------------------------
double __fastcall Wnd(int n,int kind,int N2)     // Окно
{
    double tn=((double)n/(double)(N2-1)-0.5);
    switch (kind)
    {
        case wRect: if ((n<N2)&&(n>=0)) return 1;
                    else return 0.;
        case wTriangle: if ((n<N2)&&(n>=0)) return (1-2*fabsl(tn));
                    else return 0.;
        case wHann: if ((n<N2)&&(n>=0)) return (0.5+0.5*cos(2*M_PI*tn));
                    else return 0.;
        case wHemming: if ((n<N2)&&(n>=0)) return (0.54+0.46*cos(2*M_PI*tn));
                    else return 0.;
        case wNattol: if ((n<N2)&&(n>=0)) return (0.3635819+0.4891775*cos(2*M_PI*tn)+0.1365995*cos(4*M_PI*tn)+0.0106411*cos(6*M_PI*tn));
                    else return 0.;
        case wGauss: if ((n<N2)&&(n>=0)) return expl(-(5*tn)*(5*tn)/2);
                    else return 0.;
        default: return 0.;
    };
};
//---------------------------------------------------------------------------
AnsiString __fastcall WndName(int wKind)
{
	switch (wKind)
    {
        case wRect: return "Прямоугольное";
        case wTriangle: return "Треугольное";
        case wHann: return "Ханна";
        case wHemming: return "Хемминга";
        case wNattol: return "Наттола";
        case wGauss: return "Гауссовское";
    };
    return "";
};
//----------------------------------------------------------------------

#pragma package(smart_init)
