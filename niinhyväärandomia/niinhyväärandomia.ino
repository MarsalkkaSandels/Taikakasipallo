char *actors[][] = {
  {"Vastaukseni on ei", "Älä luota siihen", "Lähteeni sanoo ei", "Epäilen", "Lopputulema ei ole hyvä", "Parempi, etten kerro nyt", "En voi ennustaa nyt", "Kysy uudestaan myöhemmin", "Epäselvä kysymys, kysy uudelleen", "Keskity ja kysy uudelleen", "Lopputulema on hyvä", "Todennäköisesti", "Merkit viittaavat että kyllä", "Miten näen asian, kyllä", "Kyllä", "Voit luottaa siihen", "Epäilemättä", 
  "Kyllä, ehdottomasti", "Se on juurikin näin", "Se on varma"},
  {"Kaliaa", "Siideriä", "Lonkeroa", "Jaloviinaa", "Gambiinaa", "Salmari", "Fisu", "Lökö", "Horna", "Tekilaa"}
};

int moodi = 0;

char randomFunk() {
  char palautettavaVastaus = actors[moodi, random(sizeof(actors[moodi])/sizeof(*char))];
  return palautettavaVastaus;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(moodi == 1) {
    moodi = 0;
  }
  if(moodi == 0) {
    moodi = 1;
  }
  Serial.println(randomFunk());
  delay(500);
}