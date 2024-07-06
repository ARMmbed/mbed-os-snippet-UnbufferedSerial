#Agitador magnetico - magnetic stirrer

## descripcion:
El sistema consta de un motor DC acoplado a imanes que hace girar un buzo magnético dentro de
un erlenmeyer agitando su contenido, simulado por un led.
El usuario debe poder controlar encendido y setear un tiempo de agitacion entre valores
predefinidos y la velocidad mediante un potenciómetro. Como opcion para el usuario se podra
setear y activar desde una terminal serie, además de monitorizar tiempo restante, velocidad seteada
y temperatura.

## perifericos:

botones:
    start:          BUTTON1 (PC13)
    cambiar set_time: D4      (PB5)

LEDs:
    leds set_time:    D0      (PA3)
                    D1      (PA2)
                    D2      (PA10)

    Motor           D3      (PB3)

Analogico:
    Pote:           A0      (PA0)

    
Nota: partio de un fork de ejemplo serial de mbed, por lo que esta lleno de porquerias, hacer que quede bien y no perder el arbol de commits implica mucho bardo que no justifica, probablemente para siguientes TP simplemente copie la ultima version a un repo nuevo y que la historia se desvanezca.
