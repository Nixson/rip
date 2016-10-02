object ThresholdForm: TThresholdForm
  Left = 203
  Top = 205
  BorderStyle = bsDialog
  Caption = 'ThresholdForm'
  ClientHeight = 177
  ClientWidth = 289
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 120
    Width = 105
    Height = 16
    Caption = #1059#1088#1086#1074#1077#1085#1100' '#1087#1086#1088#1086#1075#1072
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 8
    Width = 101
    Height = 16
    Caption = #1052#1080#1085'. '#1076#1072#1083#1100#1085#1086#1089#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 8
    Top = 64
    Width = 107
    Height = 16
    Caption = #1052#1072#1082#1089'. '#1076#1072#1083#1100#1085#1086#1089#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Button1: TSpeedButton
    Left = 120
    Top = 8
    Width = 161
    Height = 161
    Caption = #1053#1054#1056#1052#1040
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 8
    Top = 144
    Width = 89
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = '10'#160'000'
  end
  object UpDown1: TUpDown
    Left = 97
    Top = 144
    Width = 16
    Height = 24
    Associate = Edit1
    Min = 0
    Max = 32767
    Position = 10000
    TabOrder = 1
    Wrap = False
  end
  object Edit2: TEdit
    Left = 8
    Top = 32
    Width = 89
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = '0'
  end
  object Edit3: TEdit
    Left = 8
    Top = 88
    Width = 89
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = '0'
  end
  object UpDown2: TUpDown
    Left = 97
    Top = 32
    Width = 16
    Height = 24
    Associate = Edit2
    Min = 0
    Max = 10000
    Position = 0
    TabOrder = 4
    Wrap = False
  end
  object UpDown3: TUpDown
    Left = 97
    Top = 88
    Width = 16
    Height = 24
    Associate = Edit3
    Min = 0
    Max = 10000
    Position = 0
    TabOrder = 5
    Wrap = False
  end
end
