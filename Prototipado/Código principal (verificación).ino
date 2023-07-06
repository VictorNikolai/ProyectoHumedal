#include <Servo.h>

// Pines de los reles
const int rele1Pin = 2;
const int rele2Pin = 3;
const int rele3Pin = 4;
// Pin del sensor ultrasónico
const int triggerPin = 5;
const int echoPin = 6;
// Pin del potenciómetro
const int potenciometroPin = A0;
// Variables de tiempo
unsigned long tiempoInicio = 0;
const unsigned long tiempoEsperaRele1 = 15000; // 15 segundos
const unsigned long tiempoEsperaRele2 = 10000; // 10 segundos
// Variables de control
int estado = 0;
int distanciaSensor = 0;
int valorPotenciometro = 0;
// Objeto del servo
Servo servo;
void setup() {
  Serial.begin(9600);
  // Configurar pines de los reles como salidas
  pinMode(rele1Pin, OUTPUT);
  pinMode(rele2Pin, OUTPUT);
  pinMode(rele3Pin, OUTPUT);
  // Configurar pines del sensor ultrasónico
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Inicializar el objeto del servo
  servo.attach(9);
  // Abrir los reles 2 y 3
  digitalWrite(rele2Pin, HIGH);
  digitalWrite(rele3Pin, HIGH);
  // Esperar 15 segundos antes de cambiar al caso 1
  delay(tiempoEsperaRele1);
  // Apagar el rele 1 y cambiar al caso 1
  digitalWrite(rele1Pin, LOW);
  estado = 1;
}
void loop() {
  switch (estado) {
    case 1:
      // Abrir rele 2
      digitalWrite(rele2Pin, LOW);

      // Esperar 10 segundos antes de activar el rele 1 y el sensor ultrasónico
      delay(tiempoEsperaRele2);

      // Activar el rele 1 y el sensor ultrasónico
      digitalWrite(rele1Pin, HIGH);
      activarSensorUltrasonico();

      // Calcular el valor del potenciómetro
      valorPotenciometro = (100 * analogRead(potenciometroPin)) / 800;

      if (distanciaSensor < 6) {
        Serial.println("Agua lista para el riego");
        // Abrir rele 3 para permitir el paso del agua
        digitalWrite(rele3Pin, LOW);
      } else {
        Serial.println("Enciende la bomba");
        // Abrir rele 3 para permitir el paso del agua
        digitalWrite(rele3Pin, LOW);

        // Activar el servo con rotación de 0 a 180
        for (int angulo = 0; angulo <= 180; angulo++) {
          servo.write(angulo);
          delay(15);
        }
        delay(3000); // Esperar 3 segundos

        // Cerrar rele 3
        digitalWrite(rele3Pin, HIGH);
      }

      estado = 2; // Cambiar al caso 2
      break;

    case 2:
      if (distanciaSensor > 9) {
        // Cerrar rele 3
        digitalWrite(rele3Pin, HIGH);
      }

      // Realizar otras operaciones en el caso 2 si es necesario
      // ...

      estado = 0; // Volver al caso 0 para reintar el ciclo
      break;
  }
}

void activarSensorUltrasonico() {
  // Generar un pulso corto en el pin del trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Medir la duración del pulso en el pin del echo
  unsigned long duracionPulso = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros
  distanciaSensor = (duracionPulso /2 ) / 29.1; // Dividir por 58 para obtener la distancia en centímetros
}
