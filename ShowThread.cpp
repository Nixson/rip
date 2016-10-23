//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
#include "ShowThread.h"
#include "SettingsUnitRLM.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ShowThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
//extern unsigned long BytesReceived;
//extern unsigned long BytesProcessed;

#define ADC_SAMPLES_NUMBER		(13)

extern __int64 BytesReceived;
extern __int64 BytesProcessed;
extern short int *CmdBuffer;
extern int CmdBufNum;
extern int CmdBufNumPosition;
//extern TSpectrumFixFigure *OscView;
extern unsigned char *RxBuf;
extern int TxBufSize;
int OscLen = BufferSize;
int OscNumber = 1;

//short int Chan1[BufferNumber][BufferSize];
//short int Chan2[BufferNumber][BufferSize];

extern short int * Chan1[BufferNumber];
extern short int * Chan2[BufferNumber];

extern short int * GeoChan;
extern int IndicatorIdx;
extern int LastIndicatorIdx;

unsigned int CmdSizeST=0;
unsigned int CmdByteST=0;
extern unsigned char *CmdPtr;

const unsigned int PtrArr = 256*256;
//const unsigned int RxPtrArrSize = 256;
extern struct TPtrArr RxPtrArr[RxPtrArrSize];
extern int RxPtrArrWrIdx;
extern int RxPtrArrRdIdx;

int CmdAcked =0;
int CmdPS = 0;

extern int SaveToFileEnable;
extern int GeoFileEnable;
extern int RadarRefreshEnable;
extern int IndicatorEnable;

__fastcall TShowThread::TShowThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TShowThread::Execute()
{
    //CmdPtr = new unsigned char[50000000];

    /*for (int i = 0; i< BufferNumber; i++)
    {
        Chan1[i] = new short int[BufferSize] ;
        Chan2[i] = new short int[BufferSize] ;
    }  */

    int tCur;
    int tLast=0;
    int CmdCounter =0;

    while(!Terminated)
    {
         GetCmd();

         if (CmdAcked == 1 && RadarRefreshEnable == 1)
         {
                CmdCounter++;
                CmdAcked = 0;
                //SettingsUnitForm->BCmd2Click(this);
         }

         tCur = Idglobal::GetTickCount();
         if(tCur >= tLast + 1000)
         {
                CmdPS =CmdCounter;
                CmdCounter =0;
                tLast =  tCur;
                Synchronize(UpdateIndicator);
         }

         Idglobal::Sleep(1);
    }

    //delete [] CmdPtr;
    /*for (int i = 0; i< BufferNumber; i++)
    {
        delete [] Chan1[i];
        delete [] Chan2[i];
    }  */

}
//---------------------------------------------------------------------------
void __fastcall TShowThread::GetCmd()
{
        unsigned char RxWord;
        unsigned int CmdType;

        while (RxPtrArrRdIdx != RxPtrArrWrIdx)
        {
                for (int i=0; i < RxPtrArr[RxPtrArrRdIdx].len; i++)
                {
                        //RxWord = RxPtrArr[RxPtrArrRdIdx].ptr[i];

                        //if (CmdByteST!=0) CmdPtr[CmdByte] = RxWord;

                        CmdPtr[CmdByteST] = RxPtrArr[RxPtrArrRdIdx].ptr[i];

                        switch (CmdByteST)
                        {
                        case 0:
                                if (CmdPtr[CmdByteST] == 0xab)
                                {
                                        ++CmdByteST;
                                }
                                break;
                        case 1:
                                if (CmdPtr[CmdByteST] == 0xcd) ++CmdByteST;
                                else CmdByteST =0; break;
                        case 2:
                                if (CmdPtr[CmdByteST] == 0xdc) ++CmdByteST;
                                else CmdByteST =0; break;
                        case 3:
                                if (CmdPtr[CmdByteST] == 0xba) ++CmdByteST;
                                else CmdByteST =0; break;
                        case 11:
                                CmdSizeST = (CmdPtr[8]) | ((CmdPtr[9]) << 8) | ((CmdPtr[10]) << 16) | ((CmdPtr[11]) << 24);
                                ++CmdByteST;
                                break;
                        default:
                                ++CmdByteST;
                                if (CmdByteST == CmdSizeST)
                                {
                                        CmdType = (CmdPtr[6]) | (CmdPtr[7] << 8);

                                        //Synchronize(CommandDone);
                                        CmdByteST = 0;
                                        switch (CmdType)
                                        {
                                                case 0x0001: Cmd1(CmdPtr,CmdSizeST); break;
                                                case 0x0002: Cmd2(CmdPtr,CmdSizeST); break;
                                                case 0x0006: Cmd6(CmdPtr,CmdSizeST); break;
                                                case 0x0008: Cmd8(CmdPtr,CmdSizeST); break;
                                                case 0x000A: Cmd10(CmdPtr,CmdSizeST); break;
                                                case 0x0004: Cmd4(CmdPtr,CmdSizeST); break;
                                                default: break;
                                        }


                                }

                                break;
                        }

                }

                RxPtrArrRdIdx = (RxPtrArrRdIdx + 1) % RxPtrArrSize;
        }
};

