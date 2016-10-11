
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
#include "Worker.h"
#include "formChanels.h"
#include "math.h"
#pragma package(smart_init)

#define RAD 57.295779513082320877


extern const unsigned int N;
extern double *DataBuf;
double *DataBufWorker;
extern double *OriginalPulse;
extern double *OriginalPulseRe;
extern double *OriginalPulseIm;

double *OriginalPulseReWorker;
double *OriginalPulseImWorker;

extern double *MainResXXRe, *MainResXXIm,*MainResYYRe, *MainResYYIm;
extern double *MainResXXAbs, *MainResXXAng,*MainResYYAbs, *MainResYYAng;
extern double *MainResXXPhase, *MainResYYPhase;



extern int MainPosition;
extern int WorkerPosition;
extern int WorkerPositionResponse;
extern unsigned int WorkerCmdNum;
extern short int *WorkerBuffer;

__fastcall TWorker::TWorker(bool CreateSuspended)
        : TThread(CreateSuspended)
{
        BufferOnMain = new short int [1030];
        DataBufWorker = new double [bufferSizeDouble];
        OriginalPulseReWorker = new double [1024];
        OriginalPulseImWorker = new double [1024];
}

void __fastcall TWorker::MyCorrelation(double* in, int dataSize, double* kernel, int kernelSize, double* out)
{
		int i, j, k;

		// check validity of params
		if(!in || !out || !kernel) return;
		if(dataSize <=0 || kernelSize <= 0) return;

		// start convolution from out[kernelSize-1] to out[dataSize-1] (last)
		for(i = kernelSize-1; i < dataSize; ++i)
		{
				out[i] = 0;                             // init to 0 before accumulate

				for(j = i, k = 0; k < kernelSize; --j, ++k)
						out[i] += in[j] * kernel[k];
		}

		// convolution from out[0] to out[kernelSize-2]
		for(i = 0; i < kernelSize - 1; ++i)
		{
				out[i] = 0;                             // init to 0 before sum

				for(j = i, k = 0; j >= 0; --j, ++k)
						out[i] += in[j] * kernel[k];
		}

		return;
};
//---------------------------------------------------------------------------
void __fastcall TWorker::Math1(unsigned int BufSize, double *DataBuf)
{
	 double *a0XX, *a1XX, *a0YY, *a1YY;
	 unsigned int Size = BufSize/4;

	 a0XX = DataBuf+Size*0;
	 a1XX = DataBuf+Size*1;
	 a0YY = DataBuf+Size*2;
	 a1YY = DataBuf+Size*3;

	 MyCorrelation(a0XX, Size, OriginalPulseReWorker, Size, a0XXsv0);
	 MyCorrelation(a0XX, Size, OriginalPulseImWorker, Size, a0XXsv1);
	 MyCorrelation(a1XX, Size, OriginalPulseReWorker, Size, a1XXsv0);
	 MyCorrelation(a1XX, Size, OriginalPulseImWorker, Size, a1XXsv1);
	 MyCorrelation(a0YY, Size, OriginalPulseReWorker, Size, a0YYsv0);
	 MyCorrelation(a0YY, Size, OriginalPulseImWorker, Size, a0YYsv1);
	 MyCorrelation(a1YY, Size, OriginalPulseReWorker, Size, a1YYsv0);
	 MyCorrelation(a1YY, Size, OriginalPulseImWorker, Size, a1YYsv1);

	 for(unsigned int i=0; i<Size; ++i)
	 {
			// произведение сигналов с двух антенн в режиме ХХ с комплексным сопряжением
                        // Изменил знаки, как в YY, стало лучше, но сдвиг углов между гор. и верт. поляризац. около 3 град.
			ResXXRe[i] = (a0XXsv0[i]*a1XXsv0[i] + a0XXsv1[i]*a1XXsv1[i])/1152/8;//*cos(M_PI/2);

                        // 	ResXXRe[i] = -(a0XXsv0[i]*a1XXsv0[i] + a0XXsv1[i]*a1XXsv1[i])/1152/8;//*cos(M_PI/2);
			// изменён порядок вычисления нумерации антенн 2015.01.31
			ResXXIm[i] = (a0XXsv0[i]*a1XXsv1[i] - a0XXsv1[i]*a1XXsv0[i])/1152/8;//*sin(M_PI/2);
                       // - заменил на +
                       // ResXXIm[i] = (-a0XXsv0[i]*a1XXsv1[i] - a0XXsv1[i]*a1XXsv0[i])/1152/8;//*sin(M_PI/2);
			// вычисление фазы сигнала с антенны 0 в режиме ХХ
			if((a0XXsv1[i] != 0) && (a0XXsv0[i] != 0))

				 ResXXPhase[i] = 180*(atan2(a0XXsv1[i], a0XXsv0[i])+M_PI)/M_PI;
                                 //Убрал из   ResXXPhase[i]    +M_PI для устранения фазового сдвига между каналами - не помогло
                                // ResXXPhase[i] = 180*(atan2(a0XXsv1[i], a0XXsv0[i]))/M_PI;

			if((a0YYsv1[i] != 0) && (a0YYsv0[i] != 0))
				 ResYYPhase[i] = 180*(atan2(a0YYsv1[i], a0YYsv0[i])+M_PI)/M_PI;

			// вычисление модуля и аргумента произведения сигналов в режиме ХХ
			ResXXAbs[i] = pow(ResXXRe[i]*ResXXRe[i] + ResXXIm[i]*ResXXIm[i], 0.5);
			if(ResXXAbs[i] >=1e-13) ResXXAng[i] = RAD*(atan2(ResXXIm[i], ResXXRe[i]));
			else ResXXAng[i] = 0; // аргумент от -180 до 180 градусов

			// произведение сигналов с двух антенн в режиме YY с комплексным сопряжением
			// изменён порядок вычисления нумерации антенн 2015.01.31
			// !!!!! УБРАНА ИНВЕРСИЯ ПРОИЗВЕДЕНИЯ СИГНАЛОВ АНТЕНН ДЛЯ УСТРАНЕНИЯ СИСТЕМАТИЧЕСКОЙ
			// ПОГРЕШНОСТИ РАЗНОСТИ ФАЗ В 180 ГРАДУСОВ
			ResYYRe[i] = (a0YYsv0[i]*a1YYsv0[i] + a0YYsv1[i]*a1YYsv1[i])/1152/8;
			ResYYIm[i] = (a0YYsv0[i]*a1YYsv1[i] - a0YYsv1[i]*a1YYsv0[i])/1152/8;

			// вычисление модуля и аргумента произведения сигналов в режиме YY
			ResYYAbs[i] = pow(ResYYRe[i]*ResYYRe[i] + ResYYIm[i]*ResYYIm[i], 0.5);
			if(ResYYAbs[i] >= 1e-13) ResYYAng[i] = RAD*(atan2(ResYYIm[i], ResYYRe[i]));
			else ResYYAng[i] = 0;

	 }
//   formExpDraw->DrawOscCoherentAccum(ResXXAbs, Size);
}
extern unsigned int RasterCounter;

