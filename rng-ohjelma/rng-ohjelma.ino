long randNumber;

void setup() {
  Serial.begin(9600);

  // jos analog input 0 on kytkemätön, random analog
  // noise lähettää kutsun randomSeed() luomaan
  // erin numeron joka kerta kun ohjelma suoritetaan.
  // randomSeed() sitten sekoittaa random funktion.
  randomSeed(analogRead(0));
}

void loop() {
  // tulostaa randomin numeron 0-20 välillä
  randNumber = random(20);
  Serial.println(randNumber);

   delay(1000);
}
