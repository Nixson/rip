//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ProgressUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformProgress *formProgress;

double TimeLeft = 100;

//---------------------------------------------------------------------------
__fastcall TformProgress::TformProgress(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TformProgress::Timer1Timer(TObject *Sender)
{
   if(formProgress->Visible)
   {

      Label1->Caption = "Осталось примерно " + FloatToStrF(TimeLeft, ffFixed, 8, 1) + " секунд ";
      if(TimeLeft*1000 > Timer1->Interval)
         TimeLeft = (TimeLeft*1000 - Timer1->Interval)/1000;
      else
      {
         BitBtn1->Visible = true;
      }
   }
   ProgressBar1->StepIt();
}
//---------------------------------------------------------------------------

void __fastcall TformProgress::FormHide(TObject *Sender)
{
   BitBtn1->Visible = false;
   Label2->Caption = "";
   Label3->Caption = "";
   Timer1->Enabled = false;
   ProgressBar1->Position = 0;   
}
//---------------------------------------------------------------------------
void __fastcall TformProgress::FormShow(TObject *Sender)
{
   Timer1->Enabled = true;    
}
//---------------------------------------------------------------------------
