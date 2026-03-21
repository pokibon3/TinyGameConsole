# TinyGameConsole (UIAPduino)

MountRiver Studio 1 プロジェクトを、`wch-sdk` (`noneos-sdk`) の PlatformIO 環境でビルドできるようにした構成です。  
`cw_decoder3` のハードウェア配線に合わせて、LCD と入力ピンを調整しています。

## Build Environment

- Board: `UIAPduino`
- PlatformIO platform: `https://github.com/Community-PIO-CH32V/platform-ch32v.git`
- Framework: `noneos-sdk`
- Upload: `wch-link`

`platformio.ini` では以下を使用:

- Linker script: `Ld/Link.ld`
- Startup: `Startup/startup_ch32v00x.S`
- Source filter:
  - include: `User/*.c`, `User/games/*.c`, `Debug/debug.c`
  - exclude: `User/games/pvd_test.c`

## Hardware Wiring

### ST7735 (SPI)

- `SCK`: `PC5`
- `MOSI`: `PC6`
- `CS`: `PC3`
- `DC`: `PD0`
- `RST`: `PC7`

補足:

- `LCD_DEFAULT_ROTATION` は `2`（上下反転側）。
- `MISO` は未使用（TX only）。

### Buttons (3 buttons)

- `LEFT`: `PA1`
- `RIGHT`: `PC4`
- `ACTION`: `PD2`
- `UP/DOWN`: 未使用（コード上は常に未押下扱い）

### Speaker (PWM)

- `SPK_OUT`: `PC0` (`TIM2 CH3`)

## Controls

### Common

- メインメニュー:
  - `RIGHT`: 次の項目
  - `LEFT`: 前の項目
  - `ACTION`: 決定

### Game Specific

- Snake:
  - `LEFT/RIGHT`: 左右移動
  - `ACTION`: 縦方向を交互切替（UP/DOWN）
  - Game Over 復帰: `ACTION`

- Jump Bird:
  - `ACTION`: ジャンプ
  - Game Over 復帰: `ACTION`

- Dino Run:
  - `ACTION`: ジャンプ
  - `RIGHT`: しゃがみ（押下中）
  - Game Over 復帰: `ACTION`

- Pong:
  - `LEFT/RIGHT`: パドル上下移動
  - `LEFT + RIGHT`: 強制ゲームオーバー（デバッグ用途）
  - Game Over 復帰: `ACTION`

- High Score:
  - `ACTION`: メニューへ戻る
  - `LEFT + RIGHT + ACTION`: 全データ消去確認画面へ
  - 確認画面:
    - `LEFT`: キャンセル
    - `RIGHT`: 全データ消去

- Flashlight:
  - `ACTION`: メニューへ戻る

## Build / Upload

```bash
cd /Users/ooe/src/TinyGameConsole
pio run
pio run -t upload
```

## Firmware Updater Tool Usage

1. UIAPduino のリセットボタンを押しながら USB ケーブルを接続し、BootLoader モードにします。
2. ツールは mac 版と Windows 版があります。
   - mac: `tools/mac/firmwareUpdater`
   - Windows: `tools/win/firmwareUpdater`
3. スクリプトを実行します。
   - mac: `flash.sh`
   - Windows: `flash.bat`
4. 書き込み後、UIAPduino をリセットするか、電源を再投入します。

## References

- Repository: `max7d3/CH32V003-MiniGameConsole`
  - URL: [https://github.com/max7d3/CH32V003-MiniGameConsole](https://github.com/max7d3/CH32V003-MiniGameConsole)
- Repository: `Community-PIO-CH32V/platform-ch32v`
  - URL: [https://github.com/Community-PIO-CH32V/platform-ch32v](https://github.com/Community-PIO-CH32V/platform-ch32v)
- Repository: `Community-PIO-CH32V/framework-wch-noneos-sdk`
  - URL: [https://github.com/Community-PIO-CH32V/framework-wch-noneos-sdk](https://github.com/Community-PIO-CH32V/framework-wch-noneos-sdk)
