#|
$JSON
{"authURL":[],"YaVersion":"223","Source":"Form","Properties":{"$Name":"ControlScreen","$Type":"Form","$Version":"27","AppName":"MEPER","BackgroundColor":"&HFF1B3C6E","ScreenOrientation":"portrait","Sizing":"Responsive","Theme":"DeviceDefault","Title":"Controle","TitleVisible":"False","$Components":[
{"$Name":"HorizontalArrangement1","$Type":"HorizontalArrangement","$Version":"4","AlignVertical":"2","BackgroundColor":"&HFF112240","Height":"50","Width":"-2","$Components":[
{"$Name":"BtnVoltar","$Type":"Button","$Version":"7","BackgroundColor":"&HFF334466","FontBold":"True","FontSize":"12","Height":"36","Text":"< MENU","TextColor":"&HFFFFFFFF","Width":"80"},
{"$Name":"ListPickerBT","$Type":"ListPicker","$Version":"9","BackgroundColor":"&HFFE8692A","FontBold":"True","FontSize":"14","Height":"40","Text":"Conectar","TextColor":"&HFFFFFFFF","Width":"120"},
{"$Name":"LabelStatus","$Type":"Label","$Version":"5","FontSize":"12","Text":"Desconectado","TextColor":"&HFFFF4444","Width":"-2"}
]},
{"$Name":"LabelTitulo","$Type":"Label","$Version":"5","FontBold":"True","FontSize":"18","Text":"MEPER","TextColor":"&HFFE8692A","Width":"-2","TextAlignment":"1"},
{"$Name":"LabelInstrucaoBT","$Type":"Label","$Version":"5","FontSize":"11","Text":"Toque em Conectar e selecione o dispositivo","TextColor":"&H99FFFFFF","Width":"-2","TextAlignment":"1"},
{"$Name":"LabelDirecao","$Type":"Label","$Version":"5","FontBold":"True","FontSize":"20","Height":"30","Text":"--","TextColor":"&HFFFFFFFF","Width":"-2","TextAlignment":"1"},
{"$Name":"CanvasJoystick","$Type":"Canvas","$Version":"14","BackgroundColor":"&HFF152D4A","Height":"230","Width":"230","$Components":[
{"$Name":"BallKnob","$Type":"Ball","$Version":"7","Radius":"37","X":"115","Y":"115","PaintColor":"&HFFE8692A","Enabled":"True"}
]},
{"$Name":"VerticalArrangementSliders","$Type":"VerticalArrangement","$Version":"4","AlignHorizontal":"3","Height":"-1","Width":"260","$Components":[
{"$Name":"LabelSpeed","$Type":"Label","$Version":"5","FontBold":"True","FontSize":"12","Text":"VELOCIDADE: 150","TextColor":"&HFFFFFFFF","Width":"-2"},
{"$Name":"SliderSpeed","$Type":"Slider","$Version":"2","ColorLeft":"&HFFE8692A","ColorRight":"&HFF334466","MaxValue":"255","MinValue":"0","ThumbPosition":"150","Width":"250"},
{"$Name":"LabelTrimOrTurn","$Type":"Label","$Version":"5","FontBold":"True","FontSize":"12","Text":"TRIM: 0","TextColor":"&HFFFFFFFF","Width":"-2"},
{"$Name":"SliderTrimOrTurn","$Type":"Slider","$Version":"2","ColorLeft":"&HFFE8692A","ColorRight":"&HFF334466","MaxValue":"50","MinValue":"-50","ThumbPosition":"0","Width":"250"}
]},
{"$Name":"LabelComando","$Type":"Label","$Version":"5","FontSize":"11","Text":"Aguardando comando...","TextColor":"&H99FFFFFF","Width":"-2","TextAlignment":"1"},
{"$Name":"BluetoothClient1","$Type":"BluetoothClient","$Version":"7"},
{"$Name":"Clock1","$Type":"Clock","$Version":"4","TimerInterval":"100","TimerEnabled":"True"},
{"$Name":"TinyDB1","$Type":"TinyDB","$Version":"2"}
]}}
|#
