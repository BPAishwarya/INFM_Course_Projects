#include <stdlib.h>
int randomNumber;      //the random number that the user is trying to guess
//const char *chGuess;
String guess = "";   
String newChar = "";

void setup()
{
 
  randomSeed(analogRead(7));        //seed the random number generator based on a 'random' reading from an unconnected analog input
  randomNumber = random(100);   //generate a random number
 
  Serial.begin(9600); //start the Serial port at a baud rate of 9600 bits per second (bps)
  
  //print prompts  
  Serial.println("Hello! Please enter a number between 0 and 100. "); 
  
  //uncomment the next 2 lines if you want to see the random number for debugging
  Serial.print("Random Number: "); 
  Serial.println(randomNumber); 
 
  
} //setup() is done, go to loop()

void genRandom(){
    randomSeed(analogRead(7));        //seed the random number generator based on a 'random' reading from an unconnected analog input
    randomNumber = random(100);   //generate a random number
    Serial.println("Hello! Please enter a number between 0 and 100. "); 
}

void loop()
{
  //check is there are any characters incoming on the serial buffer.
  if(Serial.available() > 0)
  {

    
    guess = Serial.parseInt(); //Serial.parseInt is an advanced version of Serial.read(). Serial.parseInt() reads several characters and trys to create an integer out of them. This operation takes longer than a standard Serial.read() which just reads one character at a time
    delay(100);
    unsigned char text[0] = guess;
  int intValue;
  
  sscanf(text,"%04d",&intValue);
   Serial.println(intValue);
    
//    Serial.print("You Guessed: ");  //print static text
//    Serial.println(guess); //print the data that was recieved
// 
//    if(guess == randomNumber)
//    {
//      Serial.println("You guessed correctly!");  //print static text
//      Serial.flush();
//      delay(1000);
//      Serial.println("Press any character to continue");
//      genRandom();
//    }
//    if(guess > randomNumber)
//    {
//      Serial.println("You Guess is too high");  //print static text
//    }    
//    else if(guess < randomNumber)
//    {
//      Serial.println("Here in low");
//      Serial.println("You Guess is too low");  //print static text
//    }
//     
  }

  //Serial.println("Outside if loop");
 delay(100);
}//go back to the first line in loop()  
