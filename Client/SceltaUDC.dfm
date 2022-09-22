object FormSceltaUDC: TFormSceltaUDC
  Left = 444
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'UDC'
  ClientHeight = 556
  ClientWidth = 591
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  OnDeactivate = FormDeactivate
  DesignSize = (
    591
    556)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 591
    Height = 32
    Align = alTop
    Caption = 'UDC Choice'
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
    Top = 448
    Width = 591
    Height = 59
    Align = alBottom
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object btAll: TBitBtn
      Left = 70
      Top = 15
      Width = 80
      Height = 24
      Caption = 'All'
      TabOrder = 0
      OnClick = btAllClick
    end
    object btNone: TBitBtn
      Left = 383
      Top = 15
      Width = 80
      Height = 24
      Caption = 'None'
      TabOrder = 1
      OnClick = btNoneClick
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 507
    Width = 591
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
    object BitBtn4: TBitBtn
      Left = 363
      Top = 6
      Width = 100
      Height = 38
      Caption = 'Close'
      TabOrder = 0
      OnClick = BitBtn4Click
    end
    object BitBtn5: TBitBtn
      Left = 110
      Top = 6
      Width = 100
      Height = 38
      Caption = 'Ok'
      TabOrder = 1
      OnClick = BitBtn5Click
    end
  end
  object GroupBox4: TGroupBox
    Left = 10
    Top = 100
    Width = 574
    Height = 345
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = 'UDC'
    TabOrder = 3
    object cb1: TCheckBox
      Tag = 1
      Left = 299
      Top = 168
      Width = 30
      Height = 15
      Hint = 'cb1'
      Caption = '1'
      TabOrder = 0
    end
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 34
    Width = 575
    Height = 60
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 4
    object lbZona: TLabel
      Left = 113
      Top = 4
      Width = 89
      Height = 25
      AutoSize = False
      Caption = 'lbZona'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbRoom: TLabel
      Left = 113
      Top = 32
      Width = 89
      Height = 25
      AutoSize = False
      Caption = 'lbRoom'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbCustPos: TLabel
      Left = 13
      Top = 16
      Width = 89
      Height = 25
      AutoSize = False
      Caption = 'lbCustPos'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
end
