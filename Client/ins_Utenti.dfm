object FrmInsUtenti: TFrmInsUtenti
  Left = 227
  Top = 318
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'xxxxxx'
  ClientHeight = 240
  ClientWidth = 433
  Color = clWindow
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlue
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnActivate = FormActivate
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 16
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 433
    Height = 41
    Align = alTop
    Caption = 'Gestione Utente'
    Color = cl3DLight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    ExplicitWidth = 528
  end
  object Panel2: TPanel
    Left = 0
    Top = 191
    Width = 433
    Height = 49
    Align = alBottom
    BevelOuter = bvLowered
    Color = cl3DLight
    ParentBackground = False
    TabOrder = 1
    ExplicitWidth = 528
    object BitBtn1: TBitBtn
      Left = 23
      Top = 6
      Width = 170
      Height = 41
      Caption = 'Conferma'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn2: TBitBtn
      Left = 244
      Top = 6
      Width = 160
      Height = 41
      Caption = 'Esci'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
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
    Width = 433
    Height = 150
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 2
    ExplicitWidth = 528
    object Label1: TLabel
      Left = 14
      Top = 24
      Width = 17
      Height = 16
      Caption = 'Id '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 14
      Top = 114
      Width = 40
      Height = 16
      Caption = 'Livello'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 14
      Top = 53
      Width = 92
      Height = 16
      Caption = 'Identificativo'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 14
      Top = 84
      Width = 63
      Height = 16
      Caption = 'Password'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object edId: TEdit
      Left = 120
      Top = 21
      Width = 73
      Height = 22
      Enabled = False
      NumbersOnly = True
      ReadOnly = True
      TabOrder = 0
    end
    object edPassword: TEdit
      Left = 120
      Top = 81
      Width = 284
      Height = 22
      MaxLength = 20
      TabOrder = 1
    end
    object edUser: TEdit
      Left = 120
      Top = 51
      Width = 284
      Height = 22
      MaxLength = 20
      TabOrder = 2
    end
    object cbLivello: TComboBox
      Left = 120
      Top = 111
      Width = 91
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        '8'
        '9')
    end
    object cbSistema: TCheckBox
      Left = 307
      Top = 115
      Width = 97
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Sistema'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
    end
  end
  object Query: TADOQuery
    Connection = dmDB.ADOConnection1
    Parameters = <>
    Left = 48
    Top = 8
  end
end
