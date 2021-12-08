#include <NoiascaLiquidCrystal.h>      // download library from https://werner.rothschopf.net/202009_arduino_liquid_crystal_intro.htm 
#include <NoiascaHW/lcd_4bit.h>        // parallel interface, 4bit
<<<<<<< HEAD


char *actors[]={"Vastaukseni on ei", "Ala luota siihen", "Lähteeni sanoo ei", "Epäilen", "Lopputulema ei ole hyvä", "Parempi etten kerro nyt", "En voi ennustaa nyt", "Kysy uudestaan myöhemmin", "Epäselvä kysymys, kysy uudelleen", "Keskity ja kysy uudelleen", "Lopputulema on hyvä", "Todennäköisesti", "Merkit viittaavat että kyllä", "Miten näen asian, kyllä", "Kyllä", "Voit luottaa siihen",
=======
#include <Regexp.h>

unsigned long regex;
float muuttuja16;

char *actors[]={"Vastaukseni on ei", "Ala luota siihen", "Lähteeni sanoo ei", "Epäilen", "Lopputulema ei ole hyvä", "Parempi etten kerro nyt", "En voi ennustaa nyt", "Kysy uudestaan myöhemmin", "Epäselvä kysymys, kysy uudelleen", "Keskity ja kysy uudelleen", "Lopputulema on hyvä", "Todennäköisesti", "Merkkien mukaan kyllä", "Miten näen asian, kyllä", "Kyllä", "Voit luottaa siihen",
>>>>>>> 02f5af63cabe994fd77b81d8740f3e14489b20e4
 "Epäilemättä", "Kyllä, ehdottomasti", "Se on juurikin näin", "Se on varma"};

const byte cols = 16;                  // columns/characters per row
const byte rows = 2;                   // how many rows
const byte rs = 2;
const byte en = 3;
const byte d4 = 4;
const byte d5 = 5;
const byte d6 = 6;
const byte d7 = 7;
const byte bl = 255;                    // set to 255 if not used

LiquidCrystal_4bit lcd(rs, en, d4, d5, d6, d7, bl, cols, rows);  // create lcd object
//LiquidCrystal_4bit_base lcd(rs, en, d4, d5, d6, d7, bl, cols, rows);  // base class without UTF-8

<<<<<<< HEAD
=======
// called for each match
void match_callback  (const char * match,          // matching string (not null-terminated)
                      const unsigned int length,   // length of matching string
                      const MatchState & ms)      // MatchState in use (to get captures)
{
char cap [16];   // must be large enough to hold captures
  
  Serial.print ("Matched: ");
  Serial.write ((byte *) match, length);
  Serial.println ();
  
  for (byte i = 0; i < ms.level; i++)
    {
   
    ms.GetCapture (cap, i);
 
      if (muuttuja16 + length <= 16) {
        lcd.setCursor(0, 0);
        muuttuja16 = muuttuja16 + length/3;
        Serial.println(cap);
        lcd.print(cap);
        Serial.println("Rivi 1");

      }
      else if (muuttuja16 + length >= 16) {
        lcd.setCursor(1, 0);
        muuttuja16 = muuttuja16 + length/3;
        Serial.println(cap);
        lcd.print(cap);
        Serial.println("Rivi 2");

      }
      
    }  // end of for each capture

}  // end of match_callback 


>>>>>>> 02f5af63cabe994fd77b81d8740f3e14489b20e4

void setup() {
  lcd.begin(); // määritetään näytön koko
  Serial.begin(9600);   //Aappon rng
  randomSeed(600);
  

}
int randomNumero() {   //funktio rng:lle
  long actor1;
  actor1 = random(sizeof(actors)/sizeof(char*));
  return actor1;
}

void tulostusFunk(int a = 0) {     //funktio tulostukselle
<<<<<<< HEAD
    lcd.print(actors[a]); // printataan ensimmäiselle riville
    delay(2000);
    lcd.clear(); //näytön tyhjennys
=======
    muuttuja16 = 0;
    lcd.clear();
    MatchState ms (actors[a]);
    regex = ms.GlobalMatch ("(%a+)(%p?)( ?)", match_callback);
    
>>>>>>> 02f5af63cabe994fd77b81d8740f3e14489b20e4

}
void loop() {   //kutsutaan molemmat funktiot
  tulostusFunk(randomNumero());
<<<<<<< HEAD
  delay(1000);
}
=======
  delay(10000);
}
>>>>>>> 02f5af63cabe994fd77b81d8740f3e14489b20e4
