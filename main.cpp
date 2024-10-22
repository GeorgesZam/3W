#include <Arduino.h>
#include "3W.h"

// Définition des macros
#define BOUTON_ROUGE 2 // Port du bouton rouge
#define BOUTON_VERT 3 // Port du bouton vert
#define PHOTORESISTANCE A0 // Port de la photorésistance

// Utiliser pour basculer entre chaque mode
#define CONFIGURATION 0   
#define STANDARD 1
#define ECONOMIQUE 2
#define MAINTENANCE 3

// Structure pour les capteurs
typedef struct {
    float temperatureAir;
    float hygrometrie;
    int lumiere;
} Capteurs;

// Déclaration des variables globales
Capteurs capteurs;
int modeCourant = STANDARD;
volatile bool boutonAppuye = false;
unsigned long debutAppuiBouton = 0;

// Prototypes des fonctions
void modeConfig();
void modeStandard();
void modeEco();
void modeMaint();
Capteurs lireCapteurs();
String lireHeure();
void sauvegarderCSV(Capteurs capteurs, String time);
void  onButtonPress();

// Fonction pour obtenir les lectures des capteurs 
Capteurs get_data() {
    Capteurs data;
    data.lumiere = analogRead(PHOTORESISTANCE); // Lecture de la photorésistance
    data.temperatureAir = dht.readTemperature(); // Lecture de la température
    data.hygrometrie = dht.readHumidity(); // Lecture de l'humidité
    return data;
}

// Fonction pour obtenir l'heure actuelle 
String get_time() {
    DateTime now = rtc.now();  // Obtenir l'heure actuelle depuis le RTC
    String time = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " +
                  String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    return time;
}

// Fonction pour sauvegarder les données dans un fichier CSV 
void save_data_csv(Capteurs capteurs, String time) {
    File dataFile = SD.open("data.csv", FILE_WRITE);
    if (dataFile) {
        dataFile.print(time); // Sauvegarder le temps
        dataFile.print(",");
        dataFile.print(capteurs.temperatureAir); // Sauvegarder la température
        dataFile.print(",");
        dataFile.print(capteurs.hygrometrie); // Sauvegarder l'humidité
        dataFile.print(",");
        dataFile.println(capteurs.lumiere); // Sauvegarder la luminosité
        dataFile.close();
    } else {
        Serial.println("Erreur d'ouverture du fichier");
    }
}

// Fonctions pour les différents modes
void modeConfiguration() {
    Serial.println("Mode Configuration activé");
    // Logique pour le mode configuration
}

void modeStandard() {
    Serial.println("Mode Standard activé");
    // Logique pour le mode standard
}

void modeEconomique() {
    Serial.println("Mode Économique activé");
    // Logique pour le mode économique
}

void modeMaintenance() {
    Serial.println("Mode Maintenance activé");
    // Logique pour le mode maintenance
}

// Fonction d'interruption pour gérer l'appui sur le bouton
void onButtonPress() {
    boutonAppuye = true;
}

void setup() {
    Serial.begin(9600);
    pinMode(BOUTON_ROUGE, INPUT_PULLUP);
    pinMode(BOUTON_VERT, INPUT);
    attachInterrupt(digitalPinToInterrupt(BOUTON_ROUGE), onButtonPress, FALLING);
    // Initialisation des capteurs et modules (e.g., RTC, SD card)
    modeStandard();
}

void loop() {
    if (buttonPressed) {
        buttonPressed = false; // Reset the flag
        unsigned long dureeAppui = millis();

        while (digitalRead(BOUTON_ROUGE) == LOW) {
            // Do nothing, just wait until button is released
        }

        dureeAppui = millis() - pressDuration;

        if (pressDuration > 2000) { // Si le bouton est appuyé pendant plus de 2 secondes
            currentMode = (currentMode + 1) % 4; // Passer au mode suivant
            switch (currentMode) {
                case CONFIGURATION:
                    modeConfiguration();
                    break;
                case STANDARD:
                    modeStandard();
                    break;
                case ECONOMIQUE:
                    modeEconomique();
                    break;
                case MAINTENANCE:
                    modeMaintenance();
                    break;
            }
        }
    }

    // Lecture des capteurs
    capteurs = get_data();
    String time = get_time();

    // Sauvegarde des données
    save_data_csv(capteurs, time);

    delay(1000); // Attente d'une seconde avant la prochaine lecture
}
