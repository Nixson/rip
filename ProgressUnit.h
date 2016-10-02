//---------------------------------------------------------------------------

#ifndef ProgressUnitH
#define ProgressUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TformProgress : public TForm
{
__published:	// IDE-managed Components
   TProgressBar *ProgressBar1;
   TTimer *Timer1;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TBitBtn *BitBtn1;
   void __fastcall Timer1Timer(TObject *Sender);
   void __fastcall FormHide(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TformProgress(TComponent* Owner);
   double TimeLeft;
};
//---------------------------------------------------------------------------
extern PACKAGE TformProgress *formProgress;
//---------------------------------------------------------------------------
#endif
