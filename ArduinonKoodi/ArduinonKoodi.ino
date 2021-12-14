#include <NoiascaLiquidCrystal.h>       
#include <NoiascaHW/lcd_4bit.h>        
#include <Regexp.h>                    //ladataan LCD:lle ja RegExpille kirjastot

// alustetaan lcd-näyttö
const byte cols = 16;                  // merkkien määrä rivillä
const byte rows = 2;                   // rivien määrä
const byte rs = 2;
const byte en = 3;
const byte d4 = 4;
const byte d5 = 5;
const byte d6 = 6;
const byte d7 = 7;
const byte bl = 255;      //TODO: tomi kommentoi              

LiquidCrystal_4bit lcd(rs, en, d4, d5, d6, d7, bl, cols, rows);  // luodaan näytölle objekti

// Tästä alkaa settings menuun sisältyvät tiedot
#define button_T    A0
#define button_A    A1
#define button_Y    A2
#define button_E    A3

// buttonit T,A,Y,E tulevat sanoista:
// T = takaisin
// A = alas
// Y = ylös
// E = enter
// tässä on myös määritelty mitä arduinon pinnejä napit käyttävät

#define DEFAULT_DELAY 300

//Seuraavaksi alustetaan lista vastauksista
char *actors[] = {"Vastaukseni on ei", "Älä luota siihen", "Lähteeni sanoo ei", "Epäilen", "Lopputulema ei ole hyvä", "Parempi, etten kerro nyt", "En voi ennustaa nyt", "Kysy uudestaan myöhemmin", "Epäselvä kysymys, kysy uudelleen", "Keskity ja kysy uudelleen", "Lopputulema on hyvä", "Todennäköisesti", "Merkit viittaavat että kyllä", "Miten näen asian, kyllä", "Kyllä", "Voit luottaa siihen",
 "Epäilemättä", "Kyllä, ehdottomasti", "Se on juurikin näin", "Se on varma"};
 
//TODO: opiskelijamode taulukko

// nämä myös sisältyvät settingsMenu() toimintaan
char nappiPaino = '0';

byte menuTaso = 0;      // Taso 1 näyttää kotinäytön
                        // Taso 2 näyttää päävalikon
                        // Taso 3 näyttää niin sanotun "submenun"
byte menu = 1;
byte sub = 1;

unsigned long hNumero_1 = 0; 
unsigned long hNumero_2 = 0;
unsigned long hNumero_3 = 0;

bool LED_STATE = false; // bool-pitää sisällään joko true or false tietoa

bool currState_T = HIGH;
bool currState_A = HIGH;
bool currState_Y = HIGH;
bool currState_E = HIGH;
          
bool prevState_T = HIGH; 
bool prevState_A = HIGH;
bool prevState_Y = HIGH; 
bool prevState_E = HIGH; 
// seuraavat muuttujat ovat unsigned longeja koska aika jota mitataan
// millisekuntteina tuottaa nopeasti suuremman luvun mitä voidaan tallentaa int muuttujaan
unsigned long prevTime_T = 0;
unsigned long prevTime_A = 0;
unsigned long prevTime_Y = 0;
unsigned long prevTime_E = 0;

unsigned long waitTime_T = 50;
unsigned long waitTime_A = 50;
unsigned long waitTime_Y = 50;
unsigned long waitTime_E = 50;

//Määritellään ledien animaatiossa käytettävät pinnit ja setupissa outputeiksi
const int latchPin = 10;
const int clockPin = 11;
const int dataPin = 12;

//Animaatiossa käytettävä taulukko
int datArray[8];  //Todnäk ei tarvitse olla globaali, mutta jostain syystä ei toiminut testatessa kuin globaalina

bool ravistusLippu = false;     //Lippu ravistusfunktion toiminnalle
int sensorValue = 0;            //Muuttuja johon luku kiihtyvyysanturilta
unsigned long aika = millis();  //Muuttuja jossa aika millisekunteina
int ravistusLaskuri = 0;        //Laskurit ravistuksentunnistusta varten
int nollausLaskuri = 0;
const int analogInPin = A4;     //Kiihtyvyysanturin sarjakytkennän sisääntulo

