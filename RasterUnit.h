//---------------------------------------------------------------------------

#ifndef RasterUnitH
#define RasterUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Raster.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TformRaster : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TCheckBox *CheckBox1;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *Edit1;
        TEdit *Edit2;
        TPanel *Panel2;
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall Scaling(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TformRaster(TComponent* Owner);
   __fastcall ~TformRaster(void);
   TTable *Table;
   TRaster *Raster1;

};
//---------------------------------------------------------------------------
//extern PACKAGE TformRaster *formRaster;
//---------------------------------------------------------------------------
#endif
