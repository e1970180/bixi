#include "../strings/bxistrconv.h"
#include "../strings/bxistring.h"
#include "../types/bxiboints.h"
#include "../math/bximath.h"

const char bxi_hexes_high[] = "0123456789ABCDEF";
const char bxi_hexes_low [] = "0123456789abcdef";

i32 str2i32(const char * str, i32 * len)
{
    i32 sign = 0;
    u32 ures = 0;
    i32 pos  = 0;
    u32 skp  = 0;

    if (!str)
    {
       if (len) *len = BXI_STRERROR_NOSTRING;
       return 0;
    }

    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
    {
        sign = +1;
        str++;
    }

    while(isasciispace(str[pos]))
    {
        str++;
        skp++;
    }

    while (str[pos])
    {
        if ((str[pos] >= '0') && (str[pos] <= '9'))
        {
            if (ures > U32_MAX / 10)
            {
                if (len) *len = BXI_STRERROR_CONVOVERFLOW;
                return 0;
            }
            ures *= 10;

            if ((u32)(ures + str[pos] - '0') < ures)
            {
                if (len) *len = BXI_STRERROR_CONVOVERFLOW;
                return 0;
            }
            ures += str[pos] - '0';
        }
        else
        {
            if (pos && !sign)
                break;

            if (len) *len = BXI_STRERROR_BADSTRING;
            return 0;
        }

        pos++;
    }

    if (!pos)
    {
        if (len) *len = BXI_STRERROR_BADSTRING;
        return 0;
    }

    if (((ures >      I32_MAX    )  && (!sign || (sign ==  1))) ||
        ((ures > (u32)I32_MAX + 1)  && (sign == -1)))
    {
        if (len) *len = BXI_STRERROR_CONVOVERFLOW;
        return 0;
    }

    if (len) *len = pos + (sign ? 1 : 0) + skp;

    return (i32)ures * (sign ? sign : 1);
}

u32 i2str(char * str, i32 val)
{
    u32 pos = 0;
    u32 len = 1;
    u32 ttl = 0;
    i32 tmp = val;

    if (!str)
        return 0;

    if (val < 0)
        str[pos++] = '-';

    while (tmp / 10 != 0)
    {
        len++;
        tmp /= 10;
    }

    str[len + pos] = '\0';
    ttl = len + pos;

    while (len--)
    {
        str[len + pos] = bxi_abs(val % 10) + '0';
        val /= 10;
    }

    return ttl;
}

u32 u2str(char * str, u32 val)
{
    u32 len = 1;
    u32 ttl = 0;
    u32 tmp = val;

    if (!str)
        return 0;

    while (tmp / 10 != 0)
    {
        len++;
        tmp /= 10;
    }

    str[len] = '\0';
    ttl = len;

    while (len--)
    {
        str[len] = val % 10 + '0';
        val /= 10;
    }

    return ttl;
}
/*
u32 i2hex(char * str, i32 val)
{

} */

u32 u2hex_doublelowhex(char * str, u32 val, u32 len)
{
    const u32 w = 2;
    u32 ttl;

    len += len % w;
    str[len] = '\0';
    ttl = len;

    while (len--)
    {
        str[len] = bxi_hexes_low[val % 16];

        val /= 16;
    }

    return ttl;
}

u32 u2hex(char * str, u32 val, bxi_hex_format fmt)
{
    u32 len = 1;
    u32 tmp = val;

    if (!str)
        return 0;

    while (tmp / 16 != 0)
    {
        len++;
        tmp /= 16;
    }

    switch (fmt)
    {
    case BXI_HEX_DOUBLELOWHEX : return u2hex_doublelowhex(str, val, len);
    default: ;
    }

    return 0;
}

u32 i2hex(char * str, i32 val, bxi_hex_format fmt)
{
    bool minus = false;

    if (!str)
        return 0;

    if (val < 0)
    {
        *str++ = '-';
        val = -(u32)val;
        minus = true;
    }

    return u2hex(str, (u32)val, fmt) + minus;
}

