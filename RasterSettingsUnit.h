//---------------------------------------------------------------------------

#ifndef RasterSettingsUnitH
#define RasterSettingsUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TformRasterSettings : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TLabel *LabelX1;
        TLabel *LabelX2;
        TEdit *EditX1;
        TEdit *EditX2;
        TUpDown *UpDownX1;
        TUpDown *UpDownX2;
        TCheckBox *CheckBoxX1;
        TLabel *LabelX3;
        TEdit *EditX3;
        TLabel *LabelX4;
        TEdit *EditX4;
        TGroupBox *GroupBox2;
        TLabel *LabelY1;
        TLabel *LabelY2;
        TLabel *LabelY3;
        TLabel *LabelY4;
        TEdit *EditY1;
        TEdit *EditY2;
        TUpDown *UpDownY1;
        TUpDown *UpDownY2;
        TCheckBox *CheckBoxY1;
        TEdit *EditY3;
        TEdit *EditY4;
        TCheckBox *chXXparam;
        TCheckBox *chYYparam;
        void __fastcall EditChange(TObject *Sender);
        void __fastcall CheckBoxX1Click(TObject *Sender);
        void __fastcall CheckBoxY1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall UpDownX1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDownX2Click(TObject *Sender, TUDBtnType Button);
        void __fastcall EditX2Change(TObject *Sender);
        void __fastcall EditX1Change(TObject *Sender);
        void __fastcall UpDownY1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall EditY2Change(TObject *Sender);
        void __fastcall EditY1Change(TObject *Sender);
        void __fastcall UpDownY2Click(TObject *Sender, TUDBtnType Button);
        void __fastcall chXXparamClick(TObject *Sender);
        void __fastcall chYYparamClick(TObject *Sender);
private:	// User declarations
        int XRlo;
        int XRhi;
        AnsiString XZlo;
        AnsiString XZhi;
        bool Xauto;
        int YRlo;
        int YRhi;
        AnsiString YZlo;
        AnsiString YZhi;
        bool Yauto;
public:		// User declarations
        __fastcall TformRasterSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformRasterSettings *formRasterSettings;
//---------------------------------------------------------------------------
#endif
