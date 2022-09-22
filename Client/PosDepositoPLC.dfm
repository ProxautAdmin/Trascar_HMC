object FormPosDepPLC: TFormPosDepPLC
  Left = 411
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Plc Signal'
  ClientHeight = 418
  ClientWidth = 689
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
    Width = 689
    Height = 32
    Align = alTop
    Caption = 'Status Deposito'
    Color = clAqua
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
    Width = 689
    Height = 337
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
      Height = 94
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
        Caption = 'Connesso   '
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
        Caption = 'Watch Dog'
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
      Width = 687
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
        Width = 84
        Height = 19
        Caption = 'Posizione:'
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
        Width = 136
        Height = 19
        Caption = 'Nome Posizione:'
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
      object Label1: TLabel
        Left = 409
        Top = 9
        Width = 46
        Height = 19
        Caption = 'Zona:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbZona: TLabel
        Left = 470
        Top = 9
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
      object Label4: TLabel
        Left = 520
        Top = 9
        Width = 81
        Height = 19
        Caption = 'Tipo Pos.:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbTipoPos: TLabel
        Left = 632
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
    end
    object GroupBox3: TGroupBox
      Left = 418
      Top = 40
      Width = 263
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
        Top = 21
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
    object gbPos: TGroupBox
      Left = 418
      Top = 92
      Width = 263
      Height = 45
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      object Label7: TLabel
        Left = 141
        Top = 12
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
      object Label8: TLabel
        Left = 8
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
      object eHDep1: TEdit
        Left = 185
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
        OnChange = ckPresenzaUDCClick
      end
      object eHPrel1: TEdit
        Left = 67
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
        OnChange = ckPresenzaUDCClick
      end
    end
    object gbPLCOut: TGroupBox
      Left = 414
      Top = 143
      Width = 267
      Height = 176
      Caption = 'Output'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 4
      object Label11: TLabel
        Left = 24
        Top = 111
        Width = 46
        Height = 16
        Caption = 'Id Pallet'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object cbRichiestaAbilitaDep: TCheckBox
        Tag = 3
        Left = 21
        Top = 30
        Width = 204
        Height = 17
        Caption = 'Richiesta Deposito'
        TabOrder = 0
      end
      object cbAGVInIngombro: TCheckBox
        Tag = 3
        Left = 21
        Top = 54
        Width = 132
        Height = 17
        Caption = 'AGV In Ingombro'
        TabOrder = 1
      end
      object SetOutBtn: TBitBtn
        Tag = 1
        Left = 187
        Top = 148
        Width = 66
        Height = 25
        Caption = 'Set'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = BitBtn1Click
      end
      object editIdPallet: TEdit
        Left = 82
        Top = 108
        Width = 121
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Text = '0'
      end
    end
    object gbMatter: TGroupBox
      Left = 7
      Top = 143
      Width = 401
      Height = 125
      Caption = 'Input'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 5
      object cbIn0: TCheckBox
        Tag = 3
        Left = 242
        Top = 30
        Width = 156
        Height = 17
        Caption = 'Pronta Al Deposito'
        TabOrder = 0
      end
      object cbIn1: TCheckBox
        Tag = 3
        Left = 242
        Top = 54
        Width = 146
        Height = 17
        Caption = 'Ok Deposito'
        TabOrder = 1
      end
      object cbIn2: TCheckBox
        Tag = 3
        Left = 25
        Top = 30
        Width = 156
        Height = 17
        Caption = 'Ready'
        TabOrder = 2
      end
      object cbIn3: TCheckBox
        Tag = 3
        Left = 25
        Top = 54
        Width = 156
        Height = 17
        Caption = 'In Allarme'
        TabOrder = 3
      end
    end
    object GroupBox6: TGroupBox
      Left = 8
      Top = 274
      Width = 400
      Height = 46
      Caption = 'Dati UDC'
      TabOrder = 6
      Visible = False
      object ckInput1: TCheckBox
        Tag = 1
        Left = 24
        Top = 21
        Width = 137
        Height = 17
        Caption = 'Presenza UDC'
        TabOrder = 0
        OnClick = ckInput1Click
      end
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 369
    Width = 689
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
      Caption = 'Chiudi'
      TabOrder = 0
      OnClick = BitBtn4Click
    end
    object BitBtn5: TBitBtn
      Left = 8
      Top = 8
      Width = 129
      Height = 38
      Caption = 'Conferma'
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
