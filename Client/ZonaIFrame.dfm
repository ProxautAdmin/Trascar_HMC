object frZonaI: TfrZonaI
  Left = 0
  Top = 0
  Width = 1275
  Height = 324
  TabOrder = 0
  object pnI: TPanel
    Left = 0
    Top = 0
    Width = 1275
    Height = 324
    Align = alClient
    TabOrder = 0
    ExplicitHeight = 317
    object pnPosI1: TPanel
      Tag = 201
      AlignWithMargins = True
      Left = 30
      Top = 7
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 1'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 0
      OnMouseUp = pnPosIMouseUp
    end
    object pnPosI2: TPanel
      Tag = 202
      AlignWithMargins = True
      Left = 196
      Top = 7
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 2'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 1
      OnMouseUp = pnPosIMouseUp
    end
    object pnPosI3: TPanel
      Tag = 203
      AlignWithMargins = True
      Left = 357
      Top = 7
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 3'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 2
      OnMouseUp = pnPosIMouseUp
    end
    object pnPosI4: TPanel
      Tag = 204
      AlignWithMargins = True
      Left = 523
      Top = 7
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 4'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 3
      OnMouseUp = pnPosIMouseUp
    end
    object pnPosI5: TPanel
      Tag = 205
      AlignWithMargins = True
      Left = 689
      Top = 7
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 5'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 4
      OnMouseUp = pnPosIMouseUp
    end
    object pnPosI6: TPanel
      Tag = 206
      AlignWithMargins = True
      Left = 30
      Top = 130
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 6'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 5
      OnMouseUp = pnPosIMouseUp
    end
    object pnPosI7: TPanel
      Tag = 207
      AlignWithMargins = True
      Left = 196
      Top = 130
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 7'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 6
      OnMouseUp = pnPosIMouseUp
    end
    object pnPosI8: TPanel
      Tag = 208
      AlignWithMargins = True
      Left = 357
      Top = 130
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 8'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 7
      OnMouseUp = pnPosIMouseUp
    end
    object pnPosI9: TPanel
      Tag = 209
      AlignWithMargins = True
      Left = 523
      Top = 130
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 9'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 8
      OnMouseUp = pnPosIMouseUp
    end
    object pnPosI10: TPanel
      Tag = 210
      AlignWithMargins = True
      Left = 689
      Top = 130
      Width = 160
      Height = 117
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 10'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 9
      OnMouseUp = pnPosIMouseUp
    end
    inline FrameMatPrime: TFrameMatPrime
      Left = 870
      Top = 7
      Width = 400
      Height = 310
      TabOrder = 10
      ExplicitLeft = 870
      ExplicitTop = 7
      ExplicitWidth = 400
      ExplicitHeight = 310
      inherited Panel1: TPanel
        Width = 394
        Height = 300
        ExplicitWidth = 394
        ExplicitHeight = 300
        inherited DBGrid1: TDBGrid
          Width = 392
          Height = 298
        end
      end
    end
    object pInfoUDC: TPanel
      Left = 8
      Top = 254
      Width = 859
      Height = 57
      TabOrder = 11
      object Label2: TLabel
        Left = 7
        Top = 12
        Width = 121
        Height = 25
        AutoSize = False
        Caption = 'Selezione'
        Color = cl3DLight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        Transparent = False
        Layout = tlCenter
      end
      object leDescArticolo: TLabeledEdit
        Left = 515
        Top = 12
        Width = 326
        Height = 21
        AutoSelect = False
        AutoSize = False
        BorderStyle = bsNone
        Color = clBtnHighlight
        DoubleBuffered = True
        EditLabel.Width = 65
        EditLabel.Height = 16
        EditLabel.Caption = 'Descrizione'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -13
        EditLabel.Font.Name = 'Tahoma'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        EditLabel.Transparent = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        LabelPosition = lpLeft
        LabelSpacing = 10
        ParentDoubleBuffered = False
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
      object leIdUDC: TLabeledEdit
        Left = 159
        Top = 12
        Width = 96
        Height = 21
        AutoSelect = False
        AutoSize = False
        BorderStyle = bsNone
        Color = clBtnHighlight
        DoubleBuffered = True
        EditLabel.Width = 11
        EditLabel.Height = 16
        EditLabel.Caption = 'Id'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -13
        EditLabel.Font.Name = 'Tahoma'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        EditLabel.Transparent = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        LabelPosition = lpLeft
        LabelSpacing = 10
        ParentDoubleBuffered = False
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
      end
      object leCodArt: TLabeledEdit
        Left = 327
        Top = 12
        Width = 96
        Height = 21
        AutoSelect = False
        AutoSize = False
        BorderStyle = bsNone
        Color = clBtnHighlight
        DoubleBuffered = True
        EditLabel.Width = 51
        EditLabel.Height = 16
        EditLabel.Caption = 'Cod. Art.'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -13
        EditLabel.Font.Name = 'Tahoma'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        EditLabel.Transparent = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        LabelPosition = lpLeft
        LabelSpacing = 10
        ParentDoubleBuffered = False
        ParentFont = False
        ReadOnly = True
        TabOrder = 2
      end
    end
  end
end
