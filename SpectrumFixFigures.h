//---------------------------------------------------------------------------
#ifndef SpectrumFixFiguresH
#define SpectrumFixFiguresH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "myprocs.h"
// SpectrumFixFigure States
enum {fsSpFree, fsSpZoom, fsSpSelect, fsSpLock, fsSpDragMarker, fsSpConstruct};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
struct TSpectrumFixShowPoint
{
   float x;
   float y;
   int Color;
   unsigned char SizeX;
   unsigned char SizeY;
   unsigned char Shape; // 0 - Ellipse, 1 - Rectangle
   AnsiString Text;
};
//---------------------------------------------------------------------------
typedef TMyArray<TSpectrumFixShowPoint> TSpectrumFixShowPointsArray;
//---------------------------------------------------------------------------
class TSpectrumFixFigureThread;
//---------------------------------------------------------------------------
class TSpectrumFixRepaintRegions
{
public:
  TRect **Regions;
  TCanvas *to, *from;
  int N;
  __fastcall TSpectrumFixRepaintRegions(TCanvas *_to, TCanvas *_from,int _count);
  void __fastcall Add(int X,int Y,int X1,int Y1);
  void __fastcall Clear();
  void __fastcall Restore();
  __fastcall ~TSpectrumFixRepaintRegions();
};
//---------------------------------------------------------------------------
class TSpectrumFixMouseRect
{
public:
   TSpectrumFixRepaintRegions *Regs;
   TCanvas *to, *from;
   int X0,Y0,X1,Y1;
   int State;
   __fastcall TSpectrumFixMouseRect(TCanvas *_to,TCanvas *_from);
   void __fastcall Set(int aX0, int aY0, int aState);
   void __fastcall Pull(int aX1, int aY1);
   void __fastcall Repaint();
   void __fastcall Remove();
   __fastcall ~TSpectrumFixMouseRect();
};
//---------------------------------------------------------------------------
class TSpectrumFixFigure;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TMySpectrumFixMarker
{
public:
   float x;    // точка постановки
   __fastcall TMySpectrumFixMarker(float ax);
   __fastcall TMySpectrumFixMarker();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TSpectrumFixFigureData
{
public:
  float * X;             // Массив значений по оси Х
  float * Y;             // Массив значений по оси У
  int N;                  // Размер массивов
  TColor Color;           // Цвет графика
  int Width;              // Толщина линий графика
  TSpectrumFixFigureData(float *x, float * y,int n,int _width,TColor _color);
  LogTransformY(float zero,float multiplier);
  LogTransformX(float zero,float multiplier);
  float maxy();
  float miny();
  float maxx();
  float minx();

  int ibegin,iend;
  float StepX; // шаг по оси Х
  int iShowStep; // шаг отображения

  bool Uniform;
  void __fastcall SetBeginEnd(float xmin,float xmax, int PointCount);
  TSpectrumFixFigureData();
  ~TSpectrumFixFigureData();
  bool DeleteArrays; // удалять ли массивы при закрытии
};
//---------------------------------------------------------------------------
class TSpectrumFixFParams          // Параметры системы координат
{
public:
   bool grid;
   int k;
   TColor axis,strings,numbers;    // Цвета
   AnsiString namex,namey,name,name2,UnitX,UnitY;    // Названия осей и графика
   float xmin,xmax,ymin,ymax;     // Параметры системы координат
   TForm *MainForm;
   TStatusPanel *Panel0;
   TStatusPanel *Panel1;
   TStatusPanel *Panel2;
   TStatusPanel *Panel3;

   int dX0, dY0, dX1, dY1; // отступы зоны отображения на канве формы
   __fastcall TSpectrumFixFParams();
};
//---------------------------------------------------------------------------
class TSpectrumFixFigure:TComponent
{
__published:	// IDE-managed Components
   //TStatusBar *StatusBar1;
   //TToolBar *ToolBar1;
   //TSplitter *Splitter1;
   //TSpeedButton *SpeedButton2;
   //TSpeedButton *SpeedButton1;
   //TSplitter *Splitter2;
   //TSplitter *Splitter3;
   //TSaveDialog *SaveDialog1;
   //TSpeedButton *SpeedButton5;
   //TSplitter *Splitter5;
   //TSplitter *Splitter7;
   //TSpeedButton *SpeedButton6;
   //TSpeedButton *SpeedButton7;

   void __fastcall FormPaint(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall RestoreOriginalState();
   void __fastcall FormDeactivate(TObject *Sender);
   void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall SpeedButton1Click(TObject *Sender);
   void __fastcall SpeedButton5Click(TObject *Sender);
   void __fastcall SpeedButton6Click(TObject *Sender);
   void __fastcall SpeedButton7Click(TObject *Sender);
private:	// User declarations

public:		// User declarations
   TSpectrumFixFigure **SelfPointer;     // Указатель на самого себя
   bool isSquare;
   int num;                    // кол-во графиков
   TSpectrumFixFigureData ** data;        // массив графиков
   int Y0,X0,W0,H0;
   int Y1,X1,W1,H1;            // параметры прямоугольника, где рисуется график
   float xmax,xmin,ymax,ymin; // параметры системы координат
   float mastabx,mastaby;
   float xstep,ystep,rxmin,rymin;
   int jxaxis,jyaxis,jx,jy;
   float normx,normy;
   int State;
   Graphics::TBitmap *Invisible;
   TCanvas *Canvas;
   TSpectrumFixFParams *Prm;
   TSpectrumFixMouseRect *MouseRect;
   __fastcall TSpectrumFixFigure(TComponent* Owner, TSpectrumFixFParams *Params,TSpectrumFixFigureData ** _data,int _num);
   __fastcall ~TSpectrumFixFigure();
   float __fastcall X2x(int aX);
   float __fastcall Y2y(int aY);
   int __fastcall x2X(float ax);
   int __fastcall y2Y(float ay);
   virtual void __fastcall DrawCS();
   virtual void __fastcall Draw();
//   virtual void __fastcall 
   void __fastcall setmastab();
   void __fastcall setxyminmax();
   void __fastcall setSquare();
   TSpectrumFixShowPointsArray SpectrumFixShowPointsArray;
   TMyArray<TMySpectrumFixMarker> SpectrumFixMarkerArray;
   int DragMarkerNumber;

   bool BeforeCloseFunctionEnable;
   void (*BeforeCloseFunction)(TSpectrumFixFigure* ThisPointer);

   bool MarkerFunctionEnable;
   void (*MarkerFunction)(TSpectrumFixFigure* ThisPointer, int aX, int aY, float ax);

   int DrawFigureType; // 0-minimum and maximium, 1-maximum only, 2-vertical lines
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
