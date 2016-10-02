//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "formRasterUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Raster"
#pragma resource "*.dfm"
TformRaster *formRaster;

//---------------------------------------------------------------------------
__fastcall TformRaster::TformRaster(TComponent* Owner)
   : TForm(Owner)
{
   Raster1->Align = alClient;
   Table = new TTable();
   Raster1->SetTable(Table);
}
//---------------------------------------------------------------------------