unsigned long regex;            //RegExpin muuttuja
float muuttuja16;               //RegExpin pituuden laskuri
byte kursoriNolla = 1;          //Tulostusfunktion looppia ja regExiä varten muuttuja ettei tulostu samalle riville kahdesti

void setup() {
  lcd.begin(); // käynnistetään näyttö
  Serial.begin(9600);   
  randomSeed(analogRead(A5));      //rng:n pohja luetaan arduinon tyhjältä analog pinniltä melua randomin seediksi
  pinMode(button_T, INPUT_PULLUP); //määritellään pinnien tehtävät pinMode(), kuuluvat settingsMenu()
  pinMode(button_A, INPUT_PULLUP); //määritellään pinnien tehtävät pinMode(), kuuluvat settingsMenu()
  pinMode(button_Y, INPUT_PULLUP); //määritellään pinnien tehtävät pinMode(), kuuluvat settingsMenu()
  pinMode(button_E, INPUT_PULLUP); //määritellään pinnien tehtävät pinMode(), kuuluvat settingsMenu()

  pinMode(A4, INPUT);
 
  pinMode(latchPin, OUTPUT);       //Ledien  animaatioon käytettävät pinnit outputeiksi
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //melodia(0, 1); //avausmelodia, lyhyt duuri kolme nuottia
  naytaKotinaytto();               //Ensimmäinen näyttö kuva missä
  //TODO: Testifunktioita kaikille toimiville osille
}

int randomNumero() {   //funktio rng:lle, kutsutaan tulostusfunktiossa valitsemaan random vastaus
  long actor1 = 0;
  actor1 = random(sizeof(actors)/sizeof(char*)); //numero on välillä 0-[listan pituus]
  return actor1;
}

// kutsutaan jokaisa sanaa kohti tulostusfunktiossa
void match_callback  (const char * match,          // matching string (not null-terminated)
                      const unsigned int length,   // length of matching string
                      const MatchState & ms) {      // MatchState in use (to get captures)

char cap [16];   // must be large enough to hold captures

    for (byte i = 0; i < ms.level; i++){
    ms.GetCapture (cap, i);                   //otetaan capturet jokaisesta halutusta merkistä.
      if (muuttuja16 + length <= 16) {        //printataan ensimmäiselle riville
        muuttuja16 = muuttuja16 + length/2;   //laskuri rivinvaihtoa varten, koodi pyörähtää kerran jokaista regexin syntaksin vaihetta kohden, eli kolmesti meidän tapauksessa. Tämän takia vain kolmasosa matchin pituudesta lisätään laskuriin kierroksessa               
        lcd.print(cap);                       //tulostetaan näytölle
      }
      else if (muuttuja16 + length >= 16) {   //jos vastaus ei mahdu ensimmäiselle riville, katkaistaan se välilyönnissä
        if(kursoriNolla == 0){
          lcd.setCursor(0, 1); //asetetaan näytön kursori toiselle riville
          kursoriNolla++;
        }
        muuttuja16 = muuttuja16 + length/2;
        lcd.print(cap);                       //tulostetaan näytölle
      }
    }  // capturen ja printtauksen loppu
}  // match_callbackin loppu

void tulostusFunk(int a = 0) {      //funktio tulostukselle
    kursoriNolla = 0;               //Nollataan rivinvaihto
    muuttuja16 = 0;                 //nollataan laskuri
    lcd.setCursor(0,0);
    lcd.clear();                    //tyhjennetään näyttö
    MatchState ms (actors[a]);      //alustetaan RegExpille kohde
    regex = ms.GlobalMatch ("(%S+)( ?)", match_callback);
}

