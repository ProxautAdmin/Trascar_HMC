object FrameInfo: TFrameInfo
  Left = 0
  Top = 0
  Width = 760
  Height = 159
  TabOrder = 0
  object GroupBox1: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 478
    Height = 94
    Ctl3D = True
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 11
      Top = 11
      Width = 85
      Height = 13
      Caption = 'From Bitech in GO'
    end
    object Label2: TLabel
      Left = 11
      Top = 68
      Width = 57
      Height = 13
      Caption = 'Double UDC'
    end
    object Label3: TLabel
      Left = 11
      Top = 41
      Width = 93
      Height = 13
      Caption = 'Last Mission Center'
    end
    object lbTimeToProx: TLabel
      Left = 357
      Top = 6
      Width = 114
      Height = 13
      AutoSize = False
      Caption = '  -  -  -'
    end
    object lbTimeDouble: TLabel
      Left = 357
      Top = 60
      Width = 114
      Height = 13
      AutoSize = False
      Caption = '  -  -  -'
    end
    object lbTimeToMission: TLabel
      Left = 357
      Top = 33
      Width = 114
      Height = 13
      AutoSize = False
      Caption = '  -  -  -'
    end
    object edToProx: TEdit
      Left = 130
      Top = 6
      Width = 221
      Height = 21
      ReadOnly = True
      TabOrder = 0
    end
    object edDouble: TEdit
      Left = 130
      Top = 60
      Width = 221
      Height = 21
      ReadOnly = True
      TabOrder = 1
    end
    object edMisC: TEdit
      Left = 130
      Top = 33
      Width = 221
      Height = 21
      ReadOnly = True
      TabOrder = 2
    end
  end
  object gbZoom: TGroupBox
    Left = 487
    Top = 0
    Width = 266
    Height = 154
    Caption = 'Zoom Zone'
    TabOrder = 1
    object btZona1: TButton
      Left = 16
      Top = 31
      Width = 113
      Height = 25
      Caption = 'Zona 1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btZona1Click
    end
    object btZona2: TButton
      Left = 135
      Top = 31
      Width = 113
      Height = 25
      Caption = 'Zona 2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btZona2Click
    end
  end
  object GroupBox2: TGroupBox
    Left = 3
    Top = 103
    Width = 478
    Height = 53
    TabOrder = 2
    object lbCont: TLabel
      Left = 410
      Top = 19
      Width = 49
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0'
    end
    object Label5: TLabel
      Left = 357
      Top = 19
      Width = 66
      Height = 13
      Caption = 'Stat. Count.'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 3
      Top = 19
      Width = 98
      Height = 13
      Caption = 'Cerca Udc su mappa'
    end
    object Button3: TButton
      Left = 255
      Top = 12
      Width = 77
      Height = 25
      Caption = 'Reset Search'
      TabOrder = 0
      OnClick = Button3Click
    end
    object Button1: TButton
      Left = 191
      Top = 12
      Width = 44
      Height = 25
      Caption = 'Search'
      TabOrder = 1
      OnClick = Button1Click
    end
    object Edit1: TEdit
      Left = 119
      Top = 16
      Width = 66
      Height = 21
      TabOrder = 2
    end
  end
  object TimerInfo: TTimer
    Interval = 10000
    OnTimer = TimerInfoTimer
    Left = 704
    Top = 83
  end
end
