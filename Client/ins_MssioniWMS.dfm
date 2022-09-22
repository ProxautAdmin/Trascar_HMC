object FIns_MissioniWMS: TFIns_MissioniWMS
  Left = 227
  Top = 318
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Insert/Modify Type'
  ClientHeight = 328
  ClientWidth = 697
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlue
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 16
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 697
    Height = 41
    Align = alTop
    Caption = 'Inserisci / Modifica Missioni WMS'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
  end
  object Panel2: TPanel
    Left = 0
    Top = 279
    Width = 697
    Height = 49
    Align = alBottom
    BevelOuter = bvLowered
    TabOrder = 1
    object BitBtn1: TBitBtn
      Left = 88
      Top = 6
      Width = 193
      Height = 41
      Caption = 'Conferma'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn2: TBitBtn
      Left = 416
      Top = 6
      Width = 193
      Height = 41
      Caption = 'Esci'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = BitBtn2Click
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 41
    Width = 697
    Height = 238
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 2
    object Label8: TLabel
      Left = 496
      Top = 71
      Width = 89
      Height = 16
      Caption = 'Corsia Deposito'
    end
    object Label7: TLabel
      Left = 198
      Top = 118
      Width = 55
      Height = 16
      Caption = 'Peso UDC'
    end
    object Label5: TLabel
      Left = 16
      Top = 118
      Width = 40
      Height = 16
      Caption = 'ID UDC'
    end
    object Label3: TLabel
      Left = 16
      Top = 74
      Width = 52
      Height = 16
      Caption = 'Data/Ora'
    end
    object Label2: TLabel
      Left = 338
      Top = 71
      Width = 73
      Height = 16
      Caption = 'Pos.Deposito'
    end
    object Label1: TLabel
      Left = 200
      Top = 71
      Width = 50
      Height = 16
      Caption = 'Pos.Prel.'
    end
    object Label4: TLabel
      Left = 517
      Top = 115
      Width = 58
      Height = 16
      Caption = 'Tipo Miss.'
    end
    object Label6: TLabel
      Left = 16
      Top = 23
      Width = 12
      Height = 16
      Caption = 'ID'
    end
    object Label9: TLabel
      Left = 16
      Top = 161
      Width = 41
      Height = 16
      Caption = 'Priorita'
    end
    object Label10: TLabel
      Left = 350
      Top = 118
      Width = 53
      Height = 16
      Caption = 'Tipo UDC'
    end
    object Label11: TLabel
      Left = 338
      Top = 161
      Width = 64
      Height = 16
      Caption = 'Stato WMS'
    end
    object Label12: TLabel
      Left = 197
      Top = 161
      Width = 40
      Height = 16
      Caption = 'ID AGV'
    end
    object Label13: TLabel
      Left = 504
      Top = 161
      Width = 77
      Height = 16
      Caption = 'Stato Proxaut'
    end
    object edDataOraCreazioneWMS: TEdit
      Left = 91
      Top = 68
      Width = 97
      Height = 24
      TabOrder = 0
    end
    object edCorsiaDeposito: TEdit
      Left = 592
      Top = 68
      Width = 73
      Height = 24
      TabOrder = 1
    end
    object edPesoUDC: TEdit
      Left = 269
      Top = 115
      Width = 63
      Height = 24
      Alignment = taRightJustify
      NumbersOnly = True
      TabOrder = 2
    end
    object edIDUDC: TEdit
      Left = 88
      Top = 115
      Width = 100
      Height = 24
      TabOrder = 3
    end
    object edPosizioneDeposito: TEdit
      Left = 416
      Top = 68
      Width = 65
      Height = 24
      TabOrder = 4
    end
    object edPosizionePrelievo: TEdit
      Left = 269
      Top = 68
      Width = 63
      Height = 24
      TabOrder = 5
    end
    object edTipoMissione: TEdit
      Left = 592
      Top = 115
      Width = 73
      Height = 24
      Alignment = taRightJustify
      MaxLength = 1
      NumbersOnly = True
      TabOrder = 6
    end
    object edID: TEdit
      Left = 91
      Top = 20
      Width = 62
      Height = 24
      Enabled = False
      ReadOnly = True
      TabOrder = 7
    end
    object edPriorita: TEdit
      Left = 88
      Top = 158
      Width = 48
      Height = 24
      Alignment = taRightJustify
      MaxLength = 1
      NumbersOnly = True
      TabOrder = 8
    end
    object edTipoUDC: TEdit
      Left = 416
      Top = 115
      Width = 65
      Height = 24
      Alignment = taRightJustify
      MaxLength = 1
      NumbersOnly = True
      TabOrder = 9
    end
    object edStatoMissioneWMS: TEdit
      Left = 416
      Top = 158
      Width = 65
      Height = 24
      Alignment = taRightJustify
      MaxLength = 1
      NumbersOnly = True
      TabOrder = 10
    end
    object edIDAgv: TEdit
      Left = 269
      Top = 158
      Width = 63
      Height = 24
      Alignment = taRightJustify
      MaxLength = 1
      NumbersOnly = True
      TabOrder = 11
    end
    object edStatoMissioneProxaut: TEdit
      Left = 592
      Top = 158
      Width = 73
      Height = 24
      Alignment = taRightJustify
      MaxLength = 2
      NumbersOnly = True
      TabOrder = 12
    end
  end
  object Query: TADOQuery
    Connection = dmDB.ADOConnection1
    Parameters = <>
    Left = 48
    Top = 8
  end
end
