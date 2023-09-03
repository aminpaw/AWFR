#include "util.h"

float readSerialFloat()
{
    stdio_flush();
    bool stringComplete = false;
    int16_t ch = getchar_timeout_us(100);
    char buff[32] = "";
    char *buffPtr = buff;
    while (stringComplete == false)
    {
        if (ch != PICO_ERROR_TIMEOUT)
        {
            if (ch == '\n')
            {
                *buffPtr++ = '\0';
                stringComplete = true;
                buffPtr = buff;
            }
            else if (ch == 'b')
            {
                return -1;
            }
            else
            {
                *buffPtr++ = ch;
            }
        }
        ch = getchar_timeout_us(100);
    }
    return atof(buff);
}

int readSerialInt()
{
    stdio_flush();
    bool stringComplete = false;
    int16_t ch = getchar_timeout_us(100);
    char buff[32] = "";
    char *buffPtr = buff;
    while (stringComplete == false)
    {
        if (ch != PICO_ERROR_TIMEOUT)
        {
            if (ch == '\n')
            {
                *buffPtr++ = '\0';
                stringComplete = true;
                buffPtr = buff;
            }
            else if (ch == 'b')
            {
                return -1;
            }
            else
            {
                *buffPtr++ = ch;
            }
        }
        ch = getchar_timeout_us(100);
    }
    return atoi(buff);
}

char readSerialFirstChar()
{
    getchar();
    stdio_flush();
    int16_t ch = getchar_timeout_us(100);
    while (ch != PICO_ERROR_TIMEOUT)
    {
        ch = getchar_timeout_us(100);
    }
    return ch;
}

string floatToString(float value, int precision)
{
    ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}