# CONTROL DE MASCOTAS
Programa basado en C++ y arduino que permite al usuario controlar a su mascota y sus necesidades.

## Integrantes del equipo

Jimena López Maldonado -> Jimenalopez02 .
Gregorio Marianini Ríos -> gmarianini .
Juan Ignacio Martín-> juanignacio-martin .

## Objetivos del trabajo

Nuestro objetivo es realizar un programa que satisfaga las necesidades de una
mascota sin la obligación de la presencia física del dueño.
El programa notificará al usuario si la temeperatura de la caseta es muy alta, y otras opciones como el control de que el recipiente de
agua esté siempre lleno y que se encienda una luz de noche fuera de la caseta.
## Sensores a utilizar
- Sensor Ultrasonido
Los sensores ultrasónicos miden la distancia mediante el uso de ondas ultrasónicas. El cabezal emite una onda ultrasónica y recibe la onda reflejada que retorna desde el objeto. Los sensores ultrasónicos miden la distancia al objeto contando el tiempo entre la emisión y la recepción.
- -Sensor de temperatura.
- Medida de intensidad luminosa- Sensor de luz LDR GL55:

Fuente: https://www.luisllamas.es/medir-nivel-luz-con-arduino-y-fotoresistencia-ldr/ El sensor LDR es un dispositivo formado por un semiconductor y su funcionamiento es tal que, al aumentar la incidencia de la luz sobre él, disminuye su resistencia. Este cambio de resistencia se debe a que, cuando recibe luz, el semiconductor que lo forma absorbe fotones y los electrones pasan a la banda de conducción y así disminuye su resistencia. Su mayor desventaja es la poca precisión y su uso limitado, ya que no puede ser usado para medir la intesidad lumínica, solo detecta los valores de "oscuridad" y "luminosidad".
- Micro Servo 9g SG90 de Tower Pro

## Dinámica de la aplicación
MENU:
1. Dispensador de comida
2. Iluminación
3. Control temperatura de la caseta
4. 4. Automático de luces.
## Opción 1
El usuario deberá introducir la altura a la que quiere que esté la comida en función del recipiente. Si esta altura es inferior, el comedero echará comida hasta que se consiga dicha altura.
## Opción 2
Habrá unas luces situadas en la facahada de la caseta de tal manera que se encenderá cuado el usuario lo indique.
## Opción 3 
Si se supera un cierto nivel de temperatura dentro de la caseta , el programa avisará al usuario de la temperatura límite.
## Opción 4 
Iluminación automática en función de la oscuridad.
