//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
#include "SettingsUnitRLM.h"
#include "ShowThread.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSettingsUnitForm *SettingsUnitForm;

TShowThread * ShowThread;
//TIndicatorThread * IndicatorThread;
//TSpectrumFixFigure *OscView;

int SpLenMax = 1024*1024*16;
int Td = 1;

extern unsigned int WorkerCmdNum;
extern int lePacketNumberMain;

short int *CmdBuffer = NULL;

unsigned int TmpCnt =0;
int CmdBufNum = 0;
int CmdBufNumPosition = 0;
int CallType = 0;
const double DAC_FREQ= 2000000000;
unsigned char *RxBuf;
unsigned char *TxBuf;
unsigned int TxBufSize = 32 * 1048576;
//unsigned long BytesReceived;
//unsigned long BytesProcessed;

__int64 BytesReceived;
__int64 BytesProcessed;

#define CONTINIOUS_MODE_MASK 1
#define PULSE_MODE_MASK 2
#define COHERENT_POLAR_MODE_MASK 4
#define SYNC_EN_MASK 8
#define DAC1_RST_MASK 16
#define DAC2_RST_MASK 32
#define ADC1_RST_MASK 64
#define ADC2_RST_MASK 128

//Ключи узла коммутации (маски)
#define SW_LPF_BPF (1)
#define SW_X_RES (1 << 1)
#define SW_P_L   (1 << 2)
#define SW_Rx1V  (1 << 3)
#define SW_Rx2V  (1 << 5)
#define SW_Rx1H  (1 << 6)
#define SW_Rx2H  (1 << 4)
#define SW_Tx1V  (1 << 7)
#define SW_Tx2V  (1 << 8)
#define SW_Tx1H  (1 << 10)
#define SW_Tx2H  (1 << 9)
#define SW_VSK   (1 << 31)

//const unsigned int PtrArr = 256*1024;
//const unsigned int RxPtrArrSize = 256;
struct TPtrArr RxPtrArr[RxPtrArrSize];

short int * Chan1[BufferNumber];
short int * Chan2[BufferNumber];
short int * GeoChan;
short int * TmpChan1;
short int * TmpChan2;
unsigned char *CmdPtr;

TFileStream * FileSave;


int  RxPtrArrWrIdx = 0;
int  RxPtrArrRdIdx = 0;
int IndicatorIdx=0;
int LastIndicatorIdx=0;
int SaveToFileEnable=0;
int GeoFileEnable = 0;
int RadarRefreshEnable=0;
int IndicatorEnable=0;
//---------------------------------------------------------------------------
__fastcall TSettingsUnitForm::TSettingsUnitForm(TComponent* Owner)
        : TForm(Owner)
{
        //DecimalSeparator = ',';
        TimeSeparator = '_';
        DateSeparator = '-';
        PacketNum = 0;

        CmdPtr = new unsigned char[50000000];

        TmpChan1 = new short int [BufferSize];
        TmpChan2 = new short int [BufferSize];
        

        GeoChan  = new short int [2*BufferSize];

        for (int i = 0; i< BufferNumber; i++)
        {
                Chan1[i] = new short int[BufferSize] ;
                Chan2[i] = new short int[BufferSize] ;
        }

        //OscView = NULL;

}

void __fastcall TSettingsUnitForm::cmdCall1(){}
void __fastcall TSettingsUnitForm::cmdCall2(){}
void __fastcall TSettingsUnitForm::cmdCall4(){}
void __fastcall TSettingsUnitForm::cmdCall6(){}
void __fastcall TSettingsUnitForm::cmdCall8(){}
void __fastcall TSettingsUnitForm::cmdCall10(){
        int OscLen = leAdcSamples->Text.ToInt();
        short int *Buffer = new short int [1024+6];
        memset(Buffer,0,1024*2+12);
        int FileSubBufNum = 0;
        for(int i = 0; i < 32; i++){
                memcpy(Buffer, &CmdBufNum, 4);
		memcpy(Buffer+2, &i, 4);
                FileSubBufNum = 0;
                if(i==31)
        		FileSubBufNum = 1;
		memcpy(Buffer+4, &FileSubBufNum, 4);
                memcpy(Buffer+6,CmdBuffer+CmdBufNum*(32*OscLen)+OscLen*i, OscLen*2);
                formMain->ProcessAnswer(Buffer, 0x0006);
        }
        ++CmdBufNum;
        delete [] Buffer;
        if(CmdBufNum < CmdBufNumPosition)
                cmdCall10();

}


