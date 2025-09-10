
# Solucion reto 2

## Explicacion del encendido de una secuencia de LEDs con un boton

Para controlar el encendido de los led's se utilizó un botòn que envia una señal por medio de una interrupciòn para ejecetar el encendido de cada led en orden utilizando la funciòn `for`

```c
#define NUM_LEDS 8
int BOTON = 2;


int leds[NUM_LEDS] = {3, 4, 5, 6, 7, 8, 9, 10};
volatile int contador = 0;  

void setup() {
  for (int i = 0; i < NUM_LEDS; i++) {  
    digitalWrite(leds[i], LOW);
  }

  pinMode(BOTON, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(BOTON), contarClick, FALLING);
}

void loop() {

  for (int i = 0; i < NUM_LEDS; i++) {
    if (contador > i) {
      digitalWrite(leds[i], HIGH);
    } else {
      digitalWrite(leds[i], LOW);
    }
  }
}


void contarClick() {
  contador++;
  if (contador > NUM_LEDS) {
    contador = 0;  
  }
}

```

#### Imagen del circuito:

![Circuito](./asets/Screenshot%202025-08-26%20083642.png)





