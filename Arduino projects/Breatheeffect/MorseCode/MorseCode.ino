const int ledPin = 12;
String newNum = "";
int len = 0;
char ch;
int delayNum = 100;

/*
 * . = 100ms dot()
 * - = 300ms dash()
 * space between parts in a letter = 100ms insideSpace()
 * space between two letters in a word = 300ms outsideSpace()
 */

void dot()
{
Serial.print(".");
digitalWrite(ledPin, HIGH);
delay(delayNum);
}

void dash()
{
Serial.print("-");
digitalWrite(ledPin, HIGH);
delay(delayNum * 3);
}

void insideSpace(){
  digitalWrite(ledPin, LOW);
  delay(delayNum);
  Serial.print(" ");
}

void outsideSpace(){
  digitalWrite(ledPin, LOW);
  delay(delayNum * 3);
  Serial.print("   ");
}


/* morse code mapping*/
void one()
{
dot();
insideSpace();
dash();
insideSpace();
dash();
insideSpace();
dash();
insideSpace();
dash();
outsideSpace();
}
void two()
{
dot();
insideSpace();
dot();
insideSpace();
dash();
insideSpace();
dash();
insideSpace();
dash();
outsideSpace();
}
void three()
{
dot();
insideSpace();
dot();
insideSpace();
dot();
insideSpace();
dash();
insideSpace();
dash();
outsideSpace();
}
void four()
{
dot();
insideSpace();
dot();
insideSpace();
dot();
insideSpace();
dot();
insideSpace();
dash();
outsideSpace();
}
void five()
{
dot();
insideSpace();
dot();
insideSpace();
dot();
insideSpace();
dot();
insideSpace();
dot();
outsideSpace();
}
void six()
{
dash();
insideSpace();
dot();
insideSpace();
dot();
insideSpace();
dot();
insideSpace();
dot();
outsideSpace();
}
void seven()
{
dash();
insideSpace();
dash();
insideSpace();
dot();
insideSpace();
dot();
insideSpace();
dot();
outsideSpace();
}
void eight()
{
dash();
insideSpace();
dash();
insideSpace();
dash();
insideSpace();
dot();
insideSpace();
dot();
outsideSpace();
}
void nine()
{
dash();
insideSpace();
dash();
insideSpace();
dash();
insideSpace();
dash();
insideSpace();
dot();
outsideSpace();
}
void zero()
{
dash();
insideSpace();
dash();
insideSpace();
dash();
insideSpace();
dash();
insideSpace();
dash();
outsideSpace();
}

void morse()
{ 
switch(ch) {

  case '0' :
              zero();
              break;
 
  case '1' :
              one();
              break;

  case '2' :
              two();
              break;

  case '3' :
              three();
              break;

  case '4' :
              four();
              break;

  case '5' :
              five();
              break;

  case '6' :
              six();
              break;

  case '7' :
              seven();
              break;

  case '8' :
              eight();
              break;

  case '9' :
              nine();
              break;

  case ' ' :
              delay(delayNum*3);
              break;
  
  default :
  Serial.println("");
}
}


/*convert one character at a time to morse*/
void String2Ch()
{
len = newNum.length();
for (int i = 0; i < len-1; i++)
{
ch = newNum.charAt(i);
morse();
}
}


/*setup*/
void setup() {
Serial.begin(9600);
pinMode(ledPin, OUTPUT);
}


/*loop*/
void loop() {
if(Serial.available()>0)
{
newNum = Serial.readString();
Serial.print(newNum);
Serial.print(" = ");
String2Ch();
Serial.println("");
}
delay(1000);
}
