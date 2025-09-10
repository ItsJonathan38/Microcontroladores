# Solucion reto 5

## Explicacion del control de Brillo con Pulsadores (PWM)

Para ajustar el brillo del led, se utilizaron 2 botones, uno amentará el brillo y el otro lo disminuirá mediante señales PWM.

```c
int LED = 9;
int BOTON_UP = 2;
int BOTON_DOWN = 3;


int brillo = 0;        
int paso = 10;        

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTON_UP, INPUT_PULLUP);
  pinMode(BOTON_DOWN, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BOTON_UP) == HIGH) {
    brillo += paso;
    if (brillo > 80) brillo = 80; 
    analogWrite(LED, brillo);
    delay(200); 
  }

  if (digitalRead(BOTON_DOWN) == HIGH) {
    brillo -= paso;
    if (brillo < 0) brillo = 0; 
    analogWrite(LED, brillo);
    delay(200); 
  }
}

```

#### Imagen del circuito:

![Circuito](./asets/Screenshot%202025-08-20%20081823.png)