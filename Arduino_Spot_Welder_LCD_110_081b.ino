


#include <Wire.h>
// Get the LCD I2C Library here:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <LiquidCrystal_I2C.h>
#include<EEPROM.h>


/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol//////
//dla wersji bez A    0x20
LiquidCrystal_I2C lcd(0x38, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);  // Set the LCD I2C address

/*-----( Declare Variables )-----*/
// this constant won't change:
const int ledPin = 13;       // the pin that t      3-he LED is attached to
const int  zeroCrossPin = 2;    // the pin that the pushbutton is attached to
//const int  startButtonPin = ??
//const int  beepOutputPin =??


// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
boolean zeroCrossState = 0;         // current state of the button
boolean lastZeroCrossState = 0;     // previous state of the button
boolean startButtonState = 0;
boolean lastStartButtonState = 0;
boolean setButtonState = 0;
boolean lastSetButtonState = 0;

int czasZgrzewaniaTriak = 4;
int czasZgrzewaniaTriak2 = 1;//wartość razy 10ms
//int mocTriak = 100;       //moc w procentach
int mocAktualna = 50;
int timeMidWeld = 100;
int buttonValue1 = 0;
int buttonValue2 = 0;
int timeValue1 = 20;
bool pozwoleniePracy = 0;
bool pozwoleniePracy2 = 0;
int krokCyklu = 1;
int menu1 = 0;
int menu2 = 0;
int bufor1 = 0;
int opoznienieZalaczenia = 0;
bool podwojnyZgrzew = 0;

/*adresy zmiennych eeprom
 0 - podwojny zgrzew
 1 - czas 1 zg
 2 - czas 2 zg
 3 - czas przerwy
 4- opoznienie zalaczenia
 */

/*
a6
 967 up1
 926 dn1
 888 up2
 853 dn2
 
 a7
 967 esc
 926 set
 
 
 */

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  //attachInterrupt(0, zero_crosss_int, RISING);
  pinMode(4, OUTPUT);
  pinMode(3, INPUT);
  pinMode(11, OUTPUT); //buzzer
  pinMode(zeroCrossPin, INPUT);
  //digitalWrite(3, LOW);
  pinMode(A7, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);
  lcd.begin(8, 2);  // initialize the lcd for 8 chars 2 lines, turn on backlight
  lcd.clear();
  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("EMSITECH");
  lcd.setCursor(0, 1);
  lcd.print("v0.80b");
  delay(2000);
  lcd.clear();
  podwojnyZgrzew = EEPROM.read(0);
  czasZgrzewaniaTriak = EEPROM.read(1);
  czasZgrzewaniaTriak2 = EEPROM.read(2);


}/*--(end setup )---*/

void getSetBTN(){
  if (analogRead(A7) >= 910 && analogRead(A7) <= 940) {
    setButtonState = 1;
  }
  else
  {
    setButtonState = 0;
  }
}

