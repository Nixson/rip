//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "mmsystem.h"
#include "MainUnit.h"
#include "DrawUnit.h"
#include "RasterUnit.h"
#include "SettingsUnit.h"
#include "RasterSettingsUnit.h"
#include "ProgressUnit.h"
#include "ThresholdUnit.h"
#include "MeasureUnit.h"
#include "CreateCoord.h"
#include "TargetUnit.h"
#include "ControlOptionsUnit.h"
#include "OrgMUnit.h"
#include "OrgDMUnit.h"
#include "OrgCMUnit.h"
#include "mem.h"
#include "mymath.h"
#include "params.h"
#include "ap.h"
#include "fasttransforms.h"        
#include "alglibinternal.h"
#include "ScatterUnit.h"
#include "formChanels.h"
#include "SettingsUnitRLM.h"
#include "Worker.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"

#define RAD 57.295779513082320877
#define MaxSpeed 50
#define PacketCount 46

TformMain *formMain;
TformDraw *formOscDraw;
TformDraw *formSpDraw;
TformDraw *formExpDraw;
TformRaster *formRasterDrawXX;
TformRaster *formRasterDrawYY;
TWorker * Worker;

int MainPosition = 0;
int WorkerPosition = 0;
int WorkerPositionResponse = 0;

int WorkerBufSize = 0;
int PlotPosition = 0;
unsigned int WorkerCmdNum = 0;
short int *WorkerBuffer;
int lePacketNumberMain = 0;

AnsiString Path = ExtractFilePath(Application->ExeName);

const unsigned int N = 65536*2;
double *DataBuf;
double OriginalPulse[N];
comp OriginalPulseComp[N];
double *OriginalPulseRe;
double *OriginalPulseIm;
double *MainResXXRe, *MainResXXIm, *MainResYYRe, *MainResYYIm;
double *MainResXXAbs, *MainResXXAng, *MainResYYAbs, *MainResYYAng;
double *MainResXXPhase, *MainResYYPhase;

double ResXXReAvg[N], ResXXImAvg[N];
double ResYYReAvg[N], ResYYImAvg[N];
double *CorrPtr;
double CorrNum;
double RefDataBuf[N];
double CoherentAccumBuf[N];
unsigned int CoherentAuccumNum = 32; //32 == 30��
unsigned int CoherentAuccumState = 0;

/*ALGLIB*/
alglib::real_1d_array a, b, r;
alglib::ae_int_t ia, ib;
alglib::complex_1d_array ALOriginalPulseComp;
/*******/

unsigned short CurrentCmdNum = 0;
unsigned int CurrentTxPacketNum = 1; // ����� �������� ������������� ������
unsigned int CurrentRxPacketNum = 0; // ����� �������� ��������� ������
unsigned int CurrentAckPacketNum = 0; // ����� ��������������� ������
unsigned int ARPAttempt = 0;
unsigned int DataAttempt = 0;
unsigned int ARPRetries = 0;

// ���������� ��� ������ ������ �� �����
unsigned int FileSubBufNum;
unsigned int FileVarNum;
unsigned int FileBufNum;
short int *FileBuffer = NULL;

short int *FileBufferFind = NULL;
unsigned int FileReadMode;
unsigned int FileReadType;
unsigned int FileReadBlockSize;


bool AckRecieved = false;
bool ARPRequestRecieved = false;
bool ARPAnswerRecieved = false;
bool WaitingARPAnswer = true;
bool WaitingAck = false;

TMemoryStream *PulseResponseMemory;
TMemoryStream *FileResponseMemory;
unsigned int *RepeatFrameList;

bool recieve_ok = false;
unsigned int FrameCnt = 0;
unsigned int LostPacketCnt = 0;

