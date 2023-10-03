#if !defined(BROKEN_FIELD) || !defined(VAR_BROKEN_FIELD)
#error You need to define BROKEN_FIELD and VAR_BROKEN_FIELD macro
#else

BROKEN_FIELD(cl_ord_id, Token, 9, 14)
BROKEN_FIELD(match_number, Integer, 23, 4)
VAR_BROKEN_FIELD(reason, BreakReason, 27)
VAR_BROKEN_FIELD(mmt[1], Char, 28)
VAR_BROKEN_FIELD(mmt[2], Char, 29)
VAR_BROKEN_FIELD(mmt[10], Char, 30)

#undef BROKEN_FIELD
#undef VAR_BROKEN_FIELD

#endif
