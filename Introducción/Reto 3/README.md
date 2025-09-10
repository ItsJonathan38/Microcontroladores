# Solucion reto 3

## Explicacion "Luz de emergencia" con 2 Led's

Con un led rojo y uno azul, se simula el funcionamiento de una luz de emergencia. Un botón activará un parpadeo alterno entre los Leds y el otro botón detendrá el parpadeo. 

```c
int LED_ROJO = 4;
int LED_AZUL = 5;
int BOTON_INICIO = 2;
int BOTON_DETENER = 3;

bool parpadeoActivo = false;

void setup() {
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  pinMode(BOTON_INICIO, INPUT_PULLUP);
  pinMode(BOTON_DETENER, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BOTON_INICIO) == LOW) {
    parpadeoActivo = true;
    delay(200); 
  }

  if (digitalRead(BOTON_DETENER) == LOW) {
    parpadeoActivo = false;
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_AZUL, LOW);
    delay(200);
  }

  if (parpadeoActivo) {
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_AZUL, LOW);
    delay(300);

    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_AZUL, HIGH);
    delay(300);
  }
}

```
#### Imagen del circuito:

![Circuito](./asets/Screenshot%202025-08-26%20081907.png)