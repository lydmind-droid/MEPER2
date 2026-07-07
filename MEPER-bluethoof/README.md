# MEPER - Controle Bluetooth

Aplicativo Android (MIT App Inventor) + firmware ESP32 para controle de carrinhos MEPER via Bluetooth Classic (SPP).

## Carrinhos suportados

| Carrinho | Motores | Driver | Pinos GPIO | Controles |
|----------|---------|--------|------------|-----------|
| **Vector V1** | 4 | 2x L298N | 12 | Joystick + Speed + Trim |
| **Sisyphus** | 2 | 1x L298N | 6 | Joystick + Speed Linear + Speed Turn |

## Estrutura

```
MEPER/
  app/
    MEPER.aia              # Importar no MIT App Inventor
    build_aia.py           # Gera .aia a partir dos fontes
    src/                   # Fontes versionaveis do app
  firmware/
    vector-v1-bluetooth/   # Firmware Vector V1 (PlatformIO)
    sisyphus-bluetooth/    # Firmware Sisyphus (PlatformIO)
```

## Protocolo Bluetooth (SPP)

Comandos ASCII terminados com `\n`:

| Comando | Descricao |
|---------|-----------|
| `forward\n` | Mover para frente |
| `backward\n` | Mover para tras |
| `left\n` | Virar esquerda |
| `right\n` | Virar direita |
| `stop\n` | Parar motores |
| `speed:150\n` | Velocidade linear (0-255) |
| `trim:25\n` | Trim (-50 a +50) - Vector V1 |
| `turnspeed:150\n` | Velocidade de giro (0-255) - Sisyphus |

## Como usar

### Firmware

1. Instalar [PlatformIO](https://platformio.org/)
2. Compilar e fazer upload:
   ```bash
   cd firmware/vector-v1-bluetooth   # ou sisyphus-bluetooth
   pio run -t upload
   ```

### App

1. Gerar o .aia:
   ```bash
   cd app
   python build_aia.py
   ```
2. Abrir [MIT App Inventor](https://ai2.appinventor.mit.edu/)
3. Importar `app/MEPER.aia`
4. Build > App (APK) para instalar no celular

### Teste sem hardware

Usar o app "Serial Bluetooth Terminal" no celular para verificar os comandos enviados pelo app MEPER.

## Teste E2E

1. Upload do firmware no ESP32
2. Instalar app no celular
3. Parear Bluetooth (Vector V1 ou Sisyphus)
4. Conectar pelo app
5. Controlar com joystick e sliders
