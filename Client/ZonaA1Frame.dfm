object frZonaA1: TfrZonaA1
  Left = 0
  Top = 0
  Width = 1135
  Height = 250
  TabOrder = 0
  object pnA: TPanel
    Left = 0
    Top = 0
    Width = 1135
    Height = 250
    Align = alClient
    TabOrder = 0
    ExplicitWidth = 915
    object lbSelA: TLabel
      Left = 64
      Top = 8
      Width = 169
      Height = 17
      AutoSize = False
      Caption = '0'
      Transparent = False
    end
    object Label1: TLabel
      Left = 936
      Top = 40
      Width = 105
      Height = 13
      AutoSize = False
      Caption = 'Tipo Articolo'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object pnPosA1: TPanel
      Tag = 101
      AlignWithMargins = True
      Left = 16
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
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA2: TPanel
      Tag = 102
      AlignWithMargins = True
      Left = 143
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
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA3: TPanel
      Tag = 103
      AlignWithMargins = True
      Left = 270
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
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA4: TPanel
      Tag = 104
      AlignWithMargins = True
      Left = 397
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
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA5: TPanel
      Tag = 105
      AlignWithMargins = True
      Left = 524
      Top = 40
      Width = 121
      Height = 193
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 5'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 4
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA6: TPanel
      Tag = 106
      AlignWithMargins = True
      Left = 651
      Top = 40
      Width = 121
      Height = 193
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 6'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 5
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA7: TPanel
      Tag = 107
      AlignWithMargins = True
      Left = 778
      Top = 40
      Width = 121
      Height = 193
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 7'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 6
      OnMouseUp = pnPosAMouseUp
    end
    object ListBox1: TListBox
      Left = 936
      Top = 59
      Width = 161
      Height = 174
      DoubleBuffered = True
      ItemHeight = 13
      Items.Strings = (
        'art 1'
        'art 2'
        'art 3')
      ParentDoubleBuffered = False
      TabOrder = 7
    end
  end
end
