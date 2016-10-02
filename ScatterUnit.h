//---------------------------------------------------------------------------

#ifndef ScatterUnitH
#define ScatterUnitH
#define _WINSOCKAPI_
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <Buttons.hpp>
#include <Sockets.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TScatterForm : public TForm
{
__published:	// IDE-managed Components
    TIdUDPServer *IdUDPServer1;
    TIdUDPClient *IdUDPClient1;
    TEdit *AddressEdit;
    TLabel *HostLabel;
    TButton *ScanButtonAvg;
    TGroupBox *GroupBox1;
    TSpeedButton *DownSpeedButton;
    TSpeedButton *LeftSpeedButton;
    TSpeedButton *RightSpeedButton;
    TSpeedButton *UpSpeedButton;
    TButton *ScanButton;
    TButton *ApplyAddressButton;
    TMemo *Memo1;
    TLabel *Label1;
    TEdit *PortEdit;
    TEdit *Edit1;
    TButton *Button1;
    TButton *Button2;
    TTimer *Timer1;
    TEdit *StepEditX;
    TEdit *StepEditY;
    TLabel *Label2;
    TLabel *Label3;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall AddressEditChange(TObject *Sender);
    void __fastcall ScanButtonClick(TObject *Sender);
    void __fastcall ScanButtonAvgClick(TObject *Sender);
    void __fastcall IdUDPServer1UDPRead(TObject *Sender, TStream *AData,
          TIdSocketHandle *ABinding);
    void __fastcall ApplyAddressButtonClick(TObject *Sender);
    void __fastcall UpSpeedButtonClick(TObject *Sender);
    void __fastcall DownSpeedButtonClick(TObject *Sender);
    void __fastcall RightSpeedButtonClick(TObject *Sender);
    void __fastcall LeftSpeedButtonClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
    bool avg;
    int mode;
    bool stop;
    unsigned recieve;
    int stepX;
    int stepY;
public:		// User declarations
    int posX; // текущие координаты по отчёту поворотного устройства
    int posY;
    int PosX; // положение повортоного устройства в момент начала сканирования
    int PosY;

    __fastcall TScatterForm(TComponent* Owner);
    __fastcall ~TScatterForm();
    void __fastcall UpdatePosition();
    void __fastcall StartSensing();
};
//---------------------------------------------------------------------------
extern PACKAGE TScatterForm *ScatterForm;
//---------------------------------------------------------------------------
#endif
