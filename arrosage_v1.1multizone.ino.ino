//appel bibliotheque lcd et des capteurs
#include <LiquidCrystal_I2C.h>                // bibliotheque liquidCrystal I2C
#include <Wire.h>

#include <DHT.h>
#include <DHT_U.h>                            // bibliotheque DHT22
#include <Adafruit_Sensor.h>

// configuration du capteur DHT22
#define DHTPIN A5                             // initialise l'entre analogique
#define DHTTYPE DHT22                         // defini le type de capteur

DHT dht(DHTPIN, DHTTYPE);                      //defini l'objet DHT
LiquidCrystal_I2C lcd(0x27,16,2);              //defini l'objet LCD

// initialisation du bouton poussoir
int monBouton=2;

// definition des led
int red=9;
int blue=11;
int green=10;

// definition de la pompe
int pompe=13;

// definition sortie electrovanne
int zone1=7;
int zone2=6;
int zone3=5;

    double humidSolZ1;   //information a affiché en %
    double traitementZ1;
    double lectureSolZ1; //valeur en sortie de capteur

    double humidSolZ2;   //information a affiché en %
    double traitementZ2;
    double lectureSolZ2; //valeur en sortie de capteur

    double humidSolZ3;   //information a affiché en %
    double traitementZ3;
    double lectureSolZ3; //valeur en sortie de capteur

        
void setup()
    {   
        //mise en route du capteur DHT22
        dht.begin();
        // initialisation de la sortie du relais
        pinMode(pompe,OUTPUT);
        // initialisation bouton poussoir
        pinMode(monBouton,INPUT_PULLUP);
        // initialisation des LED
        pinMode(blue,OUTPUT); // LED arrosage auto en cours (bleu)
        pinMode(green,OUTPUT); // LED arrosage manuelle en cours (vert)
        pinMode(red,OUTPUT);  // LED terre seche (rouge)
        // initialisation des sortie electrovanne
        pinMode(zone1,OUTPUT);
        pinMode(zone2,OUTPUT);
        pinMode(zone3,OUTPUT);

        // initialisation de l'affiche LCD
        lcd.init();
        lcd.init();
        lcd.backlight();
        lcd.setCursor(0,0);
        lcd.print("Arrosage Auto");

        // interruption pour arrosage manuel
        attachInterrupt(0,arrosageManuel,CHANGE);

        // mise a l'etat bas de toutes les pins numerique
        digitalWrite(pompe,HIGH);
        digitalWrite(blue,LOW);
        digitalWrite(red,LOW);
        digitalWrite(green,LOW);
    }

void loop()
    {
        // on recupere la temperature de l'air
        float dtempExt = dht.readTemperature();
        
        // on recupere l'humidite de l'air
        float dhumidExt = dht.readHumidity();
        
        // on recupere l'humiditée du sol
        moistZone();

        // affichage temperature LCD.
        lcd.init();
        lcd.setCursor(0,0);
        lcd.print("temperature :");
        lcd.setCursor(0,1);
        lcd.print(dtempExt);
        lcd.print(" deg C");
        delay(1000);
          
        // affichage humiditée exterieur LCD
        lcd.init();
        lcd.setCursor(0,0);
        lcd.print("hum ambiant :");
        lcd.setCursor(0,1);
        lcd.print(dhumidExt);
        lcd.print(" %");
        delay(1000);

        // affichage humititée sol LCD
        lcd.init();
        lcd.setCursor(0,0);
        lcd.print("hum sol :");
        lcd.setCursor(0,1);
        lcd.print(humidSolZ1);
        lcd.print(" %");
        delay(1000);

        lcd.init();
        lcd.setCursor(0,0);
        lcd.print("hum sol :");
        lcd.setCursor(0,1);
        lcd.print(humidSolZ2);
        lcd.print(" %");
        delay(1000);

        lcd.init();
        lcd.setCursor(0,0);
        lcd.print("hum sol :");
        lcd.setCursor(0,1);
        lcd.print(humidSolZ3);
        lcd.print(" %");
        delay(1000);
          
        if (lectureSolZ1>=428)
          {
            arrosageAutoZ1();
          }
        if (lectureSolZ2>=428)
          {
            arrosageAutoZ2();
          }
        if (lectureSolZ3>=428)
          {
            arrosageAutoZ3();
          }
        digitalWrite(blue,LOW); // eteint led bleu
        digitalWrite(red,LOW); // eteint led rouge
        digitalWrite(pompe,HIGH); // pompe eteinte

        if(digitalRead(green)==HIGH)
          {
            digitalWrite(pompe,LOW);
            digitalWrite(zone1,LOW);
            digitalWrite(zone2,LOW);
            digitalWrite(zone3,LOW);
            delay(5000);
            digitalWrite(green,LOW);
            digitalWrite(pompe,HIGH);
            digitalWrite(zone1,HIGH);
            digitalWrite(zone2,HIGH);
            digitalWrite(zone3,HIGH);
          }
                      
    }




