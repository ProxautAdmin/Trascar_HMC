object FormPlcLavatrice: TFormPlcLavatrice
  Left = 444
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Plc Signal Wash'
  ClientHeight = 459
  ClientWidth = 626
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
    Width = 626
    Height = 32
    Align = alTop
    Caption = 'Data Washing Machine'
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
    Top = 32
    Width = 626
    Height = 378
    Align = alClient
    Anchors = []
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    DesignSize = (
      626
      378)
    object GroupBox2: TGroupBox
      Left = 1
      Top = 219
      Width = 291
      Height = 159
      Anchors = []
      Caption = 'Plc Signal Input'
      TabOrder = 0
      object ckInput1: TCheckBox
        Left = 8
        Top = 25
        Width = 225
        Height = 17
        Caption = 'Richiesta deposito telaio sporco'
        TabOrder = 0
      end
      object ckInput2: TCheckBox
        Left = 8
        Top = 75
        Width = 192
        Height = 17
        Caption = 'Richiesta prelievo telaio pulito'
        TabOrder = 1
      end
      object BitBtn6: TBitBtn
        Tag = 1
        Left = 93
        Top = 131
        Width = 97
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
        OnClick = BitBtn6Click
      end
      object ckInput3: TCheckBox
        Left = 8
        Top = 48
        Width = 225
        Height = 17
        Caption = 'Abilita ingresso telai sporchio'
        TabOrder = 3
      end
      object ckInput4: TCheckBox
        Left = 8
        Top = 98
        Width = 225
        Height = 17
        Caption = 'Abilita ingresso telai puliti'
        TabOrder = 4
      end
    end
    object Panel5: TPanel
      Left = 1
      Top = 1
      Width = 624
      Height = 40
      Align = alTop
      BevelOuter = bvLowered
      Ctl3D = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentCtl3D = False
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
        Left = 213
        Top = 9
        Width = 160
        Height = 19
        Caption = 'Customer Position :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label24: TLabel
        Left = 386
        Top = 11
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
      Left = 1
      Top = 115
      Width = 290
      Height = 52
      Anchors = []
      Caption = 'Position Status'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object ckDisabilitaPos: TCheckBox
        Left = 7
        Top = 21
        Width = 121
        Height = 25
        Caption = 'Disable Position'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object cPrenotataPos: TCheckBox
        Left = 149
        Top = 21
        Width = 121
        Height = 25
        Caption = 'Position reserved'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
    end
    object GroupBox4: TGroupBox
      Left = 298
      Top = 219
      Width = 322
      Height = 159
      Anchors = []
      Caption = 'Plc Signal Output'
      TabOrder = 3
      object ckOutput1: TCheckBox
        Left = 3
        Top = 25
        Width = 187
        Height = 17
        Caption = 'Fuori ingombro telai sporchi'
        TabOrder = 0
      end
      object ckOutput2: TCheckBox
        Left = 3
        Top = 73
        Width = 187
        Height = 17
        Caption = 'Fuori ingombro telai puliti'
        TabOrder = 1
      end
      object ckOutput3: TCheckBox
        Left = 3
        Top = 50
        Width = 187
        Height = 17
        Caption = 'Telaio prelevato'
        TabOrder = 2
      end
      object BitBtn1: TBitBtn
        Tag = 1
        Left = 137
        Top = 131
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
        OnClick = BitBtn1Click
      end
      object ckOutput4: TCheckBox
        Left = 3
        Top = 96
        Width = 187
        Height = 17
        Caption = 'Telaio depositato'
        TabOrder = 4
      end
    end
    object GroupBox1: TGroupBox
      Left = 297
      Top = 120
      Width = 322
      Height = 44
      Anchors = []
      TabOrder = 4
      object Label25: TLabel
        Left = 16
        Top = 12
        Width = 36
        Height = 13
        Caption = 'H.Dep.'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label26: TLabel
        Left = 180
        Top = 12
        Width = 36
        Height = 13
        Caption = 'H.Prel.'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object eHDep: TEdit
        Left = 76
        Top = 9
        Width = 58
        Height = 21
        Alignment = taRightJustify
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
      object eHprel: TEdit
        Left = 242
        Top = 9
        Width = 58
        Height = 21
        Alignment = taRightJustify
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
    object GroupBox5: TGroupBox
      Left = 1
      Top = 173
      Width = 618
      Height = 38
      Anchors = []
      TabOrder = 5
      object Label3: TLabel
        Left = 296
        Top = 14
        Width = 129
        Height = 13
        Caption = 'Ultimo Tipo UDC lavato'
        Font.Charset = ANSI_CHARSET
        Font.Color = clCaptionText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label13: TLabel
        Left = 6
        Top = 11
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
      object SpeedButton2: TSpeedButton
        Left = 162
        Top = 7
        Width = 28
        Height = 25
        Glyph.Data = {
          CA050000424DCA05000000000000360000002800000016000000150000000100
          1800000000009405000000000000000000000000000000000000EAF0EFEAF0EF
          EAF0EFEAEFEFEAEFEFEAEFEFEAEFEFEAF0EFEAF0EFEAF0EFEAF0EFEAEFEFEAEF
          EFEAEFEFEAF0EFEAF0EFEAF0EFEAF0EFEAF0EFEAF0EFEAF0EFEAF0EF0000EAF0
          EFEAF0EFEAF0EFEAEFEFEAEFEFEAEFEFEAEFEFEAF0EFEAF0EFEAF0EFEAF0EFEA
          EFEFEAEFEFEAEFEFEAF0EFEAF0EFEAF0EFEAF0EFEAF0EFEAF0EFEAF0EFEAF0EF
          0000ECF1F1ECF1F1A6A8AA5C6FAF9281A1CAA5AAEAE9E9ECF1F1ECF1F1ECF1F1
          ECF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1
          F1EBF1F10000ECF1F1ECF1F18BA9BB3C85DF5A6DAD9181A1C8A3A9E9E8E8ECF1
          F1ECF1F1ECF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EB
          F1F1EBF1F1EBF1F10000ECF1F1ECF1F198D0F949AEFE3D84DF5B6DAC9181A1CA
          A6ABE9E8E8ECF1F1ECF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1
          EBF1F1EBF1F1EBF1F1EBF1F10000ECF1F1ECF1F1ECF1F194CCF85DC0FE49AEFE
          3C84DE596DAD93819FE8E4E4ECF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1
          F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F10000ECF1F1ECF1F1ECF1F1ECF1F198CD
          F85CC0FF4AAFFF3C83DD596EADCCADB2EBEBEBEBF1F1EBF1F1EBF1F1EBF1F1EB
          F1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F1EBF1F10000ECF1F1ECF1F1ECF1F1EC
          F1F1ECF1F199CEF85BBEFE49AEFE3B83DE9D94A6E3E7E6EBF1F1E2E1E1D4C9C9
          DCD2CEE3D8D5E1D6D6E5E5E5EBF1F1EBF1F1EBF1F1EBF1F10000ECF1F1ECF1F1
          ECF1F1ECF1F1ECF1F1ECF1F199CEF857BCFF4CAFFF8B9098C9CBCBD3C5C5C29E
          97BD9182C7A797D0B1A1CEADA0CCAFAAD7C6C6EAEFEFEBF1F1EBF1F10000EEF2
          F1EEF2F1EEF2F1EFF2F0EFF2F0EFF2F0EFF2F0EFF2F1E2ECF1BA9896CE9B88F4
          DAB1FFFAC9FFFECEFFFFD9FFFFDFFFFFE2FFFFF4F4EEE9CBAFA5EEF2F1EEF2F1
          0000EEF2F1EEF2F1EEF2F1EFF2F0EFF2F0EFF2F0EFF2F0EFF2F1EFF2F1CE9D8D
          F4D7AEFFF7C8FFF1BEFFFBC7FFFFE1FFFFEAFFFFF3FFFFFFFFFFFFF4EEE3DAD1
          D0EEF2F10000EEF2F1EEF2F1EEF2F1EFF2F0EFF2F0EFF2F0EFF2F0EFF2F1EEEE
          EDE0BCA1FFF6C5FFE9B6FFECB9FFFDCBFFFFE2FFFFEFFFFFFCFFFFFEFFFFFAFF
          FFE4B18D8DEDEEED0000EEF2F1EEF2F1EEF2F1EFF2F0EFF2F0EFF2F0EFF2F0EF
          F2F1E6DADAFAF5C5FFECBAFFD9A6FFE6B3FFFBC7FFFFDFFFFFE8FFFFECFFFFEB
          FFFFE6FFFFD8B9988CE5DADA0000EEF2F1EEF2F1EEF2F1EFF2F0EFF2F0EFF2F0
          EFF2F0EFF2F1E6DBDBFEFDCAFFE7B2FFD6A3FFE8B4FFF7C3FFFFD7FFFFDDFFFF
          DFFFFFDFFFFFDCFFFFD3BC988AE5DBDB0000EEF2F1EEF2F1EEF2F1EFF2F0EFF2
          F0EFF2F0EFF2F0EFF2F1E5D9DAFAF4C4FFF3C7FFE9BDFFE2B2FFECB8FFFFCFFF
          FFD2FFFFD6FFFFD5FFFFD1FFFFCDB48B81E4D9DA0000EEF2F1EEF2F1EEF2F1EF
          F2F0EFF2F0EFF2F0EFF2F0EFF2F1EAE5E5EEDCBAFFFCDAFFEECEFFDFB2FFDAA8
          FFF1BFFFF6C4FFFBC9FFFBC8FFF3C1FFFECBA9807DE9E5E50000F0F3F1F0F3F1
          F0F3F1EFF3F1EFF3F1EFF3F1EFF3F1EFF3F1EFF3F1C9A49BEDDECFFFFFFFFFFF
          FFFFEAC9FFDAA9FFD4A1FFDEABFFF4BFFFF2C6EDC098ECEBEAF0F2F10000F0F3
          F1F0F3F1F0F3F1EFF3F1EFF3F1EFF3F1EFF3F1EFF3F1EFF3F1DBCDCDC5A19BED
          E0D7FFFFFDFFFFECFFF2BEFFF6C2FFFBCBFFEBBDEEBD95B88881F0F2F1F0F2F1
          0000F0F3F1F0F3F1F0F3F1EFF3F1EFF3F1EFF3F1EFF3F1EFF3F1EFF3F1EFF3F1
          DDD1CFB8908CCBA791EDD6B1FDF0C5F9E6BBF1D2AAD3A793BC9290DFD4D4F0F2
          F1F0F2F10000F0F3F1F0F3F1F0F3F1EFF3F1EFF3F1EFF3F1EFF3F1EFF3F1EFF3
          F1EFF3F1EFF3F1EFF3F2EFF3F2EFF3F2EFF2F2EFF2F2EFF2F2EFF2F2EFF2F2F0
          F2F1F0F2F1F0F2F10000F0F3F1F0F3F1F0F3F1EFF3F1EFF3F1EFF3F1EFF3F1EF
          F3F1EFF3F1EFF3F1EFF3F1EFF3F2EFF3F2EFF3F2EFF2F2EFF2F2EFF2F2EFF2F2
          EFF2F2F0F2F1F0F2F1F0F2F10000}
        OnClick = SpeedButton2Click
      end
      object SpeedButton3: TSpeedButton
        Left = 189
        Top = 7
        Width = 28
        Height = 25
        Glyph.Data = {
          F6060000424DF606000000000000360000002800000018000000180000000100
          180000000000C006000000000000000000000000000000000000FFFFFF6763CF
          1712B61813BA1812BA1812BA1812BA1712B91712B91712B91711B91711B91711
          B91711B91711B81711B81711B81711B81711B81711B81711B71610B36662CDFF
          FFFF6966D4201DD52724ED2624ED2623ED2622ED2622ED2621ED2621ED2520ED
          2520ED251FED251EED241EED241DED241DED241DED241CED241CED241CED241C
          ED241CED1E17D36763D11A16BD2929EE2829EE2828EE2828EE2220DA2321DF27
          26ED2725ED2725ED2724ED2724ED2624ED2623ED2622ED2622ED2621ED1A15C2
          211CDF2520ED251FED251EED241EED1812B91D19C42A2EEE2A2DEE2A2DEE2526
          E1140FAA140FAA2525E1292AEE2929EE2929EE2829EE2828EE2828EE2827ED28
          26ED1A16C1130DA6140FAA2320E02724ED2624ED2623ED1A15C01E1BC72C33EE
          2C32EE272BE21611AF6C69CA403CBA1611AF2629E22B2FEE2A2EEE2A2DEE2A2D
          EE2A2CEE2A2CEE1C19C5140FABA7A5DF6C69CA1610AF2524E12828EE2828EE1C
          17C31F1DC92D37EE2930E31813B46D6ACDFFFFFFF0F0FA423DBF1813B4282CE3
          2C33EE2C32EE2C31EE2C31EE2324D71611B0A8A5E1FFFFFFFFFFFF6D6ACD1812
          B42526DF2A2CEE1D19C6201FCC2F3DEF2225CE1812B6A9A6E4FFFFFFFFFFFFF1
          F0FA6E6BD11915B92930E42D37EE2D37EE2528D91812B6A8A6E3FFFFFFFFFFFF
          F1F0FB433FC41914B9282CE32C31EE1F1BC92120CE3141EF3141EF2225CE1A14
          BBA9A7E6FFFFFFFFFFFFFFFFFF706CD41B16BF2B34E5272CDB1A14BBA9A7E5FF
          FFFFFFFFFFFFFFFF4540C81B16BE2931E42D37EE2D36EE1F1DCB2222D13346EF
          3245EF3245EF2427D11B15C0AAA7E8FFFFFFFFFFFFFFFFFF716DD71D17C31B15
          C0AAA7E7FFFFFFFFFFFFFFFFFF716DD81D17C32B35E62F3CEE2F3BEE2E3BEE21
          1ECD2423D3344AEF344AEF3449EF3449EF2932DC1D16C4AAA8E9FFFFFFFFFFFF
          FFFFFF716DDA716DDAFFFFFFFFFFFFFFFFFF726DDB1E18C72D3AE73141EF3141
          EF3040EF3040EF2220D02525D5374EF0374DEF364DEF364CEF364CEF2D38E01E
          16C9ABA8EBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF726EDD1F19CB2F3EE8
          3346EF3346EF3245EF3245EF3244EF2322D35656E04C61F23951F03950F03850
          F0384FF0384FF02E3AE21F17CD817DE3FFFFFFFFFFFFFFFFFFFFFFFF736EE01F
          17CD3141E8354BEF354BEF344AEF344AEF3449EF475AF15454DE5D5DE36D7EF4
          6679F45066F23E55F03A52F03A52F03547EA211CD3736FE2FFFFFFFFFFFFFFFF
          FFFFFFFF817DE52018D12F3AE4384EF0374EF03A50F04D60F16274F36879F35B
          5BE15E5EE56E81F56E81F56E80F46E80F45E73F34E5EEE302BDA746FE5FFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFABA8EF2F27D84752EA5C6FF36B7CF46B7C
          F46B7CF46A7CF45D5CE35F5FE77184F57083F57083F57083F56B79F25A56E497
          94EDFFFFFFFFFFFFFFFFFF9E9BEF9490EDFFFFFFFFFFFFFFFFFFC1BFF45953E3
          656EEE6D7EF46C7EF46C7EF46C7EF45D5DE55F60E97286F57286F57186F56D7C
          F25B56E79794EFFFFFFFFFFFFFFFFFFFC1BFF65953E65953E69794F0FFFFFFFF
          FFFFFFFFFFC1BFF65953E66164EC6E81F56E80F46E80F45E5EE76060EA7489F5
          7488F56E7EF35B56EA7873EDF5F4FEFFFFFFFFFFFFC1BFF75953E96973F06D7C
          F35B56EA9794F1FFFFFFFFFFFFFFFFFFC1BFF75953E96265EE7083F57083F55F
          5FE86061EC758BF57080F35C57EC7974EFF5F4FEFFFFFFFFFFFFC1BFF85A54EC
          6A75F17488F57488F56F7EF35C57EC9894F3FFFFFFFFFFFFFFFFFFC1BFF85A54
          EC646AF07185F55F60EA6162EE778DF56E7BF35C58EF9894F5FFFFFFFFFFFFC1
          BFF95A54EE6A73F2758BF5758AF5758AF5758AF57080F45C57EF7974F2F5F4FE
          FFFFFF9894F55C57EF6F7FF47388F56060EB6263EF798FF5798FF57384F45D58
          F09895F6C2BFFA5B55F0666AF2788EF5778DF5778DF5778DF5778CF5778CF571
          82F45D58F07A75F39895F65C58F07081F4758AF5758AF56061ED6263F07B92F6
          7B91F67B91F67586F55D59F25B55F2676BF37A90F57A90F5798FF5798FF5798F
          F5798FF5798FF5788EF57384F45D59F25D59F27283F4778DF5778DF5778CF561
          62EE6161F17D93F67D93F67D93F67D93F67787F56A70F47C92F67C92F67C92F6
          7C92F67B91F67B91F67A91F57A90F57A90F57A90F57485F4707EF4798FF5798F
          F5798FF5798FF5605FEF9797F6717CF47E94F67E94F67E94F67E94F67E94F67E
          94F67E94F67E94F67D93F67D93F67D93F67D93F67D93F67D92F67C92F67C92F6
          7C92F67C92F67B91F67B91F66F7AF39796F5FFFFFF9897F86261F36465F36465
          F36465F36465F36465F36465F36465F36465F36465F36465F36465F36465F364
          65F36465F36465F36465F26464F26464F26160F29797F7FFFFFF}
        OnClick = SpeedButton3Click
      end
      object eUDC: TEdit
        Left = 77
        Top = 9
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
      object ComboBox1: TComboBox
        Left = 431
        Top = 8
        Width = 168
        Height = 24
        TabOrder = 1
        Text = 'ComboBox1'
      end
    end
    object Panel3: TPanel
      Left = 7
      Top = 47
      Width = 611
      Height = 62
      Anchors = []
      BevelOuter = bvLowered
      TabOrder = 6
      object Label1: TLabel
        Left = 288
        Top = 29
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
        Top = 30
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
        Top = 45
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
        Top = 45
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
        Width = 609
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
        Top = 32
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
      object GroupBox6: TGroupBox
        Left = 144
        Top = 25
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
  end
  object Panel4: TPanel
    Left = 0
    Top = 410
    Width = 626
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
      Left = 259
      Top = 6
      Width = 115
      Height = 38
      AllowAllUp = True
      GroupIndex = 1
      Down = True
      Caption = 'Refresh Mode'
    end
    object BitBtn4: TBitBtn
      Left = 490
      Top = 6
      Width = 129
      Height = 38
      Caption = 'Close'
      TabOrder = 0
      OnClick = BitBtn4Click
    end
    object BitBtn5: TBitBtn
      Left = 7
      Top = 6
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
