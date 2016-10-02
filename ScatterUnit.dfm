object ScatterForm: TScatterForm
  Left = 879
  Top = 222
  Width = 321
  Height = 363
  Caption = #1048#1079#1084#1077#1088#1077#1085#1080#1077' '#1069#1055#1056
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Visible = True
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object HostLabel: TLabel
    Left = 8
    Top = 8
    Width = 91
    Height = 13
    Caption = #1040#1076#1088#1077#1089' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072
  end
  object Label1: TLabel
    Left = 8
    Top = 48
    Width = 85
    Height = 13
    Caption = #1055#1086#1088#1090' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072
  end
  object Label2: TLabel
    Left = 144
    Top = 72
    Width = 45
    Height = 13
    Caption = #1064#1072#1075' '#1087#1086' X'
  end
  object Label3: TLabel
    Left = 232
    Top = 72
    Width = 45
    Height = 13
    Caption = #1064#1072#1075' '#1087#1086' Y'
  end
  object AddressEdit: TEdit
    Left = 8
    Top = 24
    Width = 129
    Height = 21
    TabOrder = 0
    Text = '192.168.114.33'
    OnChange = AddressEditChange
  end
  object ScanButtonAvg: TButton
    Left = 144
    Top = 40
    Width = 161
    Height = 25
    Caption = #1048#1079#1084#1077#1088#1077#1085#1080#1077' c '#1091#1089#1088#1077#1076#1085#1077#1085#1080#1077#1084
    Enabled = False
    TabOrder = 1
    OnClick = ScanButtonAvgClick
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 128
    Width = 129
    Height = 137
    Caption = #1056#1091#1095#1085#1086#1077' '#1091#1087#1088#1072#1074#1083#1077#1085#1080#1077
    TabOrder = 2
    object DownSpeedButton: TSpeedButton
      Left = 48
      Top = 96
      Width = 33
      Height = 33
      OnClick = DownSpeedButtonClick
    end
    object LeftSpeedButton: TSpeedButton
      Left = 8
      Top = 56
      Width = 33
      Height = 33
      OnClick = LeftSpeedButtonClick
    end
    object RightSpeedButton: TSpeedButton
      Left = 88
      Top = 56
      Width = 33
      Height = 33
      OnClick = RightSpeedButtonClick
    end
    object UpSpeedButton: TSpeedButton
      Left = 48
      Top = 16
      Width = 33
      Height = 33
      OnClick = UpSpeedButtonClick
    end
  end
  object ScanButton: TButton
    Left = 144
    Top = 8
    Width = 161
    Height = 25
    Caption = #1048#1079#1084#1077#1088#1077#1085#1080#1077' '#1073#1077#1079' '#1091#1089#1088#1077#1076#1085#1077#1085#1080#1103
    TabOrder = 3
    OnClick = ScanButtonClick
  end
  object ApplyAddressButton: TButton
    Left = 8
    Top = 96
    Width = 129
    Height = 25
    Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
    TabOrder = 4
    OnClick = ApplyAddressButtonClick
  end
  object Memo1: TMemo
    Left = 144
    Top = 152
    Width = 161
    Height = 169
    TabOrder = 5
  end
  object PortEdit: TEdit
    Left = 8
    Top = 64
    Width = 129
    Height = 21
    TabOrder = 6
    Text = '3333'
    OnChange = AddressEditChange
  end
  object Edit1: TEdit
    Left = 8
    Top = 272
    Width = 129
    Height = 21
    TabOrder = 7
    Text = 'Edit1'
  end
  object Button1: TButton
    Left = 8
    Top = 296
    Width = 129
    Height = 25
    Caption = #1047#1072#1076#1072#1090#1100
    TabOrder = 8
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 144
    Top = 120
    Width = 161
    Height = 25
    Caption = #1054#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1077
    TabOrder = 9
    OnClick = Button2Click
  end
  object StepEditX: TEdit
    Left = 144
    Top = 88
    Width = 73
    Height = 21
    TabOrder = 10
    Text = '1'
  end
  object StepEditY: TEdit
    Left = 232
    Top = 88
    Width = 73
    Height = 21
    TabOrder = 11
    Text = '1'
  end
  object IdUDPServer1: TIdUDPServer
    Bindings = <>
    DefaultPort = 3333
    OnUDPRead = IdUDPServer1UDPRead
    Left = 168
    Top = 200
  end
  object IdUDPClient1: TIdUDPClient
    Active = True
    Host = '192.168.114.33'
    Port = 3333
    Left = 208
    Top = 208
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 256
    Top = 208
  end
end
