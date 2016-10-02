//---------------------------------------------------------------------------

#ifndef OrgMUnitH
#define OrgMUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TFormOrgM : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TCheckBox *CheckBox1;
        TCheckBox *CheckBox2;
        TGroupBox *GroupBox2;
        TCheckBox *CheckBox3;
        TCheckBox *CheckBox4;
        TCheckBox *CheckBox5;
        TCheckBox *CheckBox6;
        TLabel *Label1;
        TCSpinEdit *CSpinEdit1;
        TCSpinEdit *CSpinEdit2;
        TLabel *Label2;
        TLabel *Label3;
private:	// User declarations
public:		// User declarations
        __fastcall TFormOrgM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOrgM *FormOrgM;
//---------------------------------------------------------------------------
#endif
