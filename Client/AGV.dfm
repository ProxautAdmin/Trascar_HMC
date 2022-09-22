inherited fAGV: TfAGV
  Tag = 1
  Left = 274
  Top = 77
  Caption = 'Stato Agv'
  ClientHeight = 940
  ClientWidth = 1266
  Position = poDesigned
  OnActivate = FormActivate
  OnKeyDown = FormKeyDown
  ExplicitWidth = 1282
  ExplicitHeight = 979
  PixelsPerInch = 96
  TextHeight = 19
  inherited PanelButtons: TPanel
    Left = 1158
    Height = 900
    ExplicitLeft = 1158
    ExplicitHeight = 900
    inherited PanelClose: TPanel
      Top = 814
      ExplicitTop = 814
    end
  end
  inherited PanelTitle: TPanel
    Width = 1266
    ExplicitWidth = 1266
  end
  object ScrollBox1: TScrollBox
    Left = 0
    Top = 40
    Width = 1158
    Height = 900
    VertScrollBar.Range = 2000
    Align = alClient
    AutoScroll = False
    TabOrder = 2
    object Panel4: TPanel
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 1131
      Height = 1994
      Align = alClient
      BorderWidth = 3
      TabOrder = 0
      object Panel1: TPanel
        Left = 4
        Top = 4
        Width = 1123
        Height = 1986
        Align = alClient
        BorderWidth = 3
        TabOrder = 0
        inline frAgvL1: TfrAgvL
          Left = 8
          Top = 8
          Width = 1156
          Height = 188
          TabOrder = 0
          ExplicitLeft = 8
          ExplicitTop = 8
          inherited Panel1: TPanel
            inherited lbTipoCarica: TLabel
              Top = 141
              Width = 26
              AutoSize = False
              ExplicitTop = 141
              ExplicitWidth = 26
            end
            inherited leDestinazione: TLabeledEdit [3]
            end
            inherited GroupBox4: TGroupBox [4]
            end
            inherited leMissione: TLabeledEdit
              Left = 341
              EditLabel.ExplicitLeft = 263
              ExplicitLeft = 341
            end
            inherited lePosizione: TLabeledEdit [6]
            end
            inherited GroupBox1: TGroupBox [7]
            end
            inherited leLivBatt: TLabeledEdit [8]
            end
            inherited pInMis: TPanel [9]
            end
          end
        end
      end
    end
  end
  object TimerUpd: TTimer
    Enabled = False
    OnTimer = TimerUpdTimer
    Left = 144
    Top = 8
  end
end
