object frZonaH: TfrZonaH
  Left = 0
  Top = 0
  Width = 1111
  Height = 305
  Align = alClient
  TabOrder = 0
  ExplicitWidth = 451
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1111
    Height = 305
    Align = alClient
    TabOrder = 0
    ExplicitWidth = 451
    object lbSelH: TLabel
      Left = 64
      Top = 8
      Width = 169
      Height = 17
      AutoSize = False
      Caption = '0'
      Transparent = False
    end
    object pnPosH1: TPanel
      Tag = 401
      AlignWithMargins = True
      Left = 200
      Top = 40
      Width = 121
      Height = 193
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 1'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 0
      OnMouseUp = pnPosH1MouseUp
    end
    object pnPosH2: TPanel
      Tag = 402
      AlignWithMargins = True
      Left = 327
      Top = 40
      Width = 121
      Height = 193
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 2'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 1
      OnMouseUp = pnPosH1MouseUp
    end
    object pnPosH3: TPanel
      Tag = 403
      AlignWithMargins = True
      Left = 454
      Top = 40
      Width = 121
      Height = 193
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 3'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 2
      OnMouseUp = pnPosH1MouseUp
    end
    object pnPosH4: TPanel
      Tag = 404
      AlignWithMargins = True
      Left = 581
      Top = 40
      Width = 121
      Height = 193
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 4'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 3
      OnMouseUp = pnPosH1MouseUp
    end
    object Panel2: TPanel
      Left = 835
      Top = 8
      Width = 209
      Height = 217
      TabOrder = 4
      object Label1: TLabel
        Left = 8
        Top = 9
        Width = 193
        Height = 25
        Alignment = taCenter
        AutoSize = False
        Caption = 'Articoli'
        Color = cl3DLight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        Transparent = False
        Layout = tlCenter
      end
      object sbCerca: TSpeedButton
        Left = 8
        Top = 163
        Width = 193
        Height = 46
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C0000CA990000CA9900000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFBFBFBE2E2E2B9B9B9838383131313353535E8E8E8FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8
          F8F8C5C5C5696969272727090909010101000000090909B8B8B8FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEEAEAEA76
          76761515150000000000000000000000000000000A0A0ABEBEBEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFECECEC5E5E5E04
          0404000000000000000000000000000000000000161616D2D2D2FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFC88888806060600
          0000000000000000000000000000000000000000313131EBEBEBFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFECECECE1F1F1F00000000
          00000000000000000000000000000000000000006C6C6CFCFCFCFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4F4F45E5E5E00000000000000
          00000000000000000000000000000000000E0E0EBCBCBCFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFDA2A2A20D0D0D00000000000000
          0000000000000000000000000000000000565656F3F3F3FFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFED2D2D227272700000000000000000000
          0000000000000000000000000000262626CECECEFEFEFEFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFBFBFBEDEDEDDFDFDFD8D8D8DEDE
          DEECECECFAFAFAFEFEFEFFFFFFF3F3F359595901010100000000000000000000
          00000000000000000101012C2C2CB6B6B6FDFDFDFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFEFEFEEEEEEEB0B0B06565653636362020201919191F1F
          1F333333616161ACACACECECECF6F6F68C8C8C16161600000000000000000000
          0000000000111111696969D8D8D8FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFF9F9F9B6B6B64545450A0A0A0000000000000000000000000000
          00000000000000090909404040B1B1B1F3F3F3B0B0B022222200000000000004
          04043F3F3FB7B7B7F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFF4F4F48989891414140000000000000000000000000000000000000000
          00000000000000000000000000111111808080F0F0F0B5B5B51C1C1C12121278
          7878E5E5E5FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          F8F8F88686860A0A0A0000000000000000000000000000000000000000000000
          000000000000000000000000000000000909097B7B7BF2F2F2ACACACB1B1B1F8
          F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE
          AAAAAA1111110000000000000000000000000000000404041717172222221717
          170505050000000000000000000000000000000C0C0C9E9E9EFCFCFCFEFEFEFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE3E3E3
          3232320000000000000000000000000404043D3D3D9D9D9DD3D3D3E1E1E1D3D3
          D3A0A0A04040400505050000000000000000000000002C2C2CDDDDDDFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE959595
          040404000000000000000000040404656565E5E5E5FEFEFEFFFFFFFFFFFFFFFF
          FFFEFEFEE9E9E9717171070707000000000000000000020202878787FDFDFDFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5F5F5484848
          000000000000000000000000494949E9E9E9FEFEFEFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFEEEEEE545454000000000000000000000000393939EDEDEDFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDDDDDD1F1F1F
          0000000000000000000E0E0EB9B9B9FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFEFEFEC1C1C1111111000000000000000000151515D0D0D0FF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC4C4C40D0D0D
          0000000000000000002F2F2FEAEAEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFEFEFEF393939000000000000000000070707B5B5B5FF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB7B7B7080808
          000000000000000000454545F6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFAFAFA525252000000000000000000050505ABABABFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCBCBC090909
          0000000000000000003D3D3DF3F3F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFF7F7F7494949000000000000000000050505ADADADFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF141414
          0000000000000000001E1E1ED8D8D8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFDFDFDF2525250000000000000000000C0C0CC2C2C2FF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAEAEA2F2F2F
          000000000000000000030303868686FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFCFCFC929292050505000000000000000000232323DFDFDFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFC6A6A6A
          0000000000000000000000001B1B1BB7B7B7FDFDFDFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFDFDFDC2C2C22222220000000000000000000000005B5B5BF8F8F8FF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBF
          1111110000000000000000000000002121219E9E9EECECECFDFDFDFEFEFEFDFD
          FDEFEFEFA5A5A52727270000000000000000000000000E0E0EB6B6B6FEFEFEFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6F6
          6868680202020000000000000000000000000909093939396E6E6E8585857070
          703D3D3D0B0B0B0000000000000000000000000101015D5D5DF4F4F4FFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE
          DFDFDF3B3B3B0000000000000000000000000000000000000000000000000000
          00000000000000000000000000000000000000313131D6D6D6FEFEFEFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FEFEFECCCCCC3535350101010000000000000000000000000000000000000000
          000000000000000000000000000000002F2F2FC4C4C4FEFEFEFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFEFEFED7D7D75555550808080000000000000000000000000000000000
          000000000000000000000707074C4C4CD1D1D1FEFEFEFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFEFEFEF0F0F0A5A5A54848481515150303030000000000000000
          00020202131313434343A0A0A0EDEDEDFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3F3CECECEA0A0A03333330000003131
          319D9D9DCBCBCBF2F2F2FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        OnClick = sbCercaClick
      end
      object lIdUDC: TLabel
        Left = 8
        Top = 40
        Width = 193
        Height = 13
        Alignment = taCenter
        AutoSize = False
        Caption = '0'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object tDescArticolo: TEdit
        Left = 8
        Top = 126
        Width = 193
        Height = 27
        Alignment = taCenter
        AutoSize = False
        Color = clWhite
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
        Text = '...'
      end
      object edIDArt: TEdit
        Left = 8
        Top = 65
        Width = 193
        Height = 27
        Alignment = taCenter
        Color = 8454143
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
        Text = '...'
      end
      object edCodArt: TEdit
        Left = 8
        Top = 98
        Width = 193
        Height = 27
        Alignment = taCenter
        AutoSize = False
        Color = clWhite
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 2
        Text = '...'
      end
    end
  end
  object ImageListMenu: TImageList
    Left = 107
    Top = 118
    Bitmap = {
      494C010104001800200510001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000002000000001002000000000000020
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FEFEFE00FCFCFC00FBFBFB00FBFB
      FB0000000000FCFCFC0000000000FBFBFB00FBFBFB00F2F2F200212121000000
      0000060606009F9F9F00FBFBFB00FEFEFE00FBFBFB00FFFFFF00FFFFFF00FFFF
      FF00FCFCFC00FFFFFF00FFFFFF00FFFFFF00FCFCFC00FFFFFF00FFFFFF00FFFF
      FF00FDFDFD00FFFFFF00FFFFFF00FFFFFF00FDFDFD000E0E0E000E0E0E000E0E
      0E000E0E0E000E0E0E000E0E0E000E0E0E000E0E0E000E0E0E000E0E0E000E0E
      0E000E0E0E000E0E0E000E0E0E00FDFDFD000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000003838380000000000F2F2F200FCFCFC00F6F6F600F6F6F600F8F8
      F800FBFBFB00FEFEFE00FAFAFA00F8F8F800BEBEBE0005050500E6E6E600FCFC
      FC00FCFCFC004545450017171700FBFBFB00FFFFFF00FBFBFB00FFFFFF00FFFF
      FF00FFFFFF00FCFCFC00FFFFFF00FFFFFF00FFFFFF00FCFCFC00FFFFFF00FFFF
      FF00FFFFFF00FDFDFD00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000005F5F5F000808
      0800090909000A0A0A000E0E0E00080808000C0C0C00080808000E0E0E000A0A
      0A000E0E0E0000000000F1F1F10000000000FEFEFE00F6F6F600F6F6F600FBFB
      FB00F8F8F80000000000F7F7F700FAFAFA0004040400F6F6F600FDFDFD00F9F9
      F90000000000F7F7F70045454500A9A9A900FFFFFF00FFFFFF00DADADA004141
      41003C3C3C003C3C3C0051515100FFFFFF00FFFFFF00E9E9E900FCFCFC00FFFF
      FF00FFFFFF00FFFFFF00FDFDFD00FFFFFF00000000000B0B0B00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000C0C0C00000000000000000000000000AFAFAF00FBFB
      FB00FBFBFB00FBFBFB00FCFCFC00FBFBFB00FBFBFB00FBFBFB00FCFCFC00FBFB
      FB00FCFCFC00000000000000000000000000FCFCFC00FCFCFC00FBFBFB00F6F6
      F60000000000F4F4F40000000000C8C8C80059595900FDFDFD00F5F5F5000000
      0000F5F5F50000000000F7F7F70006060600FFFFFF00A9A9A900010101006969
      690075757500757575008C8C8C00FCFCFC00FFFFFF000000000050505000FCFC
      FC00FFFFFF00FFFFFF00FFFFFF00FDFDFD000000000072727200000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000074747400000000005050500079797900000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000484848000000000000000000FAFAFA00FBFBFB000000
      0000F3F3F30000000000F3F3F3009A9A9A0099999900F8F8F80000000000F2F2
      F20000000000F6F6F6000000000000000000FCFCFC0001010100E1E1E100FFFF
      FF00FBFBFB00FFFFFF00FFFFFF00FFFFFF00FCFCFC0000000000000000005151
      5100FCFCFC00FFFFFF00FFFFFF00FFFFFF0000000000EBEBEB00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000ECECEC00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FCFCFC00FEFEFE00FEFEFE00F6F6
      F60000000000F2F2F200000000009E9E9E009B9B9B0000000000F4F4F4000000
      0000F3F3F30000000000E5E5E50027272700FFFFFF0000000000F6F6F600FFFF
      FF00FFFFFF00F5F5F500F9F9F900F9F9F900F9F9F90000000000000000000000
      000050505000FCFCFC00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000F8F8F800F8F8F800FEFE
      FE00F5F5F50000000000F6F6F600FDFDFD00F8F8F800F6F6F60000000000F5F5
      F50000000000F6F6F60003030300F2F2F200FFFFFF0000000000F2F2F200FFFF
      FF007E7E7E000000000000000000000000000000000000000000000000000000
      00000000000052525200FCFCFC00FFFFFF000000000000000000010101000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000010101000000000000000000F1F1F10000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FCFCFC00F8F8F800F8F8F800C8C8
      C800999999009D9D9D00999999009F9F9F009C9C9C009A9A9A009F9F9F009999
      99005A5A5A0002020200BFBFBF00FBFBFB00FFFFFF0000000000F6F6F600FCFC
      FC007D7D7D000000000000000000000000000000000000000000000000000000
      0000000000000000000051515100FCFCFC000000000000000000383838000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000003A3A3A0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000181818000000000000000000FCFCFC00BEBEBE00040404005B5B
      5B00999999009B9B9B009A9A9A009F9F9F009D9D9D009B9B9B009D9D9D009A9A
      9A00C8C8C800FAFAFA00F6F6F600FBFBFB00FCFCFC0000000000F6F6F600FFFF
      FF007C7C7C000000000000000000000000000000000000000000000000000000
      0000000000000000000051515100FFFFFF000000000000000000C3C3C3000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C5C5C50000000000000000000000000000000000C5C5C5007D7D
      7D00838383007C7C7C008484840081818100848484007D7D7D00808080008484
      840084848400080808000000000000000000F2F2F20005050500F6F6F600FDFD
      FD00F6F6F60000000000F6F6F600D5D5D500D5D5D500F6F6F600FEFEFE00F6F6
      F60000000000F5F5F500FAFAFA00FCFCFC00FFFFFF0000000000F6F6F600FFFF
      FF007E7E7E000000000000000000000000000000000000000000000000000000
      00000000000051515100FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000363636005E5E5E0022222200E7E7E700FDFDFD00F5F5
      F50000000000F3F3F300D6D6D6000303030001010100D5D5D500F4F4F4000000
      0000F3F3F30000000000F9F9F900FDFDFD00FFFFFF0000000000F3F3F300FFFF
      FF00FFFFFF00F9F9F900F5F5F500F9F9F900F9F9F90000000000000000000000
      000051515100FFFFFF00FCFCFC00FFFFFF000000000000000000000000007272
      7200727272007272720072727200000000000000000072727200727272007272
      7200727272000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FAFAFA00FBFBFB000000
      0000F3F3F300000000006262620099999900999999006363630000000000F2F2
      F20000000000F6F6F6000000000000000000FFFFFF0001010100E1E1E100FCFC
      FC00FFFFFF00FFFFFF00FFFFFF00FCFCFC00FFFFFF0000000000000000004F4F
      4F00FFFFFF00FFFFFF00FFFFFF00FCFCFC000000000000000000000000000000
      0000000000000000000001010100010101000101010001010100000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000007070700FCFCFC00FCFCFC00F5F5
      F50000000000F3F3F300000000005C5C5C00C9C9C900FEFEFE00F5F5F5000000
      0000F4F4F40000000000F8F8F800FDFDFD00FDFDFD00A9A9A900010101006A6A
      6A0073737300757575008A8A8A00FFFFFF00FCFCFC000000000051515100FFFF
      FF00FBFBFB00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000000000616161005151510000000000000000004F4F4F00646464000000
      0000000000000000000000000000000000000000000000000000A3A3A3000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000068686800323232009F9F9F0045454500F7F7F700FCFC
      FC00F7F7F70000000000F6F6F60002020200F8F8F800F6F6F600FEFEFE00F8F8
      F80000000000F6F6F600FAFAFA00FBFBFB00FFFFFF00FDFDFD00DEDEDE004141
      41003C3C3C003C3C3C0053535300FFFFFF00FFFFFF00E6E6E600FFFFFF00FFFF
      FF00FFFFFF00FBFBFB00FFFFFF00FFFFFF000000000000000000000000000000
      000000000000484848007C7C7C000000000000000000797979004A4A4A000000
      000000000000000000000000000000000000000000006060600000000000A0A0
      A00098989800A2A2A2009E9E9E009A9A9A009A9A9A009F9F9F00A2A2A2009999
      99009E9E9E00414141000000000000000000FCFCFC001717170045454500F7F7
      F700FCFCFC00E6E6E60003030300BEBEBE00F6F6F600F8F8F800F9F9F900FEFE
      FE00FCFCFC00FAFAFA00F6F6F600FBFBFB00FFFFFF00FFFFFF00FDFDFD00FFFF
      FF00FFFFFF00FFFFFF00FCFCFC00FFFFFF00FFFFFF00FFFFFF00FCFCFC00FFFF
      FF00FFFFFF00FFFFFF00FBFBFB00FFFFFF000000000000000000000000000000
      000000000000F7F7F70000000000A8A8A800A8A8A80000000000F7F7F7000000
      0000000000000000000000000000000000000000000071717100000000008D8D
      8D008A8A8A00939393009A9A9A0089898900929292008D8D8D009B9B9B008D8D
      8D009A9A9A00C9C9C9000000000000000000FEFEFE00FCFCFC00AAAAAA000707
      07000000000027272700F2F2F200FBFBFB00FBFBFB00FCFCFC00FCFCFC000000
      0000FDFDFD00FDFDFD00FBFBFB00FEFEFE00FFFFFF00FFFFFF00FFFFFF00FDFD
      FD00FFFFFF00FFFFFF00FFFFFF00FCFCFC00FFFFFF00FFFFFF00FFFFFF00FCFC
      FC00FFFFFF00FFFFFF00FFFFFF00FBFBFB000000000000000000000000000000
      00000000000000000000EEEEEE001F1F1F001F1F1F00EEEEEE00000000000000
      0000000000000000000000000000000000000000000000000000949494000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000A0000000000FFF8000000008001C000
      04080000800180000A14000080013FFD942A000080017FFF0A540000C0037FFF
      84280000C0033FFF00000000C003800300000000C003C00104080000E007FFFC
      08140000E007FFFE142B0000FC3FFFFE0A140000F99F9FFC04080000F99F0001
      00000000F81F000300100000FC3F9FFF00000000000000000000000000000000
      000000000000}
  end
  object PopupMenu: TPopupMenu
    Images = ImageListMenu
    Left = 59
    Top = 110
  end
end