void __fastcall TShowThread::sync1(){
        SettingsUnitForm->cmdCall1();
}
void __fastcall TShowThread::sync2(){
        SettingsUnitForm->cmdCall2();
}
void __fastcall TShowThread::sync6(){
        SettingsUnitForm->cmdCall6();
}
void __fastcall TShowThread::sync8(){
        SettingsUnitForm->cmdCall8();
}
void __fastcall TShowThread::sync10(){
        SettingsUnitForm->cmdCall10();
}
void __fastcall TShowThread::sync4(){
        SettingsUnitForm->cmdCall4();
}
//---------------------------------------------------------------------------
void __fastcall TShowThread::CommandDone()
{
      //mainForm->Memo1->Lines->Add("Команда выполнена!");
};
//---------------------------------------------------------------------------
void __fastcall TShowThread::UpdateIndicator()
{
               //mainForm->StatusBar1->Panels->Items[0]->Text = "CmdPS = " +IntToStr(CmdPS);
               // mainForm->StatusBar1->Panels->Items[2]->Text = IntToStr(BytesProcessed);
};
//---------------------------------------------------------------------------
void __fastcall TShowThread::Cmd1(char * buf, unsigned int Bytes)
{
       /* OscLen = (Bytes - 12)/4;
        unsigned char *DataPtr = buf+12;
        int j=0;

        for (int i = 0; i< OscLen; i++)
        {
                Chan1[i] = DataPtr[j] | (DataPtr[j+1]<<8);
                Chan2[i] = DataPtr[j+OscLen*2] | (DataPtr[j+1+OscLen*2]<<8);
                if (Chan1[i] > 2048) Chan1[i]= Chan1[i]-4096;
                if (Chan2[i] > 2048) Chan2[i]= Chan2[i]-4096;
                j=j+2;
        }
        Synchronize(UpdateIndicator);

        SaveRadarDataToFile(buf,Bytes);*/
}
//---------------------------------------------------------------------------
void __fastcall TShowThread::Cmd2(char * buf, unsigned int Bytes)
{
        OscLen = (*(((int*) buf) + 13));
        OscNumber = (*(((int*) buf) + 15));
        /*if (OscNumber > 4)
        {
                OscNumber = 4;
                Synchronize(Only4OscLen);
        }  */
        unsigned int *DataPtr = (unsigned int *)(buf+112);
        int Ch2OffsetWord32 = OscLen * OscNumber/8*3 + 28;
        int idx=0;

        for (int OscIdx = 0; OscIdx < OscNumber; OscIdx++)
        {
                int j=0;
                for (int i = 0; i< OscLen ; i+=8)
                {
                        idx = OscLen*OscIdx+i;

                        Chan1[IndicatorIdx][idx+4] =  DataPtr[j] & 0xFFF;
                        Chan1[IndicatorIdx][idx+5] = (DataPtr[j] >> 12) & 0xFFF;
                        Chan1[IndicatorIdx][idx+6] = ((DataPtr[j] >> 24) & 0xFF) | ((DataPtr[j+1] & 0xF) << 8);
                        Chan1[IndicatorIdx][idx+7] = ((DataPtr[j+1] >> 4) & 0xFFF);
                        Chan1[IndicatorIdx][idx+0] = ((DataPtr[j+1] >> 16) & 0xFFF);
                        Chan1[IndicatorIdx][idx+1] = ((DataPtr[j+1] >> 28) & 0xF) | ((DataPtr[i/8+(OscLen)/4] & 0xFF) << 4);
                        Chan1[IndicatorIdx][idx+2] = ((DataPtr[i/8+(OscLen/4)] >> 8) & 0xFFF);
                        Chan1[IndicatorIdx][idx+3] = ((DataPtr[i/8+(OscLen/4)] >> 20) & 0xFFF);

                        Chan2[IndicatorIdx][idx+4] =  DataPtr[j+Ch2OffsetWord32] & 0xFFF;
                        Chan2[IndicatorIdx][idx+5] = (DataPtr[j+Ch2OffsetWord32] >> 12) & 0xFFF;
                        Chan2[IndicatorIdx][idx+6] = ((DataPtr[j+Ch2OffsetWord32] >> 24) & 0xFF) | ((DataPtr[j+1+Ch2OffsetWord32] & 0xF) << 8);
                        Chan2[IndicatorIdx][idx+7] = ((DataPtr[j+1+Ch2OffsetWord32] >> 4) & 0xFFF);
                        Chan2[IndicatorIdx][idx+0] = ((DataPtr[j+1+Ch2OffsetWord32] >> 16) & 0xFFF);
                        Chan2[IndicatorIdx][idx+1] = ((DataPtr[j+1+Ch2OffsetWord32] >> 28) & 0xF) | ((DataPtr[i/8+(OscLen)/4+Ch2OffsetWord32] & 0xFF) << 4);
                        Chan2[IndicatorIdx][idx+2] = ((DataPtr[i/8+(OscLen/4)+Ch2OffsetWord32] >> 8) & 0xFFF);
                        Chan2[IndicatorIdx][idx+3] = ((DataPtr[i/8+(OscLen/4)+Ch2OffsetWord32] >> 20) & 0xFFF);

                        j = j+ 2;
                }
                DataPtr += OscLen*3/8;
        }
        OscLen = OscLen * OscNumber;

        int j=0;
        for (int i = 0; i< OscLen ; i++)
        {
                if (Chan1[IndicatorIdx][i] > 2048) Chan1[IndicatorIdx][i]= Chan1[IndicatorIdx][i]-4096;
                if (Chan2[IndicatorIdx][i] > 2048) Chan2[IndicatorIdx][i]= Chan2[IndicatorIdx][i]-4096;

                GeoChan[j] = Chan1[IndicatorIdx][i];
                GeoChan[j+1] = Chan2[IndicatorIdx][i];
                j=j+2;
        }

        /*OscView->Prm->xmin = 0;
        OscView->Prm->xmax = OscLen*1.0e6;
        OscView->Prm->ymin = -512;
        OscView->Prm->ymax = 512;
        OscView->RestoreOriginalState();

        for (int i=0; i<OscLen; i++)
        {
         OscView->data[0]->Y[i] = Chan1[IndicatorIdx][i];
         OscView->data[1]->Y[i] = Chan2[IndicatorIdx][i];
        };

        OscView->DrawCS();
        OscView->Draw();*/


        if (SaveToFileEnable == 1)
        {
                SaveGeoRadarDataToFile(buf,Bytes);
                SaveRadarDataToFile(buf,Bytes);
        }
        LastIndicatorIdx = IndicatorIdx;
        IndicatorIdx = ( IndicatorIdx + 1 ) & (BufferNumber-1);
        CmdAcked = 1;
}

