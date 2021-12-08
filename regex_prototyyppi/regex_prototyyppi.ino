
#include <Regexp.h>

float muuttuja16;

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
        muuttuja16 = muuttuja16 + length/3;
        Serial.println(cap);
     //   Serial.print("Rivi 1");

      }
      else if (muuttuja16 + length >= 16) {
        muuttuja16 = muuttuja16 + length/3;
        Serial.println(cap);
      //  Serial.print("Rivi 2");

      }
      
    }  // end of for each capture

}  // end of match_callback 


void setup ()
{
  Serial.begin (9600);
  Serial.println ();
  unsigned long count;

  // what we are searching (the target)
  char buf [100] = "Parempi, etten kerro nyt";

  // match state object
  MatchState ms (buf);

  // original buffer
  Serial.println (buf);

  // search for three letters followed by a space (two captures)
  count = ms.GlobalMatch ("(%a+)(%p?)( ?)", match_callback);

  // show results
  Serial.print ("Found ");
  Serial.print (count);            
  Serial.println (" matches.");
 

}  // end of setup  

void loop () {}
