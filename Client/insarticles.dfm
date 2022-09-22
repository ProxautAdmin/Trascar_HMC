object FormInsertArticles: TFormInsertArticles
  Left = 227
  Top = 318
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Insert/Modify Type'
  ClientHeight = 230
  ClientWidth = 654
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 654
    Height = 41
    Align = alTop
    Caption = 'Insert / Modify Type'
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
    Top = 181
    Width = 654
    Height = 49
    Align = alBottom
    BevelOuter = bvLowered
    TabOrder = 1
    object BitBtn1: TBitBtn
      Left = 48
      Top = 4
      Width = 241
      Height = 41
      Caption = 'Insert / Modify Type'
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
      Left = 397
      Top = 6
      Width = 241
      Height = 41
      Caption = 'Close'
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
    Width = 654
    Height = 140
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 2
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 22
      Height = 16
      Caption = 'Id :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 174
      Top = 24
      Width = 71
      Height = 16
      Caption = 'Type Code '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 16
      Top = 65
      Width = 82
      Height = 16
      Caption = 'Description :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Color: TShape
      Left = 528
      Top = 63
      Width = 110
      Height = 25
      OnMouseUp = ColorMouseUp
    end
    object Label9: TLabel
      Left = 560
      Top = 24
      Width = 38
      Height = 16
      Caption = 'Color:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object EdIdArticles: TEdit
      Left = 62
      Top = 21
      Width = 73
      Height = 24
      ReadOnly = True
      TabOrder = 0
    end
    object EdName: TEdit
      Left = 294
      Top = 22
      Width = 219
      Height = 24
      CharCase = ecUpperCase
      TabOrder = 1
    end
    object EdDesc: TEdit
      Left = 110
      Top = 63
      Width = 403
      Height = 24
      TabOrder = 2
    end
    object TipoCarne: TRadioGroup
      Left = 16
      Top = 93
      Width = 622
      Height = 38
      Caption = 'Meat Type'
      Columns = 3
      Items.Strings = (
        'NONE'
        'LOW-FAT MEAT'
        'FAT MEAT')
      TabOrder = 3
    end
  end
  object Query: TADOQuery
    Connection = dmDB.ADOConnection1
    Parameters = <>
    Left = 48
    Top = 8
  end
  object ColorDialog1: TColorDialog
    Left = 104
  end
end
