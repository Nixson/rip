object formMain: TformMain
  Left = 282
  Top = 31
  Width = 703
  Height = 724
  Caption = 'formMain'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIForm
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poMainFormCenter
  WindowState = wsMaximized
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Memo: TMemo
    Left = 407
    Top = 0
    Width = 280
    Height = 647
    Align = alRight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Pitch = fpFixed
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
    Visible = False
  end
  object sbMainStatusBar: TStatusBar
    Left = 0
    Top = 647
    Width = 687
    Height = 19
    Panels = <
      item
        Width = 100
      end
      item
        Alignment = taRightJustify
        Width = 200
      end
      item
        Alignment = taRightJustify
        Width = 50
      end>
    SimplePanel = False
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 345
    Height = 647
    Align = alLeft
    TabOrder = 2
    object Button1: TButton
      Left = 144
      Top = 888
      Width = 25
      Height = 25
      Caption = 'Fill Draw'
      TabOrder = 0
      Visible = False
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 72
      Top = 889
      Width = 25
      Height = 7
      Caption = 'Send ARP Req'
      TabOrder = 1
      Visible = False
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 72
      Top = 896
      Width = 25
      Height = 9
      Caption = 'Send Bulk'
      TabOrder = 2
      Visible = False
      OnClick = Button3Click
    end
    object bSendParam: TButton
      Left = 16
      Top = 8
      Width = 145
      Height = 25
      Caption = #1054#1090#1087#1088#1072#1074#1080#1090#1100' '#1087#1072#1088#1072#1084#1077#1090#1088#1099
      TabOrder = 3
      OnClick = bSendParamClick
    end
    object bPlot: TButton
      Left = 8
      Top = 888
      Width = 65
      Height = 9
      Caption = '1 '#1080#1084#1087#1091#1083#1100#1089' (0x2)'
      TabOrder = 4
      Visible = False
      OnClick = bPlotClick
    end
    object bVarPlot: TButton
      Left = 8
      Top = 896
      Width = 65
      Height = 9
      Caption = '32 '#1080#1084#1087#1091#1083#1100#1089#1072' (0x4)'
      TabOrder = 5
      Visible = False
      OnClick = bVarPlotClick
    end
    object GroupBox2: TGroupBox
      Left = 8
      Top = 40
      Width = 161
      Height = 417
      Caption = #1053#1077#1087#1088#1077#1088#1099#1074#1085#1099#1081' '#1079#1072#1087#1088#1086#1089
      TabOrder = 6
      object SpeedButton1: TSpeedButton
        Left = 144
        Top = 8
        Width = 9
        Height = 9
        OnClick = SpeedButton1Click
      end
      object leNumberOfMultOsc: TLabeledEdit
        Left = 8
        Top = 32
        Width = 145
        Height = 21
        EditLabel.Width = 91
        EditLabel.Height = 13
        EditLabel.Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1087#1072#1095#1077#1082
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 0
        Text = '900'
        OnChange = leNumberOfMultOscChange
      end
      object leMulOscDelay: TLabeledEdit
        Left = 8
        Top = 72
        Width = 145
        Height = 21
        EditLabel.Width = 90
        EditLabel.Height = 13
        EditLabel.Caption = #1055#1077#1088#1080#1086#1076' '#1087#1072#1095#1077#1082', '#1084#1089
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 1
        Text = '100'
      end
      object leRasterPeriod: TLabeledEdit
        Left = 8
        Top = 112
        Width = 145
        Height = 21
        EditLabel.Width = 131
        EditLabel.Height = 13
        EditLabel.Caption = #1048#1085#1090#1077#1088#1074#1072#1083' '#1085#1072#1082#1086#1087#1083#1077#1085#1080#1103', '#1096#1090
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 2
        Text = '10'
      end
      object Panel2: TPanel
        Left = 8
        Top = 179
        Width = 145
        Height = 33
        TabOrder = 3
        object rbRasterAF: TRadioButton
          Left = 8
          Top = 8
          Width = 41
          Height = 17
          Caption = #1040#1060
          Checked = True
          TabOrder = 0
          TabStop = True
          OnClick = Action1Execute
        end
        object rbRasterA: TRadioButton
          Left = 59
          Top = 8
          Width = 33
          Height = 17
          Caption = #1040
          TabOrder = 1
          OnClick = Action1Execute
        end
        object rbRasterF: TRadioButton
          Left = 104
          Top = 8
          Width = 33
          Height = 17
          Caption = #1060
          TabOrder = 2
          OnClick = Action1Execute
        end
      end
      object leScanSector: TLabeledEdit
        Left = 8
        Top = 152
        Width = 145
        Height = 21
        EditLabel.Width = 140
        EditLabel.Height = 13
        EditLabel.Caption = #1057#1077#1082#1090#1086#1088' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1103', '#1075#1088#1072#1076
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 4
        Text = '90'
      end
      object btnCMD9: TButton
        Left = 8
        Top = 279
        Width = 145
        Height = 26
        Caption = 'btnCMD9'
        TabOrder = 5
        OnClick = btnCMD9Click
      end
    end
    object leExpName: TLabeledEdit
      Left = 8
      Top = 552
      Width = 161
      Height = 21
      EditLabel.Width = 126
      EditLabel.Height = 13
      EditLabel.Caption = #1053#1072#1079#1074#1072#1085#1080#1077' '#1101#1082#1089#1087#1077#1088#1080#1084#1077#1085#1090#1072
      LabelPosition = lpAbove
      LabelSpacing = 3
      TabOrder = 7
    end
    object Button5: TButton
      Left = 8
      Top = 584
      Width = 161
      Height = 25
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074#1080#1076#1080#1084#1099#1077' '#1076#1072#1085#1085#1099#1077
      TabOrder = 8
      OnClick = Button5Click
    end
    object Button7: TButton
      Left = 96
      Top = 888
      Width = 25
      Height = 25
      Caption = 'Button7'
      TabOrder = 9
      Visible = False
      OnClick = Button7Click
    end
    object Button8: TButton
      Left = 120
      Top = 888
      Width = 25
      Height = 25
      Caption = 'Button8'
      TabOrder = 10
      Visible = False
      OnClick = Button8Click
    end
    object bAvg: TButton
      Left = 8
      Top = 904
      Width = 65
      Height = 9
      Caption = '1 '#1080#1084#1087#1091#1083#1100#1089' '#1085#1072#1082#1086#1087'. (0x8)'
      TabOrder = 11
      Visible = False
      OnClick = bAvgClick
    end
    object bAvgVarPlot: TButton
      Left = 8
      Top = 912
      Width = 65
      Height = 9
      Caption = '32 '#1080#1084#1087#1091#1083#1100#1089#1072' '#1085#1072#1082#1086#1087'. (0x9)'
      TabOrder = 12
      Visible = False
      OnClick = bAvgVarPlotClick
    end
    object btOriginalPulse: TButton
      Left = 8
      Top = 616
      Width = 161
      Height = 25
      Caption = #1047#1086#1085#1076#1080#1088#1091#1102#1097#1080#1081' '#1080#1084#1087#1091#1083#1100#1089
      TabOrder = 13
      OnClick = btOriginalPulseClick
    end
    object GroupBox1: TGroupBox
      Left = 172
      Top = 8
      Width = 169
      Height = 281
      Caption = #1055#1083#1072#1090#1092#1086#1088#1084#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 14
      object Label1: TLabel
        Left = 16
        Top = 211
        Width = 7
        Height = 16
        Caption = '0'
      end
      object Label5: TLabel
        Left = 6
        Top = 119
        Width = 113
        Height = 13
        Caption = #1056#1072#1079#1084#1077#1088' '#1089#1090#1088#1086#1073#1072' ('#1076#1072#1083#1100'.)'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 6
        Top = 148
        Width = 115
        Height = 13
        Caption = #1056#1072#1079#1084#1077#1088' '#1089#1090#1088#1086#1073#1072' ('#1092#1072#1079#1072'.)'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object AddressEdit: TLabeledEdit
        Left = 8
        Top = 34
        Width = 105
        Height = 24
        EditLabel.Width = 43
        EditLabel.Height = 13
        EditLabel.Caption = 'IP '#1072#1076#1088#1077#1089
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -12
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 0
        Text = '127.0.0.1'
      end
      object PortEdit: TLabeledEdit
        Left = 120
        Top = 34
        Width = 41
        Height = 24
        EditLabel.Width = 31
        EditLabel.Height = 16
        EditLabel.Caption = #1087#1086#1088#1090
        Enabled = False
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 1
        Text = '3333'
      end
      object ApplyButton: TButton
        Left = 8
        Top = 64
        Width = 153
        Height = 25
        Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100
        TabOrder = 2
        OnClick = ApplyButtonClick
      end
      object LabeledEdit1: TLabeledEdit
        Left = 8
        Top = 182
        Width = 81
        Height = 24
        EditLabel.Width = 70
        EditLabel.Height = 13
        EditLabel.Caption = #1059#1075#1086#1083', '#1075#1088#1072#1076' (X)'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -11
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 3
      end
      object Button4: TButton
        Left = 96
        Top = 182
        Width = 65
        Height = 25
        Caption = #1047#1072#1076#1072#1090#1100
        TabOrder = 4
        OnClick = Button4Click
      end
      object AutoCheckBox: TCheckBox
        Left = 8
        Top = 94
        Width = 153
        Height = 17
        Caption = #1040#1074#1090#1086#1089#1086#1087#1088#1086#1074#1086#1078#1076#1077#1085#1080#1077
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object Button6: TButton
        Left = 96
        Top = 206
        Width = 65
        Height = 25
        Caption = #1057#1095#1080#1090#1072#1090#1100
        TabOrder = 6
        OnClick = Button6Click
      end
      object UpDownDelta: TCSpinEdit
        Left = 121
        Top = 114
        Width = 42
        Height = 26
        Increment = 2
        MaxValue = 100
        MinValue = 2
        TabOrder = 7
        Value = 20
      end
      object LabeledY: TLabeledEdit
        Left = 8
        Top = 249
        Width = 81
        Height = 24
        EditLabel.Width = 70
        EditLabel.Height = 13
        EditLabel.Caption = #1059#1075#1086#1083', '#1075#1088#1072#1076' (Y)'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -11
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 8
      end
      object Button9: TButton
        Left = 95
        Top = 249
        Width = 65
        Height = 25
        Caption = #1047#1072#1076#1072#1090#1100
        TabOrder = 9
        OnClick = Button9Click
      end
      object LeftRightDelta: TCSpinEdit
        Left = 121
        Top = 142
        Width = 42
        Height = 26
        Increment = 2
        MaxValue = 100
        MinValue = 2
        TabOrder = 10
        Value = 20
      end
    end
    object RadioGroup1: TRadioGroup
      Left = 8
      Top = 458
      Width = 161
      Height = 57
      Caption = #1056#1077#1078#1080#1084
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ItemIndex = 0
      Items.Strings = (
        #1057#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1077
        #1052#1086#1085#1086#1080#1084#1087#1091#1083#1100#1089)
      ParentFont = False
      TabOrder = 15
      OnClick = Action1Execute
    end
    object GroupBox3: TGroupBox
      Left = 172
      Top = 361
      Width = 169
      Height = 281
      Caption = #1054#1073#1083#1072#1089#1090#1100' '#1080#1079#1084#1077#1088#1077#1085#1080#1081
      TabOrder = 16
      object Label2: TLabel
        Left = 5
        Top = 232
        Width = 43
        Height = 13
        Caption = #1054#1090#1089#1095#1077#1090#1099
      end
      object Label3: TLabel
        Left = 14
        Top = 257
        Width = 34
        Height = 13
        Caption = #1052#1077#1090#1088#1099
      end
      object MinUpDown: TTrackBar
        Left = 72
        Top = 10
        Width = 28
        Height = 215
        Max = 0
        Min = -1024
        Orientation = trVertical
        Frequency = 48
        Position = 0
        SelEnd = -100
        SelStart = -50
        TabOrder = 0
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = MinUpDownChange
      end
      object MaxUpDown: TTrackBar
        Left = 114
        Top = 10
        Width = 36
        Height = 215
        Max = 0
        Min = -1024
        Orientation = trVertical
        Frequency = 48
        Position = 0
        SelEnd = 0
        SelStart = 0
        TabOrder = 1
        TickMarks = tmTopLeft
        TickStyle = tsAuto
        OnChange = MaxUpDownChange
      end
      object MinUpDownO: TCSpinEdit
        Left = 52
        Top = 228
        Width = 56
        Height = 22
        EditorEnabled = False
        Enabled = False
        MaxValue = 1024
        TabOrder = 2
      end
      object MaxUpDownO: TCSpinEdit
        Left = 111
        Top = 227
        Width = 56
        Height = 22
        EditorEnabled = False
        Enabled = False
        MaxValue = 1024
        TabOrder = 3
      end
      object MinUpDownM: TEdit
        Left = 52
        Top = 254
        Width = 56
        Height = 21
        Enabled = False
        TabOrder = 4
        Text = '0'
      end
      object MaxUpDownM: TEdit
        Left = 110
        Top = 254
        Width = 56
        Height = 21
        Enabled = False
        TabOrder = 5
        Text = '0'
      end
    end
    object GroupBox4: TGroupBox
      Left = 172
      Top = 288
      Width = 168
      Height = 72
      Caption = #1047#1072#1087#1080#1089#1100' '#1074#1080#1076#1077#1086
      TabOrder = 17
      object Label4: TLabel
        Left = 8
        Top = 16
        Width = 127
        Height = 13
        Caption = 'IP '#1072#1076#1088#1077#1089' '#1089#1077#1088#1074#1077#1088#1072' '#1079#1072#1087#1080#1089#1080
      end
      object videoServer: TEdit
        Left = 8
        Top = 37
        Width = 153
        Height = 21
        TabOrder = 0
        Text = '192.168.114.2'
      end
    end
    object chWriteView: TCheckBox
      Left = 10
      Top = 519
      Width = 159
      Height = 17
      Caption = #1047#1072#1087#1080#1089#1100' '#1087#1088#1080' '#1086#1090#1086#1073#1088#1072#1078#1077#1085#1080#1080
      TabOrder = 18
    end
  end
  object MCon: TButton
    Left = 16
    Top = 256
    Width = 145
    Height = 25
    Caption = #1055#1086#1076#1082#1083#1102#1095#1080#1090#1100
    TabOrder = 3
    OnClick = MConClick
  end
  object SendSettings: TButton
    Left = 16
    Top = 288
    Width = 145
    Height = 25
    Caption = #1047#1072#1076#1072#1090#1100' '#1087#1072#1088#1072#1084#1077#1090#1088#1099
    TabOrder = 4
    OnClick = SendSettingsClick
  end
  object MainMenu1: TMainMenu
    Left = 352
    Top = 48
    object mmFile: TMenuItem
      Caption = #1060#1072#1081#1083
      object N5: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1076#1072#1085#1085#1099#1077
        OnClick = N5Click
      end
      object N6: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
        OnClick = N6Click
      end
      object ReadFile1: TMenuItem
        Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1076#1072#1085#1085#1099#1077
        OnClick = ReadFile1Click
      end
      object N4: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = N4Click
      end
    end
    object N1: TMenuItem
      Caption = #1055#1086#1076#1082#1083#1102#1095#1077#1085#1080#1077
      object N2: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' '#1080#1085#1090#1077#1088#1092#1077#1081#1089
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = #1047#1072#1082#1088#1099#1090#1100' '#1080#1085#1090#1077#1088#1092#1077#1081#1089
        OnClick = N3Click
      end
    end
    object mmMainSettings: TMenuItem
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      object N15: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1056#1051#1052
        OnClick = N15Click
      end
      object Settings1: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1103
        OnClick = mmSettingsClick
      end
      object Threshold1: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1090#1088#1077#1074#1086#1075#1080
        OnClick = Threshold1Click
      end
      object RasterSetting1: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1080#1085#1076#1080#1082#1072#1090#1086#1088#1086#1074
        OnClick = RasterSetting1Click
      end
      object MeasureSettings1: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1080#1079#1084#1077#1088#1077#1085#1080#1081
        OnClick = MeasureSettings1Click
      end
      object N8: TMenuItem
        Caption = #1050#1086#1085#1090#1088#1086#1083#1100' '#1087#1072#1088#1072#1084#1077#1090#1088#1086#1074' '#1056#1051#1057' '
        OnClick = N8Click
      end
      object mmChanels: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1082#1072#1085#1072#1083#1086#1074
        OnClick = mmChanelsClick
      end
    end
    object Window1: TMenuItem
      Caption = #1054#1082#1085#1086
      object mmTile: TMenuItem
        Caption = #1047#1072#1084#1086#1089#1090#1080#1090#1100
        OnClick = mmTileClick
      end
      object mmShowOsc: TMenuItem
        Caption = #1054#1089#1094#1080#1083#1083#1086#1075#1088#1072#1084#1084#1072
        OnClick = mmShowOscClick
      end
      object mmShowSp: TMenuItem
        Caption = #1057#1087#1077#1082#1090#1088
        OnClick = mmShowSpClick
      end
      object mmShowExp: TMenuItem
        Caption = #1069#1082#1089#1087#1077#1088#1080#1084#1077#1085#1090
        OnClick = mmShowExpClick
      end
      object mmRasterShow: TMenuItem
        Caption = #1056#1072#1089#1090#1088#1099
        OnClick = mmRasterShowClick
      end
      object ScatterMenu: TMenuItem
        Caption = #1048#1079#1084#1077#1088#1077#1085#1080#1077' '#1069#1055#1056
        OnClick = ScatterMenuClick
      end
    end
    object N9: TMenuItem
      Caption = #1050#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1103' '#1056#1051#1057
      object N10: TMenuItem
        Caption = #1060#1086#1088#1084#1080#1088#1086#1074#1072#1085#1080#1077' '#1088#1072#1076#1080#1086#1089#1080#1075#1085#1072#1083#1086#1074' '#1074' '#1052' '#1076#1080#1072#1087#1072#1079#1086#1085#1077' '#1095#1072#1089#1090#1086#1090
        OnClick = N10Click
      end
      object N11: TMenuItem
        Caption = #1060#1086#1088#1084#1080#1088#1086#1074#1072#1085#1080#1077' '#1088#1072#1076#1080#1086#1089#1080#1075#1085#1072#1083#1086#1074' '#1074' '#1044#1052' '#1076#1080#1072#1087#1072#1079#1086#1085#1077' '#1095#1072#1089#1090#1086#1090
        OnClick = N11Click
      end
      object N12: TMenuItem
        Caption = #1060#1086#1088#1084#1080#1088#1086#1074#1072#1085#1080#1077' '#1088#1072#1076#1080#1086#1089#1080#1075#1085#1072#1083#1086#1074' '#1074' '#1057#1052' '#1076#1080#1072#1087#1072#1079#1086#1085#1077' '#1095#1072#1089#1090#1086#1090
        OnClick = N12Click
      end
    end
    object N7: TMenuItem
      Caption = #1054#1090#1083#1072#1076#1082#1072
      OnClick = N7Click
    end
    object N13: TMenuItem
      Caption = #1042#1080#1076#1077#1086
      object N14: TMenuItem
        Caption = #1047#1072#1087#1080#1089#1100
        OnClick = N14Click
      end
    end
  end
  object TimerARP: TTimer
    Enabled = False
    OnTimer = TimerARPTimer
    Left = 352
    Top = 16
  end
  object TimerAck: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerAckTimer
    Left = 352
    Top = 128
  end
  object ActionManager1: TActionManager
    Left = 384
    Top = 48
    object Action1: TAction
      Caption = 'Action1'
      OnExecute = Action1Execute
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = #1060#1072#1081#1083' '#1089' '#1076#1072#1085#1085#1099#1084#1080' (bin)|*.BIN|'#1060#1072#1081#1083' dat|*.DAT'
    Left = 384
    Top = 16
  end
  object FileReadTimer: TTimer
    Enabled = False
    Interval = 2
    OnTimer = FileReadTimerTimer
    Left = 352
    Top = 88
  end
  object IdUDPClient1: TIdUDPClient
    Port = 0
    Left = 384
    Top = 128
  end
  object IdUDPServer1: TIdUDPServer
    Bindings = <>
    DefaultPort = 3333
    OnUDPRead = IdUDPServer1UDPRead
    Left = 392
    Top = 176
  end
  object IdUDPClient2: TIdUDPClient
    Host = '192.168.114.40'
    Port = 4444
    Left = 352
    Top = 176
  end
  object PlotTimer: TTimer
    Interval = 40
    OnTimer = PlotTimerTimer
    Left = 384
    Top = 88
  end
  object IdUDPClient3: TIdUDPClient
    Port = 11211
    Left = 304
    Top = 293
  end
end
