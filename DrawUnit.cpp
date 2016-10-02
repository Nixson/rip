//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DrawUnit.h"
#include "params.h"
#include <math.hpp>
#include "mymath.h"
#include "TeeStore.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformDraw *formDraw;

extern unsigned int CoherentAuccumNum;// = 32;

double OscUnitCoef = 1e9;
double SpMax = 90;
AnsiString OscBottomAxisUnitLabel = "нс";

//---------------------------------------------------------------------------
__fastcall TformDraw::TformDraw(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TformDraw::DrawOsc(double *Data, unsigned int N)
{
   cPlot->Clear();
   for(unsigned int i=0;i<N;i++){
      cPlot->AddXY(i, Data[i], FloatToStrF(i*Td*OscUnitCoef,  ffFixed, 8, 2), clRed);
   }
}
//---------------------------------------------------------------------------
void TformDraw::DrawOscI(double *Data, unsigned int N)
{
   cPlotI->Clear();
   for(unsigned int i=0;i<N;i++){
      cPlotI->AddXY(i, Data[i], FloatToStrF(i*Td*OscUnitCoef,  ffFixed, 8, 2), clRed);
   }
}
//---------------------------------------------------------------------------
void TformDraw::DrawOscQ(double *Data, unsigned int N)
{
   cPlotQ->Clear();
   for(unsigned int i=0;i<N;i++){
      cPlotQ->AddXY(i, Data[i], FloatToStrF(i*Td*OscUnitCoef,  ffFixed, 8, 2), clGreen);
   }
}

//---------------------------------------------------------------------------
void TformDraw::DrawOscIQ(comp *Data, unsigned int N)
{
        if(N > 0 && cPlotI){
                cPlot->Clear();
                cPlotQ->Clear();
                for(unsigned int i=0;i<N;i++){
                        cPlotI->AddXY(i, Data[i]._M_re, FloatToStrF(i*Td*OscUnitCoef,  ffFixed, 8, 2), clGreen);
                        cPlotQ->AddXY(i, Data[i]._M_im, FloatToStrF(i*Td*OscUnitCoef,  ffFixed, 8, 2), clGreen);
                }
        }
}
//---------------------------------------------------------------------------
void TformDraw::DrawOscCoherentAccum(double *Data, unsigned int N)
{
   cCoherentAccum->Clear();
   for(unsigned int i=0;i<N;i++){
      cCoherentAccum->AddXY(i, Data[i], FloatToStrF(i*Td*OscUnitCoef,  ffFixed, 8, 2), clRed);
   }
}
//---------------------------------------------------------------------------
void TformDraw::DrawSp(double *Data, unsigned int N)
{
   double S = 0;
   int L = Log2(N);
   comp *FFTSpec;
   double *FFTData;

   FFTSpec = new comp[N];
   FFTData = new double[N];
   ZeroMemory(FFTSpec, N*sizeof(comp));
   ZeroMemory(FFTData, N*sizeof(double));
   
   for (unsigned int i=0; i<N; i++) S+=Data[i];
   S = S/double(N);
   
   for (unsigned int i=0; i<N; i++)
   {
      FFTData[i] = (Data[i] - S)*Wnd(i, wNattol, N);
   };
   
   FFT(FFTSpec, FFTData, L, NULL);

   double max = -10000000000000;
   double tmp;
   for (unsigned int i=0; i<N; i++)
      {
         tmp = abs(FFTSpec[i]);
         if (tmp <= 1e-12) FFTSpec[i] = -240;
         else FFTSpec[i] = -6.2+20*log10(tmp);
      };

   for (unsigned int i=0; i<N; i++) FFTSpec[i]-=SpMax;

   cPlot->Clear();
   for(unsigned int i=0;i<(N>>1);i++) cPlot->AddXY(i, FFTSpec[i+(N>>1)]._M_re, (Fd/2+i*Fd/N) * 1e-6, clRed);

   delete [] FFTSpec;
   delete [] FFTData;
}
//---------------------------------------------------------------------------
void TformDraw::DrawSpCoherentAccum(double *Data, unsigned int N)
{
   double S = 0;
   int L = Log2(N);
   comp *FFTSpec;
   double *FFTData;

   FFTSpec = new comp[N];
   FFTData = new double[N];
   ZeroMemory(FFTSpec, N*sizeof(comp));
   ZeroMemory(FFTData, N*sizeof(double));

   for (unsigned int i=0; i<N; i++) S+=Data[i];
   S = S/double(N);
   
   for (unsigned int i=0; i<N; i++)
   {
      FFTData[i] = (Data[i] - S)*Wnd(i, wNattol, N);
   };

   FFT(FFTSpec, FFTData, L, NULL);

   double max = -10000000000000;
   double tmp;
   for (unsigned int i=0; i<N; i++)
      {
         tmp = abs(FFTSpec[i]);
         if (tmp <= 1e-12) FFTSpec[i] = -240;
         else FFTSpec[i] = -6.2+20*log10(tmp);
      };

   for (unsigned int i=0; i<N; i++) FFTSpec[i]-=SpMax;

   cCoherentAccum->Clear();
   for(unsigned int i=0;i<(N>>1);i++) cCoherentAccum->AddXY(i, FFTSpec[i+(N>>1)]._M_re, (Fd/2+i*Fd/N) * 1e-6, clRed);

   delete [] FFTSpec;
   delete [] FFTData;
}
//---------------------------------------------------------------------------
void __fastcall TformDraw::N1Click(TObject *Sender)
{
   OscUnitCoef = 1e9;
   OscBottomAxisUnitLabel = "нс";
   Chart->BottomAxis->Title->Caption = "Время, "+OscBottomAxisUnitLabel;
   PopupMenu1->Items->Items[0]->Checked = true;
   PopupMenu1->Items->Items[1]->Checked = false;
   PopupMenu1->Items->Items[2]->Checked = false;
   PopupMenu1->Items->Items[3]->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TformDraw::N2Click(TObject *Sender)
{
   OscUnitCoef = 1e6;
   OscBottomAxisUnitLabel = "мкс";
   Chart->BottomAxis->Title->Caption = "Время, "+OscBottomAxisUnitLabel;
   PopupMenu1->Items->Items[0]->Checked = false;
   PopupMenu1->Items->Items[1]->Checked = true;
   PopupMenu1->Items->Items[2]->Checked = false;
   PopupMenu1->Items->Items[3]->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TformDraw::N3Click(TObject *Sender)
{
   OscUnitCoef = 3*1e8/2;
   OscBottomAxisUnitLabel = "м";
   Chart->BottomAxis->Title->Caption = "Растояние, "+OscBottomAxisUnitLabel;
   PopupMenu1->Items->Items[0]->Checked = false;
   PopupMenu1->Items->Items[1]->Checked = false;
   PopupMenu1->Items->Items[2]->Checked = true;
   PopupMenu1->Items->Items[3]->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TformDraw::SaveTEE(AnsiString FileName)
{
   AnsiString Path;
   Path = ExtractFilePath(Application->ExeName);

   SaveChartToFile(Chart, Path+FileName);
}
//---------------------------------------------------------------------------
void __fastcall TformDraw::SaveBMP(AnsiString FileName)
{
   AnsiString Path;
   Path = ExtractFilePath(Application->ExeName);

   Chart->SaveToBitmapFile(Path+FileName);
}
//---------------------------------------------------------------------------
void __fastcall TformDraw::SaveTXT(AnsiString FileName)
{
   AnsiString Path;
   Path = ExtractFilePath(Application->ExeName);
   unsigned Size = Chart->Series[0]->XValues->Count();

   TStringList *PlotTxt = new TStringList;
   for(unsigned int i=0;i<Size;i++)
   {
      PlotTxt->Add(FloatToStr(Chart->Series[0]->YValues->Value[i]));
   }
   PlotTxt->SaveToFile(Path+FileName);
}
//---------------------------------------------------------------------------
void __fastcall TformDraw::SaveDAT(AnsiString FileName)
{
   AnsiString Path;
   Path = ExtractFilePath(Application->ExeName);

   unsigned Size = Chart->Series[0]->XValues->Count();
   char *Data = new char[Size];

   for(unsigned i=0;i<Size;i++)
   Data[i] = (int) Chart->Series[0]->YValues->Value[i];

   TFileStream *PlotDat = new TFileStream(Path+FileName, fmCreate);
   PlotDat->WriteBuffer(Data, Size);
   delete PlotDat;
   delete [] Data;
}
//---------------------------------------------------------------------------
void __fastcall TformDraw::Samples1Click(TObject *Sender)
{
   OscUnitCoef = 1/Td;
   OscBottomAxisUnitLabel = "отсчёты";
   Chart->BottomAxis->Title->Caption = "Время, "+OscBottomAxisUnitLabel;
   PopupMenu1->Items->Items[0]->Checked = false;
   PopupMenu1->Items->Items[1]->Checked = false;
   PopupMenu1->Items->Items[2]->Checked = false;
   PopupMenu1->Items->Items[3]->Checked = true;

}
//---------------------------------------------------------------------------

