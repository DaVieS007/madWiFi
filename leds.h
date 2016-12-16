int __led1 = 0;
int __led2 = 0;

void led1_blink(int interval)
{
    if(trigger(interval,&__led1))
    {
        digitalWrite(LED1,!digitalRead(LED1));
    }
}

void led1_clear(int interval)
{
    if(trigger(interval,&__led1))
    {
        digitalWrite(LED1,HIGH);
    }
}


void led2_blink(int interval)
{
    if(trigger(interval,&__led2))
    {
        digitalWrite(LED2,!digitalRead(LED2));
    }
}

void led2_clear(int interval)
{
    if(trigger(interval,&__led2))
    {
        digitalWrite(LED2,HIGH);
    }
}

