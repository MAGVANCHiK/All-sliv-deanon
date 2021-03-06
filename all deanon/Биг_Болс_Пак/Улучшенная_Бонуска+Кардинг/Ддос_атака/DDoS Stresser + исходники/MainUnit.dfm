object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Fastream Web Stress Tester 4.0'
  ClientHeight = 355
  ClientWidth = 298
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 153
    Top = 39
    Width = 58
    Height = 13
    Caption = 'Connection:'
  end
  object Label2: TLabel
    Left = 170
    Top = 65
    Width = 40
    Height = 13
    Caption = 'Method:'
  end
  object RunButton: TButton
    Left = 8
    Top = 158
    Width = 282
    Height = 25
    Caption = 'Run!'
    TabOrder = 8
    OnClick = RunButtonClick
  end
  object ThreadCountLabeledEdit: TLabeledEdit
    Left = 8
    Top = 32
    Width = 49
    Height = 21
    EditLabel.Width = 39
    EditLabel.Height = 13
    EditLabel.Caption = 'Threads'
    LabelPosition = lpRight
    TabOrder = 1
    Text = '1'
    OnKeyPress = ThreadCountLabeledEditKeyPress
  end
  object ClientsPerThreadLabeledEdit: TLabeledEdit
    Left = 8
    Top = 59
    Width = 49
    Height = 21
    EditLabel.Width = 88
    EditLabel.Height = 13
    EditLabel.Caption = 'Clients per Thread'
    LabelPosition = lpRight
    TabOrder = 2
    Text = '1'
    OnKeyPress = ClientsPerThreadLabeledEditKeyPress
  end
  object URLLabeledEdit: TLabeledEdit
    Left = 56
    Top = 5
    Width = 234
    Height = 21
    EditLabel.Width = 19
    EditLabel.Height = 13
    EditLabel.Caption = 'URL'
    LabelPosition = lpLeft
    TabOrder = 0
    Text = 'https://localhost'
  end
  object MethodComboBox: TComboBox
    Left = 216
    Top = 62
    Width = 55
    Height = 21
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 4
    Text = 'GET'
    Items.Strings = (
      'GET'
      'HEAD'
      'POST')
  end
  object KeepAliveComboBox: TComboBox
    Left = 216
    Top = 35
    Width = 74
    Height = 21
    Style = csDropDownList
    ItemIndex = 1
    TabOrder = 3
    Text = 'keep-alive'
    Items.Strings = (
      'close'
      'keep-alive')
  end
  object LabeledEdit2xx: TLabeledEdit
    Left = 42
    Top = 189
    Width = 79
    Height = 21
    Color = clBtnFace
    EditLabel.Width = 22
    EditLabel.Height = 13
    EditLabel.Caption = '2xx:'
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 9
  end
  object LabeledEditDropped: TLabeledEdit
    Left = 196
    Top = 189
    Width = 86
    Height = 21
    Color = clBtnFace
    EditLabel.Width = 45
    EditLabel.Height = 13
    EditLabel.Caption = 'Dropped:'
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 13
  end
  object LabeledEdit3xx: TLabeledEdit
    Left = 42
    Top = 216
    Width = 79
    Height = 21
    Color = clBtnFace
    EditLabel.Width = 22
    EditLabel.Height = 13
    EditLabel.Caption = '3xx:'
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 10
  end
  object LabeledEdit4xx: TLabeledEdit
    Left = 42
    Top = 242
    Width = 79
    Height = 21
    Color = clBtnFace
    EditLabel.Width = 22
    EditLabel.Height = 13
    EditLabel.Caption = '4xx:'
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 11
  end
  object LabeledEdit5xx: TLabeledEdit
    Left = 42
    Top = 269
    Width = 79
    Height = 21
    Color = clBtnFace
    EditLabel.Width = 22
    EditLabel.Height = 13
    EditLabel.Caption = '5xx:'
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 12
  end
  object LabeledEditHitsPerSec: TLabeledEdit
    Left = 196
    Top = 216
    Width = 86
    Height = 21
    Color = clBtnFace
    EditLabel.Width = 42
    EditLabel.Height = 13
    EditLabel.Caption = 'Hits/sec:'
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 14
  end
  object LabeledEditElapsedTime: TLabeledEdit
    Left = 196
    Top = 296
    Width = 86
    Height = 21
    Color = clBtnFace
    EditLabel.Width = 66
    EditLabel.Height = 13
    EditLabel.Caption = 'Elapsed Time:'
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 17
  end
  object LabeledEditTotalKB: TLabeledEdit
    Left = 196
    Top = 269
    Width = 86
    Height = 21
    Color = clBtnFace
    EditLabel.Width = 43
    EditLabel.Height = 13
    EditLabel.Caption = 'Total KB:'
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 16
  end
  object LabeledEditKBPerSec: TLabeledEdit
    Left = 196
    Top = 242
    Width = 86
    Height = 21
    Color = clBtnFace
    EditLabel.Width = 36
    EditLabel.Height = 13
    EditLabel.Caption = 'KB/sec:'
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 15
  end
  object Memo: TMemo
    Left = 8
    Top = 120
    Width = 282
    Height = 36
    Lines.Strings = (
      'Testing web server with this memo as the post data.')
    TabOrder = 7
  end
  object ProxyIPLabeledEdit: TLabeledEdit
    Left = 56
    Top = 93
    Width = 113
    Height = 21
    EditLabel.Width = 45
    EditLabel.Height = 13
    EditLabel.Caption = 'Proxy IP:'
    LabelPosition = lpLeft
    TabOrder = 5
    Text = 'localhost'
  end
  object ProxyPortLabeledEdit: TLabeledEdit
    Left = 240
    Top = 93
    Width = 50
    Height = 21
    EditLabel.Width = 55
    EditLabel.Height = 13
    EditLabel.Caption = 'Proxy Port:'
    LabelPosition = lpLeft
    TabOrder = 6
    Text = '8080'
  end
  object Edit1: TEdit
    Left = 8
    Top = 329
    Width = 282
    Height = 21
    Alignment = taCenter
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 18
    Text = 'All Rights Reserved 2009-2011 Fastream Technologies'
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 112
    Top = 32
  end
end
