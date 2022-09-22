object dmDB: TdmDB
  OldCreateOrder = False
  Height = 404
  Width = 506
  object ADOConnection1: TADOConnection
    CommandTimeout = 20
    ConnectionString = 
      'Provider=SQLOLEDB.1;Password=pwd;Persist Security Info=True;User' +
      ' ID=sa;Initial Catalog=Bonfanti;Data Source=NB-PANCALDI-L\SQLEXP' +
      'RESS2017'
    ConnectionTimeout = 20
    ConnectOptions = coAsyncConnect
    LoginPrompt = False
    Provider = 'SQLOLEDB.1'
    AfterConnect = ADOConnection1AfterConnect
    OnConnectComplete = ADOConnection1ConnectComplete
    Left = 40
    Top = 16
  end
  object TimerConnect: TTimer
    Enabled = False
    OnTimer = TimerConnectTimer
    Left = 40
    Top = 88
  end
  object QueryMissioniSocket: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 144
    Top = 16
  end
  object AlarmTable: TADOTable
    Connection = ADOConnection1
    Left = 120
    Top = 80
  end
  object MissTable: TADOTable
    Connection = ADOConnection1
    Left = 96
    Top = 152
  end
  object QueryNewAlarm: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 224
    Top = 16
  end
  object QueryPos: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 192
    Top = 87
  end
  object QueryMappa: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 280
    Top = 87
  end
  object QueryCorsie: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 152
    Top = 159
  end
  object TimerPassword: TTimer
    Enabled = False
    Interval = 120000
    OnTimer = TimerPasswordTimer
    Left = 36
    Top = 192
  end
  object QueryUDC: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 216
    Top = 159
  end
  object QueryMessAlarm: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 224
    Top = 280
  end
  object QueryPeso: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 224
    Top = 224
  end
  object QueryTipologie: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 136
    Top = 264
  end
  object ADOConnectionExtra: TADOConnection
    CommandTimeout = 5
    ConnectionString = 
      'Provider=MSDASQL.1;Password=PCS;Persist Security Info=True;User ' +
      'ID=PCS;Data Source=ICS80DAT'
    ConnectionTimeout = 3
    ConnectOptions = coAsyncConnect
    LoginPrompt = False
    Provider = 'MSDASQL.1'
    AfterConnect = ADOConnection1AfterConnect
    OnConnectComplete = ADOConnection1ConnectComplete
    Left = 32
    Top = 288
  end
  object QueryCentroMissioniSocket: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 304
    Top = 16
  end
  object QueryTabelle: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 136
    Top = 312
  end
end
