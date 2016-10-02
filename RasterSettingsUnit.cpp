//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RasterSettingsUnit.h"
#include "MainUnit.h"
#include "RasterUnit.cpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformRasterSettings *formRasterSettings;
extern TformRaster *formRasterDrawXX;
extern TformRaster *formRasterDrawYY;
//---------------------------------------------------------------------------
__fastcall TformRasterSettings::TformRasterSettings(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::EditChange(TObject *Sender)
{
  TEdit *Edit = (TEdit*)Sender;
  if(Edit->Modified)
  {
    double value;
    if(!TryStrToFloat(Edit->Text, value))
    {
      Edit->Font->Color = clRed;
    }
    else
    {
      Edit->Font->Color = clWindowText;
      // определение канала, которым управлять
      double zlo, zhi;
      if(Edit->Name == "EditX3" || Edit->Name == "EditX4")
      {
        if(!TryStrToFloat(EditX3->Text, value)) return;
        if(!TryStrToFloat(EditX4->Text, value)) return;
        zlo = formRasterSettings->EditX3->Text.ToDouble();
        zhi = formRasterSettings->EditX4->Text.ToDouble();
        formRasterDrawXX->Raster1->SetLimitZ(zlo, zhi);
        if(!CheckBoxX1->Checked) formRasterDrawXX->Raster1->Plot();
      }
      else
      {
        if(!TryStrToFloat(EditY3->Text, value)) return;
        if(!TryStrToFloat(EditY4->Text, value)) return;
        zlo = formRasterSettings->EditY3->Text.ToDouble();
        zhi = formRasterSettings->EditY4->Text.ToDouble();
        formRasterDrawYY->Raster1->SetLimitZ(zlo, zhi);
        if(!CheckBoxY1->Checked) formRasterDrawYY->Raster1->Plot();
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::CheckBoxX1Click(TObject *Sender)
{
  formRasterDrawXX->Raster1->AutoScaleZ = CheckBoxX1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::CheckBoxY1Click(TObject *Sender)
{
  formRasterDrawYY->Raster1->AutoScaleZ = CheckBoxY1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::FormShow(TObject *Sender)
{
  XRlo = UpDownX1->Position;
  XRhi = UpDownX2->Position;
  Xauto = CheckBoxX1->Checked;
  XZlo = EditX3->Text;
  XZhi = EditX4->Text;

  YRlo = UpDownY1->Position;
  YRhi = UpDownY2->Position;
  Yauto = CheckBoxY1->Checked;
  YZlo = EditY3->Text;
  YZhi = EditY4->Text;
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action = caNone;
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::UpDownX1Click(TObject *Sender,
      TUDBtnType Button)
{
  UpDownX2->Min = UpDownX1->Position;
  EditX2->Text = IntToStr(UpDownX2->Position);
  formMain->Action1Execute(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::UpDownX2Click(TObject *Sender,
      TUDBtnType Button)
{
  UpDownX1->Max = UpDownX2->Position;
  EditX1->Text = IntToStr(UpDownX1->Position);
  formMain->Action1Execute(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::EditX2Change(TObject *Sender)
{
  if(EditX2->Modified)
  {
    int value;
    if(TryStrToInt(EditX2->Text, value))
    {
      if(value < UpDownX2->Min) EditX2->Text = IntToStr(UpDownX2->Min);
      else if(value > UpDownX2->Max) EditX2->Text = IntToStr(UpDownX2->Max);
      UpDownX2->Position = value;
      UpDownX2Click(Sender, btNext);
    }
    else if(EditX2->Text != "") EditX2->Text = IntToStr(UpDownX2->Position);
  }
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::EditX1Change(TObject *Sender)
{
  if(EditX1->Modified)
  {
    int value;
    if(TryStrToInt(EditX1->Text, value))
    {
      if(value < UpDownX1->Min) EditX1->Text = IntToStr(UpDownX1->Min);
      else if(value > UpDownX1->Max) EditX1->Text = IntToStr(UpDownX1->Max);
      UpDownX1->Position = value;
      UpDownX1Click(Sender, btNext);
    }
    else if(EditX1->Text != "") EditX1->Text = IntToStr(UpDownX1->Position);
  }
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::UpDownY1Click(TObject *Sender,
      TUDBtnType Button)
{
  UpDownY2->Min = UpDownY1->Position;
  EditY2->Text = IntToStr(UpDownY2->Position);
  formMain->Action1Execute(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TformRasterSettings::UpDownY2Click(TObject *Sender,
      TUDBtnType Button)
{
  UpDownY1->Max = UpDownY2->Position;
  EditY1->Text = IntToStr(UpDownY1->Position);
  formMain->Action1Execute(Sender);

}
//---------------------------------------------------------------------------

void __fastcall TformRasterSettings::EditY2Change(TObject *Sender)
{
  if(EditY2->Modified)
  {
    int value;
    if(TryStrToInt(EditY2->Text, value))
    {
      if(value < UpDownY2->Min) EditY2->Text = IntToStr(UpDownY2->Min);
      else if(value > UpDownY2->Max) EditY2->Text = IntToStr(UpDownY2->Max);
      UpDownY2->Position = value;
      UpDownY2Click(Sender, btNext);
    }
    else if(EditY2->Text != "") EditY2->Text = IntToStr(UpDownY2->Position);
  }

}
//---------------------------------------------------------------------------

void __fastcall TformRasterSettings::EditY1Change(TObject *Sender)
{
  if(EditY1->Modified)
  {
    int value;
    if(TryStrToInt(EditY1->Text, value))
    {
      if(value < UpDownY1->Min) EditY1->Text = IntToStr(UpDownY1->Min);
      else if(value > UpDownY1->Max) EditY1->Text = IntToStr(UpDownY1->Max);
      UpDownY1->Position = value;
      UpDownY1Click(Sender, btNext);
    }
    else if(EditY1->Text != "") EditY1->Text = IntToStr(UpDownY1->Position);
  }
}
//---------------------------------------------------------------------------


void __fastcall TformRasterSettings::chXXparamClick(TObject *Sender)
{
        if(chXXparam->Checked)
                formRasterDrawXX->Raster1->NameY = "Дальность, отсчёты";
        else
                formRasterDrawXX->Raster1->NameY = "Дальность, метры";
        formMain->Action1Execute(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TformRasterSettings::chYYparamClick(TObject *Sender)
{
        if(chYYparam->Checked)
                formRasterDrawYY->Raster1->NameY = "Дальность, отсчёты";
        else
                formRasterDrawYY->Raster1->NameY = "Дальность, метры";
        formMain->Action1Execute(Sender);
}
//---------------------------------------------------------------------------

