//---------------------------------------------------------------------------

#ifndef MeasureUnitH
#define MeasureUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CSPIN.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TformMeasureSettings : public TForm
{
__published:	// IDE-managed Components
        TLabeledEdit *ThresholdEdit;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *MinEdit;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *MaxEdit;
        TEdit *MinSEdit;
        TEdit *MaxSEdit;
        TUpDown *MinUpDown;
        TUpDown *MaxUpDown;
        TLabeledEdit *BearingEdit;
        void __fastcall MinSEditChange(TObject *Sender);
        void __fastcall MaxSEditChange(TObject *Sender);
        void __fastcall MinUpDownClick(TObject *Sender, TUDBtnType Button);
        void __fastcall MaxUpDownClick(TObject *Sender, TUDBtnType Button);
        void __fastcall MinEditChange(TObject *Sender);
        void __fastcall MaxEditChange(TObject *Sender);
        void __fastcall ThresholdEditChange(TObject *Sender);
        void __fastcall BearingEditChange(TObject *Sender);
private:	// User declarations
        bool autochange;
        bool editchange;
public:		// User declarations
        double Threshold;
        double BearingBase;
        __fastcall TformMeasureSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformMeasureSettings *formMeasureSettings;
//---------------------------------------------------------------------------
#endif
