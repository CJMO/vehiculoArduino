#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NewPing.h>
#include <Servo.h>

//#include <Timer.h>
//Timer t;

/*Aqui se configuran los pines donde debemos conectar el sensor de ultrasonido*/
#define TRIGGER_PIN  D3
#define ECHO_PIN     D8
#define MAX_DISTANCE 200

/*Crear el objeto de la clase NewPing*/
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Declaramos la variable para controlar el servo
Servo servoMotor;

const char* ssid = "RED";
const char* password = "CLAVE";

#include "L9110.h"
L9110 vehiculo(D4,D5,D6,D7);

String datos = "";

uint32_t TIEMPO_DeepSleep = 1000000;

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.print("Conectando.."); 
  }

  // Iniciamos el servo para que empiece a trabajar con el pin 9
  //servoMotor.attach(D1);
 
  // Inicializamos al ángulo 0 el servomotor
  servoMotor.write(0);
}

void loop() {

      //circuito();
    
      // Verificar si ya se realizó lectura de datos
      if( datos.equals("")){
    
        if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     
        HTTPClient http;  //Declare an object of class HTTPClient
     
        http.begin("http://ruta-vehiculo.azurewebsites.net/resultado.php");  
        int httpCode = http.GET();                                                                  
     
        if (httpCode > 0) { //Check the returning code
     
          datos = http.getString();   //Get the request response payload
          //Serial.println(datos);                     //Print the response payload
        
        }
      
        http.end();   //Close connection

        // Apagar Wifi
        WiFi.forceSleepBegin();
        Serial.println("Wifi apagado");
       
        }
      }

      // Procesar datos

      // Convert from String Object to String.
      char buf[5000];
      datos.toCharArray(buf, 5000);
      char *p = buf;
      char *str;
      char *strOpe;


      String parametros[4];
            
      while ((str = strtok_r(p, ";", &p)) != NULL){ // delimiter is the semicolon

        char bufOpe[50];
        char *temp = bufOpe;
        String datos2 = String(str);
        datos2.toCharArray(bufOpe, 50);
        
        //Serial.println(datos2);

        int i = 0;
        
        while ((strOpe = strtok_r(temp, "/", &temp)) != NULL){

          String t = String(strOpe);     
          //Serial.println(t);
          
          parametros[i] = t; 
          ++i;
          
        }
        
        // parámetros para operación
        ejecutarMovimiento(parametros);
        delay(500);
       
    }
        
 
  //delay(1000);
  
  /*vehiculo.forward(120,120,500);
  Serial.println("Adelante");
  //delay(1000);  
  //ESP.deepSleep(TIEMPO_DeepSleep, WAKE_RF_DEFAULT);  // Desabilita la señal de radio después del reencendido
  WiFi.forceSleepBegin(0);
  Serial.println("Wifi apagado");
  Serial.print(WiFi.status());
  vehiculo.reverse(10,10,500);
  Serial.println("Atrás");
  //delay(1000); 
  //ESP.deepSleep(TIEMPO_DeepSleep, WAKE_RF_DEFAULT);  // Desabilita la señal de radio después del reencendido 
  WiFi.forceSleepWake();
  Serial.println("Wifi encendido");
  Serial.print(WiFi.status());*/
}

void ejecutarMovimiento(String parametros[4]){

  int distancia = 0;

  Serial.println(parametros[0]);
  Serial.println(parametros[1]);
  Serial.println(parametros[2]);
  Serial.println(parametros[3]);

  // Obtener medicion de tiempo de viaje del sonido y guardar en variable uS
  int uS = sonar.ping_median();
  distancia = (uS / US_ROUNDTRIP_CM);
  // Imprimir la distancia medida a la consola serial
  Serial.print("Distancia: ");
  // Calcular la distancia con base en una constante
  Serial.print(distancia);
  Serial.println("cm");
  

  if(parametros[0].equals("avanzar")){

    // controlar distancia
    if(distancia > 5){
      Serial.println("Avanzando");  
      vehiculo.forward(parametros[1].toInt(),parametros[2].toInt(),parametros[3].toInt());
    }
    // de lo contrario regresar
    else{
      Serial.println("Retrocediendo");
      vehiculo.reverse(parametros[1].toInt(),parametros[2].toInt(),parametros[3].toInt());
    }
    
      
  }
  else if(parametros[0].equals("reversar")){
    Serial.println("Retrocediendo");
    vehiculo.reverse(parametros[1].toInt(),parametros[2].toInt(),parametros[3].toInt());
           
  }
  else if(parametros[0].equals("girar")){
    Serial.println("Girando");
    vehiculo.rotate(parametros[1].toInt(),parametros[2].toInt(),parametros[3].toInt());
           
  }
  else if(parametros[0].equals("detener")){
    Serial.println("Deteniendo");
    vehiculo.brake(parametros[3].toInt());
           
  }

  else if(parametros[0].equals("conducir")){
    Serial.println("Conduciendo");
    vehiculo.drive(parametros[1].toInt(),parametros[2].toInt(),parametros[3].toInt());
           
  }

  // eventos de cámara
  else if(parametros[0].equals("girar_camara_detener")){
    Serial.println("Girando cámara");
    // configurar giro con velocidad recibida
    girar_camara_detener();
           
  }

  else if(parametros[0].equals("girar_camara_cont")){
    Serial.println("Girando cámara continuamente");
    // configurar giro con velocidad recibida
    //t.every(parametros[1].toInt(), girar_camara_continua);
    girar_camara_continua(parametros[1].toInt());
    //t.every(parametros[1].toInt(), pausar);
           
  }
  
}

void girar_camara_detener() {

   // Desplazamos al ángulo correspondiente
   servoMotor.write(10);
   // Hacemos una pausa con el temporizador Timer
   servoMotor.detach();
}

void girar_camara_continua(int tiempo_pausa) {

   servoMotor.attach(D1); 
   // Desplazamos al ángulo correspondiente
   servoMotor.write(10);
   // Hacer pausa en giro
   servoMotor.detach();
   delay(tiempo_pausa);
  
}
