object FrameArticoliInLavorazione: TFrameArticoliInLavorazione
  Left = 0
  Top = 0
  Width = 536
  Height = 304
  DoubleBuffered = True
  ParentDoubleBuffered = False
  TabOrder = 0
  OnEnter = FrameEnter
  object Panel1: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 5
    Width = 530
    Height = 294
    Margins.Top = 5
    Margins.Bottom = 5
    Align = alClient
    Caption = 'Panel1'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 0
    VerticalAlignment = taAlignTop
    ExplicitWidth = 392
    ExplicitHeight = 277
    object DBGrid1: TDBGrid
      Left = 1
      Top = 1
      Width = 528
      Height = 292
      Align = alClient
      DataSource = DataSource1
      Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
      TabOrder = 0
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = 'Posizione'
          Title.Caption = 'Pos.'
          Title.Font.Charset = DEFAULT_CHARSET
          Title.Font.Color = clWindowText
          Title.Font.Height = -11
          Title.Font.Name = 'Tahoma'
          Title.Font.Style = [fsBold]
          Width = 40
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'Articolo'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ReadOnly = True
          Title.Font.Charset = DEFAULT_CHARSET
          Title.Font.Color = clWindowText
          Title.Font.Height = -11
          Title.Font.Name = 'Tahoma'
          Title.Font.Style = [fsBold]
          Width = 120
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'Descrizione Articolo'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ReadOnly = True
          Title.Caption = 'Descrizione'
          Title.Font.Charset = DEFAULT_CHARSET
          Title.Font.Color = clWindowText
          Title.Font.Height = -11
          Title.Font.Name = 'Tahoma'
          Title.Font.Style = [fsBold]
          Width = 300
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'Sovrapposto'
          Title.Caption = 'Imp.'
          Title.Font.Charset = DEFAULT_CHARSET
          Title.Font.Color = clWindowText
          Title.Font.Height = -11
          Title.Font.Name = 'Tahoma'
          Title.Font.Style = [fsBold]
          Width = 40
          Visible = True
        end>
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
end