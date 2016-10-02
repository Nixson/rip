//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "myprocs.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall Brenzehem(int x1, int y1, int x2, int y2, TCanvas *Canvas)
{
   int X=1, Y=1; // - начальные инкременты по X и по Y
   int DI = y2-y1;
   if (DI <= 0)
   {
      Y=-1;
      DI=-DI; //  (дистанция по Y должна быть > 0)
   };
   int DIAG_Y = Y; // - приращение по Y для диагональных эл-тов
   int SI = x2-x1;
   if (SI <= 0)
   {
      X=-1;
      SI=-SI; // (дистанция по X должна быть > 0)
   };
   int DIAG_X = X; //- приращение по X для диагональных эл-тов
   //{ вертикальны или горизонтальны прямые сегменты }
   if (SI >= DI) Y=0;// - для прямых сегментов Y не меняется (т.е. они горизонтальны). Переход на шаг 11.
   else
   {
      X=0; //- прямые сегменты вертикальны.
      int tmp = SI;
      SI = DI;
      DI = tmp; // SI <-> DI - обмен SWAP, чтобы больший отрезок был в SI.
   };
  //{ вычисление выравнивающего фактора }
   int SHORT = DI; // - более короткий отрезок
   int STR_X = X;
   int STR_Y = Y; // - инкременты для прямых сегментов (один из них = 0)
   int STR_COUNT = 2*SHORT;
   int DIAG_COUNT= 2*SHORT - 2*SI;
   int BX=2*SHORT-SI; // - начальное значение переключателя
   //{ подготовка к выводу линии }

   X = x1;
   Y = y1; // - начальные координаты
   SI = SI+1; // - прибавить один пиксел для конца отрезка
  //{ вывод линии }
   while (1)
   {
      Sleep(10);
      SI = SI-1;
      if (SI == 0) break;
      Canvas->Pixels[X][Y] = clBlack;
      if (BX < 0)
      {
         X+=STR_X;
         Y+=STR_Y;
         BX+=STR_COUNT;
         continue;
      }
      X+=DIAG_X;
      Y+=DIAG_Y;
      BX+=DIAG_COUNT;
   };
};
//---------------------------------------------------------------------------
AnsiString __fastcall B2S(bool X)
{
   if (X) return "true";
   return "false";
};
//---------------------------------------------------------------------------
AnsiString __fastcall F2S(double X,int num)
{
   return FloatToStrF(X,ffFixed,20,num);
};
//---------------------------------------------------------------------------
AnsiString __fastcall I2S(int X)
{
   return IntToStr(X);
};
//---------------------------------------------------------------------------
int __fastcall S2I(AnsiString X)
{
   return StrToInt(X);
};
//---------------------------------------------------------------------------
double __fastcall S2F(AnsiString X)
{
   return StrToFloat(X);
};
//---------------------------------------------------------------------------
AnsiString __fastcall C2SExp(comp X,int num)
{
  AnsiString tmp;
  tmp = F2S(abs(X), num) + "*exp(j*" + F2S(arg(X), num) + ")";
  return tmp;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
double __fastcall stdvar(double *X,double avr,int N)
{
  double S=0;
  for (int i=0; i<N; i++) S+=sqr(X[i]-avr);
  S=S/double(N);
  return sqrt(S);
};
//---------------------------------------------------------------------------
double __fastcall avr(double *X,int N)
{
  double S=0;
  for (int i=0; i<N; i++) S+=X[i];
  S=S/double(N);
  return S;
};
//---------------------------------------------------------------------------
double __fastcall gauss(double m,double sigma)
{
	return m + sigma*sqrt(1e-20-2*log(1e-20+(double)rand()/(double)(RAND_MAX+1e-5)))*cos(2*M_PI*(double)rand()/(double)RAND_MAX);
}
//---------------------------------------------------------------------------
void __fastcall Normalize360(double &A)
{
   while (A<0.0) A+=360.0;
   while (A>=360.0) A-=360.0;
   return;
};
//---------------------------------------------------------------------------
void __fastcall Normalize180(double &A)
{
   while (A<-180.0) A+=360.0;
   while (A>=180.0) A-=360.0;
   return;
};
//---------------------------------------------------------------------------

__fastcall TDigitalFilter::TDigitalFilter(AnsiString AFileName)
{
  int Handle,Length;
  double a[2000],b[2000];
  int na,nb;
  bool NUMERATOR=true;
  char Buffer;
  AnsiString STR="";
  na=0;
  nb=0;
  Handle=FileOpen(AFileName,fmOpenRead);
  Length=FileSeek(Handle,0,2);
  FileSeek(Handle,0,0);
  for (int i=0; i<Length; i++)
  {
    FileRead(Handle,&Buffer,1);
    if (Buffer=='.') Buffer=',';
    if (Buffer=='\n')
    {
      if ((STR!="Numerator:")&&(STR!="Denominator:"))
      {
        if (NUMERATOR)
        {
          a[na]=StrToFloat(STR);
          na++;
        }
        else
        {
          b[nb]=StrToFloat(STR);
          nb++;
        };
      }
      else if (STR=="Denominator:") NUMERATOR=false;
      STR="";
    }
    else if (Buffer!=' ') STR+=Buffer;
  };
  FileClose(Handle);
  A=new double[na];
  B=new double[nb];
  NA=na-1;
  NB=nb-1;
  for (int i=0; i<=NA; i++) A[i]=a[i];
  for (int i=0; i<=NB; i++) B[i]=b[i];
};
//---------------------------------------------------------------------------
void __fastcall TDigitalFilter::Filtering(int N,double *X,double *Y)
{
  int i,n;
  double S;
  for (n=0; n<N; n++)
  {
    S=0;
    for (i=n-1; (i>=0)&&(i>=(n-NB)); i--) S-=B[n-i]*Y[i];
    for (i=n; (i>=0)&&(i>=(n-NA)); i--) S+=A[n-i]*X[i];
    Y[n]=S/B[0];
  };
  return;
};
//---------------------------------------------------------------------------
TDigitalFilter::~TDigitalFilter()
{
  delete[] A;
  delete[] B;
};
//---------------------------------------------------------------------------
__fastcall TDoplerPelengator::TDoplerPelengator(AnsiString FName1,AnsiString FName2)
{
  int FH,L;
  FH=FileOpen(FName1,fmOpenRead);
  L=FileSeek(FH,0,2);
  FileSeek(FH,0,0);
  N=L/sizeof(float);
  float *data=new float[N];
  Data1=new double[N];
  Data2=new double[N];
  FileRead(FH,data,L);
  FileClose(FH);
  for (int i=0; i<N; i++) Data1[i]=data[i];
  FH=FileOpen(FName2,fmOpenRead);
  FileRead(FH,data,L);
  FileClose(FH);
  for (int i=0; i<N; i++) Data2[i]=data[i];
  delete[] data;
};
//---------------------------------------------------------------------------
TDoplerPelengator::~TDoplerPelengator()
{
  delete[] Data1;
  delete[] Data2;
};
//---------------------------------------------------------------------------
void __fastcall QDemodulation(double *InSignal,int N,double F0,double Fd,AnsiString FilterName,double *ACos,double *ASin)
{
  double w0=Pi2*F0;
  double Td=1/Fd;
  TDigitalFilter *F=new TDigitalFilter(FilterName);
  double *tmp=new double[N];
  for (int i=0; i<N; i++) tmp[i]=InSignal[i]*2.0*cos(w0*i*Td);
  F->Filtering(N,tmp,ACos);
  for (int i=0; i<N; i++) tmp[i]=-InSignal[i]*2.0*sin(w0*i*Td);
  F->Filtering(N,tmp,ASin);
  delete[] tmp;
  delete F;
  return;
};
//---------------------------------------------------------------------------
void __fastcall QCompDemodulation(double *InSignal,int N,double F0,double Fd,AnsiString FilterName,comp *CompSignal)
{
  double w0=Pi2*F0;
  double Td=1/Fd;
  TDigitalFilter *F=new TDigitalFilter(FilterName);
  double *tmp=new double[N];
  double *ACos=new double[N];
  double *ASin=new double[N];
  for (int i=0; i<N; i++) tmp[i]=InSignal[i]*2.0*cos(w0*i*Td);
  F->Filtering(N,tmp,ACos);
  for (int i=0; i<N; i++) tmp[i]=-InSignal[i]*2.0*sin(w0*i*Td);
  F->Filtering(N,tmp,ASin);
  for (int i=0; i<N; i++)
  {
    CompSignal[i]=comp(ACos[i],ASin[i]);
  };
  delete[] tmp;
  delete[] ASin;
  delete[] ACos;
  delete F;
  return;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall SaveCompMatrixToFile(AnsiString fname, AnsiString Comment, int Fidelity, comp **A, int a1, int b1, int a2, int b2)
{
   AnsiString tmp;
   char *str;
   int k;
   if (FileExists(fname)) DeleteFile(fname);
   int H1 = FileCreate(fname);

   tmp = Comment;
   tmp += "\x0A\x0D";
   str = tmp.c_str();
   k = 0;
   while (str[k]!=0)
   {
      FileWrite(H1, &str[k], 1);
      k++;
   };

   for (int i=a1; i<=b1; i++)
   {
      tmp = "";
      for (int j=a2; j<=b2; j++)
      {
         tmp += "A[" + I2S(i) + "][" + I2S(j) + "]=" + C2SExp(A[i][j], Fidelity);
         tmp += "   ";
      };
      tmp += "\x0A\x0D";
      str = tmp.c_str();
      k = 0;
      while (str[k]!=0)
      {
        FileWrite(H1, &str[k], 1);
        k++;
      }
   };
   FileClose(H1);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall SaveDoubleMatrixToFile(AnsiString fname, AnsiString Comment, int Fidelity, double **A, int a1, int b1, int a2, int b2)
{
   AnsiString tmp;
   char *str;
   int k;
   if (FileExists(fname)) DeleteFile(fname);
   int H1 = FileCreate(fname);

   tmp = Comment;
   tmp += "\x0A\x0D";
   str = tmp.c_str();
   k = 0;
   while (str[k]!=0)
   {
      FileWrite(H1, &str[k], 1);
      k++;
   };

   for (int i=a1; i<=b1; i++)
   {
      tmp = "";
      for (int j=a2; j<=b2; j++)
      {
         tmp += "A[" + I2S(i) + "][" + I2S(j) + "]=" + F2S(A[i][j], Fidelity);
         tmp += "   ";
      };
      tmp += "\x0A\x0D";
      str = tmp.c_str();
      k = 0;
      while (str[k]!=0)
      {
        FileWrite(H1, &str[k], 1);
        k++;
      }
   };
   FileClose(H1);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall SaveFloatMatrixToFile(AnsiString fname, AnsiString Comment, int Fidelity, float **A, int a1, int b1, int a2, int b2)
{
   AnsiString tmp;
   char *str;
   int k;
   if (FileExists(fname)) DeleteFile(fname);
   int H1 = FileCreate(fname);

   tmp = Comment;
   tmp += "\x0A\x0D";
   str = tmp.c_str();
   k = 0;
   while (str[k]!=0)
   {
      FileWrite(H1, &str[k], 1);
      k++;
   };

   for (int i=a1; i<=b1; i++)
   {
      tmp = "";
      for (int j=a2; j<=b2; j++)
      {
         tmp += "A[" + I2S(i) + "][" + I2S(j) + "]=" + F2S(A[i][j], Fidelity);
         tmp += "   ";
      };
      tmp += "\x0A\x0D";
      str = tmp.c_str();
      k = 0;
      while (str[k]!=0)
      {
        FileWrite(H1, &str[k], 1);
        k++;
      }
   };
   FileClose(H1);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall SaveIntMatrixToFile(AnsiString fname, AnsiString Comment, int **A, int a1, int b1, int a2, int b2)
{
   AnsiString tmp;
   char *str;
   int k;
   if (FileExists(fname)) DeleteFile(fname);
   int H1 = FileCreate(fname);

   tmp = Comment;
   tmp += "\x0A\x0D";
   str = tmp.c_str();
   k = 0;
   while (str[k]!=0)
   {
      FileWrite(H1, &str[k], 1);
      k++;
   };

   for (int i=a1; i<=b1; i++)
   {
      tmp = "";
      for (int j=a2; j<=b2; j++)
      {
         tmp += "A[" + I2S(i) + "][" + I2S(j) + "]=" + I2S(A[i][j]);
         tmp += "   ";
      };
      tmp += "\x0A\x0D";
      str = tmp.c_str();
      k = 0;
      while (str[k]!=0)
      {
        FileWrite(H1, &str[k], 1);
        k++;
      }
   };
   FileClose(H1);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int __fastcall ReadFileStr(int H, AnsiString &str)
{
   int i = 0, j;
   char b,a[256];
   while (1)
   {
      j = FileRead(H, &b, 1);
      if (j == 0)
      {
         if (i == 0) return -1;
         break;
      };
      if (b == '\r')
      {
         FileRead(H, &b, 1);
         if (b!='\n') throw -2;
         break;
      };
      if (i>255) throw -3;
      a[i] = b;
      i++;
   };
   str.SetLength(i);
   for (j=1; j<=i; j++) str[j] = a[j-1];
   return i;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int __fastcall WriteFileStr(int H, AnsiString str)
{
   int L = str.Length();
   char a;
   for (int i=1; i<=L; i++) FileWrite(H, &(str[i]), 1);
   a = '\r';
   FileWrite(H, &a, 1);
   a = '\n';
   return FileWrite(H, &a, 1);
};
//---------------------------------------------------------------------------
AnsiString FloatStrComma2Point(AnsiString str)
{
   int iComma = str.Pos(",");
   if (iComma) str[iComma] = '.';
   return str;
};
//---------------------------------------------------------------------------
AnsiString FloatStrPoint2Comma(AnsiString str)
{
   int iComma = str.Pos(".");
   if (iComma) str[iComma] = ',';
   return str;
};
//---------------------------------------------------------------------------
AnsiString RemoveSpaces(AnsiString &str)
{
   AnsiString a;
   int len = str.Length();
   a.SetLength(len);
   int k = 1;
   for (int i=1; i<=len; i++)
   {
      if (str[i] != ' ')
      {
         a[k] = str[i];
         k++;
      };
   };
   a.SetLength(k-1);
   return a;
};
//---------------------------------------------------------------------------
AnsiString RemoveSemiColons(AnsiString &str)
{
   AnsiString a;
   int len = str.Length();
   a.SetLength(len);
   int k = 1;
   for (int i=1; i<=len; i++)
   {
      if (str[i] != ';')
      {
         a[k] = str[i];
         k++;
      };
   };
   a.SetLength(k-1);
   return a;
};
//---------------------------------------------------------------------------
AnsiString RemoveAfterSemiColons(AnsiString &str)
{
   AnsiString a;
   int len = str.Length();
   a.SetLength(len);
   int k = 1;
   for (int i=1; i<=len; i++)
   {
      if (str[i] != ';')
      {
         a[k] = str[i];
         k++;
      }
      else break;
   };
   a.SetLength(k-1);
   return a;
};
//---------------------------------------------------------------------------
__fastcall TPrmFile::TPrmFile(AnsiString FileName, AnsiString aSeparator)
{
   H = FileOpen(FileName, fmOpenRead);
   if (H == -1) throw "can't open ini file";
   Separator = aSeparator;
// чтение из файла в память
   AnsiString str;
   strarr = new TStringList();
   while (ReadFileStr(H, str) !=-1)
   {
      strarr->Add(str);
   };
   Error = 0;
};
//---------------------------------------------------------------------------
__fastcall TPrmFile::~TPrmFile()
{
   FileClose(H);
   delete strarr;
};
//---------------------------------------------------------------------------
int __fastcall TPrmFile::GetPrm(AnsiString PrmName, AnsiString &Prm)
{
   AnsiString str1, str2;
   int lens = Separator.Length();
   for (int i=0; i<strarr->Count; i++)
   {
      int pos = strarr->Strings[i].Pos(Separator);
      int len = strarr->Strings[i].Length();
      str1 = strarr->Strings[i].SubString(1,pos-1);
      str1 = RemoveSpaces(str1);
      if (str1 == PrmName)
      {
         str2 = strarr->Strings[i].SubString(pos + lens, len - pos + 1 - lens);
         str2 = RemoveAfterSemiColons(str2);
         str2 = RemoveSpaces(str2);
         Prm = str2;
         return 1;
      };
   };
   Error++;
   return 0;
};
//---------------------------------------------------------------------------
int __fastcall TPrmFile::GetPrm(AnsiString PrmName, int &Prm)
{
   AnsiString str1, str2;
   int lens = Separator.Length();
   for (int i=0; i<strarr->Count; i++)
   {
      int pos = strarr->Strings[i].Pos(Separator);
      int len = strarr->Strings[i].Length();
      str1 = strarr->Strings[i].SubString(1,pos-1);
      str1 = RemoveSpaces(str1);
      if (str1 == PrmName)
      {
         str2 = strarr->Strings[i].SubString(pos + lens, len - pos + 1 - lens);
         str2 = RemoveAfterSemiColons(str2);
         str2 = RemoveSpaces(str2);
         Prm = StrToInt(str2);
         return 1;
      };
   };
   Error++;
   return 0;
};
//---------------------------------------------------------------------------
int __fastcall TPrmFile::GetPrm(AnsiString PrmName, double &Prm)
{
   AnsiString str1, str2;
   int lens = Separator.Length();
   for (int i=0; i<strarr->Count; i++)
   {
      int pos = strarr->Strings[i].Pos(Separator);
      int len = strarr->Strings[i].Length();
      str1 = strarr->Strings[i].SubString(1,pos-1);
      str1 = RemoveSpaces(str1);
      if (str1 == PrmName)
      {
         str2 = strarr->Strings[i].SubString(pos + lens, len - pos + 1 - lens);
         str2 = RemoveAfterSemiColons(str2);
         str2 = RemoveSpaces(str2);
         Prm = StrToFloat(str2);
         return 1;
      };
   };
   Error++;
   return 0;
};
//---------------------------------------------------------------------------
int __fastcall TPrmFile::GetPrm(AnsiString PrmName, bool &Prm)
{
   AnsiString str1, str2;
   int lens = Separator.Length();
   for (int i=0; i<strarr->Count; i++)
   {
      int pos = strarr->Strings[i].Pos(Separator);
      int len = strarr->Strings[i].Length();
      str1 = strarr->Strings[i].SubString(1,pos-1);
      str1 = RemoveSpaces(str1);
      if (str1 == PrmName)
      {
         str2 = strarr->Strings[i].SubString(pos + lens, len - pos + 1 - lens);
         str2 = RemoveAfterSemiColons(str2);
         str2 = RemoveSpaces(str2);
         if (str2 == "true") Prm = true;
         else Prm = false;
         return 1;
      };
   };
   Error++;
   return 0;
};
//---------------------------------------------------------------------------
bool __fastcall TPrmFile::CheckError()
{
   if (Error) return true;
   return false;
};
//---------------------------------------------------------------------------



#pragma package(smart_init)

