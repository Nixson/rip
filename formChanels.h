//---------------------------------------------------------------------------

#ifndef formChanelsH
#define formChanelsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TFchanels : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TCSpinEdit *CVN0;
        TCSpinEdit *CVN2;
        TCSpinEdit *CVN1;
        TCSpinEdit *CVN3;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TCSpinEdit *CVS0;
        TCSpinEdit *CVS2;
        TCSpinEdit *CVS1;
        TCSpinEdit *CVS3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        void __fastcall CVN0Change(TObject *Sender);
        void __fastcall CVN2Change(TObject *Sender);
        void __fastcall CVN1Change(TObject *Sender);
        void __fastcall CVN3Change(TObject *Sender);
        void __fastcall CVS0Change(TObject *Sender);
        void __fastcall CVS2Change(TObject *Sender);
        void __fastcall CVS1Change(TObject *Sender);
        void __fastcall CVS3Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFchanels(TComponent* Owner);
        unsigned int cvn0;
        unsigned int cvn1;
        unsigned int cvn2;
        unsigned int cvn3;

        unsigned int cvs0;
        unsigned int cvs1;
        unsigned int cvs2;
        unsigned int cvs3;

};
//---------------------------------------------------------------------------
extern PACKAGE TFchanels *Fchanels;
//---------------------------------------------------------------------------
#endif
