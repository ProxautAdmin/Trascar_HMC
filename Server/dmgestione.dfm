object DMGestioneEventi: TDMGestioneEventi
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 357
  Width = 556
  object TimerEventi: TTimer
    Enabled = False
    OnTimer = TimerEventiTimer
    Left = 32
    Top = 24
  end
  object TimerMissioni: TTimer
    OnTimer = TimerMissioniTimer
    Left = 136
    Top = 24
  end
  object TimerPuliziaStorici: TTimer
    Interval = 3600000
    OnTimer = TimerPuliziaStoriciTimer
    Left = 268
    Top = 24
  end
end
