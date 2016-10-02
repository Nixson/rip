//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
#include "SettingsUnitRLM.h"
#include "SettingsUnit.h"
#include <IniFiles.hpp>
//#include "CommandUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TformSettings *formSettings;

unsigned int MaxSubBufNum = 1;

//---------------------------------------------------------------------------
__fastcall TformSettings::TformSettings(TComponent* Owner)
   : TForm(Owner)
{
   
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::FormCreate(TObject *Sender)
{
   SetWindowPos(this->Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

   if (!cbPulseMod->Checked) cbLFM->Enabled = false;

   BitBtn1->Caption = "Отправить параметры и \r\n принять импульс";

   LoadConfig("ExitState.ini");
   formMain->IdUDPServer1->Active = true; // включение UDP сервера
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::FormDestroy(TObject *Sender)
{
   SaveConfig("ExitState.ini");
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::cbPulseModClick(TObject *Sender)
{
   if (!formSettings->cbPulseMod->Checked)
   {
      formSettings->cbLFM->Checked = false;
      formSettings->cbLFM->Enabled = false;
   }
   else formSettings->cbLFM->Enabled = true;

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton1UpClick(TObject *Sender)
{
   formSettings->leFreq->Text = FloatToStr(formSettings->leFreq->Text.ToDouble() + 50);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton1DownClick(TObject *Sender)
{
   formSettings->leFreq->Text = FloatToStr(formSettings->leFreq->Text.ToDouble() - 50);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton2DownClick(TObject *Sender)
{
   if(formSettings->leAmp->Text.ToDouble()>0)
   formSettings->leAmp->Text = FloatToStr(formSettings->leAmp->Text.ToDouble() - 1);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton2UpClick(TObject *Sender)
{
   if(formSettings->leAmp->Text.ToDouble()<100)
   formSettings->leAmp->Text = FloatToStr(formSettings->leAmp->Text.ToDouble() + 1);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton5DownClick(TObject *Sender)
{
   formSettings->leFreqRange->Text = FloatToStr(formSettings->leFreqRange->Text.ToDouble() - 1);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton3DownClick(TObject *Sender)
{
   if(   formSettings->lePeriod->Text.ToDouble()>0.0066 &
         formSettings->lePeriod->Text.ToDouble()>formSettings->leDuration->Text.ToDouble()
      )
   formSettings->lePeriod->Text = FloatToStr(formSettings->lePeriod->Text.ToDouble() - 0.0033);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton3UpClick(TObject *Sender)
{
   if(formSettings->lePeriod->Text.ToDouble()<100)
   formSettings->lePeriod->Text = FloatToStr(formSettings->lePeriod->Text.ToDouble() + 0.0033);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton4DownClick(TObject *Sender)
{
   if(formSettings->leDuration->Text.ToDouble()>0.0066)
      formSettings->leDuration->Text = FloatToStr(formSettings->leDuration->Text.ToDouble() - 0.0033);

   if(!(cbGate->Checked))
      leGateDuration->Text = leDuration->Text;
       
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton4UpClick(TObject *Sender)
{
   if(   formSettings->leDuration->Text.ToDouble()<100 &
         formSettings->leDuration->Text.ToDouble()<formSettings->lePeriod->Text.ToDouble()
     ) formSettings->leDuration->Text = FloatToStr(formSettings->leDuration->Text.ToDouble() + 0.0033);

   if(!(cbGate->Checked))
      leGateDuration->Text = leDuration->Text;
      
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton5UpClick(TObject *Sender)
{
   formSettings->leFreqRange->Text = FloatToStr(formSettings->leFreqRange->Text.ToDouble() + 1);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton6DownClick(TObject *Sender)
{
   if(formSettings->leBurstLen->Text.ToDouble()>0)
   formSettings->leBurstLen->Text = FloatToStr(formSettings->leBurstLen->Text.ToDouble() - 1);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton6UpClick(TObject *Sender)
{
   if(formSettings->leBurstLen->Text.ToDouble()<65535)
   formSettings->leBurstLen->Text = FloatToStr(formSettings->leBurstLen->Text.ToDouble() + 1);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::sbTxAttUpClick(TObject *Sender)
{
   if((formSettings->leTxAtt->Text.ToDouble()<63))
   formSettings->leTxAtt->Text = FloatToStr(formSettings->leTxAtt->Text.ToDouble() + 0.5);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::sbTxAttDownClick(TObject *Sender)
{
   if((formSettings->leTxAtt->Text.ToDouble()>0))
   formSettings->leTxAtt->Text = FloatToStr(formSettings->leTxAtt->Text.ToDouble() - 0.5);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::sbRxAttUpClick(TObject *Sender)
{
   if((formSettings->leRxAtt->Text.ToDouble()<63))
   formSettings->leRxAtt->Text = FloatToStr(formSettings->leRxAtt->Text.ToDouble() + 0.5);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::sbRxAttDownClick(TObject *Sender)
{
   if((formSettings->leRxAtt->Text.ToDouble()>0))
   formSettings->leRxAtt->Text = FloatToStr(formSettings->leRxAtt->Text.ToDouble() - 0.5);

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbTxPolXXClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbTxPolXYClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbTxPolYXClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbTxPolYYClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbRxPolXXClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbRxPolXYClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbRxPolYXClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbRxPolYYClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbRxAnt0Click(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::rbRxAnt1Click(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::cbContClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::BitBtn1Click(TObject *Sender)
{
   MaxSubBufNum = leSubBufNum->Text.ToInt();

   formMain->bSendParamClick(this);

   formMain->bPlotClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::cbLFMClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::cbUWBClick(TObject *Sender)
{
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::cbGateClick(TObject *Sender)
{
   if(cbGate->Checked)
   {
      leGateDuration->Enabled = true;
   }else
   {
      leGateDuration->Enabled = false;
   }
   
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TformSettings::SaveConfig(AnsiString FileName)
{
   AnsiString Path;
   Path = ExtractFilePath(Application->ExeName);

   TIniFile *ini = new TIniFile(Path+FileName);

   ini->WriteString("LabeledEdit Text", "leGeterodin", formSettings->leGeterodin->Text);
   ini->WriteString("LabeledEdit Text", "leFreq", formSettings->leFreq->Text);
   ini->WriteString("LabeledEdit Text", "leAmp", formSettings->leAmp->Text);
   ini->WriteString("LabeledEdit Text", "lePeriod", formSettings->lePeriod->Text);
   ini->WriteString("LabeledEdit Text", "leDuration", formSettings->leDuration->Text);
   ini->WriteString("LabeledEdit Text", "leFreqRange", formSettings->leFreqRange->Text);
   ini->WriteString("LabeledEdit Text", "leGateDelay", formSettings->leGateDelay->Text);
   ini->WriteString("LabeledEdit Text", "leGateDuration", formSettings->leGateDuration->Text);
   ini->WriteString("LabeledEdit Text", "leBurstLen", formSettings->leBurstLen->Text);
   ini->WriteString("LabeledEdit Text", "leTxAtt", formSettings->leTxAtt->Text);
   ini->WriteString("LabeledEdit Text", "leRxAtt", formSettings->leRxAtt->Text);
   ini->WriteString("LabeledEdit Text", "leSubBufNum", formSettings->leSubBufNum->Text);
   ini->WriteString("LabeledEdit Text", "leNumberOfMultOsc", formMain->leNumberOfMultOsc->Text);
   ini->WriteString("LabeledEdit Text", "leMulOscDelay", formMain->leMulOscDelay->Text);
   ini->WriteString("LabeledEdit Text", "leScanSector", formMain->leScanSector->Text);
   ini->WriteString("LabeledEdit Text", "AddressEdit", formMain->IdUDPClient1->Host);
   ini->WriteString("LabeledEdit Text", "videoServer", formMain->videoServer->Text);
   ini->WriteInteger("LabeledEdit Text","UpDownDelta",formMain->UpDownDelta->Value);
   ini->WriteString("LabeledEdit Text", "PortEdit", IntToStr(formMain->IdUDPClient1->Port));

   ini->WriteBool("CheckBox Position", "cbPulseMod", formSettings->cbPulseMod->Checked);
   ini->WriteBool("CheckBox Position", "cbLFM", formSettings->cbLFM->Checked);
   ini->WriteBool("CheckBox Position", "cbGate", formSettings->cbGate->Checked);
   ini->WriteBool("CheckBox Position", "cbUWB", formSettings->cbUWB->Checked);
   ini->WriteBool("CheckBox Position", "cbCont", formSettings->cbCont->Checked);
   ini->WriteBool("CheckBox Position", "cbDDSReset", formSettings->cbDDSReset->Checked);

   ini->WriteBool("CheckBox Position", "rbTxPolXX", formSettings->rbTxPolXX->Checked);
   ini->WriteBool("CheckBox Position", "rbTxPolXY", formSettings->rbTxPolXY->Checked);
   ini->WriteBool("CheckBox Position", "rbTxPolYX", formSettings->rbTxPolYX->Checked);
   ini->WriteBool("CheckBox Position", "rbTxPolYY", formSettings->rbTxPolYY->Checked);
   ini->WriteBool("CheckBox Position", "rbRxPolXX", formSettings->rbRxPolXX->Checked);
   ini->WriteBool("CheckBox Position", "rbRxPolXY", formSettings->rbRxPolXY->Checked);
   ini->WriteBool("CheckBox Position", "rbRxPolYX", formSettings->rbRxPolYX->Checked);
   ini->WriteBool("CheckBox Position", "rbRxPolYY", formSettings->rbRxPolYY->Checked);

   ini->WriteBool("CheckBox Position", "rbRxAnt0", formSettings->rbRxAnt0->Checked);
   ini->WriteBool("CheckBox Position", "rbRxAnt1", formSettings->rbRxAnt1->Checked);

   ini->WriteString("LabeledEdit Text", "SU_leAmpCh1", SettingsUnitForm->leAmpCh1->Text);
   ini->WriteString("LabeledEdit Text", "SU_leAmpCh2", SettingsUnitForm->leAmpCh2->Text);
   ini->WriteString("LabeledEdit Text", "SU_leCarFreqStart", SettingsUnitForm->leCarFreqStart->Text);
   ini->WriteString("LabeledEdit Text", "SU_leCarFreqFin", SettingsUnitForm->leCarFreqFin->Text);
   ini->WriteString("LabeledEdit Text", "SU_lePulseWidth", SettingsUnitForm->lePulseWidth->Text);
   ini->WriteString("LabeledEdit Text", "SU_lePulsePeriod", SettingsUnitForm->lePulsePeriod->Text);
   ini->WriteString("LabeledEdit Text", "SU_lePacketPeriod", SettingsUnitForm->lePacketPeriod->Text);
   ini->WriteString("LabeledEdit Text", "SU_leRotPhaseCh1", SettingsUnitForm->leRotPhaseCh1->Text);
   ini->WriteString("LabeledEdit Text", "SU_leRotPhaseCh2", SettingsUnitForm->leRotPhaseCh2->Text);
   ini->WriteString("LabeledEdit Text", "SU_leRotFreqStart", SettingsUnitForm->leRotFreqStart->Text);
   ini->WriteString("LabeledEdit Text", "SU_leRotFreqFin", SettingsUnitForm->leRotFreqFin->Text);
   ini->WriteString("LabeledEdit Text", "SU_leRotPeriod", SettingsUnitForm->leRotPeriod->Text);
   ini->WriteString("LabeledEdit Text", "SU_leAdcSamples", SettingsUnitForm->leAdcSamples->Text);
   ini->WriteString("LabeledEdit Text", "SU_lePulseNumber", SettingsUnitForm->lePulseNumber->Text);
   ini->WriteString("LabeledEdit Text", "SU_lePacketNumber", SettingsUnitForm->lePacketNumber->Text);
   ini->WriteString("LabeledEdit Text", "SU_leAdcWriteDelay", SettingsUnitForm->leAdcWriteDelay->Text);
   ini->WriteString("LabeledEdit Text", "SU_EIpAdr", SettingsUnitForm->EIpAdr->Text);
   ini->WriteString("LabeledEdit Text", "SU_EPort", SettingsUnitForm->EPort->Text);
   ini->WriteString("LabeledEdit Text", "SU_LTx_X1V", SettingsUnitForm->LTx_X1V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LTx_X2V", SettingsUnitForm->LTx_X2V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LTx_X1H", SettingsUnitForm->LTx_X1H->Text);
   ini->WriteString("LabeledEdit Text", "SU_LTx_X2H", SettingsUnitForm->LTx_X2H->Text);
   ini->WriteString("LabeledEdit Text", "SU_LTx_PL1V", SettingsUnitForm->LTx_PL1V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LTx_PL2V", SettingsUnitForm->LTx_PL2V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LTx_PL1H", SettingsUnitForm->LTx_PL1H->Text);
   ini->WriteString("LabeledEdit Text", "SU_LTx_PL2H", SettingsUnitForm->LTx_PL2H->Text);

   ini->WriteString("LabeledEdit Text", "SU_LRx_X1V", SettingsUnitForm->LRx_X1V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LRx_X2V", SettingsUnitForm->LRx_X2V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LRx_X1H", SettingsUnitForm->LRx_X1H->Text);
   ini->WriteString("LabeledEdit Text", "SU_LRx_X2H", SettingsUnitForm->LRx_X2H->Text);
   ini->WriteString("LabeledEdit Text", "SU_LRx_PL1V", SettingsUnitForm->LRx_PL1V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LRx_PL2V", SettingsUnitForm->LRx_PL2V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LRx_PL1H", SettingsUnitForm->LRx_PL1H->Text);
   ini->WriteString("LabeledEdit Text", "SU_LRx_PL2H", SettingsUnitForm->LRx_PL2H->Text);
   ini->WriteString("LabeledEdit Text", "SU_LGeterodinFreq", SettingsUnitForm->LGeterodinFreq->Text);
   ini->WriteString("LabeledEdit Text", "SU_LPh_Tx_X1V", SettingsUnitForm->LPh_Tx_X1V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LPh_Tx_X2V", SettingsUnitForm->LPh_Tx_X2V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LPh_Tx_X1H", SettingsUnitForm->LPh_Tx_X1H->Text);
   ini->WriteString("LabeledEdit Text", "SU_LPh_Tx_X2H", SettingsUnitForm->LPh_Tx_X2H->Text);

   ini->WriteString("LabeledEdit Text", "SU_LPh_Rx_X1V", SettingsUnitForm->LPh_Rx_X1V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LPh_Rx_X2V", SettingsUnitForm->LPh_Rx_X2V->Text);
   ini->WriteString("LabeledEdit Text", "SU_LPh_Rx_X1H", SettingsUnitForm->LPh_Rx_X1H->Text);
   ini->WriteString("LabeledEdit Text", "SU_LPh_Rx_X2H", SettingsUnitForm->LPh_Rx_X2H->Text);
   ini->WriteString("LabeledEdit Text", "SU_ComboBox1", SettingsUnitForm->ComboBox1->Text);
   ini->WriteString("LabeledEdit Text", "SU_RGBand", IntToStr(SettingsUnitForm->RGBand->ItemIndex));

   ini->WriteBool("CheckBox Position", "SU_CRefresh", SettingsUnitForm->CRefresh->Checked);
   ini->WriteBool("CheckBox Position", "SU_CSaveToFile", SettingsUnitForm->CSaveToFile->Checked);
   ini->WriteBool("CheckBox Position", "SU_CDac1Rst", SettingsUnitForm->CDac1Rst->Checked);
   ini->WriteBool("CheckBox Position", "SU_CDac2Rst", SettingsUnitForm->CDac2Rst->Checked);
   ini->WriteBool("CheckBox Position", "SU_CAdc1Rst", SettingsUnitForm->CAdc1Rst->Checked);
   ini->WriteBool("CheckBox Position", "SU_CAdc2Rst", SettingsUnitForm->CAdc2Rst->Checked);
   ini->WriteBool("CheckBox Position", "SU_CTx_X1V", SettingsUnitForm->CTx_X1V->Checked);
   ini->WriteBool("CheckBox Position", "SU_CTx_X2V", SettingsUnitForm->CTx_X2V->Checked);
   ini->WriteBool("CheckBox Position", "SU_CTx_X1H", SettingsUnitForm->CTx_X1H->Checked);
   ini->WriteBool("CheckBox Position", "SU_CTx_X2H", SettingsUnitForm->CTx_X2H->Checked);
   ini->WriteBool("CheckBox Position", "SU_CRx_X1V", SettingsUnitForm->CRx_X1V->Checked);
   ini->WriteBool("CheckBox Position", "SU_CRx_X2V", SettingsUnitForm->CRx_X2V->Checked);
   ini->WriteBool("CheckBox Position", "SU_CRx_X1H", SettingsUnitForm->CRx_X1H->Checked);
   ini->WriteBool("CheckBox Position", "SU_CRx_X2H", SettingsUnitForm->CRx_X2H->Checked);
   ini->WriteBool("CheckBox Position", "SU_CTxGeterodinOff", SettingsUnitForm->CTxGeterodinOff->Checked);

   delete ini;
}

void __fastcall TformSettings::LoadConfig(AnsiString FileName)
{
   AnsiString Path;
   Path = ExtractFilePath(Application->ExeName);

   TIniFile *ini = new TIniFile(Path+FileName);

   formSettings->leGeterodin->Text = ini->ReadString("LabeledEdit Text", "leGeterodin", "8000");
   formSettings->leFreq->Text = ini->ReadString("LabeledEdit Text", "leFreq", "1777");
   formSettings->leAmp->Text = ini->ReadString("LabeledEdit Text", "leAmp", "100");
   formSettings->lePeriod->Text = ini->ReadString("LabeledEdit Text", "lePeriod", "1");
   formSettings->leDuration->Text = ini->ReadString("LabeledEdit Text", "leDuration", "0.0033");
   formSettings->leFreqRange->Text = ini->ReadString("LabeledEdit Text", "leFreqRange", "500");
   formSettings->leGateDelay->Text = ini->ReadString("LabeledEdit Text", "leGateDelay", "0");
   formSettings->leGateDuration->Text = ini->ReadString("LabeledEdit Text", "leGateDuration", "0.0033");
   formSettings->leBurstLen->Text = ini->ReadString("LabeledEdit Text", "leBurstLen", "100");
   formSettings->leTxAtt->Text = ini->ReadString("LabeledEdit Text", "leTxAtt", "10");
   formSettings->leRxAtt->Text = ini->ReadString("LabeledEdit Text", "leRxAtt", "5");
   formSettings->leSubBufNum->Text = ini->ReadString("LabeledEdit Text", "leSubBufNum", "1");
   formMain->leNumberOfMultOsc->Text = ini->ReadString("LabeledEdit Text", "leNumberOfMultOsc", "750");
   formMain->leMulOscDelay->Text = ini->ReadString("LabeledEdit Text", "leMulOscDelay", "100");
   formMain->leScanSector->Text = ini->ReadString("LabeledEdit Text", "leScanSector", "90");
   formMain->IdUDPClient1->Host = ini->ReadString("LabeledEdit Text", "AddressEdit", "192.168.114.85");
   formMain->AddressEdit->Text = formMain->IdUDPClient1->Host;
   formMain->IdUDPClient1->Port = 3333;//ini->ReadInteger("LabeledEdit Text", "PortEdit", 3333);
//   formMain->IdUDPServer1->DefaultPort = formMain->IdUDPClient1->Port;
   formMain->PortEdit->Text = IntToStr(formMain->IdUDPClient1->Port);
   formMain->videoServer->Text = ini->ReadString("LabeledEdit Text","videoServer","192.168.114.10");
   formMain->UpDownDelta->Value = ini->ReadInteger("LabeledEdit Text","UpDownDelta",20);

   formSettings->cbPulseMod->Checked = ini->ReadBool("CheckBox Position", "cbPulseMod", NULL);
   formSettings->cbLFM->Checked = ini->ReadBool("CheckBox Position", "cbLFM", NULL);
   formSettings->cbGate->Checked = ini->ReadBool("CheckBox Position", "cbGate", NULL);
   formSettings->cbUWB->Checked = ini->ReadBool("CheckBox Position", "cbUWB", NULL);
   formSettings->cbCont->Checked = ini->ReadBool("CheckBox Position", "cbCont", NULL);
   formSettings->cbDDSReset->Checked = ini->ReadBool("CheckBox Position", "cbDDSReset", NULL);

   formSettings->rbTxPolXX->Checked = ini->ReadBool("CheckBox Position", "rbTxPolXX", NULL);
   formSettings->rbTxPolXY->Checked = ini->ReadBool("CheckBox Position", "rbTxPolXY", NULL);
   formSettings->rbTxPolYX->Checked = ini->ReadBool("CheckBox Position", "rbTxPolYX", NULL);
   formSettings->rbTxPolYY->Checked = ini->ReadBool("CheckBox Position", "rbTxPolYY", NULL);
   formSettings->rbRxPolXX->Checked = ini->ReadBool("CheckBox Position", "rbRxPolXX", NULL);
   formSettings->rbRxPolXY->Checked = ini->ReadBool("CheckBox Position", "rbRxPolXY", NULL);
   formSettings->rbRxPolYX->Checked = ini->ReadBool("CheckBox Position", "rbRxPolYX", NULL);
   formSettings->rbRxPolYY->Checked = ini->ReadBool("CheckBox Position", "rbRxPolYY", NULL);

   formSettings->rbRxAnt0->Checked = ini->ReadBool("CheckBox Position", "rbRxAnt0", NULL);
   formSettings->rbRxAnt1->Checked = ini->ReadBool("CheckBox Position", "rbRxAnt1", NULL);

   
   SettingsUnitForm->RGBand->ItemIndex = ini->ReadInteger("LabeledEdit Text","SU_RGBand",0);

   SettingsUnitForm->leAmpCh1->Text = ini->ReadString("LabeledEdit Text","SU_leAmpCh1","");
   SettingsUnitForm->leAmpCh2->Text = ini->ReadString("LabeledEdit Text","SU_leAmpCh2","");
   SettingsUnitForm->leCarFreqStart->Text = ini->ReadString("LabeledEdit Text","SU_leCarFreqStart","");
   SettingsUnitForm->leCarFreqFin->Text = ini->ReadString("LabeledEdit Text","SU_leCarFreqFin","");
   SettingsUnitForm->lePulseWidth->Text = ini->ReadString("LabeledEdit Text","SU_lePulseWidth","");
   SettingsUnitForm->lePulsePeriod->Text = ini->ReadString("LabeledEdit Text","SU_lePulsePeriod","");
   SettingsUnitForm->lePacketPeriod->Text = ini->ReadString("LabeledEdit Text","SU_lePacketPeriod","");
   SettingsUnitForm->leRotPhaseCh1->Text = ini->ReadString("LabeledEdit Text","SU_leRotPhaseCh1","");
   SettingsUnitForm->leRotPhaseCh2->Text = ini->ReadString("LabeledEdit Text","SU_leRotPhaseCh2","");
   SettingsUnitForm->leRotFreqStart->Text = ini->ReadString("LabeledEdit Text","SU_leRotFreqStart","");
   SettingsUnitForm->leRotFreqFin->Text = ini->ReadString("LabeledEdit Text","SU_leRotFreqFin","");
   SettingsUnitForm->leRotPeriod->Text = ini->ReadString("LabeledEdit Text","SU_leRotPeriod","");
   SettingsUnitForm->leAdcSamples->Text = ini->ReadString("LabeledEdit Text","SU_leAdcSamples","");
   SettingsUnitForm->lePulseNumber->Text = ini->ReadString("LabeledEdit Text","SU_lePulseNumber","");
   SettingsUnitForm->lePacketNumber->Text = ini->ReadString("LabeledEdit Text","SU_lePacketNumber","");
   SettingsUnitForm->leAdcWriteDelay->Text = ini->ReadString("LabeledEdit Text","SU_leAdcWriteDelay","");
   SettingsUnitForm->EIpAdr->Text = ini->ReadString("LabeledEdit Text","SU_EIpAdr","");
   SettingsUnitForm->EPort->Text = ini->ReadString("LabeledEdit Text","SU_EPort","");
   SettingsUnitForm->LTx_X1V->Text = ini->ReadString("LabeledEdit Text","SU_LTx_X1V","");
   SettingsUnitForm->LTx_X2V->Text = ini->ReadString("LabeledEdit Text","SU_LTx_X2V","");
   SettingsUnitForm->LTx_X1H->Text = ini->ReadString("LabeledEdit Text","SU_LTx_X1H","");
   SettingsUnitForm->LTx_X2H->Text = ini->ReadString("LabeledEdit Text","SU_LTx_X2H","");
   SettingsUnitForm->LTx_PL1V->Text = ini->ReadString("LabeledEdit Text","SU_LTx_PL1V","");
   SettingsUnitForm->LTx_PL2V->Text = ini->ReadString("LabeledEdit Text","SU_LTx_PL2V","");
   SettingsUnitForm->LTx_PL1H->Text = ini->ReadString("LabeledEdit Text","SU_LTx_PL1H","");
   SettingsUnitForm->LTx_PL2H->Text = ini->ReadString("LabeledEdit Text","SU_LTx_PL2H","");
   SettingsUnitForm->LRx_X1V->Text = ini->ReadString("LabeledEdit Text","SU_LRx_X1V","");
   SettingsUnitForm->LRx_X2V->Text = ini->ReadString("LabeledEdit Text","SU_LRx_X2V","");
   SettingsUnitForm->LRx_X1H->Text = ini->ReadString("LabeledEdit Text","SU_LRx_X1H","");
   SettingsUnitForm->LRx_X2H->Text = ini->ReadString("LabeledEdit Text","SU_LRx_X2H","");
   SettingsUnitForm->LRx_PL1V->Text = ini->ReadString("LabeledEdit Text","SU_LRx_PL1V","");
   SettingsUnitForm->LRx_PL2V->Text = ini->ReadString("LabeledEdit Text","SU_LRx_PL2V","");
   SettingsUnitForm->LRx_PL1H->Text = ini->ReadString("LabeledEdit Text","SU_LRx_PL1H","");
   SettingsUnitForm->LRx_PL2H->Text = ini->ReadString("LabeledEdit Text","SU_LRx_PL2H","");

   SettingsUnitForm->LGeterodinFreq->Text = ini->ReadString("LabeledEdit Text","SU_LGeterodinFreq","");
   SettingsUnitForm->LPh_Tx_X1V->Text = ini->ReadString("LabeledEdit Text","SU_LPh_Tx_X1V","");
   SettingsUnitForm->LPh_Tx_X2V->Text = ini->ReadString("LabeledEdit Text","SU_LPh_Tx_X2V","");
   SettingsUnitForm->LPh_Tx_X1H->Text = ini->ReadString("LabeledEdit Text","SU_LPh_Tx_X1H","");
   SettingsUnitForm->LPh_Tx_X2H->Text = ini->ReadString("LabeledEdit Text","SU_LPh_Tx_X2H","");
   SettingsUnitForm->LPh_Rx_X1V->Text = ini->ReadString("LabeledEdit Text","SU_LPh_Rx_X1V","");
   SettingsUnitForm->LPh_Rx_X2V->Text = ini->ReadString("LabeledEdit Text","SU_LPh_Rx_X2V","");
   SettingsUnitForm->LPh_Rx_X1H->Text = ini->ReadString("LabeledEdit Text","SU_LPh_Rx_X1H","");
   SettingsUnitForm->LPh_Rx_X2H->Text = ini->ReadString("LabeledEdit Text","SU_LPh_Rx_X2H","");
   SettingsUnitForm->ComboBox1->Text = ini->ReadString("LabeledEdit Text","SU_ComboBox1","");

   SettingsUnitForm->CRefresh->Checked = ini->ReadBool("CheckBox Position", "SU_CRefresh", NULL);
   SettingsUnitForm->CSaveToFile->Checked = ini->ReadBool("CheckBox Position", "SU_CSaveToFile", NULL);
   SettingsUnitForm->CDac1Rst->Checked = ini->ReadBool("CheckBox Position", "SU_CDac1Rst", NULL);
   SettingsUnitForm->CDac2Rst->Checked = ini->ReadBool("CheckBox Position", "SU_CDac2Rst", NULL);
   SettingsUnitForm->CAdc1Rst->Checked = ini->ReadBool("CheckBox Position", "SU_CAdc1Rst", NULL);
   SettingsUnitForm->CAdc2Rst->Checked = ini->ReadBool("CheckBox Position", "SU_CAdc2Rst", NULL);

   SettingsUnitForm->CTx_X1V->Checked = ini->ReadBool("CheckBox Position", "SU_CTx_X1V", NULL);
   SettingsUnitForm->CTx_X2V->Checked = ini->ReadBool("CheckBox Position", "SU_CTx_X2V", NULL);
   SettingsUnitForm->CTx_X1H->Checked = ini->ReadBool("CheckBox Position", "SU_CTx_X1H", NULL);
   SettingsUnitForm->CTx_X2H->Checked = ini->ReadBool("CheckBox Position", "SU_CTx_X2H", NULL);
   SettingsUnitForm->CRx_X1V->Checked = ini->ReadBool("CheckBox Position", "SU_CRx_X1V", NULL);
   SettingsUnitForm->CRx_X2V->Checked = ini->ReadBool("CheckBox Position", "SU_CRx_X2V", NULL);
   SettingsUnitForm->CRx_X1H->Checked = ini->ReadBool("CheckBox Position", "SU_CRx_X1H", NULL);
   SettingsUnitForm->CRx_X2H->Checked = ini->ReadBool("CheckBox Position", "SU_CRx_X2H", NULL);
   SettingsUnitForm->CTxGeterodinOff->Checked = ini->ReadBool("CheckBox Position", "SU_CTxGeterodinOff", NULL);

   delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TformSettings::sbMaxSubBufNumDownClick(TObject *Sender)
{
   if(formSettings->leSubBufNum->Text.ToDouble()>1)
   formSettings->leSubBufNum->Text = FloatToStr(formSettings->leSubBufNum->Text.ToInt() / 2);
}
//---------------------------------------------------------------------------

void __fastcall TformSettings::sbMaxSubBufNumUpClick(TObject *Sender)
{
   if(formSettings->leSubBufNum->Text.ToDouble()<32)
   formSettings->leSubBufNum->Text = FloatToStr(formSettings->leSubBufNum->Text.ToInt() * 2);
}
//---------------------------------------------------------------------------


void __fastcall TformSettings::cbCoherentAccumClick(TObject *Sender)
{
   formMain->bSendParamClick(this);   
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::cbDDSResetClick(TObject *Sender)
{
   if (cbDDSReset->Checked)
   {
      //rbDdsRstBurst->Enabled = true;
      rbDdsRstPulse->Enabled = true;
   }
   else
   {
      //rbDdsRstBurst->Enabled = false;
      rbDdsRstPulse->Enabled = false;
   }

   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::cbLOGMClick(TObject *Sender)
{
   if(cbLOGM->Checked) leDuration->Text = "0.0660";
   formMain->bSendParamClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton7UpClick(TObject *Sender)
{
   double value = leGeterodin->Text.ToDouble();
   if(value < 3e4) leGeterodin->Text = FloatToStr(value + 50);
}
//---------------------------------------------------------------------------
void __fastcall TformSettings::CSpinButton7DownClick(TObject *Sender)
{
   double value = leGeterodin->Text.ToDouble();
   if(value > 50) leGeterodin->Text = FloatToStr(value - 50);
}
//---------------------------------------------------------------------------

