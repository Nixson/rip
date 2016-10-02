object formProgress: TformProgress
  Left = 421
  Top = 179
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'formProgress'
  ClientHeight = 108
  ClientWidth = 350
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object Label2: TLabel
    Left = 88
    Top = 80
    Width = 3
    Height = 13
  end
  object Label3: TLabel
    Left = 88
    Top = 64
    Width = 3
    Height = 13
  end
  object ProgressBar1: TProgressBar
    Left = 16
    Top = 32
    Width = 329
    Height = 17
    Min = 0
    Max = 100
    Smooth = True
    Step = 1
    TabOrder = 0
  end
  object BitBtn1: TBitBtn
    Left = 256
    Top = 72
    Width = 75
    Height = 25
    TabOrder = 1
    Visible = False
    Kind = bkOK
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 16
    Top = 80
  end
end
