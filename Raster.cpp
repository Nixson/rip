//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "Raster.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TTable::TTable()
{
  InitializeCriticalSection(&cSection);
  fRow = 0;
  fCol = 0;
  memsize = 0;
  LoClr = clBlack;
  HiClr = clWhite;
  pData = NULL;
}
//---------------------------------------------------------------------------
__fastcall TTable::TTable(int row, int col)
{
  InitializeCriticalSection(&cSection);
  fRow = row;
  fCol = col;
  memsize = fCol*sizeof(double);
  LoClr = clBlack;
  HiClr = clWhite;
  pData = (double**) malloc(fRow*sizeof(double*));
  for(int r = 0; r < fRow; r++)
  {
    pData[r] = (double*)malloc(memsize);
    setmem(pData[r], memsize, 0);
  }
}
//---------------------------------------------------------------------------
void __fastcall TTable::Resize(int row, int col)
{
  EnterCriticalSection(&cSection);
  try
  {
    if(pData)
    {
      if(row != fRow)
      {
        pData = (double**)realloc(pData, row*sizeof(double*));
        if(row > fRow) setmem(&pData[fRow], (row - fRow)*sizeof(double*), 0);
      }
    }
    else
    {
      pData = (double**)malloc(row*sizeof(double*));
      setmem(pData, row*sizeof(double*), 0);
    }
    fRow = row;

    memsize = col*sizeof(double);
    for(int r = 0; r < fRow; r++)
    {
      if(pData[r])
      {
        if(col != fCol)
        {
          pData[r] = (double*)realloc(pData[r], memsize);
          if(col > fCol) setmem(&pData[r][fCol], (col - fCol)*sizeof(double), 0);
        }
      }
      else
      {
        pData[r] = (double*)malloc(memsize);
        setmem(pData[r], memsize, 0);
      }
    }
    fCol = col;
  }
  __finally
  {
    LeaveCriticalSection(&cSection);
  }
}
//---------------------------------------------------------------------------
void __fastcall TTable::Lock(void)
{
  EnterCriticalSection(&cSection);
}
//---------------------------------------------------------------------------
void __fastcall TTable::Unlock(void)
{
  LeaveCriticalSection(&cSection);
}
//---------------------------------------------------------------------------
double __fastcall TTable::GetData(int row, int col)
{
  if(row < 0) row = 0;
  else if(row >= fRow) row = fRow - 1;
  if(col < 0) col = 0;
  else if(col >= fCol) col = fCol - 1;
  return pData[row][col];
}
//---------------------------------------------------------------------------
void __fastcall TTable::SetData(int row, int col, double value)
{
  if(row >= 0 && row < fRow)
  {
    if(col >= 0 && col < fCol)
    {
      pData[row][col] = value;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TTable::GetRangeZ(double &min, double &max)
{
  if(pData)
  {
    min = max = pData[0][0];
    for(int r = 0; r < Row; r++)
      for(int c = 0; c < Col; c++)
      {
        if(pData[r][c] > max) max = pData[r][c];
        if(pData[r][c] < min) min = pData[r][c];
      }
  }
}
//---------------------------------------------------------------------------
__fastcall TTable::~TTable()
{
  if(pData)
  {
    for(int r = 0; r < fRow; r ++)
      if(pData[r]) free(pData[r]);
  }
  free(pData);
  fRow = 0;
  fCol = 0;
  memsize = 0;
  LoClr = clBlack;
  HiClr = clBlack;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TRaster *)
{
        new TRaster(NULL);
}
//---------------------------------------------------------------------------
__fastcall TRaster::TRaster(TComponent* Owner)
//        : TWinControl(Owner)
{
  InitializeCriticalSection(&cSection);
//  BevelKind = bkTile;
//  BevelWidth = 1;
//  BevelOuter = bvRaised;
  FOwner = (TWinControl*)Owner;
  originalWindowProc = FOwner->WindowProc;
  FOwner->WindowProc = EventProc;
//  if(FOwner->CanFocus()) Create();
  Margin = 20;
  xlo = xhi = xlod = xhid = 0;
  ylo = yhi = ylod = yhid = yloReal = yhiReal = 0;
  yType = true;

  xsc = ysc = 0;
  xmp = ymp = 0;
  xml= yml = 1;

  maxYp = minYp = 0;
  wLineW = 0;
  wLineH = 0;
  wLineMin = 0;
  wLineMax = 0;
  wLineT = 0;
  wLineL = 0;

  frame = false;
  zoom = false;
  slct = false;
  autoscale = true;
  FTitle = "»зображение";
  FNameX = "ось X";
  FNameY = "ось Y";
  Table = NULL;
  Nx_line = 0;
  Ny_line = 0;
  x_line = y_line = NULL;
  x_text = y_text = NULL;
}
//---------------------------------------------------------------------------
__fastcall TRaster::~TRaster()
{
//  if(FOwner->CanFocus()) Destroy();
  FOwner->WindowProc = originalWindowProc;
  if(x_line) delete[] x_line;
  if(x_text) delete[] x_text;
  if(y_line) delete[] y_line;
  if(y_text) delete[] y_text;
  DeleteCriticalSection(&cSection);
}
//---------------------------------------------------------------------------
/*
namespace Raster
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TRaster)};
                 RegisterComponents("Samples", classes, 0);
        }
}
*/
//---------------------------------------------------------------------------
void __fastcall TRaster::EventProc(Messages::TMessage &Message)
{
  if(Message.Msg != WM_ERASEBKGND) originalWindowProc(Message);
  switch(Message.Msg)
  {
    case WM_CREATE:
      Create();
      break;
    case WM_SIZE:
      Size();
      break;
    case WM_PAINT:
      Paint();
      break;
    case WM_ERASEBKGND:
      Message.Result = 1;
      break;
    case WM_DESTROY:
      Destroy();
      break;
    case WM_MOUSEMOVE:
      MoveMouse(Message.WParam, Message.LParamLo, Message.LParamHi);
      break;
    case WM_LBUTTONDOWN:
      LeftButton(Message.WParam, Message.LParamLo, Message.LParamHi, true);
      break;
    case WM_LBUTTONUP:
      LeftButton(Message.WParam, Message.LParamLo, Message.LParamHi, false);
      break;
    case WM_CONTEXTMENU:
      ContextMenu(Message.LParamHi, Message.LParamLo);
      break;
    case WM_COMMAND:
      Command(Message.WParamHi, Message.WParamLo);
      break;
  }

}
//---------------------------------------------------------------------------
void __fastcall TRaster::Create()
{
  dc = GetDC((HWND)FOwner->Handle);
  if(dc)
  {
    cdc = CreateCompatibleDC(dc);
    if(cdc)
    {
      SetTextAlign(cdc, TA_CENTER);
      bitmap = CreateCompatibleBitmap(dc, width, height);
      orgbitmap = SelectObject(cdc, bitmap);

      dibitmap = NULL;
      bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFO);
      bmpinfo.bmiHeader.biWidth = 512;
      bmpinfo.bmiHeader.biHeight = 512;
      bmpinfo.bmiHeader.biPlanes = 1;
      bmpinfo.bmiHeader.biBitCount = 32;
      bmpinfo.bmiHeader.biCompression = BI_RGB;
      bmpinfo.bmiHeader.biSizeImage = 0;
      bmpinfo.bmiHeader.biClrUsed = 0;
      bmpinfo.bmiHeader.biClrImportant = 0;

      frmpen = CreatePen(PS_SOLID, 2, 0);
      grdpen = CreatePen(PS_DOT, 1, clBlack);
      redpen = CreatePen(PS_SOLID, 1, clRed);
      bgbrush = CreateSolidBrush(RGB(255,255,255));
      tfont = CreateFont(18, 0, 0, 0, FW_LIGHT, false, false, false,
                         DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                         DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
      xfont = CreateFont(16, 0, 0, 0, FW_LIGHT, false, false, false,
                         DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                         DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
      yfont = CreateFont(16, 0, 900, 0, FW_LIGHT, false, false, false,
                         DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                         DEFAULT_QUALITY, DEFAULT_PITCH, NULL);

      pu_menu = CreatePopupMenu();
      if(pu_menu)
      {
        MENUITEMINFO item;
        AnsiString i_title;
        i_title = "ќтменить";
        item.cbSize = sizeof(MENUITEMINFO);
        item.fMask = MIIM_TYPE;
        item.fType = MFT_STRING;
        item.fState = MFS_ENABLED;
        item.wID = 0;
        item.hSubMenu = NULL;
        item.hbmpChecked = NULL;
        item.hbmpUnchecked = NULL;
        item.dwItemData = 0;
        item.dwTypeData = i_title.c_str();
        item.cch = i_title.Length();
        InsertMenuItem(pu_menu, 0, true, &item);
      }
      else pu_menu = NULL;
    }
    else
    {
      ReleaseDC((HWND)FOwner->Handle, dc);
      dc = NULL;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::Destroy()
{
  EnterCriticalSection(&cSection);
  try
  {
    if(dc)
    {
      bitmap = SelectObject(cdc, orgbitmap);
      if(pu_menu) DestroyMenu(pu_menu);

      DeleteObject(bgbrush);
      DeleteObject(bitmap);
      DeleteObject(frmpen);
      DeleteObject(tfont);
      DeleteObject(xfont);
      DeleteObject(yfont);
      if(dibitmap) DeleteObject(dibitmap);

      DeleteDC(cdc);
      ReleaseDC((HWND)FOwner->Handle, dc);
    }
  }
  __finally
  {
    LeaveCriticalSection(&cSection);
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::Paint()
{
  if(cdc)
  {
    StretchBlt(dc, 0, 0, width, height,
               cdc, 0, 0, width, height, SRCCOPY);
    if(slct)
    {
      pen = SelectObject(dc, grdpen);
      MoveToEx(dc, xsPos, ysPos, NULL);
      LineTo(dc, xfPos, ysPos);
      LineTo(dc, xfPos, yfPos);
      LineTo(dc, xsPos, yfPos);
      LineTo(dc, xsPos, ysPos);
      SelectObject(dc, pen);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::Size()
{
  EnterCriticalSection(&cSection);
  try
  {
    GetSizes();
    bitmap = CreateCompatibleBitmap(dc, width, height);
    bitmap = SelectObject(cdc, bitmap);
    DeleteObject(bitmap);
    DrawBkGnd();
    if((top < bottom) && (left < right) &&
       (xlo != xhi) && (ylo != yhi))
    {
      GetScales();
      DrawLabel();
      if(!Table)
      {
        DrawGrid();
        DrawFrame();
      }
      else Plot();
    }
  }
  __finally
  {
    LeaveCriticalSection(&cSection);
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::MoveMouse(int Keys, int xPos, int yPos)
{
  bool res;
  EnterCriticalSection(&cSection);
  try
  {
    if(slct)
    {
     Paint();
     xfPos = xPos;
     yfPos = yPos;
    }
  }
  __finally
  {
    LeaveCriticalSection(&cSection);
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::LeftButton(int Keys, int xPos, int yPos, bool Down)
{
  EnterCriticalSection(&cSection);
  try
  {
    if(Down)
    {
      slct = true;
      if(GetPos(xPos, yPos, xsl, ysl))
      {
        xsPos = xPos;
        ysPos = yPos;
        xfPos = xPos;
        yfPos = yPos;
      }
    }
    else
    {
      slct = false;
      if(xPos != xsPos && yPos != ysPos)
      {
        double x, y;
        if(GetPos(xPos, yPos, x, y))
        {
          zoom = true;
          SetPlotRect(xsl, ysl, x, y);
          DrawBkGnd();
          if((top < bottom-4) && (left < right-4) &&
             (xlo != xhi) && (ylo != yhi))
          {
            GetScales();
            DrawLabel();
            if(!Table)
            {
              DrawGrid();
              DrawFrame();
            }
            else Plot();
          }
        }
      }
      else Paint();
    }
  }
  __finally
  {
    LeaveCriticalSection(&cSection);
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::ContextMenu(int lParamHi, int lParamLo)
{
  if(xlo != xlod || xhi != xhid || ylo != ylod || yhi != yhid)
  TrackPopupMenuEx(pu_menu, TPM_HORIZONTAL, lParamLo,
                   lParamHi, (HWND*)FOwner->Handle, NULL);
}
//---------------------------------------------------------------------------
void __fastcall TRaster::Command(int wParamHi, int wParamLo)
{
  if(wParamHi == 0)
  {
    switch(wParamLo)
    {
      case 0:
        if(xlo != xlod || xhi != xhid || ylo != ylod || yhi != yhid)
        SetPlotRect(xlod, ylod, xhid, yhid);
        DrawBkGnd();
        if((top < bottom-4) && (left < right-4) &&
           (xlo != xhi) && (ylo != yhi))
        {
          GetScales();
          DrawLabel();
          if(!Table)
          {
            DrawGrid();
            DrawFrame();
          }
          else Plot();
        }
        break;
    }
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TRaster::GetSizes()
{
  width = FOwner->ClientWidth;
  height = FOwner->ClientHeight;

  fullrect.left = 0;
  fullrect.top = 0;
  fullrect.right = width;
  fullrect.bottom = height;

  if(NameY == "") left = Margin + 26;
  else left = Margin + 52;
  if(Title == "") top = Margin;
  else top = Margin + 26;
  right = width - Margin;
  if(NameX == "")  bottom = height - Margin - 26;
  else bottom = height - Margin - 52;
  wLineT = top;
  wLineL = left;
  wLineW = right-left - 3;
  wLineH = bottom-top - 3;

  plotrect.left = left + 1;
  plotrect.top = top + 1;
  plotrect.right = right - 2;
  plotrect.bottom = bottom - 2;
  // выделение аппартно независимого битмапа дл€ изображени€ под размер
  if(dibitmap) DeleteObject(dibitmap);
  bmpinfo.bmiHeader.biWidth = right - left - 3;
  bmpinfo.bmiHeader.biHeight = bottom - top - 3;
  dibitmap = CreateDIBSection(cdc, &bmpinfo, DIB_RGB_COLORS, (LPVOID*)&ScrData, NULL, 0);

  xgrd = (right - left)/4;
  ygrd = (bottom - top)/4;
  
  if(xgrd > 64) xgrd = 64;
  if(ygrd > 64) ygrd = 64;
}
//---------------------------------------------------------------------------
void __fastcall TRaster::GetScales()
{
  xsc = xhi - xlo;
  xgs = xsc;
//  if(xsc != 0) xsc = (right - left - 1)/xsc;
  if(xsc != 0) xsc = (right - left - 4)/xsc;
  ysc = yhiReal - yloReal;
  ygs = ysc;
//  if(ysc != 0) ysc = (bottom - top - 2)/ysc;
  if(ysc != 0) ysc = (bottom - top - 4)/ysc;

  double m = 1, g;
  if(xgs < 1)
  {
    do {xgs*=10; m /= 10;}
    while(xgs <1);
  }
  else if(xgs > 10)
  {
    do {xgs/=10; m *= 10;}
    while(xgs > 10);
  }
  xgs = int(xgs)*m/10;
  while(xgs*xsc < xgrd) xgs *= 2;
  if(Nx_line > 0) {delete[] x_line; delete[] x_text; Nx_line = 0;}
  Nx_line = (xhi - xlo)/xgs +1;
  x_line = new int[Nx_line];
  x_text = new AnsiString[Nx_line];
  Nx_line = 0;
  g = xgs*int(xlo/xgs);
  while(g < xlo) g += xgs;
  while(g < xhi-10e-10)
  {
    x_line[Nx_line] = left+1 + xsc*(g - xlo);
    x_text[Nx_line] = FloatToStr(g/xml);
    g += xgs;
    Nx_line++;
  }
  if(Nx_line == 0) {delete[] x_line; delete[] x_text; Nx_line = 0;}

  m = 1;
  if(ygs < 1)
  {
    do {ygs*=10; m /= 10;}
    while(ygs <1);

  }
  else if(ygs >10)
  {
    do {ygs/=10; m *= 10;}
    while(ygs > 10);
  }
  ygs = int(ygs)*m/10;
  while((ygs*ysc < ygrd) && (ygs*ysc>0)) ygs *= 2;
  if(Ny_line > 0) {delete[] y_line; delete[] y_text; Ny_line = 0;}
  Ny_line = (yhiReal - yloReal)/ygs +1;
  y_line = new int[Ny_line];
  y_text = new AnsiString[Ny_line];
  Ny_line = 0;
  g = ygs*int(yloReal/ygs);
  while(g < yloReal) g += ygs;
  while(g <= yhiReal)
  {
    y_line[Ny_line] = top+1 - ysc*(g - yhiReal);
    y_text[Ny_line] = FloatToStr(g/yml);
    g += ygs;
    Ny_line++;
  }
  if(Ny_line == 0) {delete[] y_line; delete[] y_text; Ny_line = 0;}
}
//---------------------------------------------------------------------------
void __fastcall TRaster::DrawFrame()
{
  if(!cdc) return;
  pen = SelectObject(cdc, frmpen);
  Rectangle(cdc, left, top, right, bottom);
  SelectObject(cdc, pen);
}
//---------------------------------------------------------------------------
void __fastcall TRaster::DrawBkGnd()
{
  if(!cdc) return;
  FillRect(cdc, &fullrect, bgbrush);
  if(top <= bottom)
  {
    if(Title != "")
    {
      font = SelectObject(cdc, tfont);
      SetTextColor(cdc, (COLORREF)clBlue);
      TextOut(cdc, width/2,  top - 28, Title.c_str(), Title.Length());
      SelectObject(cdc, font);
    }

    SetTextColor(cdc, (COLORREF)clBlack);
    font = SelectObject(cdc, xfont);
    if(xmp != 0) text = NameX + ", х 10e" + IntToStr(xmp);
    else text = NameX;
    TextOut(cdc, (left+right)/2,  bottom + 36, text.c_str(), text.Length());

    SelectObject(cdc, yfont);
    if(ymp != 0) text = NameY + ", х 10e" + IntToStr(ymp);
    else text = NameY;
    TextOut(cdc, left - 52,  (top + bottom)/2, text.c_str(), text.Length());
    SelectObject(cdc, font);
  }
  frame = true;
}
//---------------------------------------------------------------------------
void __fastcall TRaster::DrawLabel()
{
  SetTextColor(cdc, (COLORREF)clBlack);
  font = SelectObject(cdc, xfont);
  for(int x = 0; x < Nx_line; x++)
    TextOut(cdc, x_line[x],  bottom + 12, x_text[x].c_str(), x_text[x].Length());

  SelectObject(cdc, yfont);
  for(int y = 0; y < Ny_line; y++)
    TextOut(cdc, left - 28, y_line[y], y_text[y].c_str(), y_text[y].Length());
  SelectObject(cdc, font);
}
//---------------------------------------------------------------------------
void __fastcall TRaster::DrawGrid()
{
  pen = SelectObject(cdc, grdpen);
  for(int x = 0; x < Nx_line; x++)
  {
    MoveToEx(cdc, x_line[x], bottom, NULL);
    LineTo(cdc, x_line[x], top);
  }
  for(int y = 0; y < Ny_line; y++)
  {
    MoveToEx(cdc, left, y_line[y], NULL);
    LineTo(cdc, right, y_line[y]);
  }
  SelectObject(cdc, pen);
  WriteLine();
}
//---------------------------------------------------------------------------
void __fastcall TRaster::SetLimits(double _xlo, double _ylo,
                                   double _xhi, double _yhi)
{
  if(_xlo == _xhi || _ylo == _yhi) return;
  wLineMin = _ylo;
  wLineMax = _yhi;
  EnterCriticalSection(&cSection);
  try
  {
    double temp;
    if(_xlo > _xhi)
    {
      temp = _xhi;
      _xhi = _xlo;
      _xlo = temp;
    }
    if(_ylo > _yhi)
    {
      temp = _yhi;
      _yhi = _ylo;
      _ylo = temp;
    }

    bool resize;
    if((xlo == xlod && ylo == ylod && xhi == xhid && yhi == yhid) ||
       (xlo == xhi || ylo == yhi)) resize = true;
    else resize = false;

    xlod = _xlo;
    xhid = _xhi;
    ylod = _ylo;
    yhid = _yhi;

    if(resize)
    {
      SetPlotRect(xlod, ylod, xhid, yhid);
      if(cdc)
      {
        DrawBkGnd();
        if(top < bottom)
        {
          GetScales();
          DrawLabel();
          if(!Table)
          {
            DrawFrame();
            DrawGrid();
          }
          else Plot();
        }
      }
    }
  }
  __finally
  {
    LeaveCriticalSection(&cSection);
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::SetPlotRect(double _xlo, double _ylo,
                                     double _xhi, double _yhi)
{
  if(_xlo <= _xhi)
  {
    xlo = _xlo;
    xhi = _xhi;
  }
  else
  {
    xlo = _xhi;
    xhi = _xlo;
  }
  if(_ylo <= _yhi)
  {
    ylo = _ylo;
    yhi = _yhi;
  }
  else
  {
    ylo = _yhi;
    yhi = _ylo;
  }
  SetType(yType);

  double x, y;
  if(xlo >= 0) x = xhi;
  else if(xhi <= 0 ) x = -xlo;
       else if((xhi + xlo) >= 0) x = xhi;
            else x = -xlo;
  xmp = 0;
  xml = 1;
  if(x >= 1000)
  {
    do
    {
      xmp+=3;
      xml *=1000;
      x /= 1000;
    }
    while(x >=1000);
  }
  else if(x < 1)
  {
    do
    {
      xmp-=3;
      xml /= 1000;
      x *= 1000;
    }
    while(x < 1);
  }

  if(ylo >= 0) y = yhi;
  else if(yhi <= 0 ) y = -ylo;
       else if((yhi+ylo) >= 0) y = yhi;
            else y = -ylo;
  ymp = 0;
  yml = 1;
  if(y >= 1000)
  {
    do
    {
      ymp+=3;
      yml *=1000;
      y /= 1000;
    }
    while(y >=1000);
  }
  else if(y < 1)
  {
    do
    {
      ymp-=3;
      yml /= 1000;
      y *= 1000;
    }
    while(y < 1);
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::SetTable(TTable *_Table)
{
  EnterCriticalSection(&cSection);
  try
  {
    Table = _Table;
  }
  __finally
  {
    LeaveCriticalSection(&cSection);
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::ResetTable()
{
  EnterCriticalSection(&cSection);
  try
  {
    Table = NULL;
  }
  __finally
  {
    LeaveCriticalSection(&cSection);
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::Plot()
{
  if(!cdc || top > bottom || !Table) return;
  if(xhi ==  xlo || yhi == ylo) return;
  if(Table->Col == 0 || Table->Row == 0) return;

  EnterCriticalSection(&cSection);
  try
  {
    FillRect(cdc, &plotrect, bgbrush);
    DrawFrame();

    double zlo, zhi, zsc;
    int X, Y, h, w, Color;
    int XLO, XHI, YLO, YHI, n;
    double x, y, dx, dy, Mask, val, DX, DY, Val;
    double Ylo, Yhi;
    HBRUSH brush;
    if(autoscale) Table->GetRangeZ(zlo, zhi);
    else
    {
      zlo = zlod;
      zhi = zhid;
    }
    if(zlo == zhi)
    {
      brush = CreateSolidBrush(clWhite);
      FillRect(cdc, &plotrect, brush);
      DeleteObject(brush);
    }
    else
    {
      zsc = 1/(zhi - zlo);
      h = bmpinfo.bmiHeader.biHeight;
      w = bmpinfo.bmiHeader.biWidth;
      if(h < 2 || w < 2) return;
      dy = (yhi-ylo)/(h-1);
      dx = (xhi-xlo)/(w-1);

      DX = (xhid - xlod)/(Table->Col-1);
      DY = (yhid - ylod)/(Table->Row-1);
      for(Y = 0; Y < h; Y++)
      {
        y = ylo + Y*dy;
        dy /= 2; // оптимизаци€ объЄма вычислений dy - половина размера пиксела до конца итерации цикла
        y -= ylod; // оптимизаци€ объЄма вычислений ylod везде вычитаетс€
        Ylo = -dy;  // учитываетс€ коррекци€ y
        Yhi = yhid-ylod+dy;
        for(X = 0; X <w; X++)
        {
          x = xlo+X*dx;
          dx /= 2; // оптимизаци€ объЄма вычислений dx - половина размера пиксела до конца итерации цикла
          if(y < Ylo || y > Yhi || x < (xlod-dx) || (x > xhid+dx))
             ScrData[Y*w + X] = 0xFFFFFF; //если точка за пределами области
          else
          {
            x -= xlod; // оптимизаци€ объЄма вычислений xlod везде вычитаетс€
            YLO = (y-dy)/DY+1; if(YLO<0) YLO = 0;
            YHI = (y+dy)/DY;   if(YHI>=Table->Row) YHI = Table->Row-1;
            XLO = (x-dx)/DX+1; if(XLO<0) XLO = 0;
            XHI = (x+dx)/DX;   if(XHI>=Table->Col) XHI = Table->Col-1;

            Val = 0;
            n = 0;
            if(YHI >= YLO)
            {
              // поиск максимума по всем €чейкам в точке
              for(int i = YLO; i<=YHI; i++)
              {
                if(XHI >= XLO)
                {
                  for(int j = XLO; j <=XHI; j++)
                  {
                    val = Table->Data[i][j];
                    Val += val;
                    n++;
                  }
                }
                // интерпол€ци€ точек по ’ вдоль горизонтальных линий
                else
                {
                  if((x/DX-XHI)<0.5) val = Table->Data[i][XHI];
                  else val = Table->Data[i][XLO];
                  Val +=val;
                  n++;
                }
              }
              Val /= n;
            }
            else
            {
              // интерпол€ци€ точек по ” вдоль вертикальных линий
              if(XHI >= XLO)
              {
                for(int j = XLO; j <= XHI; j++)
                {
                  if((y/DY-YHI)<0.5) val = Table->Data[YHI][j];
                  else val = Table->Data[YLO][j];
                  Val += val;
                  n++;
                }
                Val/=n;
              }
              // билинейна€ интеркол€ци€
              else
              {
                if((y/DY-YHI)>=0.5) YHI = YLO;
                if((x/DX-XHI)>=0.5) XHI = XLO;
                Val = Table->Data[YHI][XHI];
              }
            }
            if(Val < zlo) Mask = 0;
            else if(Val > zhi) Mask = 1;
            else Mask = zsc*(Val-zlo);
            Color = (int)(((Table->HiClr&0xFF)*Mask) + ((Table->LoClr&0xFF)*(1-Mask)))|
            ((int)((((Table->HiClr>>8)&0xFF)*Mask) + (((Table->LoClr>>8)&0xFF)*(1-Mask)))<<8)|
            ((int)((((Table->HiClr>>16)&0xFF)*Mask) + (((Table->LoClr>>16)&0xFF)*(1-Mask)))<<16);
            ScrData[Y*w+X] = Color;
          }
          dx *= 2;
        }
        dy *= 2;
      }
      StretchDIBits(cdc, left+1, top+1, w, h, 0, 0, w, h,
                    ScrData, &bmpinfo, DIB_RGB_COLORS, SRCCOPY);
/*
    dx = (xhid - xlod)/Table->Col;
    dy = (yhid - ylod)/Table->Row;

    int r, c, R, C;
    int XLO, YLO, XHI, YHI;
    double x, y, val;
    RECT rect;
    HBRUSH brush;
    int Color;
    double Mask;

    double zlo, zhi, zsc;
    Table->GetRangeZ(zlo, zhi);
    if(zlo == zhi)
    {
      brush = CreateSolidBrush(clWhite);
      FillRect(cdc, &plotrect, brush);
      DeleteObject(brush);
    }
    else
    {
      zsc = 1/(zhi - zlo);

      if(ylo > ylod) R = (ylo - ylod)/dy;
      else R = 0;
      y = ylod + R*dy;
      while(R < Table->Row && y <= yhi)
      {
        if(xlo > xlod) C = (xlo - xlod)/dx;
        else C = 0;
        x = xlod + C*dx;
        r = R;
        while(C < Table->Col && x <= xhi)
        {
          val = Table->Data[R][C];
          y = ylod + r*dy;
          if(y < ylo) YLO = plotrect.bottom;
          else YLO = top - ysc*(y-yhi);
          YHI = top - ysc*(y-yhi);
          c = C;
          while(YHI >= YLO && r < Table->Row)
          {
            x = xlod + c*dx;
            if(x < xlo) XLO = left;
            else XLO = left + xsc*(x-xlo);

            XHI = top + xsc*(x-xlo);
            while(XHI <= XLO && c < Table->Col)
            {
              if(val < Table->Data[r][c]) val = Table->Data[r][c];
              c++;
              x = xlod + c*dx;
              XHI = left + xsc*(x-xlo);
            }
            c--;
            r++;
            y = ylod + r*dy;
            YHI = top - ysc*(y-yhi);
          }
          r--;
          rect.left = XLO;
          if(XHI < plotrect.right) rect.right = XHI;
          else rect.right = plotrect.right;
          rect.bottom = YLO;
          if(YHI > top ) rect.top = YHI;
          else rect.top = top;
          Mask = zsc*(val-zlo);
          Color = (int)(((Table->HiClr&0xFF)*Mask) + ((Table->LoClr&0xFF)*(1-Mask)))|
          ((int)((((Table->HiClr>>8)&0xFF)*Mask) + (((Table->LoClr>>8)&0xFF)*(1-Mask)))<<8)|
          ((int)((((Table->HiClr>>16)&0xFF)*Mask) + (((Table->LoClr>>16)&0xFF)*(1-Mask)))<<16);
          brush = CreateSolidBrush((COLORREF)Color);
          FillRect(cdc, &rect, brush);
          DeleteObject(brush);

          r = R;
          C = c+1;
          x = xlod + C*dx;
        }
        R = r+1;
        y = ylod + R*dy;
      }
*/
    }
    DrawGrid();
    Paint();
  }
  __finally
  {
    LeaveCriticalSection(&cSection);
  }
}
//---------------------------------------------------------------------------
void __fastcall TRaster::SetTitle(AnsiString _Title)
{
  font = SelectObject(cdc, tfont);
  if(Title != "")
  {
    SetTextColor(cdc, (COLORREF)clWhite);
    TextOut(cdc, width/2,  top - 28, Title.c_str(), Title.Length());
  }
  FTitle = _Title;
  if(Title != "")
  {
    SetTextColor(cdc, (COLORREF)clBlue);
    TextOut(cdc, width/2,  top - 28, Title.c_str(), Title.Length());
  }
  SelectObject(cdc, font);
  Paint();
}
//---------------------------------------------------------------------------
void __fastcall TRaster::SetNameX(AnsiString _NameX)
{
  font = SelectObject(cdc, xfont);
  SetTextColor(cdc, (COLORREF)clWhite);
  if(xmp != 0) text = NameX + ", х 10e" + IntToStr(xmp);
  else text = NameX;
  TextOut(cdc, (left+right)/2,  bottom + 36, text.c_str(), text.Length());
  FNameX = _NameX;
  SetTextColor(cdc, (COLORREF)clBlack);
  if(xmp != 0) text = NameX + ", х 10e" + IntToStr(xmp);
  else text = NameX;
  TextOut(cdc, (left+right)/2,  bottom + 36, text.c_str(), text.Length());
  SelectObject(cdc, font);
  Paint();
}
//---------------------------------------------------------------------------
void __fastcall TRaster::SetNameY(AnsiString _NameY)
{
  font = SelectObject(cdc, yfont);
  SetTextColor(cdc, (COLORREF)clWhite);
  if(ymp != 0) text = NameY + ", х 10e" + IntToStr(ymp);
  else text = NameY;
  TextOut(cdc, (left+right)/2,  bottom + 36, text.c_str(), text.Length());
  FNameY = _NameY;
  SetTextColor(cdc, (COLORREF)clBlack);
  if(xmp != 0) text = NameY + ", х 10e" + IntToStr(ymp);
  else text = NameY;
  TextOut(cdc, (left+right)/2,  bottom + 36, text.c_str(), text.Length());
  SelectObject(cdc, font);
  Paint();

  }
/*
//---------------------------------------------------------------------------
void __fastcall TRaster::M_Move(int Keys, int xPos, int yPos)
{
  bool res;
  if((Keys&MK_LBUTTON) != 0)
  {
    Paint();
    pen = SelectObject(dc, grdpen);
    MoveToEx(dc, xsPos, ysPos, NULL);
    LineTo(dc, xPos, ysPos);
    LineTo(dc, xPos, yPos);
    LineTo(dc, xsPos, yPos);
    LineTo(dc, xsPos, ysPos);
    SelectObject(dc, pen);
  }
}
*/
//---------------------------------------------------------------------------
bool __fastcall TRaster::GetPos(int xPos, int yPos, double &x, double &y)
{
  bool res;
  if(xsc == 0 || ysc == 0) res = false;
  else
  {
     res = true;
     x = xlo + (xPos - left)/xsc;
     y = yhi - (yPos - top)/ysc;
  }
  return res;
}
//---------------------------------------------------------------------------
void __fastcall TRaster::SetLimitZ(double _zlo, double _zhi)
{
  zlod = _zlo;
  zhid = _zhi;
}
//---------------------------------------------------------------------------
void __fastcall TRaster::SetAutoscale(bool _autoscale)
{
  autoscale = _autoscale;
  Plot();
}
//---------------------------------------------------------------------------
void __fastcall TRaster::WriteLineY(int maxP, int minP){
        if(maxYp != maxP) maxYp = maxP;
        if(minYp != minP) minYp = minP;
}
void __fastcall TRaster::WriteLine(){
if(maxYp==0 && minYp==0) return;

  pen = SelectObject(cdc, redpen);
  double range = wLineH/(wLineMax-wLineMin);
  if(maxYp > 0){
     double maxYpd = (double)maxYp - wLineMin;
     if(wLineMax > maxYpd){
        double tP = wLineH-range*maxYpd;
        double Pso = tP+top+1;
        if(Pso < bottom && Pso > top){
            MoveToEx(cdc, left, Pso, NULL);
            LineTo(cdc, right-2, Pso);
        }
     }
  }
  if(minYp > 0){
     double minYpd = (double)minYp - wLineMin;
     if(wLineMax > minYpd){
        double tPmin = wLineH-range*minYpd;
        double PsoMin = tPmin+top+1;
        if(PsoMin < bottom && PsoMin > top){
            MoveToEx(cdc, left, PsoMin, NULL);
            LineTo(cdc, right, PsoMin);
        }
     }
  }
  SelectObject(cdc, pen);
}

void __fastcall TRaster::SetType(bool param){
        yType = param;
        if(param) {
                yloReal = ylo;
                yhiReal = yhi;
        }
        else {
                yloReal = ylo*0.063;
                yhiReal = yhi*0.063;
        }
}
//---------------------------------------------------------------------------
/*
void __fastcall TRaster::LeftButton(int Keys, int xPos, int yPos, bool Down)
{
  if(Down)
  {
    if(GetPos(xPos, yPos, xsl, ysl))
    {
      xsPos = xPos;
      ysPos = yPos;
    }
  }
  else
  {
    if(xPos != xsPos && yPos != ysPos)
    {
      double x, y;
      if(GetPos(xPos, yPos, x, y))
        SetLimits(xsl, ysl, x, y , false);
    }
    else Paint();
  }
}
//---------------------------------------------------------------------------
*/
