//---------------------------------------------------------------------------

#ifndef ThresholdUnitH
#define ThresholdUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TThresholdForm : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit1;
        TUpDown *UpDown1;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *Edit2;
        TLabel *Label3;
        TEdit *Edit3;
        TUpDown *UpDown2;
        TUpDown *UpDown3;
        TSpeedButton *Button1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TThresholdForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TThresholdForm *ThresholdForm;
//---------------------------------------------------------------------------
#endif
