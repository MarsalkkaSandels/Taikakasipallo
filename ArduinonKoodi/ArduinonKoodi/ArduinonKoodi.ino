#include <LiquidCrystal.h>  // Ladataan tarvittu kirjasto


LiquidCrystal lcd = LiquidCrystal(2,3,4,5,6,7);   // Luodaan näytölle objekti.

//Seuraavaksi alustetaan lista vastauksista TODO: funktio tekemään muutoksia serialin kautta.
char *actors[] = {"Vastaukseni on ei", "Ala luota siihen", "Lähteeni sanoo ei", "Epäilen", "Lopputulema ei ole hyvä", "Parempi etten kerro nyt", "En voi ennustaa nyt", "Kysy uudestaan myöhemmin", "Epäselvä kysymys, kysy uudelleen", "Keskity ja kysy uudelleen", "Lopputulema on hyvä", "Todennäköisesti", "Merkit viittaavat että kyllä", "Miten näen asian, kyllä", "Kyllä", "Voit luottaa siihen",
 "Epäilemättä", "Kyllä, ehdottomasti", "Se on juurikin näin", "Se on varma"};

void setup() {
  lcd.begin(16, 2); // määritetään näytön koko
  Serial.begin(9600);   //Aappon rng
  randomSeed(600);
}

int randomNumero() {   //funktio rng:lle
  long actor1;
  actor1 = random(sizeof(actors)/sizeof(char*)); //numero on välillä 0-[listan pituus]
  return actor1;
}

void tulostusFunk(int a = 0) {     //funktio tulostukselle
    lcd.print(actors[a]); // printataan ensimmäiselle riville
    delay(2000);  //aika kauanko esitetään vastausta näytöllä
    lcd.clear(); //näytön tyhjennys
}

bool sensoriTriggeri() { //TODO: funktio sensoridatan kuuntelemiselle
//Tämä funktio vois pidattää loop() funktiota ja palauttaa bool arvoja kun sensorien threshold ylittyy jolloinka loop() funktio jatkuisi
}

void moottoriBrrr() { //TODO: funktio jolla voi päristää moottoria tietyissä tilanteissa
  //Käytetään vaikka arduinon porttia 15
}

void kaijutinBrrr() { //TODO: Kaijutin piippaa erinäisiä ääni erinäisissä tilanteissa
  //Käytetään vaikka arduinon porttia 14
}

void loop() {   //perustoiminto loop
  tulostusFunk(randomNumero()); //kutsutaan satunnaisella numerolla tulostus funktio
  delay(1000); //Tämä delay on turha kunhan saadaan sensoridatan kuuntelulle funktio
}