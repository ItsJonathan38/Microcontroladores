# Solución reto 1

## Explicación del contador persistente de encendidos

Este reto busca registrar cuántas veces se ha encendido el Arduino, incluso después de apagarlo o reiniciarlo. Para lograrlo, se utiliza la memoria EEPROM, que conserva los datos aunque se interrumpa la alimentación eléctrica.
Funcionamiento general:
Al iniciar (setup()), el Arduino lee el contador almacenado en la EEPROM.
Luego incrementa ese valor en 1 para indicar un nuevo encendido.
El nuevo valor se guarda nuevamente en la EEPROM.
Finalmente, se muestra el número total de encendidos en el Monitor Serial.

```c
#include <EEPROM.h>

int direccion = 0;        // Dirección de memoria EEPROM donde se guarda el contador
unsigned int contador = 0; // Variable para almacenar el número de encendidos

void setup() {
  Serial.begin(9600);

  // Leer el valor actual almacenado en EEPROM
  contador = EEPROM.read(direccion);

  // Incrementar el contador
  contador++;

  // Guardar el nuevo valor en EEPROM
  EEPROM.write(direccion, contador);

  // Mostrar el número total de encendidos
  Serial.print("Número de encendidos: ");
  Serial.println(contador);

  // Indicador visual: parpadeo según cantidad de encendidos (máximo 10)
  pinMode(LED_BUILTIN, OUTPUT);
  int parpadeos = (contador > 10) ? 10 : contador;
  for (int i = 0; i < parpadeos; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}

void loop() {
}
```
