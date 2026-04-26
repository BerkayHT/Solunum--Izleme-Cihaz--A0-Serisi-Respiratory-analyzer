#include "HX711.h"

// --- PİN TANIMLAMALARI ---
const int DT_PIN = 8;
const int SCK_PIN = 9;
const int POT_TABAN_PIN = A0;   
const int POT_ARALIK_PIN = A1;  
const int READY_LED = 2;        
const int ledPins[] = {3, 4, 5, 6, 7}; 
const int BLUE_LED = 10;        
const int BTN_DARA = 11;        
const int BTN_BASLAT = 12;      
const int BUZZER_PIN = 13;      

// --- SİSTEM DEĞİŞKENLERİ ---
HX711 terazi;
bool sistemCalisiyor = false;
unsigned long buzzerZaman = 0; 
unsigned long sonNormalBeep = 0;
unsigned long muteBitisZamani = 0;
float filtreliNefes = 0;
float alfa = 0.25; 

// --- BUTON KONTROL DEĞİŞKENLERİ ---
int basmaSayisi = 0;
unsigned long sonBasmaZamani = 0;

void setup() {
  Serial.begin(9600); 
  
  pinMode(READY_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  for (int i = 0; i < 5; i++) pinMode(ledPins[i], OUTPUT);
  
  pinMode(BTN_DARA, INPUT_PULLUP);
  pinMode(BTN_BASLAT, INPUT_PULLUP);
  
  terazi.begin(DT_PIN, SCK_PIN);
  terazi.set_scale(-1140); // Kalibrasyon faktörü
  terazi.tare();
  
  Serial.println(">>> CIHAZ HAZIR. BASLAT TUSUNA BASIN. <<<");
}

// --- SELF-TEST (DONANIM KONTROL) FONKSİYONU ---
void selfTest() {
  Serial.println("\n[!] SELF TEST BASLATILDI...");
  digitalWrite(BLUE_LED, HIGH);
  
  digitalWrite(BUZZER_PIN, HIGH); delay(200); digitalWrite(BUZZER_PIN, LOW);
  delay(500);

  for(int i=0; i<5; i++) {
    digitalWrite(ledPins[i], HIGH);
    Serial.print("LED "); Serial.print(i+1); Serial.println(" OK");
    delay(1200); 
  }
  
  digitalWrite(READY_LED, HIGH);
  Serial.println("BEYAZ LED OK");
  delay(1500); 

  digitalWrite(READY_LED, LOW);
  for(int i=0; i<5; i++) digitalWrite(ledPins[i], LOW);
  
  delay(1000);
  digitalWrite(BLUE_LED, LOW);
  Serial.println("[!] TEST TAMAMLANDI. MAVI LED SONDU.\n");
}

void loop() {
  // 1. SELF-TEST KONTROLÜ
  if (digitalRead(BTN_BASLAT) == LOW && digitalRead(BTN_DARA) == LOW) {
    selfTest();
  }

  // 2. SİSTEM BAŞLATMA VE SUSTURMA (MUTE) KONTROLÜ
  if (digitalRead(BTN_BASLAT) == LOW) {
    unsigned long suan = millis();
    if (suan - sonBasmaZamani < 1500) {
      basmaSayisi++;
    } else {
      basmaSayisi = 1;
    }
    sonBasmaZamani = suan;

    if (basmaSayisi >= 3) {
      muteBitisZamani = millis() + 30000;
      Serial.println("\n[MUTE] 8 SN SINYALI 30 SANIYE SUSTURULDU!\n");
      basmaSayisi = 0;
    } else {
      delay(300); 
      if (digitalRead(BTN_BASLAT) == HIGH) {
         sistemCalisiyor = !sistemCalisiyor;
         
         if (sistemCalisiyor) {
           Serial.println(">>> SISTEM ACILDI <<<");
           
           for(int j = 0; j < 3; j++) {
             digitalWrite(READY_LED, HIGH);
             for(int i=0; i<5; i++) digitalWrite(ledPins[i], HIGH);
             delay(200);
             digitalWrite(READY_LED, LOW);
             for(int i=0; i<5; i++) digitalWrite(ledPins[i], LOW);
             delay(200);
           }
         } else {
           Serial.println(">>> SISTEM DURDURULDU <<<");
         }
      }
    }
  }

  // 3. SIFIRLAMA (DARA) KONTROLÜ
  if (digitalRead(BTN_DARA) == LOW && digitalRead(BTN_BASLAT) == HIGH) {
    terazi.tare();
    Serial.println("\n[DARA] SIFIRLANDI.\n");
    delay(500);
  }

  // 4. ANA ÇALIŞMA DÖNGÜSÜ
  if (sistemCalisiyor) {
    float hamNefes = terazi.get_units(1);
    if(hamNefes < 0) hamNefes = 0;
    filtreliNefes = (alfa * hamNefes) + (1.0 - alfa) * filtreliNefes;

    float tabanGram = map(analogRead(POT_TABAN_PIN), 0, 1023, 1, 255); 
    float aralikGram = map(analogRead(POT_ARALIK_PIN), 0, 1023, 1, 10);

    float enUstSinir = tabanGram + (5 * aralikGram) + 20;
    float altSinir = tabanGram - 5;

    // --- SERİ MONİTÖR ÇIKTISI ---
    Serial.print("Nefes: "); Serial.print(filtreliNefes, 1);
    Serial.print("g | Esik: "); Serial.print(tabanGram, 0);
    Serial.print("g | LED Fark: "); Serial.print(aralikGram, 0);
    if(millis() < muteBitisZamani) Serial.print(" [SESSIZ MOD]");
    Serial.println();

    // --- ALARM YÖNETİMİ ---
    if (filtreliNefes > enUstSinir || (filtreliNefes < altSinir && filtreliNefes > 2)) {
      if (millis() - buzzerZaman > 100) {
        digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
        buzzerZaman = millis();
      }
    } 
    else {
      digitalWrite(BUZZER_PIN, LOW); 
      if (millis() > muteBitisZamani) {
        if (millis() - sonNormalBeep > 8000) {
          digitalWrite(BUZZER_PIN, HIGH); delay(20); digitalWrite(BUZZER_PIN, LOW);
          sonNormalBeep = millis();
        }
      }
    }

    // --- LED GÖSTERGE YÖNETİMİ ---
    if (filtreliNefes >= (tabanGram - 3.0) && filtreliNefes < tabanGram) digitalWrite(READY_LED, HIGH);
    else digitalWrite(READY_LED, LOW);

    for (int i = 0; i < 5; i++) {
      float esik = tabanGram + (i * aralikGram);
      if (filtreliNefes >= esik) digitalWrite(ledPins[i], HIGH);
      else digitalWrite(ledPins[i], LOW);
    }

  } else {
    // SİSTEM BEKLEME DURUMU
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(READY_LED, LOW);
    for (int i = 0; i < 5; i++) digitalWrite(ledPins[i], LOW);
  }
}