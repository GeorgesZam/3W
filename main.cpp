#include <Arduino.h>
#include "3W.h"

// Définition des macros
#define BOUTON_ROUGE 2   // Port du bouton rouge
#define BOUTON_VERT 3   // Port du bouton vert
#define PHOTORESISTANCE A0   // Port de la photorésistance
#define LED_PIN 7   // Port de la LED

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
void onButtonPress();
void clignoterLED();

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
        dataFile.print(time);            // Sauvegarder le temps
        dataFile.print(",");
        dataFile.print(capteurs.temperatureAir);     // Sauvegarder la température
        dataFile.print(",");
        dataFile.print(capteurs.hygrometrie);        // Sauvegarder l'humidité
        dataFile.print(",");
        dataFile.println(capteurs.lumiere);    // Sauvegarder la luminosité
        dataFile.close();
    } else {
        Serial.println("Erreur d'ouverture du fichier");
    }
}

// Fonctions pour les différents modes
void modeConfig() {
    Serial.println("Mode Configuration activé");
    // Logique pour le mode configuration
}

void modeStandard() {
    Serial.println("Mode Standard activé");
    // Logique pour le mode standard
}

void modeEco() {
    Serial.println("Mode Économique activé");
    // Logique pour le mode économique
}

void modeMaint() {
    Serial.println("Mode Maintenance activé");
    // Logique pour le mode maintenance
}

// Fonction d'interruption pour gérer l'appui sur le bouton
void onButtonPress() {
    boutonAppuye = true;
}

// Fonction pour faire clignoter la LED selon le mode
void clignoterLED() {
    static unsigned long dernierTemps = 0;
    static bool etatLED = LOW;
    unsigned long intervalle = 0;

    switch (modeCourant) {
        case CONFIGURATION:
            intervalle = 100;  // Clignotement rapide
            break;
        case STANDARD:
            intervalle = 500;  // Clignotement moyen
            break;
        case ECONOMIQUE:
            intervalle = 1000; // Clignotement lent
            break;
        case MAINTENANCE:
            intervalle = 200;  // Clignotement intermédiaire
            break;
    }

    if (millis() - dernierTemps >= intervalle) {
        dernierTemps = millis();
        etatLED = !etatLED;
        digitalWrite(LED_PIN, etatLED);
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(BOUTON_ROUGE, INPUT_PULLUP);
    pinMode(BOUTON_VERT, INPUT);
    pinMode(LED_PIN, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(BOUTON_ROUGE), onButtonPress, FALLING);
    // Initialisation des capteurs et modules (e.g., RTC, SD card)
    modeStandard();
}

void loop() {
    if (boutonAppuye) {
        boutonAppuye = false; // Reset the flag
        unsigned long dureeAppui = millis();

        while (digitalRead(BOUTON_ROUGE) == LOW) {
            // Do nothing, just wait until button is released
        }

        dureeAppui = millis() - dureeAppui;

        if (dureeAppui > 2000) { // Si le bouton est appuyé pendant plus de 2 secondes
            modeCourant = (modeCourant + 1) % 4; // Passer au mode suivant
            switch (modeCourant) {
                case CONFIGURATION:
                    modeConfig();
                    break;
                case STANDARD:
                    modeStandard();
                    break;
                case ECONOMIQUE:
                    modeEco();
                    break;
                case MAINTENANCE:
                    modeMaint();
                    break;
            }
        }
    }

    // Lecture des capteurs
    capteurs = get_data();
    String time = get_time();

    // Sauvegarde des données
    save_data_csv(capteurs, time);

    // Faire clignoter la LED en fonction du mode
    clignoterLED();

    delay(1000); // Attente d'une seconde avant la prochaine lecture
}
