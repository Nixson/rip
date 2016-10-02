object formRaster: TformRaster
  Left = 513
  Top = 172
  Width = 559
  Height = 427
  Caption = #1056#1072#1089#1090#1088#1086#1074#1099#1081' '#1080#1085#1076#1080#1082#1072#1090#1086#1088
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 551
    Height = 57
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    Visible = False
    object Label1: TLabel
      Left = 8
      Top = 25
      Width = 27
      Height = 16
      Caption = #1052#1080#1085
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 184
      Top = 25
      Width = 33
      Height = 16
      Caption = #1052#1072#1082#1089
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object CheckBox1: TCheckBox
      Left = 8
      Top = 0
      Width = 313
      Height = 17
      Caption = #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1086#1077' '#1084#1072#1089#1096#1090#1072#1073#1080#1088#1086#1074#1072#1085#1080#1077' '#1103#1088#1082#1086#1089#1090#1080
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      State = cbChecked
      TabOrder = 0
      OnClick = CheckBox1Click
    end
    object Edit1: TEdit
      Left = 40
      Top = 24
      Width = 97
      Height = 24
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Text = '0'
      OnChange = Scaling
    end
    object Edit2: TEdit
      Left = 224
      Top = 24
      Width = 97
      Height = 24
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = '0'
      OnChange = Scaling
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 57
    Width = 551
    Height = 343
    Align = alClient
    TabOrder = 1
  end
end
