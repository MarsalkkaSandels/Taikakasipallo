#include <LiquidCrystal.h>  // Ladataan tarvittu kirjasto
#include <Regexp.h> 


LiquidCrystal lcd = LiquidCrystal(2,3,4,5,6,7);   // Luodaan näytölle objekti.
char *actors[]={"Vastaukseni on ei", "Ala luota siihen", "Lähteeni sanoo ei", "Epäilen", "Lopputulema ei ole hyvä", "Parempi etten kerro nyt", "En voi ennustaa nyt", "Kysy uudestaan myöhemmin", "Epäselvä kysymys, kysy uudelleen", "Keskity ja kysy uudelleen", "Lopputulema on hyvä", "Todennäköisesti", "Merkit viittaavat että kyllä", "Miten näen asian, kyllä", "Kyllä", "Voit luottaa siihen", "Epäilemättä", "Kyllä, ehdottomasti", "Se on juurikin näin", "Se on varma"};


void setup() {
  lcd.begin(16, 2); // määritetään näytön koko
  Serial.begin(9600);   //Aappon rng
  randomSeed(600);

}
int randomNumero() {   //funktio rng:lle
  long actor1;
  actor1 = random(sizeof(actors)/sizeof(char*));
  return actor1;
}

void tulostusFunk(int a = 0) {     //funktio tulostukselle
    lcd.print(actors[a]); // printataan ensimmäiselle riville
    delay(2000);
    lcd.clear(); //näytön tyhjennys

}
void loop() {   //kutsutaan molemmat funktiot
  tulostusFunk(randomNumero());
  delay(1000);
}
