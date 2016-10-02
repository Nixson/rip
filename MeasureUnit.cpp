//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MeasureUnit.h"
#include "MainUnit.h"
#include "params.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TformMeasureSettings *formMeasureSettings;
//---------------------------------------------------------------------------
__fastcall TformMeasureSettings::TformMeasureSettings(TComponent* Owner)
        : TForm(Owner)
{
  bool Edit;
  Threshold = 2000;
  autochange = 0; // переменная для индикации автоизменений
  BearingBase = 0.1;
  MinUpDownClick(this, (TUDBtnType)0);
  MaxUpDownClick(this, (TUDBtnType)0);
}
//---------------------------------------------------------------------------
void __fastcall TformMeasureSettings::MinSEditChange(TObject *Sender)
{
  if(MinSEdit->Modified)
  {
    int value;
    if(TryStrToInt(MinSEdit->Text, value))
    {
      editchange = 1;
      MinUpDown->Position = value;
      MinUpDownClick(Sender, (TUDBtnType)0);
    }
    else if(MinSEdit->Text != "") ShowMessage("Ошибка ввода");
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMeasureSettings::MaxSEditChange(TObject *Sender)
{
  if(MaxSEdit->Modified)
  {
    int value;
    if(TryStrToInt(MaxSEdit->Text, value))
    {
      editchange = 1;
      MaxUpDown->Position = value;
      MaxUpDownClick(Sender, (TUDBtnType)0);
    }
    else if(MaxSEdit->Text != "") ShowMessage("Ошибка ввода");
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMeasureSettings::MinUpDownClick(TObject *Sender,
      TUDBtnType Button)
{
  if(editchange) editchange = 0;
  else MinSEdit->Text = IntToStr(MinUpDown->Position);
  MinEdit->Text = FormatFloat("###0.###", 3e8*MinUpDown->Position*Td/2);
  if(!autochange)
  {
    if(MaxUpDown->Position < MinUpDown->Position)
    {
//      autochange = 1;
      MaxUpDown->Position = MinUpDown->Position;
      MaxUpDownClick(Sender, (TUDBtnType)0);
    }
    //formMain->MinUpDownChange(Sender);
  }
  else autochange = 0;
  if(formMain->MinUpDownPosition != MinUpDown->Position){
  formMain->MinUpDown->Position = -MinUpDown->Position;
  /*formMain->MaxUpDown->SelStart = MinUpDown->Position;
  formMain->MaxUpDown->SelEnd = MaxUpDown->Position;
  formMain->MinUpDown->SelStart = MinUpDown->Position;
  formMain->MinUpDown->SelEnd = MaxUpDown->Position;*/
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMeasureSettings::MaxUpDownClick(TObject *Sender,
      TUDBtnType Button)
{
  if(editchange) editchange = 0;
  else MaxSEdit->Text = IntToStr(MaxUpDown->Position);
  MaxEdit->Text = FormatFloat("###0.###", 3e8*MaxUpDown->Position*Td/2);
  if(!autochange)
  {
    if(MinUpDown->Position > MaxUpDown->Position)
    {
//      autochange = 1;
      MinUpDown->Position = MaxUpDown->Position;
      MinUpDownClick(Sender, (TUDBtnType)0);
    }
    //formMain->MaxUpDownChange(Sender);
  }
  else autochange = 0;
  if(formMain->MaxUpDownPosition != MaxUpDown->Position){
  formMain->MaxUpDown->Position = -MaxUpDown->Position;
  //formMain->MaxUpDownChange(Sender);
/*  formMain->MaxUpDownPosition = MaxUpDown->Position;
  formMain->MaxUpDown->Position = -MaxUpDown->Position;
  formMain->MaxUpDown->SelStart = -MinUpDown->Position;
  formMain->MaxUpDown->SelEnd = -MaxUpDown->Position;
  formMain->MinUpDown->SelStart = -MinUpDown->Position;
  formMain->MinUpDown->SelEnd = -MaxUpDown->Position;*/
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMeasureSettings::MinEditChange(TObject *Sender)
{
  if(MinEdit->Modified)
  {
    double value;
    if(TryStrToFloat(MinEdit->Text, value))
    {
      value = MinEdit->Text.ToDouble();
      value *= 2/(3e8*Td);
      MinUpDown->Position = floor(value);
      MinUpDownClick(Sender, (TUDBtnType)0);
    }
    else if(MinEdit->Text != "") ShowMessage("Ошибка ввода");
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMeasureSettings::MaxEditChange(TObject *Sender)
{
  if(MaxEdit->Modified)
  {
    double value;
    if(TryStrToFloat(MaxEdit->Text, value))
    {
      value = MaxEdit->Text.ToDouble();
      value *= 2/(3e8*Td);
      MaxUpDown->Position = ceil(value);
      MaxUpDownClick(Sender, (TUDBtnType)0);
    }
    else if(MinEdit->Text != "") ShowMessage("Ошибка ввода");
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMeasureSettings::ThresholdEditChange(TObject *Sender)
{
  if(ThresholdEdit->Modified)
  {
    float value;
    if(TryStrToFloat(ThresholdEdit->Text, value))
    {
      Threshold = ThresholdEdit->Text.ToDouble();
    }
    else if(ThresholdEdit->Text != "") ShowMessage("Ошибка ввода");
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMeasureSettings::BearingEditChange(TObject *Sender)
{
  if(BearingEdit->Modified)
  {
    float value;
    if(TryStrToFloat(BearingEdit->Text, value))
    {
      BearingBase = BearingEdit->Text.ToDouble()/1000;
    }
    else if(BearingEdit->Text != "") ShowMessage("Ошибка ввода");
  }
}
//---------------------------------------------------------------------------


