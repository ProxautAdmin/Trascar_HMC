object FormPlcScarico: TFormPlcScarico
  Left = 411
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Plc Signal'
  ClientHeight = 350
  ClientWidth = 680
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 680
    Height = 32
    Align = alTop
    Caption = 'Status Posizione di Scarico'
    Color = clMoneyGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
  end
  object Panel2: TPanel
    Left = 0
    Top = 32
    Width = 680
    Height = 269
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Panel7: TPanel
      Left = 7
      Top = 43
      Width = 401
      Height = 78
      BevelOuter = bvLowered
      TabOrder = 0
      object Label3: TLabel
        Left = 288
        Top = 36
        Width = 61
        Height = 13
        Caption = 'Cycle Time : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lCycle: TLabel
        Left = 349
        Top = 36
        Width = 6
        Height = 13
        Caption = '0'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 288
        Top = 52
        Width = 36
        Height = 13
        Caption = 'Errors :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object lErrors: TLabel
        Left = 349
        Top = 52
        Width = 6
        Height = 13
        Caption = '0'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Panel8: TPanel
        Left = 1
        Top = 1
        Width = 399
        Height = 23
        Align = alTop
        Caption = 'Plc Status'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object Panel15: TPanel
        Left = 13
        Top = 39
        Width = 125
        Height = 25
        Alignment = taRightJustify
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Caption = 'Connected    '
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        object Shape4: TShape
          Left = 4
          Top = 4
          Width = 21
          Height = 17
          Brush.Color = clLime
          Shape = stCircle
        end
      end
      object GroupBox1: TGroupBox
        Left = 144
        Top = 32
        Width = 139
        Height = 33
        Caption = 'Wacth Dog'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        object Label9: TLabel
          Left = 7
          Top = 16
          Width = 13
          Height = 13
          Caption = 'Plc'
        end
        object lWatchDogPlc: TLabel
          Left = 26
          Top = 16
          Width = 6
          Height = 13
          Caption = '0'
        end
        object Label12: TLabel
          Left = 70
          Top = 16
          Width = 11
          Height = 13
          Caption = 'Pc'
        end
        object lWatchDogPc: TLabel
          Left = 91
          Top = 16
          Width = 6
          Height = 13
          Caption = '0'
        end
      end
    end
    object Panel5: TPanel
      Left = 1
      Top = 1
      Width = 678
      Height = 40
      Align = alTop
      BevelOuter = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      object Label20: TLabel
        Left = 8
        Top = 11
        Width = 77
        Height = 19
        Caption = 'Position :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label22: TLabel
        Left = 96
        Top = 11
        Width = 29
        Height = 19
        Caption = 'Pos'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label23: TLabel
        Left = 152
        Top = 11
        Width = 155
        Height = 19
        Caption = 'CustomerPosition :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label24: TLabel
        Left = 312
        Top = 13
        Width = 23
        Height = 16
        Caption = 'Pos'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
    object GroupBox3: TGroupBox
      Left = 413
      Top = 41
      Width = 260
      Height = 45
      Caption = 'Stato Posizione'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object ckDisabilitaPos: TCheckBox
        Left = 8
        Top = 19
        Width = 121
        Height = 25
        Caption = 'Disabilitata'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = ckDisabilitaPosClick
      end
      object cPrenotataPos: TCheckBox
        Left = 126
        Top = 19
        Width = 121
        Height = 25
        Caption = 'Prenotata'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = cPrenotataPosClick
      end
    end
    object GroupBox4: TGroupBox
      Left = 7
      Top = 130
      Width = 666
      Height = 123
      Caption = 'Deposito'
      TabOrder = 3
      object GroupBox9: TGroupBox
        Left = 330
        Top = 28
        Width = 307
        Height = 81
        Caption = 'Output'
        TabOrder = 0
        object Label1: TLabel
          Left = 3
          Top = 57
          Width = 38
          Height = 16
          Caption = 'Codice'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object ckOutput1: TCheckBox
          Left = 8
          Top = 20
          Width = 131
          Height = 17
          Caption = 'LGV fuori Ingombro'
          TabOrder = 0
        end
        object eUDC: TEdit
          Left = 58
          Top = 57
          Width = 229
          Height = 21
          Alignment = taRightJustify
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnChange = cbTipoUDCChange
        end
        object BitBtn1: TBitBtn
          Tag = 1
          Left = 183
          Top = 20
          Width = 100
          Height = 25
          Caption = 'Set'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          Visible = False
          OnClick = BitBtn1Click
        end
      end
      object GroupBox6: TGroupBox
        Left = 10
        Top = 24
        Width = 314
        Height = 96
        Caption = 'Input'
        TabOrder = 1
        object ckInput1: TCheckBox
          Left = 11
          Top = 23
          Width = 141
          Height = 17
          Caption = 'Stazione agibile'
          TabOrder = 0
        end
        object ckInput2: TCheckBox
          Left = 176
          Top = 24
          Width = 128
          Height = 17
          Caption = 'Postazione libera'
          TabOrder = 1
        end
        object ckInput3: TCheckBox
          Left = 11
          Top = 46
          Width = 129
          Height = 17
          Caption = 'Consenso Ingresso'
          TabOrder = 2
        end
        object BitBtn6: TBitBtn
          Tag = 1
          Left = 194
          Top = 47
          Width = 97
          Height = 25
          Caption = 'Set'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
          Visible = False
          OnClick = BitBtn6Click
        end
      end
    end
    object GroupBox8: TGroupBox
      Left = 414
      Top = 88
      Width = 259
      Height = 36
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      object Label25: TLabel
        Left = 133
        Top = 13
        Width = 38
        Height = 16
        Caption = 'H.Dep.'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label26: TLabel
        Left = 11
        Top = 12
        Width = 38
        Height = 16
        Caption = 'H.Prel.'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object eHDep: TEdit
        Left = 177
        Top = 10
        Width = 58
        Height = 21
        Alignment = taRightJustify
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnChange = cbTipoUDCChange
      end
      object eHPrel: TEdit
        Left = 59
        Top = 10
        Width = 58
        Height = 21
        Alignment = taRightJustify
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnChange = cbTipoUDCChange
      end
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 301
    Width = 680
    Height = 49
    Align = alBottom
    BevelOuter = bvLowered
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    object SpeedButton1: TSpeedButton
      Left = 282
      Top = 6
      Width = 115
      Height = 38
      AllowAllUp = True
      GroupIndex = 1
      Down = True
      Caption = 'Refresh Mode'
    end
    object BitBtn4: TBitBtn
      Left = 544
      Top = 6
      Width = 129
      Height = 38
      Caption = 'Close'
      TabOrder = 0
      OnClick = BitBtn4Click
    end
    object BitBtn5: TBitBtn
      Left = 8
      Top = 8
      Width = 129
      Height = 38
      Caption = 'Apply'
      TabOrder = 1
      OnClick = BitBtn5Click
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 48
    Top = 8
  end
end