//---------------------------------------------------------------------------
void __fastcall TShowThread::Cmd8(char * buf, unsigned int Bytes)
{
        OscLen = (*(((int*) buf) + 13));
        OscNumber = 1;(*(((int*) buf) + 15));
        /*if (OscNumber > 4)
        {
                OscNumber = 4;
                Synchronize(Only4OscLen);
        }  */
        unsigned int *DataPtr = (unsigned int *)(buf+112);
        int Ch2OffsetWord32 = OscLen * OscNumber/8*3 + 28;
        int idx=0;

        for (int OscIdx = 0; OscIdx < OscNumber; OscIdx++)
        {
                int j=0;
                for (int i = 0; i< OscLen ; i+=8)
                {
                        idx = OscLen*OscIdx+i;

                        Chan1[IndicatorIdx][idx+4] =  DataPtr[j] & 0xFFF;
                        Chan1[IndicatorIdx][idx+5] = (DataPtr[j] >> 12) & 0xFFF;
                        Chan1[IndicatorIdx][idx+6] = ((DataPtr[j] >> 24) & 0xFF) | ((DataPtr[j+1] & 0xF) << 8);
                        Chan1[IndicatorIdx][idx+7] = ((DataPtr[j+1] >> 4) & 0xFFF);
                        Chan1[IndicatorIdx][idx+0] = ((DataPtr[j+1] >> 16) & 0xFFF);
                        Chan1[IndicatorIdx][idx+1] = ((DataPtr[j+1] >> 28) & 0xF) | ((DataPtr[i/8+(OscLen)/4] & 0xFF) << 4);
                        Chan1[IndicatorIdx][idx+2] = ((DataPtr[i/8+(OscLen/4)] >> 8) & 0xFFF);
                        Chan1[IndicatorIdx][idx+3] = ((DataPtr[i/8+(OscLen/4)] >> 20) & 0xFFF);

                        Chan2[IndicatorIdx][idx+4] =  DataPtr[j+Ch2OffsetWord32] & 0xFFF;
                        Chan2[IndicatorIdx][idx+5] = (DataPtr[j+Ch2OffsetWord32] >> 12) & 0xFFF;
                        Chan2[IndicatorIdx][idx+6] = ((DataPtr[j+Ch2OffsetWord32] >> 24) & 0xFF) | ((DataPtr[j+1+Ch2OffsetWord32] & 0xF) << 8);
                        Chan2[IndicatorIdx][idx+7] = ((DataPtr[j+1+Ch2OffsetWord32] >> 4) & 0xFFF);
                        Chan2[IndicatorIdx][idx+0] = ((DataPtr[j+1+Ch2OffsetWord32] >> 16) & 0xFFF);
                        Chan2[IndicatorIdx][idx+1] = ((DataPtr[j+1+Ch2OffsetWord32] >> 28) & 0xF) | ((DataPtr[i/8+(OscLen)/4+Ch2OffsetWord32] & 0xFF) << 4);
                        Chan2[IndicatorIdx][idx+2] = ((DataPtr[i/8+(OscLen/4)+Ch2OffsetWord32] >> 8) & 0xFFF);
                        Chan2[IndicatorIdx][idx+3] = ((DataPtr[i/8+(OscLen/4)+Ch2OffsetWord32] >> 20) & 0xFFF);

                        j = j+ 2;
                }
                DataPtr += OscLen*3/8;
        }
        OscLen = OscLen * OscNumber;

        int j=0;
        for (int i = 0; i< OscLen ; i++)
        {
                if (Chan1[IndicatorIdx][i] > 2048) Chan1[IndicatorIdx][i]= Chan1[IndicatorIdx][i]-4096;
                if (Chan2[IndicatorIdx][i] > 2048) Chan2[IndicatorIdx][i]= Chan2[IndicatorIdx][i]-4096;

                GeoChan[j] = Chan1[IndicatorIdx][i];
                GeoChan[j+1] = Chan2[IndicatorIdx][i];
                j=j+2;
        }

        /*OscView->Prm->xmin = 0;
        OscView->Prm->xmax = OscLen*1.0e6;
        OscView->Prm->ymin = -512;
        OscView->Prm->ymax = 512;
        OscView->RestoreOriginalState();

        for (int i=0; i<OscLen; i++)
        {
         OscView->data[0]->Y[i] = Chan1[IndicatorIdx][i];
         OscView->data[1]->Y[i] = Chan2[IndicatorIdx][i];
        };

        OscView->DrawCS();
        OscView->Draw();*/


        if (SaveToFileEnable == 1)
        {
                SaveGeoRadarDataToFile(buf,Bytes);
                SaveRadarDataToFile(buf,Bytes);
        }
        LastIndicatorIdx = IndicatorIdx;
        IndicatorIdx = ( IndicatorIdx + 1 ) & (BufferNumber-1);
        CmdAcked = 1;
}