void loop()
{
  buttonValue1 = analogRead(A6);
  buttonValue2 = analogRead(A7);
  //wyswietlanie menu
  lcd.setCursor(0, 0);
  lcd.print("1:");
  lcd.print((czasZgrzewaniaTriak * 10));
  lcd.print("ms");

  lcd.setCursor(0, 1);
  lcd.print("2:");
  if (podwojnyZgrzew == 1) {
    lcd.print((czasZgrzewaniaTriak2 * 10));
    lcd.print("ms");

  }
  else {
    lcd.print("  OFF");
  }

  //menu jakies ustalające parametry
  //-------------------------------------------------------------------------------------------------------------------------------------
  getSetBTN();
  if (setButtonState != lastSetButtonState) {
    if (setButtonState == 1) {   
      lcd.clear();
      lastSetButtonState = setButtonState;    
      //ustawienie czasu ZGRZEWU NR 1
      menu1 = 1;
      bufor1 = czasZgrzewaniaTriak;
      while (menu1 == 1) {
        lcd.setCursor(0, 0);
        lcd.print("1: ");
        lcd.print((bufor1 * 10));
        lcd.print("ms");
        lcd.setCursor(0, 1);
        lcd.print("Czas +/-");
        if (analogRead(A6) >= 950 && analogRead(A6) <= 980) {
          bufor1++;
        }
        else if (analogRead(A6) >= 910 && analogRead(A6) <= 940) {
          if (bufor1 > 1) {
            bufor1--;
          }
        }
        if (analogRead(A6) >= 870 && analogRead(A6) <= 900) {
          bufor1 = bufor1 + 10;
        }
        else if (analogRead(A6) >= 840 && analogRead(A6) <= 865) {
          if (bufor1 >= 11) {
            bufor1 = bufor1 - 10;
          }
          else {
            bufor1 = 1;
          }
        }
        delay(150);
        getSetBTN();
        //wyjscie bez zapisu
        if (analogRead(A7) >= 960) {
          menu1 = 0;
        }
        else 
          if (setButtonState != lastSetButtonState) {
          if (setButtonState == 1) {
            delay(50);
            czasZgrzewaniaTriak = bufor1;
            menu1 = 0;
            lcd.setCursor(0, 1);
            lcd.print("Zapisuje");
            EEPROM.write(1, czasZgrzewaniaTriak);
            delay(150);
          }
        }
        lastSetButtonState = setButtonState;
        lcd.clear();
      }

      //ustawienie podwojnego zgrzewu  tralalalala
      menu1 = 1;
      bufor1 = podwojnyZgrzew;
      while (menu1 == 1) {
        lcd.setCursor(0, 0);
        lcd.print("2:");
        lcd.setCursor(0, 1);
        if (bufor1 == 1) {
          lcd.print("ON");
        }
        else {
          lcd.print("OFF");
        }
        if (analogRead(A6) >= 910 && analogRead(A6) <= 940) {
          if (bufor1 == 1) {
            bufor1--;
          }
        }
        else if (analogRead(A6) >= 950 && analogRead(A6) <= 980) {
          if (bufor1 == 0) {
            bufor1++;
          }
        }

        delay(150);
        getSetBTN();
        //wyjscie bez zapisu
        if (analogRead(A7) >= 960) {
          menu1 = 0;
        }
        else        
          if (setButtonState != lastSetButtonState) {
          if (setButtonState == 1) {
            delay(50);
            podwojnyZgrzew = bufor1;
            menu1 = 0;
            lcd.setCursor(0, 1);
            lcd.print("Zapisuje");
            EEPROM.write(0, podwojnyZgrzew);
            delay(150);
          }
        }
        lastSetButtonState = setButtonState;
        lcd.clear();
      }

      //koniec ustawienia włączenia zgrzewu2
      //ustawienie   czesci zgrzewu zgrzewu albo coś jakoś czymś gdzieś po coś
      //bredzisz...
      if (podwojnyZgrzew == 1) {
        menu1 = 1;
        bufor1 = czasZgrzewaniaTriak2;
        while (menu1 == 1) {
          lcd.setCursor(0, 0);
          lcd.print("2:");
          lcd.print((bufor1 * 10));
          lcd.print("ms");
          lcd.setCursor(0, 1);
          lcd.print("Czas +/-");
          if (analogRead(A6) >= 950 && analogRead(A6) <= 980) {
            bufor1++;
          }
          else if (analogRead(A6) >= 910 && analogRead(A6) <= 940) {
            if (bufor1 > 1) {
              bufor1--;
            }
          }
          if (analogRead(A6) >= 870 && analogRead(A6) <= 900) {
            bufor1 = bufor1 + 10;
          }
          else if (analogRead(A6) >= 840 && analogRead(A6) <= 865) {
            if (bufor1 >= 11) {
              bufor1 = bufor1 - 10;
            }
            else {
              bufor1 = 1;
            }
          }
          delay(150);
          getSetBTN();
          //wyjscie bez zapisu
          if (analogRead(A7) >= 960) {
            menu1 = 0;
          }
          else 
            if (setButtonState != lastSetButtonState) {
            if (setButtonState == 1) {
              delay(50);
              czasZgrzewaniaTriak2 = bufor1;
              menu1 = 0;
              lcd.setCursor(0, 1);
              lcd.print("Zapisuje");
              EEPROM.write(2, czasZgrzewaniaTriak2);
              delay(150);
            }
          }
          lastSetButtonState = setButtonState;
          lcd.clear();
        }
      }//chyba koniec ifa
    }//KONIEC GLOWNEGO IFA MENU 

  }
  lastSetButtonState = setButtonState;


  /*
    //start zgrzewania
   if (analogRead(A6) > 500) {
   startButtonState = 1
   else {
   startButtonState = 0
   }
   }
   */
  startButtonState = digitalRead(3);
  if (startButtonState != lastStartButtonState) {
    if (startButtonState == HIGH) {
      lcd.setCursor(0, 0);
      lcd.print("--------");
      //beeeeeeeeeeeeeeeeeeeeep
      tone(11,2400);
      delay(20);
      noTone(11);
      //      delay(100);
      //      tone(11,2600);
      //      delay(100);
      //      noTone(11);
      delay(100);
      //booooooooooooooooooop
      lcd.setCursor(0, 1);
      lcd.print("--****--");
      pozwoleniePracy = 1;
      if (podwojnyZgrzew == 1) {
        pozwoleniePracy2 = 1;
      }
    }
  }
  lastStartButtonState = startButtonState;




  krokCyklu = 1;
  //zgrzew właściwy
  //-----------------------------------------------------------------------------------------------------------
  while (pozwoleniePracy == 1) {
    zeroCrossState = digitalRead(zeroCrossPin);
    if (zeroCrossState != lastZeroCrossState) {
      if (zeroCrossState == HIGH) {
        delayMicroseconds(6000);
        digitalWrite(4, HIGH);
        delayMicroseconds(100);
        krokCyklu++;
        //lcd.setCursor(0, 1);
        //lcd.print(krokCyklu);
      }
    }
    lastZeroCrossState = zeroCrossState;
    if (krokCyklu > czasZgrzewaniaTriak) {
      pozwoleniePracy = 0;
      digitalWrite(4, LOW);
      delay(timeMidWeld);
      lcd.clear();
      tone(11,2600);
      delay(20);
      noTone(11);
      delay(20);

    }

  }
  krokCyklu = 1;

  //zgrzew2
  //-----------------------------------------------------------------------------------------------------------
  while (pozwoleniePracy2 == 1) {
    zeroCrossState = digitalRead(zeroCrossPin);
    if (zeroCrossState != lastZeroCrossState) {
      if (zeroCrossState == HIGH) {
        delayMicroseconds(6000);
        digitalWrite(4, HIGH);
        delayMicroseconds(100);
        krokCyklu++;
        //lcd.setCursor(0, 1);
        //lcd.print(krokCyklu);
      }
    }
    lastZeroCrossState = zeroCrossState;
    if (krokCyklu > czasZgrzewaniaTriak2) {
      pozwoleniePracy2 = 0;
      digitalWrite(4, LOW);
      delay(timeMidWeld);
      lcd.clear();
      tone(11,2600);
      delay(20);   
      noTone(11);
    }
  }

  // podwojny beep po zgrzewie

}/* --(end main loop )-- */


/* ( THE END ) */










/*
  startButtonState = digitalRead(5);
 if (startButtonState != lastStartButtonState) {
 if (startButtonState == HIGH) {
 delayMicroseconds(6000);
 }
 }
 lastStartButtonState = startButtonState;
 
 */
/*

 if (analogRead(A7) >= 910 && analogRead(A7) <= 940) {
 setButtonState = 1;
 }
 else
 {
 setButtonState = 0;
 }
 
 if (setButtonState != lastSetButtonState) {
 if (setButtonState == 1) {
 delay(50);
 }
 }
 lastSetButtonState = setButtonState;
 
 */

























