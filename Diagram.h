//---------------------------------------------------------------------------

#ifndef DiagramH
#define DiagramH
//---------------------------------------------------------------------------
//#include <SysUtils.hpp>
//#include <Classes.hpp>
//#include <Controls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TLine
{
  private:
    CRITICAL_SECTION cSection;
    int memsize;
    int fCount;
    double *pArrayX;
    double *pArrayY;
    double __fastcall GetPointX(int n);
    void __fastcall SetPointX(int n, double value);
    double __fastcall GetPointY(int n);
    void __fastcall SetPointY(int n, double value);
  public:
    TColor Color;
    int Width;
    __property int Count = {read = fCount};
    __property double ArrayX[int n] = {read = GetPointX, write = SetPointX};
    __property double ArrayY[int n] = {read = GetPointY, write = SetPointY};

    __fastcall TLine(void);
    __fastcall TLine(int count, int width, TColor color);
    void __fastcall Lock(void);
    void __fastcall Unlock(void);
    void __fastcall Resize(int count);
    __fastcall ~TLine(void);
    AnsiString Title;
    bool visible;
};
//---------------------------------------------------------------------------
enum {amNone = 0, amAuto, amZoom, amMove};
class PACKAGE TDiagram : public TWinControl
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
  HMENU pl_menu;

  HPEN pen;
  HFONT font;
  HBITMAP bitmap;

  HPEN frmpen;
  HPEN pltpen;
  HPEN grdpen;
  RECT fullrect;
  RECT plotrect;
  HBRUSH bgbrush;
  HBRUSH fmbrush;
  COLORREF fmcolor;
  HBITMAP orgbitmap;
  HFONT tfont;
  HFONT xfont;
  HFONT yfont;

//  bool frame;
  bool zoom;
  int mode;
  AnsiString text;
  int width;
  int height;
  int left;
  int top;
  int right;
  int bottom;
  int xgrd;
  int ygrd;

  TList *LineList;

  double xlod;
  double xhid;
  double ylod;
  double yhid;
  double xlo;
  double xhi;
  double ylo;
  double yhi;
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
  int xsPos;
  int ysPos;
  int xfPos;
  int yfPos;
  double xsl;
  double ysl;
  TColor FbackColor;
  TColor FplotColor;

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
  void __fastcall DrawBkGnd();
  void __fastcall DrawFrame();
  void __fastcall AutoScale();

  void __fastcall GetScales();
  void __fastcall DrawLabel();
  void __fastcall DrawGrid();

  int __fastcall GetPntRgn(double x, double y);
  void __fastcall SetPlotRect(double _xlo, double _ylo, double _xhi, double _yhi);
  bool __fastcall GetPos(int xPos, int yPos, double &x, double &y);
  void __fastcall SetTitle(AnsiString _Title);
  void __fastcall SetNameX(AnsiString _NameX);
  void __fastcall SetNameY(AnsiString _NameY);

protected:
public:
  int Margin;

  __fastcall TDiagram(TComponent* Owner);
  __fastcall ~TDiagram();
  void __fastcall SetLimits(double _xlo, double _ylo, double _xhi, double _yhi);
  void __fastcall AddLine(TLine *Line);
  void __fastcall RemoveLine(TLine *Line);
  void __fastcall Plot();

__published:
  __property AnsiString Title = {read = FTitle, write = SetTitle};
  __property AnsiString NameX = {read = FNameX, write = SetNameX};
  __property AnsiString NameY = {read = FNameY, write = SetNameY};
  __property Align;
  __property BevelKind;
};
//---------------------------------------------------------------------------
#endif
