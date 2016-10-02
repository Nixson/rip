object SettingsUnitForm: TSettingsUnitForm
  Left = 758
  Top = 54
  Width = 427
  Height = 597
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1056#1051#1052
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 411
    Height = 559
    ActivePage = TabSheet1
    Align = alClient
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #1056#1051#1052
      object GroupBox1: TGroupBox
        Left = 0
        Top = 56
        Width = 201
        Height = 145
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1089#1080#1075#1085#1072#1083#1072' '#1085#1077#1089#1091#1097#1077#1081
        TabOrder = 0
        object leCarFreqStart: TLabeledEdit
          Left = 8
          Top = 80
          Width = 177
          Height = 21
          EditLabel.Width = 175
          EditLabel.Height = 13
          EditLabel.Caption = #1063#1072#1089#1090#1086#1090#1072' '#1085#1077#1089#1091#1097#1077#1081' '#1085#1072#1095#1072#1083#1100#1085#1072#1103' ('#1052#1043#1094')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 0
          Text = '1250'
        end
        object leAmpCh1: TLabeledEdit
          Left = 8
          Top = 40
          Width = 81
          Height = 21
          EditLabel.Width = 57
          EditLabel.Height = 13
          EditLabel.Caption = #1040#1084#1087'. '#1050'1 (%)'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 1
          Text = '100'
        end
        object leCarFreqFin: TLabeledEdit
          Left = 8
          Top = 120
          Width = 177
          Height = 21
          EditLabel.Width = 169
          EditLabel.Height = 13
          EditLabel.Caption = #1063#1072#1089#1090#1086#1090#1072' '#1085#1077#1089#1091#1097#1077#1081' '#1082#1086#1085#1077#1095#1085#1072#1103' ('#1052#1043#1094')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 2
          Text = '1250'
        end
        object leAmpCh2: TLabeledEdit
          Left = 104
          Top = 40
          Width = 81
          Height = 21
          EditLabel.Width = 57
          EditLabel.Height = 13
          EditLabel.Caption = #1040#1084#1087'. '#1050'2 (%)'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 3
          Text = '100'
        end
      end
      object GroupBox2: TGroupBox
        Left = 208
        Top = 56
        Width = 201
        Height = 193
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1089#1080#1075#1085#1072#1083#1072' '#1088#1086#1090#1072#1094#1080#1080
        TabOrder = 1
        object leRotFreqFin: TLabeledEdit
          Left = 8
          Top = 120
          Width = 177
          Height = 21
          EditLabel.Width = 166
          EditLabel.Height = 13
          EditLabel.Caption = #1063#1072#1089#1090#1086#1090#1072' '#1088#1086#1090#1072#1094#1080#1080' '#1082#1086#1085#1077#1095#1085#1072#1103' ('#1052#1043#1094')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 0
          Text = '0'
        end
        object leRotFreqStart: TLabeledEdit
          Left = 8
          Top = 80
          Width = 177
          Height = 21
          EditLabel.Width = 172
          EditLabel.Height = 13
          EditLabel.Caption = #1063#1072#1089#1090#1086#1090#1072' '#1088#1086#1090#1072#1094#1080#1080' '#1085#1072#1095#1072#1083#1100#1085#1072#1103' ('#1052#1043#1094')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 1
          Text = '0'
        end
        object leRotPhaseCh1: TLabeledEdit
          Left = 8
          Top = 40
          Width = 81
          Height = 21
          EditLabel.Width = 68
          EditLabel.Height = 13
          EditLabel.Caption = #1060#1072#1079#1072' '#1050'1 ('#1075#1088'.)'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 2
          Text = '0'
        end
        object leRotPeriod: TLabeledEdit
          Left = 8
          Top = 160
          Width = 177
          Height = 21
          EditLabel.Width = 161
          EditLabel.Height = 13
          EditLabel.Caption = #1055#1077#1088#1080#1086#1076' '#1088#1086#1090#1072#1094#1080#1080' ('#1050#1086#1075'. '#1088'-'#1084') ('#1084#1082#1089')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 3
          Text = '100'
        end
        object leRotPhaseCh2: TLabeledEdit
          Left = 104
          Top = 40
          Width = 81
          Height = 21
          EditLabel.Width = 68
          EditLabel.Height = 13
          EditLabel.Caption = #1060#1072#1079#1072' '#1050'2 ('#1075#1088'.)'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 4
          Text = '0'
        end
      end
      object GroupBox3: TGroupBox
        Left = 0
        Top = 208
        Width = 201
        Height = 193
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1080#1084#1087#1091#1083#1100#1089#1085#1086#1081' '#1084#1086#1076#1091#1083#1103#1094#1080#1080
        TabOrder = 2
        object lePulseWidth: TLabeledEdit
          Left = 8
          Top = 40
          Width = 177
          Height = 21
          EditLabel.Width = 154
          EditLabel.Height = 13
          EditLabel.Caption = #1044#1083#1080#1090#1077#1083#1100#1085#1086#1089#1090#1100' '#1080#1084#1087#1091#1083#1100#1089#1072' ('#1084#1082#1089')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 0
          Text = '0.1'
        end
        object lePulsePeriod: TLabeledEdit
          Left = 8
          Top = 80
          Width = 177
          Height = 21
          EditLabel.Width = 119
          EditLabel.Height = 13
          EditLabel.Caption = #1055#1077#1088#1080#1086#1076' '#1080#1084#1087#1091#1083#1100#1089#1072' ('#1084#1082#1089')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 1
          Text = '2'
        end
        object lePacketPeriod: TLabeledEdit
          Left = 8
          Top = 120
          Width = 177
          Height = 21
          EditLabel.Width = 99
          EditLabel.Height = 13
          EditLabel.Caption = #1055#1077#1088#1080#1086#1076' '#1087#1072#1095#1082#1080' ('#1084#1082#1089')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 2
          Text = '100000'
        end
      end
      object GroupBox4: TGroupBox
        Left = 0
        Top = 0
        Width = 409
        Height = 49
        Caption = #1056#1077#1078#1080#1084' '#1088#1072#1073#1086#1090#1099
        TabOrder = 3
        object ComboBox1: TComboBox
          Left = 8
          Top = 16
          Width = 385
          Height = 21
          ItemHeight = 13
          TabOrder = 0
          Text = #1053#1091#1083#1077#1074#1072#1103' '#1085#1072#1095#1072#1083#1100#1085#1072#1103' '#1092#1072#1079#1072
          Items.Strings = (
            #1053#1077#1087#1088#1077#1088#1099#1074#1085#1099#1081' '#1089#1080#1075#1085#1072#1083
            #1053#1091#1083#1077#1074#1072#1103' '#1085#1072#1095#1072#1083#1100#1085#1072#1103' '#1092#1072#1079#1072
            #1050#1086#1075#1077#1088#1077#1085#1090#1085#1099#1081
            #1060#1050#1052' 15'
            #1055#1072#1095#1082#1072' '#1089' '#1084#1086#1076#1091#1083#1103#1094#1080#1077#1081' '#1087#1077#1088#1080#1086#1076#1072
            #1060#1050#1052' 255')
        end
      end
      object GroupBox5: TGroupBox
        Left = 208
        Top = 256
        Width = 201
        Height = 225
        Caption = #1047#1072#1087#1080#1089#1100' '#1076#1072#1085#1085#1099#1093' '#1040#1062#1055
        TabOrder = 4
        object leAdcSamples: TLabeledEdit
          Left = 8
          Top = 32
          Width = 177
          Height = 21
          EditLabel.Width = 193
          EditLabel.Height = 13
          EditLabel.Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1074#1099#1073#1086#1088#1086#1082' '#1040#1062#1055' ('#1085#1072' 1 '#1087#1072#1095#1082#1091')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 0
          Text = '256'
        end
        object lePulseNumber: TLabeledEdit
          Left = 8
          Top = 72
          Width = 177
          Height = 21
          EditLabel.Width = 158
          EditLabel.Height = 13
          EditLabel.Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1080#1084#1087#1091#1083#1100#1089#1086#1074' '#1074' '#1087#1072#1095#1082#1077
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 1
          Text = '1'
        end
        object CSaveToFile: TCheckBox
          Left = 8
          Top = 200
          Width = 97
          Height = 17
          Caption = 'CSaveToFile'
          TabOrder = 2
        end
        object lePacketNumber: TLabeledEdit
          Left = 8
          Top = 112
          Width = 177
          Height = 21
          EditLabel.Width = 91
          EditLabel.Height = 13
          EditLabel.Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1087#1072#1095#1077#1082
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 3
          Text = '1'
        end
        object leAdcWriteDelay: TLabeledEdit
          Left = 8
          Top = 152
          Width = 177
          Height = 21
          EditLabel.Width = 157
          EditLabel.Height = 13
          EditLabel.Caption = #1047#1072#1076#1077#1088#1078#1082#1072' '#1089#1090#1088#1086#1073#1072' '#1079#1072#1087#1080#1089#1080' ('#1084#1082#1089')'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 4
          Text = '0.2'
        end
      end
      object GroupBox6: TGroupBox
        Left = 208
        Top = 480
        Width = 201
        Height = 49
        TabOrder = 5
        object CDac1Rst: TCheckBox
          Left = 8
          Top = 8
          Width = 97
          Height = 17
          Caption = 'CDac1Rst'
          TabOrder = 0
        end
        object CDac2Rst: TCheckBox
          Left = 96
          Top = 8
          Width = 97
          Height = 17
          Caption = 'CDac2Rst'
          TabOrder = 1
        end
        object CAdc1Rst: TCheckBox
          Left = 8
          Top = 24
          Width = 97
          Height = 17
          Caption = 'CAdc1Rst'
          TabOrder = 2
        end
        object CAdc2Rst: TCheckBox
          Left = 96
          Top = 24
          Width = 97
          Height = 17
          Caption = 'CAdc2Rst'
          TabOrder = 3
        end
      end
      object BitBtn1: TBitBtn
        Left = 0
        Top = 440
        Width = 201
        Height = 89
        Caption = #1047#1072#1076#1072#1090#1100' '#1087#1072#1088#1072#1084#1077#1090#1088#1099' '
        TabOrder = 6
        OnClick = BitBtn1Click
      end
      object CRefresh: TCheckBox
        Left = 216
        Top = 440
        Width = 97
        Height = 17
        Caption = 'CRefresh'
        TabOrder = 7
      end
    end
    object Net: TTabSheet
      Caption = #1057#1077#1090#1100
      ImageIndex = 1
      object EIpAdr: TLabeledEdit
        Left = 8
        Top = 24
        Width = 121
        Height = 21
        EditLabel.Width = 73
        EditLabel.Height = 13
        EditLabel.Caption = 'IP '#1072#1076#1088#1077#1089' '#1056#1051#1052':'
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 0
        Text = '192.168.1.10'
      end
      object EPort: TLabeledEdit
        Left = 8
        Top = 64
        Width = 121
        Height = 21
        EditLabel.Width = 55
        EditLabel.Height = 13
        EditLabel.Caption = #1055#1086#1088#1090' '#1056#1051#1052':'
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 1
        Text = '755'
      end
      object MCon: TButton
        Left = 8
        Top = 96
        Width = 121
        Height = 25
        Caption = #1055#1086#1076#1082#1083#1102#1095#1080#1090#1100
        TabOrder = 2
        OnClick = MConClick
      end
      object MDisCon: TButton
        Left = 8
        Top = 136
        Width = 121
        Height = 25
        Caption = #1054#1090#1082#1083#1102#1095#1080#1090#1100
        TabOrder = 3
        OnClick = MDisConClick
      end
      object btnCMD1: TButton
        Left = 8
        Top = 232
        Width = 121
        Height = 25
        Caption = 'btnCMD1'
        TabOrder = 4
        OnClick = btnCMD1Click
      end
      object btnCMD3: TButton
        Left = 8
        Top = 288
        Width = 121
        Height = 25
        Caption = 'btnCMD3'
        TabOrder = 5
        OnClick = btnCMD3Click
      end
      object btnCMD5: TButton
        Left = 8
        Top = 336
        Width = 121
        Height = 25
        Caption = 'btnCMD5'
        TabOrder = 6
        OnClick = btnCMD5Click
      end
      object btnCMD7: TButton
        Left = 8
        Top = 384
        Width = 121
        Height = 26
        Caption = 'btnCMD7'
        TabOrder = 7
        OnClick = btnCMD7Click
      end
      object btnCMD9: TButton
        Left = 8
        Top = 432
        Width = 121
        Height = 26
        Caption = 'btnCMD9'
        TabOrder = 8
        OnClick = btnCMD9Click
      end
    end
    object TabSheet3: TTabSheet
      Caption = #1059#1079#1077#1083' '#1050#1086#1084#1084#1091#1090#1072#1094#1080#1080
      ImageIndex = 3
      object GTransmitter: TGroupBox
        Left = 8
        Top = 32
        Width = 393
        Height = 177
        Caption = #1055#1077#1088#1077#1076#1072#1090#1095#1080#1082
        TabOrder = 0
        object LTx_X1V: TLabeledEdit
          Left = 8
          Top = 32
          Width = 49
          Height = 21
          EditLabel.Width = 47
          EditLabel.Height = 13
          EditLabel.Caption = 'LTx_X1V '
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 0
          Text = '20'
        end
        object LTx_X1H: TLabeledEdit
          Left = 160
          Top = 32
          Width = 49
          Height = 21
          EditLabel.Width = 45
          EditLabel.Height = 13
          EditLabel.Caption = 'LTx_X1H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 1
          Text = '20'
        end
        object LTx_X2V: TLabeledEdit
          Left = 8
          Top = 72
          Width = 49
          Height = 21
          EditLabel.Width = 47
          EditLabel.Height = 13
          EditLabel.Caption = 'LTx_X2V '
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 2
          Text = '20'
        end
        object LTx_X2H: TLabeledEdit
          Left = 160
          Top = 72
          Width = 49
          Height = 21
          EditLabel.Width = 45
          EditLabel.Height = 13
          EditLabel.Caption = 'LTx_X2H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 3
          Text = '20'
        end
        object CTx_X1V: TCheckBox
          Left = 64
          Top = 32
          Width = 97
          Height = 17
          Caption = 'CTx_X1V'
          TabOrder = 4
        end
        object CTx_X2V: TCheckBox
          Left = 64
          Top = 72
          Width = 97
          Height = 17
          Caption = 'CTx_X2V'
          TabOrder = 5
        end
        object CTx_X1H: TCheckBox
          Left = 216
          Top = 32
          Width = 97
          Height = 17
          Caption = 'CTx_X1H'
          TabOrder = 6
        end
        object CTx_X2H: TCheckBox
          Left = 216
          Top = 72
          Width = 97
          Height = 17
          Caption = 'CTx_X2H'
          TabOrder = 7
        end
        object LTx_PL1V: TLabeledEdit
          Left = 8
          Top = 112
          Width = 49
          Height = 21
          EditLabel.Width = 50
          EditLabel.Height = 13
          EditLabel.Caption = 'LTx_PL1V'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 8
          Text = '0'
        end
        object LTx_PL2V: TLabeledEdit
          Left = 8
          Top = 152
          Width = 49
          Height = 21
          EditLabel.Width = 53
          EditLabel.Height = 13
          EditLabel.Caption = 'LTx_PL2V '
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 9
          Text = '0'
        end
        object LTx_PL1H: TLabeledEdit
          Left = 160
          Top = 112
          Width = 49
          Height = 21
          EditLabel.Width = 51
          EditLabel.Height = 13
          EditLabel.Caption = 'LTx_PL1H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 10
          Text = '0'
        end
        object LTx_PL2H: TLabeledEdit
          Left = 160
          Top = 152
          Width = 49
          Height = 21
          EditLabel.Width = 51
          EditLabel.Height = 13
          EditLabel.Caption = 'LTx_PL2H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 11
          Text = '0'
        end
      end
      object GReceiver: TGroupBox
        Left = 8
        Top = 208
        Width = 393
        Height = 177
        Caption = #1055#1088#1080#1077#1084#1085#1080#1082
        TabOrder = 1
        object LRx_X1V: TLabeledEdit
          Left = 8
          Top = 32
          Width = 49
          Height = 21
          EditLabel.Width = 45
          EditLabel.Height = 13
          EditLabel.Caption = 'LRx_X1V'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 0
          Text = '0'
        end
        object LRx_X2V: TLabeledEdit
          Left = 8
          Top = 72
          Width = 49
          Height = 21
          EditLabel.Width = 45
          EditLabel.Height = 13
          EditLabel.Caption = 'LRx_X2V'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 1
          Text = '0'
        end
        object LRx_X1H: TLabeledEdit
          Left = 160
          Top = 32
          Width = 49
          Height = 21
          EditLabel.Width = 46
          EditLabel.Height = 13
          EditLabel.Caption = 'LRx_X1H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 2
          Text = '0'
        end
        object LRx_X2H: TLabeledEdit
          Left = 160
          Top = 72
          Width = 49
          Height = 21
          EditLabel.Width = 46
          EditLabel.Height = 13
          EditLabel.Caption = 'LRx_X2H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 3
          Text = '0'
        end
        object CRx_X1V: TCheckBox
          Left = 64
          Top = 32
          Width = 97
          Height = 17
          Caption = 'CRx_X1V'
          TabOrder = 4
        end
        object CRx_X2V: TCheckBox
          Left = 64
          Top = 72
          Width = 97
          Height = 17
          Caption = 'CRx_X2V'
          TabOrder = 5
        end
        object CRx_X1H: TCheckBox
          Left = 216
          Top = 32
          Width = 97
          Height = 17
          Caption = 'CRx_X1H'
          TabOrder = 6
        end
        object CRx_X2H: TCheckBox
          Left = 216
          Top = 72
          Width = 97
          Height = 17
          Caption = 'CRx_X2H'
          TabOrder = 7
        end
        object LRx_PL1V: TLabeledEdit
          Left = 8
          Top = 112
          Width = 49
          Height = 21
          EditLabel.Width = 51
          EditLabel.Height = 13
          EditLabel.Caption = 'LRx_PL1V'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 8
          Text = '0'
        end
        object LRx_PL2V: TLabeledEdit
          Left = 8
          Top = 152
          Width = 49
          Height = 21
          EditLabel.Width = 53
          EditLabel.Height = 13
          EditLabel.Caption = 'LTx_PL2V '
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 9
          Text = '0'
        end
        object LRx_PL1H: TLabeledEdit
          Left = 160
          Top = 112
          Width = 49
          Height = 21
          EditLabel.Width = 52
          EditLabel.Height = 13
          EditLabel.Caption = 'LRx_PL1H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 10
          Text = '0'
        end
        object LRx_PL2H: TLabeledEdit
          Left = 160
          Top = 152
          Width = 49
          Height = 21
          EditLabel.Width = 52
          EditLabel.Height = 13
          EditLabel.Caption = 'LRx_PL2H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 11
          Text = '0'
        end
      end
      object RGBand: TRadioGroup
        Left = 8
        Top = 0
        Width = 393
        Height = 33
        Columns = 4
        ItemIndex = 0
        Items.Strings = (
          #1042#1057#1050
          'P '#1076#1080#1072#1087#1072#1079#1086#1085
          'L '#1076#1080#1072#1087#1072#1079#1086#1085
          'X '#1076#1080#1072#1087#1072#1079#1086#1085)
        TabOrder = 2
      end
      object GGeterodin: TGroupBox
        Left = 8
        Top = 384
        Width = 393
        Height = 145
        Caption = #1043#1077#1090#1077#1088#1086#1076#1080#1085
        TabOrder = 3
        object LPh_Tx_X1V: TLabeledEdit
          Left = 8
          Top = 72
          Width = 57
          Height = 21
          EditLabel.Width = 63
          EditLabel.Height = 13
          EditLabel.Caption = 'LPh_Tx_X1V'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 0
          Text = '0'
        end
        object LPh_Tx_X2V: TLabeledEdit
          Left = 8
          Top = 112
          Width = 57
          Height = 21
          EditLabel.Width = 63
          EditLabel.Height = 13
          EditLabel.Caption = 'LPh_Tx_X2V'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 1
          Text = '0'
        end
        object LPh_Tx_X1H: TLabeledEdit
          Left = 88
          Top = 72
          Width = 57
          Height = 21
          EditLabel.Width = 64
          EditLabel.Height = 13
          EditLabel.Caption = 'LPh_Tx_X1H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 2
          Text = '0'
        end
        object LPh_Tx_X2H: TLabeledEdit
          Left = 88
          Top = 112
          Width = 57
          Height = 21
          EditLabel.Width = 64
          EditLabel.Height = 13
          EditLabel.Caption = 'LPh_Tx_X2H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 3
          Text = '0'
        end
        object LPh_Rx_X1V: TLabeledEdit
          Left = 184
          Top = 72
          Width = 57
          Height = 21
          EditLabel.Width = 64
          EditLabel.Height = 13
          EditLabel.Caption = 'LPh_Rx_X1V'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 4
          Text = '0'
        end
        object LPh_Rx_X2V: TLabeledEdit
          Left = 184
          Top = 112
          Width = 57
          Height = 21
          EditLabel.Width = 64
          EditLabel.Height = 13
          EditLabel.Caption = 'LPh_Rx_X2V'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 5
          Text = '0'
        end
        object LPh_Rx_X1H: TLabeledEdit
          Left = 264
          Top = 72
          Width = 57
          Height = 21
          EditLabel.Width = 65
          EditLabel.Height = 13
          EditLabel.Caption = 'LPh_Rx_X1H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 6
          Text = '0'
        end
        object LPh_Rx_X2H: TLabeledEdit
          Left = 264
          Top = 112
          Width = 57
          Height = 21
          EditLabel.Width = 65
          EditLabel.Height = 13
          EditLabel.Caption = 'LPh_Rx_X2H'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 7
          Text = '0'
        end
        object CTxGeterodinOff: TCheckBox
          Left = 184
          Top = 32
          Width = 97
          Height = 17
          Caption = 'CTxGeterodinOff'
          TabOrder = 8
        end
        object LGeterodinFreq: TLabeledEdit
          Left = 8
          Top = 32
          Width = 137
          Height = 21
          EditLabel.Width = 73
          EditLabel.Height = 13
          EditLabel.Caption = 'LGeterodinFreq'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 9
          Text = '8740'
        end
      end
    end
  end
  object checkView: TTimer
    Interval = 500
    OnTimer = checkViewTimer
    Left = 384
  end
  object ClientSocket1: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket1Connect
    OnDisconnect = ClientSocket1Disconnect
    OnRead = ClientSocket1Read
    OnError = ClientSocket1Error
    Left = 352
  end
end
