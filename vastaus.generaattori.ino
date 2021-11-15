char *actors[]={"Vastaukseni on ei", "Ala luota siihen", "Lähteeni sanoo ei", "Epäilen", "Lopputulema ei ole hyvä", "Parempi etten kerro nyt", "En voi ennustaa nyt", "Kysy uudestaan myöhemmin", "Epäselvä kysymys, kysy uudelleen", "Keskity ja kysy uudelleen", "Lopputulema on hyvä", "Todennäköisesti", "Merkit viittaavat että kyllä", "Miten näen asian, kyllä", "Kyllä", "Voit luottaa siihen", "Epäilemättä", "Kyllä, ehdottomasti", "Se on juurikin näin", "Se on varma"};
long actor1;

void setup()
{
  Serial.begin(9600);
  randomSeed(600);
}

void loop()                     
{
  actor1 = random(sizeof(actors)/sizeof(char*));
  Serial.println(actors[actor1]);
  delay(1000); 
}
