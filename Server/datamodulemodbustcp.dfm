object DataModuleModBus: TDataModuleModBus
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 278
  Width = 391
  object TimerTx: TTimer
    Enabled = False
    OnTimer = TimerTxTimer
    Left = 120
    Top = 40
  end
  object IdModBusClient1: TIdModBusClient
    OnStatus = IdModBusClient1Status
    OnConnected = IdModBusClient1Connected
    ConnectTimeout = 3000
    IPVersion = Id_IPv4
    Port = 503
    TimeOut = 3000
    OnResponseError = IdModBusClient1ResponseError
    OnResponseMismatch = IdModBusClient1ResponseMismatch
    Left = 32
    Top = 40
  end
end
