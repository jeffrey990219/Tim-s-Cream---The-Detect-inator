/*
 Ultrasonic-Ranger

 Measure the distance to obstacles in front and display the value on 
 the Grove 4-Digit Display
 
 The circuit:
 * 4-Digit Display attached to Pin 38 and 39 (J14 plug on Grove Base BoosterPack)
 * Ultrasonic Ranger attached to Pin 24 (J6 plug on Grove Base BoosterPack)
 
 * Note:  
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/depot/Grove-Ultrasonic-Ranger-p-960.html
 */
 
#include "TM1637.h" 
#include "Ultrasonic.h"
#include "DHT.h"


/* Macro Define */
#define CLK               38              /* 4-Digit Display clock pin */
#define DIO               37              /* 4-Digit Display data pin */
#define ULTRASONIC_PIN    24              /* pin of the Ultrasonic Ranger */
#define BUZZER_PIN               39            /* sig pin of the Grove Buzzer */
#define SOUND_SENSOR       25           /* sound sensor pin */          
#define LED                RED_LED      /* LED pin */
#define SOUND_THRESHOLD    5000          /* The threshold to turn the LED on 200.00*3.3/1024 = 0.65v */
#define MOISTURE_PIN        26          /* pin of Moisture Sensor */
#define MOISTURE_THRESHOLD     300         /* threshold for watering the plant */
#define TEMP_THRESHOLD         26          /*   threshold for fire threshold   */
#define TEMP_HUMI_PIN     27                 /* pin of temperature&humidity sensor */
#define ROTARY_ANGLE_P    23          /* pin of rotary angle sensor */

/* Global Variables */
/* songs */
/*  Star Song */
int length_star = 15;                                                 /* the number of notes */
char notes_star[] = "ccggaagffeeddc ";                                /* notes in the song */
int beats_star[] = {1,1,1,1,1,1,2,1,1,1,1,1,1,2,4};                   /* length of each note */
int tempo_star = 200;                                                 /* tempo of notes */
char names_star[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };       /* note names */
int tones_star[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 }; /* tone of each note */