//---------------------------------------------------------------------------
void __fastcall TSettingsUnitForm::FormCreate(TObject *Sender)
{
   SetWindowPos(this->Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   if(CmdBuffer) delete [] CmdBuffer;
   CmdBuffer = new short int[(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32];
   memset(CmdBuffer,0,(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32*2);
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::BitBtn1Click(TObject *Sender)
{
        formMain->N6Click(formMain);
        unsigned short BuffSize = 100;
        unsigned char *Buffer = new unsigned char[BuffSize];
        unsigned char *DataPtr = Buffer;

        try
        {

        if (CSaveToFile->Checked == true) SaveToFileEnable = 1;
        else SaveToFileEnable = 0;
        if (CRefresh->Checked == true) RadarRefreshEnable = 1;
        else RadarRefreshEnable = 0;
        IndicatorEnable = 0;

        //Амплитуда
        double AmpCh1 = leAmpCh1->Text.ToDouble();
        double AmpCh2 = leAmpCh2->Text.ToDouble();
        *((int *)(DataPtr)) =  ((int (AmpCh1/100 * 32767)) & 0xFFFF) | (((int (AmpCh2/100 * 32767)) & 0xFFFF) << 16);
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Амп. К1 (%) = "+FloatToStr(AmpCh1));
        //Memo1->Lines->Add("Амп. К2 (%) = "+FloatToStr(AmpCh2));

        //Начальная частота несущей
        double CarFreqStart = leCarFreqStart->Text.ToDouble();
        *((int *)(DataPtr)) = FreqToCode(CarFreqStart);
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Начальная частота несущей (МГц) = "+FloatToStr(CarFreqStart));

        //Начальная частота ротации
        double RotFreqStart = leRotFreqStart->Text.ToDouble();
        *((int *)(DataPtr)) = FreqToCode(RotFreqStart);
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Начальная частота ротации (МГц) = "+FloatToStr(RotFreqStart));

        //Длительность импульса
        double PulseWidth = lePulseWidth->Text.ToDouble();
        *((int *)(DataPtr)) = TimeToCode(PulseWidth);
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Длительность импульса (мкс) = "+FloatToStr(PulseWidth));

        //Период импульса
        double PulsePeriod = lePulsePeriod->Text.ToDouble();
        *((int *)(DataPtr)) = TimeToCode(PulsePeriod);
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Период импульса (мкс) = "+FloatToStr(PulsePeriod));

        //Период ротации
        double RotPeriod = leRotPeriod->Text.ToDouble();
        *((int *)(DataPtr)) = TimeToCode(RotPeriod);
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Период ротации (мкс) = "+FloatToStr(RotPeriod));

        double CarFreqFin = leCarFreqFin->Text.ToDouble();
        //Memo1->Lines->Add("Конечная частота несущей (МГц) = "+FloatToStr(CarFreqFin));
        double RotFreqFin = leRotFreqFin->Text.ToDouble();
        //Memo1->Lines->Add("Конечная частота ротации (МГц) = "+FloatToStr(RotFreqFin));
        //Если включен режим когерентной ротации
        /*if (ConfigsForm->CCoherentRotEn->Checked)
        {
                //Инкремент несущей
                *((int *)(DataPtr)) = (FreqToCode(CarFreqFin - CarFreqStart) / TimeToCode(RotPeriod));
                DataPtr += sizeof(int);//4
                //Инкремент ротации
                *((int *)(DataPtr)) = (FreqToCode(RotFreqFin - RotFreqStart) / TimeToCode(RotPeriod));
                DataPtr += sizeof(int);//4
        }
        else */
        {
                //Инкремент несущей
                *((int *)(DataPtr)) = (FreqToCode(CarFreqFin - CarFreqStart) / (TimeToCode(PulseWidth)+1));
                DataPtr += sizeof(int);//4
                //Инкремент ротации
                *((int *)(DataPtr)) = (FreqToCode(RotFreqFin - RotFreqStart) / (TimeToCode(PulseWidth)+1));
                DataPtr += sizeof(int);//4
        }

        //Фаза ротации
        double RotPhaseCh1 = leRotPhaseCh1->Text.ToDouble();
        double RotPhaseCh2 = leRotPhaseCh2->Text.ToDouble();
        *((int *)(DataPtr)) = ((PhaseToCode(RotPhaseCh1) >> 16) & 0xffff) | (PhaseToCode(RotPhaseCh2) & 0xffff0000) ;
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Начальная фаза ротации К1 (гр.) = "+FloatToStr(RotPhaseCh1));
        //Memo1->Lines->Add("Начальная фаза ротации К2 (гр.) = "+FloatToStr(RotPhaseCh2));

        //Режимы
        int mode = 0;

        if (ComboBox1->Text == "Нулевая начальная фаза")
        {
                mode |= PULSE_MODE_MASK;
        }

        if (ComboBox1->Text == "Когерентный")
        {
                mode |= COHERENT_POLAR_MODE_MASK;
                mode |= PULSE_MODE_MASK;
        }

        if (ComboBox1->Text == "ФКМ 15")
        {
                mode |= 1 << 16;
                mode |= PULSE_MODE_MASK;
        }

        if (ComboBox1->Text == "Пачка с модуляцией периода")
        {
                mode |= 2 << 16;
                mode |= PULSE_MODE_MASK;
        }

        if (ComboBox1->Text == "ФКМ 255")
        {
                mode |= 3 << 16;
                mode |= PULSE_MODE_MASK;
        }

/*
        if (ConfigsForm->CCalibrate->Checked == true)
        {
                mode |= CALIBRATION_MASK;
                Memo1->Lines->Add("Калибровка");
        }
        else mode &= (~CALIBRATION_MASK);
        if (ConfigsForm->CPulseEn->Checked == true)
        {
                mode |= PULSE_MODE_MASK;
                Memo1->Lines->Add("Импульсный режим");
        }
        else mode = mode & (~PULSE_MODE_MASK);
        if (ConfigsForm->CCoherentRotEn->Checked == true)
        {
                mode |= COHERENT_POLAR_MODE_MASK;
                Memo1->Lines->Add("Когерентная ротация");
        }
        else mode = mode & (~SYNC_EN_MASK);
        if (ConfigsForm->CSyncEn->Checked == true)
        {
                mode |= SYNC_EN_MASK;
                Memo1->Lines->Add("Синхронизация");
        }
        else mode = mode & (~SYNC_EN_MASK);
        */

        if (CDac1Rst->Checked == true)
        {
                mode |= DAC1_RST_MASK;
                //Memo1->Lines->Add("Dac1Rst");
        }
        else mode = mode & (~DAC1_RST_MASK);
        if (CDac2Rst->Checked == true)
        {
                mode |= DAC2_RST_MASK;
                //Memo1->Lines->Add("Dac2Rst");
        }
        else mode = mode & (~DAC2_RST_MASK);
        if (CAdc1Rst->Checked == true)
        {
                mode |= ADC1_RST_MASK;
                //Memo1->Lines->Add("Adc1Rst");
        }
        else mode = mode & (~ADC1_RST_MASK);
        if (CAdc2Rst->Checked == true)
        {
                mode |= ADC2_RST_MASK;
                //Memo1->Lines->Add("Adc2Rst");
        }
        else mode = mode & (~ADC2_RST_MASK);

        *((int *)(DataPtr)) = mode;
        DataPtr += sizeof(int);//4

        //Количество отсчетов АЦП
        int AdcSamplesNumber = leAdcSamples->Text.ToInt();
        if (AdcSamplesNumber > 6000000) AdcSamplesNumber = ((AdcSamplesNumber / 6000000) + 1) * 6000000;

        *((int *)(DataPtr)) = AdcSamplesNumber;
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Количество отстчетов АЦП = "+IntToStr(AdcSamplesNumber));
        //Количество импульсов
        int PulseNumber = lePulseNumber->Text.ToInt();
        *((int *)(DataPtr)) = PulseNumber;
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Количество импульсов = "+IntToStr(PulseNumber));
        //Количество пачек
        int PacketNumber = lePacketNumber->Text.ToInt();
        *((int *)(DataPtr)) = PacketNumber;
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Количество пачек = "+IntToStr(PacketNumber));
        //Период пачки
        double PacketPeriod = lePacketPeriod->Text.ToDouble();
        *((int *)(DataPtr)) = ceil(PacketPeriod*325);// у Арма 1 мкс = 325 тактов таймера
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Задержка пачки (мкс) = "+FloatToStr(PacketPeriod));
        //Задержка строба записи данных с АЦП
        double AdcWriteDelay = leAdcWriteDelay->Text.ToDouble();
        *((int *)(DataPtr)) = TimeToCode(AdcWriteDelay);
        DataPtr += sizeof(int);//4
        //Memo1->Lines->Add("Задержка строба записи данных с АЦП (мкс) = "+FloatToStr(AdcWriteDelay));
        //Ключи узла коммутации
        unsigned int UCom = 0;
        switch (RGBand->ItemIndex)
        {
              case  0:
                        UCom = SW_VSK;
                        break;
              case  1:
                        UCom = SW_X_RES; //  Х/резерв  , ФНЧ
                        break;
              case  2:
                        UCom = SW_X_RES; //  Х/резерв
                        break;
              case  3:
                        UCom = SW_LPF_BPF | SW_X_RES /*| SW_P_L*/;
                        break;
                default: UCom = 0;
        }
        if (CTx_X1V->Checked) UCom |= SW_Tx1V;
        if (CTx_X2V->Checked) UCom |= SW_Tx2V;
        if (CTx_X1H->Checked) UCom |= SW_Tx1H;
        if (CTx_X2H->Checked) UCom |= SW_Tx2H;
        if (CRx_X1V->Checked) UCom |= SW_Rx1V;
        if (CRx_X2V->Checked) UCom |= SW_Rx2V;
        if (CRx_X1H->Checked) UCom |= SW_Rx1H;
        if (CRx_X2H->Checked) UCom |= SW_Rx2H;
        *((int *)(DataPtr)) = UCom;
        DataPtr += sizeof(int);//4
        //Значения аттенюаторов передатчика узла коммутации X диапазона
        unsigned int AttXTx = LTx_X1V->Text.ToInt() |
                             (LTx_X2V->Text.ToInt() << 8) |
                             (LTx_X1H->Text.ToInt() << 24) |   //! Исправил, было не правильно!
                             (LTx_X2H->Text.ToInt() << 16) ;   //!
        *((int *)(DataPtr)) = AttXTx;
        DataPtr += sizeof(int);//4
        //Значения аттенюаторов передатчика узла коммутации PL диапазона
        unsigned int AttPLTx = LTx_PL1V->Text.ToInt() |
                              (LTx_PL2V->Text.ToInt() << 8) |
                              (LTx_PL1H->Text.ToInt() << 16) |
                              (LTx_PL2H->Text.ToInt() << 24) ;
        *((int *)(DataPtr)) = AttPLTx;
        DataPtr += sizeof(int);//4
        //Значения аттенюаторов приемника узла коммутации X диапазона
        unsigned int AttXRx = LRx_X1V->Text.ToInt() |
                            (LRx_X2V->Text.ToInt() << 16) |
                            (LRx_X1H->Text.ToInt() << 24) |     //! Исправил, было не правильно!
                            (LRx_X2H->Text.ToInt() << 8) ;     //!
        *((int *)(DataPtr)) = AttXRx;
        DataPtr += sizeof(int);//4
        //Значения аттенюаторов приемника узла коммутации PL диапазона
        unsigned int AttPLRx = LRx_PL1V->Text.ToInt() |
                            (LRx_PL2V->Text.ToInt() << 8) |
                            (LRx_PL1H->Text.ToInt() << 16) |
                            (LRx_PL2H->Text.ToInt() << 24) ;
        *((int *)(DataPtr)) = AttPLRx;
        DataPtr += sizeof(int);//4
        //Регистр частоты и управления гетеродином
        unsigned int GeterodinFreq = LGeterodinFreq->Text.ToInt()/20;
        if (CTxGeterodinOff->Checked) GeterodinFreq |= (1 << 31);
        *((int *)(DataPtr)) = GeterodinFreq;//((GeterodinFreq & 0xFF) << 8) | ((GeterodinFreq >> 8) & 0xFF) ;
        DataPtr += sizeof(int);//4
        //Регистр управления фазовращателями №1
        unsigned int PH1 =   LPh_Tx_X1V->Text.ToInt() << 8  |
                            (LPh_Tx_X2V->Text.ToInt() << 24) |
                            (LPh_Tx_X1H->Text.ToInt())       |
                            (LPh_Tx_X2H->Text.ToInt() << 16) ;
        *((int *)(DataPtr)) = PH1;
        DataPtr += sizeof(int);//4
        //Регистр управления фазовращателями №2
        unsigned int PH2 =   LPh_Rx_X1V->Text.ToInt() << 16  |
                            (LPh_Rx_X2V->Text.ToInt())       |
                            (LPh_Rx_X1H->Text.ToInt() << 24) |
                            (LPh_Rx_X2H->Text.ToInt() << 8) ;
        *((int *)(DataPtr)) = PH2;
        DataPtr += sizeof(int);//4
        SendCmdPkt(BuffSize,Buffer,0x0001);
        }
        catch(...)
        {
                ShowMessage("Данные введены не корректно!");
        }
        delete [] Buffer;

}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::btnCMD1Click(TObject *Sender)
{
        if(CmdBuffer) delete [] CmdBuffer;
        CmdBuffer = new short int[(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32];
        memset(CmdBuffer,0,(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32*2);
        CmdBufNum = 0;
        CmdBufNumPosition = 0;
        CallType = 1;
        unsigned short BuffSize = 4;
        unsigned char *Buffer = new unsigned char[BuffSize];
        unsigned char *DataPtr = Buffer;
        SendCmdPkt(BuffSize,Buffer,0x0002);
        delete [] Buffer;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::btnCMD3Click(TObject *Sender)
{
        if(CmdBuffer) delete [] CmdBuffer;
        CmdBuffer = new short int[(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32];
        memset(CmdBuffer,0,(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32*2);
        CmdBufNum = 0;
        CmdBufNumPosition = 0;
        CallType = 3;
        unsigned short BuffSize = 4;
        unsigned char *Buffer = new unsigned char[BuffSize];
        unsigned char *DataPtr = Buffer;

        SendCmdPkt(BuffSize,Buffer,0x0003);
        delete [] Buffer;
        
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::btnCMD5Click(TObject *Sender)
{
        if(CmdBuffer) delete [] CmdBuffer;
        CmdBuffer = new short int[(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32];
        memset(CmdBuffer,0,(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32*2);
        CmdBufNum = 0;
        CmdBufNumPosition = 0;
        CallType = 5;
        unsigned short BuffSize = 4;
        unsigned char *Buffer = new unsigned char[BuffSize];
        unsigned char *DataPtr = Buffer;
        SendCmdPkt(BuffSize,Buffer,0x0005);
        delete [] Buffer;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::btnCMD7Click(TObject *Sender)
{
        if(CmdBuffer) delete [] CmdBuffer;
        CmdBuffer = new short int[(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32];
        memset(CmdBuffer,0,(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32*2);
        CmdBufNum = 0;
        CmdBufNumPosition = 0;
        CallType = 7;
        unsigned short BuffSize = 4;
        unsigned char *Buffer = new unsigned char[BuffSize];
        unsigned char *DataPtr = Buffer;
        SendCmdPkt(BuffSize,Buffer,0x0007);
        delete [] Buffer;
        
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::btnCMD9Click(TObject *Sender)
{
        if(CmdBuffer) delete [] CmdBuffer;
        CmdBuffer = new short int[(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32];
        memset(CmdBuffer,0,(leAdcSamples->Text.ToInt()+56)*lePacketNumber->Text.ToInt()*32*2);
        CmdBufNum = 0;
        CmdBufNumPosition = 0;
        CallType = 9;
        unsigned short BuffSize = 4;
        unsigned char *Buffer = new unsigned char[BuffSize];
        unsigned char *DataPtr = Buffer;
        SendCmdPkt(BuffSize,Buffer,0x0009);
        delete [] Buffer;
}
//---------------------------------------------------------------------------
void __fastcall TSettingsUnitForm::SendCmdPkt(unsigned short Bytes, unsigned char *Buffer, unsigned short CmdNum)
{
        PacketNum++;
        unsigned int PktSize = Bytes + 12;
        unsigned char *Pkt = new unsigned char[PktSize];
        unsigned char *DataPtr = Pkt;

        DataPtr[0] = 0xab;
        DataPtr[1] = 0xcd;
        DataPtr[2] = 0xdc;
        DataPtr[3] = 0xba;
        DataPtr[4] = CmdNum & 0xFF;
        DataPtr[5] = (CmdNum >> 8) & 0xFF;
        DataPtr[6] = CmdNum & 0xFF;
        DataPtr[7] = (CmdNum >> 8) & 0xFF;
        DataPtr[8] = *((unsigned char *) &PktSize);
        DataPtr[9] = *((unsigned char *) &PktSize+1);
        DataPtr[10] = *((unsigned char *) &PktSize+2);
        DataPtr[11] = *((unsigned char *) &PktSize+3);

        DataPtr += 12;
        memcpy(DataPtr, Buffer, Bytes);
        formMain->cleanView();
        ClientSocket1->Socket->SendBuf(Pkt,PktSize);

        delete [] Pkt;

        return;
}
//---------------------------------------------------------------------------
int __fastcall TSettingsUnitForm::FreqToCode(double FreqMHz)
{
        // Функция пересчитывает частоту в МГц в код частоты
        int FreqCode = (int) (FreqMHz * 1000000 / DAC_FREQ * 4294967296);
        return FreqCode;
}
//---------------------------------------------------------------------------
int __fastcall TSettingsUnitForm::TimeToCode(double TimeUs)
{
        // Функция пересчитывает время в мкс в количество тактов ЦАП
        int TimeCode = (int) (TimeUs / 1000000 * DAC_FREQ /8);
        return TimeCode;
}
//---------------------------------------------------------------------------
unsigned int __fastcall TSettingsUnitForm::PhaseToCode(double PhaseGrad)
{
        // Функция пересчитывает фазу в град в код фазы
        unsigned int PhaseCode = (unsigned int) (PhaseGrad / 360 * 4294967296);
        return PhaseCode;
}




void __fastcall TSettingsUnitForm::MConClick(TObject *Sender)
{
        ClientSocket1->Address = EIpAdr->Text;
        ClientSocket1->Port = EPort->Text.ToInt();
        ClientSocket1->Active = true;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::MDisConClick(TObject *Sender)
{
        ClientSocket1->Active = false;        
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::checkViewTimer(TObject *Sender)
{
        if(CmdBufNumPosition > CmdBufNum) {
                if(CallType > 0){
                        switch (CallType){
                                case 1:  cmdCall2(); break;
                                case 3:  cmdCall4(); break;
                                case 5:  cmdCall6(); break;
                                case 7:  cmdCall8(); break;
                                case 9:  cmdCall10(); break;
                        }
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::ClientSocket1Connect(TObject *Sender,
      TCustomWinSocket *Socket)
{
        MCon->Enabled = false;
        MDisCon->Enabled = true;

        //Подключение установлено => выделяем память

        RxBuf = new unsigned char[TxBufSize];
        TxBuf = new unsigned char[TxBufSize];
        BytesReceived = 0;
        BytesProcessed = 0;
        ShowThread = new TShowThread(false);
        //IndicatorThread = new TIndicatorThread(false);

        for (int i=0; i< RxPtrArrSize; i++)
        {
                RxPtrArr[i].ptr = new char[8192];
                RxPtrArr[i].len = 0;
        }
        RxPtrArrWrIdx = 0;
        RxPtrArrRdIdx = 0;        
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::ClientSocket1Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
        ClientSocket1->Active = false;
        ErrorCode = 0;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::ClientSocket1Disconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
        MCon->Enabled = true;
        MDisCon->Enabled = false;

        //Подключение разорвано => освобождаем память
        //delete IndicatorThread;
        delete ShowThread;
        delete [] RxBuf;
        delete [] TxBuf;

        for (int i=0; i< RxPtrArrSize; i++)
        {
                delete [] RxPtrArr[i].ptr;
                RxPtrArr[i].len = 0;
        }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::ClientSocket1Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
        RxPtrArr[RxPtrArrWrIdx].len = ClientSocket1->Socket->ReceiveLength();
        ClientSocket1->Socket->ReceiveBuf(RxPtrArr[RxPtrArrWrIdx].ptr, RxPtrArr[RxPtrArrWrIdx].len);
        RxPtrArrWrIdx = (RxPtrArrWrIdx + 1) % RxPtrArrSize;        
}
//---------------------------------------------------------------------------

void __fastcall TSettingsUnitForm::lePacketNumberChange(TObject *Sender)
{
        if(lePacketNumber->Text != formMain->leNumberOfMultOsc->Text)
                lePacketNumber->Text = formMain->leNumberOfMultOsc->Text;
        lePacketNumberMain = lePacketNumber->Text.ToInt()+128;
}
//---------------------------------------------------------------------------

