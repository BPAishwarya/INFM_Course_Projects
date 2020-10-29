int randomNumber;
String inString = "";
const int ledPin = 11;
//int newNum;
String inNum = "";
int playerNumber;
int newGuess;
#define MAX_GUESS 5

unsigned long currentMillis;
const unsigned long period = 10000;


#include <time.h>

#include "pitches.h"


// notes in the Mario melody:
int melodyMario[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int melodyGOT[] = {
  NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void playStartMelody(){


for(int k = 0; k < 16; k++)
{
  tone(3, melodyMario[k], 200);
  delay(100);
  noTone(3);
}
}

void playWinningMelody(){

for(int i=0; i<2; i++)
    {
    tone(3, NOTE_G4);
    delay(500);
    noTone(3);
    tone(3, NOTE_C4);
    delay(500);
    noTone(3);
    tone(3, NOTE_E4);
    delay(250);
    noTone(3);
    tone(3, NOTE_F4);
    delay(250);
    noTone(3);
    }
Serial.println("Congrats!! you guessed within 10s!!!!!!!!");
 tone(3, NOTE_G4);
        delay(500);
        noTone(3);
        tone(3, NOTE_C4);
        delay(500);
        tone(3, NOTE_DS4);
        delay(250);
        noTone(3);
        tone(3, NOTE_F4);
        delay(250);
        noTone(3);
        tone(3, NOTE_D4);
        delay(500);
        noTone(3);
for(int i=0; i<3; i++)
    {
    tone(3, NOTE_G3);
    delay(500);
    noTone(3);
    tone(3, NOTE_AS3);
    delay(250);
    noTone(3);
    tone(3, NOTE_C4);
    delay(250);
    noTone(3);
    tone(3, NOTE_D4);
    delay(500);
    noTone(3);
    }
        
}

void playMarioDeath(){
//Begin playing Death sound effect & song
tone(3,262,31);
delay(40);
noTone(3);
tone(3,277,31);
delay(40);
noTone(3);
tone(3,294,62);
delay(80);
noTone(3);
tone(3,0,250);
delay(325);
noTone(3);
tone(3,0,500);
delay(650);
noTone(3);
tone(3,247,125);
delay(162);
noTone(3);
tone(3,349,125);
delay(162);
noTone(3);
Serial.println("Sorry! you used up maximum number of tries. \n");
tone(3,0,125);
delay(162);
noTone(3);
tone(3,349,125);
delay(162);
noTone(3);
tone(3,349,166);
delay(215);
noTone(3);
tone(3,330,166);
delay(215);
noTone(3);
tone(3,294,166);
delay(215);
noTone(3);
tone(3,262,125);
delay(162);
noTone(3);
tone(3,165,125);
delay(162);
noTone(3);
tone(3,0,125);
delay(162);
noTone(3);
tone(3,165,125);
delay(162);
noTone(3);
tone(3,131,125);
delay(162);
noTone(3);
}

void playStarwars(){
  tone(3,220,500);
delay(501);
tone(3,220,500);
delay(501);
tone(3,220,500);
delay(501);
tone(3,174,375);
delay(376);
tone(3,261,125);
delay(126);
tone(3,220,500);
delay(501);
tone(3,174,375);
delay(376);
tone(3,261,125);
delay(126);
tone(3,220,1000);
delay(1001);
noTone(3);
}

//******* Generate Random number

void genRandom() {
  randomSeed(analogRead(7));        //seed the random number generator based on a 'random' reading from an unconnected analog input
  randomNumber = random(20);   //generate a random number
  //Serial.println("Hello! Please enter a number between 0 and 50. ");
  delay(500);
}




//******* Check number proximity

int checkProximity() {
  int guessNum = newGuess;
  int low = (randomNumber - 2);
  int high = (randomNumber + 2);
  delay(1000);
  if (guessNum == randomNumber) {
    Serial.println("Correct!!!");
    delay(500);
    return 0;
  }
  else if ( guessNum >= low && guessNum <= high ) {
    Serial.println(" You're Close!");
    tone(3,3000,125);
    delay(500);
    noTone(3);
    delay(500);
    return 1;
  }
  else {
    Serial.println("Too far!");
    tone(3,200,125);
    delay(500);
    noTone(3);
    delay(500);
    return 1;
  }

}



//******* Convert string to Integer

void convertToInt() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      Serial.print("You guessed:");
      Serial.println(inString.toInt());
      playerNumber = inString.toInt();

      // clear the string for new input:
      inString = "";
    }
  }
}

