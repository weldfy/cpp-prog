#if !defined(EXECUTION_FIELD) || !defined(VAR_EXECUTION_FIELD)
#error You need to define EXECUTION_FIELD and VAR_EXECUTION_FIELD macro
#else

EXECUTION_FIELD(cl_ord_id, Token, 9, 14)
EXECUTION_FIELD(filled_volume, Integer, 23, 4)
EXECUTION_FIELD(price, Price, 27, 4)
EXECUTION_FIELD(match_number, Integer, 32, 4)
EXECUTION_FIELD(counterpart, Token, 36, 4)
EXECUTION_FIELD(liquidity_indicator, LiquidityIndicator, 43, 4)
EXECUTION_FIELD(internalized, Bit, 43, 6)
EXECUTION_FIELD(self_trade, Bit, 43, 8)
VAR_EXECUTION_FIELD(mmt[1], Char, 40)
VAR_EXECUTION_FIELD(mmt[2], Char, 41)
VAR_EXECUTION_FIELD(mmt[10], Char, 42)

#undef EXECUTION_FIELD
#undef VAR_EXECUTION_FIELD

#endif
