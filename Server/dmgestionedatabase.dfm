object dmDBServer: TdmDBServer
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 245
  Width = 377
  object TimerMissioniDB: TTimer
    OnTimer = TimerMissioniDBTimer
    Left = 120
    Top = 88
  end
  object TimerAggiornamentiMinuti: TTimer
    OnTimer = TimerAggiornamentiMinutiTimer
    Left = 244
    Top = 48
  end
end