/* Birthday Song */
int length_bday = 26;                                                                           /* the number of notes */
char notes_bday[] = "ggagCbggagDCggGECbaFFECDC ";                                               /* notes in the song */
int beats_bday[] = {3,1,4,4,4,8,3,1,4,4,4,8,3,1,4,4,4,4,4,3,1,4,4,4,8,8};                       /* length of each note */
int tempo_bday = 100;                                                                           /* tempo of notes */
char names_bday[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C','D','E','F','G' };                 /* note names */
int tones_bday[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 850,760,716,638};           /* tone of each note */

/* Jingle Bells */
int length_bells = 48;                                                                                                /* the number of notes */
char notes_bells[] = "eeeeeeegcdefffffeeeddedgeeeeeeegcdefffffeeggfdc";                                               /* notes in the song */
int beats_bells[] = {1,1,2,1,1,2,1,1,1,1,4,1,1,1,1,1,1,2,1,1,1,1,2,2,1,1,2,1,1,2,1,1,1,1,4,1,1,1,1,1,1,2,1,1,1,1,4};  /* length of each note */
int tempo_bells = 200;                                                                                                /* tempo of notes */
char names_bells[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };                                                      /* note names */
int tones_bells[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };                                                /* tone of each note */

/*  Fire Burning   */
int length_fire = 61;                                                                                                                             /* the number of notes */
char notes_fire[]="CCDEEE EEEEEDCDCa aEDE CCDEGECDEEEGEEEDCa aEDE ACBgACBgfcfedg";                                                                /* notes in the song */
int beats_fire[] = {1,1,1,2,2,2,2,1,1,1,1,1,1,1,1,2,2,1,1,1,2,1,4,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,2,1,8,2,2,2,2,2,2,2,2,2,2,2,2,4,4};   /* length of each note */
int tempo_fire = 200;                                                                                                                             /* tempo of notes */
char names_fire[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C','D','E','F','G' };                                                                   /* note names */
int tones_fire[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 850,760,716,638};                                                             /* tone of each note */

/* other variables */
TM1637 tm1637(CLK, DIO);                  /* 4-Digit Display object */
Ultrasonic ultrasonic(ULTRASONIC_PIN);    /* Ultrasonic Ranger object */
int distance = 0;                         /* variable to store the distance to obstacles in front */
int8_t bits[4] = {0};                     /* array to store the single bits of the value */
int analog_value = 0;                     /* variable to store the value coming from Moisture Sensor */
int8_t moist_bits[4] = {0};               /* array to store the single digits of the value */
DHT dht(TEMP_HUMI_PIN, DHT22);            /* temperature&humidity sensor object */
int8_t t_bits[2] = {0};                   /* array to store the single bits of the temperature */
int8_t h_bits[2] = {0};                   /* array to store the single bits of the humidity */  
int sound_value = 0;                      /* variable to store the value coming from sound sensor */
int rotary_value = 0;                     /* variable to store the value coming from rotary angle sensor */
int display = 1;


/* the setup() method runs once, when the sketch starts */
void setup() 
{
    /* Initialize 4-Digit Display */
    tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);
    tm1637.point(POINT_ON);              /* light the clock point ":" */

    pinMode(LED, OUTPUT);  
    digitalWrite(LED, LOW);
    pinMode(BUZZER_PIN, OUTPUT);       
    //Serial.begin(9600); // msp430g2231 must use 4800

    dht.begin();                         /* initialize temperature humidity sensor */
}

/* the loop() method runs over and over again */
void loop() 
{   
    rotary_value = analogRead(ROTARY_ANGLE_P);      /* read the value from the rotary angle sensor */
    distance = ultrasonic.MeasureInCentimeters();   /* read the value from the ultrasonic distance sensor */  
    analog_value = analogRead(MOISTURE_PIN);        /* read the value from the moisture sensor */
    int _temperature = dht.readTemperature();       /* read the temperature value from the temperature_humidity sensor */
    int _humidity = dht.readHumidity();             /* read the humidity value from the temperature_humidity sensor */    
    sound_value = analogRead(SOUND_SENSOR);         /* read the value from the sound sensor */
    memset(bits, 0, 4);                             /* reset array before we use it */
    
    //Serial.println(distance);
    //Serial.println(sound_value);
    
    if (distance < 50.0){                            /* Play star song when distance value is less than 50. */
        //Serial.println(2);
        //play_fire();
        play_star();
    }
  
        
    if(sound_value > SOUND_THRESHOLD)                /* play birthday song if sound level is greater than sound threshold. */
    {
        digitalWrite(LED, HIGH); `
        play_bday();
    }

    
    if(analog_value > MOISTURE_THRESHOLD)            /* Play bells song if moisture level is greater than moisture threshold. */
    {
        play_bells();
        digitalWrite(LED, HIGH);
    }

    
    if (_temperature > TEMP_THRESHOLD) {             /* Play fire song if temperature is greater than temperature threshold.*/
        play_fire();
    }
    
    if (rotary_value < 1000 && rotary_value > 0) {
         for(int i = 3; i >= 0; i--) {
             /* Convert the value to individual decimal digits for display */
             bits[i] = distance % 10;
             distance = distance / 10;  
             tm1637.display(i, bits[i]);                 /* display on 4-Digit Display */
        
         }
    }
    

    
    
    /* Rotary Mode 1: Display Temperature and Humidity*/
    if (rotary_value < 2000 && rotary_value > 1001){
                                                      /* 4-Digit Display [0,1] is used to display temperature */
        t_bits[0] = _temperature % 10;                
        _temperature /= 10;         
        t_bits[1] = _temperature % 10;
                                                      /* 4-Digit Display [2,3] is used to display humidity */
        h_bits[0] = _humidity % 10;                   /* Convert the value to digits */
        _humidity /= 10;                             
        h_bits[1] = _humidity % 10;
    
        tm1637.display(1, t_bits[0]);                 /* Update each number on 4-Digit Display */
        tm1637.display(0, t_bits[1]);
        tm1637.display(3, h_bits[0]);
        tm1637.display(2, h_bits[1]);
    }

    memset(moist_bits, 0, 4);                             /* Reset array before we use it */
   
    /* Rotary Mode 2: Display Moisture */
    if (rotary_value < 3000 && rotary_value > 2001) {
      
      for(int i = 3; i >= 0; i--) {
          moist_bits[i] = analog_value % 10;                /* Convert the value to digits */
          analog_value = analog_value / 10;  
          tm1637.display(i, moist_bits[i]);                 /* Update each digit on 4-Digit Display */
          }
    }
    
    digitalWrite(LED, LOW);
    delay(10);
}

/* Functions below are Helper functions. They are called in the main function (void loop()) */

void play_star()
{
  /* Loop through each note and play each note */
  for(int i = 0; i < length_star; i++) 
  {
    if(notes_star[i] == ' ')                                       /* Pause for a given duration if the note is a space */
    {
      delay(beats_star[i] * tempo_star);
    } 
    else                                                           /* Otherwise, play the note for the duration indicated*/
    {
      playNote_star(notes_star[i], beats_star[i] * tempo_star);
    }
    delay(tempo_star / 2);                                         /* delay half a tempo between every two consecutive notes */
  }
}

void play_bday()
{
  /* Loop through each note and play each note */
  for(int i = 0; i < length_bday; i++) 
  {
    if(notes_bday[i] == ' ')                                       /* Pause for a given duration if the note is a space */
    {
      delay(beats_bday[i] * tempo_bday);
    } 
    else                                                           /* Otherwise, play the note for the duration indicated*/
    {
      playNote_bday(notes_bday[i], beats_bday[i] * tempo_bday);
    }
    delay(tempo_bday / 2);                                         /* delay half a tempo between every two consecutive notes */
  }
}

void play_bells()
{
  /* Loop through each note and play each note */
  for(int i = 0; i < length_bells; i++) 
  {
    if(notes_bells[i] == ' ')                                     /* Pause for a given duration if the note is a space */
    {
      delay(beats_bells[i] * tempo_bells);
    } 
    else                                                          /* Otherwise, play the note for the duration indicated*/
    {
      playNote_bells(notes_bells[i], beats_bells[i] * tempo_bells);
    }
    delay(tempo_bells / 2);                                       /* delay half a tempo between every two consecutive notes */
  }
}

void play_fire()
{
  /* Loop through each note and play each note */
  for(int i = 0; i < length_fire; i++) 
  {
    if(notes_fire[i] == ' ')                                      /* Pause for a given duration if the note is a space */
    {
      delay(beats_fire[i] * tempo_fire);
    } 
    else                                                          /* Otherwise, play the note for the duration indicated*/
    {
      playNote_fire(notes_fire[i], beats_fire[i] * tempo_fire);
    }
    delay(tempo_fire / 2);                                        /* delay half a tempo between every two consecutive notes */
  }
}

/* Functions below are 2nd-order HELPER functions. They are called in HELPER functions (play_xxxx()) defined above */

void playNote_star(char note, int duration) 
{
  for (int i = 0; i < 8; i++)  
  {
    if (names_star[i] == note)                          /* For every note, play the note with its corresponding tone */
    {
      playTone_star(tones_bday[i], duration);         
    }
  }
}

void playNote_bday(char note, int duration) 
{
  for (int i = 0; i < 12; i++) 
  {
    if (names_bday[i] == note)                          /* For every note, play the note with its corresponding tone */
    {
      playTone_bday(tones_bday[i], duration);
    }
  }
}

void playNote_bells(char note, int duration) 
{
  for (int i = 0; i < 8; i++) 
  {
    if (names_bells[i] == note)                        /* For every note, play the note with its corresponding tone */
    {
      playTone_bells(tones_bells[i], duration);
    }
  }
}

void playNote_fire(char note, int duration) 
{
  for (int i = 0; i < 8; i++) 
  {
    if (names_fire[i] == note)                        /* For every note, play the note with its corresponding tone */
    {
      playTone_fire(tones_fire[i], duration);
    }
  }
}

/* Functions below are 3rd-order HELPER functions. They are called in 2nd-order HELPER functions (playNote_xxxx()) defined above. */

void playTone_star(int tone, int duration) 
{
  for (long i = 0; i < duration * 1000L; i += tone * 2)     /* Set up tones to be played */
  {
    digitalWrite(BUZZER_PIN, HIGH);                         
    delayMicroseconds(tone);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(tone);
  }
}

void playTone_bday(int tone, int duration) 
{
  for (long i = 0; i < duration * 1000L; i += tone * 2)     /* Set up tones to be played */
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(tone);
  }
}

void playTone_bells(int tone, int duration) 
{
  for (long i = 0; i < duration * 1000L; i += tone * 2)     /* Set up tones to be played */
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(tone);
  }
}

void playTone_fire(int tone, int duration) 
{
  for (long i = 0; i < duration * 1000L; i += tone * 2)     /* Set up tones to be played */
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(tone);
  }
} 
