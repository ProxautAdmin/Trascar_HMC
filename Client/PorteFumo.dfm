object FormStatusPorteFumo: TFormStatusPorteFumo
  Left = 428
  Top = 226
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Status Porte'
  ClientHeight = 557
  ClientWidth = 623
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
  OnDeactivate = FormDeactivate
  PixelsPerInch = 96
  TextHeight = 16
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 623
    Height = 41
    Align = alTop
    Caption = 'Status Porte Fumo'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
  end
  object Panel2: TPanel
    Left = 0
    Top = 497
    Width = 623
    Height = 60
    Align = alBottom
    BevelOuter = bvLowered
    TabOrder = 1
    object SpeedButton1: TSpeedButton
      Left = 259
      Top = 4
      Width = 124
      Height = 52
      AllowAllUp = True
      GroupIndex = 1
      Down = True
      Caption = 'Refresh Mode'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object BitBtn1: TBitBtn
      Left = 24
      Top = 4
      Width = 124
      Height = 52
      Caption = 'OK'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn3: TBitBtn
      Left = 472
      Top = 4
      Width = 124
      Height = 52
      Caption = 'Close'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Glyph.Data = {
        360C0000424D360C000000000000360000002800000020000000200000000100
        180000000000000C0000120B0000120B00000000000000000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFDF8F8F8FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEDFDFDF7D7D7D585858E0E0
        E0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB9797972E2E2E0303030B0B0BBCBC
        BCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFF6F6F6B2B2B24141410707070101010000000A0A0ABBBB
        BBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFAFAFAC7C7C75B5B5B0F0F0F0101010000000000000000000A0A0ABBBB
        BBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        DDDDDD7575751818180101010000000000000000000000000000000808088E8E
        8EC2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2C2D9D9D9FEFEFEFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2
        4B4B4B0303030000000000000000000000000000000000000000000101010909
        090C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0D0D0D353535E4E4E4FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000303033838
        384D4D4D4D4D4D4D4D4D4D4D4D4D4D4D4747471010101A1A1AD8D8D8FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0AB6B6
        B6F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9E8E8E83232321A1A1AD8D8D8FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE3434341A1A1AD8D8D8FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE3434341A1A1AD8D8D8FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE3434341A1A1AD8D8D8FDFDFDD7
        D7D7F1F1F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0F0F04A4A4A333333DDDDDDEAEAEA3B
        3B3B595959D6D6D6FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFDE3E3E3E0E0E0FAFAFAE6E6E628
        28280202022C2C2CACACACF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFDFDFDDFDFDFD0D0D0D0D0D0D0D0D0D0D0D0BBBBBB20
        20200000000101011212127A7A7AE8E8E8FFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFC2C2C229292914141414141414141414141412121204
        0404000000000000000000050505484848D3D3D3FFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFA2A2A206060600000000000000000000000000000000
        0000000000000000000000010101121212A6A6A6FFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFECECEC8B8B8B6C6C6C6C6C6C6C6C6C6C6C6C61616111
        11110000000000000202022B2B2BAAAAAAF8F8F8FFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFEFEFEFFFFFFE6E6E628
        2828010101080808555555D5D5D5FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6F68E8E8E808080EAEAEAE6E6E628
        2828191919898989EEEEEEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE3434341A1A1AD8D8D8F4F4F485
        8585BBBBBBFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000A0A0ABABA
        BAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE3434341A1A1AD8D8D8FFFFFFFE
        FEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000000000000000D0D0DBFBF
        BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE3434341A1A1AD8D8D8FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000000000000000101011515156F6F6FECEC
        ECFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE3434341A1A1AD8D8D8FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000000000101010B0B0B4F4F4FBFBFBFF9F9F9FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEEEE3434341A1A1AD8D8D8FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E000000000000000000030303343434A0A0A0EBEBEBF9F9F9FAFAFAFAFA
        FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAE9E9E93232321A1A1AD8D8D8FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBEBEB
        2E2E2E0000000000000000000707073737374E4E4E4F4F4F4F4F4F4F4F4F4F4F
        4F4F4F4F4F4F4F4F4F4F4F4F4F4F4F4F4949491010101A1A1AD8D8D8FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2
        4B4B4B0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C
        0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C343434E4E4E4FFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        E0E0E0C1C1C1C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C1C1C1D8D8D8FEFEFEFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentFont = False
      TabOrder = 1
      OnClick = BitBtn3Click
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 41
    Width = 623
    Height = 456
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 2
    object Label1: TLabel
      Left = 24
      Top = 24
      Width = 102
      Height = 19
      Caption = 'Door Name :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 331
      Top = 24
      Width = 141
      Height = 19
      Caption = 'Door Id Number :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 478
      Top = 24
      Width = 115
      Height = 19
      Caption = 'Port Number :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 24
      Top = 60
      Width = 103
      Height = 19
      Caption = 'Description :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label10: TLabel
      Left = 136
      Top = 24
      Width = 102
      Height = 19
      Caption = 'Door Name :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label7: TLabel
      Left = 24
      Top = 158
      Width = 249
      Height = 19
      Caption = 'Agvx String (Proxaut settings)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label11: TLabel
      Left = 24
      Top = 94
      Width = 213
      Height = 19
      Caption = 'Position for Open the Port'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label12: TLabel
      Left = 24
      Top = 128
      Width = 201
      Height = 19
      Caption = 'Position for Port Opened'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object RadioGroup2: TRadioGroup
      Left = 19
      Top = 186
      Width = 590
      Height = 46
      Caption = 'Door Mode'
      Columns = 4
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Items.Strings = (
        'Undefined'
        'Automatic '
        'Manual'
        'Disabled')
      ParentFont = False
      TabOrder = 0
      OnClick = RadioGroup2Click
    end
    object Edit2: TEdit
      Left = 136
      Top = 60
      Width = 473
      Height = 24
      TabOrder = 1
    end
    object Edit4: TEdit
      Left = 279
      Top = 158
      Width = 330
      Height = 24
      TabOrder = 2
    end
    object GroupBox2: TGroupBox
      Left = 19
      Top = 238
      Width = 590
      Height = 113
      Caption = 'Door Input Status'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      object Label5: TLabel
        Left = 16
        Top = 24
        Width = 72
        Height = 16
        Caption = 'Status Clima'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clDefault
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 255
        Top = 24
        Width = 78
        Height = 16
        Caption = 'Status Door 1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clDefault
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label8: TLabel
        Left = 416
        Top = 24
        Width = 78
        Height = 16
        Caption = 'Status Door 2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clDefault
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 16
        Top = 52
        Width = 111
        Height = 16
        Caption = 'Status Load/Unload'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clDefault
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object eInput1: TEdit
        Left = 152
        Top = 22
        Width = 73
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object eInput2: TEdit
        Left = 352
        Top = 22
        Width = 33
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object eInput3: TEdit
        Left = 503
        Top = 22
        Width = 33
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object eInput4: TEdit
        Left = 152
        Top = 50
        Width = 73
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object cbInput1: TCheckBox
        Left = 16
        Top = 82
        Width = 120
        Height = 17
        Caption = 'Door 1 Opened'
        TabOrder = 4
        OnClick = CheckBox1Click
      end
      object cbInput2: TCheckBox
        Left = 152
        Top = 82
        Width = 129
        Height = 17
        Caption = 'Door 1 Closed'
        TabOrder = 5
        OnClick = CheckBox1Click
      end
      object cbInput3: TCheckBox
        Left = 287
        Top = 82
        Width = 130
        Height = 17
        Caption = 'Door 2 Opened'
        TabOrder = 6
        OnClick = CheckBox1Click
      end
      object cbInput4: TCheckBox
        Left = 431
        Top = 82
        Width = 122
        Height = 17
        Caption = 'Door 2 Closed'
        TabOrder = 7
        OnClick = CheckBox1Click
      end
    end
    object RadioGroup1: TRadioGroup
      Left = 19
      Top = 355
      Width = 590
      Height = 93
      Caption = 'Ouput Status'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Items.Strings = (
        'None'
        'Agv Loading/Unloading Line 1'
        'Agv Loading/Unloading Line 2')
      ParentFont = False
      TabOrder = 4
      OnClick = RadioGroup1Click
    end
    object Edit1: TEdit
      Left = 243
      Top = 94
      Width = 366
      Height = 24
      TabOrder = 5
    end
    object Edit3: TEdit
      Left = 243
      Top = 128
      Width = 366
      Height = 24
      TabOrder = 6
    end
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 32
    Top = 16
  end
end
