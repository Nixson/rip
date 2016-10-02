//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("MainUnit.cpp", formMain);
USEFORM("SettingsUnit.cpp", formSettings);
USEFORM("DrawUnit.cpp", formDraw);
USEFORM("ProgressUnit.cpp", formProgress);
USEFORM("RasterUnit.cpp", formRaster);
USEFORM("RasterSettingsUnit.cpp", formRasterSettings);
USEFORM("ThresholdUnit.cpp", ThresholdForm);
USEFORM("TargetUnit.cpp", formTargetDraw);
USEFORM("MeasureUnit.cpp", formMeasureSettings);
USEFORM("ControlOptionsUnit.cpp", FormControlOptions);
USEFORM("OrgMUnit.cpp", FormOrgM);
USEFORM("OrgDMUnit.cpp", FormOrgDM);
USEFORM("OrgCMUnit.cpp", FormOrgCM);
USEFORM("ScatterUnit.cpp", ScatterForm);
USEFORM("formChanels.cpp", Fchanels);
USEFORM("SettingsUnitRLM.cpp", SettingsUnitForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
       Application->Initialize();
       Application->CreateForm(__classid(TformMain), &formMain);
                 Application->CreateForm(__classid(TSettingsUnitForm), &SettingsUnitForm);
                 Application->CreateForm(__classid(TformSettings), &formSettings);
                 Application->CreateForm(__classid(TformProgress), &formProgress);
                 Application->CreateForm(__classid(TformRasterSettings), &formRasterSettings);
                 Application->CreateForm(__classid(TThresholdForm), &ThresholdForm);
                 Application->CreateForm(__classid(TformMeasureSettings), &formMeasureSettings);
                 Application->CreateForm(__classid(TFormControlOptions), &FormControlOptions);
                 Application->CreateForm(__classid(TFormOrgM), &FormOrgM);
                 Application->CreateForm(__classid(TFormOrgDM), &FormOrgDM);
                 Application->CreateForm(__classid(TFormOrgCM), &FormOrgCM);
                 Application->CreateForm(__classid(TFchanels), &Fchanels);
                 Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   catch (...)
   {
       try
       {
          throw Exception("");
       }
       catch (Exception &exception)
       {
          Application->ShowException(&exception);
       }
   }
   return 0;
}
//---------------------------------------------------------------------------
