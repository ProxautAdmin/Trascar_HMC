object dmExtraFunction: TdmExtraFunction
  OldCreateOrder = False
  Height = 232
  Width = 350
  object IdSSLIOHandlerSocketOpenSSL1: TIdSSLIOHandlerSocketOpenSSL
    Destination = ':25'
    MaxLineAction = maException
    Port = 25
    DefaultPort = 0
    SSLOptions.Mode = sslmUnassigned
    SSLOptions.VerifyMode = []
    SSLOptions.VerifyDepth = 0
    Left = 192
    Top = 24
  end
  object IdSMTP1: TIdSMTP
    IOHandler = IdSSLIOHandlerSocketOpenSSL1
    SASLMechanisms = <>
    UseTLS = utUseRequireTLS
    Left = 184
    Top = 88
  end
  object IdMessage1: TIdMessage
    AttachmentEncoding = 'UUE'
    BccList = <>
    CCList = <>
    Encoding = meDefault
    FromList = <
      item
      end>
    Recipients = <>
    ReplyTo = <>
    ConvertPreamble = True
    Left = 224
    Top = 160
  end
end
