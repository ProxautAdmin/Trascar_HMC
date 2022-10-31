object frZonaA2: TfrZonaA2
  Left = 0
  Top = 0
  Width = 1206
  Height = 382
  TabOrder = 0
  object pnA: TPanel
    Left = 0
    Top = 0
    Width = 1206
    Height = 382
    Align = alClient
    TabOrder = 0
    object pnPosA1: TPanel
      Tag = 111
      AlignWithMargins = True
      Left = 19
      Top = 8
      Width = 200
      Height = 169
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 1'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 0
      OnDblClick = pnPosA1DblClick
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA2: TPanel
      Tag = 112
      AlignWithMargins = True
      Left = 225
      Top = 8
      Width = 200
      Height = 169
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 2'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 1
      OnDblClick = pnPosA1DblClick
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA3: TPanel
      Tag = 113
      AlignWithMargins = True
      Left = 431
      Top = 8
      Width = 200
      Height = 169
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 3'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 2
      OnDblClick = pnPosA1DblClick
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA4: TPanel
      Tag = 114
      AlignWithMargins = True
      Left = 19
      Top = 183
      Width = 200
      Height = 169
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 4'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 3
      OnDblClick = pnPosA1DblClick
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA5: TPanel
      Tag = 115
      AlignWithMargins = True
      Left = 225
      Top = 183
      Width = 200
      Height = 169
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 5'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 4
      OnDblClick = pnPosA1DblClick
      OnMouseUp = pnPosAMouseUp
    end
    object pnPosA6: TPanel
      Tag = 116
      AlignWithMargins = True
      Left = 431
      Top = 183
      Width = 200
      Height = 169
      BorderWidth = 10
      BorderStyle = bsSingle
      Caption = 'Posizione 6'
      Color = clHighlightText
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 5
      OnDblClick = pnPosA1DblClick
      OnMouseUp = pnPosAMouseUp
    end
    inline FrameUDCinMateriePrime: TFrameUDCinMateriePrime
      Left = 672
      Top = 8
      Width = 513
      Height = 344
      TabOrder = 6
      ExplicitLeft = 672
      ExplicitTop = 8
      ExplicitWidth = 513
      ExplicitHeight = 344
      inherited Panel1: TPanel
        Width = 507
        Height = 334
        ExplicitWidth = 507
        ExplicitHeight = 334
        inherited DBGrid1: TDBGrid
          Width = 505
          Height = 332
        end
      end
    end
  end
  object ADOQuery1: TADOQuery
    Connection = dmDB.ADOConnection1
    CursorType = ctStatic
    Parameters = <>
    SQL.Strings = (
      'select * from AnagraficaArticoliVasche')
    Left = 56
    Top = 232
  end
  object DataSource1: TDataSource
    DataSet = ADOQuery1
    Left = 319
    Top = 232
  end
  object ADOQuery2: TADOQuery
    Connection = dmDB.ADOConnection1
    CursorType = ctStatic
    Parameters = <>
    SQL.Strings = (
      'select * from AnagraficaArticoliVasche')
    Left = 56
    Top = 232
  end
  object DataSource2: TDataSource
    DataSet = ADOQuery2
    Left = 319
    Top = 232
  end
end