void ravistus(){
  unsigned long kulunutAika;      //Lasketaan kahden peräkkäisen mittaustuloksen kulmakerroin.
  kulunutAika = millis() - aika;
  aika = millis();

  int sensorPrevious = sensorValue;
  sensorValue = analogRead(analogInPin);

  int kulmakerroin = (sensorValue - sensorPrevious) / (int)kulunutAika;

  if (kulmakerroin != 0) {        //Nostetaan laskuria kun saatava kulmakerroin ei 0
    ravistusLaskuri++;
    nollausLaskuri=0;
  }

  if(kulmakerroin == 0){          //Nostetaan nollauslaskuria kun kulma 0
    nollausLaskuri++;
  }

  if(nollausLaskuri>=6){          //Tätä vertailua muuttamalla herkkyyden muutos
    ravistusLippu = false;         
    ravistusLaskuri=0;
    nollausLaskuri=0;
  }

  if (ravistusLaskuri >= 11) {    //Tietyn määrän jälkeen led päälle ja if 0 looppiin
    ravistusLippu = true;         //vertailtavaa lukua muuttamalla herkkyyden säätö
    ravistusLaskuri = 0;          //Tähän tietenkin sisälle koodi joka signaloi
    nollausLaskuri = 0;
  }//TODO: testaa taulukointia mittaustuloksissa
 
  Serial.print(kulmakerroin);     //testausta varten
  Serial.print("  ");
  Serial.print(ravistusLaskuri);
  Serial.print("  ");
  Serial.println(nollausLaskuri);

  //HUOM! Funktiossa delay koska funktion toiminta prosessorin kellotaajuudella aivan liian herkkä. Ehkä tarvetta keksiä jokin muu ratkaisu?
  //delay(50);
}

void moottoriBrrr() { //TODO: funktio jolla voi päristää moottoria tietyissä tilanteissa
  //PNP transistori eli low common = päällä
  //TODO: korjaa funktio ja lisää tähän, kutsutaan
}

void melodia(int a = 0, int b = 1) { //Tässä a on melodia mitä halutaan soittaa ja b on toistojen määrä
  switch(a){   // 0 caseksi lyhyt iloinen piippaus, pitemmät melodiat valintojen taakse
    case 0: //Timer1 käyttöön interrupteihin
      tone(9, 392, 100); //g
      delay(50);
      tone(9, 440, 100); //a
      delay(50);
      tone(9, 523, 400); //c
    break;

    case 1: //2400 = tahti
    for(int n = 0; n < b; n++) {    
      tone(9, 293, 200); //d
      delay(400);
      tone(9, 440, 200); //a
      delay(400);
      tone(9, 329, 200); //e
      delay(400);
      tone(9, 440, 200); //a
      delay(400);
      //Tämä kommentti on tahtiviiva
      tone(9, 349, 600); //f
      tone(9, 392, 200); //g
      delay(100);
      tone(9, 440, 200); //a
      delay(100);
      tone(9, 349, 200); //f
      delay(400);
      tone(9, 493, 200); //b
      delay(400);
      //Tämä kommentti on tahtiviiva
      tone(9, 293, 200); //d
      delay(200);
      tone(9, 440, 200); //a
      delay(200);
      tone(9, 329, 200); //e
      delay(200);
      tone(9, 440, 200); //f
      delay(200);
      tone(9, 293, 200); //e
      delay(200);
      tone(9, 440, 200); //f
      tone(9, 329, 200); //e
      tone(9, 440, 200); //d
      delay(200);
      tone(9, 440, 200); //a
      delay(200);
      //Tämä kommentti on tahtiviiva
      tone(9, 440, 200); //a
      delay(200);
      tone(9, 261, 200); //c
      delay(200);
      tone(9, 392, 200); //g
      delay(200);
      tone(9, 440, 200); //a
      delay(200);
      tone(9, 349, 600); //f
      }
    break;

    case 2:
    //melodia
    break; 

    default: //virheen kuuloinen piippaus esim terssi alas g-d#
    tone(9, 196, 100); //g
    tone(9, 155, 200); //d#
    Serial.println("melodia() default");  //Jos virhe niin serialiin viite virheestä
  }
}