//---------------------------------------------------------------------------
void __fastcall TShowThread::Cmd10(char * buf, unsigned int Bytes)
{
        OscLen = (*(((int*) buf) + 13));
        OscNumber = 23;//(*(((int*) buf) + 15));
        /*if (OscNumber > 4)
        {
                OscNumber = 4;
                Synchronize(Only4OscLen);
        }  */
        unsigned int *DataPtr = (unsigned int *)(buf+112);
        int Ch2OffsetWord32 = OscLen * OscNumber/8*3 + 28;
        int idx=0;

        for (int OscIdx = 0; OscIdx < OscNumber; OscIdx++)
        {
                int j=0;
                for (int i = 0; i< OscLen ; i+=8)
                {
                        idx = OscLen*OscIdx+i;

                        Chan1[IndicatorIdx][idx+4] =  DataPtr[j] & 0xFFF;
                        Chan1[IndicatorIdx][idx+5] = (DataPtr[j] >> 12) & 0xFFF;
                        Chan1[IndicatorIdx][idx+6] = ((DataPtr[j] >> 24) & 0xFF) | ((DataPtr[j+1] & 0xF) << 8);
                        Chan1[IndicatorIdx][idx+7] = ((DataPtr[j+1] >> 4) & 0xFFF);
                        Chan1[IndicatorIdx][idx+0] = ((DataPtr[j+1] >> 16) & 0xFFF);
                        Chan1[IndicatorIdx][idx+1] = ((DataPtr[j+1] >> 28) & 0xF) | ((DataPtr[i/8+(OscLen)/4] & 0xFF) << 4);
                        Chan1[IndicatorIdx][idx+2] = ((DataPtr[i/8+(OscLen/4)] >> 8) & 0xFFF);
                        Chan1[IndicatorIdx][idx+3] = ((DataPtr[i/8+(OscLen/4)] >> 20) & 0xFFF);

                        Chan2[IndicatorIdx][idx+4] =  DataPtr[j+Ch2OffsetWord32] & 0xFFF;
                        Chan2[IndicatorIdx][idx+5] = (DataPtr[j+Ch2OffsetWord32] >> 12) & 0xFFF;
                        Chan2[IndicatorIdx][idx+6] = ((DataPtr[j+Ch2OffsetWord32] >> 24) & 0xFF) | ((DataPtr[j+1+Ch2OffsetWord32] & 0xF) << 8);
                        Chan2[IndicatorIdx][idx+7] = ((DataPtr[j+1+Ch2OffsetWord32] >> 4) & 0xFFF);
                        Chan2[IndicatorIdx][idx+0] = ((DataPtr[j+1+Ch2OffsetWord32] >> 16) & 0xFFF);
                        Chan2[IndicatorIdx][idx+1] = ((DataPtr[j+1+Ch2OffsetWord32] >> 28) & 0xF) | ((DataPtr[i/8+(OscLen)/4+Ch2OffsetWord32] & 0xFF) << 4);
                        Chan2[IndicatorIdx][idx+2] = ((DataPtr[i/8+(OscLen/4)+Ch2OffsetWord32] >> 8) & 0xFFF);
                        Chan2[IndicatorIdx][idx+3] = ((DataPtr[i/8+(OscLen/4)+Ch2OffsetWord32] >> 20) & 0xFFF);

                        j = j+ 2;
                }
                DataPtr += OscLen*3/8;
        }


        int j=0;
        for (int i = 0; i< OscLen ; i++)
        {
                if (Chan1[IndicatorIdx][i] > 2048) Chan1[IndicatorIdx][i]= Chan1[IndicatorIdx][i]-4096;
                if (Chan2[IndicatorIdx][i] > 2048) Chan2[IndicatorIdx][i]= Chan2[IndicatorIdx][i]-4096;

                GeoChan[j] = Chan1[IndicatorIdx][i];
                GeoChan[j+1] = Chan2[IndicatorIdx][i];
                j=j+2;
        }
        memcpy(CmdBuffer+CmdBufNum*(2*OscLen*OscNumber),Chan1[IndicatorIdx],OscLen*OscNumber*2);
        memcpy(CmdBuffer+CmdBufNum*(2*OscLen*OscNumber)+OscLen*OscNumber,Chan2[IndicatorIdx],OscLen*OscNumber*2);
        ++CmdBufNumPosition;

        //OscLen = OscLen * OscNumber;
        /*OscView->Prm->xmin = 0;
        OscView->Prm->xmax = OscLen*1.0e6;
        OscView->Prm->ymin = -512;
        OscView->Prm->ymax = 512;
        OscView->RestoreOriginalState();

        for (int i=0; i<OscLen; i++)
        {
         OscView->data[0]->Y[i] = Chan1[IndicatorIdx][i];
         OscView->data[1]->Y[i] = Chan2[IndicatorIdx][i];
        };

        OscView->DrawCS();
        OscView->Draw();
        */


        if (SaveToFileEnable == 1)
        {
                SaveGeoRadarDataToFile(buf,Bytes);
                SaveRadarDataToFile(buf,Bytes);
        }
        LastIndicatorIdx = IndicatorIdx;
        IndicatorIdx = ( IndicatorIdx + 1 ) & (BufferNumber-1);
        CmdAcked = 1;
}
//------------------------------------------------------------
void __fastcall TShowThread::Cmd6(char * buf, unsigned int Bytes)
{
        OscLen = (*(((int*) buf) + 13));
        OscNumber = (*(((int*) buf) + 15))  *16;
        /*if (OscNumber > 4)
        {
                OscNumber = 4;
                Synchronize(Only4OscLen);
        }  */
        unsigned int *DataPtr = (unsigned int *)(buf+112);
        int Ch2OffsetWord32 = OscLen * OscNumber/8*3 + 28;
        int idx=0;

        for (int OscIdx = 0; OscIdx < OscNumber; OscIdx++)
        {
                int j=0;
                for (int i = 0; i< OscLen ; i+=8)
                {
                        idx = OscLen*OscIdx+i;

                        Chan1[IndicatorIdx][idx+4] =  DataPtr[j] & 0xFFF;
                        Chan1[IndicatorIdx][idx+5] = (DataPtr[j] >> 12) & 0xFFF;
                        Chan1[IndicatorIdx][idx+6] = ((DataPtr[j] >> 24) & 0xFF) | ((DataPtr[j+1] & 0xF) << 8);
                        Chan1[IndicatorIdx][idx+7] = ((DataPtr[j+1] >> 4) & 0xFFF);
                        Chan1[IndicatorIdx][idx+0] = ((DataPtr[j+1] >> 16) & 0xFFF);
                        Chan1[IndicatorIdx][idx+1] = ((DataPtr[j+1] >> 28) & 0xF) | ((DataPtr[i/8+(OscLen)/4] & 0xFF) << 4);
                        Chan1[IndicatorIdx][idx+2] = ((DataPtr[i/8+(OscLen/4)] >> 8) & 0xFFF);
                        Chan1[IndicatorIdx][idx+3] = ((DataPtr[i/8+(OscLen/4)] >> 20) & 0xFFF);

                        Chan2[IndicatorIdx][idx+4] =  DataPtr[j+Ch2OffsetWord32] & 0xFFF;
                        Chan2[IndicatorIdx][idx+5] = (DataPtr[j+Ch2OffsetWord32] >> 12) & 0xFFF;
                        Chan2[IndicatorIdx][idx+6] = ((DataPtr[j+Ch2OffsetWord32] >> 24) & 0xFF) | ((DataPtr[j+1+Ch2OffsetWord32] & 0xF) << 8);
                        Chan2[IndicatorIdx][idx+7] = ((DataPtr[j+1+Ch2OffsetWord32] >> 4) & 0xFFF);
                        Chan2[IndicatorIdx][idx+0] = ((DataPtr[j+1+Ch2OffsetWord32] >> 16) & 0xFFF);
                        Chan2[IndicatorIdx][idx+1] = ((DataPtr[j+1+Ch2OffsetWord32] >> 28) & 0xF) | ((DataPtr[i/8+(OscLen)/4+Ch2OffsetWord32] & 0xFF) << 4);
                        Chan2[IndicatorIdx][idx+2] = ((DataPtr[i/8+(OscLen/4)+Ch2OffsetWord32] >> 8) & 0xFFF);
                        Chan2[IndicatorIdx][idx+3] = ((DataPtr[i/8+(OscLen/4)+Ch2OffsetWord32] >> 20) & 0xFFF);

                        j = j+ 2;
                }
                DataPtr += OscLen*3/8;
        }
        OscLen = OscLen * OscNumber;

        int j=0;
        for (int i = 0; i< OscLen ; i++)
        {
                if (Chan1[IndicatorIdx][i] > 2048) Chan1[IndicatorIdx][i]= Chan1[IndicatorIdx][i]-4096;
                if (Chan2[IndicatorIdx][i] > 2048) Chan2[IndicatorIdx][i]= Chan2[IndicatorIdx][i]-4096;

                GeoChan[j] = Chan1[IndicatorIdx][i];
                GeoChan[j+1] = Chan2[IndicatorIdx][i];
                j=j+2;
        }

        /*OscView->Prm->xmin = 0;
        OscView->Prm->xmax =  OscLen*1.0e6;;
        OscView->Prm->ymin = -512;
        OscView->Prm->ymax = 512;
        OscView->RestoreOriginalState();

        for (int i=0; i<OscLen; i++)
        {
         OscView->data[0]->Y[i] = Chan1[IndicatorIdx][i];
         OscView->data[1]->Y[i] = Chan2[IndicatorIdx][i];
        };*/

        if (SaveToFileEnable == 1)
        {
                //SaveGeoRadarDataToFile(buf,Bytes);
                SaveRadarDataToFile(buf,Bytes);
        }
        LastIndicatorIdx = IndicatorIdx;
        IndicatorIdx = ( IndicatorIdx + 1 ) & (BufferNumber-1);
        CmdAcked = 1;
}
//---------------------------------------------------------------------------
void __fastcall TShowThread::Only4OscLen()
{
        //mainForm->Memo1->Lines->Add("На индикаторе отображаются только первые 4 импульса.");
}
//---------------------------------------------------------------------------
void __fastcall TShowThread::Cmd4(char * buf, unsigned int Bytes)
{
        //mainForm->BSendArrClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TShowThread::SaveRadarDataToFile(char * buf, unsigned int Bytes)
{
        TFileStream * File;
        AnsiString DirName = "data/"+DateToStr(Date());
        CreateDir(DirName);
        AnsiString FileName = DirName+"/radar_" + DateToStr(Date()) + "_" + TimeToStr(Time())+ ".bin";
        File = new TFileStream(FileName, fmCreate);
        File->Write(buf,112);
        File->Write(Chan1[IndicatorIdx],OscLen*2);
        File->Write(Chan2[IndicatorIdx],OscLen*2);
        delete File;
}
//---------------------------------------------------------------------------
void __fastcall TShowThread::SaveGeoRadarDataToFile(unsigned char * buf, unsigned int Bytes)
{
        //Частота несущей (МГЦ)
        unsigned int CarrierFreqI = buf[16] | (buf[17] << 8) | (buf[18] << 16) | (buf[19] << 24);
        float CarrierFreq = float(CarrierFreqI)/4294967296*2000+8750;

        //Начальная частота ротации (МГЦ)
        unsigned int RotatorFreqStartI = buf[20] | (buf[21] << 8) | (buf[22] << 16) | (buf[23] << 24);
        float RotatorFreqStart = float(RotatorFreqStartI)/4294967296*2000;

        //Длительность импульса(мкс)
        unsigned int PulseWidthI = buf[24] | (buf[25] << 8) | (buf[26] << 16);
        float PulseWidth = ((float)(PulseWidthI))*4/1000;

        //Инкремент частоты ротации(МГц)
        unsigned int RotatorFreqIncI = buf[40] | (buf[41] << 8) | (buf[42] << 16) | (buf[43] << 24);
        float RotatorFreqInc = float(RotatorFreqIncI)/4294967296*2000;

        //Конечная частота ротации
        float RotatorFreqFinish = RotatorFreqInc * PulseWidthI +RotatorFreqStart;

        //Количество выборок АЦП
        unsigned int AdcSamplesI = (buf[52] | (buf[53] << 8) | (buf[54] << 16) | (buf[55] << 24)) ;

        //Количество импульсов
        unsigned int PulseNumber = (buf[56] | (buf[57] << 8) | (buf[58] << 16) | (buf[59] << 24)) ;

        //Количество пачек
        unsigned int PacketNumber = (buf[60] | (buf[61] << 8) | (buf[62] << 16) | (buf[63] << 24)) ;

        //Длина записи импульса
        float AdcSamplesTime = float(AdcSamplesI)/1000;

        unsigned char *DataPtr = new char[Bytes*2];
        unsigned int offset = 0;

        *((unsigned int*)(DataPtr+offset)) =  0xAABBCCDD;
        offset+=sizeof(unsigned int);
        *((float*)(DataPtr+offset)) =  CarrierFreq;
        offset+=sizeof(float);
        *((float*)(DataPtr+offset)) =  RotatorFreqStart;
        offset+=sizeof(float);
        *((float*)(DataPtr+offset)) =  RotatorFreqFinish;
        offset+=sizeof(float);
        *((float*)(DataPtr+offset)) =  PulseWidth;
        offset+=sizeof(float);
        *((float*)(DataPtr+offset)) =  AdcSamplesTime;
        offset+=sizeof(float);
        *((int*)(DataPtr+offset)) =  PacketNumber;
        offset+=sizeof(int);
        *((float*)(DataPtr+offset)) =  0;
        offset+=sizeof(float);
        *((float*)(DataPtr+offset)) =  0;
        offset+=sizeof(float);
        *((int*)(DataPtr+offset)) =  AdcSamplesI*PacketNumber;
        for (int i=0; i< 22; i++)
        {
                *((int*)(DataPtr+offset)) =  0;
                offset+=sizeof(int);
        }
        memcpy((DataPtr+offset),GeoChan,AdcSamplesI*PacketNumber*4);
        offset+=AdcSamplesI*PacketNumber*4;

        TFileStream * File;
        AnsiString FileName = "georadar_data/georadar_" + DateToStr(Date()) + "_" + TimeToStr(Time())+ ".dat";
        File = new TFileStream(FileName, fmCreate);
        File->Write(DataPtr,offset);
        delete File;
        delete [] DataPtr;
}
