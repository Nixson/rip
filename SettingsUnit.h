//---------------------------------------------------------------------------

#ifndef SettingsUnitH
#define SettingsUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TformSettings : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TLabeledEdit *leSubBufNum;
   TBitBtn *BitBtn1;
   TGroupBox *GroupBox2;
   TLabeledEdit *leFreq;
   TLabeledEdit *leAmp;
   TCSpinButton *CSpinButton1;
   TCSpinButton *CSpinButton2;
   TGroupBox *GroupBox3;
   TLabeledEdit *lePeriod;
   TLabeledEdit *leDuration;
   TCheckBox *cbPulseMod;
   TCheckBox *cbLFM;
   TLabeledEdit *leFreqRange;
   TCSpinButton *CSpinButton3;
   TCSpinButton *CSpinButton4;
   TCSpinButton *CSpinButton5;
   TCheckBox *cbUWB;
   TGroupBox *GroupBox4;
   TLabeledEdit *leGateDelay;
   TLabeledEdit *leGateDuration;
   TCheckBox *cbGate;
   TGroupBox *GroupBox5;
   TCheckBox *cbCont;
   TCheckBox *cbCoherentAccum;
   TCheckBox *cbDDSReset;
   TGroupBox *GroupBox6;
   TRadioButton *rbTxPolXX;
   TRadioButton *rbTxPolXY;
   TRadioButton *rbTxPolYX;
   TRadioButton *rbTxPolYY;
   TLabeledEdit *leTxAtt;
   TCSpinButton *sbTxAtt;
   TGroupBox *GroupBox7;
   TRadioButton *rbRxPolXX;
   TRadioButton *rbRxPolYY;
   TRadioButton *rbRxPolXY;
   TRadioButton *rbRxPolYX;
   TLabeledEdit *leRxAtt;
   TCSpinButton *sbRxAtt;
   TPanel *Panel2;
   TRadioButton *rbRxAnt0;
   TRadioButton *rbRxAnt1;
   TCSpinButton *sbMaxSubBufNum;
   TLabeledEdit *leBurstLen;
   TCSpinButton *CSpinButton6;
   TRadioButton *rbDdsRstBurst;
   TRadioButton *rbDdsRstPulse;
   TCheckBox *cbLOGM;
        TLabeledEdit *leGeterodin;
        TCSpinButton *CSpinButton7;
        TCheckBox *cbMGEN;
        TCSpinEdit *seMLEN;

   void __fastcall FormCreate(TObject *Sender);

   void __fastcall cbPulseModClick(TObject *Sender);

   void __fastcall CSpinButton1UpClick(TObject *Sender);
   void __fastcall CSpinButton1DownClick(TObject *Sender);
   void __fastcall CSpinButton2UpClick(TObject *Sender);
   void __fastcall CSpinButton2DownClick(TObject *Sender);
   void __fastcall CSpinButton3UpClick(TObject *Sender);
   void __fastcall CSpinButton3DownClick(TObject *Sender);
   void __fastcall CSpinButton4UpClick(TObject *Sender);
   void __fastcall CSpinButton4DownClick(TObject *Sender);
   void __fastcall CSpinButton5UpClick(TObject *Sender);
   void __fastcall CSpinButton5DownClick(TObject *Sender);
   void __fastcall CSpinButton6UpClick(TObject *Sender);
   void __fastcall CSpinButton6DownClick(TObject *Sender);

   void __fastcall sbTxAttUpClick(TObject *Sender);
   void __fastcall sbTxAttDownClick(TObject *Sender);
   void __fastcall sbRxAttUpClick(TObject *Sender);
   void __fastcall sbRxAttDownClick(TObject *Sender);
   void __fastcall rbTxPolXXClick(TObject *Sender);
   void __fastcall rbTxPolXYClick(TObject *Sender);
   void __fastcall rbTxPolYXClick(TObject *Sender);
   void __fastcall rbTxPolYYClick(TObject *Sender);
   void __fastcall rbRxPolXXClick(TObject *Sender);
   void __fastcall rbRxPolXYClick(TObject *Sender);
   void __fastcall rbRxPolYXClick(TObject *Sender);
   void __fastcall rbRxPolYYClick(TObject *Sender);

   void __fastcall rbRxAnt0Click(TObject *Sender);
   void __fastcall rbRxAnt1Click(TObject *Sender);
   void __fastcall cbContClick(TObject *Sender);
   void __fastcall BitBtn1Click(TObject *Sender);
   void __fastcall cbLFMClick(TObject *Sender);
   void __fastcall cbUWBClick(TObject *Sender);
   void __fastcall cbGateClick(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall sbMaxSubBufNumDownClick(TObject *Sender);
   void __fastcall sbMaxSubBufNumUpClick(TObject *Sender);
   void __fastcall cbCoherentAccumClick(TObject *Sender);
   void __fastcall cbDDSResetClick(TObject *Sender);
   void __fastcall cbLOGMClick(TObject *Sender);
        void __fastcall CSpinButton7UpClick(TObject *Sender);
        void __fastcall CSpinButton7DownClick(TObject *Sender);



private:
public:		// User declarations
   __fastcall TformSettings(TComponent* Owner);
   void __fastcall TformSettings::SaveConfig(AnsiString FileName);
   void __fastcall TformSettings::LoadConfig(AnsiString FileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TformSettings *formSettings;
//---------------------------------------------------------------------------
#endif
