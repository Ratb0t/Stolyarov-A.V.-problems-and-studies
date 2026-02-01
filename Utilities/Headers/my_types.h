#ifndef REINCLUDE_PROTECTION_MY_TYPES
#define REINCLUDE_PROTECTION_MY_TYPES
typedef union universal_data_type
{
    long long as_long;
    int as_int;
    double as_double;
    short as_short;
    char as_char;

    unsigned long long as_ulong;
    unsigned int as_uint;
    unsigned short as_ushort;
    unsigned char as_uchar;

    void *as_void;
} top_type;


#endif /*!REINCLUDE_PROTECTION_MY_TYPES*/