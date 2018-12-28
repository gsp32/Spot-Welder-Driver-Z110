
const int ledPin = 13;       
const int  zeroCrossPin = 2;    


boolean startButtonState = 0;
boolean lastStartButtonState = 0;

int buttonPushCounter = 0;   
int zeroCrossState = 0;         
int lastZeroCrossState = 0;     
int czasZgrzewaniaTriak = 4;
int czasZgrzewaniaTriak2 = 1;//wartość razy 10ms
int mocTriak = 100;       //
int mocAktualna = 50;  
int buttonValue1 = 0;
int potentiometerValue1 = 0;
int potentiometerValue2 = 0;
int timeValue1 = 20;
int pozwoleniePracy = 0;
int pozwoleniePracy2 = 0;
int krokCyklu=1;
int menu1 = 0;
int menu2 = 0;
int bufor1 =0;
int opoznienieZalaczenia = 0;
int podwojnyZgrzew =0;




void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  //attachInterrupt(0, zero_crosss_int, RISING);
  pinMode(3, INPUT);//START BUTTON
  pinMode(4, OUTPUT);//MOC3021
  pinMode(11, OUTPUT); //buzzer
  pinMode(zeroCrossPin, INPUT);
  digitalWrite(3, LOW);
  pinMode(A7, INPUT_PULLUP);  //FIRST POTENTIOMETER
  pinMode(A6, INPUT_PULLUP); // SECOND POTENTIOMETER

}/*--(end setup )---*/



void loop() 
{
  potentiometerValue1 = analogRead(A7);
  potentiometerValue2 = analogRead(A6);

  czasZgrzewaniaTriak = (potentiometerValue1/50) + (potentiometerValue2/2);


  /*
  //start zgrzewania
   if (digitalRead(7) == HIGH){
   while(digitalRead(7)== HIGH){
   delay(100);
   }
   pozwoleniePracy = 1;
   }
   */




  startButtonState = digitalRead(3);
  if (startButtonState != lastStartButtonState) {
    if (startButtonState == HIGH) {
      delay(100);
      pozwoleniePracy = 1;
      if (podwojnyZgrzew == 1) {
        pozwoleniePracy2 = 1;
      }
    }
  }
  lastStartButtonState = startButtonState;


  krokCyklu=1;
  //zgrzew właściwy
  while(pozwoleniePracy == 1){
    zeroCrossState = digitalRead(zeroCrossPin);
    if (zeroCrossState != lastZeroCrossState) {
      if (zeroCrossState == HIGH) {
        delayMicroseconds(6000);
        digitalWrite(4, HIGH);
        digitalWrite(13, HIGH);   
        delayMicroseconds(100);   
        krokCyklu++;
      } 
    }
    lastZeroCrossState = zeroCrossState;
    if (krokCyklu >czasZgrzewaniaTriak){
      pozwoleniePracy = 0;
      digitalWrite(4, LOW);
      digitalWrite(13, LOW);
      tone(11,2600);
      delay(20);   
      noTone(11);  
      delay(1000);

    }
  }

}/* --(end main loop )-- */


/* ( THE END ) */

































