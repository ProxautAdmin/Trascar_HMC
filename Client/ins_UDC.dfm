object FormInsUDC: TFormInsUDC
  Left = 227
  Top = 318
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Gestione UDC'
  ClientHeight = 417
  ClientWidth = 807
  Color = clBtnFace
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
  object pnTitolo: TPanel
    Left = 0
    Top = 0
    Width = 807
    Height = 41
    Align = alTop
    Caption = 'Gestione UDC'
    Color = cl3DLight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
  end
  object Panel2: TPanel
    Left = 0
    Top = 368
    Width = 807
    Height = 49
    Align = alBottom
    BevelOuter = bvLowered
    Color = cl3DLight
    ParentBackground = False
    TabOrder = 1
    object btConferma: TBitBtn
      Left = 112
      Top = 6
      Width = 193
      Height = 41
      Caption = 'Conferma'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = btConfermaClick
    end
    object btEsci: TBitBtn
      Left = 528
      Top = 6
      Width = 193
      Height = 41
      Caption = 'Esci'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = btEsciClick
    end
  end
  object pnUDC: TPanel
    Left = 0
    Top = 41
    Width = 807
    Height = 190
    Align = alTop
    BevelOuter = bvLowered
    TabOrder = 2
    object Label5: TLabel
      Left = 341
      Top = 74
      Width = 30
      Height = 16
      Caption = 'Stato'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 13
      Top = 9
      Width = 40
      Height = 16
      Caption = 'ID UDC'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label1: TLabel
      Left = 13
      Top = 74
      Width = 79
      Height = 16
      Caption = 'Cod. tipo UDC'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 13
      Top = 44
      Width = 50
      Height = 16
      Caption = 'Cod.UDC'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 341
      Top = 44
      Width = 65
      Height = 16
      Caption = 'Descrizione'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 13
      Top = 116
      Width = 76
      Height = 16
      Caption = 'Codice colata'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 341
      Top = 116
      Width = 27
      Height = 16
      AutoSize = False
      Caption = 'Peso'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 341
      Top = 146
      Width = 93
      Height = 16
      AutoSize = False
      Caption = 'Ind.Impilabilita'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object edIDUDC: TEdit
      Left = 112
      Top = 6
      Width = 97
      Height = 24
      TabOrder = 0
    end
    object cbTipoUDC: TComboBox
      Left = 112
      Top = 71
      Width = 186
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
    object cbStatus: TComboBox
      Left = 430
      Top = 71
      Width = 188
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object edCodUDC: TEdit
      Left = 112
      Top = 41
      Width = 186
      Height = 24
      MaxLength = 20
      TabOrder = 3
    end
    object edDescUDC: TEdit
      Left = 430
      Top = 41
      Width = 250
      Height = 24
      MaxLength = 30
      TabOrder = 4
    end
    object edLotto: TEdit
      Left = 112
      Top = 113
      Width = 186
      Height = 24
      MaxLength = 20
      TabOrder = 5
    end
    object edPeso: TEdit
      Left = 430
      Top = 113
      Width = 186
      Height = 24
      MaxLength = 20
      NumbersOnly = True
      TabOrder = 6
    end
    object cbRiservato: TCheckBox
      Left = 13
      Top = 150
      Width = 114
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Riservato'
      TabOrder = 7
    end
    object cbParziale: TCheckBox
      Left = 184
      Top = 150
      Width = 114
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Parziale'
      TabOrder = 8
    end
    object edIntImp: TEdit
      Left = 430
      Top = 143
      Width = 186
      Height = 24
      MaxLength = 20
      TabOrder = 9
    end
  end
  object pnArticolo: TPanel
    Left = 0
    Top = 231
    Width = 807
    Height = 68
    Align = alClient
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 3
    object Label2: TLabel
      Left = 20
      Top = 22
      Width = 59
      Height = 16
      Caption = 'ID Articolo'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Left = 341
      Top = 22
      Width = 65
      Height = 16
      Caption = 'Descrizione'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lbDescArt: TLabel
      Left = 430
      Top = 18
      Width = 250
      Height = 24
      AutoSize = False
      Caption = 'lbDescArt'
      Color = cl3DLight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Transparent = False
      Layout = tlCenter
    end
    object edCodArt: TEdit
      Left = 95
      Top = 19
      Width = 114
      Height = 24
      MaxLength = 20
      TabOrder = 0
    end
  end
  object pnContenitore: TPanel
    Left = 0
    Top = 299
    Width = 807
    Height = 69
    Align = alBottom
    TabOrder = 4
    object Label12: TLabel
      Left = 20
      Top = 22
      Width = 43
      Height = 16
      Caption = 'ID Base'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label13: TLabel
      Left = 341
      Top = 22
      Width = 65
      Height = 16
      Caption = 'Descrizione'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lbDescContenitore: TLabel
      Left = 430
      Top = 18
      Width = 250
      Height = 24
      AutoSize = False
      Caption = 'Label11'
      Color = cl3DLight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Transparent = False
      Layout = tlCenter
    end
    object cbBase: TComboBox
      Left = 95
      Top = 19
      Width = 203
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object Query: TADOQuery
    Connection = dmDB.ADOConnection1
    Parameters = <>
    Left = 48
    Top = 8
  end
end
