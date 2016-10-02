object formMeasureSettings: TformMeasureSettings
  Left = 263
  Top = 157
  Width = 313
  Height = 243
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1080#1079#1084#1077#1088#1077#1085#1080#1081
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ThresholdEdit: TLabeledEdit
    Left = 8
    Top = 176
    Width = 137
    Height = 24
    EditLabel.Width = 131
    EditLabel.Height = 16
    EditLabel.Caption = #1055#1086#1088#1086#1075' '#1086#1073#1085#1072#1088#1091#1078#1077#1085#1080#1103
    EditLabel.Font.Charset = DEFAULT_CHARSET
    EditLabel.Font.Color = clWindowText
    EditLabel.Font.Height = -13
    EditLabel.Font.Name = 'MS Sans Serif'
    EditLabel.Font.Style = []
    EditLabel.ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    LabelPosition = lpAbove
    LabelSpacing = 3
    ParentFont = False
    TabOrder = 0
    Text = '2000'
    OnChange = ThresholdEditChange
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 289
    Height = 145
    Caption = #1054#1073#1083#1072#1089#1090#1100' '#1080#1079#1084#1077#1088#1077#1085#1080#1081
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 161
      Height = 16
      Caption = #1052#1080#1085#1080#1084#1072#1083#1100#1085#1072#1103' '#1076#1072#1083#1100#1085#1086#1089#1090#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 8
      Top = 56
      Width = 56
      Height = 16
      Caption = #1054#1090#1089#1095#1105#1090#1099
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 168
      Top = 56
      Width = 43
      Height = 16
      Caption = #1052#1077#1090#1088#1099
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 8
      Top = 88
      Width = 167
      Height = 16
      Caption = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1072#1103' '#1076#1072#1083#1100#1085#1086#1089#1090#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 8
      Top = 120
      Width = 56
      Height = 16
      Caption = #1054#1090#1089#1095#1105#1090#1099
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 168
      Top = 120
      Width = 43
      Height = 16
      Caption = #1052#1077#1090#1088#1099
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object MinEdit: TEdit
      Left = 216
      Top = 48
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
      OnChange = MinEditChange
    end
    object MaxEdit: TEdit
      Left = 216
      Top = 112
      Width = 57
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Text = '600'
      OnChange = MaxEditChange
    end
    object MinSEdit: TEdit
      Left = 72
      Top = 48
      Width = 65
      Height = 21
      TabOrder = 2
      Text = 'MinSEdit'
      OnChange = MinSEditChange
    end
    object MinUpDown: TUpDown
      Left = 136
      Top = 48
      Width = 17
      Height = 25
      Min = 0
      Max = 1024
      Position = 60
      TabOrder = 3
      Wrap = False
      OnClick = MinUpDownClick
    end
    object MaxUpDown: TUpDown
      Left = 136
      Top = 112
      Width = 17
      Height = 25
      Min = 0
      Max = 1024
      Position = 600
      TabOrder = 4
      Wrap = False
      OnClick = MaxUpDownClick
    end
  end
  object MaxSEdit: TEdit
    Left = 80
    Top = 120
    Width = 65
    Height = 21
    TabOrder = 2
    Text = 'Edit1'
    OnChange = MaxSEditChange
  end
  object BearingEdit: TLabeledEdit
    Left = 160
    Top = 176
    Width = 137
    Height = 24
    EditLabel.Width = 138
    EditLabel.Height = 16
    EditLabel.Caption = #1041#1072#1079#1072' '#1087#1077#1083#1077#1085#1075#1072#1094#1080#1080', '#1084#1084
    EditLabel.Font.Charset = DEFAULT_CHARSET
    EditLabel.Font.Color = clWindowText
    EditLabel.Font.Height = -13
    EditLabel.Font.Name = 'MS Sans Serif'
    EditLabel.Font.Style = []
    EditLabel.ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    LabelPosition = lpAbove
    LabelSpacing = 3
    ParentFont = False
    TabOrder = 3
    Text = '100'
    OnChange = BearingEditChange
  end
end
