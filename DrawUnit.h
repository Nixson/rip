//---------------------------------------------------------------------------

#ifndef DrawUnitH
#define DrawUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Menus.hpp>
#include "myprocs.h"
//---------------------------------------------------------------------------
class TformDraw : public TForm
{
__published:	// IDE-managed Components
   TChart *Chart;
   TFastLineSeries *cPlot;
   TFastLineSeries *cPlotI;
   TFastLineSeries *cPlotQ;
   TFastLineSeries *cCoherentAccum;
   TPopupMenu *PopupMenu1;
   TMenuItem *N1;
   TMenuItem *N2;
   TMenuItem *N3;
        TMenuItem *Samples1;

   void __fastcall N1Click(TObject *Sender);
   void __fastcall N2Click(TObject *Sender);
   void __fastcall N3Click(TObject *Sender);
        void __fastcall Samples1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TformDraw(TComponent* Owner);
   void TformDraw::DrawOsc(double *Data, unsigned int N);
   void TformDraw::DrawOscI(double *Data, unsigned int N);
   void TformDraw::DrawOscQ(double *Data, unsigned int N);
   void TformDraw::DrawOscIQ(comp *Data, unsigned int N);
   void TformDraw::DrawOscCoherentAccum(double *Data, unsigned int N);
   void TformDraw::DrawSp(double *Data, unsigned int N);
   void TformDraw::DrawSpCoherentAccum(double *Data, unsigned int N);

   void __fastcall TformDraw::SaveTEE(AnsiString FileName);
   void __fastcall TformDraw::SaveBMP(AnsiString FileName);
   void __fastcall TformDraw::SaveTXT(AnsiString FileName);
   void __fastcall TformDraw::SaveDAT(AnsiString FileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TformDraw *formDraw;
//---------------------------------------------------------------------------
#endif
