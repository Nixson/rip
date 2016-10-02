//---------------------------------------------------------------------------

#ifndef TargetUnitH
#define TargetUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
//---------------------------------------------------------------------------
class TformTargetDraw : public TForm
{
__published:	// IDE-managed Components
        TChart *Chart1;
        TLineSeries *Series1;
        TButton *Button1;
        TLineSeries *Series2;
        TLineSeries *Series3;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TformTargetDraw(TComponent* Owner);
//        void __fastcall 
};
//---------------------------------------------------------------------------
extern PACKAGE TformTargetDraw *formTargetDraw;
//---------------------------------------------------------------------------
#endif
