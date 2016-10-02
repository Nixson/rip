object formDraw: TformDraw
  Left = 298
  Top = 214
  Width = 777
  Height = 393
  Caption = 'formDraw'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  PixelsPerInch = 96
  TextHeight = 13
  object Chart: TChart
    Left = 0
    Top = 0
    Width = 769
    Height = 366
    BackWall.Brush.Color = clWhite
    BackWall.Brush.Style = bsClear
    Title.Text.Strings = (
      '')
    BottomAxis.Automatic = False
    BottomAxis.AutomaticMinimum = False
    BottomAxis.LabelsOnAxis = False
    BottomAxis.LabelsSeparation = 20
    BottomAxis.Title.Caption = #1054#1089#1100' X'
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.ExactDateTime = False
    LeftAxis.Increment = 5
    LeftAxis.LabelsMultiLine = True
    LeftAxis.Maximum = 477
    LeftAxis.Minimum = -1000
    LeftAxis.Title.Caption = #1054#1089#1100' Y'
    Legend.Visible = False
    View3D = False
    Align = alClient
    TabOrder = 0
    object cPlot: TFastLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clRed
      Title = 'cPlot'
      LinePen.Color = clRed
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object cCoherentAccum: TFastLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clGreen
      Title = 'cCoherentAccum'
      LinePen.Color = clGreen
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object cPlotI: TFastLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clRed
      Title = 'cPlot'
      LinePen.Color = clRed
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object cPlotQ: TFastLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clGreen
      Title = 'cPlot'
      LinePen.Color = clGreen
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 208
    Top = 112
    object N1: TMenuItem
      Caption = #1085#1089
      Checked = True
      GroupIndex = 1
      RadioItem = True
      OnClick = N1Click
    end
    object N2: TMenuItem
      Caption = #1084#1082#1089
      GroupIndex = 2
      RadioItem = True
      OnClick = N2Click
    end
    object N3: TMenuItem
      Caption = #1084
      GroupIndex = 3
      RadioItem = True
      OnClick = N3Click
    end
    object Samples1: TMenuItem
      Caption = #1086#1090#1089#1095#1105#1090#1099
      GroupIndex = 3
      OnClick = Samples1Click
    end
  end
end
