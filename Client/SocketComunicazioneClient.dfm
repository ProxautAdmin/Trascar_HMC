object SocketDataModule: TSocketDataModule
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  OnDestroy = DataModuleDestroy
  Height = 194
  Width = 406
  object ClientSocket: TClientSocket
    Active = False
    Address = '127.0.0.1'
    ClientType = ctNonBlocking
    Port = 10007
    OnDisconnect = ClientSocketDisconnect
    OnRead = ClientSocketRead
    OnError = ClientSocketError
    Left = 24
    Top = 16
  end
  object TimerConnected: TTimer
    Enabled = False
    Interval = 2000
    OnTimer = TimerConnectedTimer
    Left = 152
    Top = 16
  end
  object TimerTimeout: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = TimerTimeoutTimer
    Left = 272
    Top = 16
  end
end