void buttonCheck() { // navigointia varten tehty funktio
  // Tässä niin sanotusti "debouncataan" painonapit, jotta varmistutaan että
  // että napin painallukset antavat luotettavia tuloksia
  bool currRead_T = digitalRead(button_T);
  bool currRead_A = digitalRead(button_A);
  bool currRead_Y = digitalRead(button_Y);
  bool currRead_E = digitalRead(button_E);

  if (currRead_T != prevState_T) {
    prevTime_T = millis();
  }
  if (currRead_A != prevState_A) {
    prevTime_A = millis();
  }
  if (currRead_Y != prevState_Y) {
    prevTime_Y = millis();
  }
  if (currRead_E != prevState_E) {
    prevTime_E = millis();
  }

  if ((millis() - prevTime_T) > waitTime_T) {
    if (currRead_T != currState_T) {
      currState_T = currRead_T;
      if (currState_T == LOW) {
        nappiPaino = 'T';  // määrittää napin painamisen toiminnoksi 'takaisin'
      } 
    }
  } else nappiPaino = '0';
  if ((millis() - prevTime_A) > waitTime_A) {
    if (currRead_A != currState_A) {
      currState_A = currRead_A;
      if (currState_A == LOW) {
        nappiPaino = 'A'; // määrittää napin painamisen toiminnoksi 'alas'
      } 
    }
  } else nappiPaino = '0';
  if ((millis() - prevTime_Y) > waitTime_Y) {
    if (currRead_Y != currState_Y) {
      currState_Y = currRead_Y;
      if (currState_Y == LOW) {
        nappiPaino = 'Y'; // määrittää napin painamisen toiminnoksi 'ylös'
      } else {
        //nappiPaino = '0';
      }
    }
  } else nappiPaino = '0';
  if ((millis() - prevTime_E) > waitTime_E) {
    if (currRead_E != currState_E) {
      currState_E = currRead_E;
      if (currState_E == LOW) {
        nappiPaino = 'E'; // määrittää napin painamisen toiminnoksi 'entteri'
      } 
    }
  } else nappiPaino = '0';

  prevState_T = currRead_T;
  prevState_A = currRead_A;
  prevState_Y = currRead_Y;
  prevState_E = currRead_E;

  settingsMenu(nappiPaino);
} //TODO: settingsmenu kutsumaan buttoncheckiä arvona ja buttoncheck palauttamaan nappipaino arvo
//TODO: looppiin rakenne settingsmenu ja nappipaino 

void settingsMenu(char nappiPaino) { // asetusvalikon ohjelma
  switch(menuTaso) {    //käytetään switchcasea navigoinnissa
    case 0: // 
      switch(nappiPaino) {
        case 'E': // Entteri
          menuTaso = 1;
          menu = 1;
          paivitaMenu();     // Siirtää näkymän päävalikkoon
          delay(DEFAULT_DELAY);
          break;
        case 'Y': // ylös
          break;
        case 'A': // alas
          break;
        case 'T': // takaisin
          break;
        default:
          Serial.println("settingsMenu(): case0 default");
          break;
      }
      break;
    case 1: // Taso 1, päävalikko
      switch (nappiPaino) {
        case 'E': // Enter
          paivitaSub();   // Sub = submenu
          menuTaso = 2;  // menee submenuun
          paivitaSub();
          delay(DEFAULT_DELAY);
          break;
        case 'Y': // ylös
          menu++;
          paivitaMenu();
          delay(DEFAULT_DELAY);
          break;
        case 'A': // alas
          menu--;
          paivitaMenu();
          delay(DEFAULT_DELAY);
          break;
        case 'T': // takaisin
          menuTaso = 0;  // piilottaa menun ja siirtyy takaisin kotinaytolle
          naytaKotinaytto();
          delay(DEFAULT_DELAY);
          break;
        default:
          Serial.println("settingsMenu() case 1 default");
          break;
      } 
      break;
    case 2: // taso 2, submenu
      switch (nappiPaino) {
        case 'E': 
          menuTaso = 1;
          paivitaMenu();
          delay(DEFAULT_DELAY);
          break;
        case 'Y': // U
          if (menu == 1) {
            if (hNumero_1 < 3600000) {  // 1 tunti
              hNumero_1 = hNumero_1 + 1;
            } else {
              hNumero_1 = 3600000;
            }
          } else if (menu == 2) {       
            if (hNumero_2 < 3600000) {  // 1 tunti
              hNumero_2 = hNumero_2 + 1;
            } else {
              hNumero_2 = 3600000;
            }
          } else if (menu == 3) {
            if (hNumero_3 < 3600000) {  // 1 tunti  
              hNumero_3 = hNumero_3 + 1;
            } else {
              hNumero_3 = 3600000;
            }
          }
          paivitaSub();
          delay(DEFAULT_DELAY);
          break;
        case 'A': // A
          if (menu == 1) {
            if (hNumero_1 == 0) {
              hNumero_1 = 0;
            } else {
              hNumero_1 = hNumero_1 - 1;
            }
          } else if (menu == 2) {
            if (hNumero_2 == 0) {
              hNumero_2 = 0;
            } else {
              hNumero_2 = hNumero_2 - 1;
            }
          } else if (menu == 3) {
            if (hNumero_3 == 0) {
              hNumero_3 = 0;
            } else {
              hNumero_3 = hNumero_3 - 1;
            }
          }
          paivitaSub();
          delay(DEFAULT_DELAY);
          break;
        case 'T': // L
          menuTaso = 1;  // takaisin päävalikkoon
          paivitaMenu();
          delay(DEFAULT_DELAY);
          break;
        default:
          Serial.println("settingsMenu() case2 default");
          break;
      } 
      break;
    case 3: // Taso 3, jos lisättäisiin sub menulle vielä toinen sub menu
      break;
    default:
      Serial.println("settinsMenu() menutaso default");
      break;
  }
}