unsigned int RasterCounter = 0;
//---------------------------------------------------------------------------
void __fastcall RasterClean()
{
	 for(int i = 0; i<formRasterDrawXX->Table->Row; i++)
			for(int j = 0; j<formRasterDrawXX->Table->Col; j++)
				 formRasterDrawXX->Table->Data[i][j] = 0;

	 for(int i = 0; i<formRasterDrawYY->Table->Row; i++)
			for(int j = 0; j<formRasterDrawYY->Table->Col; j++)
				 formRasterDrawYY->Table->Data[i][j] = 0;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
double __fastcall TformMain::atan2Safe(double x, double y){
        return x == 0 && y == 0 ? 0 : atan2(x, y);
}
void __fastcall AlgLib_Convolution(double *ain, int aSize, double *bin, int bSize, double *res)
{
	 a.setlength(aSize);
	 b.setlength(bSize);
	 r.setlength(aSize+bSize-2);
	 a.setcontent(aSize, ain);
	 b.setcontent(bSize, bin);

	 alglib::convr1d(a, aSize, b, bSize, r);

	 res = r.getcontent();
}

//---------------------------------------------------------------------------
__fastcall TformMain::TformMain(TComponent* Owner)
	 : TForm(Owner)
{
	 //TIniFile *ini= new TIniFile(NULL);
	 //Hardware = new THardware();
	 //Hardware->OnRecieve = OnRecieve;
	 //Hardware->LoadConfig(ini);//
	 //CopyMAC_SRC(); // ����������� MAC �� Hardware
	 //Hardware->SetFilter("arp or udp dst port 30583"); // capture ������
	 //if(ini) delete ini;
         DataBuf = new double[N];
	 DecimalSeparator = '.';
	 CoordFileName = ExtractFileDir(Application->ExeName) + "\\CoordFile.bin";

	 IndicatorXX = NULL;
	 IndicatorYY = NULL;
	 CurrentAngle = 0;
	 PlatformAngle = 0;
	 mmFileReadTimer = 0;
         mmPlotTimer = 0;
         FileReadTimer->Enabled = false;
         videoServerStatus = false;
	 if(IndicatorXX) delete[] IndicatorXX;
	 if(IndicatorYY) delete[] IndicatorYY;
	 int size = 1024*leNumberOfMultOsc->Text.ToInt();
	 IndicatorXX = new double[size];
	 IndicatorYY = new double[size];
         Worker = new TWorker(false);

}
void __fastcall TformMain::cleanView(){
        MainPosition = 0;
        WorkerPosition = 0;
        PlotPosition = 0;
        WorkerPositionResponse = 0;
        int lePacketNumberLocal = SettingsUnitForm->lePacketNumber->Text.ToInt();
        if(lePacketNumberLocal > lePacketNumberMain)
                lePacketNumberMain = lePacketNumberLocal;
        if(WorkerBuffer) delete [] WorkerBuffer;
        WorkerBuffer = new short int [lePacketNumberMain*1030*PacketCount];

        if(MainResXXRe) delete [] MainResXXRe;
        if(MainResXXIm) delete [] MainResXXIm;
        if(MainResYYRe) delete [] MainResYYRe;
        if(MainResYYIm) delete [] MainResYYIm;
        MainResXXRe = new double [lePacketNumberMain*1024];
        MainResXXIm = new double [lePacketNumberMain*1024];
        MainResYYRe = new double [lePacketNumberMain*1024];
        MainResYYIm = new double [lePacketNumberMain*1024];

        if(MainResXXAbs) delete [] MainResXXAbs;
        if(MainResXXAng) delete [] MainResXXAng;
        if(MainResYYAbs) delete [] MainResYYAbs;
        if(MainResYYAng) delete [] MainResYYAng;
        if(MainResXXPhase) delete [] MainResXXPhase;
        if(MainResYYPhase) delete [] MainResYYPhase;
        MainResXXAbs = new double [lePacketNumberMain*1024];
        MainResXXAng = new double [lePacketNumberMain*1024];
        MainResYYAbs = new double [lePacketNumberMain*1024];
        MainResYYAng = new double [lePacketNumberMain*1024];
        MainResXXPhase = new double [lePacketNumberMain*1024];
        MainResYYPhase = new double [lePacketNumberMain*1024];

}

void CALLBACK mmTimerPulseClick(unsigned int uID, unsigned int uMsg, DWORD dwUser, DWORD dw1, DWORD dw2){
        
}
void CALLBACK mmPlotTimerProc(unsigned int uID, unsigned int uMsg, DWORD dwUser, DWORD dw1, DWORD dw2){
				try {
					formMain->PlotTimerTimer(formMain);
				}
				catch (Exception &exception) {
								ShowMessage("������ � �������!");
				}
}

void __fastcall TformMain::MathAndPlot(int BufSize, int CurBufNum)
{
		// �������������� ��������� ����������� �����������
		//Math1(BufSize, DataBuf);
		// ���������� ������ � ������ ��� ��������� ��������, ������ 1024 ������� �� ������
		if(IndicatorXX) memcpy(&IndicatorXX[1024*CurBufNum], ResXXAbs, 1024*sizeof(double));
		if(IndicatorYY) memcpy(&IndicatorYY[1024*CurBufNum], ResYYAbs, 1024*sizeof(double));
		// ������� ���������� ��� �� ������
		/*else */RasterCounter++;
		if(RasterCounter >= (unsigned)(leRasterPeriod->Text.ToIntDef(10)))
			RasterCounter = 0;

		int offset, length;
		if(rbRasterAF->Checked) // ����� �� ��������� � ������ "��"
		{
			 if(RasterCounter == 0) RasterClean();
			 PlotDrawParam(3,ResXXAbs,BufSize/4);
			 //!!formExpDraw->DrawOscCoherentAccum(ResXXAbs, BufSize/4);

			 // ���������� ������ ����������� ��
			 offset = formRasterSettings->UpDownX1->Position;
			 length = formRasterDrawXX->Table->Row; // ����� �������
			 if(BufSize/4 < (length+offset)) length = (BufSize/4-offset); // �������� ������ �� ������� �������
			 double value = atan2Safe(0, -10);
			 int index;
			 for(int i=0; i<length; i++)
			 {
					// !!! �������� ��������� � ��������� �� -180 �� 180
					index = round(ResXXAng[i+offset]+180); // ���������� ��������� ��� ����������� ������� ������
					// ���������� ���� � ��������� �� 0 �� 360
					while(index >= formRasterDrawXX->Table->Col) index -= formRasterDrawXX->Table->Col;
					while(index < 0) index += formRasterDrawXX->Table->Col;
					// ���������� ��������
					value = formRasterDrawXX->Table->Data[i][index];
					formRasterDrawXX->Table->Data[i][index] = (value+ResXXAbs[i+offset]);
			 }
			 //!!formRasterDrawXX->Raster1->Plot();

			 offset = formRasterSettings->UpDownY1->Position;
			 length = formRasterDrawYY->Table->Row; // ����� �������
			 if(BufSize/4 < (length+offset)) length = (BufSize/4-offset); // �������� ������ �� ������� �������
			 for(int i=0; i<length; i++)
			 {
					// !!! �������� ��������� � ��������� �� -180 �� 180
					index = round(ResYYAng[i+offset]+180); // ���������� ��������� ��� ����������� ������� ������
					// ���������� ���� � ��������� �� 0 �� 360
					while(index >= formRasterDrawYY->Table->Col) index -= formRasterDrawYY->Table->Col;
					while(index < 0) index += formRasterDrawYY->Table->Col;
					// ���������� ��������
					value = formRasterDrawYY->Table->Data[i][index];
					formRasterDrawYY->Table->Data[i][index] = (value+ResYYAbs[i+offset]);
			 }
			 //!!formRasterDrawYY->Raster1->Plot();
		}
		else // ����� ������������ ��� �������� ������ � �������
		{

			 if(rbRasterA->Checked) PlotDrawParam(3,ResXXAbs,BufSize/4);
			 else  PlotDrawParam(3,ResXXAng,BufSize/4);
				//!!if(rbRasterA->Checked) formExpDraw->DrawOscCoherentAccum(ResXXAbs, BufSize/4); // ����������� �����������
				//!!else formExpDraw->DrawOscCoherentAccum(ResXXAng, BufSize/4); // ������� �����������

				// ���������� � ����� ����������� ��
				offset = formRasterSettings->UpDownX1->Position; // ������ �� ���� ����������
				length = formRasterDrawXX->Table->Row; // ����� �������
				if(BufSize/4 < (length+offset)) length = (BufSize/4-offset); // �������� ������ �� ������� �������
				for(int i=0; i<length; i++)
				{
					if(rbRasterA->Checked) formRasterDrawXX->Table->Data[i][CurBufNum] = (ResXXAbs[i+offset]); //���������
					else formRasterDrawXX->Table->Data[i][CurBufNum] = (ResXXPhase[i+offset]); // ����
				}
				// ���������� � ����� ����������� YY
				offset = formRasterSettings->UpDownY1->Position; // ������ �� ���� ����������
				length = formRasterDrawYY->Table->Row; // ����� �������
				if(BufSize/4 < (length+offset)) length = (BufSize/4-offset); // �������� ������ �� ������� �������
				for(int i=0; i<length; i++)
				{
					if(rbRasterA->Checked) formRasterDrawYY->Table->Data[i][CurBufNum] = (ResYYAbs[i+offset]); //���������
					else formRasterDrawYY->Table->Data[i][CurBufNum] = (ResYYPhase[i+offset]); // ����
				}
				// ���������� �������
				//!!formRasterDrawXX->Raster1->Plot();
				//!!formRasterDrawYY->Raster1->Plot();
		}
}
//---------------------------------------------------------------------------
__fastcall TformMain::~TformMain()
{
     if(ScatterForm) ScatterForm->Close();
	 //TIniFile *ini= new TIniFile(NULL);
	 //Hardware->SaveConfig(ini);
         FileReadTimer->Enabled = false;
         PlotTimer->Enabled = false;
	 //timeKillEvent(mmFileReadTimer);
	 timeKillEvent(mmPlotTimer);
	 mmFileReadTimer = 0;
         mmPlotTimer = 0;
	 //if(Hardware) delete Hardware;
	 //delete ini;
}
//---------------------------------------------------------------------------
unsigned int Count = 0;

void __fastcall TformMain::PrintPacket(unsigned Size, unsigned char *Buffer)
{
         if(!Memo->Visible)
                return;
	 AnsiString s = "";
	 for(unsigned int i=0;i<Size;i++)
	 {
			s += IntToHex(Buffer[i], 2)+" ";
			if(((i+1)%16 == 0) || (i == Size-1))
			{
				 while(s.Length()<48) s+=" ";
				 Memo->Lines->Add(s);
				 s = "";
			}
	 }
}
void __fastcall TformMain::Log(AnsiString &msg){
        if(Memo->Visible)
                Memo->Lines->Add(msg);
}
//---------------------------------------------------------------------------
unsigned int now=0, last=0, avg_time=0;
unsigned long avg_size = 0;
//---------------------------------------------------------------------------

void __fastcall TformMain::OnRecieve(unsigned &Size, char *Buffer)
{
	 Count++;
	 
	 /*if(avg_time > 1000)
	 {
			double speed = (avg_size/(avg_time+1e-9))*1e-3;
			formMain->Caption = IntToStr(Count) +" "+ FloatToStrF(speed, ffFixed, 2, 1)+" ��/�";

			avg_time = 0;
			last = ::GetTickCount();
			avg_size = 0;
	 }
	 else
	 {
			avg_time = (::GetTickCount() - last);
			avg_size += Size;
	 }

	 // ARP
	 ARPRequestRecieved = CheckForARPRequest((unsigned char *) Buffer); // Automatically Answer if Request recieved
	 if(ARPRequestRecieved)
	 {

			return;
	 }

	 if(WaitingARPAnswer)
	 {
			ARPAnswerRecieved = CheckForARPAnswer((unsigned char *) Buffer);//

			if(ARPAnswerRecieved)
			{

				 WaitingARPAnswer = false;
			}
			return;
	 }

	 // ACK
	 if(WaitingAck)
	 {
			AckRecieved = CheckForAck(Buffer, CurrentAckPacketNum);
			if(AckRecieved)
			{
				 if((CurrentTxPacketNum-1) == CurrentAckPacketNum)
				 {

						WaitingAck = false;
				 }
			}
			return;
	 }

	 // Answer
	 bool Status = CheckForAnswer(Buffer, CurrentCmdNum, CurrentRxPacketNum);
	if(Status)
	{

		//ProcessAnswer(Buffer, CurrentCmdNum);
			//if((FrameCnt%32) == 0) formProgress->Label3->Caption = "������� "+IntToStr(FrameCnt)+" �������";
			//FrameCnt++;
	};
        */
}
//---------------------------------------------------------------------------
void __fastcall TformMain::N2Click(TObject *Sender)
{
	 //if(Hardware) Hardware->SelectAdapter();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::N3Click(TObject *Sender)
{
	 //if(Hardware) Hardware->CloseAdapter();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::mmShowOscClick(TObject *Sender)
{
		if(formOscDraw) return;
				formOscDraw = new TformDraw(this);
				formOscDraw->Chart->PopupMenu = formOscDraw->PopupMenu1;
				formOscDraw->Caption = "�������������";
				formOscDraw->Chart->Title->Text->Text = "�������������";
				formOscDraw->Chart->LeftAxis->Title->Caption = "��� ���";
				formOscDraw->Chart->BottomAxis->Title->Caption = "�����, ��";
				formOscDraw->Chart->BottomAxis->Automatic = true;
				formOscDraw->Chart->LeftAxis->Automatic = false;
				formOscDraw->Chart->LeftAxis->Maximum = 512;
				formOscDraw->Chart->LeftAxis->Minimum = -512;
				formOscDraw->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::mmShowSpClick(TObject *Sender)
{
		if(formSpDraw) return;
				formSpDraw = new TformDraw(this);
				formSpDraw->Caption = "������";
				formSpDraw->Chart->Title->Text->Text = "������";
				formSpDraw->Chart->LeftAxis->Title->Caption = "�������, ��";
				formSpDraw->Chart->BottomAxis->Title->Caption = "�������, ���";
				formSpDraw->Chart->LeftAxis->AutomaticMaximum = false;
				formSpDraw->Chart->LeftAxis->Maximum = 0;
				formSpDraw->Chart->LeftAxis->Minimum = -70;
				formSpDraw->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::mmShowExpClick(TObject *Sender)
{
		if(formExpDraw) return;
				formExpDraw = new TformDraw(this);
				formExpDraw->Caption = "�����������";
				formExpDraw->Chart->PopupMenu = formExpDraw->PopupMenu1;
				formExpDraw->Chart->LeftAxis->Automatic = true;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::mmTileClick(TObject *Sender)
{
	 TileMode = tbVertical;
	 Tile();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::FormActivate(TObject *Sender)
{
	 formMain->WindowState = wsMaximized;
	 mmShowOscClick(this);
	 mmShowSpClick(this);
	 mmShowExpClick(this);
	 mmShowExpClick(this);
	 mmRasterShowClick(this);

	 formOscDraw->WindowState = wsMinimized;
	 formSpDraw->WindowState = wsMinimized;
	 mmTileClick(this);
         formMain->btOriginalPulseClick(formMain);
         //timeSetEvent(2000,0,mmTimerPulseClick,NULL,TIME_ONESHOT);
         //mmPlotTimer = timeSetEvent(60,0,mmPlotTimerProc,NULL,TIME_PERIODIC);


}
//---------------------------------------------------------------------------
void __fastcall TformMain::mmSettingsClick(TObject *Sender)
{
	 formSettings->Show();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::N4Click(TObject *Sender)
{
	 Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::Button1Click(TObject *Sender)
{
	 for(unsigned int i=0;i<1024;i++)
	 formOscDraw->Chart->Series[0]->AddXY(i, i, IntToStr(i), clRed);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::N5Click(TObject *Sender)
{
	 AnsiString FileName = leExpName->Text+"_"+FormatDateTime("yyyy.mm.dd_hh.mm.ss", Now());
	 formOscDraw->SaveBMP("DATA\\bmp\\"+FileName+".bmp");
	 formOscDraw->SaveTEE("DATA\\tee\\"+FileName+".tee");
	 formOscDraw->SaveDAT("DATA\\dat\\"+FileName+".dat");
	 formOscDraw->SaveTXT("DATA\\txt\\"+FileName+".txt");
	 formSettings->SaveConfig("DATA\\ini\\"+FileName+".ini");
}
//---------------------------------------------------------------------------
void __fastcall TformMain::N6Click(TObject *Sender)
{
	 AnsiString FileName = "ExitState.ini";
	 formSettings->SaveConfig(FileName);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::Button2Click(TObject *Sender)
{
	 //SendARPRequest();
	 WaitingARPAnswer = true;
}
//---------------------------------------------------------------------------
unsigned int test = 0;
// ������� ��������� ������� �� ��� �������� ��������� �� ����� ������ � ����� �������

void __fastcall TformMain::ProcessAnswer(short int *Buffer, unsigned int CmdNum){
        WorkerCmdNum = CmdNum;
        //1030 = 1024 + 6(��������������)
        memcpy(WorkerBuffer+MainPosition*1030,Buffer,1030*2);
        ++MainPosition;
}

// ������� �������� ������ � ��������� ���������
void __fastcall TformMain::ControlAndMeasure(int BufSize, int CurBufNum)
{
		// �������� ������
		if(ThresholdForm)
		{
				int Rmin, Rmax, Threshold;
				// ����������� ������ ������
				double Value;
				Rmin = ThresholdForm->UpDown2->Position;
				Rmax = ThresholdForm->UpDown3->Position;
				// ����������������
				if(Rmax < Rmin)
				{
					Threshold = Rmin;
					Rmin = Rmax;
					Rmax = Threshold;
				}
				Threshold = ThresholdForm->UpDown1->Position;
				Value = 0;

				int r;
				for(r = Rmin; ((r <= Rmax) && (r < BufSize)); r++) // ���������� ������ � ������
						Value += ResXXAbs[r];
				Value /= (r-Rmin);
				// ����� ������� �������
				if(Value > Threshold)
				{
						ThresholdForm->Button1->Caption = "�������";
						ThresholdForm->Button1->Font->Color = clRed;
						ThresholdForm->Button1->Enabled = 1;
						ThresholdForm->Visible = 1;
				}
		}

		// ���������� ������ � ������ ��� ��������� ���������, ������ 1024 ������� �� ������
		if(IndicatorXX) memcpy(&IndicatorXX[1024*CurBufNum], ResXXAbs, 1024*sizeof(double));
		if(IndicatorYY) memcpy(&IndicatorYY[1024*CurBufNum], ResYYAbs, 1024*sizeof(double));
		// �������� ������  �� ��������� ���������

		if(RadioGroup1->ItemIndex == 1) // ��������� � ������ ���������� �����������
		{
			 // ����������� ���������� ���� ���������
			 int lo = 0, hi= 1024;
			 double Threshold = 2000;
			 if(formMeasureSettings) // ���������� �������� ��������� ���������, ���� ����
			 {
				 lo = formMeasureSettings->MinUpDown->Position;
				 hi = formMeasureSettings->MaxUpDown->Position;
				 Threshold = formMeasureSettings->Threshold;
			 }
			 // ��������� ����������� ������
			 if(RasterCounter == 0)
			 {
				 // ����� ���������� �� ���������� ���������
				 int n = leRasterPeriod->Text.ToIntDef(10);
				 double ResXXAbsAvg = 0, ResYYAbsAvg = 0;
				 int indexXX = -1, indexYY = -1;
				 double maxXX = 0, maxYY = 0;
				 double SumXXRe = 0, SumXXIm = 0, SumYYRe = 0, SumYYIm = 0, AbsYY=0, AbsXX=0, AbsSS=0,PhasXXYY=0, DistXY=0;
				 for(int i = lo; i < hi; i++)
				 {
					 ResXXReAvg[i] /= n; ResXXImAvg[i] /= n;
					 SumXXRe += ResXXReAvg[i]; SumXXIm += ResXXImAvg[i]; // ������������ �������� � ������
					 ResXXAbsAvg = sqrt(ResXXReAvg[i]*ResXXReAvg[i] + ResXXImAvg[i]*ResXXImAvg[i]);
					 if((ResXXAbsAvg > maxXX) && (ResXXAbsAvg > Threshold)) {indexXX = i; maxXX = ResXXAbsAvg;}

					 ResYYReAvg[i] /= n; ResYYImAvg[i] /= n;
					 SumYYRe += ResYYReAvg[i]; SumYYIm += ResYYImAvg[i]; // ������������ �������� � ������
					 ResYYAbsAvg = sqrt(ResYYReAvg[i]*ResYYReAvg[i] + ResYYImAvg[i]*ResYYImAvg[i]);
					 if((ResYYAbsAvg > maxYY) && (ResYYAbsAvg > Threshold)) {indexYY = i; maxYY = ResYYAbsAvg;}
				 }
				 double DistXX = 0, DistYY = 0, PhasXX = 0, PhasYY = 0;
				 // ����������� ��������� ����� � ������ �������� ��������
				 // � �������������� ������
				 if(indexXX >=0)
				 {
					 DistXX = 3e8*indexXX*Td/2; // ��������� ��������� �� ��������� � ������
					 // �������� ��� �� ����� � ������
					 if((fabs(SumXXIm) > 1e-12) || (fabs(SumXXRe) > 1e-12))
							 PhasXX = atan2Safe(SumXXIm, SumXXRe)*RAD;
					 else PhasXX = 0;
                                         if(PhasXX >= (double)LeftRightDelta->Value ){
                                                 PhasXX = 0;
                                                 indexXX = -1;
                                                 AnsiString info = "X: "+FloatToStr(PhasXX)+": "+FloatToStr(SumXXIm)+": "+FloatToStr(SumXXRe);
                                                 Log(info);
                                         }
				 }
				 // � ������������ ������
				 if(indexYY >=0)
				 {
					 DistYY = 3e8*indexYY*Td/2; // ��������� ��������� �� ��������� � ������
					 // �������� ��� �� ����� � ������ !!! ���� ������������ �����
					 if((fabs(SumYYIm) > 1e-12) || (fabs(SumYYRe) > 1e-12))
							 PhasYY = atan2Safe(SumYYIm, SumYYRe)*RAD;

					 else PhasYY = 0;
                                         if(PhasYY >= (double)LeftRightDelta->Value ) {
                                                PhasYY = 0;
                                                indexYY = -1;
                                                AnsiString info = "Y: "+FloatToStr(PhasYY)+": "+FloatToStr(SumYYIm)+": "+FloatToStr(SumYYRe);
                                                Log(info);
                                         }
				 }

				 // ����� ������ �� �������
        			 double bearingCoef, bearing;
				 // ������ ������� ������� �� ��
				 bearingCoef = formSettings->leFreq->Text.ToDouble() + formSettings->leGeterodin->Text.ToDouble();
				 // �������� � ����������� �������������� ������������� ��� �������� ��� � ��������
				 bearingCoef *= 360*formMeasureSettings->BearingBase/3e2; // ������� � ���, ������� ������� �� 300

				 if(formTargetDraw)
				 {

					 formTargetDraw->Chart1->Title->Text->Clear();
					 if(indexXX >= 0)
					 {
						 bearing = PhasXX/bearingCoef;
						 if(fabs(bearing <=1)) PhasXX = RAD*asin(bearing);  //
						 else
						 {
							 PhasXX = 0;
							 DistXX = 0;
						 }
						 formTargetDraw->Series2->AddXY(PhasXX, DistXX); // ����� �� �������� ���, � ����
						 formTargetDraw->Chart1->Title->Text->Add("XX: ��������� = " + FormatFloat("##0.##", DistXX) + " �; " + "���������� = " + FormatFloat("##0.##", PhasXX) + " ����");
					 }
					 if(indexYY >= 0)
					 {
						 bearing = PhasYY/bearingCoef;
						 if(fabs(bearing <=1)) PhasYY = RAD*asin(bearing);
						 else
						 {
							 PhasYY = 0;
							 DistYY = 0;
						 }
						 formTargetDraw->Series3->AddXY(PhasYY, DistYY); // ����� �� �������� ���, � ����
						 formTargetDraw->Chart1->Title->Text->Add("YY: ��������� = " + FormatFloat("##0.##", DistYY) + " �; "+"���������� = " + FormatFloat("##0.##", PhasYY) + " ����");
					 }
                                 }
				 // !!!! �������������� ������������� ���� ��������� ����

                                      // ������ XX, YY  ��� �������� ����.
                                       AbsYY= pow(SumYYIm*SumYYIm + SumYYRe*SumYYRe, 0.5);
                                       AbsXX= pow(SumXXIm*SumXXIm + SumXXRe*SumXXRe, 0.5);
                                       AbsSS=AbsYY+AbsXX;
                                       PhasXXYY= (AbsYY*PhasYY+AbsXX*PhasXX)/AbsSS;
                                       DistXY=(DistXX+DistYY)/2;
                                       // ����� ���������� ����������
                                       formTargetDraw->Series1->AddXY(PhasXXYY, DistXY); // ����� �� �������� ���, � ����
						 formTargetDraw->Chart1->Title->Text->Add("XY: ��������� = " + FormatFloat("##0.##", DistXY) + " �; "+"���������� = " + FormatFloat("##0.##", PhasXXYY) + " ����");

                                        // ������� 5.05.2015  ������  CurrentAngle = PlatformAngle + PhasYY; ��, ��� ����
                                        //    CurrentAngle = PlatformAngle + (PhasYY+PhasXX)/2;     ResYYAbs[i]


                                 CurrentAngle = PlatformAngle + PhasXXYY;

                                 IdUDPClient2->Send(FloatToStr(CurrentAngle)+"/"+FloatToStr(DistYY));

				 if(AutoCheckBox->Checked && (indexYY>=0))
				 {
					 // ������� ��������� �� ����� ������
					 if(fabs(PhasXXYY) > 0.5)
					 {
						 // ��������� �������� �������� ���������
						 double arg = (RAD*asin(180/bearingCoef)); // �������� �������������� ��������������
						 // ���� �������� �������� � �������� � ������, �������� 50
						 arg = MaxSpeed*fabs(PhasYY)/arg; // ���������� ������� �� �������� ��������������
                                                 //IdUDPClient1->Send("S"+FormatFloat("000", round(arg)));
						 SetPosition(CurrentAngle,arg);  // ����� ������ ������� ��������
					 }
					 else IdUDPClient1->Send("A"); // ����� ��������� ���������, � ������ �������� ������ ��������� ���������
                                         //Memo->Lines->Add("\r\n"+IntToStr(indexXX)+": ("+IntToStr(fabs(PhasXX))+")");
					 // ��������� ������ ��������� ��� ��������
					 if(formMeasureSettings)
					 {
						 formMeasureSettings->MinUpDown->Position = indexYY - (int)UpDownDelta->Value;
						 formMeasureSettings->MaxUpDown->Position = indexYY + (int)UpDownDelta->Value;
						 formMeasureSettings->MinUpDownClick(this, (TUDBtnType)0);
						 formMeasureSettings->MaxUpDownClick(this, (TUDBtnType)0);
					 }
				 }
				 // ����� �������� � ������� ��������� ������

				 // ������� ���������� ��� ����� ��������
				 memset(ResXXReAvg, 0, 1024*sizeof(double));
				 memset(ResXXImAvg, 0, 1024*sizeof(double));
				 memset(ResYYReAvg, 0, 1024*sizeof(double));
				 memset(ResYYImAvg, 0, 1024*sizeof(double));
			 }
			 // ������������ ���������
			 for(int i = lo; i < hi; i++)
			 {
				 // ���������� ���������� ��� ����������
				 ResXXReAvg[i] += ResXXRe[i];
				 ResXXImAvg[i] += ResXXIm[i];
				 ResYYReAvg[i] += ResYYRe[i];
				 ResYYImAvg[i] += ResYYIm[i];
			 }
		}
		// ��������� ����������� ������������� ������������
		else if(((CurBufNum+1) == leNumberOfMultOsc->Text.ToInt()) && (IndicatorXX || IndicatorYY)) // ������������ ������������ �������
		{
			int Na = CurBufNum+1;
			int Nd = 1024;
			int lo = 0, hi = Nd;
			// ��������� ������� ��������� �� ����� �������
			if(formMeasureSettings)
			{
				lo = formMeasureSettings->MinUpDown->Position;
				hi = formMeasureSettings->MaxUpDown->Position;
				Nd = hi-lo;
			}
			// ����������� ������� ��������� � ���������� ������
			double *SamplesXX = new double[Na*Nd];
			double *SamplesYY = new double[Na*Nd];
			for(int na = 0; na < Na; na++)
			{
				if(IndicatorXX) memcpy(&SamplesXX[na*Nd], &IndicatorXX[na*1024+lo], Nd*sizeof(double));
				if(IndicatorYY) memcpy(&SamplesYY[na*Nd], &IndicatorYY[na*1024+lo], Nd*sizeof(double));
			}

			// ���������� ��� ������� ��������� �� ������ �������
			double Dmin, Dmax, Amin, Amax, dD, dA, Thold;
			if(formMeasureSettings)
			{
				Dmin = 3e8*Td*lo/2; // ������� �������, � ������
				Dmax = 3e8*Td*hi/2; // ������� ������� � ������
			}
			else
			{
				Dmin = 0; // ������� �������, � ������
				Dmax = 3e8*Td*Nd/*(formRasterSettings->UpDownX2->Position+1)*//2; // ������� ������� � ������
			}
			dD = 3e9*Td/2; // ����������� ����������� �� ��������� � ������
			Amin = 0; // ����������� ���� ������� � ��������
			Amax = leScanSector->Text.ToDouble(); // ������������ ���� ������� � ��������
			dA = 3; // ����������� ����������� � ��������
			if(formMeasureSettings) Thold = formMeasureSettings->Threshold; // ����� �����������
			else Thold = 1000;

			int Nt;
			float *X = NULL,*Y = NULL; //���������� ��� ��������� ���������
			// X - ���������
			// Y - ���� �������
			// ���� ����� ������� ������� ���������
			try
			{
				formTargetDraw->Chart1->Title->Text->Clear();
				// ��������� ��������� �� ������ ����������� XX
				Nt = CreateCoord(Nd, Na, SamplesXX, dD, dA, Thold, Dmin, Dmax, Amin, Amax, &X, &Y);
				// ����������� ���������
				if((Nt > 0) && X && Y && formTargetDraw)
				{
					// ���������� ���������, ���������� ���� � ��������� ���������
					double D, A;
					D = X[0];//sqrt(X[0]*X[0]+Y[0]*Y[0]); // ���������
					A = Y[0];//180/M_PI*atan2(Y[0], X[0]); // ������
					while(A < 0) A+=360;
					while(A > 360) A-=360;
					// ����� ������ � ����������� ��������� ��������� ��� ������������
					formTargetDraw->Chart1->Title->Text->Add("��������� XX = " + FormatFloat("##0.##", D) +
																									 " �: ������ XX = " + FormatFloat("##0.##", A) + " ����");
					// ��������������� ����, ������� ���������
					CurrentAngle = A;
					if(AutoCheckBox->Checked)
					{
						 // ������ ������� �������
						 SetPosition(A, MaxSpeed);
/*
						 // ������������ � �������������� �����
						 RadioGroup1->ItemIndex = 1;
//             rbRasterAF->Checked = 1;
						 leNumberOfMultOsc->Text = leRasterPeriod->Text;
//             bSendParamClick(this);
*/
						 // ��������� ������ ��������� ��� ��������
						 if(formMeasureSettings)
						 {
							 int indexXX = round(D/(3e8*Td/2));
							 formMeasureSettings->MinUpDown->Position = indexXX - (int)UpDownDelta->Value;
							 formMeasureSettings->MinUpDownClick(this, (TUDBtnType)0);
							 formMeasureSettings->MaxUpDown->Position = indexXX + (int)UpDownDelta->Value;
							 formMeasureSettings->MaxUpDownClick(this, (TUDBtnType)0);
						 }
					}
					// ��������� ��������� �� ��������
					formTargetDraw->Series2->Clear();
					formTargetDraw->Series2->AddXY(A, D);
				}
				else // ���� ���� ��� ������������ �� ������� - ��������� � ��������� ������ ������������
				{
					if(AutoCheckBox->Checked) // ������� � ������ ������ � ��
					{
						AutoCheckBox->Checked = 0;
						SetPosition(PlatformAngle, MaxSpeed);
					}
				}
				if(X) {delete[] X; X = NULL;}
				if(Y) {delete[] Y; Y = NULL;}
				// ��������� ��������� �� ������ ����������� XX
				Nt = CreateCoord(Nd, Na, SamplesYY, dD, dA, Thold, Dmin, Dmax, Amin, Amax, &X, &Y);
				// ����������� ���������
				if((Nt > 0) && X && Y && formTargetDraw)
				{
					// ���������� ���������, ���������� ���� � ��������� ���������
					double D, A;
					D = X[0];//sqrt(X[0]*X[0]+Y[0]*Y[0]); // ���������
					A = Y[0];//180/M_PI*atan2(Y[0], X[0]); // ������
					while(A < 0) A+=360;
					while(A > 360) A-=360;
					// ����� ������ � ����������� ��������� ��������� ��� ������������
					formTargetDraw->Chart1->Title->Text->Add("��������� YY = " + FormatFloat("##0.##", D) +
																									 " �: ������ YY = " + FormatFloat("##0.##", A) + " ����");
					// ��������� ��������� �� ��������
					formTargetDraw->Series3->Clear();
					formTargetDraw->Series3->AddXY(A, D);
				}
			}
			catch(Exception &ex)
			{
				ShowMessage("������ ��������� ���������\n" + ex.Message);
			}
			// ������� ������
			delete[] SamplesXX;
			delete[] SamplesYY;
			if(X) delete[] X;
			if(Y) delete[] Y;
		}

}
//---------------------------------------------------------------------------
void __fastcall TformMain::TimerAckTimer(TObject *Sender)
{

	 if(WaitingAck)//&& ARPAnswerRecieved
	 {
			if(DataAttempt<9)
			{
				 DataAttempt++;
			}else
			{
				 WaitingAck = false;
				 DataAttempt = 0;
				 sbMainStatusBar->Panels->Items[2]->Text = "Ack Recieved: no";
			}
	 }else
	 {
			WaitingAck = false;
			TimerAck->Enabled = false;
			DataAttempt = 0;
	 }

	 sbMainStatusBar->Panels->Items[0]->Text = "Resend Attempt: " + IntToStr(DataAttempt);

	 if(AckRecieved)
	 {
			sbMainStatusBar->Panels->Items[2]->Text = "Ack Recieved: yes";
	 }
}

void __fastcall TformMain::TimerARPTimer(TObject *Sender)
{
        int BufSize = 1024;
        int VarBufSize = BufSize*4;
        if(PlotPosition < WorkerPositionResponse) {
                for( ; PlotPosition <= WorkerPositionResponse; ++PlotPosition){
                        memcpy(ResXXRe,MainResXXRe+PlotPosition*1024,1024*sizeof(double));
                        memcpy(ResXXIm,MainResXXIm+PlotPosition*1024,1024*sizeof(double));
                        memcpy(ResYYRe,MainResYYRe+PlotPosition*1024,1024*sizeof(double));
                        memcpy(ResYYIm,MainResYYIm+PlotPosition*1024,1024*sizeof(double));

                        memcpy(ResXXAbs,MainResXXAbs+PlotPosition*1024,1024*sizeof(double));
                        memcpy(ResYYAbs,MainResYYAbs+PlotPosition*1024,1024*sizeof(double));
                        memcpy(ResXXAng,MainResXXAng+PlotPosition*1024,1024*sizeof(double));
                        memcpy(ResYYAng,MainResYYAng+PlotPosition*1024,1024*sizeof(double));

                        memcpy(ResXXPhase,MainResXXPhase+PlotPosition*1024,1024*sizeof(double));
                        memcpy(ResYYPhase,MainResYYPhase+PlotPosition*1024,1024*sizeof(double));
						if(formOscDraw)
						{
							if(formOscDraw->WindowState != wsMinimized){
								PlotDrawParam(1,DataBuf,VarBufSize);
							}
						}
						if(formSpDraw)
						{
							if(formSpDraw->WindowState != wsMinimized){
								PlotDrawParam(2,DataBuf,VarBufSize);
							}
						}
                        sbMainStatusBar->Panels->Items[0]->Text = "����� "+IntToStr(PlotPosition);
			MathAndPlot(1024*4, PlotPosition);
			// �������� ������ � ���� � ��������� ���������
			// ����� ������ ����� MathAndPlot

			ControlAndMeasure(1024, PlotPosition);
                }
        }
	 /*if(WaitingARPAnswer)
	 {
			if(ARPAttempt<2)
			{
				 SendARPRequest();
				 ARPAttempt++;
			}else
			{
				 WaitingARPAnswer = false;
				 ARPAttempt = 0;
			}
	 }
         */
	 /*if(ARPRetries<100)
	 {
			ARPRetries++;
	 }else
	 {
			ARPRetries = 0;
			ARPAttempt = 0;
			ARPAnswerRecieved = false;
	 }

	 sbMainStatusBar->Panels->Items[1]->Text = "ARP retry timer: " + IntToStr(100-ARPRetries); */
}

void __fastcall TformMain::RestartTimer(TTimer *Timer)
{
	 Timer->Enabled = false;
	 Timer->Enabled = true;
}

void __fastcall TformMain::SendData(int Size, void *Buffer)
{
	 if(!ARPAnswerRecieved)
	 {
			WaitingARPAnswer = true;
			while(WaitingARPAnswer){Application->ProcessMessages();::Sleep(1);};
	 }

	 if(!ARPAnswerRecieved) return;

	 //Hardware->SendData(Size, Buffer);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::Button3Click(TObject *Sender)
{
	 //SendBulkPacket(CurrentTxPacketNum);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::bSendParamClick(TObject *Sender)
{
	 unsigned short BuffSize = 70;
	 unsigned char *Buffer = new unsigned char[BuffSize];
	 unsigned char *DataPtr = Buffer;

	 *((double *)(DataPtr)) = formSettings->leFreq->Text.ToDouble();
	 DataPtr += sizeof(double);//8
	 *((double *)(DataPtr)) = formSettings->leAmp->Text.ToDouble()>0?((formSettings->leAmp->Text.ToDouble()/100)*0xFFF8)-1:0;
	 DataPtr += sizeof(double);//16
	 *((double *)(DataPtr)) = formSettings->lePeriod->Text.ToDouble();
	 DataPtr += sizeof(double);//24
	 *((double *)(DataPtr)) = formSettings->leDuration->Text.ToDouble();
	 DataPtr += sizeof(double);//32
	 *((int *)(DataPtr)) = formSettings->leBurstLen->Text.ToIntDef(1);
	 DataPtr += sizeof(int);//36
	 *((double *)(DataPtr)) = formSettings->leFreqRange->Text.ToDouble();
	 DataPtr += sizeof(double);//44
	 *((double *)(DataPtr)) = formSettings->leGateDelay->Text.ToDouble();
	 DataPtr += sizeof(double);//52
	 *((double *)(DataPtr)) = formSettings->leGateDuration->Text.ToDouble();
	 DataPtr += sizeof(double);//60
	 *((int *)(DataPtr)) =   (((formSettings->cbLOGM->Checked)?2:((formSettings->rbDdsRstBurst->Checked)?0:1))<<23)|
													 ((formSettings->cbMGEN->Checked)<<9)|
													 ((formSettings->cbDDSReset->Checked)<<6)|
													 ((formSettings->cbCont->Checked)<<4)|
													 ((formSettings->cbUWB->Checked)<<3)|
													 ((formSettings->cbGate->Checked)<<2)|
													 ((formSettings->cbLFM->Checked)<<1)|
													 ((formSettings->cbPulseMod->Checked)<<0);
	 DataPtr += sizeof(int);//64
	 *((short *)(DataPtr)) = (short)(formSettings->leTxAtt->Text.ToDouble()*2);
	 DataPtr += sizeof(short);//66
	 *((short *)(DataPtr)) = (short)(formSettings->leRxAtt->Text.ToDouble()*2);
	 DataPtr += sizeof(short);//68

	 short TxPolDec =  (formSettings->rbTxPolYY->Checked<<3)|(formSettings->rbTxPolYX->Checked<<2)|
										 (formSettings->rbTxPolXY->Checked<<1)|(formSettings->rbTxPolXX->Checked<<0);
	 short RxPolDec =  (formSettings->rbRxPolYY->Checked<<3)|(formSettings->rbRxPolYX->Checked<<2)|
										 (formSettings->rbRxPolXY->Checked<<1)|(formSettings->rbRxPolXX->Checked<<0);
	 short RxAntDec =  (formSettings->rbRxAnt1->Checked<<1) |(formSettings->rbRxAnt0->Checked<<0);

	 short TxPol, RxPol, RxAnt;

	 switch(TxPolDec)
	 {
			case 0x1:{TxPol = 0;} break;
			case 0x2:{TxPol = 1;} break;
			case 0x4:{TxPol = 2;} break;
			case 0x8:{TxPol = 3;} break;
	 }

	 switch(RxPolDec)
	 {
			case 0x1:{RxPol = 0;} break;
			case 0x2:{RxPol = 1;} break;
			case 0x4:{RxPol = 2;} break;
			case 0x8:{RxPol = 3;} break;
	 }

	 switch(RxAntDec)
	 {
			case 0x1:{RxAnt = 0;} break;
			case 0x2:{RxAnt = 1;} break;
	 }

	 *((short *)(DataPtr)) = (RxAnt<<4)|(RxPol<<2)|(TxPol<<0);
	 DataPtr += sizeof(short);//70

	 *((short *)(DataPtr)) = formSettings->seMLEN->Value;
	 DataPtr += sizeof(short);//72

	 //SendCmdPacket(BuffSize, Buffer, 0x0001, CurrentTxPacketNum);
	 delete [] Buffer;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::bPlotClick(TObject *Sender)
{
	 unsigned short BuffSize = 4;
	 unsigned char *Buffer = new unsigned char[BuffSize];
	 unsigned char *DataPtr = Buffer;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::mmRasterShowClick(TObject *Sender)
{
	 int lo = formRasterSettings->UpDownX1->Position;
	 int hi = formRasterSettings->UpDownX2->Position;
	 // ��������� ��������� ��������
	 if(formTargetDraw) return;
				formTargetDraw = new TformTargetDraw(this);
				formTargetDraw->Chart1->LeftAxis->Minimum = 3e8*Td*lo/2; // ��������� � ������
				formTargetDraw->Chart1->LeftAxis->Maximum = 3e8*Td*(hi+1)/2;
				formTargetDraw->Chart1->BottomAxis->Minimum = 0;
				formTargetDraw->Chart1->BottomAxis->Maximum = leScanSector->Text.ToDouble();
				formTargetDraw->Chart1->LeftAxis->Title->Caption = "���������, �";
				formTargetDraw->Chart1->BottomAxis->Title->Caption = "����, ����";
				formTargetDraw->Series1->Clear();
				formTargetDraw->Series2->Clear();
				formTargetDraw->Series3->Clear();
	 // ��������� ��������� ���������� ����������� ��
	 if(formRasterDrawXX) return;
				formRasterDrawXX = new TformRaster(this);
                                formRasterDrawXX->Raster1->SetType(formRasterSettings->chXXparam->Checked);
				formRasterDrawXX->Raster1->SetLimits(-180, 60, 179, 600);
				formRasterDrawXX->Raster1->NameX = "�������� ���, ����";
                                if(formRasterSettings->chXXparam->Checked)
        				formRasterDrawXX->Raster1->NameY = "���������, �������";
                                else
        				formRasterDrawXX->Raster1->NameY = "���������, �����";
				formRasterDrawXX->Raster1->Title = "�������������� ����������� XX";
				//formRasterDrawXX->Table->LoClr = clGreen;
				//formRasterDrawXX->Table->HiClr = clMaroon<<16;
				formRasterDrawXX->Table->Resize(540, 360);
                                /*formRasterDrawXX->Raster1->WriteLineY(MinUpDown->Position,1);
                                formRasterDrawXX->Raster1->WriteLineY(MaxUpDown->Position,2);*/

	 // ��������� ��������� ���������� ����������� ��
	 if(formRasterDrawYY) return;
				formRasterDrawYY = new TformRaster(this);
                                formRasterDrawYY->Raster1->SetType(formRasterSettings->chYYparam->Checked);
				formRasterDrawYY->Raster1->SetLimits(-180, 60, 179, 600);
				formRasterDrawYY->Raster1->NameX = "�������� ���, ����";
                                if(formRasterSettings->chYYparam->Checked)
        				formRasterDrawYY->Raster1->NameY = "���������, �������";
                                else
        				formRasterDrawYY->Raster1->NameY = "���������, �����";
				formRasterDrawYY->Raster1->Title = "������������ ����������� YY";
				//formRasterDrawYY->Table->LoClr = clGreen;
				//formRasterDrawYY->Table->HiClr = clMaroon<<16;
				formRasterDrawYY->Table->Resize(540, 360);
                                /*formRasterDrawYY->Raster1->WriteLineY(MinUpDown->Position,1);
                                formRasterDrawYY->Raster1->WriteLineY(MaxUpDown->Position,2);*/
/*
	 formRasterDraw->Raster1->SetLimits(0, 0, 360, 500);
	 formRasterDraw->Table->LoClr = clGreen;
	 formRasterDraw->Table->HiClr = clMaroon<<16;
	 formRasterDraw->Raster1->Plot();
	 formRasterDraw->Table->Resize(100, 100);
	 for(int i=0; i<100; i++)
			for(int j=0; j<100; j++)
				 formRasterDraw->Table->Data[i][j] = i*j*1000;
	 formRasterDraw->Raster1->Plot();
*/
}
//---------------------------------------------------------------------------

void __fastcall TformMain::Action1Execute(TObject *Sender)
{
	 int lo, hi;
	 // ��������� ����������� ��� ����������-�������� ������
	 if(rbRasterAF->Checked)
	 {
			// ��������� ���������� ��
			lo = formRasterSettings->UpDownX1->Position;
			hi = formRasterSettings->UpDownX2->Position;
                        formRasterDrawXX->Raster1->SetType(formRasterSettings->chXXparam->Checked);
			formRasterDrawXX->Raster1->SetLimits(-180, lo, 179, hi+1);
			formRasterDrawXX->Raster1->NameX = "�������� ���, ����";
			formRasterDrawXX->Table->Resize(hi-lo+1, 360);
			if(formTargetDraw) // ��������� ��������� � ����������� �� ������
			{
				formTargetDraw->Chart1->LeftAxis->Minimum = 3e8*Td*lo/2; // ��������� � ������
				formTargetDraw->Chart1->LeftAxis->Maximum = 3e8*Td*(hi+1)/2;
				formTargetDraw->Chart1->LeftAxis->Title->Caption = "���������, �";
				double bearingCoef;
				switch(RadioGroup1->ItemIndex)
				{
					case 1:
						// ������ ������ ������������ ��������� ���� ��� ����������
						// ������ ������� ������� �� ��
						bearingCoef = formSettings->leFreq->Text.ToDouble() + formSettings->leGeterodin->Text.ToDouble();
						// �������� � ����������� �������������� ������������� ��� �������� ��� � ��������
						bearingCoef *= 2*formMeasureSettings->BearingBase/3e2; // ������� � ���, ������� ������� �� 300
						// �������� � ������������ ���� ����������
						bearingCoef = RAD*asin(1.0/bearingCoef);
						// ��������� ��������� ����� �������
						formTargetDraw->Series1->Active = 1;
						formTargetDraw->Series2->Active = 1;
						formTargetDraw->Series2->Active = 1;
						formTargetDraw->Chart1->Legend->Visible = 1;
						formTargetDraw->Chart1->BottomAxis->Minimum = -bearingCoef;
						formTargetDraw->Chart1->BottomAxis->Maximum = bearingCoef;
						formTargetDraw->Chart1->BottomAxis->Title->Caption = "����������, ����";
						break;
					default:
						formTargetDraw->Series1->Active = 0;
						formTargetDraw->Series2->Active = 1;
						formTargetDraw->Series2->Active = 1;
						formTargetDraw->Chart1->Legend->Visible = 1;
						formTargetDraw->Chart1->BottomAxis->Minimum = 0;
						formTargetDraw->Chart1->BottomAxis->Maximum = leScanSector->Text.ToDouble();
						formTargetDraw->Chart1->BottomAxis->Title->Caption = "����, ����";
				}
				formTargetDraw->Series1->Clear();
				formTargetDraw->Series2->Clear();
				formTargetDraw->Series3->Clear();
			}
			// ��������� ����� ��������� ���������� YY
			lo = formRasterSettings->UpDownY1->Position;
			hi = formRasterSettings->UpDownY2->Position;
                        formRasterDrawYY->Raster1->SetType(formRasterSettings->chYYparam->Checked);
			formRasterDrawYY->Raster1->SetLimits(-180, lo, 179, hi+1);
			formRasterDrawYY->Raster1->NameX = "�������� ���, ����";
			formRasterDrawYY->Table->Resize(hi-lo+1, 360);
			// ������� �����������
                        /*

                        */
			RasterClean();
	 }
	 // ��������� ����������� ��� ������������ � �������� ������
	 if(rbRasterA->Checked || rbRasterF->Checked)
	 {
			lo = formRasterSettings->UpDownX1->Position;
			hi = formRasterSettings->UpDownX2->Position;
			double Max;
			double bearingCoef;
			switch(RadioGroup1->ItemIndex)
			{
				case 1: // �������������� ����� - ����������� �������� ���
					Max = leNumberOfMultOsc->Text.ToIntDef(750);
					formRasterDrawXX->Raster1->NameX = "����������";
					formRasterDrawYY->Raster1->NameX = "����������";
					// ������ ������ ������������ ��������� ���� ��� ����������
					// ������ ������� ������� �� ��
					bearingCoef = formSettings->leFreq->Text.ToDouble() + formSettings->leGeterodin->Text.ToDouble();
					// �������� � ����������� �������������� ������������� ��� �������� ��� � ��������
					bearingCoef *= 2*formMeasureSettings->BearingBase/3e2; // ������� � ���, ������� ������� �� 300
					// �������� � ������������ ���� ����������
					bearingCoef = RAD*asin(1.0/bearingCoef);

					formTargetDraw->Series1->Active = 0;
					formTargetDraw->Series2->Active = 1;
					formTargetDraw->Series2->Active = 1;
					formTargetDraw->Chart1->Legend->Visible = 1;
					formTargetDraw->Chart1->BottomAxis->Minimum = -bearingCoef;
					formTargetDraw->Chart1->BottomAxis->Maximum = bearingCoef;
					formTargetDraw->Chart1->BottomAxis->Title->Caption = "����������, ����";
					break;
				default: // ����� ������������ - ����������� ������� � ��������
					Max = leScanSector->Text.ToDouble()/(1-1./leNumberOfMultOsc->Text.ToIntDef(750));
					formRasterDrawXX->Raster1->NameX = "����, ����";
					formRasterDrawYY->Raster1->NameX = "����, ����";
					formTargetDraw->Series1->Active = 0;
					formTargetDraw->Series2->Active = 1;
					formTargetDraw->Series2->Active = 1;
					formTargetDraw->Chart1->Legend->Visible = 1;
					formTargetDraw->Chart1->BottomAxis->Minimum = 0;
					formTargetDraw->Chart1->BottomAxis->Maximum = leScanSector->Text.ToDouble();
					formTargetDraw->Chart1->BottomAxis->Title->Caption = "����, ����";
			}
			// ��������� ����� ��������� ���������� ��
                        formRasterDrawXX->Raster1->SetType(formRasterSettings->chXXparam->Checked);
			formRasterDrawXX->Raster1->SetLimits(0, lo, Max, hi+1);
			formRasterDrawXX->Table->Resize(hi-lo+1, leNumberOfMultOsc->Text.ToIntDef(750));
			if(formTargetDraw) // ��������� ����� ��������� ��������
			{
				formTargetDraw->Chart1->LeftAxis->Minimum = 3e8*Td*lo/2; // ����������� ��������� � ������
				formTargetDraw->Chart1->LeftAxis->Maximum = 3e8*Td*(hi+1)/2; // ������������ ��������� � ������
//        formTargetDraw->Chart1->LeftAxis->Title->Caption = "���������, �";
				formTargetDraw->Series1->Clear();
				formTargetDraw->Series2->Clear();
				formTargetDraw->Series3->Clear();
			}
			// ��������� ����� ��������� ���������� ��
			lo = formRasterSettings->UpDownY1->Position;
			hi = formRasterSettings->UpDownY2->Position;
                        formRasterDrawYY->Raster1->SetType(formRasterSettings->chYYparam->Checked);
			formRasterDrawYY->Raster1->SetLimits(0, lo, Max, hi+1);
			formRasterDrawYY->Table->Resize(hi-lo+1, leNumberOfMultOsc->Text.ToIntDef(750));
			// ������� �������
			RasterClean();
	 }
		formRasterDrawXX->Raster1->Plot();
		formRasterDrawYY->Raster1->Plot();
	 // ��������� ���� ���������� ��������
	 if(ThresholdForm)
	 {
				ThresholdForm->UpDown2->Max = formRasterDrawXX->Table->Row;
				ThresholdForm->UpDown3->Max = formRasterDrawXX->Table->Row;
	 }
}
//---------------------------------------------------------------------------
void __fastcall TformMain::Threshold1Click(TObject *Sender)
{
	if(!ThresholdForm) ThresholdForm = new TThresholdForm(this);
	ThresholdForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::RasterSetting1Click(TObject *Sender)
{
	if(!formRasterSettings) formRasterSettings = new TformRasterSettings(this);
	formRasterSettings->Show();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::ReadFile1Click(TObject *Sender)
{
// �������� ������ �� �����
	OpenDialog1->FileName = "";
	OpenDialog1->Options << ofAllowMultiSelect;
	OpenDialog1->Filter ="���� � ������� (bin)|*.BIN|���� dat|*.DAT";
	//timeKillEvent(mmFileReadTimer);
        FileReadTimer->Enabled = false;
        timeKillEvent(mmPlotTimer);
	mmFileReadTimer = 0;
	if(OpenDialog1->Execute())
	{
		FileCounter = 0;
		bool Message = !(OpenDialog1->Files->Count > 1);
		while(FileCounter < OpenDialog1->Files->Count)
		{
				if(InitFileRead(OpenDialog1->Files->Strings[FileCounter], Message))
				{
						// ��������� � ������ �������, ������������ ������ �� �������
						float FileReadTimerIntervalF = leMulOscDelay->Text.ToInt()/(32*formSettings->leSubBufNum->Text.ToInt());
						if(FileReadTimerIntervalF < 1) FileReadTimerInterval = 1;
						else FileReadTimerInterval = (int) FileReadTimerIntervalF;
                                                FileReadTimerInterval = 4;
                                                FileReadTimer->Enabled = true;
						//mmFileReadTimer = timeSetEvent(FileReadTimerInterval,0,TimerProc,NULL,TIME_PERIODIC);
						Action1Execute(this);
                                                cleanView();
						RasterClean();
                                                FileReadTimerProc();
						break;
				}
				FileCounter++;
		}
	}
	if(FileCounter >= OpenDialog1->Files->Count) ShowMessage("��� ������, ��������� ��� ������");
        //mmPlotTimer = timeSetEvent(60,0,mmPlotTimerProc,NULL,TIME_PERIODIC);
}


void CALLBACK TimerProc(unsigned int uID, unsigned int uMsg, DWORD dwUser, DWORD dw1, DWORD dw2){
				try {
					formMain->FileReadTimerProc();
				}
				catch (Exception &exception) {
								ShowMessage("������ � �������!");
				}
}
int PlotDrawParamPlot1 = 0;
double *PlotDrawParamDataBuf1;
int PlotDrawParamVarBufSize1;
int PlotDrawParamPlot2 = 0;
double *PlotDrawParamDataBuf2;
int PlotDrawParamVarBufSize2;
int PlotDrawParamPlot3 = 0;
double *PlotDrawParamDataBuf3;
int PlotDrawParamVarBufSize3;
void __fastcall TformMain::PlotDrawParam(int type, double *DataBuf, int VarBufSize){
	switch(type){
		case 1: 
					PlotDrawParamPlot1 = 1;
					PlotDrawParamDataBuf1 = DataBuf;
					PlotDrawParamVarBufSize1 = VarBufSize;
					break;
		case 2: 
					PlotDrawParamPlot2 = 1;
					PlotDrawParamDataBuf2 = DataBuf;
					PlotDrawParamVarBufSize2 = VarBufSize;
					break;
		case 3: 
					PlotDrawParamPlot3 = 1;
					PlotDrawParamDataBuf3 = DataBuf;
					PlotDrawParamVarBufSize3 = VarBufSize;
					break;
	}
		//���� ����� �������� ����, ������� ����� ���������� FileReadTimerTimer
		//1 - DrawOsc
		//2 - DrawSp
		//3 - DrawOscCoherentAccum
}
//int FileReadTimerTimerInt = 0;
void __fastcall TformMain::FileReadTimerTimer(TObject *Sender)
{
        //FileReadTimer->Enabled = false;
        for(int i = 0; i < PacketCount; ++i){
                if(FileReadTimer->Enabled){
                        FileReadTimerProc();
                }
                else {
                        break;
                }
        }
        //FileReadTimerProc();
        
        //FileReadTimerTimerInt++;
        //Memo->Lines->Add(IntToStr(FileReadTimerTimerInt));
	/*if(PlotDrawParamPlot1==1){
                PlotDrawParamPlot3 = 0;
		formOscDraw->DrawOsc(PlotDrawParamDataBuf1, PlotDrawParamVarBufSize1);
	}
	if(PlotDrawParamPlot2==1){
                PlotDrawParamPlot3 = 0;
		formSpDraw->DrawSp(PlotDrawParamDataBuf2, PlotDrawParamVarBufSize2);
	}
	if(PlotDrawParamPlot3==1){
                PlotDrawParamPlot3 = 0;
		formExpDraw->DrawOscCoherentAccum(PlotDrawParamDataBuf3, PlotDrawParamVarBufSize3);
	}*/
	// � ���� ������ ������� ����������� ���������
				/*if(formRasterDrawXX && formRasterDrawXX->WindowState != wsMinimized) formRasterDrawXX->Raster1->Plot();
				if(formRasterDrawYY && formRasterDrawYY->WindowState != wsMinimized) formRasterDrawYY->Raster1->Plot();
                                */
}

//---------------------------------------------------------------------------
unsigned int FileReadTimerProcCnt = 0;
void __fastcall TformMain::FileReadTimerProc()
{
        //unsigned int sleepTimer = 1;
        //Sleep(sleepTimer);
        //Memo->Lines->Add("FileReadTimerProc");
        FileReadTimerProcCnt++;
        if(FileReadTimerProcCnt > 20){
                //Memo->Lines->Append("\n"+IntToStr(FileBufNum));
                FileReadTimerProcCnt = 0;
                PlotTimerTimer(this);
                //sleepTimer = 10;
                //Sleep(sleepTimer);
        }
	unsigned int MaxSubBufCount;
	switch(FileReadMode)
	{
		case 0x0007:
			MaxSubBufCount = formSettings->leSubBufNum->Text.ToInt()*sizeof(int);
			break;
		default: MaxSubBufCount = formSettings->leSubBufNum->Text.ToInt();
	}
	if(FileResponseMemory)
	{
		// �������� ��������� ������� ������ ��� ������ �� �����
		if(!FileBuffer)
		{
			FileBuffer = new short int [1024+12];
		}
                if(!FileBufferFind)
                        FileBufferFind = new short int [PacketCount*(1024+112)];
                memset(FileBuffer, 0, 1024*2+24);
		// ���������� �������� ���������
		memcpy(FileBuffer, &FileBufNum, 4);
		memcpy(FileBuffer+2, &FileVarNum, 4);
		memcpy(FileBuffer+4, &FileSubBufNum, 4);
		try
		{
			// ������ �� ������ ������
                        if(FileVarNum == 0){
                                memset(FileBufferFind, 0, PacketCount*(1024+112));
			        unsigned int BufSize = FileBufNum*(112+FileReadBlockSize*PacketCount*2);
                                FileResponseMemory->Position = (__int64) (BufSize);
                                FileResponseMemory->Read(FileBufferFind, FileReadBlockSize*PacketCount*2+112);
                        }
			unsigned int CurBytePos = FileVarNum*FileReadBlockSize;
                        memcpy(FileBuffer+6,FileBufferFind+56+CurBytePos,FileReadBlockSize*2);
		}
		// ���� ���������� ���������� ��� ������, ����������� ������
		catch(Exception &exception)
		{
			//timeKillEvent(mmFileReadTimer);
			mmFileReadTimer = 0;
			FileReadTimer->Enabled = false;
			if(FileResponseMemory) {delete FileResponseMemory; FileResponseMemory = NULL;}
			if(FileBuffer) {delete[] FileBuffer; FileBuffer = NULL;}
			ShowMessage("������ ������!");
                        /*if(!mmPlotTimer)
                            mmPlotTimer = timeSetEvent(60,0,mmPlotTimerProc,NULL,TIME_PERIODIC);*/
			return;
		}
		// ������� ���������� ������
                try {
		        ProcessAnswer(FileBuffer, FileReadMode);
                }
                catch(Exception &exception) {
                      mmFileReadTimer = 0;
                }

		// ��������� ���������
		/*++FileSubBufNum;
		if(FileSubBufNum >= MaxSubBufCount)
		{*/
			FileSubBufNum = 0;
			FileVarNum++;
                        if(FileVarNum == PacketCount-1) FileSubBufNum = 1;
			if(FileVarNum >= PacketCount)
			{
				FileVarNum = 0;
				FileBufNum++;
				if(FileBufNum >= (unsigned)(formMain->leNumberOfMultOsc->Text.ToInt()))
				{ // ����� ������
						//timeKillEvent(mmFileReadTimer);
						mmFileReadTimer = 0;
						FileReadTimer->Enabled = false;
						if(FileResponseMemory) {delete FileResponseMemory; FileResponseMemory = NULL;}
						if(FileBuffer) {delete[] FileBuffer; FileBuffer = NULL;}
						//  ������������� ���������� ����� �� ������
						FileCounter++;
						bool Message = !(OpenDialog1->Files->Count > 1);
						while(FileCounter < OpenDialog1->Files->Count)
						{
								if(InitFileRead(OpenDialog1->Files->Strings[FileCounter], Message))
								{
										// ��������� � ������ �������, ������������ ������ �� �������
//                    FileReadTimer->Interval = leNumberOfMultOsc->Text.ToInt()/(32*formSettings->leSubBufNum->Text.ToInt());
										float FileReadTimerIntervalF = leMulOscDelay->Text.ToDouble()/(32*formSettings->leSubBufNum->Text.ToInt());
										if(FileReadTimerIntervalF < 1) FileReadTimerInterval = 1;
										else FileReadTimerInterval = (int) FileReadTimerIntervalF;
										FileReadTimer->Enabled = true;
                                                                                FileReadTimerInterval = 4;
                                                                                cleanView();
										RasterClean();
                                                                                FileReadTimerProc();
										break;
								}
								FileCounter++;
						}
                                                /*if(!FileReadTimer->Enabled){
                                                if(!mmPlotTimer)
                                                        mmPlotTimer = timeSetEvent(60,0,mmPlotTimerProc,NULL,TIME_PERIODIC);
                                                }*/
                                                return;
				}// else FileReadTimerProc();
			}//else FileReadTimerProc();
                //}//else FileReadTimerProc();

	}
	else {
	  FileReadTimer->Enabled = false;
          /*AnsiString logMsg = "timeSetEvent";
          Log(logMsg);*/
          /*if(!mmPlotTimer)
                mmPlotTimer = timeSetEvent(60,0,mmPlotTimerProc,NULL,TIME_PERIODIC);*/
	  //timeKillEvent(mmFileReadTimer);
          mmFileReadTimer = 0;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TformMain::InitFileRead(AnsiString FileName, bool Message)
{
		// ����������� ���� ������������ ������ ��� ���������
		AnsiString Dir = ExtractFileDir(FileName);
		if(Dir.AnsiPos("\\avg_pr")!= 0) FileReadMode = 0x7;
		else FileReadMode =  0x6;

		// �������� ������� ����� ��������
		AnsiString Name = ExtractFileName(FileName);
		Dir = ExtractFilePath(Application->ExeName);
		Dir = Dir + "DATA\\ini\\"+ChangeFileExt(Name, ".ini");
                FileReadType = 0;
		if(!FileExists(Dir))
		{
                        FileReadType = 1;
                        // ���������� ������ ����� � ������.

			/*// ���� �������� ��� - �����
			if(Message) ShowMessage("��� ����� ��������");
			return false; */
		}
		else formSettings->LoadConfig("DATA\\ini\\"+ChangeFileExt(Name, ".ini"));


		if(FileResponseMemory) delete FileResponseMemory;
		FileResponseMemory = new TMemoryStream();
		FileResponseMemory->LoadFromFile(FileName);
		__int64 Size = FileResponseMemory->Size;
		// ��������� ������� ��� ������ �� �����, ����� ���������
		FileBuffer = new short int [1024+12];
                FileBufferFind = new short int [PacketCount*(1024+112)];
                int FileReadBSize;
                if(FileReadType == 1) {
                        int *FileBufferTmp = new int[112];
                        FileReadBlockSize = 0;
                        FileResponseMemory->Position = (__int64) (0);
			FileResponseMemory->Read(FileBufferTmp, 112);
                        FileReadBlockSize = FileBufferTmp[13];
                        int bSize = 2*(FileReadBlockSize*PacketCount+112);
                        FileReadBSize = Size/bSize;
                        AnsiString FileReadBSizeStr = IntToStr(FileReadBSize);
                        SettingsUnitForm->lePacketNumber->Text = FileReadBSizeStr;
                        lePacketNumberMain = FileReadBSize+128;
                        unsigned char *CmdPtr = (unsigned char *)FileBuffer;
                        //FileReadBSize = (CmdPtr[8]) | ((CmdPtr[9]) << 8) | ((CmdPtr[10]) << 16) | ((CmdPtr[11]) << 24);
                        delete [] FileBufferTmp;
                }
		memset(FileBuffer, 0, (1024+12)*2);
		FileSubBufNum = 0;
		FileVarNum = 0;
		FileBufNum = 0;
                FileReadBSize = 0;

		// ���������� ������� ��� �������� ������
                if(IndicatorXX) delete[] IndicatorXX;
	        if(IndicatorYY) delete[] IndicatorYY;
	        int size = 1024*leNumberOfMultOsc->Text.ToInt();
                if(lePacketNumberMain*1024 > size) {
                   leNumberOfMultOsc->Text = lePacketNumberMain;
                   size = lePacketNumberMain*1024;
                }

	        IndicatorXX = new double[size];
	        IndicatorYY = new double[size];
		memset(IndicatorXX, 0, size*sizeof(double));
		memset(IndicatorYY, 0, size*sizeof(double));

		if(RadioGroup1->ItemIndex == 0) CurrentAngle = 0;
		else CurrentAngle = leScanSector->Text.ToDouble()/2;

		return true;
}
//---------------------------------------------------------------------------

void __fastcall TformMain::MeasureSettings1Click(TObject *Sender)
{
	if(formMeasureSettings) formMeasureSettings->Show();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::ApplyButtonClick(TObject *Sender)
{
	int port;
	if(TryStrToInt(PortEdit->Text, port))
	{
		IdUDPClient1->Host = AddressEdit->Text;
		IdUDPClient1->Port = port;
	}
	else ShowMessage("�������� ����");
}
//---------------------------------------------------------------------------
bool __fastcall TformMain::PrepareScan(double Sector, double Time)
{
	CurrentAngle = 0;
	if((Sector/Time) > 15) return false;
	AnsiString sSector = FormatFloat("000", round(Sector));
	AnsiString sTime = FormatFloat("000", round(Time));
//  IdUDPClient1->Send("X000");
	IdUDPClient1->Send("U"+sSector);
	IdUDPClient1->Send("T"+sTime);
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::StartScan()
{
	IdUDPClient1->Send("P");
}
//---------------------------------------------------------------------------
void __fastcall TformMain::SetPosition(double Angle, double Speed)
{
	AnsiString sAngle = FormatFloat("00", Speed);
	IdUDPClient1->Send("S"+sAngle);
	sAngle = FormatFloat("000.00", Angle);
	IdUDPClient1->Send("X"+sAngle);
}
//---------------------------------------------------------------------------
// ���� �� UDP ������ �� ����������� � ��������� ���������
void __fastcall TformMain::IdUDPServer1UDPRead(TObject *Sender,
			TStream *AData, TIdSocketHandle *ABinding)
{
	AnsiString String;
	String = ((TStringStream*)AData)->DataString.SubString(0, AData->Size);
	// ������ ������ �� ����������� ���������
	int offset = 0, size = String.Length()+1, res;
	char *data = new char[size];
	double value;
	memcpy(data, String.c_str(), size-1); data[size-1] = 0;
	while(offset < size)
	{
		res = sscanf(&data[offset],"%lf", &value);
		if(res > 0) break;
		else if(res == EOF) offset = size;
		offset++;
	}
	if(offset < size)
	{
		PlatformAngle = value;
		Label1->Caption = FormatFloat("###0.##", value);
	}
	// ���������� ������������ � ��
	if(AutoCheckBox->Checked)
	{
		if(RadioGroup1->ItemIndex == 0) // ���� ��������� ������������ ������������
		{                               // ������������� � �������������� �����
			 RadioGroup1->ItemIndex = 1;
			 rbRasterAF->Checked = 1; // ����������� � ��
			 leNumberOfMultOsc->Text = leRasterPeriod->Text; // ����� ����� ����� ����� ���������� - 1 ���������;
		}
		//bContView32AvgClick(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TformMain::Button4Click(TObject *Sender)
{
	double value;
	if(LabeledEdit1->Text != "")
	{
		if(TryStrToFloat(LabeledEdit1->Text, value))
		{
			SetPosition(value,MaxSpeed);
		}
	}
}
//---------------------------------------------------------------------------
// ������ ��������� ������� �������
void __fastcall TformMain::Button6Click(TObject *Sender)
{
	IdUDPClient1->Send("A");
}
//---------------------------------------------------------------------------


//int FileReadTimerTimerInt = 0;
void __fastcall TformMain::PlotTimerTimer(TObject *Sender)
{
        //FileReadTimerTimerInt++;
        //Memo->Lines->Add("PlotTimerTimer");
        FileReadTimerProcCnt = 0;
        //Memo->Lines->Add(IntToStr(FileReadTimerTimerInt));
	if(PlotDrawParamPlot1==1){
                PlotDrawParamPlot1 = 0;
		formOscDraw->DrawOsc(PlotDrawParamDataBuf1, PlotDrawParamVarBufSize1);
	}
	if(PlotDrawParamPlot2==1){
                PlotDrawParamPlot2 = 0;
		formSpDraw->DrawSp(PlotDrawParamDataBuf2, PlotDrawParamVarBufSize2);
	}
	if(PlotDrawParamPlot3==1){
                PlotDrawParamPlot3 = 0;
		formExpDraw->DrawOscCoherentAccum(PlotDrawParamDataBuf3, PlotDrawParamVarBufSize3);
	}
	// � ���� ������ ������� ����������� ���������
	if(formRasterDrawXX && formRasterDrawXX->WindowState != wsMinimized) formRasterDrawXX->Raster1->Plot();
	if(formRasterDrawYY && formRasterDrawYY->WindowState != wsMinimized) formRasterDrawYY->Raster1->Plot();

}
//---------------------------------------------------------------------------

void __fastcall TformMain::N8Click(TObject *Sender)
{
        FormControlOptions->Show();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::N10Click(TObject *Sender)
{
        FormOrgM->Show();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::N11Click(TObject *Sender)
{
        FormOrgDM->Show();        
}
//---------------------------------------------------------------------------

void __fastcall TformMain::N12Click(TObject *Sender)
{
        FormOrgCM->Show();        
}
//---------------------------------------------------------------------------

void __fastcall TformMain::MinUpDownChange(TObject *Sender)
{
        MinUpDownPosition = -MinUpDown->Position;
        if(MinUpDownPosition > MaxUpDownPosition)
        {
                MinUpDown->Position = -MaxUpDownPosition;
                MinUpDownPosition = MaxUpDownPosition;
        }
        formMeasureSettings->MinUpDown->Position = MinUpDownPosition;
        if(formRasterDrawXX) {
                formRasterDrawXX->Raster1->WriteLineY(MinUpDownPosition,MaxUpDownPosition);
        }
        if(formRasterDrawYY) {
                formRasterDrawYY->Raster1->WriteLineY(MinUpDownPosition,MaxUpDownPosition);
        }
        formMeasureSettings->MinUpDownClick(Sender,(TUDBtnType)0);
        //formMeasureSettings->MinEdit->Text = FormatFloat("###0.###", MinUpDown->Position);
        MaxUpDown->SelStart = -MaxUpDownPosition;
        MaxUpDown->SelEnd = -MinUpDownPosition;
        MinUpDown->SelStart = -MaxUpDownPosition;
        MinUpDown->SelEnd = -MinUpDownPosition;
        if(MinUpDownO->Value!=MinUpDownPosition){
                MinUpDownO->Value = MinUpDownPosition;
                MinUpDownM->Text = FormatFloat("###0.###", 3e8*MinUpDownPosition*Td/2);
        }
}
//---------------------------------------------------------------------------

void __fastcall TformMain::MaxUpDownChange(TObject *Sender)
{
        MaxUpDownPosition = -MaxUpDown->Position;
        if(MaxUpDownPosition < MinUpDownPosition)
        {
                MaxUpDown->Position = -MinUpDownPosition;
                MaxUpDownPosition = MinUpDownPosition;
        }
        formMeasureSettings->MaxUpDown->Position = MaxUpDownPosition;
        if(formRasterDrawXX) {
                formRasterDrawXX->Raster1->WriteLineY(MinUpDownPosition,MaxUpDownPosition);
        }
        if(formRasterDrawYY) {
            formRasterDrawYY->Raster1->WriteLineY(MinUpDownPosition,MaxUpDownPosition);
    }
    formMeasureSettings->MaxUpDownClick(Sender,(TUDBtnType)0);
    //formMeasureSettings->MaxEdit->Text = FormatFloat("###0.###", MinUpDown->Position);
    MaxUpDown->SelStart = -MaxUpDownPosition;
    MaxUpDown->SelEnd = -MinUpDownPosition;
    MinUpDown->SelStart = -MaxUpDownPosition;
    MinUpDown->SelEnd = -MinUpDownPosition;
    if(MaxUpDownPosition!=MaxUpDownO->Value){
            MaxUpDownO->Value = MaxUpDownPosition;
            MaxUpDownM->Text = FormatFloat("###0.###", 3e8*MaxUpDownPosition*Td/2);
    }
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TformMain::ScatterMenuClick(TObject *Sender)
{
  if(!ScatterForm) ScatterForm = new TScatterForm(formMain);
}
//---------------------------------------------------------------------------


void __fastcall TformMain::Button9Click(TObject *Sender)
{
	double value;
	if(LabeledY->Text != "")
	{
		if(TryStrToFloat(LabeledY->Text, value))
		{
                        AnsiString sAngle = FormatFloat("000.00", value);
                        IdUDPClient1->Send("Y"+sAngle);
		}
	}

}

void __fastcall TformMain::mmChanelsClick(TObject *Sender)
{
        //���� ��������
        Fchanels->Show();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::N14Click(TObject *Sender)
{
    IdUDPClient3->Host = videoServer->Text;
    if(videoServerStatus){
            IdUDPClient3->Send("q");
            N14->Caption = "������";
            videoServerStatus = false;
    }
    else {
            IdUDPClient3->Send("start");
            N14->Caption = "����";
            videoServerStatus = true;
    }  
}

//---------------------------------------------------------------------------

void __fastcall TformMain::N15Click(TObject *Sender)
{
        SettingsUnitForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btnCMD9Click(TObject *Sender)
{
        SettingsUnitForm->btnCMD9Click(SettingsUnitForm);
}
//---------------------------------------------------------------------------

void __fastcall TformMain::MConClick(TObject *Sender)
{
        SettingsUnitForm->MConClick(SettingsUnitForm);
}
//---------------------------------------------------------------------------

void __fastcall TformMain::SendSettingsClick(TObject *Sender)
{
       SettingsUnitForm->BitBtn1Click(SettingsUnitForm);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::btOriginalPulseClick(TObject *Sender)
{
	 unsigned int BufSize = 1024;
         if(OriginalPulseRe) delete [] OriginalPulseRe;
         if(OriginalPulseIm) delete [] OriginalPulseIm;
         OriginalPulseRe = new double [BufSize];
         OriginalPulseIm = new double [BufSize];

	 for(unsigned int i=0; i<BufSize;i++)
	 {
			if(i>0 && i<10)
			{
				 OriginalPulseComp[i]._M_re = 128*cos(2*M_PI*i/Fd*(formSettings->leFreq->Text.ToDouble())*1e6);
				 OriginalPulseRe[i] = OriginalPulseComp[i]._M_re;
				 OriginalPulseComp[i]._M_im = 128*sin(2*M_PI*i/Fd*(formSettings->leFreq->Text.ToDouble())*1e6);
				 OriginalPulseIm[i] = OriginalPulseComp[i]._M_im;
			}
			else
			{
				 OriginalPulseComp[i] = 0;
			}
	 }

         Worker->sendData();
	 formOscDraw->DrawOscIQ(OriginalPulseComp, BufSize);
        return;
}

void __fastcall TformMain::bVarPlotClick(TObject *Sender)
{
	 unsigned short BuffSize = 3*sizeof(int);
	 unsigned char *Buffer = new unsigned char[BuffSize];
	 unsigned char *DataPtr = Buffer;

	 *((unsigned int *)(DataPtr)) = formSettings->leSubBufNum->Text.ToInt();
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = 32;
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = 1;
	 DataPtr += sizeof(int);

//	 SendCmdPacket(12, Buffer, 0x0004, CurrentTxPacketNum);
	 delete [] Buffer;
}
void __fastcall TformMain::N7Click(TObject *Sender)
{
	 Memo->Visible = !Memo->Visible;
	 Tile();   
}
void __fastcall TformMain::Button5Click(TObject *Sender)
{
	 N5Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::SpeedButton1Click(TObject *Sender)
{
//	 SendCmdPacket(0, NULL, 0x1000, CurrentTxPacketNum);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::bPRClick(TObject *Sender)
{
	 unsigned short BuffSize = 4*sizeof(int);
	 unsigned char *Buffer = new unsigned char[BuffSize];
	 unsigned char *DataPtr = Buffer;

	 *((unsigned int *)(DataPtr)) = formSettings->leSubBufNum->Text.ToInt();
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = 32;
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = leNumberOfMultOsc->Text.ToInt();
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = leMulOscDelay->Text.ToInt();
	 DataPtr += sizeof(int);

	 if(RadioGroup1->ItemIndex == 0 && !ScatterForm) // ���� ������������ ������������
	 {
		 double time = leNumberOfMultOsc->Text.ToIntDef(750)*(1e-3*leMulOscDelay->Text.ToDouble() +
		 32e-6*formSettings->lePeriod->Text.ToDouble()*formSettings->leBurstLen->Text.ToIntDef(1));
		 if(PrepareScan(leScanSector->Text.ToDouble(), time)) // ����������
			 StartScan(); // ����
		 else ShowMessage("������������ �������� ������������");
	 }
	 else CurrentAngle = leScanSector->Text.ToDouble()/2;
//	 SendCmdPacket(BuffSize, Buffer, 0x0005, CurrentTxPacketNum);
	 delete [] Buffer;

	 PulseResponseMemory = new TMemoryStream();
	 unsigned int Size = leNumberOfMultOsc->Text.ToInt()*formSettings->leSubBufNum->Text.ToInt()*PacketCount*1024;
	 PulseResponseMemory->SetSize((int)Size);
	 FrameCnt = 0;
	 LostPacketCnt = 0;
	 formProgress->TimeLeft = leNumberOfMultOsc->Text.ToInt()*leMulOscDelay->Text.ToInt()/1000 +
														leNumberOfMultOsc->Text.ToInt()*formSettings->leSubBufNum->Text.ToInt()/0.1/1024;
	 if(!ScatterForm) formProgress->ShowModal();
     else formProgress->Show();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::Button7Click(TObject *Sender)
{
	/* AnsiString PulseResponseName = Path+"DATA\\pr\\"+"test.dat";
	 //TFileStream *ReadFile = new TFileStream(PulseResponseName, fmOpenRead);

	 TMemoryStream *ReadData = new TMemoryStream();
	 ReadData->LoadFromFile(PulseResponseName);

	 unsigned int numpts = ReadData->Seek(0, soFromEnd);

	 formOscDraw->cPlot->Clear();
	 formOscDraw->cPlot->FillSampleValues(numpts);

	 ms->Position = ms->Seek(0, soFromBeginning);
	 for (unsigned int i=0; i < numpts; i++)
	 {
			ReadData->Read(&cd, numbytes);
			Series1->XValues->Value[i] = cd.x;
			Series1->YValues->Value[i] = cd.y;
	 }*/

}
//---------------------------------------------------------------------------
void __fastcall TformMain::Button8Click(TObject *Sender)
{
         return;
	 unsigned short BuffSize = 4*sizeof(int);
	 unsigned char *Buffer = new unsigned char[BuffSize];
	 unsigned char *DataPtr = Buffer;

	 *((unsigned int *)(DataPtr)) = formSettings->leSubBufNum->Text.ToInt();
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = 32;
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = leNumberOfMultOsc->Text.ToInt();
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = leMulOscDelay->Text.ToInt();
	 DataPtr += sizeof(int);

//	 SendCmdPacket(BuffSize, Buffer, 0x0006, CurrentTxPacketNum);
	 WaitingAck = true;
	 RestartTimer(TimerAck);

	 delete [] Buffer;
}


void __fastcall TformMain::bAvgClick(TObject *Sender)
{
	 unsigned short BuffSize = 4;
	 unsigned char *Buffer = new unsigned char[BuffSize];
	 unsigned char *DataPtr = Buffer;

	 *((unsigned int *)(DataPtr)) = formSettings->leSubBufNum->Text.ToInt()*sizeof(int);//AVG_DATA_LEN/MAX_PKT_DATA_LEN;
//	 SendCmdPacket(4, Buffer, 0x0008, CurrentTxPacketNum);
	 delete [] Buffer;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::bAvgVarPlotClick(TObject *Sender)
{
	 unsigned short BuffSize = 3*sizeof(int);
	 unsigned char *Buffer = new unsigned char[BuffSize];
	 unsigned char *DataPtr = Buffer;

	 *((unsigned int *)(DataPtr)) = formSettings->leSubBufNum->Text.ToInt()*sizeof(int);
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = 32;
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = 1;
	 DataPtr += sizeof(int);

//	 SendCmdPacket(12, Buffer, 0x0009, CurrentTxPacketNum);
	 delete [] Buffer;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::bAvgPRClick(TObject *Sender)
{
	 unsigned short BuffSize = 4*sizeof(int);
	 unsigned char *Buffer = new unsigned char[BuffSize];
	 unsigned char *DataPtr = Buffer;

	 *((unsigned int *)(DataPtr)) = formSettings->leSubBufNum->Text.ToInt()*sizeof(int);
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = 32;
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = leNumberOfMultOsc->Text.ToInt();
	 DataPtr += sizeof(int);
	 *((unsigned int *)(DataPtr)) = leMulOscDelay->Text.ToInt();
	 DataPtr += sizeof(int);

	 if(RadioGroup1->ItemIndex == 0) // ���� ������������ ������������
	 {
		 double time = leNumberOfMultOsc->Text.ToIntDef(750)*(1e-3*leMulOscDelay->Text.ToDouble() +
		 32e-6*formSettings->lePeriod->Text.ToDouble()*formSettings->leBurstLen->Text.ToIntDef(1));
		 if(PrepareScan(leScanSector->Text.ToDouble(), time)) // ����������
			 StartScan(); // ����
		 else ShowMessage("������������ �������� ������������");
	 }
	 else CurrentAngle = leScanSector->Text.ToDouble()/2;

///	 SendCmdPacket(BuffSize, Buffer, 0x000B, CurrentTxPacketNum);
	 delete [] Buffer;

	 PulseResponseMemory = new TMemoryStream();
	 unsigned int Size = leNumberOfMultOsc->Text.ToInt()*formSettings->leSubBufNum->Text.ToInt()*PacketCount*sizeof(int)*1024;
	 PulseResponseMemory->SetSize((int)Size);
	 FrameCnt = 0;
	 LostPacketCnt = 0;
	 formProgress->TimeLeft = leNumberOfMultOsc->Text.ToInt()*leMulOscDelay->Text.ToInt()/1000 + leNumberOfMultOsc->Text.ToInt()*formSettings->leSubBufNum->Text.ToInt()*4/0.085/1024;
	 formProgress->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::leNumberOfMultOscChange(TObject *Sender)
{
        if(SettingsUnitForm->lePacketNumber->Text != leNumberOfMultOsc->Text)
                SettingsUnitForm->lePacketNumber->Text = leNumberOfMultOsc->Text;
        lePacketNumberMain = SettingsUnitForm->lePacketNumber->Text.ToInt()+128;
}
//---------------------------------------------------------------------------

