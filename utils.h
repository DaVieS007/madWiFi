/** StringIndex **/
String StringIndex(String data, String delim, int pos)
{
    String buffer[20];
    String tmp;
    int len = data.length();
    int index = 0;

    /** TRIM **/
    while(data.indexOf(delim + delim) > 0)
    {
        data.replace(delim+delim,delim);
    }
    /** TRIM **/
    
    for(int i=0;i<len;i++)
    {
        tmp = data.substring(i,i+1);
        if(tmp != " ")
        {
            buffer[index] += tmp;
        }
        else
        {
            index++;
        }
    }

   return buffer[pos];
}
/** StringIndex **/

/** hexDump **/
void hexDump(uint8_t *data, int len)
{
    int wrap = 0;
    char buff[6];
    for(int i=0;i<len;i++)
    {
        buff[0] = 0x00;
        sprintf(buff, "[%02x] %c", data[i],data[i]);
        Serial.print(buff);
        wrap++;

        if(wrap >= 6)
        {
            Serial.println("");
            wrap = 0;
        }
    }

    return;
}
/** hexDump **/

/** HEXLIB **/
inline byte hexToByte(char* hex)
{
  byte high = hex[0];
  byte low = hex[1];
  if (high > '9'){
    high -= (('A'-'0')-0xA); //even if hex is lower case (e.g. 'a'), the lower nibble will have the correct value as (('a'-'A')&0x0F) = 0.
  }
  if (low > '9'){
    low -= (('A'-'0')-0xA); //even if hex is lower case (e.g. 'a'), the lower nibble will have the correct value as (('a'-'A')&0x0F) = 0.
  }
  return (((high & 0xF)<<4) | (low & 0xF));
}

inline unsigned int hexToInt(char* hex)
{
  return ((hexToByte(hex) << 8) | hexToByte(hex+2));
}

inline unsigned long hexToLong(char* hex)
{
  return ((((unsigned long)hexToInt(hex)) << 16) | hexToInt(hex+4));
}
/** HEXLIB **/
