//appel bibliotheque lcd et des capteurs
#include <LiquidCrystal_I2C.h>                // bibliotheque liquidCrystal I2C
#include <Wire.h>

#include <DHT.h>
#include <DHT_U.h>                            // bibliotheque DHT22
#include <Adafruit_Sensor.h>

// configuration du capteur DHT22
#define DHTPIN A2                             // initialise l'entre analogique
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
        double humidSol;   //information a affiché en %
        double lectureSol; //valeur en sortie de capteur
        lectureSol = analogRead(A1);
        humidSol =100-((lectureSol*100)/642);

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
        lcd.print(humidSol);
        lcd.print(" %");
        delay(1000);
          
        if (lectureSol>=428)
          {
            digitalWrite(red,HIGH); // allumage led rouge      
            // Condition d'arrosage
            do 
              {
                digitalWrite(green,LOW);
                digitalWrite(red,HIGH); // allumage led rouge
                //declenchement du relais de la pompe
                digitalWrite(pompe,LOW);
                //temoin lumineux arrosage en cours led bleu
                digitalWrite(blue,HIGH);
                delay(1000);
                //lecture capteur de sol
                lectureSol = analogRead(A1);
                lcd.init();
                lcd.setCursor(0,0);
                lcd.print("hum sol :");
                lcd.setCursor(0,1);
                lcd.print(humidSol);
                lcd.print(" %");
             } while ( lectureSol >= 428 );
          }
        digitalWrite(blue,LOW); // eteint led bleu
        digitalWrite(red,LOW); // eteint led rouge
        digitalWrite(pompe,HIGH); // pompe eteinte

        if(digitalRead(green)==HIGH)
          {
            digitalWrite(pompe,LOW);
            delay(5000);
            digitalWrite(green,LOW);
            digitalWrite(pompe,HIGH);
          }
                      
    }




//declaration fonction
void arrosageManuel ()
  {
            // declenchement du relais de la pompe
            digitalWrite(pompe,LOW);
            // temoin lumineux arrosage en cours
            digitalWrite(green,HIGH);
  }






    /*pensez a l'ajout du wifi ainsi que les differentes electrovane pour alimenter plusieur secteur
    */
