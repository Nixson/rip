//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "formChanels.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFchanels *Fchanels;
//---------------------------------------------------------------------------
__fastcall TFchanels::TFchanels(TComponent* Owner)
        : TForm(Owner)
{
        cvn0 = (unsigned int)CVN0->Value;
        cvn1 = (unsigned int)CVN1->Value;
        cvn2 = (unsigned int)CVN2->Value;
        cvn3 = (unsigned int)CVN3->Value;
        cvs0 = (unsigned int)CVS0->Value;
        cvs1 = (unsigned int)CVS1->Value;
        cvs2 = (unsigned int)CVS2->Value;
        cvs3 = (unsigned int)CVS3->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFchanels::CVN0Change(TObject *Sender)
{
        cvn0 = (unsigned int)CVN0->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFchanels::CVN2Change(TObject *Sender)
{
        cvn2 = (unsigned int)CVN2->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFchanels::CVN1Change(TObject *Sender)
{
        cvn1 = (unsigned int)CVN1->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFchanels::CVN3Change(TObject *Sender)
{
        cvn3 = (unsigned int)CVN3->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFchanels::CVS0Change(TObject *Sender)
{
        cvs0 = (unsigned int)CVS0->Value;        
}
//---------------------------------------------------------------------------

void __fastcall TFchanels::CVS2Change(TObject *Sender)
{
        cvs2 = (unsigned int)CVS2->Value;        
}
//---------------------------------------------------------------------------

void __fastcall TFchanels::CVS1Change(TObject *Sender)
{
        cvs1 = (unsigned int)CVS1->Value;        
}
//---------------------------------------------------------------------------

void __fastcall TFchanels::CVS3Change(TObject *Sender)
{
        cvs3 = (unsigned int)CVS3->Value;        
}
//---------------------------------------------------------------------------


