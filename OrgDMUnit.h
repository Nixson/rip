//---------------------------------------------------------------------------

#ifndef OrgDMUnitH
#define OrgDMUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TFormOrgDM : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TCheckBox *CheckBox1;
        TCheckBox *CheckBox2;
        TGroupBox *GroupBox2;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TCheckBox *CheckBox3;
        TCheckBox *CheckBox4;
        TCheckBox *CheckBox5;
        TCheckBox *CheckBox6;
        TCSpinEdit *CSpinEdit1;
        TCSpinEdit *CSpinEdit2;
private:	// User declarations
public:		// User declarations
        __fastcall TFormOrgDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOrgDM *FormOrgDM;
//---------------------------------------------------------------------------
#endif
