//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH

//---------------------------------------------------------------------------
#include "CSPIN.h"
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <Menus.hpp>
#include <ScktComp.hpp>
#include <StdCtrls.hpp>

#define MainbufferSizeDouble (65536*2)
//---------------------------------------------------------------------------
class TformMain : public TForm
{
__published:	// IDE-managed Components
   TMemo *Memo;
   TMainMenu *MainMenu1;
   TMenuItem *mmFile;
        TMenuItem *mmMainSettings;
   TMenuItem *mmShowOsc;
   TMenuItem *mmShowSp;
   TMenuItem *mmTile;
   TStatusBar *sbMainStatusBar;
   TPanel *Panel1;
   TMenuItem *N1;
   TMenuItem *N2;
   TMenuItem *N3;
   TMenuItem *N4;
   TMenuItem *N5;
   TMenuItem *N6;
   TButton *Button1;
   TButton *Button2;
   TTimer *TimerARP;
   TTimer *TimerAck;
   TButton *Button3;
   TButton *bSendParam;
   TButton *bPlot;
   TButton *bVarPlot;
   TGroupBox *GroupBox2;
   TLabeledEdit *leNumberOfMultOsc;
   TLabeledEdit *leMulOscDelay;
   TLabeledEdit *leExpName;
   TMenuItem *N7;
   TButton *Button5;
   TSpeedButton *SpeedButton1;
   TButton *Button7;
   TButton *Button8;
   TButton *bAvg;
   TButton *bAvgVarPlot;
   TButton *btOriginalPulse;
   TMenuItem *mmShowExp;
   TMenuItem *mmRasterShow;
   TLabeledEdit *leRasterPeriod;
   TPanel *Panel2;
   TRadioButton *rbRasterAF;
   TRadioButton *rbRasterA;
   TRadioButton *rbRasterF;
   TActionManager *ActionManager1;
   TAction *Action1;
        TMenuItem *Threshold1;
        TMenuItem *RasterSetting1;
        TOpenDialog *OpenDialog1;
        TTimer *FileReadTimer;
        TMenuItem *ReadFile1;
        TLabeledEdit *leScanSector;
        TMenuItem *Window1;
        TMenuItem *MeasureSettings1;
        TMenuItem *Settings1;
        TGroupBox *GroupBox1;
        TLabeledEdit *AddressEdit;
        TLabeledEdit *PortEdit;
        TButton *ApplyButton;
        TIdUDPClient *IdUDPClient1;
        TRadioGroup *RadioGroup1;
        TIdUDPServer *IdUDPServer1;
        TLabeledEdit *LabeledEdit1;
        TButton *Button4;
        TCheckBox *AutoCheckBox;
        TButton *Button6;
        TLabel *Label1;
        TIdUDPClient *IdUDPClient2;
        TTimer *PlotTimer;
        TMenuItem *N8;
        TMenuItem *N9;
        TMenuItem *N10;
        TMenuItem *N11;
        TMenuItem *N12;
        TGroupBox *GroupBox3;
        TTrackBar *MinUpDown;
        TTrackBar *MaxUpDown;
        TLabel *Label2;
        TLabel *Label3;
        TCSpinEdit *MinUpDownO;
        TCSpinEdit *MaxUpDownO;
        TEdit *MinUpDownM;
        TEdit *MaxUpDownM;
        TGroupBox *GroupBox4;
        TEdit *videoServer;
        TLabel *Label4;
        TIdUDPClient *IdUDPClient3;
        TLabel *Label5;
        TCSpinEdit *UpDownDelta;
    TMenuItem *ScatterMenu;
        TCheckBox *chWriteView;
        TLabeledEdit *LabeledY;
        TButton *Button9;
        TMenuItem *mmChanels;
        TMenuItem *N13;
        TMenuItem *N14;
        TLabel *Label6;
        TCSpinEdit *LeftRightDelta;
        TMenuItem *N15;
        TButton *MCon;
        TButton *SendSettings;
        TButton *btnCMD9;
   void __fastcall N2Click(TObject *Sender);
   void __fastcall N3Click(TObject *Sender);
   void __fastcall mmShowOscClick(TObject *Sender);
   void __fastcall mmShowSpClick(TObject *Sender);
   void __fastcall mmTileClick(TObject *Sender);
   void __fastcall FormActivate(TObject *Sender);
   void __fastcall mmSettingsClick(TObject *Sender);
   void __fastcall N4Click(TObject *Sender);
   void __fastcall N5Click(TObject *Sender);
   void __fastcall Button1Click(TObject *Sender);
   void __fastcall N6Click(TObject *Sender);
   void __fastcall Button2Click(TObject *Sender);
   void __fastcall TimerARPTimer(TObject *Sender);
   void __fastcall Button3Click(TObject *Sender);
   void __fastcall bSendParamClick(TObject *Sender);
   void __fastcall bPlotClick(TObject *Sender);
   void __fastcall bVarPlotClick(TObject *Sender);
   void __fastcall N7Click(TObject *Sender);
   void __fastcall Button5Click(TObject *Sender);
   void __fastcall SpeedButton1Click(TObject *Sender);
   void __fastcall bPRClick(TObject *Sender);
   void __fastcall Button7Click(TObject *Sender);
   void __fastcall Button8Click(TObject *Sender);
   void __fastcall TimerAckTimer(TObject *Sender);
   void __fastcall bAvgClick(TObject *Sender);
   void __fastcall bAvgVarPlotClick(TObject *Sender);
   void __fastcall bAvgPRClick(TObject *Sender);
   void __fastcall btOriginalPulseClick(TObject *Sender);
   void __fastcall mmShowExpClick(TObject *Sender);
   void __fastcall mmRasterShowClick(TObject *Sender);
   void __fastcall Action1Execute(TObject *Sender);
        void __fastcall Threshold1Click(TObject *Sender);
        void __fastcall RasterSetting1Click(TObject *Sender);
        void __fastcall ReadFile1Click(TObject *Sender);
        void __fastcall FileReadTimerTimer(TObject *Sender);
        void __fastcall FileReadTimerProc();
        void __fastcall MeasureSettings1Click(TObject *Sender);
        void __fastcall ApplyButtonClick(TObject *Sender);
        void __fastcall IdUDPServer1UDPRead(TObject *Sender,
          TStream *AData, TIdSocketHandle *ABinding);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall PlotTimerTimer(TObject *Sender);
        void __fastcall N8Click(TObject *Sender);
        void __fastcall N10Click(TObject *Sender);
        void __fastcall N11Click(TObject *Sender);
        void __fastcall N12Click(TObject *Sender);
        void __fastcall MinUpDownChange(TObject *Sender);
        void __fastcall MaxUpDownChange(TObject *Sender);
    void __fastcall ScatterMenuClick(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall mmChanelsClick(TObject *Sender);
        void __fastcall N14Click(TObject *Sender);
        void __fastcall N15Click(TObject *Sender);
        void __fastcall btnCMD9Click(TObject *Sender);
        void __fastcall MConClick(TObject *Sender);
        void __fastcall SendSettingsClick(TObject *Sender);
        void __fastcall leNumberOfMultOscChange(TObject *Sender);
private:	// User declarations
   double *IndicatorXX; // данные для хранения сектора
   double *IndicatorYY; // данные для хранения сектора
   void __fastcall PlotDrawParam(int type, double *DataBuf, int VarBufSize);
   void __fastcall ControlAndMeasure(int BufSize, int CurBufNum); // контроль порога и измерение координат
   int FileCounter;
   bool __fastcall InitFileRead(AnsiString FileName, bool Message);
   AnsiString CoordFileName;
   double CurrentAngle;
   double PlatformAngle;
   int mmFileReadTimer;
   int mmPlotTimer;
   int FileReadTimerInterval;
   bool videoServerStatus;
   bool __fastcall PrepareScan(double Sector, double Time);
   void __fastcall StartScan();
   void __fastcall SetPosition(double Angle, double Speed);

public:		// User declarations
   __fastcall TformMain(TComponent* Owner);
   __fastcall ~TformMain();
   int MaxUpDownPosition;
   int MinUpDownPosition;
   void __fastcall OnRecieve(unsigned &Size, char *Buffer);
   void __fastcall ProcessAnswer(short int *Buffer, unsigned int CmdNum);
   void __fastcall MathAndPlot(int BufSize, int CurBufNum);
   void __fastcall RestartTimer(TTimer *Timer);
   void __fastcall SendData(int Size, void *Buffer);
   void __fastcall PrintPacket(unsigned Size, unsigned char *Buffer);
   void __fastcall Log(AnsiString &msg);
   void __fastcall cleanView();

   double ResXXRe[MainbufferSizeDouble], ResXXIm[MainbufferSizeDouble];
   double ResYYRe[MainbufferSizeDouble], ResYYIm[MainbufferSizeDouble];
   double ResXXAbs[MainbufferSizeDouble], ResXXAng[MainbufferSizeDouble];
   double ResYYAbs[MainbufferSizeDouble], ResYYAng[MainbufferSizeDouble];
   double ResXXPhase[MainbufferSizeDouble], ResYYPhase[MainbufferSizeDouble];


};
   void CALLBACK TimerProc(unsigned int uID, unsigned int uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
   void CALLBACK mmPlotTimerProc(unsigned int uID, unsigned int uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
   void CALLBACK mmTimerPulseClick(unsigned int uID, unsigned int uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
//---------------------------------------------------------------------------
extern PACKAGE TformMain *formMain;
//---------------------------------------------------------------------------
#endif


