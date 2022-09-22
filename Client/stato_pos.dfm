inherited fCorsia: TfCorsia
  BorderStyle = bsSingle
  Caption = 'Stato Corsia'
  ClientHeight = 693
  ClientWidth = 1747
  DoubleBuffered = True
  Font.Height = -13
  OnActivate = FormActivate
  OnCreate = FormCreate
  ExplicitWidth = 1753
  ExplicitHeight = 722
  PixelsPerInch = 96
  TextHeight = 16
  inherited PanelButtons: TPanel
    Left = 1639
    Height = 653
    ExplicitLeft = 1639
    ExplicitHeight = 653
    inherited PanelClose: TPanel
      Top = 567
      ExplicitTop = 567
      inherited BitBtnClose: TBitBtn
        Caption = 'Close'
      end
    end
  end
  inherited PanelTitle: TPanel
    Width = 1747
    ExplicitWidth = 1747
  end
  object MainPanel: TPanel
    Left = 0
    Top = 40
    Width = 1639
    Height = 653
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 2
    object PanelScaffale: TPanel
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 517
      Height = 645
      Align = alLeft
      BevelOuter = bvLowered
      Color = clInactiveBorder
      ParentBackground = False
      TabOrder = 0
    end
    object PanelData: TPanel
      Left = 974
      Top = 1
      Width = 664
      Height = 651
      Align = alRight
      Caption = '...'
      TabOrder = 1
      object PanelFila: TPanel
        Left = 1
        Top = 344
        Width = 662
        Height = 306
        Align = alBottom
        BevelOuter = bvLowered
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Visible = False
        object Label11: TLabel
          Left = 571
          Top = 39
          Width = 12
          Height = 19
          Caption = 'A'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label12: TLabel
          Left = 515
          Top = 39
          Width = 50
          Height = 19
          Caption = 'Area :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbl2: TLabel
          Left = 20
          Top = 198
          Width = 66
          Height = 16
          Caption = 'Annotazioni'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Panel9: TPanel
          Left = 1
          Top = 1
          Width = 660
          Height = 32
          Align = alTop
          Caption = 'Lane Data'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object Panel10: TPanel
          Left = 1
          Top = 244
          Width = 660
          Height = 61
          Align = alBottom
          TabOrder = 1
          object BitBtn1: TBitBtn
            AlignWithMargins = True
            Left = 559
            Top = 4
            Width = 97
            Height = 53
            Align = alRight
            Caption = 'Successiva'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            Layout = blGlyphRight
            ParentFont = False
            Spacing = 3
            TabOrder = 0
            WordWrap = True
            OnClick = BitBtn1Click
          end
          object BitBtn3: TBitBtn
            AlignWithMargins = True
            Left = 114
            Top = 4
            Width = 95
            Height = 53
            Align = alLeft
            Caption = 'Svuota Corsia'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Spacing = 3
            TabOrder = 1
            WordWrap = True
            OnClick = BitBtn3Click
          end
          object BitBtn4: TBitBtn
            AlignWithMargins = True
            Left = 408
            Top = 4
            Width = 145
            Height = 53
            Align = alRight
            Caption = 'Applica modifiche'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Spacing = 3
            TabOrder = 2
            WordWrap = True
            OnClick = BitBtn4Click
          end
          object BitBtn5: TBitBtn
            AlignWithMargins = True
            Left = 4
            Top = 4
            Width = 104
            Height = 53
            Align = alLeft
            Caption = 'Precedente'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Spacing = 3
            TabOrder = 3
            WordWrap = True
            OnClick = BitBtn5Click
          end
          object btDisabilitaPiani: TBitBtn
            AlignWithMargins = True
            Left = 215
            Top = 4
            Width = 140
            Height = 53
            Align = alLeft
            Caption = 'Disabilita/Abilita piani '
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Spacing = 3
            TabOrder = 4
            WordWrap = True
            OnClick = btDisabilitaPianiClick
          end
        end
        object GroupBox1: TGroupBox
          Left = 9
          Top = 64
          Width = 248
          Height = 97
          Caption = 'Status Fila'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object ckUsoPrel: TCheckBox
            Left = 124
            Top = 60
            Width = 222
            Height = 26
            Caption = 'Corsia uso Prel.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = ckAbilitaClick
          end
          object ckUsoDep: TCheckBox
            Left = 124
            Top = 38
            Width = 222
            Height = 25
            Caption = 'Corsia uso Dep.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = ckAbilitaClick
          end
          object ckAbilita: TCheckBox
            Left = 11
            Top = 15
            Width = 144
            Height = 25
            Caption = 'Corsia Abilitata'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            OnClick = ckAbilitaClick
          end
          object ckPrenotata: TCheckBox
            Left = 11
            Top = 38
            Width = 107
            Height = 25
            Caption = 'Corsia Riservata'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            OnClick = ckAbilitaClick
          end
          object ckPiena: TCheckBox
            Left = 11
            Top = 61
            Width = 94
            Height = 25
            Caption = 'Corsia Piena'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            OnClick = ckAbilitaClick
          end
          object ckVuota: TCheckBox
            Left = 124
            Top = 15
            Width = 111
            Height = 25
            Caption = 'Corsia Vuota'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            OnClick = ckAbilitaClick
          end
        end
        object GroupBox2: TGroupBox
          Left = 263
          Top = 64
          Width = 394
          Height = 97
          Caption = 'Settaggi'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object Label9: TLabel
            Left = 4
            Top = 22
            Width = 65
            Height = 16
            Caption = 'Tipo Corsia'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
          end
          object Label10: TLabel
            Left = 4
            Top = 62
            Width = 80
            Height = 16
            Caption = 'Fila prioritaria'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
          end
          object cbTipo: TComboBox
            Left = 94
            Top = 23
            Width = 187
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnChange = cbTipoChange
            Items.Strings = (
              '')
          end
          object cbPriorita: TComboBox
            Left = 94
            Top = 56
            Width = 89
            Height = 27
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Items.Strings = (
              '1'
              '2'
              '3'
              '4'
              '5'
              '6'
              '7'
              '8'
              '9'
              '10'
              '11'
              '12'
              '13'
              '14'
              '15'
              '16')
          end
          object leNumUDC: TLabeledEdit
            Left = 288
            Top = 59
            Width = 89
            Height = 24
            Alignment = taCenter
            EditLabel.Width = 115
            EditLabel.Height = 16
            EditLabel.Caption = 'N'#176' Full UDC in Lane '
            EditLabel.Font.Charset = DEFAULT_CHARSET
            EditLabel.Font.Color = clBlue
            EditLabel.Font.Height = -13
            EditLabel.Font.Name = 'Tahoma'
            EditLabel.Font.Style = []
            EditLabel.ParentFont = False
            LabelPosition = lpLeft
            ReadOnly = True
            TabOrder = 2
          end
        end
        object edtAnnotazioni: TEdit
          Left = 104
          Top = 198
          Width = 553
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          MaxLength = 99
          ParentFont = False
          TabOrder = 4
          OnClick = ckAbilitaClick
        end
        object ckIgnoraOrdine: TCheckBox
          Left = 20
          Top = 167
          Width = 222
          Height = 25
          Caption = 'Ignora posizione da Ordine'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          OnClick = ckAbilitaClick
        end
        object cbDepSelettivo: TCheckBox
          Left = 263
          Top = 167
          Width = 121
          Height = 25
          Caption = 'Deposito Selettivo'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
          OnClick = ckAbilitaClick
        end
      end
      object PanelPosizione: TPanel
        Left = 1
        Top = 1
        Width = 662
        Height = 317
        Align = alTop
        BevelOuter = bvLowered
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object Label1: TLabel
          Left = 489
          Top = 44
          Width = 69
          Height = 19
          Caption = 'Num.Pos.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 489
          Top = 69
          Width = 115
          Height = 19
          Caption = 'NumPosizione'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label3: TLabel
          Left = 601
          Top = 149
          Width = 77
          Height = 19
          Caption = 'Nome Pos.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label4: TLabel
          Left = 601
          Top = 174
          Width = 76
          Height = 19
          Caption = 'NomePos'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          Visible = False
        end
        object Label17: TLabel
          Left = 489
          Top = 94
          Width = 39
          Height = 19
          Caption = 'Piano'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbPiano: TLabel
          Left = 489
          Top = 119
          Width = 46
          Height = 19
          Caption = 'Piano'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbnomepiano: TLabel
          Left = 489
          Top = 230
          Width = 98
          Height = 19
          Caption = 'NomeCorsia'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label30: TLabel
          Left = 489
          Top = 205
          Width = 86
          Height = 19
          Caption = 'Nome Piano'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object lbl1: TLabel
          Left = 10
          Top = 49
          Width = 82
          Height = 16
          Caption = 'Tipo Posizione'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label13: TLabel
          Left = 43
          Top = 78
          Width = 34
          Height = 13
          Caption = 'Id UDC'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label14: TLabel
          Left = 41
          Top = 105
          Width = 51
          Height = 13
          Caption = 'Tab Carico'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label15: TLabel
          Left = 229
          Top = 105
          Width = 55
          Height = 13
          Caption = 'Tab Scarico'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label16: TLabel
          Left = 13
          Top = 132
          Width = 79
          Height = 13
          Caption = 'Lato Forche Agv'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label18: TLabel
          Left = 250
          Top = 132
          Width = 34
          Height = 13
          Caption = 'Tabella'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label7: TLabel
          Left = 601
          Top = 127
          Width = 97
          Height = 19
          Caption = 'Nome Corsia:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label8: TLabel
          Left = 601
          Top = 188
          Width = 131
          Height = 19
          Caption = 'Numero Corsia :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          Visible = False
        end
        object Label5: TLabel
          Left = 489
          Top = 149
          Width = 65
          Height = 19
          Caption = 'Corsia N.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label6: TLabel
          Left = 489
          Top = 174
          Width = 131
          Height = 19
          Caption = 'Numero Corsia :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Panel7: TPanel
          Left = 1
          Top = 1
          Width = 660
          Height = 32
          Align = alTop
          Caption = 'Dati Posizione'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object Panel8: TPanel
          Left = 1
          Top = 255
          Width = 660
          Height = 61
          Align = alBottom
          TabOrder = 1
          object BitBtnChange: TBitBtn
            AlignWithMargins = True
            Left = 632
            Top = 4
            Width = 24
            Height = 53
            Align = alRight
            Caption = 'Deposito Manuale'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Spacing = 3
            TabOrder = 0
            Visible = False
            WordWrap = True
            OnClick = BitBtnChangeClick
          end
          object BitBtn2: TBitBtn
            AlignWithMargins = True
            Left = 4
            Top = 4
            Width = 157
            Height = 53
            Align = alLeft
            Caption = 'Vuota Posizione'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Spacing = 3
            TabOrder = 1
            Visible = False
            WordWrap = True
            OnClick = BitBtn2Click
          end
          object BitBtn6: TBitBtn
            AlignWithMargins = True
            Left = 167
            Top = 4
            Width = 210
            Height = 53
            Align = alLeft
            Caption = 'Applica Modifiche su Posizione'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Spacing = 3
            TabOrder = 2
            Visible = False
            WordWrap = True
            OnClick = BitBtn6Click
          end
          object BitBtn7: TBitBtn
            AlignWithMargins = True
            Left = 600
            Top = 4
            Width = 26
            Height = 53
            Align = alRight
            Caption = 'Prelievo Manuale'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Spacing = 3
            TabOrder = 3
            Visible = False
            WordWrap = True
            OnClick = BitBtn7Click
          end
          object btnModificaAltezza: TBitBtn
            AlignWithMargins = True
            Left = 383
            Top = 4
            Width = 210
            Height = 53
            Align = alLeft
            Caption = 'Modifica altezza'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            Spacing = 3
            TabOrder = 4
            Visible = False
            WordWrap = True
            OnClick = btnModificaAltezzaClick
          end
        end
        object GroupBox3: TGroupBox
          Left = 295
          Top = 171
          Width = 178
          Height = 78
          Caption = 'Stato Posizione'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Visible = False
          object ckDisabilitaPos: TCheckBox
            Left = 7
            Top = 17
            Width = 134
            Height = 25
            Caption = 'Disabilita Posizione'
            TabOrder = 0
            OnClick = ckDisabilitaPosClick
          end
          object cPrenotataPos: TCheckBox
            Left = 7
            Top = 38
            Width = 162
            Height = 25
            Caption = 'Posizione Riservata'
            TabOrder = 1
            OnClick = cPrenotataPosClick
          end
        end
        object GroupBox8: TGroupBox
          Left = 7
          Top = 171
          Width = 274
          Height = 78
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object Label25: TLabel
            Left = 14
            Top = 3
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
            Left = 171
            Top = 3
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
            Top = 3
            Width = 58
            Height = 21
            Alignment = taRightJustify
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object eHprel: TEdit
            Left = 213
            Top = 3
            Width = 58
            Height = 21
            Alignment = taRightJustify
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object ckDisabilitaPiano: TCheckBox
            Left = 3
            Top = 30
            Width = 134
            Height = 25
            Caption = 'Disabilita Piano'
            TabOrder = 2
            Visible = False
            OnClick = ckDisabilitaPosClick
          end
          object ckPianoRiservato: TCheckBox
            Left = 143
            Top = 30
            Width = 222
            Height = 25
            Caption = 'Piano Riservato'
            TabOrder = 3
            Visible = False
            OnClick = cPrenotataPosClick
          end
        end
        object cbbTipoPosizione: TComboBox
          Left = 98
          Top = 46
          Width = 186
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Visible = False
          OnChange = cbTipoChange
          Items.Strings = (
            '')
        end
        object edtRifOrdine: TEdit
          Left = 290
          Top = 48
          Width = 176
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          Visible = False
          OnChange = cbTipoChange
        end
        object eUDC: TEdit
          Left = 98
          Top = 76
          Width = 87
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 6
          Visible = False
        end
        object Edit1: TEdit
          Left = 98
          Top = 103
          Width = 87
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 7
          Visible = False
        end
        object Edit2: TEdit
          Left = 290
          Top = 103
          Width = 87
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 8
          Visible = False
        end
        object Edit3: TEdit
          Left = 98
          Top = 130
          Width = 87
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 9
        end
        object editTabella: TEdit
          Left = 290
          Top = 130
          Width = 87
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 10
        end
      end
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 770
    Top = 144
  end
end
