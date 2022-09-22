object frZonaC: TfrZonaC
  Left = 0
  Top = 0
  Width = 915
  Height = 250
  TabOrder = 0
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 915
    Height = 250
    Align = alClient
    TabOrder = 0
    object lbSelH: TLabel
      Left = 64
      Top = 8
      Width = 169
      Height = 17
      AutoSize = False
      Caption = '0'
      Transparent = False
    end
    object pnPosC1: TPanel
      Tag = 601
      AlignWithMargins = True
      Left = 232
      Top = 31
      Width = 433
      Height = 193
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 1'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 0
      OnMouseUp = pnPosCMouseUp
    end
  end
end
