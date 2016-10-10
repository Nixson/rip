//---------------------------------------------------------------------------

#ifndef WorkerH
#define WorkerH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#define bufferSizeDouble 65536
//---------------------------------------------------------------------------
class TWorker : public TThread
{            
private:
protected:
        void __fastcall Execute();
        void __fastcall copyBuffer();
        void __fastcall copyResult();
public:
        __fastcall TWorker(bool CreateSuspended);
        void __fastcall MyCorrelation(double* in, int dataSize, double* kernel, int kernelSize, double* out);
        void __fastcall Math1(unsigned int BufSize, double *DataBuf);
        void __fastcall MathAndPlot(int BufSize, int CurBufNum);
        void __fastcall ProcessAnswer(short int *Buffer, unsigned int CmdNum);
        double a0XXsv0[bufferSizeDouble], a1XXsv0[bufferSizeDouble], a0YYsv0[bufferSizeDouble], a1YYsv0[bufferSizeDouble];
        double a0XXsv1[bufferSizeDouble], a1XXsv1[bufferSizeDouble], a0YYsv1[bufferSizeDouble], a1YYsv1[bufferSizeDouble];
        double ResXXRe[bufferSizeDouble], ResXXIm[bufferSizeDouble];
        double ResYYRe[bufferSizeDouble], ResYYIm[bufferSizeDouble];
        double ResXXAbs[bufferSizeDouble], ResXXAng[bufferSizeDouble];
        double ResYYAbs[bufferSizeDouble], ResYYAng[bufferSizeDouble];
        double ResXXPhase[bufferSizeDouble], ResYYPhase[bufferSizeDouble];





};
//---------------------------------------------------------------------------
#endif