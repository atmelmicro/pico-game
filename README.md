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

## Software
- C++
- Raspberry Pi Pico SDK
- LittleFS

# Knihovny

## Screen
Screen je knihovna která ovládá displej, součástí je framebuffer (microbuffer) díky kterému dokáže pico-game plynule obnovit obrazovku. Bohužel framebuffer využívá hodně paměti. Existují jiné metody obnovování displeje, ale ty jsou pomalejší. Celý framebuffer používá ~40kB paměti ((128[width]*160[height]*2[bit depth {bytes}) / 1000[bytes->kilobytes]) = 40.96. 

Každý element na obrazovce je SObject (Screen Object). Základní typy jsou `Rect`, `Fill_Rect`, `Image`, `Text`, atd... 

Microbuffer podporuje 24-bitové bitmapy a 32-bitové bitmapy které jsou uloženy na Picu. Protože čtení z filesystému je pomalé, je nejlepší když sprity jsou načteny do paměti jednou. Pro obrázky které se nepoužívají často jde je přečíst jednou a freenout.

## Input
Kinhovna input zpracovává vstupy z tlačítek a akcelerometru.

Tlačítka zavolají funkci přes interupty. Akcelometr bude muset vývojář pollovat.

## Filesystem

Filesystem spravuje LittleFS oddíl na SPI paměti. Velikost oddílu je nastavitelná. 

Pro nahrání souborů se používá speciální firmware který potřebuje aby soubour byl rozdělen to bytů. V boudoucnosti je možnost udělat SwiftUI nebo web aplikaci pro jednoduché nahrávání souborů.

## Sound

Pro zvuk je plán použít stanadrtní formát zvuku (např .ogg, .midi). Existují 2 typy zvuku - `BackgroundMusic` a `SoundEffect`. 

`BackgroundMusic` stačí zapnout jednou a vždy se bude přehrávat na pozadí, hudba se bude opakovat.

`SoundEffect` se spustí jednou a potom vypne, `SoundEffect` překryje `BackgroundMusic`

## GameEngine

GameEngine je klihovna pro malý 2D herní engine. Podporuje zákládní části game enginu jako fyziku (RigidBody, atd.) a sprity.

# Timeline / Milestones
- [x] Základní knihovna na displej
- [x] Základní knihovna na filesystem
- [x] Základní knihovna na akcelerometr
- [ ] Dokončit knihovnu pro displej (kruhy, kružnice, průhlednost, otáčení objektů)
- [ ] Input knihovna
- [ ] Knihovna pro zvuk
- [ ] Game Engine
- 
