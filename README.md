# pico-game
pico-game je přenosná herní konzole s otevřeným SDK a firmwarem

Hry budou uloženy na SPI pamětích z kterých se bude pico-game bootovat

SDK je rozdělené do knihoven pro
- Zvuk
- Soubory
- Obrazovka
- Ovládání

Součástí SDK je i malý 2D herní engine

## Hardware
- Raspberry Pi Pico (RP2040)
- 1.8" ST7735 TFT Displej
- Akcelerometr
- Tlačítka
- (později) Baterie
- Reproduktor Seeed Grove Speaker

## Software
- C++
- Raspberry Pi Pico SDK
- LittleFS