void __fastcall TWorker::copyResult(){
        try {
                memcpy(DataBuf,DataBufWorker,1024*sizeof(double));
                memcpy(MainResXXRe+WorkerPositionResponse*1024,ResXXRe,1024*sizeof(double));
                memcpy(MainResXXIm+WorkerPositionResponse*1024,ResXXIm,1024*sizeof(double));
                memcpy(MainResYYRe+WorkerPositionResponse*1024,ResYYRe,1024*sizeof(double));
                memcpy(MainResYYIm+WorkerPositionResponse*1024,ResYYIm,1024*sizeof(double));

                memcpy(MainResXXAbs+WorkerPositionResponse*1024,ResXXAbs,1024*sizeof(double));
                memcpy(MainResYYAbs+WorkerPositionResponse*1024,ResYYAbs,1024*sizeof(double));

                memcpy(MainResXXAng+WorkerPositionResponse*1024,ResXXAng,1024*sizeof(double));
                memcpy(MainResYYAng+WorkerPositionResponse*1024,ResYYAng,1024*sizeof(double));

                memcpy(MainResXXPhase+WorkerPositionResponse*1024,ResXXPhase,1024*sizeof(double));
                memcpy(MainResYYPhase+WorkerPositionResponse*1024,ResYYPhase,1024*sizeof(double));
        }
        catch (...){}
}
void __fastcall TWorker::MathAndPlot(int BufSize, int CurBufNum)
{
		// корреляционная обработка перекрёстных поляризаций
		Math1(BufSize, DataBufWorker);
                Synchronize(copyResult);
                ++WorkerPositionResponse;
}

