object DMPlc: TDMPlc
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 357
  Width = 556
  object TimerEventi: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerEventiTimer
    Left = 32
    Top = 24
  end
  object TimerPlc: TTimer
    Enabled = False
    Interval = 400
    OnTimer = TimerPlcTimer
    Left = 48
    Top = 96
  end
  object TimerWatchDogPlc: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerWatchDogPlcTimer
    Left = 32
    Top = 224
  end
  object TimerCarScar: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerCarScarTimer
    Left = 40
    Top = 160
  end
end
