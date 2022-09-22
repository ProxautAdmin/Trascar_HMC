object FormDatiUDC: TFormDatiUDC
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'Data UDC'
  ClientHeight = 601
  ClientWidth = 968
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnActivate = FormActivate
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 197
    Height = 49
    TabOrder = 0
    object Label13: TLabel
      Left = 20
      Top = 13
      Width = 38
      Height = 13
      Caption = 'Id UDC'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object eUDC: TEdit
      Left = 87
      Top = 11
      Width = 79
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 0
    end
  end
  object GroupBox3: TGroupBox
    Left = 211
    Top = 8
    Width = 355
    Height = 49
    TabOrder = 1
    object Label21: TLabel
      Left = 5
      Top = 19
      Width = 59
      Height = 13
      Caption = 'Id Art.UDC'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label1: TLabel
      Left = 173
      Top = 18
      Width = 59
      Height = 13
      Caption = 'Id Articolo'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object eIDArtUDCm: TEdit
      Left = 70
      Top = 11
      Width = 79
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 0
    end
    object edArticolo: TEdit
      Left = 238
      Top = 10
      Width = 79
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 1
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 552
    Width = 968
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
    ExplicitTop = 487
    object BitBtnClose: TBitBtn
      Left = 416
      Top = 6
      Width = 129
      Height = 38
      Caption = 'Close'
      TabOrder = 0
      OnClick = BitBtnCloseClick
    end
  end
  inline fUDCSmall: TfUDCSmall
    Left = 8
    Top = 63
    Width = 953
    Height = 483
    TabOrder = 3
    ExplicitLeft = 8
    ExplicitTop = 63
    ExplicitHeight = 483
    inherited GroupBox2: TGroupBox
      inherited sbSearch: TSpeedButton
        OnClick = fUDCSmallsbSearchClick
      end
      inherited FrameModulo1: TFrameModulo
        inherited GroupBox1: TGroupBox
          ExplicitHeight = 67
        end
      end
      inherited FrameModulo2: TFrameModulo
        inherited GroupBox1: TGroupBox
          ExplicitHeight = 67
        end
      end
      inherited FrameModulo3: TFrameModulo
        inherited GroupBox1: TGroupBox
          ExplicitWidth = 461
          ExplicitHeight = 67
        end
      end
      inherited FrameModulo4: TFrameModulo
        inherited GroupBox1: TGroupBox
          ExplicitWidth = 461
          ExplicitHeight = 67
        end
      end
      inherited FrameModulo5: TFrameModulo
        inherited GroupBox1: TGroupBox
          ExplicitWidth = 461
          ExplicitHeight = 67
        end
      end
      inherited FrameModulo6: TFrameModulo
        inherited GroupBox1: TGroupBox
          ExplicitWidth = 461
          ExplicitHeight = 67
        end
      end
    end
    inherited BtnWritePlc: TBitBtn
      Left = 509
      ExplicitLeft = 509
    end
  end
end
