//---------------------------------------------------------------------------

#ifndef formRasterUnitH
#define formRasterUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Raster.h"

//---------------------------------------------------------------------------
class TformRaster : public TForm
{
__published:	// IDE-managed Components
   TRaster *Raster1;
private:	// User declarations
public:		// User declarations
   __fastcall TformRaster(TComponent* Owner);
   TTable *Table;
};
//---------------------------------------------------------------------------
extern PACKAGE TformRaster *formRaster;
//---------------------------------------------------------------------------
#endif
