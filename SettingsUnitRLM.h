//---------------------------------------------------------------------------

#ifndef SettingsUnitRLMH
#define SettingsUnitRLMH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ScktComp.hpp>

//---------------------------------------------------------------------------

#define RxPtrArrSize (4096)
#define BufferSize (1024*1024*40+100)
#define BufferNumber (4)

class TSettingsUnitForm : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox1;
        TLabeledEdit *leCarFreqStart;
        TLabeledEdit *leAmpCh1;
        TLabeledEdit *leCarFreqFin;
        TLabeledEdit *leAmpCh2;
        TGroupBox *GroupBox2;
        TLabeledEdit *leRotFreqFin;
        TLabeledEdit *leRotFreqStart;
        TLabeledEdit *leRotPhaseCh1;
        TLabeledEdit *leRotPeriod;
        TLabeledEdit *leRotPhaseCh2;
        TGroupBox *GroupBox3;
        TLabeledEdit *lePulseWidth;
        TLabeledEdit *lePulsePeriod;
        TLabeledEdit *lePacketPeriod;
        TGroupBox *GroupBox4;
        TComboBox *ComboBox1;
        TGroupBox *GroupBox5;
        TLabeledEdit *leAdcSamples;
        TLabeledEdit *lePulseNumber;
        TCheckBox *CSaveToFile;
        TLabeledEdit *lePacketNumber;
        TLabeledEdit *leAdcWriteDelay;
        TGroupBox *GroupBox6;
        TCheckBox *CDac1Rst;
        TCheckBox *CDac2Rst;
        TCheckBox *CAdc1Rst;
        TCheckBox *CAdc2Rst;
        TCheckBox *CRefresh;
        TTabSheet *Net;
        TLabeledEdit *EIpAdr;
        TLabeledEdit *EPort;
        TTabSheet *TabSheet3;
        TGroupBox *GTransmitter;
        TLabeledEdit *LTx_X1V;
        TLabeledEdit *LTx_X1H;
        TLabeledEdit *LTx_X2V;
        TLabeledEdit *LTx_X2H;
        TCheckBox *CTx_X1V;
        TCheckBox *CTx_X2V;
        TCheckBox *CTx_X1H;
        TCheckBox *CTx_X2H;
        TLabeledEdit *LTx_PL1V;
        TLabeledEdit *LTx_PL2V;
        TLabeledEdit *LTx_PL1H;
        TLabeledEdit *LTx_PL2H;
        TGroupBox *GReceiver;
        TLabeledEdit *LRx_X1V;
        TLabeledEdit *LRx_X2V;
        TLabeledEdit *LRx_X1H;
        TLabeledEdit *LRx_X2H;
        TCheckBox *CRx_X1V;
        TCheckBox *CRx_X2V;
        TCheckBox *CRx_X1H;
        TCheckBox *CRx_X2H;
        TLabeledEdit *LRx_PL1V;
        TLabeledEdit *LRx_PL2V;
        TLabeledEdit *LRx_PL1H;
        TLabeledEdit *LRx_PL2H;
        TGroupBox *GGeterodin;
        TLabeledEdit *LPh_Tx_X1V;
        TLabeledEdit *LPh_Tx_X2V;
        TLabeledEdit *LPh_Tx_X1H;
        TLabeledEdit *LPh_Tx_X2H;
        TLabeledEdit *LPh_Rx_X1V;
        TLabeledEdit *LPh_Rx_X2V;
        TLabeledEdit *LPh_Rx_X1H;
        TLabeledEdit *LPh_Rx_X2H;
        TCheckBox *CTxGeterodinOff;
        TLabeledEdit *LGeterodinFreq;
        TButton *MCon;
        TButton *MDisCon;
        TButton *btnCMD1;
        TButton *btnCMD3;
        TButton *btnCMD5;
        TButton *btnCMD7;
        TButton *btnCMD9;
        TTimer *checkView;
        TClientSocket *ClientSocket1;
        TCheckBox *CTx_PL1V;
        TCheckBox *CTx_PL2V;
        TCheckBox *CTx_PL1H;
        TCheckBox *CTx_PL2H;
        TCheckBox *CRx_PL1V;
        TCheckBox *CRx_PL2V;
        TCheckBox *CRx_PL1H;
        TCheckBox *CRx_PL2H;
        TLabeledEdit *leUComTmp;
        TRadioGroup *RGBand;
        TRadioGroup *RGBand2;
        TRadioGroup *RGSync;
        TBitBtn *BitBtn1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall btnCMD1Click(TObject *Sender);
        void __fastcall btnCMD3Click(TObject *Sender);
        void __fastcall btnCMD5Click(TObject *Sender);
        void __fastcall btnCMD7Click(TObject *Sender);
        void __fastcall btnCMD9Click(TObject *Sender);
        void __fastcall MConClick(TObject *Sender);
        void __fastcall MDisConClick(TObject *Sender);
        void __fastcall checkViewTimer(TObject *Sender);
        void __fastcall ClientSocket1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ClientSocket1Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall ClientSocket1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ClientSocket1Read(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall lePacketNumberChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSettingsUnitForm(TComponent* Owner);
        void __fastcall SendCmdPkt(unsigned short Bytes, unsigned char *Buffer, unsigned short CmdNum);
        unsigned int __fastcall PhaseToCode(double PhaseGrad);
        int __fastcall TimeToCode(double TimeUs);
        int __fastcall FreqToCode(double FreqMHz);
        unsigned int PacketNum;
        void __fastcall cmdCall1();
        void __fastcall cmdCall2();
        void __fastcall cmdCall4();
        void __fastcall cmdCall6();
        void __fastcall cmdCall8();
        void __fastcall cmdCall10();
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsUnitForm *SettingsUnitForm;
//---------------------------------------------------------------------------
struct TPtrArr
{
        char * ptr;
        int len;
};
#endif
