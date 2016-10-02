object formRasterSettings: TformRasterSettings
  Left = 416
  Top = 143
  BorderStyle = bsDialog
  Caption = 'formRasterSettings'
  ClientHeight = 408
  ClientWidth = 338
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 4
    Top = 0
    Width = 329
    Height = 201
    Caption = #1048#1085#1076#1080#1082#1072#1090#1086#1088' XX'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object LabelX1: TLabel
      Left = 8
      Top = 24
      Width = 228
      Height = 16
      Caption = #1052#1080#1085#1080#1084#1072#1083#1100#1085#1072#1103' '#1076#1072#1083#1100#1085#1086#1089#1090#1100', '#1086#1090#1089#1095#1105#1090#1086#1074
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LabelX2: TLabel
      Left = 8
      Top = 56
      Width = 234
      Height = 16
      Caption = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1072#1103' '#1076#1072#1083#1100#1085#1086#1089#1090#1100', '#1086#1090#1089#1095#1105#1090#1086#1074
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LabelX3: TLabel
      Left = 8
      Top = 121
      Width = 150
      Height = 16
      Caption = #1052#1080#1085#1080#1084#1072#1083#1100#1085#1099#1081' '#1091#1088#1086#1074#1077#1085#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LabelX4: TLabel
      Left = 8
      Top = 153
      Width = 156
      Height = 16
      Caption = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1099#1081' '#1091#1088#1086#1074#1077#1085#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object EditX1: TEdit
      Left = 248
      Top = 16
      Width = 57
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = '60'
      OnChange = EditX1Change
    end
    object EditX2: TEdit
      Left = 248
      Top = 48
      Width = 57
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Text = '199'
      OnChange = EditX2Change
    end
    object UpDownX1: TUpDown
      Left = 305
      Top = 16
      Width = 16
      Height = 24
      Associate = EditX1
      Min = 0
      Max = 1023
      Position = 60
      TabOrder = 2
      Wrap = False
      OnClick = UpDownX1Click
    end
    object UpDownX2: TUpDown
      Left = 305
      Top = 48
      Width = 16
      Height = 24
      Associate = EditX2
      Min = 0
      Max = 1023
      Position = 199
      TabOrder = 3
      Wrap = False
      OnClick = UpDownX2Click
    end
    object CheckBoxX1: TCheckBox
      Left = 8
      Top = 88
      Width = 281
      Height = 17
      Caption = #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1072#1103' '#1088#1077#1075#1091#1083#1080#1088#1086#1074#1082#1072' '#1103#1088#1082#1086#1089#1090#1080
      Checked = True
      State = cbChecked
      TabOrder = 4
      OnClick = CheckBoxX1Click
    end
    object EditX3: TEdit
      Left = 184
      Top = 112
      Width = 97
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      Text = '0'
      OnChange = EditChange
    end
    object EditX4: TEdit
      Left = 184
      Top = 144
      Width = 97
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      Text = '32767'
      OnChange = EditChange
    end
    object chXXparam: TCheckBox
      Left = 8
      Top = 177
      Width = 313
      Height = 17
      Caption = #1054#1090#1086#1073#1088#1072#1078#1072#1090#1100' '#1086#1090#1089#1095#1077#1090#1099
      Checked = True
      State = cbChecked
      TabOrder = 7
      OnClick = chXXparamClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 4
    Top = 204
    Width = 329
    Height = 199
    Caption = #1048#1085#1076#1080#1082#1072#1090#1086#1088' YY'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object LabelY1: TLabel
      Left = 8
      Top = 24
      Width = 228
      Height = 16
      Caption = #1052#1080#1085#1080#1084#1072#1083#1100#1085#1072#1103' '#1076#1072#1083#1100#1085#1086#1089#1090#1100', '#1086#1090#1089#1095#1105#1090#1086#1074
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LabelY2: TLabel
      Left = 8
      Top = 56
      Width = 234
      Height = 16
      Caption = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1072#1103' '#1076#1072#1083#1100#1085#1086#1089#1090#1100', '#1086#1090#1089#1095#1105#1090#1086#1074
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LabelY3: TLabel
      Left = 8
      Top = 121
      Width = 150
      Height = 16
      Caption = #1052#1080#1085#1080#1084#1072#1083#1100#1085#1099#1081' '#1091#1088#1086#1074#1077#1085#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LabelY4: TLabel
      Left = 8
      Top = 153
      Width = 156
      Height = 16
      Caption = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1099#1081' '#1091#1088#1086#1074#1077#1085#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object EditY1: TEdit
      Left = 248
      Top = 16
      Width = 57
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = '60'
      OnChange = EditY1Change
    end
    object EditY2: TEdit
      Left = 248
      Top = 48
      Width = 57
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Text = '199'
      OnChange = EditY2Change
    end
    object UpDownY1: TUpDown
      Left = 305
      Top = 16
      Width = 16
      Height = 24
      Associate = EditY1
      Min = 0
      Max = 1023
      Position = 60
      TabOrder = 2
      Wrap = False
      OnClick = UpDownY1Click
    end
    object UpDownY2: TUpDown
      Left = 305
      Top = 48
      Width = 16
      Height = 24
      Associate = EditY2
      Min = 0
      Max = 1023
      Position = 199
      TabOrder = 3
      Wrap = False
      OnClick = UpDownY2Click
    end
    object CheckBoxY1: TCheckBox
      Left = 8
      Top = 88
      Width = 281
      Height = 17
      Caption = #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1072#1103' '#1088#1077#1075#1091#1083#1080#1088#1086#1074#1082#1072' '#1103#1088#1082#1086#1089#1090#1080
      Checked = True
      State = cbChecked
      TabOrder = 4
      OnClick = CheckBoxY1Click
    end
    object EditY3: TEdit
      Left = 184
      Top = 112
      Width = 97
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      Text = '0'
      OnChange = EditChange
    end
    object EditY4: TEdit
      Left = 184
      Top = 144
      Width = 97
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      Text = '32767'
      OnChange = EditChange
    end
    object chYYparam: TCheckBox
      Left = 8
      Top = 177
      Width = 313
      Height = 17
      Caption = #1054#1090#1086#1073#1088#1072#1078#1072#1090#1100' '#1086#1090#1089#1095#1077#1090#1099
      Checked = True
      State = cbChecked
      TabOrder = 7
      OnClick = chYYparamClick
    end
  end
end