//******* Set Led value

int lightUp(int val) {
  int analogVal = val;
  Serial.println(analogVal);
  delay(100);
  //  int mapValue = constrain(map(analogVal, 0, 1023, 0, 255), 0, 100);
  analogWrite(ledPin, analogVal);
  delay(1000);
}



//******* Guess Loop main

int guess(int num) {
  int i, y;
  unsigned long periodStartMillis = millis();
//    Serial.println("Start Millis: ");
//    Serial.println(periodStartMillis);
  for ( i = 0; i < MAX_GUESS; i++) {

    
    Serial.println("Guess a number between 0 and 20.");
    while (Serial.available() == 0) {}
    delay(500);
    
    //***************convert string to int
    convertToInt();
    delay(500);

//***Code to check if value entered is above the max range ----- currently causing problems in the code
//    if( playerNumber > 20 ){
//      Serial.println("Guess again below 20!!");
//      while (Serial.available() == 0) {}
//      delay(500);
//      convertToInt();
//      delay(500);
//    }


    newGuess = playerNumber; //set the ledPin value
    lightUp(newGuess);
    //Serial.println("lit the led with your number");
    delay(500);

    //****check if guessed number is close or far
    y = checkProximity();
    if (y == 0) {
      currentMillis = millis();
//      unsigned long temp1 = currentMillis - periodStartMillis; 
//      Serial.println("Difference is: ");
//      Serial.println(temp1);
      if( currentMillis - periodStartMillis <= period){
        playWinningMelody();
        i = 10;
      }
      else{
        i = 8;
      }
      break;
    }
    delay(500);

  }
  
  return i;
}


void loop() {
  // put your main code here, to run repeatedly:
  
  String answer = "";
  int x;
 char newCh;

  do {
     
    //Start new game
    playStartMelody();
    Serial.println("**********************************");
    Serial.println("Instructions");
    Serial.println(" ->The Led light brightness indicates a value between 0-20");
    Serial.println(" ->You must guess a number between 0, 20 inclusive");
    delay(2000);
    Serial.println(" ->When your number is + or - 2 digits away from my number, You will hear this");
    delay(2000);
     tone(3,3000,125);
    delay(500);
    noTone(3);
    delay(500);
    Serial.println(" ->If your number is farther away from my number, you will hear this");
    delay(2000);
    tone(3,300,125);
    delay(500);
    noTone(3);
    delay(500);
    Serial.println("**********************************");
    Serial.println("Let's begin a new game");
    Serial.println("**********************************");
    

    //set the random number
    genRandom(); //set the random number
    
    //initialize the ledPin value
    lightUp(randomNumber);
    // Serial.println("Just lit the led with random");

    // Start guessing
    x = guess(randomNumber);
    

    if (x == MAX_GUESS) {
    //Serial.println("Sorry! you used up maximum number of tries. \n");
    playMarioDeath();
    }
    else if (x == 8) {
      Serial.println("Congratulations you won!!!");
      playStarwars();
    }

    Serial.println("");
    Serial.println("************************");
    Serial.println("Want to play again? Enter any key \n");
    Serial.println("************************");
    while (Serial.available() == 0) {}
    delay(1000);
    
    // To flush the newline character
    
    answer = Serial.readString();
    Serial.println(answer);
    delay(1000);
   
    int len = answer.length();
    for (int i = 0; i < len-1; i++)
    {
    newCh = answer.charAt(i);
    
    }

//    bool check = (newCh == 'n');
//    Serial.println(check);
    
  } while ( newCh != 'n' || newCh != 'N' );
  
  delay(5000);
  Serial.println("No");
  Serial.println("outside do while");

  delay(3000);

  //***** End of code
}
