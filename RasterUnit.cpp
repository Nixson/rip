//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RasterUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TformRaster *formRaster;

//---------------------------------------------------------------------------
__fastcall TformRaster::TformRaster(TComponent* Owner)
   : TForm(Owner)
{
   Panel1->Align = alTop;
   Table = new TTable();
   Raster1 = new TRaster(Panel2);
   Raster1->SetTable(Table);
}
//---------------------------------------------------------------------------
__fastcall TformRaster::~TformRaster(void)
{
   if(Raster1)
   {
     Raster1->ResetTable();
     delete Raster1;
   }
   if(Table) delete Table;
}
//---------------------------------------------------------------------------
void __fastcall TformRaster::CheckBox1Click(TObject *Sender)
{
  Raster1->AutoScaleZ = CheckBox1->Checked;
  Edit1->Enabled = !CheckBox1->Checked;
  Edit2->Enabled = !CheckBox1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TformRaster::Scaling(TObject *Sender)
{
  double zlo, zhi;
  if(TryStrToFloat(Edit1->Text, zlo) && TryStrToFloat(Edit2->Text, zhi))
  {
    zlo = Edit1->Text.ToDouble() ;
    zhi = Edit2->Text.ToDouble() ;
    if(zlo <= zhi)
    {
      Raster1->SetLimitZ(zlo, zhi);
      Raster1->Plot();
    }
    else ShowMessage("Zмин > Z макс");
  }
  else if(Edit1->Text!="" && Edit2->Text!="")
    ShowMessage("Неверный ввод");
}
//---------------------------------------------------------------------------

