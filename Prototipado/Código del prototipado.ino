#include <WiFi.h>
#include <Servo.h>

Estas líneas incluyen las bibliotecas necesarias para utilizar la funcionalidad de Wi-Fi y Servo en Arduino.


const char* ssid = "TU_SSID";       // Nombre de tu red Wi-Fi
const char* password = "TU_PASSWORD";   // Contraseña de tu red Wi-Fi

Aquí debemos reemplazar `"NUESTRO_SSID"` por el nombre de tu red Wi-Fi y `"NUESTRA_CONTRASEÑA"` por la contraseña correspondiente.


const char* host = "tu-servidor";   // Dirección IP o dominio de tu servidor

Debes reemplazar `"NUESTRO-servidor"` por la dirección IP o dominio de tu servidor al que deseas enviar alertas o resultados.

Servo servoMotor;       // Objeto para controlar el servo motor
int potPin = A0;         // Pin analógico donde está conectado el potenciómetro
int servoPin = 9;        // Pin donde está conectado el servo motor

Aquí se declara el objeto `servoMotor` de la clase `Servo` para controlar el servo motor. `potPin` es el número del pin analógico al que está conectado el potenciómetro, y `servoPin` es el número del pin digital al que está conectado el servo motor.

int sensorPin = A1;      // Pin analógico donde está conectado el sensor de Nitrógeno
int threshold = 700;     // Umbral para detectar altas concentraciones de Nitrógeno

`sensorPin` es el número del pin analógico al que está conectado el sensor de Nitrógeno. `threshold` es el umbral utilizado para determinar si la concentración de Nitrógeno es alta o no.


void setup() {
  Serial.begin(115200);
  delay(10);

  servoMotor.attach(servoPin);   // Configuración del pin del servo motor

  pinMode(sensorPin, INPUT);     // Configuración del pin del sensor de Nitrógeno

  WiFi.begin(ssid, password);    // Conexión a la red Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conexión Wi-Fi establecida");
}

La función `setup()` se ejecuta una vez al iniciar el Arduino. Configura la comunicación en serie, adjunta el servo motor al pin correspondiente, configura el pin del sensor de Nitrógeno como entrada y se conecta a nuestra red Wi-Fi especificada.

void loop() {
  int potValue = analogRead(potPin);   // Lectura del valor del potenciómetro
  int angle = map(potValue, 0, 1023, 0, 180);   // Mapeo del valor del potenciómetro al rango de ángulo del servo motor

  servoMotor.write(angle);   // Movimiento del servo motor al ángulo calculado

  int sensorValue = analogRead(sensorPin);   // Lectura del valor del sensor de Nitrógeno

  // Verificar si la concentración de Nitrógeno es alta
  if (sensorValue > threshold) {
    // Realizar acciones para ajust

ar el flujo de agua según sea necesario
    // ...
    // Aquí podemos agregar la lógica para ajustar la válvula y el flujo de agua

    // Envío de alerta a través de Wi-Fi
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      if (client.connect(host, 80)) {
        String data = "Alerta: Alta concentración de Nitrógeno detectada";
        client.println("POST /enviar-alerta HTTP/1.1");
        client.println("Host: " + String(host));
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.println("Content-Length: " + String(data.length()));
        client.println();
        client.println(data);
        client.println();
        delay(500);
        client.stop();
      }
    }
  }

  delay(1000);   // Espera de 1 segundo antes de repetir el bucle
}

