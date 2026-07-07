#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

//--------------------------------------------- (PINS) ------------
//--------------- (L298n R)
#define RENA 25
#define RIN1 26
#define RIN2 27
#define RIN3 32
#define RIN4 33
#define RENB 14

//--------------- (L298n L)
#define LENA 12
#define LIN1 13
#define LIN2 15
#define LIN3 4
#define LIN4 16
#define LENB 17

//--------------------------------------------- (VARIABLES) --------
int speed = 150;
int trim = 0;  // -50 a +50: positivo = compensa lado direito (reduz motor R)
WebServer server(80);
DNSServer dnsServer;

// Calcula velocidades com trim aplicado
int speedR() { return constrain(speed - trim, 0, 255); }
int speedL() { return constrain(speed + trim, 0, 255); }

//--------------------------------------------- (MOTOR FUNCTIONS) ---
void forward() {
  digitalWrite(RIN2, HIGH); digitalWrite(RIN1, LOW);
  digitalWrite(RIN3, HIGH); digitalWrite(RIN4, LOW);
  analogWrite(RENA, speedR()); analogWrite(RENB, speedR());
  digitalWrite(LIN1, HIGH); digitalWrite(LIN2, LOW);
  digitalWrite(LIN4, HIGH); digitalWrite(LIN3, LOW);
  analogWrite(LENA, speedL()); analogWrite(LENB, speedL());
}

void backward() {
  digitalWrite(RIN2, LOW); digitalWrite(RIN1, HIGH);
  digitalWrite(RIN3, LOW); digitalWrite(RIN4, HIGH);
  analogWrite(RENA, speedR()); analogWrite(RENB, speedR());
  digitalWrite(LIN1, LOW); digitalWrite(LIN2, HIGH);
  digitalWrite(LIN4, LOW); digitalWrite(LIN3, HIGH);
  analogWrite(LENA, speedL()); analogWrite(LENB, speedL());
}

void right() {
  digitalWrite(RIN2, LOW); digitalWrite(RIN1, HIGH);
  digitalWrite(RIN3, LOW); digitalWrite(RIN4, HIGH);
  analogWrite(RENA, speedR()); analogWrite(RENB, speedR());
  digitalWrite(LIN1, HIGH); digitalWrite(LIN2, LOW);
  digitalWrite(LIN4, HIGH); digitalWrite(LIN3, LOW);
  analogWrite(LENA, speedL()); analogWrite(LENB, speedL());
}

void left() {
  digitalWrite(RIN2, HIGH); digitalWrite(RIN1, LOW);
  digitalWrite(RIN3, HIGH); digitalWrite(RIN4, LOW);
  analogWrite(RENA, speedR()); analogWrite(RENB, speedR());
  digitalWrite(LIN1, LOW); digitalWrite(LIN2, HIGH);
  digitalWrite(LIN4, LOW); digitalWrite(LIN3, HIGH);
  analogWrite(LENA, speedL()); analogWrite(LENB, speedL());
}

void stopMotors() {
  digitalWrite(RIN2, LOW); digitalWrite(RIN1, LOW);
  digitalWrite(RIN3, LOW); digitalWrite(RIN4, LOW);
  analogWrite(RENA, 0); analogWrite(RENB, 0);
  digitalWrite(LIN1, LOW); digitalWrite(LIN2, LOW);
  digitalWrite(LIN4, LOW); digitalWrite(LIN3, LOW);
  analogWrite(LENA, 0); analogWrite(LENB, 0);
}

//--------------------------------------------- (LOGO BASE64) ------
const char LOGO_B64[] PROGMEM = "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";

