//---------------------------------------------------------------------------

#ifndef ControlOptionsUnitH
#define ControlOptionsUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
//---------------------------------------------------------------------------
class TFormControlOptions : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TButton *Button1;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TChart *Chart1;
private:	// User declarations
public:		// User declarations
        __fastcall TFormControlOptions(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormControlOptions *FormControlOptions;
//---------------------------------------------------------------------------
#endif
