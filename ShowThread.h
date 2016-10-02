//---------------------------------------------------------------------------

#ifndef ShowThreadH
#define ShowThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>


//---------------------------------------------------------------------------
class TShowThread : public TThread
{            
private:
protected:
        void __fastcall Execute();
public:
        __fastcall TShowThread(bool CreateSuspended);
        void __fastcall UpdateIndicator();
        void __fastcall CommandDone();
        void __fastcall CheckCmd();
        void __fastcall GetCmd();
        void __fastcall Cmd1(char * buf, unsigned int Bytes);
        void __fastcall Cmd2(char * buf, unsigned int Bytes);
        void __fastcall Cmd6(char * buf, unsigned int Bytes);
        void __fastcall Cmd8(char * buf, unsigned int Bytes);
        void __fastcall Cmd10(char * buf, unsigned int Bytes);
        void __fastcall Cmd4(char * buf, unsigned int Bytes);
        void __fastcall Cmd22(char * buf, unsigned int Bytes);
        void __fastcall sync1();
        void __fastcall sync2();
        void __fastcall sync6();
        void __fastcall sync8();
        void __fastcall sync10();
        void __fastcall sync4();

        void __fastcall SaveRadarDataToFile(char * buf, unsigned int Bytes);
        void __fastcall SaveGeoRadarDataToFile(unsigned char * buf, unsigned int Bytes);
        void __fastcall Only4OscLen();
};
//---------------------------------------------------------------------------
#endif
