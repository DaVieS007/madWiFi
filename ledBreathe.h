#define BRIGHT    500     //max led intensity (1-500)
#define INHALE    2000    //Inhalation time in milliseconds.
#define PULSE     INHALE*1000/BRIGHT
#define REST      1000    //Rest Between Inhalations.

int _led_i = 1;
byte _led_state = 0;
byte _led_busy = 0;
byte _led_inited = 0;

/** LED_DIM **/
void ledBreathe(int ledPin)
{
    if(_led_inited == 0)
    {
        digitalWrite(ledPin, HIGH);
    }

    if(_led_state == 0)
    {
        int lwait = PULSE-_led_i*10;
        if(lwait <= 0)
        {
            lwait = 1;
        }

        digitalWrite(ledPin, LOW);          // turn the LED on.
        delayMicroseconds(_led_i*10);         // wait
        digitalWrite(ledPin, HIGH);         // turn the LED off.
        delayMicroseconds(lwait);   // wait
        _led_i++;

        if(_led_i >= BRIGHT)
        {
            _led_state = 1;
        }
    }
    else if(_led_state == 1)
    {
        int lwait = PULSE-_led_i*10;
        if(lwait <= 0)
        {
            lwait = 1;
        }

        digitalWrite(ledPin, LOW);          // turn the LED on.
        delayMicroseconds(_led_i*10);          // wait
        digitalWrite(ledPin, HIGH);         // turn the LED off.
        delayMicroseconds(lwait);  // wait

        _led_i--;

        if(_led_i <= 1)
        {
            _led_state = 2;
            _led_i = REST;
        }
    }
    else if(_led_state == 2)
    {
        delay(1);

        _led_i--;
        if(_led_i <= 1)
        {
            _led_state = 0;
            _led_i = 1;
        }
    }


    return;
}
/** LED_DIM **/
