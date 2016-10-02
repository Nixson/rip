
#pragma hdrstop

#include "CreateCoord.h"
#include <stdio.h>
#include <math.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

int CreateCoord(int Nr, int Na, double *Data, double d_R, double d_a, double Threshold, double R_min, double R_max, double a_min, double a_max, float **X, float **Y)
{
 int Count,step_a,step_r,N_dr,N_da,p_r,p_a,num_r,num_a,num,n,n_max,e_max;
 double dR,da,ampl,dis,angl,ampl_max,R,a,dis_0,angle_0;
 double* Data_new;
 double* Coord;
 double* Sum;

 dR = (R_max-R_min)/Nr;
 da = (a_max-a_min)/(Na-1);

 N_dr = d_R/dR;
 N_da = d_a/da;

 step_a = N_da/2;
 step_r = N_dr/2;

 if (N_dr<2)
 {
  N_dr = 1;
  step_r = 1;
 }

 if (N_da<2)
 {
  N_da= 1;
  step_a = 1;
 }

 p_r = Nr;
 p_a = Na;
 num_r = 0;
 num_a = 0;

 while ((p_r-N_dr)>0)
 {
  p_r = p_r-step_r;
  num_r = num_r+1;
 }
  num_r = num_r+1;

  while ((p_a-N_da)>0)
 {
  p_a = p_a-step_a;
  num_a = num_a+1;
 }
  num_a = num_a+1;

  Data_new = new double [3*num_a*num_r];

  int ttt = 0;
  int k_max = 0;
   ampl_max = 0;

  for (int m=0; m<num_a; m++)
  {
	  for (int l=0; l<num_r; l++)
	  {
		  ampl = 0;
		  num = 0;
		  dis = 0;
		  angl = 0;

		  for (int i= m*step_a; (i<N_da+m*step_a) && (i<Na); i++)
		  {
                    for (int j= l*step_r; (j<N_dr+l*step_r) && (j<Nr); j++)
                    {
                      if (Data[i*Nr+j]>=Threshold)
                      {
                       ampl = ampl+Data[i*Nr+j];
                       dis = dis + (R_min + dR/2) + dR*j;
                       angl = angl + a_min + da*i;
                       num = num + 1;
                      }
                    }
		  }
                  if(num > 0)
                  {
		    Data_new[3*ttt] = ampl/num;
		    Data_new[3*ttt+1] = dis /num;
		    Data_new[3*ttt+2] = angl /num;
                  }
                  else
                  {
		    Data_new[3*ttt] = 0;
		    Data_new[3*ttt+1] = 0;
		    Data_new[3*ttt+2] = 0;
                  }
                  if ((Data_new[3*ttt]-ampl_max)>1e-13)
                  {
                   ampl_max = Data_new[3*ttt];
                   k_max = ttt;
                  }
                  ttt = ttt+1;
	  }
  }

  int ee1=0;
  Coord = new double[2*ttt];

  for (int ee = 0; ee < ttt; ee++)
  {
   if (fabs(Data_new[3*ee]-ampl_max)<1e-13)
   {
    Coord[2*ee1]  = Data_new[3*ee+1];
    Coord[2*ee1+1] = Data_new[3*ee+2];
    ee1=ee1+1;
    }
   }

   R = Coord[0];
   a = Coord[1];

   delete[] Data_new;

  if (ee1>=1)
  {
   Sum = new double[3*ee1];

   int ttt1 = 0;
   n_max = 0;
   e_max = 0;
   for (int e1 = 0; e1 < ee1; e1++)
   {
    dis_0 = 0;
    angle_0 = 0;
    n = 0;
    for (int e2 = 0; e2 < ee1; e2++)
    {
     if (fabs(Coord[2*e2]-Coord[2*e1])<=step_r*dR)
     {
      if (fabs(Coord[2*e2+1]-Coord[2*e1+1])<=step_a*da)
      {
       dis_0 = dis_0 + Coord[2*e2];
       angle_0 = angle_0 + Coord[2*e2+1];
       n = n+1;
      }
     }
    }
    Sum[3*ttt1] = dis_0/n;
    Sum[3*ttt1+1] = angle_0/n;
    Sum[3*ttt1+2] = n;

    if(Sum[3*ttt1+2]>n_max)
    {
     n_max = Sum[3*ttt1+2];
     e_max = ttt1;
    }
    ttt1 = ttt1 + 1;
   }
   R = Sum[3*e_max];
   a = Sum[3*e_max+1];
   delete[] Sum;
  }

  delete[] Coord;

  Count = 1;
  *X = new float[Count];
  *Y = new float[Count];

  *X[0] = R;
  *Y[0] = a;

return Count;   
}