void paivitaMenu() {   // Avataan päävalikko josta avautuu eri valinnat
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(" >Aloita Peli "); // ">" käytetään kursorina ohjaamaan haluttua submenua
      lcd.setCursor(0, 1);
      lcd.print(" Syota Vastaus ");
      break;
    case 2:
      lcd.clear();
      lcd.print("  Aloita Peli ");
      lcd.setCursor(0, 1);
      lcd.print(">Syota Vastaus "); // ">" käytetään kursorina ohjaamaan haluttua submenua
      break;
    case 3:
      lcd.clear();
      lcd.print(" >Opiskelija "); // ">" käytetään kursorina ohjaamaan haluttua submenua
      lcd.setCursor(0, 1);
      lcd.print("             ");
      break;
    default:
      Serial.println("paivitaMenu() default"); //Default virheilmoitus
      break;
  }
}

void paivitaSub() {
  switch (menu) {
    case 1:  //Tässä pelitoiminto eli ravistus yms
      toimintaFunk(); //Perustoiminto pelille
      break;
    case 2: //TODO: tehdää tästä testaus valinta ja toiminta
      lcd.clear();
      lcd.print(" Vastaus:");
      lcd.setCursor(0, 1);
      lcd.print(" Oma Vastaus = ");
      break;
    case 3:
      lcd.clear();
      lcd.print(" Alkoholi:");
      lcd.setCursor(0, 1);
      lcd.print(" = ");
      break;
    default:
      Serial.println("päivitäSub() default");
      break;
  }
}

void naytaKotinaytto() { // Kotinäyttö näkymä
  lcd.clear();
  lcd.print("  Taika 8-pallo  ");
  lcd.setCursor(0,1);
  lcd.print(" - Paina enter  ");
}

