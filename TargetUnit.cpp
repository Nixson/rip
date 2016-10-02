//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TargetUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformTargetDraw *formTargetDraw;
//---------------------------------------------------------------------------
__fastcall TformTargetDraw::TformTargetDraw(TComponent* Owner)
        : TForm(Owner)
{
  Button1->Align = alBottom;
  Chart1->Align = alClient;
}
//---------------------------------------------------------------------------
void __fastcall TformTargetDraw::Button1Click(TObject *Sender)
{
  Series1->Clear();      
  Series2->Clear();
  Series3->Clear();
}
//---------------------------------------------------------------------------

