#ifndef ZVECTBASE_H
#define ZVECTBASE_H
#include "mem.h"
#include "limits.h"
#include <vcl.h>
//---------------------------------------------------------------------------
// Базовый вектор - динамический массив
//  управляет размером
//  доступ к элементам по индексу (на чтение)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
template <class T> class TCircularBuffer
{
protected:
    int FSize;     // размер массива
    int FCount;    // Количество элементов
    int FFirst;     // указатель на первый элемент кольца
    int FLast;      // указатель на последний элемент кольца
//----------------------
public:
    T* Data;
// Выделение памяти
   void __fastcall SetSize(int Value)
   {
     delete[] Data;
     Data = new T[Value];
     ZeroMemory(Data, Value*sizeof(T));
     FSize = Value;
     Clear();
   };
//----------------------
   __fastcall TCircularBuffer(int Size)
   {
      Data = new T[Size];
      ZeroMemory(Data, Size*sizeof(T));
      FSize = Size;
      Clear();
   };
//----------------------
   virtual __fastcall ~TCircularBuffer()
   {
      delete[] Data;
   };
//----------------------
   // На чтение
   T& __fastcall operator []( int loc ) const
   {
      int i = (FFirst + loc)%FSize;
      return Data[i];
   };
//----------------------
   void __fastcall Clear()
   {
      FFirst = 0;
      FLast = -1;
      FCount = 0;
   };
//----------------------
   int __fastcall Add(const T& t )
   {
      FLast = (FLast + 1)%FSize;
      if (FCount == FSize)
      {
         FFirst = (FFirst + 1)%FSize;
      }
      else FCount++;
      Data[FLast] = t;
      return 1;
   };
//----------------------
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
template <class T> class TBaseVector
{
protected:
    unsigned FLimit;     // Выделено памяти
    unsigned FDelta;     // Инкримент выделения памяти
    int FCount;     // Колличество элементов (всегда FCount < FLimit)

// Выделение памяти
void __fastcall SetCount(int Value)
{
     Reallocate(Value);
     FCount = Value;
};

bool __fastcall Reallocate( unsigned newSz)
{
    if( newSz <= FLimit || FDelta == 0 )   return false;
    int nd = ((newSz - FLimit)%FDelta) ? (((newSz - FLimit)+FDelta)/FDelta)*FDelta : (newSz - FLimit);
    int sz = FLimit + nd;
    T *temp = new T[sz];
    memmove(temp, Data, FLimit * sizeof(T));
    delete[] Data;
    Data = temp;
    FLimit = sz;
    return true;
};

public:
    T* Data;

__fastcall TBaseVector(unsigned ADelta, unsigned ALimit) :
  FDelta(ADelta),
  Data(new T[ALimit]),
  FLimit(ALimit),
  FCount(0)
{
};

__fastcall TBaseVector(const TBaseVector<T> &other):
  FDelta(other.FDelta),
  Data(new T[other.FLimit]),
  FLimit(other.FLimit),
  FCount(other.FCount)
{
  memmove(Data, other.Data, FCount * sizeof(T));
};

virtual __fastcall ~TBaseVector()
{
  delete[] Data;
};

// На чтение
T& __fastcall operator []( int loc ) const
{
   if( loc < 0 || loc >= FLimit ) throw "Index out of bounds";
   return Data[loc];
};

int __fastcall Remove( int loc )
{
    if( loc >= FCount || loc < 0) return 0;
    FCount--;
    for( int cur = loc; cur < FCount; cur++ )
      Data[cur] = Data[cur+1];
    return 1;
};

void __fastcall Clear()
{  FCount = 0;     };

T& __fastcall First()
{  return Data[0]; };

T& __fastcall Last()
{  return Data[FCount - 1]; };


};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
template <class T> class TBaseArray: public TBaseVector<T>
{
   public:
      int __fastcall GetCount() { return FCount; };
      void __fastcall SetCount(int Value) {TBaseVector<T>::SetCount(Value);};

   __fastcall TBaseArray(unsigned ADelta, unsigned ALimit) :
      TBaseVector<T>(ADelta, ALimit) {};

   __fastcall TBaseArray(const TBaseArray<T> &init ) :
      TBaseVector<T>(init) {};


   int __fastcall Remove( int loc )
   {
      return TBaseVector<T>::Remove(loc);
   }

   int __fastcall Remove( const T& t )
   {
      return Remove(Find(t));
   };

   int __fastcall Add( const T& t )
   {
      Reallocate( FCount + 1 );
      Data[FCount] = t;
      FCount++;
      return 1;
   };

   int __fastcall Insert( const T& t, int loc )
   {
      if( loc >= (int)FLimit && !Reallocate(loc + 1) )      return 0;
      if( FCount == (int)FLimit && !Reallocate(FLimit + 1) )   return 0;
      if( loc > FCount )  FCount = loc;
      memmove(&Data[loc + 1], &Data[loc], (FCount - loc)*sizeof(T));
      Data[loc] = t;
      FCount++;
      return 1;
   };

   void __fastcall Move(int CurIndex, int NewIndex)
   {
      if(CurIndex == NewIndex) return;
      T t = Data[CurIndex];
      Remove(CurIndex);
      Insert(t, NewIndex);
   };

   int __fastcall AddArray(const TBaseArray<T>& array)
   {
      Reallocate(FCount + array.FCount);
      memmove(&Data[FCount], array.Data, array.FCount * sizeof(T));
      FCount += array.FCount;
      return array.FCount;
   };

   void __fastcall AddElements(T* t_ar, int size)
   {
      Reallocate(FCount + size);
      memmove(&Data[FCount], t_ar, size * sizeof(T));
      FCount += size;
   }

   int __fastcall Find( const T& t ) const
   {
      for( int loc = 0; loc < FCount; loc++ )
      if( Data[loc] == t )
      return loc;
      return -1;
   }

   T& __fastcall operator []( int loc )
   {
      Reallocate( loc+1 );
      return Data[loc];
   }

   T& __fastcall operator []( int loc ) const
   {
      return TBaseVector<T>::operator []( loc );
   }

__property int Count = {read = GetCount, write = SetCount};
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
template <class T> class TPointerArray: public TBaseArray<T*>
{
public:
   __fastcall TPointerArray( unsigned ADelta, unsigned ALimit) : TBaseArray<T*>(ADelta, ALimit) {}

   int __fastcall Destroy( T* t )
   {
      delete t;
      return Remove(t);
   };

   int __fastcall Destroy( int loc )
   {
      delete Data[loc];
      return Remove(loc);
   };

   void __fastcall DestroyAll()
   {
      for(int i = 0; i < FCount; i++)
      if(Data[i]) delete Data[i];
      Clear();
   };

   __fastcall ~TPointerArray()
   {
      DestroyAll();
   }
};
//---------------------------------------------------------------------------
template <class T> class TMyPointerArray: public TPointerArray<T>
{
public:
   __fastcall TMyPointerArray() : TPointerArray<T>(10, 10) {};

   __fastcall SortByIncrease()
   {
      int k;
      T x;
      for (int i=0; i<FCount-1; i++)
      {
         k = i;
         x = Data[i][0];
         for (int j=i+1; j<FCount; j++)
         {
            if (Data[j][0] < x)
            {
               k = j;
               x = Data[k][0];
            };
         };
         Data[k][0] = Data[i][0];
         Data[i][0] = x;
      };
      return 0;
   };

   __fastcall SortByDecrease()
   {
      int k;
      T x;
      for (int i=0; i<FCount-1; i++)
      {
         k = i;
         x = Data[i][0];
         for (int j=i+1; j<FCount; j++)
         {
            if (Data[j][0] > x)
            {
               k = j;
               x = Data[k][0];
            };
         };
         Data[k][0] = Data[i][0];
         Data[i][0] = x;
      };
      return 0;
   };
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
template <class T> class TMyArray: public TBaseArray<T>    // массив простых данных
{
public:
   __fastcall TMyArray() : TBaseArray<T>(100, 100) {};

   __fastcall SortByIncrease()
   {
      int k;
      T x;
      for (int i=0; i<FCount-1; i++)
      {
         k = i;
         x = Data[i];
         for (int j=i+1; j<FCount; j++)
         {
            if (Data[j] < x)
            {
               k = j;
               x = Data[k];
            };
         };
         Data[k] = Data[i];
         Data[i] = x;
      };
      return;
   };

   __fastcall SortByDecrease()
   {
      int k;
      T x;
      for (int i=0; i<FCount-1; i++)
      {
         k = i;
         x = Data[i];
         for (int j=i+1; j<FCount; j++)
         {
            if (Data[j] > x)
            {
               k = j;
               x = Data[k];
            };
         };
         Data[k] = Data[i];
         Data[i] = x;
      };
      return;
   };

};
//---------------------------------------------------------------------------
template <class T> class TMyArray1000: public TBaseArray<T>    // массив простых данных
{
public:
   __fastcall TMyArray1000() : TBaseArray<T>(1000, 1000) {};
};
//---------------------------------------------------------------------------
typedef TMyArray<double> TMyDoubleArray;
typedef TMyArray<int> TMyIntArray;
//typedef TMyArray<AnsiString> TMyStringArray; //вызывает ошибки при количестве строк больше 50
// вместо него лучше использовать TStringList
#endif