//---------------------------------------------------------------------------

#ifndef RasterH
#define RasterH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TTable
{
  private:
    CRITICAL_SECTION cSection;
    int fRow;
    int fCol;
    int memsize;
    double **pData;
    double __fastcall GetData(int row, int col);
    void __fastcall SetData(int row, int col, double value);
  public:
    TColor LoClr;
    TColor HiClr;
    __property int Row = {read = fRow};
    __property int Col = {read = fCol};
    __property double Data [int row][int col] = {read = GetData, write = SetData};

    __fastcall TTable(void);
    __fastcall TTable(int row, int col);
    void __fastcall Resize(int row, int col);
    void __fastcall Lock(void);
    void __fastcall Unlock(void);
    void __fastcall GetRangeZ(double &min, double &max);
    __fastcall ~TTable(void);
};
//---------------------------------------------------------------------------
class PACKAGE TRaster //: public TWinControl
{
private:
  CRITICAL_SECTION cSection;

  AnsiString FTitle;
  AnsiString FNameX;
  AnsiString FNameY;

  HDC dc;
  HBRUSH dcbrush;
  HDC cdc;
  HMENU pu_menu;

  HPEN pen;
  HFONT font;
  HBITMAP bitmap;
  // аппаратно независимый битмап для рисования изображения
  HBITMAP dibitmap;
  BITMAPINFO bmpinfo;
  unsigned *ScrData;

  HPEN frmpen;
  HPEN pltpen;
  HPEN grdpen;
  HPEN redpen;
  RECT fullrect;
  RECT plotrect;
  HBRUSH bgbrush;
  HBITMAP orgbitmap;
  HFONT tfont;
  HFONT xfont;
  HFONT yfont;

  bool frame;
  AnsiString text;
  int width;
  int height;
  int left;
  int top;
  int right;
  int bottom;
  int xgrd;
  int ygrd;

  TTable *Table;

  double xlod;
  double xhid;
  double ylod;
  double yhid;
  double zlod;
  double zhid;
  double dx;
  double dy;
  double xlo;
  double xhi;
  double ylo;
  double yhi;
  double yloReal;
  double yhiReal;
  bool yType;
  int xmp;
  int ymp;
  double xml;
  double yml;

  double xsc;
  double ysc;
  double xgs;
  double ygs;
  int Nx_line;
  int Ny_line;
  int *x_line;
  int *y_line;
  AnsiString *x_text;
  AnsiString *y_text;

  bool slct;
  bool zoom;
  int xsPos;
  int ysPos;
  int xfPos;
  int yfPos;
  double xsl;
  double ysl;
  bool autoscale;
  int maxYp;
  int minYp;
  int wLineW;
  int wLineH;
  double wLineMin;
  double wLineMax;
  int wLineT;
  int wLineL;

  void __fastcall EventProc(Messages::TMessage &Message);
  void __fastcall Create();
  void __fastcall Paint();
  void __fastcall Size();
  void __fastcall MoveMouse(int Keys, int xPos, int yPos);
  void __fastcall LeftButton(int Keys, int xPos, int yPos, bool Down);
  void __fastcall ContextMenu(int lParamHi, int lParamLo);
  void __fastcall Command(int wParamHi, int wParamLo);
  void __fastcall Destroy();
  void __fastcall GetSizes();
  void __fastcall GetScales();
  void __fastcall DrawFrame();
  void __fastcall DrawLabel();
  void __fastcall DrawGrid();
  void __fastcall DrawBkGnd();
  void __fastcall SetPlotRect(double _xlo, double _ylo, double _xhi, double _yhi);
  bool __fastcall GetPos(int xPos, int yPos, double &x, double &y);
  void __fastcall SetTitle(AnsiString _Title);
  void __fastcall SetNameX(AnsiString _NameX);
  void __fastcall SetNameY(AnsiString _NameY);
  TWndMethod originalWindowProc;
  TWinControl *FOwner;
  void __fastcall SetAutoscale(bool _autoscale);

protected:
public:
  int Margin;

  __fastcall TRaster(TComponent* Owner);
  __fastcall ~TRaster();
  void __fastcall SetLimits(double _xlo, double _ylo, double _xhi, double _yhi);
  void __fastcall SetType(bool param);
  void __fastcall Plot();
  void __fastcall SetTable(TTable *_Table);
  void __fastcall ResetTable();
  void __fastcall SetLimitZ(double _zlo, double _zhi);
  void __fastcall WriteLineY(int maxP, int minP);
  void __fastcall WriteLine();
//__published:
  __property AnsiString Title = {read = FTitle, write = SetTitle};
  __property AnsiString NameX = {read = FNameX, write = SetNameX};
  __property AnsiString NameY = {read = FNameY, write = SetNameY};
  __property bool AutoScaleZ = {read = autoscale, write = SetAutoscale};
};
//---------------------------------------------------------------------------
#endif