//declaration fonction
void arrosageManuel ()
  {
    // declenchement du relais de la pompe et des electrovannes
    digitalWrite(pompe,LOW);
    digitalWrite(zone1,LOW);
    digitalWrite(zone2,LOW);
    digitalWrite(zone3,LOW);
    // temoin lumineux arrosage en cours
    digitalWrite(green,HIGH);
  }

void moistZone()
  {
    // lecture humidite du sol en zone 1
    /*double humidSolZ1;   //information a affiché en %
    double traitementZ1;
    double lectureSolZ1;*/ //valeur en sortie de capteur
    lectureSolZ1 = analogRead(A1);
    traitementZ1 = (lectureSolZ1*100)/642;
    humidSolZ1 =100-traitementZ1;

    // lecture humidite du sol en zone 1
    /*double humidSolZ2;   //information a affiché en %
    double traitementZ2;
    double lectureSolZ2;*/ //valeur en sortie de capteur
    lectureSolZ2 = analogRead(A2);
    traitementZ2 = (lectureSolZ2*100)/642;
    humidSolZ2 =100-traitementZ2;

   // lecture humidite du sol en zone 1
   /*double humidSolZ3;   //information a affiché en %
   double traitementZ3;
   double lectureSolZ3;*/ //valeur en sortie de capteur
   lectureSolZ3 = analogRead(A3);
   traitementZ3 = (lectureSolZ3*100)/642;
   humidSolZ3 =100-traitementZ3;
  }

void arrosageAutoZ1()
  {
    digitalWrite(red,HIGH); // allumage led rouge      
            // Condition d'arrosage
            do 
              {
                digitalWrite(green,LOW);
                digitalWrite(red,HIGH); // allumage led rouge
                //declenchement du relais de la pompe
                digitalWrite(pompe,LOW);
                digitalWrite(zone1,LOW);
                //temoin lumineux arrosage en cours led bleu
                digitalWrite(blue,HIGH);
                delay(1000);
                //lecture capteur de sol
                lectureSolZ1 = analogRead(A1);
                lcd.init();
                lcd.setCursor(0,0);
                lcd.print("hum sol zone1:");
                lcd.setCursor(0,1);
                lcd.print(humidSolZ1);
                lcd.print(" %");
             } while ( lectureSolZ1 >= 428 );
  }

void arrosageAutoZ2()
  {
    digitalWrite(red,HIGH); // allumage led rouge      
            // Condition d'arrosage
            do 
              {
                digitalWrite(green,LOW);
                digitalWrite(red,HIGH); // allumage led rouge
                //declenchement du relais de la pompe et electrovanne
                digitalWrite(pompe,LOW);
                digitalWrite(zone2,LOW);
                //temoin lumineux arrosage en cours led bleu
                digitalWrite(blue,HIGH);
                delay(1000);
                //lecture capteur de sol
                lectureSolZ2 = analogRead(A2);
                lcd.init();
                lcd.setCursor(0,0);
                lcd.print("hum sol zone2:");
                lcd.setCursor(0,1);
                lcd.print(humidSolZ2);
                lcd.print(" %");
             } while ( lectureSolZ2 >= 428 );
  }

void arrosageAutoZ3()
  {
    digitalWrite(red,HIGH); // allumage led rouge      
            // Condition d'arrosage
            do 
              {
                digitalWrite(green,LOW);
                digitalWrite(red,HIGH); // allumage led rouge
                //declenchement du relais de la pompe
                digitalWrite(pompe,LOW);
                digitalWrite(zone3,LOW);
                //temoin lumineux arrosage en cours led bleu
                digitalWrite(blue,HIGH);
                delay(1000);
                //lecture capteur de sol
                lectureSolZ3 = analogRead(A3);
                lcd.init();
                lcd.setCursor(0,0);
                lcd.print("hum sol zone3 :");
                lcd.setCursor(0,1);
                lcd.print(humidSolZ3);
                lcd.print(" %");
             } while ( lectureSolZ3 >= 428 );
  }