void lediAnimaatio(int n = 0) {
  //perustuu https://dronebotworkshop.com/shift-registers/ löytyvään koodiin
  //kohdasta "74HC595 and 74HC165 Sketch 2 – Exciting!"
    switch (n) {
      case 0:
        datArray[0] = B11111111;
        datArray[1] = B01111110;
        datArray[2] = B10111101;
        datArray[3] = B11011011;
        datArray[4] = B11100111;
        datArray[5] = B11011011;
        datArray[6] = B10111101;
        datArray[7] = B01111110;
        break;

      case 1:
        datArray[0] = B00000001;
        datArray[1] = B00000010;
        datArray[2] = B00000100;
        datArray[3] = B00001000;
        datArray[4] = B00010000;
        datArray[5] = B00100000;
        datArray[6] = B01000000;
        datArray[7] = B10000000;
        break;

      case 2:
        datArray[0] = B10000001;
        datArray[1] = B01000010;
        datArray[2] = B00100100;
        datArray[3] = B00011000;
        datArray[4] = B00000000;
        datArray[5] = B00100100;
        datArray[6] = B01000010;
        datArray[7] = B10000001;
        break;

      case 3:
        datArray[0] = B10101010;
        datArray[1] = B01010101;
        datArray[2] = B10101010;
        datArray[3] = B01010101;
        datArray[4] = B10101010;
        datArray[5] = B01010101;
        datArray[6] = B10101010;
        datArray[7] = B01010101;
        break;

      case 4:
        datArray[0] = B10000000;
        datArray[1] = B00000001;
        datArray[2] = B01000000;
        datArray[3] = B00000010;
        datArray[4] = B00100000;
        datArray[5] = B00000100;
        datArray[6] = B00010000;
        datArray[7] = B00001000;
        break;

      case 5:
        datArray[0] = B11000000;
        datArray[1] = B01100000;
        datArray[2] = B00110000;
        datArray[3] = B00011000;
        datArray[4] = B00001100;
        datArray[5] = B00000110;
        datArray[6] = B00000011;
        datArray[7] = B10000001;
        break;

      case 6:
        datArray[0] = B11100000;
        datArray[1] = B01110000;
        datArray[2] = B00111000;
        datArray[3] = B00011100;
        datArray[4] = B00001110;
        datArray[5] = B00000111;
        datArray[6] = B10000011;
        datArray[7] = B11000001;
        break;

      case 7:
        datArray[0] = B10001000;
        datArray[1] = B01000100;
        datArray[2] = B00100010;
        datArray[3] = B00010001;
        datArray[4] = B10001000;
        datArray[5] = B01000100;
        datArray[6] = B00100010;
        datArray[7] = B00010001;
        break;

      default:
      break;
    }

    //Ledien väläyttely
    for (int i=0; i < 8; i++){
      //latch "kiinni"
      digitalWrite(latchPin, LOW);

      //Bitit sisään, datapin kirjoittaa isoimman bitin ekana joka luetaan clockpinin sykkeellä
      //Tässä tapauksessa animaatiot tulevat x arvosta riippuen, i pyörityksellä käydään läpi
      shiftOut(dataPin,clockPin,MSBFIRST,datArray[i]);
      
      //latch "auki"
      digitalWrite(latchPin, HIGH);
      //delay(50); TÄMÄN TAKIA EI TOIMINU
    }
}

void toimintaFunk(){
  bool toisto = true; //while looppia varten
  byte kiertavaLuku = 0; //ledien kiertämistä loopissa varten
  lcd.clear();
    while(toisto){
      //TODO: jos onnistuu niin arvontamelodialle interrupteilla tähän ravistuslippu riippuvainen melodia
      ravistus(); //kuunnellaan ravistus joka muuttaa ravistuslipun tilanteesta riippuen
      if(ravistusLippu == true) { //jos on ravistettu niin että raja menee rikki niin ledit ja kaijutin seis, tulostetaan vastaus näytölle
        tulostusFunk(randomNumero());
        ravistusLippu = false;
        toisto = false;
    } //Ei pistetän lcd.clear tähän että vastaus on näkyvillä käyttäjän haluaman ajan
      lediAnimaatio(kiertavaLuku); //jos laitetta ei ravistettu niin päivitetään ledejä ja niihin liittyviä muuttujia
      kiertavaLuku++;
      if(kiertavaLuku >= 8) { //ledi variaatioita vain 8 joten kierrätettävä luku on resetattava
        kiertavaLuku = 0;
    }
  }
}

void loop() {     //perustoiminto loop tässä suunnitelmassa vain buttoncheckin kutsu koska siitä alkaa menuralli
  buttonCheck();  //Aloittaa toimintopuun asetusmenulle
}