//--------------------------------------------- (HTML PAGE) --------
const char PAGE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html data-theme="light">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
  <meta charset="UTF-8">
  <title>MEPER - Vector V1</title>
  <style>
    :root{--blue-dark:#1B3C6E;--blue:#1B5C9C;--blue-light:#00A4CC;--orange:#E8692A;--orange-light:#F28C28}
    [data-theme="light"]{--bg:#f5f7fa;--bg-card:#fff;--header-bg:#1B3C6E;--text:#1B3C6E;--text-sec:#5a6a7a;--border:#d0dbe6;--joy-base:radial-gradient(circle,#edf1f7,#dce3ed);--joy-knob:radial-gradient(circle at 35% 35%,var(--orange-light),var(--orange) 70%);--joy-knob-shadow:rgba(232,105,42,0.4);--joy-arrow:rgba(27,60,110,0.25);--joy-dash:rgba(27,60,110,0.12);--slider-track:#e0e7ef;--status-bg:#fff;--log-color:var(--blue);--shadow:0 2px 10px rgba(0,0,0,0.06);--hero-bg:#fff}
    [data-theme="dark"]{--bg:#0b1929;--bg-card:#112240;--header-bg:#0a1525;--text:#e5e5e5;--text-sec:#7ba7cc;--border:#1e3a5f;--joy-base:radial-gradient(circle,#152d4a,#0b1929);--joy-knob:radial-gradient(circle at 35% 35%,var(--orange-light),var(--orange) 70%);--joy-knob-shadow:rgba(232,105,42,0.5);--joy-arrow:rgba(0,164,204,0.35);--joy-dash:rgba(0,164,204,0.15);--slider-track:#152d4a;--status-bg:#0d1b30;--log-color:var(--blue-light);--shadow:0 2px 10px rgba(0,0,0,0.3);--hero-bg:#0d1f3c}
    *{margin:0;padding:0;box-sizing:border-box}
    body{background:var(--bg);text-align:center;font-family:Arial,Helvetica,sans-serif;color:var(--text);min-height:100vh;touch-action:none;transition:background .3s,color .3s}
    .header{background:var(--header-bg);padding:8px 16px;display:flex;align-items:center;justify-content:space-between;box-shadow:0 2px 10px rgba(0,0,0,.25)}
    .header-left{display:flex;align-items:center;gap:10px}
    .header img{height:40px;border-radius:6px}
    .header-brand h1{font-size:16px;font-weight:900;color:#fff;letter-spacing:3px;line-height:1.1}
    .header-brand .tagline{font-size:8px;font-weight:400;color:rgba(255,255,255,.6);letter-spacing:1px}
    .theme-toggle{background:rgba(255,255,255,.12);border:1px solid rgba(255,255,255,.2);border-radius:50%;width:34px;height:34px;cursor:pointer;font-size:16px;line-height:34px;text-align:center;transition:background .3s}
    .theme-toggle:active{transform:scale(.9)}
    .hero{padding:12px 16px 10px;background:var(--hero-bg);border-bottom:2px solid var(--orange);transition:background .3s}
    .vehicle-name{font-size:24px;font-weight:900;color:var(--orange);letter-spacing:4px}
    .slogan{font-size:11px;font-weight:400;color:var(--text-sec);margin-top:2px}
    #directionLabel{font-size:18px;font-weight:700;color:var(--blue);height:24px;letter-spacing:2px;margin:12px 0 6px;transition:color .3s}
    [data-theme="dark"] #directionLabel{color:var(--blue-light)}
    #joystickArea{position:relative;width:230px;height:230px;margin:0 auto}
    #joystickBase{position:absolute;width:230px;height:230px;border-radius:50%;background:var(--joy-base);border:3px solid var(--blue);box-shadow:var(--shadow);transition:background .3s,border-color .3s}
    [data-theme="dark"] #joystickBase{border-color:var(--blue-light)}
    #joystickBase::before{content:'';position:absolute;top:50%;left:50%;width:175px;height:175px;transform:translate(-50%,-50%);border-radius:50%;border:1px dashed var(--joy-dash)}
    .joy-arrow{position:absolute;color:var(--joy-arrow);font-size:14px}
    .joy-arrow.up{top:15px;left:50%;transform:translateX(-50%)}
    .joy-arrow.down{bottom:15px;left:50%;transform:translateX(-50%)}
    .joy-arrow.left{left:15px;top:50%;transform:translateY(-50%)}
    .joy-arrow.right{right:15px;top:50%;transform:translateY(-50%)}
    #joystickKnob{position:absolute;width:75px;height:75px;border-radius:50%;background:var(--joy-knob);box-shadow:0 0 18px var(--joy-knob-shadow),inset 0 -2px 4px rgba(0,0,0,.15);left:77.5px;top:77.5px}
    .speed-section{margin:14px auto;width:260px;background:var(--bg-card);border:1px solid var(--border);border-radius:12px;padding:12px 16px;box-shadow:var(--shadow);transition:background .3s}
    .speed-section label{font-size:11px;font-weight:700;color:var(--text-sec);display:flex;justify-content:space-between;align-items:center;letter-spacing:1px}
    .speed-section label span{color:#fff;font-size:15px;font-weight:700;background:var(--orange);padding:2px 14px;border-radius:6px}
    .speed-slider{width:100%;height:24px;margin-top:8px;-webkit-appearance:none;background:var(--slider-track);border-radius:12px;outline:none;border:1px solid var(--border);transition:background .3s}
    .speed-slider::-webkit-slider-thumb{-webkit-appearance:none;width:26px;height:26px;border-radius:50%;background:var(--joy-knob);box-shadow:0 0 8px var(--joy-knob-shadow);cursor:pointer}
    .status-bar{margin:0 auto;padding:7px 16px;background:var(--status-bg);border:1px solid var(--border);border-radius:8px;width:260px;display:flex;align-items:center;justify-content:center;gap:8px;box-shadow:var(--shadow);transition:background .3s}
    .status-dot{width:8px;height:8px;border-radius:50%;background:var(--orange);box-shadow:0 0 6px var(--orange);animation:pulse 2s infinite}
    @keyframes pulse{0%,100%{opacity:1}50%{opacity:.3}}
    #log{color:var(--log-color);font-size:12px}
    .footer{margin-top:10px;padding:8px;font-size:9px;color:var(--text-sec);letter-spacing:.5px;line-height:1.6}
    .footer .desc{font-size:8px;opacity:.7}
  </style>
</head>
<body>
  <div class="header">
    <div class="header-left">
      <img src="/logo" alt="MEPER">
      <div class="header-brand">
        <h1>MEPER</h1>
        <div class="tagline">Moldes de Encaixe para Eletr&#244;nicos Rob&#243;ticos</div>
      </div>
    </div>
    <button class="theme-toggle" id="themeBtn" onclick="toggleTheme()">&#9789;</button>
  </div>
  <div class="hero">
    <div class="vehicle-name">VECTOR V1</div>
    <div class="slogan">Tecnologia que voc&#234; encaixa</div>
  </div>
  <div id="directionLabel">--</div>
  <div id="joystickArea">
    <div id="joystickBase">
      <div class="joy-arrow up">&#9650;</div>
      <div class="joy-arrow down">&#9660;</div>
      <div class="joy-arrow left">&#9664;</div>
      <div class="joy-arrow right">&#9654;</div>
    </div>
    <div id="joystickKnob"></div>
  </div>
  <div class="speed-section">
    <label>VELOCIDADE <span id="speedValue">150</span></label>
    <input type="range" min="0" max="255" value="150" class="speed-slider" id="speedSlider" oninput="updateSpeed(this.value)">
  </div>
  <div class="speed-section">
    <label>TRIM (E &#8592; &#8594; D) <span id="trimValue">0</span></label>
    <input type="range" min="-50" max="50" value="0" class="speed-slider" id="trimSlider" oninput="updateTrim(this.value)">
  </div>
  <div class="status-bar">
    <div class="status-dot"></div>
    <div id="log">Aguardando comando...</div>
  </div>
  <div class="footer">
    Construa rob&#244;s reais. Controle. Compita.<br>
    <span class="desc">MEPER &#8212; Salvador-BA</span>
  </div>
  <script>
    var themeBtn=document.getElementById('themeBtn');
    function setTheme(t){document.documentElement.setAttribute('data-theme',t);themeBtn.innerHTML=t==='light'?'&#9789;':'&#9788;';localStorage.setItem('meper-theme',t)}
    function toggleTheme(){var c=document.documentElement.getAttribute('data-theme');setTheme(c==='light'?'dark':'light')}
    var saved=localStorage.getItem('meper-theme');if(saved)setTheme(saved);

    var knob=document.getElementById('joystickKnob'),base=document.getElementById('joystickBase'),area=document.getElementById('joystickArea'),dirLabel=document.getElementById('directionLabel'),logEl=document.getElementById('log');
    var baseRadius=115,knobRadius=37.5,maxDist=baseRadius-knobRadius,currentCmd='stop';
    function log(m){logEl.textContent=m}
    function sendCmd(cmd){
      if(cmd===currentCmd)return;currentCmd=cmd;
      var labels={forward:'FRENTE',backward:'RE',left:'ESQUERDA',right:'DIREITA',stop:'--'};
      dirLabel.textContent=labels[cmd]||cmd;
      log("Comando: "+cmd);
      var x=new XMLHttpRequest();x.open("GET","/"+cmd,true);x.send();
    }
    function updateSpeed(v){
      document.getElementById('speedValue').textContent=v;
      log("Velocidade: "+v);
      var x=new XMLHttpRequest();x.open("GET","/speed?value="+v,true);x.send();
    }
    function updateTrim(v){
      document.getElementById('trimValue').textContent=v;
      log("Trim: "+v);
      var x=new XMLHttpRequest();x.open("GET","/trim?value="+v,true);x.send();
    }
    function getDir(dx,dy){
      var d=Math.sqrt(dx*dx+dy*dy);if(d<20)return'stop';
      var a=Math.atan2(-dy,dx)*180/Math.PI;
      if(a>45&&a<=135)return'forward';if(a>-135&&a<=-45)return'backward';
      if(a>-45&&a<=45)return'right';return'left';
    }
    function moveKnob(cx,cy){
      var r=base.getBoundingClientRect(),dx=cx-(r.left+baseRadius),dy=cy-(r.top+baseRadius),d=Math.sqrt(dx*dx+dy*dy);
      if(d>maxDist){dx=dx/d*maxDist;dy=dy/d*maxDist}
      knob.style.left=(baseRadius-knobRadius+dx)+'px';knob.style.top=(baseRadius-knobRadius+dy)+'px';
      sendCmd(getDir(dx,dy));
    }
    function resetKnob(){knob.style.left=(baseRadius-knobRadius)+'px';knob.style.top=(baseRadius-knobRadius)+'px';sendCmd('stop')}
    area.addEventListener('touchstart',function(e){e.preventDefault();moveKnob(e.touches[0].clientX,e.touches[0].clientY)});
    area.addEventListener('touchmove',function(e){e.preventDefault();moveKnob(e.touches[0].clientX,e.touches[0].clientY)});
    area.addEventListener('touchend',function(e){e.preventDefault();resetKnob()});
    var md=false;
    area.addEventListener('mousedown',function(e){md=true;moveKnob(e.clientX,e.clientY)});
    document.addEventListener('mousemove',function(e){if(md)moveKnob(e.clientX,e.clientY)});
    document.addEventListener('mouseup',function(){if(md){md=false;resetKnob()}});
  </script>
</body>
</html>
)rawliteral";

//--------------------------------------------- (WEB HANDLERS) -----
void handleRoot() {
  server.send(200, "text/html", PAGE_HTML);
}

void handleLogo() {
  // Decode base64 and send as JPEG
  String b64 = FPSTR(LOGO_B64);
  int len = b64.length();
  int decodedLen = len * 3 / 4;
  uint8_t* decoded = (uint8_t*)malloc(decodedLen);
  if (!decoded) { server.send(500, "text/plain", "OOM"); return; }

  // Base64 decode
  const char* b64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int j = 0;
  for (int i = 0; i < len; i += 4) {
    uint32_t n = 0;
    for (int k = 0; k < 4; k++) {
      n <<= 6;
      char c = b64[i + k];
      if (c == '=') n += 0;
      else { const char* p = strchr(b64chars, c); if (p) n += (p - b64chars); }
    }
    if (j < decodedLen) decoded[j++] = (n >> 16) & 0xFF;
    if (j < decodedLen) decoded[j++] = (n >> 8) & 0xFF;
    if (j < decodedLen) decoded[j++] = n & 0xFF;
  }
  // Adjust for padding
  if (b64[len-1] == '=') j--;
  if (b64[len-2] == '=') j--;

  server.sendHeader("Cache-Control", "max-age=86400");
  server.send_P(200, "image/jpeg", (const char*)decoded, j);
  free(decoded);
}

void handleSpeed() {
  if (server.hasArg("value")) {
    speed = server.arg("value").toInt();
    Serial.print("Speed: "); Serial.println(speed);
  }
  server.send(200, "text/plain", "OK");
}

void handleTrim() {
  if (server.hasArg("value")) {
    trim = server.arg("value").toInt();
    Serial.print("Trim: "); Serial.println(trim);
    Serial.print("  Motor R: "); Serial.print(speedR());
    Serial.print(" | Motor L: "); Serial.println(speedL());
  }
  server.send(200, "text/plain", "OK");
}

void handleCommand() {
  String command = server.uri();
  if (command == "/forward") forward();
  else if (command == "/backward") backward();
  else if (command == "/left") left();
  else if (command == "/right") right();
  else if (command == "/stop") stopMotors();
  server.send(200, "text/plain", "OK");
}

// Captive portal: redirect all unknown requests to root
void handleNotFound() {
  server.sendHeader("Location", "http://192.168.4.1", true);
  server.send(302, "text/plain", "");
}

//--------------------------------------------- (SETUP) ------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Motor pins
  int pins[] = {RENA, RIN1, RIN2, RIN3, RIN4, RENB, LENA, LIN1, LIN2, LIN3, LIN4, LENB};
  for (int i = 0; i < 12; i++) pinMode(pins[i], OUTPUT);
  stopMotors();

  // WiFi Access Point - SSID: "Vector V1", Password: "vectorv1v"
  WiFi.softAP("Vector V1", "vectorv1v");
  delay(100);
  Serial.println("=== MEPER - Vector V1 ===");
  Serial.print("WiFi: Vector V1 | Senha: vectorv1v | IP: ");
  Serial.println(WiFi.softAPIP());

  // DNS server - captive portal (all domains -> 192.168.4.1)
  dnsServer.start(53, "*", WiFi.softAPIP());

  // Web server routes
  server.on("/", handleRoot);
  server.on("/logo", handleLogo);
  server.on("/speed", handleSpeed);
  server.on("/trim", handleTrim);
  server.on("/forward", handleCommand);
  server.on("/backward", handleCommand);
  server.on("/left", handleCommand);
  server.on("/right", handleCommand);
  server.on("/stop", handleCommand);
  server.onNotFound(handleNotFound);  // Captive portal redirect
  server.begin();
  Serial.println("Servidor web rodando. Captive portal ativo.");
}

//--------------------------------------------- (MAIN LOOP) --------
void loop() {
  dnsServer.processNextRequest();  // Captive portal DNS
  server.handleClient();
  delay(2);
}
