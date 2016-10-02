//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
#include "ScatterUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TScatterForm *ScatterForm = NULL;
//---------------------------------------------------------------------------
__fastcall TScatterForm::TScatterForm(TComponent* Owner)
    : TForm(Owner)
{
    if(formMain)
    {
        formMain->IdUDPServer1->Active = false;
    }
    posY = 0;
    stop = 1;
    ApplyAddressButtonClick(this);
}
//---------------------------------------------------------------------------
__fastcall TScatterForm::~TScatterForm()
{
    if(formMain)
    {
        formMain->IdUDPServer1->Active = true;
    }
    ScatterForm = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::AddressEditChange(TObject *Sender)
{
    if(AddressEdit->Modified)
    {
        AddressEdit->Font->Color = clGreen;
    }
}
//---------------------------------------------------------------------------
// запуск измерений без усреднением
void __fastcall TScatterForm::ScanButtonClick(TObject *Sender)
{
    if(formMain)
    {
        stop = 0;
        avg = 0;
        PosX = posX;
        PosY = posY;
        ScanButton->Enabled = 0;
        ScanButtonAvg->Enabled = 0;
        StartSensing();
        Timer1->Enabled = true;
        stepX = StrToIntDef(StepEditX->Text,1);
        stepY = StrToIntDef(StepEditY->Text,1);
    }
}
//---------------------------------------------------------------------------
// запуск измерений с усреднением
void __fastcall TScatterForm::ScanButtonAvgClick(TObject *Sender)
{
    if(formMain)
    {
        stop = 0;
        avg = 1;
        PosX = posX;
        PosY = posY;
        ScanButton->Enabled = 0;
        ScanButtonAvg->Enabled = 0;
        StartSensing();
        stepX = StrToIntDef(StepEditX->Text,1);
        stepY = StrToIntDef(StepEditY->Text,1);
    }
}
//---------------------------------------------------------------------------
// обработка завершения цикла зондирования при текущем положении
void __fastcall TScatterForm::UpdatePosition()
{
    Timer1->Enabled = false;
    posX += stepX;
/*
    if((posX-PosX) < 200) // поворот вперед при фиксированном угле падения
    {
        IdUDPClient1->Send("XF001");
    }
    else if((posY-PosY) <= 2) // совершён полный круг - изменить угол падения
    {
        posY += 1;
        IdUDPClient1->Send("Y"+FormatFloat("00", posY));
        IdUDPClient1->Send("XB199");
    }
    else
    {
        stop = 1;
        ScanButton->Enabled = 1;
        ScanButtonAvg->Enabled = 1;
    }
*/
    AnsiString String = Memo1->Lines->Strings[0];
    String = String + "; Scaned";
    Memo1->Lines->Strings[0] = String;
    mode = 2;
    Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
// обработка завершения программы поворотного устройства
void __fastcall TScatterForm::StartSensing()
{
    if(formMain)
    {
        if(avg) formMain->bAvgPRClick(this);
        else formMain->bPRClick(this);
        recieve = Idglobal::GetTickCount();
        Timer1->Enabled = true;
        Memo1->Lines->Insert(0, "Y=" + FormatFloat("00",posY) + "; X=" + FormatFloat("000", posX));
        mode = 0; // режим зондирования
    }
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::IdUDPServer1UDPRead(TObject *Sender,
      TStream *AData, TIdSocketHandle *ABinding)
{
    // извлечение текстовой строки из принятого пакета
	AnsiString String;
	String = ((TStringStream*)AData)->DataString.SubString(0, AData->Size);
	// запуск нового сканирования, если программа поворота завершена
    posX = StrToInt(String);
    if(stop)
    {
        Memo1->Lines->Insert(0, "Y=" + FormatFloat("00", posY) + "; X=" + FormatFloat("000", posX));
    }
    else
    {
        String = Memo1->Lines->Strings[0];
        String = String + "; Moved";
        Memo1->Lines->Strings[0] = String;
    }
    if(stop)
    {
        ScanButton->Enabled = 1;
        ScanButtonAvg->Enabled = 1;
    }
    else
    {
        mode = 1;
        Timer1->Enabled = true;
    };
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::ApplyAddressButtonClick(TObject *Sender)
{
    int port;
    if(TryStrToInt(PortEdit->Text, port))
    {
        try
        {
            AddressEdit->Font->Color = clWindowText;
            IdUDPClient1->Active = false;
            IdUDPServer1->Active = false;
            IdUDPClient1->Host = AddressEdit->Text;
            IdUDPClient1->Port = port;
            IdUDPServer1->DefaultPort = port;
            IdUDPClient1->Active = true;
            IdUDPServer1->Active = true;
            IdUDPClient1->Send("Y00"); posY = 0;// угол падения установить в ноль
            IdUDPClient1->Send("S010"); // скорость поворота поставить на минимум
        }
        catch(Exception &exception)
        {
            ShowMessage(exception.Message);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::UpSpeedButtonClick(TObject *Sender)
{
    if(posY < 90)
    {
        posY++;
        IdUDPClient1->Send("Y"+FormatFloat("00", posY)); // угол падения установить в ноль
    }
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::DownSpeedButtonClick(TObject *Sender)
{
    if(posY > 0)
    {
        posY--;
        IdUDPClient1->Send("Y"+FormatFloat("00", posY)); // угол падения установить в ноль
    }
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::RightSpeedButtonClick(TObject *Sender)
{
    IdUDPClient1->Send("XF001"); // угол падения установить в ноль
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::LeftSpeedButtonClick(TObject *Sender)
{
    IdUDPClient1->Send("XB001"); // угол падения установить в ноль
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::Button1Click(TObject *Sender)
{
    IdUDPClient1->Send(Edit1->Text);
    int index = Edit1->Text.Pos("Y");
    AnsiString Text;
    if(index >= 0)
    {
        Text = Edit1->Text.SubString(index+1,2);
        posY = StrToInt(Text);
    }
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::Button2Click(TObject *Sender)
{
    stop = true;
    ScanButton->Enabled = 1;
    ScanButtonAvg->Enabled = 1;
}
//---------------------------------------------------------------------------
void __fastcall TScatterForm::Timer1Timer(TObject *Sender)
{
    unsigned timer;
    switch(mode)
    {
        case 0:
            timer = Idglobal::GetTickCount() - recieve;
            if(timer > 0x80000000) timer = 0xFFFFFFFF - timer;
            if(timer > 5000)
            {
                recieve = Idglobal::GetTickCount();
                AnsiString String = Memo1->Lines->Strings[0];
                String = String + "; Timeout";
                Memo1->Lines->Strings[0] = String;
                StartSensing();
            }
            break;
        case 1:
            StartSensing(); // запуск зондирования после поворота
            break;
        case 2: // запуск поворота объекта
            if((posX-PosX) < 200) // поворот вперед при фиксированном угле падения
            {
                IdUDPClient1->Send("XF"+FormatFloat("000", stepX));
            }
            else if((posY-PosY) <= 40) // совершён полный круг - изменить угол падения
            {
                posY += stepY;
                IdUDPClient1->Send("Y"+FormatFloat("00", posY));
                IdUDPClient1->Send("XB"+FormatFloat("000", 200-stepX));
            }
            else
            {
                stop = 1;
                mode = -1;
                ScanButton->Enabled = 1;
                ScanButtonAvg->Enabled = 1;
            }
            break;
    }
    Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

