/*
descripcion: Control basico de agitador magnetico con control de set_time de agitado y velocidad del motor

Perifericos:

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


programmer: Ariel Wels
*/

#include "mbed.h"
#include <cstdint>


//instancio objetos de botones
DigitalIn start_button(PC_13);
DigitalIn time_button(PB_5);

//instancio obj. entrada analogica
AnalogIn speed_input(PA_0);

//instancio objetos de leds
DigitalOut led_time_0(PA_3);
DigitalOut led_time_1(PA_2);
DigitalOut led_time_2(PA_10);

DigitalOut motor(PB_3);

//instancio uart driver
UnbufferedSerial serial_port(USBTX, USBRX); //seria ideal limitar el scope a una biblioteca de manejo serial


// to do: pasar un puntero a una estructura que contenga todo el estado del aparato para poder monitorear y modificar mas libremente
bool manage_serial(uint8_t vel, uint8_t tim){
    bool ret = false;
    //atiendo comandos seriales:
    char received = 0; // '\0'
    if( serial_port.readable() ){
        serial_port.read( &received, 1);
        switch(received){
            //---------------------------------------------------------------
            case 'A':
                serial_port.write( "Starting stirer\r\n", 17);// estaria bueno una funcion que le pases el const char* y cuente solo los caracteres.
                ret=true;
            break;
            //---------------------------------------------------------------
            case 'S':
                char str[30];
                sprintf(str,"La velocidad seteada es: %d\n", vel);
                serial_port.write( str, strlen(str) );

                sprintf(str,"el tiempo seteado es: %d\n", tim);
                serial_port.write( str, strlen(str) );

                //queda por implementar medicion de temperatura, pero no deja de ser solo otro A/D
                //sprintf(str,"La temperatura actual es %d\n", speed);
                //serial.write( str, strlen(str) );
            break;
            //---------------------------------------------------------------
            default:
                //availableCommands();
            break;
            //---------------------------------------------------------------
        }
        
    }
    return ret;
}


int main(void)
{
    //habilito pullups
    start_button.mode(PullUp); // este ya esta externo en la placa pero no molesta.
    time_button.mode(PullUp);

    //config de serial port
    // Set desired properties (9600-8-N-1).
    serial_port.baud(9600);
    serial_port.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );


    /* comento porque creo que la idea de este tp no es usar irq, callbacks ni nada por el estilo.
    // Register a callback to process a Rx (receive) interrupt.
    serial_port.attach(&on_rx_interrupt, SerialBase::RxIrq); 
    */


    bool working=false;
    uint8_t set_time = 0, speed=0;
    uint32_t elapsed_time=0;

    //infinite loop
    while(true){
/*
    // Read the data to clear the receive interrupt.
    if (serial_port.read(&c, 1)) {
        // Echo the input back to the terminal.
        serial_port.write(&c, 1);
    }
*/

        

        thread_sleep_for(1); //uso de base de tiempo falopa, tambien me cambia frecuencia de pwm, polemiquisimo.
        

        if( manage_serial(speed, set_time) == true ){
            working=true;
        }




        //leo la velocidad seteada
        speed = speed_input * 99; // escalo a 0 a 255 para el pwm casero

        //espero a que se aprete boton de start para arrancar
        if(start_button == 0 ){
            //habilito a contar set_time
            working=true;
        }
        //boton de cambio de tiempo seteado
        if(time_button == 0){
            //cambio set_time seteado
            set_time++;
            if(set_time >= 8 )
                set_time = 0;
        }


        //-----------------------------------------------------------------------------------
        //-----------------------------------------------------------------------------------
        //-----------------------------------------------------------------------------------

        // si se disparo el funcionamiento, empiezo a contar tiempo y pongo el motor a la velocidad que va
        if(working){
            elapsed_time++;

            //hago control de velocidad, bastante precario, sin control de frecuencia, fase, ni nada de nada, de casualidad cambia el ancho de pulso
            static uint8_t pwm_cnt=0;
            pwm_cnt = pwm_cnt < 99 ? pwm_cnt+1 : 0;

            motor = pwm_cnt < speed ? 0 : 1 ; //pwm :s
        }else{
            motor=0;
        }

        //Si se acabo el tiempo apago (cuento en base a minutos, 60000 millisegundos)
        if(elapsed_time >= ((uint32_t)set_time*60000)){
            working = false; 
            elapsed_time=0;
        }

        //actualizo los leds con el set_time seleccionado
        switch (set_time) {
            //---------------------
            case 0:
                led_time_0 = 0;
                led_time_1 = 0;
                led_time_2 = 0;
            break;
            //---------------------
            case 1:
                led_time_0 = 1;
                led_time_1 = 0;
                led_time_2 = 0;
            break;
            //---------------------
            case 2:
                led_time_0 = 0;
                led_time_1 = 1;
                led_time_2 = 0;
            break;
            //---------------------
            case 3:
                led_time_0 = 1;
                led_time_1 = 1;
                led_time_2 = 0;
            break;
            //---------------------
            case 4:
                led_time_0 = 0;
                led_time_1 = 0;
                led_time_2 = 1;
            break;
            //---------------------
            case 5:
                led_time_0 = 1;
                led_time_1 = 0;
                led_time_2 = 1;
            break;
            //---------------------
            case 6:
                led_time_0 = 0;
                led_time_1 = 1;
                led_time_2 = 1;
            break;
            //---------------------
            case 7:
                led_time_0 = 1;
                led_time_1 = 1;
                led_time_2 = 1;
            break;
            //---------------------
            default:
                set_time = 0;
        }




    }
}