void __fastcall TWorker::ProcessAnswer(short int *Buffer, unsigned int CmdNum)
{

	 short int *DataPtr = Buffer;

	 switch(CmdNum)
	 {
			case 0x0006: // 32 импульса избранные на экран
			{
				 // непрерывный режим без усреднения
				 int CurBufNum = *((unsigned int *)DataPtr);
				 DataPtr+=2;
				 unsigned int CurVarNumL = *((unsigned int *)DataPtr);
				 DataPtr+=2;
				 int CurSubBufNum = *((unsigned int *)DataPtr);
				 DataPtr+=2;

				 unsigned int BufSize = 1024;
				 unsigned int VarBufSize = BufSize*4;

                                 bool summChanels = false;
                                 unsigned int CurVarNum = 33;
                                 if(CurVarNumL==0){
                                        memset(PlotBuf,0,VarBufSize*sizeof(int));
                                 }
                                 
                                 if(CurVarNumL==Fchanels->cvn0){
                                        CurVarNum = 0;
                                 }
                                 else if(CurVarNumL==Fchanels->cvn1){
                                        CurVarNum = 2;
                                 }
                                 else if(CurVarNumL==Fchanels->cvn2){
                                        CurVarNum = 1;
                                 }
                                 else if(CurVarNumL==Fchanels->cvn3){
                                        CurVarNum = 3;
                                 }
                                 else if(CurVarNumL==Fchanels->cvs0){
                                        CurVarNum = 0;
                                        summChanels = true;
                                 }
                                 else if(CurVarNumL==Fchanels->cvs1){
                                        CurVarNum = 2;
                                        summChanels = true;
                                 }
                                 else if(CurVarNumL==Fchanels->cvs2){
                                        CurVarNum = 1;
                                        summChanels = true;
                                 }
                                 else if(CurVarNumL==Fchanels->cvs3){
                                        CurVarNum = 3;
                                        summChanels = true;
                                 }
                                 else
                                        CurVarNum = 33;
                                 if(CurVarNum < 32){
                                        unsigned int position = CurVarNum*BufSize;
                                        unsigned int i = 0;
                                        for(i=0; i < 1024; ++i){
                                                PlotBuf[position+i] += *(DataPtr+i);
                                        }
                                 }
                                 if(summChanels)
                                        CurVarNum = 33;
                                 if(CurVarNum > 31)
                                        return;

				 // перенос данных в промежуточный буфер
				 //memcpy(&PlotBuf[CurVarNum*BufSize+1024*CurSubBufNum], DataPtr, 1024);

				 // если закончена текущая пачка вариантов поляризации
				 if( CurSubBufNum == 1 )
				 {
						 // преобразование типа данных осциллограммы
                                                for(unsigned int i=0; i < 1024*4; ++i) DataBufWorker[i] = PlotBuf[i];
						 //formOscDraw->Chart->LeftAxis->Automatic = false;
						 //formOscDraw->Chart->LeftAxis->SetMinMax(0, 255);
						 // отображение данных на осциллограмме и спектрограмме

						// математическая обработка и отображение на индикатор
						MathAndPlot(VarBufSize, CurBufNum);
				 }
			}break;
			default: break;
	 }
}

void __fastcall TWorker::copyBuffer(){
        try {
        memcpy(BufferOnMain,WorkerBuffer+WorkerPosition*1030,1030*2);
        memcpy(OriginalPulseReWorker,OriginalPulseRe,1024*sizeof(double));
        memcpy(OriginalPulseImWorker,OriginalPulseIm,1024*sizeof(double));

        }
        catch (...){}
}
//---------------------------------------------------------------------------
void __fastcall TWorker::Execute() {
        short int *Buffer = new short int[1030];
        while(!Terminated) {
                if(MainPosition > WorkerPosition){
                        for( ; WorkerPosition <= MainPosition; ++WorkerPosition){
                                Synchronize(copyBuffer);
                                ProcessAnswer(BufferOnMain,WorkerCmdNum);
                        }

                }
                Idglobal::Sleep(1);
        }
}
