TÜRKÇE

# Heat-Zone-breathe-analyzer-A0-series-
### 1. Genel Mantık
Bu cihaz, göğsüne taktığın bir kemerin ne kadar gerildiğine bakarak senin nasıl nefes aldığını anlar. 
- Derin nefes alırsan ışıklar artar.
- Nefesinde bir gariplik olursa "bip bip" diye bağırır.
- Her şey yolundaysa "ben buradayım" der gibi 8 saniyede bir ufak bir ses çıkarır.

### 2. Malzemeler
* Arduino Mega (Beyin kısmı)
* Load Cell + HX711 (Gerginliği ölçen asıl parça ve onun yardımcısı)
* 2 Tane Çevirmeli Düğme (Hassaslık ayarı yapmak için)
* 2 Tane Basmalı Düğme (Başlatmak ve sıfırlamak için)
* Renkli Lambalar (LED) ve bir tane de Ses çıkaran parça (Buzzer)

### 3. Kullanım
1. Kemerini tak, sensörün yerleştiğinden emin ol.
2. "Sıfırlama" (Dara) düğmesine bas ki cihaz senin o anki halini "başlangıç" kabul etsin.
3. Beyaz lamba yanınca "tamam, nefes alabilirsin" diyor demektir.
4. Nefes aldıkça 5 tane lamba sırayla yanar, seviyeni oradan görebilirsin.
5. Eğer çok ses çıkarırsa, düğmeye 3 kere hızlıca bas, 30 saniye boyunca susar.

## 🔌 Pin Bağlantıları

| Bileşen | Arduino Pini | Açıklama |
| :--- | :--- | :--- |
| **HX711 DT** | D8 | Veri (Data) |
| **HX711 SCK** | D9 | Saat (Clock) |
| **Potansiyometre 1** | A0 | Eşik (Taban) Ayarı |
| **Potansiyometre 2** | A1 | LED Aralık Ayarı |
| **Başlat/Durdur Butonu** | D12 | Dahili Pull-up |
| **Dara (Sıfırlama) Butonu** | D11 | Dahili Pull-up |
| **Buzzer (+)** | D13 | Uyarı Sesi |
| **Mavi LED** | D10 | Self-Test Göstergesi |
| **Beyaz LED** | D2 | Hazır Göstergesi |
| **LED 1 - LED 5** | D3, D4, D5, D6, D7 | Solunum Kademeleri |

FOR ENGLISH

   ### 1. General Idea
This device watches a chest strap and checks how much it stretches when you breathe.
- Breathe deep, and more lights turn on.
- If something is wrong with your breathing, it starts beeping fast.
- If everything is okay, it gives a tiny "I'm working" beep every 8 seconds.

### 2. Hardware
* Arduino Mega (The "Brain")
* Load Cell + HX711 (The sensor that feels the pull and its converter)
* 2 Knobs (To adjust how sensitive the lights are)
* 2 Buttons (To start/stop and to reset the scale)
* Colored Lights (LEDs) and a little speaker (Buzzer)

### 3. Steps
1. Put the strap on and make sure the sensor is snug.
2. Click the "Tare" (Reset) button so the device knows where to start counting from.
3. When the White Light turns on, it's saying "I'm ready, start breathing!".
4. Watch the 5 lights go up and down as you breathe.
5. If the noise bothers you, click the main button 3 times quickly to mute it for 30 seconds.

🔌 ARDUINO BOARD PINOUT 🔌
   ____________________________________
  |  [PIN]  |      [COMPONENT]         |
  |=========|==========================|
  |   D2    | White LED (Ready)     |
  |   D3    | LED 1 (Level 1)       |
  |   D4    |LED 2 (Level 2)        |
  |   D5    | LED 3 (Level 3)       |
  |   D6    | LED 4 (Level 4)       |
  |   D7    | LED 5 (Level 5)       |
  |---------|--------------------------|
  |   D8    | HX711 DT (Data)      |
  |   D9    | HX711 SCK (Clock)    |
  |   D10   | Blue LED (Test)      |
  |---------|--------------------------|
  |   D11   | Tare Button (Reset)  |
  |   D12   | Start/Stop Button    |
  |   D13   | Buzzer (+)           |
  |---------|--------------------------|
  |   A0    | Pot 1 (Threshold)    |
  |   A1    | Pot 2 (LED Range)    |
  |_________|__________________________|
   
