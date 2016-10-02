//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ThresholdUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TThresholdForm *ThresholdForm;
//---------------------------------------------------------------------------
__fastcall TThresholdForm::TThresholdForm(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TThresholdForm::Button1Click(TObject *Sender)
{
  Button1->Caption = "мнплю";
  Button1->Font->Color = clWindowText;
  Button1->Enabled = 0;
}
//---------------------------------------------------------------------------

void __fastcall TThresholdForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action = caNone;
  Visible = 0;      
}
//---------------------------------------------------------------------------

