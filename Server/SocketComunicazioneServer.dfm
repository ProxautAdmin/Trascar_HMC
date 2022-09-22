object SocketDataModule: TSocketDataModule
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  OnDestroy = DataModuleDestroy
  Height = 150
  Width = 215
  object ServerSocket1: TServerSocket
    Active = False
    Port = 10000
    ServerType = stNonBlocking
    OnClientRead = ServerSocket1ClientRead
    OnClientError = ServerSocket1ClientError
    Left = 24
    Top = 16
  end
  object TimerSendClientData: TTimer
    Interval = 500
    OnTimer = TimerSendClientDataTimer
    Left = 128
    Top = 16
  end
end