u32 i82str(char * str, i8 val)
{
    return i2str(str, val);
}

u32 u82str (char * str, u8 val)
{
    return u2str(str, val);
}

u32 i162str(char * str, i16 val)
{
    return i2str(str, val);
}

u32 u162str(char * str, u16 val)
{
    return u2str(str, val);
}

u32 i322str(char * str, i32 val)
{
    return i2str(str, val);
}

u32 u322str(char * str, u32 val)
{
    return u2str(str, val);
}

u32 i16_le2str(char * str, i16_le val)
{
    return i2str(str, IS_LE ? val : (i16)chgend16(val));
}

u32 u16_le2str(char * str, u16_le val)
{
    return u2str(str, IS_LE ? val : (u16)chgend16(val));
}

u32 i32_le2str(char * str, i32_le val)
{
    return i2str(str, IS_LE ? val : (i32)chgend32(val));
}

u32 u32_le2str(char * str, u32_le val)
{
    return u2str(str, IS_LE ? val : (u32)chgend32(val));
}

u32 i16_be2str(char * str, i16_be val)
{
    return i2str(str, IS_BE ? val : (i16)chgend16(val));
}

u32 u16_be2str(char * str, u16_be val)
{
    return u2str(str, IS_BE ? val : (u16)chgend16(val));
}

u32 i32_be2str(char * str, i32_be val)
{
    return i2str(str, IS_BE ? val : (i32)chgend32(val));
}

u32 u32_be2str(char * str, u32_be val)
{
    return u2str(str, IS_BE ? val : (u32)chgend32(val));
}

u32 i82hex (char * str, i8  val)
{
    return i2hex(str, val, BXI_HEX_DEFAULT);
}

u32 u82hex (char * str, u8 val)
{
    return u2hex(str, val, BXI_HEX_DEFAULT);
}

u32 i162hex(char * str, i16 val)
{
    return i2hex(str, val, BXI_HEX_DEFAULT);
}

u32 u162hex(char * str, u16 val)
{
    return u2hex(str, val, BXI_HEX_DEFAULT);
}

u32 i322hex(char * str, i32 val)
{
    return i2hex(str, val, BXI_HEX_DEFAULT);
}

u32 u322hex(char * str, u32 val)
{
    return u2hex(str, val, BXI_HEX_DEFAULT);
}

u32 i16_le2hex(char * str, i16_le val)
{
    return i2hex(str, IS_LE ? val : (i16)chgend16(val), BXI_HEX_DEFAULT);
}

u32 u16_le2hex(char * str, u16_le val)
{
    return u2hex(str, IS_LE ? val : (u16)chgend16(val), BXI_HEX_DEFAULT);
}

u32 i32_le2hex(char * str, i32_le val)
{
    return i2hex(str, IS_LE ? val : (i32)chgend32(val), BXI_HEX_DEFAULT);
}

u32 u32_le2hex(char * str, u32_le val)
{
    return u2hex(str, IS_LE ? val : (u32)chgend32(val), BXI_HEX_DEFAULT);
}

u32 i16_be2hex(char * str, i16_be val)
{
    return i2hex(str, IS_BE ? val : (i16)chgend16(val), BXI_HEX_DEFAULT);
}

u32 u16_be2hex(char * str, u16_be val)
{
    return u2hex(str, IS_BE ? val : (u16)chgend16(val), BXI_HEX_DEFAULT);
}

u32 i32_be2hex(char * str, i32_be val)
{
    return i2hex(str, IS_BE ? val : (i32)chgend32(val), BXI_HEX_DEFAULT);
}

u32 u32_be2hex(char * str, u32_be val)
{
    return u2hex(str, IS_BE ? val : (u32)chgend32(val), BXI_HEX_DEFAULT);
}
