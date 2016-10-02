//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SpectrumFixFigures.h"
//#include "SpectrumFigures_Opt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//TSpectrumFixFigure *SpectrumFixFigure;
//---------------------------------------------------------------------------
__fastcall TSpectrumFixRepaintRegions::TSpectrumFixRepaintRegions(TCanvas *_to, TCanvas *_from, int _count)
{
   to = _to;
   from = _from;
   N = 0;
   Regions = new TRect*[_count];
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixRepaintRegions::Add(int X,int Y,int X1,int Y1)
{
   Regions[N] = new TRect(X,Y,X1,Y1);
   N++;
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixRepaintRegions::Clear()
{
   for (int i=0; i<N; i++) delete Regions[i];
   N = 0;
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixRepaintRegions::Restore()
{
   for (int j=0; j<N; j++) to->CopyRect(Regions[j][0], from, Regions[j][0]);
   Clear();
};
//---------------------------------------------------------------------------
__fastcall TSpectrumFixRepaintRegions::~TSpectrumFixRepaintRegions()
{
   Clear();
   delete[] Regions;
};
//---------------------------------------------------------------------------
__fastcall TSpectrumFixMouseRect::TSpectrumFixMouseRect(TCanvas *_to,TCanvas *_from)
{
   to = _to;
   from = _from;
   Regs = new TSpectrumFixRepaintRegions(to, from, 1);
   State = fsSpFree;
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixMouseRect::Set(int aX0, int aY0, int aState)
{
   X0 = aX0;
   Y0 = aY0;
   State = aState;
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixMouseRect::Pull(int aX1, int aY1)
{
   int tmp;
   X1 = aX1;
   Y1 = aY1;
 //  if (aX1 >= X0) X1 = aX1; else
 //  {
 //     tmp = X0;
 //     X0 = aX1;
 //     X1 = tmp;
 //  };
 //  if (aY1 >= Y0) Y1 = aY1; else
 // {
 //     tmp = Y0;
 //     Y0 = aY1;
 //     Y1 = tmp;
 //  };
   Repaint();
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixMouseRect::Repaint()
{
   if (State == fsSpFree) return;
   to->Lock();
   from->Lock();
   Regs->Restore();
   if (State == fsSpSelect)
   {
      to->Pen->Color = clGreen;
      to->Pen->Style = psDot;
   }
   else
   {
      to->Pen->Color = clBlack;
      to->Pen->Style = psSolid;
   }
   to->MoveTo(X0,Y0);
   to->LineTo(X0,Y1);
   to->LineTo(X1,Y1);
   to->LineTo(X1,Y0);
   to->LineTo(X0,Y0);
   if (X0<X1 && Y0<Y1)
   {
      Regs->Add(X0-1,Y0-1,X1+1,Y1+1);
   };
   if (X1<X0 && Y0<Y1)
   {
      Regs->Add(X1-1,Y0-1,X0+1,Y1+1);
   };
   if (X1<X0 && Y1<Y0)
   {
      Regs->Add(X1-1,Y1-1,X0+1,Y0+1);
   };
   if (X0<X1 && Y1<Y0)
   {
      Regs->Add(X0-1,Y1-1,X1+1,Y0+1);
   };
   from->Unlock();
   to->Unlock();
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixMouseRect::Remove()
{
   if (State == fsSpFree) return;
   to->Lock();
   from->Lock();
   Regs->Restore();
   from->Unlock();
   to->Unlock();
   State = fsSpFree;
};
//---------------------------------------------------------------------------
__fastcall TSpectrumFixMouseRect::~TSpectrumFixMouseRect()
{
   delete Regs;
};
//---------------------------------------------------------------------------
__fastcall TMySpectrumFixMarker::TMySpectrumFixMarker(float ax)
{
   x = ax;
};
//---------------------------------------------------------------------------
__fastcall TMySpectrumFixMarker::TMySpectrumFixMarker()
{
   x = 0;
};
//---------------------------------------------------------------------------
TSpectrumFixFigureData::TSpectrumFixFigureData(float *x, float *y,int n,int _width,TColor _color)
{
   X = x;
   Y = y;
   N = n;
   Width = _width;
   Color = _color;
   ibegin = 0;
   iend = N;
   Uniform = true;
   DeleteArrays = true; // удалять ли массивы при закрытии
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TSpectrumFixFigureData::~TSpectrumFixFigureData()
{
   if (DeleteArrays)
   {
	   delete[] X;
	   delete[] Y;
   };
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigureData::SetBeginEnd(float xmin,float xmax, int PointCount)
{
   int i1,i2,i;
   ibegin=0;
   iend=N;
   iShowStep = 1;
   if (Uniform)
   {
      StepX = (X[N-1]-X[0])/float(N-1);
      ibegin = (int)floor((xmin-X[0])/StepX);
      iend = 2 + (int)floor((xmax-X[0])/StepX);
      if (iend>N) iend=N;
      if (iend<0) iend=0;
      if (ibegin<0) ibegin=0;
      if (ibegin>=N) ibegin=N-1;

      iShowStep = (iend - ibegin)/PointCount;
      if (iShowStep==0) iShowStep=1;
  };
};
//---------------------------------------------------------------------------
TSpectrumFixFigureData::LogTransformY(float zero,float multiplier)
{
   for (int i=0; i<N; i++)
   {
      float dtmp = Y[i]/zero;
      if (dtmp <= 1e-12) Y[i] = -12*multiplier;
      else Y[i] = multiplier*log10(dtmp);
   };
   return 0;
};
//---------------------------------------------------------------------------
TSpectrumFixFigureData::LogTransformX(float zero,float multiplier)
{
   for (int i=0; i<N; i++)
   {
      float dtmp = X[i]/zero;
      if (dtmp <= 1e-12) X[i] = -12*multiplier;
      else X[i] = multiplier*log10(dtmp);
   };
   return 0;
};
//---------------------------------------------------------------------------
float TSpectrumFixFigureData::maxy()
{
   float tmp=Y[0];
   for (int i=1; i<N; i++) if (tmp<Y[i]) tmp=Y[i];
   return tmp;
};
//---------------------------------------------------------------------------
float TSpectrumFixFigureData::miny()
{
   float tmp=Y[0];
   for (int i=1; i<N; i++) if (tmp>Y[i]) tmp=Y[i];
   return tmp;
};
//---------------------------------------------------------------------------
float TSpectrumFixFigureData::maxx()
{
   float tmp=X[0];
   for (int i=1; i<N; i++) if (tmp<X[i]) tmp=X[i];
   return tmp;
};
//---------------------------------------------------------------------------
float TSpectrumFixFigureData::minx()
{
   float tmp=X[0];
   for (int i=1; i<N; i++) if (tmp>X[i]) tmp=X[i];
   return tmp;
};
//---------------------------------------------------------------------------
_fastcall TSpectrumFixFParams::TSpectrumFixFParams()
{
   axis = clBlack;
   strings = clBlack;
   numbers = clBlue;
   namex = "X";
   namey = "Y";
   name = "";
   name2 = "";
   xmin = 0;
   xmax = 1;
   ymin = 0;
   ymax = 1;
   UnitX = "";
   UnitY = "";
   grid = true;
   k = 10;
   MainForm = NULL;
   Panel0 = NULL;
   Panel1 = NULL;
   Panel2 = NULL;
   Panel3 = NULL;
   dX0 = 0;
   dY0 = 0;
   dX1 = 0;
   dY1 = 0;
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::DrawCS()
{
   float x,y;
   AnsiString stmp;
   Canvas->Lock();
   Invisible->Canvas->Lock();
   Invisible->Canvas->Brush->Color = clWhite;
   Invisible->Canvas->Pen->Color = clWhite;
   Invisible->Canvas->Rectangle(X0,Y0,X0 + W0,Y0 + H0);
   Invisible->Canvas->Pen->Color = Prm->axis;
   Invisible->Canvas->Pen->Width = 1;
   Invisible->Canvas->Pen->Style = psSolid;
   Invisible->Canvas->Rectangle(X1,Y1,X1 + W1,Y1 + H1);
   Invisible->Canvas->Font->Color = Prm->numbers;
   Invisible->Canvas->Font->Size = 10;
   Invisible->Canvas->Font->Style = TFontStyles()<<fsBold;
   if (Prm->grid)
   {
      Invisible->Canvas->Pen->Color = clGreen;
      Invisible->Canvas->Pen->Width = 1;
      Invisible->Canvas->Pen->Style = psDot;
      x = rxmin;
      while ( x < xmax)
      {
         Invisible->Canvas->MoveTo(X1 + mastabx*(x - xmin),Y1);
         Invisible->Canvas->LineTo(X1 + mastabx*(x - xmin),Y1 + H1);
         x+= xstep;
      };
      y = rymin;
      while (y < ymax)
      {
         Invisible->Canvas->MoveTo(X1,Y1 + mastaby*(ymax - y));
         Invisible->Canvas->LineTo(X1 + W1,Y1 + mastaby*(ymax - y));
         y+= ystep;
      };
   };
   Invisible->Canvas->Pen->Color = Prm->axis;
   Invisible->Canvas->Pen->Width = 1;
   Invisible->Canvas->Pen->Style = psSolid;
   x = rxmin;
   while (x < xmax)
   {
      Invisible->Canvas->MoveTo(X1 + mastabx*(x - xmin), Y1 + H1 - 1);
      Invisible->Canvas->LineTo(X1 + mastabx*(x - xmin), Y1 + H1 - 6);
      stmp = F2S(x*normx, jx);
      Invisible->Canvas->TextOutA(X1 + mastabx*(x - xmin) - Invisible->Canvas->TextWidth(stmp)/2, Y1 + H1, stmp);
      x+=xstep;
   };
   if (normx!=1)Invisible->Canvas->TextOutA(X1 + W1 - 25, Y1 + H1 + 12, "x10e" + F2S(-log10(normx), 0));

   y = rymin;
   while (y < ymax)
   {
      Invisible->Canvas->MoveTo(X1 + 1, Y1 + mastaby*(ymax - y));
      Invisible->Canvas->LineTo(X1 + 6, Y1 + mastaby*(ymax - y));
      stmp = F2S(y*normy,jy);
      Invisible->Canvas->TextOutA(X1 - 2 - Invisible->Canvas->TextWidth(stmp), Y1 + mastaby*(ymax - y) - Invisible->Canvas->TextHeight("1")/2, stmp);
      y+=ystep;
   };
   if (normy!=1)Invisible->Canvas->TextOutA(X1, Y0 + 40, "x10e" + F2S(-log10(normy), 0));

   Invisible->Canvas->Font->Color = Prm->strings;
   int name1x = X1 + W1/2 - Invisible->Canvas->TextWidth(Prm->name)/2;
   int name1y = (Y1 + 2*Y0)/3 - (Invisible->Canvas->TextHeight(Prm->name))/2;
   int name2x = X1 + W1/2 - Invisible->Canvas->TextWidth(Prm->name2)/2;
   int name2y = (2*Y1 + Y0)/3 - (Invisible->Canvas->TextHeight(Prm->name2))/2;

   Invisible->Canvas->TextOutA(name1x, name1y , Prm->name);
   Invisible->Canvas->TextOutA(name2x, name2y , Prm->name2);

   stmp = Prm->namex;
   if (Prm->UnitX!="") stmp+=", " + Prm->UnitX;
   Invisible->Canvas->TextOutA(X1 + W1/2 - Invisible->Canvas->TextWidth(stmp)/2, 6 + (Y1 + H1 + Y0 + H0 - Invisible->Canvas->TextHeight(stmp))/2, stmp);
   stmp = Prm->namey;
   if (Prm->UnitY!="") stmp+=", " + Prm->UnitY;
   Invisible->Canvas->TextOutA(X0 + 5, (2*Y1 + Y0)/3 - (Invisible->Canvas->TextHeight(stmp))/2, stmp);
   Canvas->Unlock();
   Invisible->Canvas->Unlock();
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::Draw()
{
   if (data == NULL) return;
   Canvas->Lock();
   Invisible->Canvas->Lock();
   int i,i1,i2,j,k,ibegin,iend, iShowStep;
   double x,y;
   float *DX1;
   float *DY1;
   float Ymaximum, Yminimum;
   HRGN MyRgn;
   MyRgn=::CreateRectRgn(X1+1,Y1+1,X1+W1-1,Y1+H1-1);
   ::SelectClipRgn(Invisible->Canvas->Handle,MyRgn);
   for (j=0; j<num; j++)
   {
      DX1=data[j]->X;
      DY1=data[j]->Y;
      data[j]->SetBeginEnd(xmin,xmax,W1);
      ibegin=data[j]->ibegin;
      iend=data[j]->iend;
      iShowStep = data[j]->iShowStep;
      Invisible->Canvas->Pen->Color=data[j]->Color;
      Invisible->Canvas->Pen->Width=data[j]->Width;
      Invisible->Canvas->Pen->Style = psSolid;
      Invisible->Canvas->Brush->Color = clWhite;
      // ставим первую точку
      x=X1+mastabx*(DX1[ibegin]-xmin);
      y=Y1+mastaby*(ymax-DY1[ibegin]);
      if (y>20000)  y=20000;
      else if (y<-20000) y=-20000;
      Invisible->Canvas->MoveTo(x,y);
      // последовательно перебираем отрезки и на каждом выводим макисмум по Y
      i1 = ibegin;
      i2 = ibegin + iShowStep;
      while(i2<iend)
      {
         Ymaximum = DY1[i1];
         Yminimum = DY1[i1];
         for (i=i1; i<i2; i++)
         {
            if (DY1[i]>Ymaximum) Ymaximum = DY1[i];
            if (DY1[i]<Yminimum) Yminimum = DY1[i];
         };
         x=X1+mastabx*(DX1[i1]-xmin);
         y=Y1+mastaby*(ymax-Ymaximum);
         if (y>20000)  y=20000;
         else if (y<-20000) y=-20000;
         if (DrawFigureType == 2) Invisible->Canvas->MoveTo(x,Y1+H1);
         Invisible->Canvas->LineTo(x,y);
         if (iShowStep>1 && DrawFigureType==0)
         {
            y=Y1+mastaby*(ymax-Yminimum);
            if (y>20000)  y=20000;
            else if (y<-20000) y=-20000;
            Invisible->Canvas->LineTo(x,y);
         };
         i1+=iShowStep;
         i2+=iShowStep;
      }
      // выводим последнюю точку
      Ymaximum = DY1[i1];
      for (i=i1; i<iend; i++)
      {
         if (DY1[i]>Ymaximum) Ymaximum = DY1[i];
      };
      x=X1+mastabx*(DX1[iend-1]-xmin);
      y=Y1+mastaby*(ymax-Ymaximum);
      if (y>20000)  y=20000;
      else if (y<-20000) y=-20000;
      Invisible->Canvas->LineTo(x,y);
   };

   // рисуем точки
   int Count = SpectrumFixShowPointsArray.Count;
   for (int i=0; i<Count; i++)
   {
      Invisible->Canvas->Pen->Color = SpectrumFixShowPointsArray[i].Color;
      Invisible->Canvas->Brush->Color = SpectrumFixShowPointsArray[i].Color;
      if (SpectrumFixShowPointsArray[i].Shape == 0)
      {
         Invisible->Canvas->Ellipse(x2X(SpectrumFixShowPointsArray[i].x)-SpectrumFixShowPointsArray[i].SizeX, y2Y(SpectrumFixShowPointsArray[i].y)-SpectrumFixShowPointsArray[i].SizeY,
                             x2X(SpectrumFixShowPointsArray[i].x)+SpectrumFixShowPointsArray[i].SizeX, y2Y(SpectrumFixShowPointsArray[i].y)+SpectrumFixShowPointsArray[i].SizeY);
      }
      else Invisible->Canvas->FillRect(Rect(x2X(SpectrumFixShowPointsArray[i].x)-SpectrumFixShowPointsArray[i].SizeX, y2Y(SpectrumFixShowPointsArray[i].y)-SpectrumFixShowPointsArray[i].SizeY,
                              x2X(SpectrumFixShowPointsArray[i].x)+SpectrumFixShowPointsArray[i].SizeX, y2Y(SpectrumFixShowPointsArray[i].y)+SpectrumFixShowPointsArray[i].SizeY));
      if (SpectrumFixShowPointsArray[i].Text.Length() > 0)
      {
         Invisible->Canvas->Brush->Color = clWhite;
         Invisible->Canvas->TextOutA(x2X(SpectrumFixShowPointsArray[i].x)+SpectrumFixShowPointsArray[i].SizeX, y2Y(SpectrumFixShowPointsArray[i].y)+SpectrumFixShowPointsArray[i].SizeY, SpectrumFixShowPointsArray[i].Text);
      };
   };
   // рисуем маркеры

   Count = SpectrumFixMarkerArray.Count;
   for (int jj=0; jj<Count; jj++)
   {

      float mx = SpectrumFixMarkerArray[jj].x;
      int mX = x2X(mx);
      Invisible->Canvas->Pen->Color = clBlue;
      Invisible->Canvas->Pen->Style = psDot;
      Invisible->Canvas->Brush->Color = clWhite;
      Invisible->Canvas->MoveTo(mX, Y1);
      Invisible->Canvas->LineTo(mX, Y1 + H1);
      int itmp,i1,i2,i;
      AnsiString stmp1,stmp2;
      i=0;
      if ((data[0]->X[data[0]->ibegin] < mx) && (data[0]->X[data[0]->iend-1] > mx))
      {
         i1 = data[0]->ibegin; i2 = data[0]->iend-1;
         while (i2 - i1 > 1)
         {
            i = (i1 + i2)/2;
            if (data[0]->X[i] > mx) i2 = i;
            else i1=i;
         };
         i = i1;
         if ( i < 0) i = 0;
         if (i >= data[0]->N) i = data[0]->N - 1;
         itmp = i;

         float tmpX = mx;
         float tmpdx = (data[0]->X[i2] - data[0]->X[i1]);
         float tmpY = data[0]->Y[i1];
         if (tmpdx>0)
         {
            tmpY += ((mx-data[0]->X[i1])*(data[0]->Y[i2] - data[0]->Y[i1]))/tmpdx;
         };
         stmp1 = F2S(tmpX,jx + 1) + " " + Prm->UnitX;
         stmp2 = F2S(tmpY,jy + 1) + " " + Prm->UnitY;
         i1 = Invisible->Canvas->TextWidth(stmp1);
         i2 = Invisible->Canvas->TextWidth(stmp2);
         i = (max(i1,i2))>>1;
         Invisible->Canvas->Brush->Color = clYellow;
         Invisible->Canvas->TextOutA(mX - i1/2, 6 + Y1, stmp1);
         Invisible->Canvas->TextOutA(mX - i2/2, 6 + Y1 + Invisible->Canvas->TextHeight(stmp2), stmp2);
      };
   };
   ::SelectClipRgn(Invisible->Canvas->Handle,NULL);
   ::DeleteObject(MyRgn);
   Canvas->CopyRect(Rect(X0,Y0,X0+W0,Y0+H0),Invisible->Canvas,Rect(X0,Y0,X0+W0,Y0+H0));
   Canvas->Unlock();
   Invisible->Canvas->Unlock();
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::setmastab()
{
   const int dl = 30,k = Prm->k;
   Y0 = Prm->dY0;
   X0 = Prm->dX0;
   H0 = Prm->MainForm->ClientHeight - Prm->dY0 - Prm->dY1;

   if (!isSquare)
   {
      W0 = Prm->MainForm->ClientWidth - Prm->dX0 - Prm->dX1;
      Y1 = Y0 + dl*2;
      X1 = X0 + 1.4 * dl;
      W1 = W0 - 2 * dl;
      H1 = H0 - 3 * dl;
      mastabx = (float)W1/(xmax - xmin);
      mastaby = (float)H1/(ymax - ymin);
   }
   else
   {
      float alpha = (xmax - xmin)/(ymax - ymin);
      W0 = alpha*(H0 - 2*dl) + 2*dl;
      Y1 = Y0 + dl;
      X1 = X0 + 1.4 * dl;
      W1 = W0 - 2 * dl;
      H1 = H0 - 2 * dl;
      mastaby = (float)H1/(ymax - ymin);
      mastabx = mastaby;//(float)W1/(xmax - xmin);
   };
   normx = 1;
   normy = 1;
   xstep = floor(1 + (xmax - xmin)*pow10(-floor(log10((xmax - xmin)/k)))/k)/pow10(-floor(log10((xmax - xmin)/k)));
   ystep = floor(1 + (ymax - ymin)*pow10(-floor(log10((ymax - ymin)/k)))/k)/pow10(-floor(log10((ymax - ymin)/k)));
   rxmin = floor(1 + xmin/xstep)*xstep;
   rymin = floor(1 + ymin/ystep)*ystep;
   jxaxis = -floor(log10(xstep));
   if ( jxaxis > 0 ) jx = jxaxis; else jx = 0;
   if ( jxaxis < 0 && jxaxis > -3 ) jxaxis = 0; else
   if ( jxaxis > 5 || jxaxis <= -3 )
   {
      normx = pow10(jxaxis);
      jxaxis = 0;
   }
   jyaxis = -floor(log10(ystep));
   if ( jyaxis > 0 ) jy = jyaxis; else jy = 0;
   if ( jyaxis < 0 && jyaxis > -3 ) jyaxis = 0; else
   if ( jyaxis > 5 || jyaxis <= -3 )
   {
      normy = pow10(jyaxis);
      jyaxis = 0;
   };
}
//---------------------------------------------------------------------------
float __fastcall TSpectrumFixFigure::X2x(int aX)
{
   return xmin + (aX - X1)/mastabx;
};
//---------------------------------------------------------------------------
float __fastcall TSpectrumFixFigure::Y2y(int aY)
{
   return ymax + (Y1 - aY)/mastaby;
};
//---------------------------------------------------------------------------
int __fastcall TSpectrumFixFigure::x2X(float ax)
{
   return X1 + mastabx*(ax - xmin);
};
//---------------------------------------------------------------------------
int __fastcall TSpectrumFixFigure::y2Y(float ay)
{
   return Y1 + mastaby*(ymax - ay);
}
//---------------------------------------------------------------------------
__fastcall TSpectrumFixFigure::TSpectrumFixFigure(TComponent* Owner, TSpectrumFixFParams *Params,TSpectrumFixFigureData **_data,int _num):TComponent(Owner)
{
   BeforeCloseFunctionEnable = false;
   MarkerFunctionEnable = false;
   MarkerFunction = NULL;
   BeforeCloseFunction = NULL;
   SelfPointer = NULL;
   isSquare = false;
   DrawFigureType = 0;
   State = fsSpConstruct;
   Prm = Params;
   data = _data;
   num = _num;
   Canvas = Prm->MainForm->Canvas;
   Prm->MainForm->OnPaint = FormPaint;
   Prm->MainForm->OnResize = FormResize;
   Prm->MainForm->OnKeyDown = FormKeyDown;
   Prm->MainForm->OnMouseMove = FormMouseMove;
   Prm->MainForm->OnMouseDown = FormMouseDown;
   Prm->MainForm->OnMouseUp = FormMouseUp;

   Invisible = new Graphics::TBitmap;
   Invisible->Height = Screen->Height;
   Invisible->Width = Screen->Width;
   Invisible->Canvas->Font->Name = "Times New Roman";
   Invisible->Canvas->Font->Size = 8;
   MouseRect = new TSpectrumFixMouseRect(Canvas,Invisible->Canvas);
   DragMarkerNumber = 0;
   setxyminmax();
   setmastab();
   DrawCS();
   Draw();
   State = fsSpFree;
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::setSquare()
{
   State = fsSpLock;
   isSquare = true;
   setmastab();
   DrawCS();
   Draw();
   State = fsSpFree;
};
//---------------------------------------------------------------------------
__fastcall TSpectrumFixFigure::~TSpectrumFixFigure()
{
   if (SelfPointer)
   {
      SelfPointer[0] = NULL;
   };
   if (Invisible) delete Invisible;
   if (MouseRect) delete MouseRect;
   if (data)
   {
      for (int i=0; i<num; i++)  delete data[i];
      delete[] data;
   };
}
//-------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::FormPaint(TObject *Sender)
{
   Canvas->CopyRect(Rect(X0,Y0,X0+W0,Y0+H0),Invisible->Canvas,Rect(X0,Y0,X0+W0,Y0+H0));
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::FormResize(TObject *Sender)
{
   if (State==fsSpFree)
   {
      setmastab();
      DrawCS();
      Draw();
   };
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
   if ( State!=fsSpLock && Prm->MainForm->Active )
   {
      if (Prm->Panel1) Prm->Panel1->Text = Prm->namex + "= "+F2S(X2x(X), jx+2) + " " + Prm->UnitX;
      if (Prm->Panel2) Prm->Panel2->Text = Prm->namey + "= "+F2S(Y2y(Y), jy+2) + " " + Prm->UnitY;
   };
   if ( State == fsSpZoom || State == fsSpSelect )
   {
      if ( X!=MouseRect->X0 || Y!=MouseRect->Y0 )
      {
         MouseRect->Pull(X,Y);
         float ddx = X2x(MouseRect->X1) - X2x(MouseRect->X0);
         float ddy = Y2y(MouseRect->Y1) - Y2y(MouseRect->Y0);
         if (Prm->Panel3) Prm->Panel3->Text = "d"+Prm->namey + "="+F2S(-ddy, jy+2) + Prm->UnitY;
         if (Prm->Panel3) Prm->Panel3->Text += ";   d"+Prm->namex + "="+F2S(ddx, jx+2) + Prm->UnitX;
      };
   }
   else
   if (State == fsSpDragMarker)
   {
      SpectrumFixMarkerArray[DragMarkerNumber].x = X2x(X);
      this->DrawCS();
      this->Draw();
   }
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::FormClose(TObject *Sender, TCloseAction &Action)
{
   if (BeforeCloseFunctionEnable)
   {
      BeforeCloseFunction(this);
   };
   Action = caFree;
   State = fsSpLock;
};
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (!Shift.Contains(ssShift) && Button == mbLeft && State == fsSpFree && Prm->MainForm->Active)
   {
      // просматриваем текущие маркеры для поиска совпадения
      bool ZoomState = true;
      for (int i=0; i<SpectrumFixMarkerArray.Count; i++)
      {
         int mX1 = x2X(SpectrumFixMarkerArray[i].x);
         if (abs(mX1-X)<16 && Y>Y1 && Y < Y1 + 30)
         {
            // если есть совпадение двигаем маркер
            DragMarkerNumber = i;
            ZoomState = false;
            State=fsSpDragMarker;
            break;
         };
      };
      // если совпадений нет тогда зум
      if (ZoomState)
      {
         State = fsSpZoom;
         MouseRect->Set(X,Y,State);
      };
   }
   else
   if ( Button == mbRight && State == fsSpFree && Prm->MainForm->Active )
   {
      // просматриваем текущие маркеры для поиска совпадения
      bool SelectVar = true;
      for (int i=0; i<SpectrumFixMarkerArray.Count; i++)
      {
         int mX1 = x2X(SpectrumFixMarkerArray[i].x);
         if (abs(mX1-X)<16 && Y>Y1 && Y < Y1 + H1)
         {
            // если есть совпадение вызываем функцию
            if (MarkerFunctionEnable) MarkerFunction(this, Prm->MainForm->ClientOrigin.x + X, Prm->MainForm->ClientOrigin.y + Y, SpectrumFixMarkerArray[i].x);
            SelectVar = false;
            break;
         };
      };
      // если совпадений нет - режим выделения
      if (SelectVar)
      {
         State = fsSpSelect;
         MouseRect->Set(X,Y,State);
      };
   }
   else
   if (Button == mbLeft && Shift.Contains(ssShift) && State == fsSpFree && Prm->MainForm->Active)
   {
      //State = fsSpMarker;
      // просматриваем текущие маркеры для поиска совпадения
      bool NewMarker = true;
      for (int i=0; i<SpectrumFixMarkerArray.Count; i++)
      {
         int mX1 = x2X(SpectrumFixMarkerArray[i].x);
         if (abs(mX1-X)<16 && Y>Y1 && Y < Y1 + H1)
         {
            // если есть совпадение удаляем старый маркер
            SpectrumFixMarkerArray.Remove(i);
            NewMarker = false;
            break;
         };
      };
      // если совпадений нет - добавляем новый маркер
      if (NewMarker) SpectrumFixMarkerArray.Add(TMySpectrumFixMarker(X2x(X)));
      this->DrawCS();
      this->Draw();
   };       
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   if (State == fsSpZoom)
   {
      State = fsSpFree;
      MouseRect->Pull(X,Y);
      MouseRect->Remove();
      if ( MouseRect->X1 - MouseRect->X0 > 10 && MouseRect->Y1 - MouseRect->Y0 > 10 )
      {
         float xi,yi,xa,ya;
         xi = X2x(MouseRect->X0);
         yi = Y2y(MouseRect->Y1);
         xa = X2x(MouseRect->X1);
         ya = Y2y(MouseRect->Y0);
         xmin = xi;
         xmax = xa;
         ymin = yi;
         ymax = ya;
         setmastab();
         DrawCS();
         Draw();
      }
      else
      if ( (MouseRect->X1 - MouseRect->X0) < -10 && (MouseRect->Y1 - MouseRect->Y0) < -10 )
      {
         State = fsSpLock;
         setxyminmax();
         setmastab();
         DrawCS();
         Draw();
         State = fsSpFree;
      };
   };
   if (State == fsSpSelect)
   {
      State = fsSpFree;
      MouseRect->Pull(X,Y);
      MouseRect->Remove();
      if ( data!=NULL && MouseRect->X1 - MouseRect->X0 > 10 && MouseRect->Y1 - MouseRect->Y0 > 10 )
      {
         float xi,xa;
         xi = X2x(MouseRect->X0);
         xa = X2x(MouseRect->X1);
         int tmpb = data[0]->ibegin;
         int tmpe = data[0]->iend;
         data[0]->SetBeginEnd(xi,xa,W1);
         int IMax;
         float SMax;
         ArrayIndexMax(data[0]->Y, data[0]->ibegin, data[0]->iend - 1, SMax, IMax);
         if (Prm->Panel0) Prm->Panel0->Text = "MAX:  " + Prm->namex + "= " + F2S(data[0]->X[IMax], jx+2) + " " + Prm->UnitX + "    " + Prm->namey + "= " + F2S(SMax,jy+2) + " " + Prm->UnitY;
         data[0]->ibegin = tmpb;
         data[0]->iend = tmpe;
      };
   };
   if (State == fsSpDragMarker)
   {
      State = fsSpFree;
   }; 
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::RestoreOriginalState()
{
   State = fsSpLock;
   setxyminmax();
   setmastab();
   DrawCS();
   Draw();
   State = fsSpFree;
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::setxyminmax()
{
   xmax = Prm->xmax;
   xmin = Prm->xmin;
   ymax = Prm->ymax;
   ymin = Prm->ymin;
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::FormDeactivate(TObject *Sender)
{
   State = fsSpFree;
// FormPaint(this);
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  /* if (State == fsSpMarker)
   {
      if (Key == 37)
      {
         SpectrumFixMarker->Pull(SpectrumFixMarker->X - 1);
      }
      else
      if (Key == 39)
      {
         SpectrumFixMarker->Pull(SpectrumFixMarker->X + 1);
      }
      else
      if (Key == 27)
      {
         State = fsSpFree;
         SpectrumFixMarker->Remove();
      };
   };*/
   if (State == fsSpFree)
   {
      if (Key == 37)
      {
         float delta = xstep;
         xmin = xmin - delta;
         xmax = xmax - delta;
         setmastab();
         DrawCS();
         Draw();
      }
      else
      if (Key == 39)
      {
         float delta = xstep;
         xmin = xmin + delta;
         xmax = xmax + delta;
         setmastab();
         DrawCS();
         Draw();
      }
      else
      if (Key == 38)
      {
         float delta = 0.2*ystep;
         ymin = ymin + delta;
         ymax = ymax + delta;
         setmastab();
         DrawCS();
         Draw();
      }
      else
      if (Key == 40)
      {
         float delta = 0.2*ystep;
         ymin = ymin - delta;
         ymax = ymax - delta;
         setmastab();
         DrawCS();
         Draw();
      }
      else
      if (Key == 33) //PgUp
      {
         State = fsSpLock;
         float xmin1 = xmin;
         float xmax1 = xmax;
         float ymin1 = ymin;
         float ymax1 = ymax;
         xmin = xmin1 + 0.25*(xmax1 - xmin1);
         xmax = xmax1 - 0.25*(xmax1 - xmin1);
         ymin = ymin1 + 0.25*(ymax1 - ymin1);
         ymax = ymax1 - 0.25*(ymax1 - ymin1);
         setmastab();
         DrawCS();
         Draw();
         State = fsSpFree;
      }
      else
      if (Key == 34) //PgDown
      {
         State = fsSpLock;
         float xmin1 = xmin;
         float xmax1 = xmax;
         float ymin1 = ymin;
         float ymax1 = ymax;
         xmin = xmin1 - 0.5*(xmax1 - xmin1);
         xmax = xmax1 + 0.5*(xmax1 - xmin1);
         ymin = ymin1 - 0.5*(ymax1 - ymin1);
         ymax = ymax1 + 0.5*(ymax1 - ymin1);
         setmastab();
         DrawCS();
         Draw();
         State = fsSpFree;
      };
   };
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::SpeedButton1Click(TObject *Sender)
{
/*   SpectrumOptions->Edit1->Text = F2S(xmin,jx+2);
   SpectrumOptions->Edit2->Text = F2S(xmax,jx+2);
   SpectrumOptions->Edit3->Text = F2S(ymin,jy+2);
   SpectrumOptions->Edit4->Text = F2S(ymax,jy+2);
   if(SpectrumOptions->ShowModal() == mrOk)
   {
      xmin = S2F(SpectrumOptions->Edit1->Text);
      xmax = S2F(SpectrumOptions->Edit2->Text);
      ymin = S2F(SpectrumOptions->Edit3->Text);
      ymax = S2F(SpectrumOptions->Edit4->Text);
      setmastab();
      DrawCS();
      Draw();
   };*/
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::SpeedButton5Click(TObject *Sender)
{
/*   if (SaveDialog1->Execute())
   {
      Graphics::TBitmap *tmp = new Graphics::TBitmap;
      tmp->Width = W0;
      tmp->Height = H0;
      tmp->Canvas->CopyRect(Rect(0,0,W0,H0),Invisible->Canvas,Rect(X0,Y0,X0+W0,Y0+H0));
      tmp->SaveToFile(SaveDialog1->FileName);
      MessageBeep(64);
      delete tmp;
   };       */
}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::SpeedButton6Click(TObject *Sender)
{

}
//---------------------------------------------------------------------------
void __fastcall TSpectrumFixFigure::SpeedButton7Click(TObject *Sender)
{

}
//---------------------------------------------------------------------------






