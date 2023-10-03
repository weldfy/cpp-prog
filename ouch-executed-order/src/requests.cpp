#include "requests.h"

std::string decode_Token(const int start, const int len, const std::vector<unsigned char> & message)
{
    std::string token = "";
    for (int i = start; i < start + len; i++) {
        if (message[i] != ' ') {
            token += message[i];
        }
    }
    return token;
}

int decode_Integer(const int start, const int len, const std::vector<unsigned char> & message)
{
    int x = 0;
    for (int i = start; i < start + len; i++) {
        x = (x << 8) | int(message[i]);
    }
    return x;
}

double decode_Price(const int start, const int len, const std::vector<unsigned char> & message)
{
    return decode_Integer(start, len, message) / 10000.0;
}

char decode_Char(const int start, const std::vector<unsigned char> & message)
{
    return message[start];
}

bool decode_Bit(const int x, const int y, const std::vector<unsigned char> & message)
{
    return bool((int(message[x]) >> (y - 1)) % 2);
}

LiquidityIndicator decode_LiquidityIndicator(const int x, const int start, const std::vector<unsigned char> & message)
{
    bool a = decode_Bit(x, start + 1, message);
    bool b = decode_Bit(x, start, message);
    return (a == false && b == true ? LiquidityIndicator::Removed : (a == false && b == false ? LiquidityIndicator::Added : LiquidityIndicator::None));
}

BreakReason decode_BreakReason(const int start, const std::vector<unsigned char> & message)
{
    char a = decode_Char(start, message);
    return (a == 'E' ? BreakReason::Erroneous : (a == 'C' ? BreakReason::Consent : (a == 'S' ? BreakReason::Supervisory : (a == 'X' ? BreakReason::External : BreakReason::Unknown))));
}

void baseMMT(char mmt[])
{
    char newmmt[15] = {'1', '2', '-', '-', '-', '-', '-', '-', '-', 'P', '-', '-', '-', '-', '\0'};
    for (int i = 0; i < 15; i++) {
        mmt[i] = newmmt[i];
    }
}

#define EXECUTION_FIELD(field, type, a, b)                 \
    {                                                      \
        exec_details.field = decode_##type(a, b, message); \
    }
#define VAR_EXECUTION_FIELD(field, type, a)             \
    {                                                   \
        exec_details.field = decode_##type(a, message); \
    }

ExecutionDetails decode_executed_order(const std::vector<unsigned char> & message)
{
    ExecutionDetails exec_details;
    baseMMT(exec_details.mmt);
#include "executionfields.h"
    return exec_details;
}

#define BROKEN_FIELD(field, type, a, b)                     \
    {                                                       \
        break_details.field = decode_##type(a, b, message); \
    }
#define VAR_BROKEN_FIELD(field, type, a)                 \
    {                                                    \
        break_details.field = decode_##type(a, message); \
    }

BrokenTradeDetails decode_broken_trade(const std::vector<unsigned char> & message)
{
    BrokenTradeDetails break_details;
    baseMMT(break_details.mmt);
#include "brokenfields.h"
    return break_details;
}
