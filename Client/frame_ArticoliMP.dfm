object FrameMatPrime: TFrameMatPrime
  Left = 0
  Top = 0
  Width = 398
  Height = 287
  TabOrder = 0
  OnEnter = FrameEnter
  object Panel1: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 5
    Width = 392
    Height = 277
    Margins.Top = 5
    Margins.Bottom = 5
    Align = alClient
    Caption = 'Panel1'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 0
    VerticalAlignment = taAlignTop
    object DBGrid1: TDBGrid
      Left = 1
      Top = 1
      Width = 390
      Height = 275
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
          FieldName = 'Componente'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ReadOnly = True
          Title.Alignment = taCenter
          Title.Font.Charset = DEFAULT_CHARSET
          Title.Font.Color = clBlue
          Title.Font.Height = -13
          Title.Font.Name = 'Tahoma'
          Title.Font.Style = [fsBold]
          Width = 85
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'Desccrizione breve'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ReadOnly = True
          Title.Alignment = taCenter
          Title.Caption = 'Descrizione'
          Title.Font.Charset = DEFAULT_CHARSET
          Title.Font.Color = clBlue
          Title.Font.Height = -13
          Title.Font.Name = 'Tahoma'
          Title.Font.Style = [fsBold]
          Width = 350
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
