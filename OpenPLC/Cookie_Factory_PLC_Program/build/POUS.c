void LOGGER_init__(LOGGER *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->TRIG,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->MSG,__STRING_LITERAL(0,""),retain)
  __INIT_VAR(data__->LEVEL,LOGLEVEL__INFO,retain)
  __INIT_VAR(data__->TRIG0,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void LOGGER_body__(LOGGER *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  if ((__GET_VAR(data__->TRIG,) && !(__GET_VAR(data__->TRIG0,)))) {
    #define GetFbVar(var,...) __GET_VAR(data__->var,__VA_ARGS__)
    #define SetFbVar(var,val,...) __SET_VAR(data__->,var,__VA_ARGS__,val)

   LogMessage(GetFbVar(LEVEL),(char*)GetFbVar(MSG, .body),GetFbVar(MSG, .len));
  
    #undef GetFbVar
    #undef SetFbVar
;
  };
  __SET_VAR(data__->,TRIG0,,__GET_VAR(data__->TRIG,));

  goto __end;

__end:
  return;
} // LOGGER_body__() 





void MIXER_RPM_FUNCTION_init__(MIXER_RPM_FUNCTION *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->OUTPUT_RPM,0,retain)
  __INIT_VAR(data__->VALVE_STATUS,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->HOPPER_WEIGHT,0,retain)
  __INIT_VAR(data__->MIXER_STATUS,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->NOISE,0,retain)
  __INIT_VAR(data__->COUNTER,0,retain)
}

// Code part
void MIXER_RPM_FUNCTION_body__(MIXER_RPM_FUNCTION *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  __SET_VAR(data__->,COUNTER,,(__GET_VAR(data__->COUNTER,) + 1));
  if ((__GET_VAR(data__->COUNTER,) > 10000)) {
    __SET_VAR(data__->,COUNTER,,0);
  };
  __SET_VAR(data__->,NOISE,,((2 == 0)?0:(__GET_VAR(data__->COUNTER,) % 2)));
  if (((__GET_VAR(data__->HOPPER_WEIGHT,) == 0.0) && (__GET_VAR(data__->MIXER_STATUS,) == 1))) {
    __SET_VAR(data__->,OUTPUT_RPM,,INT_TO_WORD(
      (BOOL)__BOOL_LITERAL(TRUE),
      NULL,
      (INT)(70 + __GET_VAR(data__->NOISE,))));
  } else if ((((__GET_VAR(data__->HOPPER_WEIGHT,) > 0.0) && (__GET_VAR(data__->MIXER_STATUS,) == 1)) && (__GET_VAR(data__->VALVE_STATUS,) == 1))) {
    __SET_VAR(data__->,OUTPUT_RPM,,INT_TO_WORD(
      (BOOL)__BOOL_LITERAL(TRUE),
      NULL,
      (INT)(35 + __GET_VAR(data__->NOISE,))));
  } else {
    __SET_VAR(data__->,OUTPUT_RPM,,INT_TO_WORD(
      (BOOL)__BOOL_LITERAL(TRUE),
      NULL,
      (INT)0));
  };

  goto __end;

__end:
  return;
} // MIXER_RPM_FUNCTION_body__() 





void VACUUM_RPM_FUNCTION_init__(VACUUM_RPM_FUNCTION *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->INPUT_RPM,0,retain)
  __INIT_VAR(data__->VACUUM_STATUS,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->OUTPUT_RPM,0,retain)
  __INIT_VAR(data__->MAX_RPM,0,retain)
  __INIT_VAR(data__->MANUAL,0,retain)
  __INIT_VAR(data__->DV,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->NOISE,0,retain)
  __INIT_VAR(data__->COUNTER,0,retain)
}

// Code part
void VACUUM_RPM_FUNCTION_body__(VACUUM_RPM_FUNCTION *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  __SET_VAR(data__->,COUNTER,,(__GET_VAR(data__->COUNTER,) + 1));
  if ((__GET_VAR(data__->COUNTER,) > 10000)) {
    __SET_VAR(data__->,COUNTER,,0);
  };
  __SET_VAR(data__->,NOISE,,((5 == 0)?0:(__GET_VAR(data__->COUNTER,) % 5)));
  if (!(__GET_VAR(data__->VACUUM_STATUS,))) {
    __SET_VAR(data__->,OUTPUT_RPM,,0);
  } else if ((__GET_VAR(data__->INPUT_RPM,) > __GET_VAR(data__->MAX_RPM,))) {
    __SET_VAR(data__->,OUTPUT_RPM,,INT_TO_WORD(
      (BOOL)__BOOL_LITERAL(TRUE),
      NULL,
      (INT)(WORD_TO_INT(
        (BOOL)__BOOL_LITERAL(TRUE),
        NULL,
        (WORD)__GET_VAR(data__->MAX_RPM,)) + __GET_VAR(data__->NOISE,))));
  } else if (((__GET_VAR(data__->DV,) == 1) && (__GET_VAR(data__->MANUAL,) == 0))) {
    __SET_VAR(data__->,OUTPUT_RPM,,INT_TO_WORD(
      (BOOL)__BOOL_LITERAL(TRUE),
      NULL,
      (INT)(1380 + __GET_VAR(data__->NOISE,))));
  } else if (((__GET_VAR(data__->DV,) == 0) && (__GET_VAR(data__->MANUAL,) == 0))) {
    __SET_VAR(data__->,OUTPUT_RPM,,INT_TO_WORD(
      (BOOL)__BOOL_LITERAL(TRUE),
      NULL,
      (INT)(1980 + __GET_VAR(data__->NOISE,))));
  } else {
    if ((((2 == 0)?0:(__GET_VAR(data__->COUNTER,) % 2)) == 1)) {
      __SET_VAR(data__->,OUTPUT_RPM,,INT_TO_WORD(
        (BOOL)__BOOL_LITERAL(TRUE),
        NULL,
        (INT)(WORD_TO_INT(
          (BOOL)__BOOL_LITERAL(TRUE),
          NULL,
          (WORD)__GET_VAR(data__->INPUT_RPM,)) + __GET_VAR(data__->NOISE,))));
    } else {
      __SET_VAR(data__->,OUTPUT_RPM,,INT_TO_WORD(
        (BOOL)__BOOL_LITERAL(TRUE),
        NULL,
        (INT)(WORD_TO_INT(
          (BOOL)__BOOL_LITERAL(TRUE),
          NULL,
          (WORD)__GET_VAR(data__->INPUT_RPM,)) - __GET_VAR(data__->NOISE,))));
    };
  };

  goto __end;

__end:
  return;
} // VACUUM_RPM_FUNCTION_body__() 





void REAL_TO_WORDS_init__(REAL_TO_WORDS *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->INPUT,0,retain)
  __INIT_VAR(data__->HIGH,0,retain)
  __INIT_VAR(data__->LOW,0,retain)
}

// Code part
void REAL_TO_WORDS_body__(REAL_TO_WORDS *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  __IL_DEFVAR_T __IL_DEFVAR;
  __IL_DEFVAR_T __IL_DEFVAR_BACK;
  #define GetFbVar(var,...) __GET_VAR(data__->var,__VA_ARGS__)
  #define SetFbVar(var,val,...) __SET_VAR(data__->,var,__VA_ARGS__,val)

      IEC_REAL var_in;
      IEC_WORD var_out[2];
      
      var_in = GetFbVar(INPUT);
      memcpy(var_out, &var_in, sizeof(var_in));
      SetFbVar(HIGH, var_out[0]);
      SetFbVar(LOW, var_out[1]);
  
  #undef GetFbVar
  #undef SetFbVar
;

  goto __end;

__end:
  return;
} // REAL_TO_WORDS_body__() 





void WORDS_TO_REAL_init__(WORDS_TO_REAL *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->HIGH,0,retain)
  __INIT_VAR(data__->LOW,0,retain)
  __INIT_VAR(data__->RESULT,0,retain)
}

// Code part
void WORDS_TO_REAL_body__(WORDS_TO_REAL *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  __IL_DEFVAR_T __IL_DEFVAR;
  __IL_DEFVAR_T __IL_DEFVAR_BACK;
  #define GetFbVar(var,...) __GET_VAR(data__->var,__VA_ARGS__)
  #define SetFbVar(var,val,...) __SET_VAR(data__->,var,__VA_ARGS__,val)

      IEC_WORD var_in[2];

      IEC_REAL var_out;

      var_in[0] = GetFbVar(HIGH);

      var_in[1] = GetFbVar(LOW);

      memcpy(&var_out, var_in, sizeof(var_out));

      SetFbVar(RESULT, var_out);
  
  #undef GetFbVar
  #undef SetFbVar
;

  goto __end;

__end:
  return;
} // WORDS_TO_REAL_body__() 





void FLOUR_ALARM_CHECK_init__(FLOUR_ALARM_CHECK *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->FLOUR_WEIGHT,0,retain)
  __INIT_VAR(data__->ALARM,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void FLOUR_ALARM_CHECK_body__(FLOUR_ALARM_CHECK *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  if ((__GET_VAR(data__->FLOUR_WEIGHT,) <= 100.0)) {
    __SET_VAR(data__->,ALARM,,__BOOL_LITERAL(TRUE));
  } else {
    __SET_VAR(data__->,ALARM,,__BOOL_LITERAL(FALSE));
  };

  goto __end;

__end:
  return;
} // FLOUR_ALARM_CHECK_body__() 





void SUGAR_ALARM_CHECK_init__(SUGAR_ALARM_CHECK *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->SUGAR_WEIGHT,0,retain)
  __INIT_VAR(data__->ALARM,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void SUGAR_ALARM_CHECK_body__(SUGAR_ALARM_CHECK *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  if ((__GET_VAR(data__->SUGAR_WEIGHT,) <= 100.0)) {
    __SET_VAR(data__->,ALARM,,__BOOL_LITERAL(TRUE));
  } else {
    __SET_VAR(data__->,ALARM,,__BOOL_LITERAL(FALSE));
  };

  goto __end;

__end:
  return;
} // SUGAR_ALARM_CHECK_body__() 





void HOPPER_ALARM_CHECK_init__(HOPPER_ALARM_CHECK *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->HOPPER_WEIGHT,0,retain)
  __INIT_VAR(data__->ALARM,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void HOPPER_ALARM_CHECK_body__(HOPPER_ALARM_CHECK *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  if ((__GET_VAR(data__->HOPPER_WEIGHT,) <= 10.0)) {
    __SET_VAR(data__->,ALARM,,__BOOL_LITERAL(TRUE));
  } else {
    __SET_VAR(data__->,ALARM,,__BOOL_LITERAL(FALSE));
  };

  goto __end;

__end:
  return;
} // HOPPER_ALARM_CHECK_body__() 





void VACUUM_ALARM_RPM_init__(VACUUM_ALARM_RPM *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->DV,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CFM,0,retain)
  __INIT_VAR(data__->VACUUM_RPM,0,retain)
  __INIT_VAR(data__->ALARM,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void VACUUM_ALARM_RPM_body__(VACUUM_ALARM_RPM *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  __SET_VAR(data__->,CFM,,((((-1.275 * 0.0000001) * (WORD_TO_REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (WORD)__GET_VAR(data__->VACUUM_RPM,)) * WORD_TO_REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (WORD)__GET_VAR(data__->VACUUM_RPM,)))) + (0.7896 * WORD_TO_REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (WORD)__GET_VAR(data__->VACUUM_RPM,)))) - 365.43));
  if (((__GET_VAR(data__->DV,) == 1) && (__GET_VAR(data__->CFM,) < 687.0))) {
    __SET_VAR(data__->,ALARM,,__BOOL_LITERAL(TRUE));
  } else if (((__GET_VAR(data__->DV,) == 0) && (__GET_VAR(data__->CFM,) < 1178.0))) {
    __SET_VAR(data__->,ALARM,,__BOOL_LITERAL(TRUE));
  } else {
    __SET_VAR(data__->,ALARM,,__BOOL_LITERAL(FALSE));
  };

  goto __end;

__end:
  return;
} // VACUUM_ALARM_RPM_body__() 





void PAPERCUTTER_CPM_FUNCTION_init__(PAPERCUTTER_CPM_FUNCTION *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ROWS,0,retain)
  __INIT_VAR(data__->PAPER_CUTTER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->COOKIE_DIAMETER,0,retain)
  __INIT_VAR(data__->COOKIE_MARGIN,0,retain)
  __INIT_VAR(data__->CONVEYOR_FPM,0,retain)
  __INIT_VAR(data__->FLOAT_CPM,0,retain)
  __INIT_VAR(data__->PAPERCUT_CPM,0,retain)
}

// Code part
void PAPERCUTTER_CPM_FUNCTION_body__(PAPERCUTTER_CPM_FUNCTION *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  if (__GET_VAR(data__->PAPER_CUTTER,)) {
    __SET_VAR(data__->,ROWS,,5.0);
    __SET_VAR(data__->,FLOAT_CPM,,((__GET_VAR(data__->CONVEYOR_FPM,) * 12.0) / (__GET_VAR(data__->ROWS,) * (__GET_VAR(data__->COOKIE_DIAMETER,) + __GET_VAR(data__->COOKIE_MARGIN,)))));
    __SET_VAR(data__->,PAPERCUT_CPM,,REAL_TO_WORD(
      (BOOL)__BOOL_LITERAL(TRUE),
      NULL,
      (REAL)__GET_VAR(data__->FLOAT_CPM,)));
  } else {
    __SET_VAR(data__->,PAPERCUT_CPM,,0);
  };

  goto __end;

__end:
  return;
} // PAPERCUTTER_CPM_FUNCTION_body__() 





void CONVEYOR_FPM_FUNCTION_init__(CONVEYOR_FPM_FUNCTION *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->WIRECUT_CPM,0,retain)
  __INIT_VAR(data__->COOKIE_MARGIN,0,retain)
  __INIT_VAR(data__->COOKIE_DIAMETER,0,retain)
  __INIT_VAR(data__->CONVEYOR_FPM,0,retain)
}

// Code part
void CONVEYOR_FPM_FUNCTION_body__(CONVEYOR_FPM_FUNCTION *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  __SET_VAR(data__->,CONVEYOR_FPM,,((WORD_TO_REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (WORD)__GET_VAR(data__->WIRECUT_CPM,)) * (__GET_VAR(data__->COOKIE_MARGIN,) + __GET_VAR(data__->COOKIE_DIAMETER,))) / 12.0));

  goto __end;

__end:
  return;
} // CONVEYOR_FPM_FUNCTION_body__() 





void WIRECUT_CPM_FUNCTION_init__(WIRECUT_CPM_FUNCTION *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->WIRECUT_CPM,0,retain)
  __INIT_VAR(data__->WIRECUTTER,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void WIRECUT_CPM_FUNCTION_body__(WIRECUT_CPM_FUNCTION *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  if (__GET_VAR(data__->WIRECUTTER,)) {
    __SET_VAR(data__->,WIRECUT_CPM,,200);
  } else {
    __SET_VAR(data__->,WIRECUT_CPM,,0);
  };

  goto __end;

__end:
  return;
} // WIRECUT_CPM_FUNCTION_body__() 





static inline INT __TAG_DISPLAY_MOVE__INT__INT1(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE174_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE174_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT2(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE120_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE120_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT3(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE133_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE133_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT4(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE144_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE144_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT5(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE156_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE156_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT6(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE163_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE163_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT7(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE188_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE188_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT8(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE205_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE205_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT9(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE231_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE231_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT10(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE281_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE281_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT11(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE228_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE228_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT12(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE241_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE241_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_ADD__INT__INT13(BOOL EN,
  UINT __PARAM_COUNT,
  INT IN1,
  INT IN2,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_ADD248_ENO,);
  __res = ADD__INT__INT(EN,
    &__TMP_ENO,
    __PARAM_COUNT,
    IN1,
    IN2);
  __SET_VAR(,data__->_TMP_ADD248_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT14(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE252_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE252_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT15(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE224_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE224_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT16(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE290_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE290_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT17(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE310_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE310_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT18(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE321_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE321_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT19(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE336_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE336_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT20(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE345_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE345_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT21(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE238_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE238_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT22(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE229_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE229_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT23(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE262_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE262_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT24(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE303_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE303_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT25(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE362_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE362_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT26(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE186_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE186_ENO,,__TMP_ENO);
  return __res;
}

static inline INT __TAG_DISPLAY_MOVE__INT__INT27(BOOL EN,
  INT IN,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_MOVE273_ENO,);
  __res = MOVE__INT__INT(EN,
    &__TMP_ENO,
    IN);
  __SET_VAR(,data__->_TMP_MOVE273_ENO,,__TMP_ENO);
  return __res;
}

void TAG_DISPLAY_init__(TAG_DISPLAY *data__, BOOL retain) {
  __INIT_VAR(data__->LCS_1_LOW,0,retain)
  __INIT_VAR(data__->LCS_1_HIGH,0,retain)
  __INIT_VAR(data__->LCS_2_LOW,0,retain)
  __INIT_VAR(data__->LCS_2_HIGH,0,retain)
  __INIT_VAR(data__->LCS_3_LOW,0,retain)
  __INIT_VAR(data__->LCS_3_HIGH,0,retain)
  __INIT_VAR(data__->LCS_4_LOW,0,retain)
  __INIT_VAR(data__->LCS_4_HIGH,0,retain)
  __INIT_VAR(data__->LCF_1_LOW,0,retain)
  __INIT_VAR(data__->LCF_1_HIGH,0,retain)
  __INIT_VAR(data__->LCF_2_LOW,0,retain)
  __INIT_VAR(data__->LCF_2_HIGH,0,retain)
  __INIT_VAR(data__->LCF_3_LOW,0,retain)
  __INIT_VAR(data__->LCF_3_HIGH,0,retain)
  __INIT_VAR(data__->LCF_4_LOW,0,retain)
  __INIT_VAR(data__->LCF_4_HIGH,0,retain)
  __INIT_VAR(data__->LCH_HIGH,0,retain)
  __INIT_VAR(data__->LCH_LOW,0,retain)
  __INIT_VAR(data__->LCM_HIGH,0,retain)
  __INIT_VAR(data__->LCM_LOW,0,retain)
  __INIT_VAR(data__->TROUGH_WEIGHT_HIGH,0,retain)
  __INIT_VAR(data__->TROUGH_WEIGHT_LOW,0,retain)
  __INIT_VAR(data__->VACUUM_RPM,0,retain)
  __INIT_VAR(data__->MIXER_RPM,0,retain)
  __INIT_VAR(data__->WIRECUT_CPM,0,retain)
  __INIT_VAR(data__->PAPERCUT_CPM,0,retain)
  __INIT_VAR(data__->CONVEYOR_FPM,0,retain)
  __INIT_VAR(data__->CONVEYOR_FPM_HIGH,0,retain)
  __INIT_VAR(data__->CONVEYOR_FPM_LOW,0,retain)
  __INIT_VAR(data__->LCS_1,0,retain)
  __INIT_VAR(data__->LCS_2,0,retain)
  __INIT_VAR(data__->LCS_3,0,retain)
  __INIT_VAR(data__->LCS_4,0,retain)
  __INIT_VAR(data__->LCF_1,0,retain)
  __INIT_VAR(data__->LCF_2,0,retain)
  __INIT_VAR(data__->LCF_3,0,retain)
  __INIT_VAR(data__->LCF_4,0,retain)
  __INIT_VAR(data__->LCH,0.0,retain)
  __INIT_VAR(data__->LCM,0.0,retain)
  __INIT_VAR(data__->TROUGH_WEIGHT,0.0,retain)
  __INIT_VAR(data__->FLOUR_WEIGHT,0.0,retain)
  __INIT_VAR(data__->FLOUR_WEIGHT_HIGH,0,retain)
  __INIT_VAR(data__->FLOUR_WEIGHT_LOW,0,retain)
  __INIT_VAR(data__->SUGAR_WEIGHT,0.0,retain)
  __INIT_VAR(data__->SUGAR_WEIGHT_HIGH,0,retain)
  __INIT_VAR(data__->SUGAR_WEIGHT_LOW,0,retain)
  __INIT_VAR(data__->TUNNEL_TEMP,0,retain)
  __INIT_VAR(data__->TUNNEL_TEMP_HIGH,0,retain)
  __INIT_VAR(data__->TUNNEL_TEMP_LOW,0,retain)
  __INIT_VAR(data__->NITROGEN_VOLUME,0,retain)
  __INIT_VAR(data__->NITROGEN_VOLUME_HIGH,0,retain)
  __INIT_VAR(data__->NITROGEN_VOLUME_LOW,0,retain)
  __INIT_VAR(data__->RV_1,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->RV_2,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->RV_3,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->DV,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->VACUUM,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->MIXER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->FLOUR_ALARM,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->SUGAR_ALARM,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->HOPPER_ALARM,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->VACUUM_ALARM,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->VACUUM_MANUAL,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->WIRECUTTER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->TROUGH_TRANSFER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CONVEYOR,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->PAPER_CUTTER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->GV_1,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->FAN_1,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->FAN_2,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->FAN_3,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->EXHAUST_FAN,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CFM,0,retain)
  __INIT_VAR(data__->RESET,__BOOL_LITERAL(FALSE),retain)
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL0,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL1,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL2,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL3,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL4,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL5,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL6,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL7,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL8,retain);
  FLOUR_ALARM_CHECK_init__(&data__->FLOUR_ALARM_CHECK0,retain);
  SUGAR_ALARM_CHECK_init__(&data__->SUGAR_ALARM_CHECK0,retain);
  HOPPER_ALARM_CHECK_init__(&data__->HOPPER_ALARM_CHECK0,retain);
  VACUUM_ALARM_RPM_init__(&data__->VACUUM_ALARM_RPM0,retain);
  REAL_TO_WORDS_init__(&data__->REAL_TO_WORDS0,retain);
  REAL_TO_WORDS_init__(&data__->REAL_TO_WORDS1,retain);
  __INIT_VAR(data__->VACUUM_MAX_RPM,3600,retain)
  __INIT_VAR(data__->MIXER_MAX_RPM,80,retain)
  MIXER_RPM_FUNCTION_init__(&data__->MIXER_RPM_FUNCTION0,retain);
  VACUUM_RPM_FUNCTION_init__(&data__->VACUUM_RPM_FUNCTION0,retain);
  __INIT_VAR(data__->COOKIE_DIAMETER,1.5,retain)
  __INIT_VAR(data__->COOKIE_MARGIN,0.5,retain)
  PAPERCUTTER_CPM_FUNCTION_init__(&data__->PAPERCUTTER_CPM_FUNCTION0,retain);
  CONVEYOR_FPM_FUNCTION_init__(&data__->CONVEYOR_FPM_FUNCTION0,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL9,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL10,retain);
  REAL_TO_WORDS_init__(&data__->REAL_TO_WORDS2,retain);
  WIRECUT_CPM_FUNCTION_init__(&data__->WIRECUT_CPM_FUNCTION0,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL11,retain);
  WORDS_TO_REAL_init__(&data__->WORDS_TO_REAL12,retain);
  __INIT_VAR(data__->BOX_MAKER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->TAPER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->LABELER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->PALLETIZER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->PALLETIZER_GRABBING,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->WRAPPER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->WRAPPER_CONVEYING,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CONVEYOR_1,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->CONVEYOR_2,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->CONVEYOR_3,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->CONVEYOR_4,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->CONVEYOR_5,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->PS_1,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->PS_2,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->PS_3,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->PS_4,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->PS_5,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->PS_6,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->BAGGER,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->PALLETIZER_ROTATION,180,retain)
  __INIT_VAR(data__->BOX_COUNT,0,retain)
  __INIT_VAR(data__->WRAPPER_WRAPPING,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->BOXING,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->TAPING,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CONVEYING_1,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CONVEYING_2,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CONVEYING_3,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CONVEYING_4,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CONVEYING_5,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->PALLETIZER_MOVING,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->BAGGING,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->MACHINE_1,0,retain)
  __INIT_VAR(data__->MACHINE_2,0,retain)
  __INIT_VAR(data__->MACHINE_3,0,retain)
  __INIT_VAR(data__->MACHINE_4,0,retain)
  __INIT_VAR(data__->STATE_0,0,retain)
  __INIT_VAR(data__->STATE_1,1,retain)
  __INIT_VAR(data__->STATE_2,2,retain)
  __INIT_VAR(data__->STATE_3,3,retain)
  __INIT_VAR(data__->STATE_4,4,retain)
  __INIT_VAR(data__->STATE_5,5,retain)
  __INIT_VAR(data__->STATE_6,6,retain)
  __INIT_VAR(data__->POS_1,225,retain)
  __INIT_VAR(data__->POS_2,180,retain)
  __INIT_VAR(data__->POS_3,130,retain)
  __INIT_VAR(data__->ROE_1,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->ROE_2,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->ROE_3,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_ADD37_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD38_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD41_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD39_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD40_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD43_OUT,0,retain)
  __INIT_VAR(data__->MIXER_RPM_FUNCTION0_COUNTER,0,retain)
  __INIT_VAR(data__->VACUUM_RPM_FUNCTION0_COUNTER,0,retain)
  __INIT_VAR(data__->_TMP_EQ167_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE174_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE174_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ113_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE120_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE120_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ125_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE133_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE133_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ130_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE144_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE144_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ141_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE156_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE156_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ154_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE163_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE163_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ171_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE188_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE188_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ197_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE205_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE205_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ219_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE231_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE231_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ243_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE281_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE281_OUT,0,retain)
  __INIT_VAR(data__->_TMP_MOVE228_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE228_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ264_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE241_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE241_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD248_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_ADD248_OUT,0,retain)
  __INIT_VAR(data__->_TMP_MOVE252_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE252_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ256_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_GE263_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_AND353_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_EQ210_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE224_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE224_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ270_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE290_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE290_OUT,0,retain)
  __INIT_VAR(data__->_TMP_MOVE310_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE310_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ314_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE321_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE321_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ145_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE336_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE336_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ347_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE345_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE345_OUT,0,retain)
  __INIT_VAR(data__->_TMP_MOVE238_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE238_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ204_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_EQ232_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE229_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE229_OUT,0,retain)
  __INIT_VAR(data__->_TMP_MOVE262_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE262_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ341_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE303_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE303_OUT,0,retain)
  __INIT_VAR(data__->_TMP_MOVE362_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE362_OUT,0,retain)
  __INIT_VAR(data__->_TMP_MOVE186_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE186_OUT,0,retain)
  __INIT_VAR(data__->_TMP_MOVE273_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_MOVE273_OUT,0,retain)
}

// Code part
void TAG_DISPLAY_body__(TAG_DISPLAY *data__) {
  // Initialise TEMP variables

  __SET_VAR(data__->WORDS_TO_REAL0.,HIGH,,__GET_VAR(data__->LCF_1_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL0.,LOW,,__GET_VAR(data__->LCF_1_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL0);
  __SET_VAR(data__->,LCF_1,,__GET_VAR(data__->WORDS_TO_REAL0.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL1.,HIGH,,__GET_VAR(data__->LCF_2_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL1.,LOW,,__GET_VAR(data__->LCF_2_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL1);
  __SET_VAR(data__->,LCF_2,,__GET_VAR(data__->WORDS_TO_REAL1.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL2.,HIGH,,__GET_VAR(data__->LCF_3_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL2.,LOW,,__GET_VAR(data__->LCF_3_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL2);
  __SET_VAR(data__->,LCF_3,,__GET_VAR(data__->WORDS_TO_REAL2.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL3.,HIGH,,__GET_VAR(data__->LCF_4_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL3.,LOW,,__GET_VAR(data__->LCF_4_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL3);
  __SET_VAR(data__->,LCF_4,,__GET_VAR(data__->WORDS_TO_REAL3.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL4.,HIGH,,__GET_VAR(data__->LCS_1_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL4.,LOW,,__GET_VAR(data__->LCS_1_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL4);
  __SET_VAR(data__->,LCS_1,,__GET_VAR(data__->WORDS_TO_REAL4.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL5.,HIGH,,__GET_VAR(data__->LCS_2_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL5.,LOW,,__GET_VAR(data__->LCS_2_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL5);
  __SET_VAR(data__->,LCS_2,,__GET_VAR(data__->WORDS_TO_REAL5.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL6.,HIGH,,__GET_VAR(data__->LCS_3_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL6.,LOW,,__GET_VAR(data__->LCS_3_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL6);
  __SET_VAR(data__->,LCS_3,,__GET_VAR(data__->WORDS_TO_REAL6.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL7.,HIGH,,__GET_VAR(data__->LCS_4_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL7.,LOW,,__GET_VAR(data__->LCS_4_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL7);
  __SET_VAR(data__->,LCS_4,,__GET_VAR(data__->WORDS_TO_REAL7.RESULT,));
  __SET_VAR(data__->,_TMP_ADD37_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_VAR(data__->LCF_1,),
    (REAL)__GET_VAR(data__->LCF_2,)));
  __SET_VAR(data__->,_TMP_ADD38_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_VAR(data__->LCF_3,),
    (REAL)__GET_VAR(data__->LCF_4,)));
  __SET_VAR(data__->,_TMP_ADD41_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_VAR(data__->_TMP_ADD37_OUT,),
    (REAL)__GET_VAR(data__->_TMP_ADD38_OUT,)));
  __SET_VAR(data__->,FLOUR_WEIGHT,,__GET_VAR(data__->_TMP_ADD41_OUT,));
  __SET_VAR(data__->FLOUR_ALARM_CHECK0.,FLOUR_WEIGHT,,__GET_VAR(data__->FLOUR_WEIGHT,));
  FLOUR_ALARM_CHECK_body__(&data__->FLOUR_ALARM_CHECK0);
  __SET_VAR(data__->,FLOUR_ALARM,,__GET_VAR(data__->FLOUR_ALARM_CHECK0.ALARM,));
  __SET_VAR(data__->,_TMP_ADD39_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_VAR(data__->LCS_1,),
    (REAL)__GET_VAR(data__->LCS_2,)));
  __SET_VAR(data__->,_TMP_ADD40_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_VAR(data__->LCS_3,),
    (REAL)__GET_VAR(data__->LCS_4,)));
  __SET_VAR(data__->,_TMP_ADD43_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_VAR(data__->_TMP_ADD39_OUT,),
    (REAL)__GET_VAR(data__->_TMP_ADD40_OUT,)));
  __SET_VAR(data__->,SUGAR_WEIGHT,,__GET_VAR(data__->_TMP_ADD43_OUT,));
  __SET_VAR(data__->WORDS_TO_REAL8.,HIGH,,__GET_VAR(data__->LCH_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL8.,LOW,,__GET_VAR(data__->LCH_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL8);
  __SET_VAR(data__->,LCH,,__GET_VAR(data__->WORDS_TO_REAL8.RESULT,));
  __SET_VAR(data__->HOPPER_ALARM_CHECK0.,HOPPER_WEIGHT,,__GET_VAR(data__->LCH,));
  HOPPER_ALARM_CHECK_body__(&data__->HOPPER_ALARM_CHECK0);
  __SET_VAR(data__->,HOPPER_ALARM,,__GET_VAR(data__->HOPPER_ALARM_CHECK0.ALARM,));
  __SET_VAR(data__->VACUUM_ALARM_RPM0.,DV,,__GET_VAR(data__->DV,));
  __SET_VAR(data__->VACUUM_ALARM_RPM0.,VACUUM_RPM,,__GET_VAR(data__->VACUUM_RPM,));
  VACUUM_ALARM_RPM_body__(&data__->VACUUM_ALARM_RPM0);
  __SET_VAR(data__->,VACUUM_ALARM,,__GET_VAR(data__->VACUUM_ALARM_RPM0.ALARM,));
  __SET_VAR(data__->,CFM,,__GET_VAR(data__->VACUUM_ALARM_RPM0.CFM,));
  __SET_VAR(data__->REAL_TO_WORDS0.,INPUT,,__GET_VAR(data__->FLOUR_WEIGHT,));
  REAL_TO_WORDS_body__(&data__->REAL_TO_WORDS0);
  __SET_VAR(data__->,FLOUR_WEIGHT_HIGH,,__GET_VAR(data__->REAL_TO_WORDS0.HIGH,));
  __SET_VAR(data__->,FLOUR_WEIGHT_LOW,,__GET_VAR(data__->REAL_TO_WORDS0.LOW,));
  __SET_VAR(data__->REAL_TO_WORDS1.,INPUT,,__GET_VAR(data__->SUGAR_WEIGHT,));
  REAL_TO_WORDS_body__(&data__->REAL_TO_WORDS1);
  __SET_VAR(data__->,SUGAR_WEIGHT_HIGH,,__GET_VAR(data__->REAL_TO_WORDS1.HIGH,));
  __SET_VAR(data__->,SUGAR_WEIGHT_LOW,,__GET_VAR(data__->REAL_TO_WORDS1.LOW,));
  __SET_VAR(data__->SUGAR_ALARM_CHECK0.,SUGAR_WEIGHT,,__GET_VAR(data__->SUGAR_WEIGHT,));
  SUGAR_ALARM_CHECK_body__(&data__->SUGAR_ALARM_CHECK0);
  __SET_VAR(data__->,SUGAR_ALARM,,__GET_VAR(data__->SUGAR_ALARM_CHECK0.ALARM,));
  __SET_VAR(data__->,MIXER_RPM_FUNCTION0_COUNTER,,__GET_VAR(data__->MIXER_RPM_FUNCTION0.COUNTER,));
  __SET_VAR(data__->MIXER_RPM_FUNCTION0.,VALVE_STATUS,,__GET_VAR(data__->RV_3,));
  __SET_VAR(data__->MIXER_RPM_FUNCTION0.,HOPPER_WEIGHT,,__GET_VAR(data__->LCH,));
  __SET_VAR(data__->MIXER_RPM_FUNCTION0.,MIXER_STATUS,,__GET_VAR(data__->MIXER,));
  __SET_VAR(data__->MIXER_RPM_FUNCTION0.,COUNTER,,__GET_VAR(data__->MIXER_RPM_FUNCTION0_COUNTER,));
  MIXER_RPM_FUNCTION_body__(&data__->MIXER_RPM_FUNCTION0);
  __SET_VAR(data__->,MIXER_RPM_FUNCTION0_COUNTER,,__GET_VAR(data__->MIXER_RPM_FUNCTION0.COUNTER));
  __SET_VAR(data__->,MIXER_RPM,,__GET_VAR(data__->MIXER_RPM_FUNCTION0.OUTPUT_RPM,));
  __SET_VAR(data__->,VACUUM_RPM_FUNCTION0_COUNTER,,__GET_VAR(data__->VACUUM_RPM_FUNCTION0.COUNTER,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,INPUT_RPM,,__GET_VAR(data__->VACUUM_RPM,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,VACUUM_STATUS,,__GET_VAR(data__->VACUUM,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,MAX_RPM,,__GET_VAR(data__->VACUUM_MAX_RPM,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,MANUAL,,__GET_VAR(data__->VACUUM_MANUAL,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,DV,,__GET_VAR(data__->DV,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,COUNTER,,__GET_VAR(data__->VACUUM_RPM_FUNCTION0_COUNTER,));
  VACUUM_RPM_FUNCTION_body__(&data__->VACUUM_RPM_FUNCTION0);
  __SET_VAR(data__->,VACUUM_RPM_FUNCTION0_COUNTER,,__GET_VAR(data__->VACUUM_RPM_FUNCTION0.COUNTER));
  __SET_VAR(data__->,VACUUM_RPM,,__GET_VAR(data__->VACUUM_RPM_FUNCTION0.OUTPUT_RPM,));
  __SET_VAR(data__->WORDS_TO_REAL9.,HIGH,,__GET_VAR(data__->TROUGH_WEIGHT_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL9.,LOW,,__GET_VAR(data__->TROUGH_WEIGHT_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL9);
  __SET_VAR(data__->,TROUGH_WEIGHT,,__GET_VAR(data__->WORDS_TO_REAL9.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL10.,HIGH,,__GET_VAR(data__->LCM_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL10.,LOW,,__GET_VAR(data__->LCM_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL10);
  __SET_VAR(data__->,LCM,,__GET_VAR(data__->WORDS_TO_REAL10.RESULT,));
  __SET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.,PAPER_CUTTER,,__GET_VAR(data__->PAPER_CUTTER,));
  __SET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.,COOKIE_DIAMETER,,__GET_VAR(data__->COOKIE_DIAMETER,));
  __SET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.,COOKIE_MARGIN,,__GET_VAR(data__->COOKIE_MARGIN,));
  __SET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.,CONVEYOR_FPM,,__GET_VAR(data__->CONVEYOR_FPM,));
  PAPERCUTTER_CPM_FUNCTION_body__(&data__->PAPERCUTTER_CPM_FUNCTION0);
  __SET_VAR(data__->,PAPERCUT_CPM,,__GET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.PAPERCUT_CPM,));
  __SET_VAR(data__->CONVEYOR_FPM_FUNCTION0.,WIRECUT_CPM,,__GET_VAR(data__->WIRECUT_CPM,));
  __SET_VAR(data__->CONVEYOR_FPM_FUNCTION0.,COOKIE_MARGIN,,__GET_VAR(data__->COOKIE_MARGIN,));
  __SET_VAR(data__->CONVEYOR_FPM_FUNCTION0.,COOKIE_DIAMETER,,__GET_VAR(data__->COOKIE_DIAMETER,));
  CONVEYOR_FPM_FUNCTION_body__(&data__->CONVEYOR_FPM_FUNCTION0);
  __SET_VAR(data__->,CONVEYOR_FPM,,__GET_VAR(data__->CONVEYOR_FPM_FUNCTION0.CONVEYOR_FPM,));
  __SET_VAR(data__->REAL_TO_WORDS2.,INPUT,,__GET_VAR(data__->CONVEYOR_FPM,));
  REAL_TO_WORDS_body__(&data__->REAL_TO_WORDS2);
  __SET_VAR(data__->,CONVEYOR_FPM_HIGH,,__GET_VAR(data__->REAL_TO_WORDS2.HIGH,));
  __SET_VAR(data__->,CONVEYOR_FPM_LOW,,__GET_VAR(data__->REAL_TO_WORDS2.LOW,));
  __SET_VAR(data__->WIRECUT_CPM_FUNCTION0.,WIRECUT_CPM,,__GET_VAR(data__->WIRECUT_CPM,));
  __SET_VAR(data__->WIRECUT_CPM_FUNCTION0.,WIRECUTTER,,__GET_VAR(data__->WIRECUTTER,));
  WIRECUT_CPM_FUNCTION_body__(&data__->WIRECUT_CPM_FUNCTION0);
  __SET_VAR(data__->,WIRECUT_CPM,,__GET_VAR(data__->WIRECUT_CPM_FUNCTION0.WIRECUT_CPM));
  __SET_VAR(data__->,WIRECUT_CPM,,__GET_VAR(data__->WIRECUT_CPM_FUNCTION0.WIRECUT_CPM,));
  __SET_VAR(data__->WORDS_TO_REAL11.,HIGH,,__GET_VAR(data__->NITROGEN_VOLUME_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL11.,LOW,,__GET_VAR(data__->NITROGEN_VOLUME_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL11);
  __SET_VAR(data__->,NITROGEN_VOLUME,,__GET_VAR(data__->WORDS_TO_REAL11.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL12.,HIGH,,__GET_VAR(data__->TUNNEL_TEMP_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL12.,LOW,,__GET_VAR(data__->TUNNEL_TEMP_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL12);
  __SET_VAR(data__->,TUNNEL_TEMP,,__GET_VAR(data__->WORDS_TO_REAL12.RESULT,));
  __SET_VAR(data__->,_TMP_EQ167_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_1,),
    (INT)__GET_VAR(data__->STATE_0,)));
  if ((__GET_VAR(data__->BOX_MAKER,) && __GET_VAR(data__->_TMP_EQ167_OUT,))) {
    __SET_VAR(data__->,BOXING,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE174_OUT,,__TAG_DISPLAY_MOVE__INT__INT1(
    (BOOL)(__GET_VAR(data__->BOX_MAKER,) && __GET_VAR(data__->_TMP_EQ167_OUT,)),
    (INT)__GET_VAR(data__->STATE_1,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE174_ENO,)) {
    __SET_VAR(data__->,MACHINE_1,,__GET_VAR(data__->_TMP_MOVE174_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ113_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_1,),
    (INT)__GET_VAR(data__->STATE_1,)));
  if (((__GET_VAR(data__->CONVEYOR_1,) && !(__GET_VAR(data__->BOXING,))) && __GET_VAR(data__->_TMP_EQ113_OUT,))) {
    __SET_VAR(data__->,CONVEYING_1,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE120_OUT,,__TAG_DISPLAY_MOVE__INT__INT2(
    (BOOL)((__GET_VAR(data__->CONVEYOR_1,) && !(__GET_VAR(data__->BOXING,))) && __GET_VAR(data__->_TMP_EQ113_OUT,)),
    (INT)__GET_VAR(data__->STATE_2,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE120_ENO,)) {
    __SET_VAR(data__->,MACHINE_1,,__GET_VAR(data__->_TMP_MOVE120_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ125_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_1,),
    (INT)__GET_VAR(data__->STATE_2,)));
  if (((__GET_VAR(data__->BAGGER,) && __GET_VAR(data__->PS_1,)) && __GET_VAR(data__->_TMP_EQ125_OUT,))) {
    __SET_VAR(data__->,BAGGING,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE133_OUT,,__TAG_DISPLAY_MOVE__INT__INT3(
    (BOOL)((__GET_VAR(data__->BAGGER,) && __GET_VAR(data__->PS_1,)) && __GET_VAR(data__->_TMP_EQ125_OUT,)),
    (INT)__GET_VAR(data__->STATE_3,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE133_ENO,)) {
    __SET_VAR(data__->,MACHINE_1,,__GET_VAR(data__->_TMP_MOVE133_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ130_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_1,),
    (INT)__GET_VAR(data__->STATE_3,)));
  __SET_VAR(data__->,_TMP_MOVE144_OUT,,__TAG_DISPLAY_MOVE__INT__INT4(
    (BOOL)((!(__GET_VAR(data__->BAGGING,)) && __GET_VAR(data__->CONVEYOR_2,)) && __GET_VAR(data__->_TMP_EQ130_OUT,)),
    (INT)__GET_VAR(data__->STATE_4,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE144_ENO,)) {
    __SET_VAR(data__->,MACHINE_1,,__GET_VAR(data__->_TMP_MOVE144_OUT,));
  };
  if (((!(__GET_VAR(data__->BAGGING,)) && __GET_VAR(data__->CONVEYOR_2,)) && __GET_VAR(data__->_TMP_EQ130_OUT,))) {
    __SET_VAR(data__->,CONVEYING_2,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_EQ141_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_1,),
    (INT)__GET_VAR(data__->STATE_5,)));
  __SET_VAR(data__->,_TMP_MOVE156_OUT,,__TAG_DISPLAY_MOVE__INT__INT5(
    (BOOL)(!(__GET_VAR(data__->PS_2,)) && __GET_VAR(data__->_TMP_EQ141_OUT,)),
    (INT)__GET_VAR(data__->STATE_0,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE156_ENO,)) {
    __SET_VAR(data__->,MACHINE_1,,__GET_VAR(data__->_TMP_MOVE156_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ154_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_2,),
    (INT)__GET_VAR(data__->STATE_0,)));
  if (((__GET_VAR(data__->PS_3,) && __GET_VAR(data__->CONVEYOR_3,)) && __GET_VAR(data__->_TMP_EQ154_OUT,))) {
    __SET_VAR(data__->,CONVEYING_3,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE163_OUT,,__TAG_DISPLAY_MOVE__INT__INT6(
    (BOOL)((__GET_VAR(data__->PS_3,) && __GET_VAR(data__->CONVEYOR_3,)) && __GET_VAR(data__->_TMP_EQ154_OUT,)),
    (INT)__GET_VAR(data__->STATE_1,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE163_ENO,)) {
    __SET_VAR(data__->,MACHINE_2,,__GET_VAR(data__->_TMP_MOVE163_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ171_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_2,),
    (INT)__GET_VAR(data__->STATE_1,)));
  if (((__GET_VAR(data__->TAPER,) && __GET_VAR(data__->PS_4,)) && __GET_VAR(data__->_TMP_EQ171_OUT,))) {
    __SET_VAR(data__->,TAPING,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE188_OUT,,__TAG_DISPLAY_MOVE__INT__INT7(
    (BOOL)((__GET_VAR(data__->TAPER,) && __GET_VAR(data__->PS_4,)) && __GET_VAR(data__->_TMP_EQ171_OUT,)),
    (INT)__GET_VAR(data__->STATE_2,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE188_ENO,)) {
    __SET_VAR(data__->,MACHINE_2,,__GET_VAR(data__->_TMP_MOVE188_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ197_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_2,),
    (INT)__GET_VAR(data__->STATE_2,)));
  if ((((__GET_VAR(data__->CONVEYOR_4,) && !(__GET_VAR(data__->TAPING,))) && !(__GET_VAR(data__->PS_5,))) && __GET_VAR(data__->_TMP_EQ197_OUT,))) {
    __SET_VAR(data__->,CONVEYING_4,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE205_OUT,,__TAG_DISPLAY_MOVE__INT__INT8(
    (BOOL)(((__GET_VAR(data__->CONVEYOR_4,) && !(__GET_VAR(data__->TAPING,))) && !(__GET_VAR(data__->PS_5,))) && __GET_VAR(data__->_TMP_EQ197_OUT,)),
    (INT)__GET_VAR(data__->STATE_0,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE205_ENO,)) {
    __SET_VAR(data__->,MACHINE_2,,__GET_VAR(data__->_TMP_MOVE205_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ219_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_3,),
    (INT)__GET_VAR(data__->STATE_2,)));
  __SET_VAR(data__->,_TMP_MOVE231_OUT,,__TAG_DISPLAY_MOVE__INT__INT9(
    (BOOL)(__GET_VAR(data__->PALLETIZER,) && __GET_VAR(data__->_TMP_EQ219_OUT,)),
    (INT)__GET_VAR(data__->STATE_3,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE231_ENO,)) {
    __SET_VAR(data__->,MACHINE_3,,__GET_VAR(data__->_TMP_MOVE231_OUT,));
  };
  if ((__GET_VAR(data__->PALLETIZER,) && __GET_VAR(data__->_TMP_EQ219_OUT,))) {
    __SET_VAR(data__->,PALLETIZER_MOVING,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_EQ243_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_3,),
    (INT)__GET_VAR(data__->STATE_4,)));
  if ((((!(__GET_VAR(data__->WRAPPER_WRAPPING,)) && !(__GET_VAR(data__->WRAPPER_CONVEYING,))) && __GET_VAR(data__->PALLETIZER,)) && __GET_VAR(data__->_TMP_EQ243_OUT,))) {
    __SET_VAR(data__->,PALLETIZER_MOVING,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE281_OUT,,__TAG_DISPLAY_MOVE__INT__INT10(
    (BOOL)(((!(__GET_VAR(data__->WRAPPER_WRAPPING,)) && !(__GET_VAR(data__->WRAPPER_CONVEYING,))) && __GET_VAR(data__->PALLETIZER,)) && __GET_VAR(data__->_TMP_EQ243_OUT,)),
    (INT)__GET_VAR(data__->STATE_5,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE281_ENO,)) {
    __SET_VAR(data__->,MACHINE_3,,__GET_VAR(data__->_TMP_MOVE281_OUT,));
  };
  __SET_VAR(data__->,_TMP_MOVE228_OUT,,__TAG_DISPLAY_MOVE__INT__INT11(
    (BOOL)(__GET_VAR(data__->PALLETIZER,) && __GET_VAR(data__->_TMP_EQ219_OUT,)),
    (INT)__GET_VAR(data__->POS_2,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE228_ENO,)) {
    __SET_VAR(data__->,PALLETIZER_ROTATION,,__GET_VAR(data__->_TMP_MOVE228_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ264_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_3,),
    (INT)__GET_VAR(data__->STATE_6,)));
  __SET_VAR(data__->,_TMP_MOVE241_OUT,,__TAG_DISPLAY_MOVE__INT__INT12(
    (BOOL)__GET_VAR(data__->_TMP_EQ264_OUT,),
    (INT)__GET_VAR(data__->POS_2,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE241_ENO,)) {
    __SET_VAR(data__->,PALLETIZER_ROTATION,,__GET_VAR(data__->_TMP_MOVE241_OUT,));
  };
  __SET_VAR(data__->,_TMP_ADD248_OUT,,__TAG_DISPLAY_ADD__INT__INT13(
    (BOOL)(__GET_VAR(data__->PALLETIZER,) && __GET_VAR(data__->_TMP_EQ264_OUT,)),
    (UINT)2,
    (INT)__GET_VAR(data__->BOX_COUNT,),
    (INT)1,
    data__));
  if (__GET_VAR(data__->_TMP_ADD248_ENO,)) {
    __SET_VAR(data__->,BOX_COUNT,,__GET_VAR(data__->_TMP_ADD248_OUT,));
  };
  __SET_VAR(data__->,_TMP_MOVE252_OUT,,__TAG_DISPLAY_MOVE__INT__INT14(
    (BOOL)(__GET_VAR(data__->PALLETIZER,) && __GET_VAR(data__->_TMP_EQ264_OUT,)),
    (INT)__GET_VAR(data__->STATE_3,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE252_ENO,)) {
    __SET_VAR(data__->,MACHINE_3,,__GET_VAR(data__->_TMP_MOVE252_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ256_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_4,),
    (INT)__GET_VAR(data__->STATE_0,)));
  __SET_VAR(data__->,_TMP_GE263_OUT,,GE__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->BOX_COUNT,),
    (INT)40));
  __SET_VAR(data__->,_TMP_AND353_OUT,,AND__BOOL__BOOL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (BOOL)__GET_VAR(data__->_TMP_EQ256_OUT,),
    (BOOL)__GET_VAR(data__->_TMP_GE263_OUT,)));
  if (((!(__GET_VAR(data__->PS_6,)) && __GET_VAR(data__->WRAPPER,)) && __GET_VAR(data__->_TMP_AND353_OUT,))) {
    __SET_VAR(data__->,WRAPPER_WRAPPING,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_EQ210_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_3,),
    (INT)__GET_VAR(data__->STATE_0,)));
  __SET_VAR(data__->,_TMP_MOVE224_OUT,,__TAG_DISPLAY_MOVE__INT__INT15(
    (BOOL)(((__GET_VAR(data__->PALLETIZER,) && !(__GET_VAR(data__->PALLETIZER_GRABBING,))) && __GET_VAR(data__->PS_5,)) && __GET_VAR(data__->_TMP_EQ210_OUT,)),
    (INT)__GET_VAR(data__->STATE_1,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE224_ENO,)) {
    __SET_VAR(data__->,MACHINE_3,,__GET_VAR(data__->_TMP_MOVE224_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ270_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_4,),
    (INT)__GET_VAR(data__->STATE_1,)));
  if (((!(__GET_VAR(data__->WRAPPER_WRAPPING,)) && __GET_VAR(data__->WRAPPER,)) && __GET_VAR(data__->_TMP_EQ270_OUT,))) {
    __SET_VAR(data__->,WRAPPER_CONVEYING,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE290_OUT,,__TAG_DISPLAY_MOVE__INT__INT16(
    (BOOL)((!(__GET_VAR(data__->PS_6,)) && __GET_VAR(data__->WRAPPER,)) && __GET_VAR(data__->_TMP_AND353_OUT,)),
    (INT)__GET_VAR(data__->STATE_1,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE290_ENO,)) {
    __SET_VAR(data__->,MACHINE_4,,__GET_VAR(data__->_TMP_MOVE290_OUT,));
  };
  __SET_VAR(data__->,_TMP_MOVE310_OUT,,__TAG_DISPLAY_MOVE__INT__INT17(
    (BOOL)((!(__GET_VAR(data__->WRAPPER_WRAPPING,)) && __GET_VAR(data__->WRAPPER,)) && __GET_VAR(data__->_TMP_EQ270_OUT,)),
    (INT)__GET_VAR(data__->STATE_2,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE310_ENO,)) {
    __SET_VAR(data__->,MACHINE_4,,__GET_VAR(data__->_TMP_MOVE310_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ314_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_4,),
    (INT)__GET_VAR(data__->STATE_2,)));
  if (((!(__GET_VAR(data__->WRAPPER_CONVEYING,)) && __GET_VAR(data__->CONVEYOR_5,)) && __GET_VAR(data__->_TMP_EQ314_OUT,))) {
    __SET_VAR(data__->,CONVEYING_5,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE321_OUT,,__TAG_DISPLAY_MOVE__INT__INT18(
    (BOOL)((!(__GET_VAR(data__->WRAPPER_CONVEYING,)) && __GET_VAR(data__->CONVEYOR_5,)) && __GET_VAR(data__->_TMP_EQ314_OUT,)),
    (INT)__GET_VAR(data__->STATE_3,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE321_ENO,)) {
    __SET_VAR(data__->,MACHINE_4,,__GET_VAR(data__->_TMP_MOVE321_OUT,));
  };
  if ((__GET_VAR(data__->PALLETIZER,) && __GET_VAR(data__->_TMP_EQ264_OUT,))) {
    __SET_VAR(data__->,PALLETIZER_GRABBING,,__BOOL_LITERAL(FALSE));
  };
  if ((__GET_VAR(data__->PALLETIZER,) && __GET_VAR(data__->_TMP_EQ264_OUT,))) {
    __SET_VAR(data__->,PALLETIZER_MOVING,,__BOOL_LITERAL(TRUE));
  };
  if ((__GET_VAR(data__->PS_1,) && __GET_VAR(data__->_TMP_EQ125_OUT,))) {
    __SET_VAR(data__->,CONVEYING_1,,__BOOL_LITERAL(FALSE));
  };
  __SET_VAR(data__->,_TMP_EQ145_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_1,),
    (INT)__GET_VAR(data__->STATE_4,)));
  __SET_VAR(data__->,_TMP_MOVE336_OUT,,__TAG_DISPLAY_MOVE__INT__INT19(
    (BOOL)(__GET_VAR(data__->PS_2,) && __GET_VAR(data__->_TMP_EQ145_OUT,)),
    (INT)__GET_VAR(data__->STATE_5,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE336_ENO,)) {
    __SET_VAR(data__->,MACHINE_1,,__GET_VAR(data__->_TMP_MOVE336_OUT,));
  };
  if ((__GET_VAR(data__->PS_2,) && __GET_VAR(data__->_TMP_EQ145_OUT,))) {
    __SET_VAR(data__->,CONVEYING_2,,__BOOL_LITERAL(FALSE));
  };
  if ((__GET_VAR(data__->PS_4,) && __GET_VAR(data__->_TMP_EQ171_OUT,))) {
    __SET_VAR(data__->,CONVEYING_3,,__BOOL_LITERAL(FALSE));
  };
  if ((__GET_VAR(data__->PS_5,) && __GET_VAR(data__->_TMP_EQ210_OUT,))) {
    __SET_VAR(data__->,CONVEYING_4,,__BOOL_LITERAL(FALSE));
  };
  __SET_VAR(data__->,_TMP_EQ347_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_4,),
    (INT)__GET_VAR(data__->STATE_3,)));
  __SET_VAR(data__->,_TMP_MOVE345_OUT,,__TAG_DISPLAY_MOVE__INT__INT20(
    (BOOL)(__GET_VAR(data__->PS_6,) && __GET_VAR(data__->_TMP_EQ347_OUT,)),
    (INT)__GET_VAR(data__->STATE_0,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE345_ENO,)) {
    __SET_VAR(data__->,MACHINE_4,,__GET_VAR(data__->_TMP_MOVE345_OUT,));
  };
  if ((__GET_VAR(data__->PS_6,) && __GET_VAR(data__->_TMP_EQ347_OUT,))) {
    __SET_VAR(data__->,CONVEYING_5,,__BOOL_LITERAL(FALSE));
  };
  if ((((__GET_VAR(data__->PALLETIZER,) && !(__GET_VAR(data__->PALLETIZER_GRABBING,))) && __GET_VAR(data__->PS_5,)) && __GET_VAR(data__->_TMP_EQ210_OUT,))) {
    __SET_VAR(data__->,PALLETIZER_MOVING,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE238_OUT,,__TAG_DISPLAY_MOVE__INT__INT21(
    (BOOL)(((__GET_VAR(data__->PALLETIZER,) && !(__GET_VAR(data__->PALLETIZER_GRABBING,))) && __GET_VAR(data__->PS_5,)) && __GET_VAR(data__->_TMP_EQ210_OUT,)),
    (INT)__GET_VAR(data__->POS_1,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE238_ENO,)) {
    __SET_VAR(data__->,PALLETIZER_ROTATION,,__GET_VAR(data__->_TMP_MOVE238_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ204_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_3,),
    (INT)__GET_VAR(data__->STATE_1,)));
  if ((__GET_VAR(data__->ROE_1,) && __GET_VAR(data__->_TMP_EQ204_OUT,))) {
    __SET_VAR(data__->,PALLETIZER_MOVING,,__BOOL_LITERAL(FALSE));
  };
  __SET_VAR(data__->,_TMP_EQ232_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_3,),
    (INT)__GET_VAR(data__->STATE_3,)));
  __SET_VAR(data__->,_TMP_MOVE229_OUT,,__TAG_DISPLAY_MOVE__INT__INT22(
    (BOOL)((__GET_VAR(data__->PALLETIZER_GRABBING,) && __GET_VAR(data__->ROE_2,)) && __GET_VAR(data__->_TMP_EQ232_OUT,)),
    (INT)__GET_VAR(data__->STATE_4,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE229_ENO,)) {
    __SET_VAR(data__->,MACHINE_3,,__GET_VAR(data__->_TMP_MOVE229_OUT,));
  };
  if ((__GET_VAR(data__->ROE_2,) && __GET_VAR(data__->_TMP_EQ232_OUT,))) {
    __SET_VAR(data__->,PALLETIZER_MOVING,,__BOOL_LITERAL(FALSE));
  };
  __SET_VAR(data__->,_TMP_MOVE262_OUT,,__TAG_DISPLAY_MOVE__INT__INT23(
    (BOOL)(((!(__GET_VAR(data__->WRAPPER_WRAPPING,)) && !(__GET_VAR(data__->WRAPPER_CONVEYING,))) && __GET_VAR(data__->PALLETIZER,)) && __GET_VAR(data__->_TMP_EQ243_OUT,)),
    (INT)__GET_VAR(data__->POS_3,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE262_ENO,)) {
    __SET_VAR(data__->,PALLETIZER_ROTATION,,__GET_VAR(data__->_TMP_MOVE262_OUT,));
  };
  __SET_VAR(data__->,_TMP_EQ341_OUT,,EQ__BOOL__INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (INT)__GET_VAR(data__->MACHINE_3,),
    (INT)__GET_VAR(data__->STATE_5,)));
  __SET_VAR(data__->,_TMP_MOVE303_OUT,,__TAG_DISPLAY_MOVE__INT__INT24(
    (BOOL)(__GET_VAR(data__->ROE_3,) && __GET_VAR(data__->_TMP_EQ341_OUT,)),
    (INT)__GET_VAR(data__->STATE_6,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE303_ENO,)) {
    __SET_VAR(data__->,MACHINE_3,,__GET_VAR(data__->_TMP_MOVE303_OUT,));
  };
  if ((__GET_VAR(data__->ROE_3,) && __GET_VAR(data__->_TMP_EQ341_OUT,))) {
    __SET_VAR(data__->,PALLETIZER_MOVING,,__BOOL_LITERAL(FALSE));
  };
  __SET_VAR(data__->,_TMP_MOVE362_OUT,,__TAG_DISPLAY_MOVE__INT__INT25(
    (BOOL)((!(__GET_VAR(data__->PALLETIZER_GRABBING,)) && __GET_VAR(data__->ROE_2,)) && __GET_VAR(data__->_TMP_EQ232_OUT,)),
    (INT)__GET_VAR(data__->STATE_0,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE362_ENO,)) {
    __SET_VAR(data__->,MACHINE_3,,__GET_VAR(data__->_TMP_MOVE362_OUT,));
  };
  __SET_VAR(data__->,_TMP_MOVE186_OUT,,__TAG_DISPLAY_MOVE__INT__INT26(
    (BOOL)(__GET_VAR(data__->ROE_1,) && __GET_VAR(data__->_TMP_EQ204_OUT,)),
    (INT)__GET_VAR(data__->STATE_2,),
    data__));
  if (__GET_VAR(data__->_TMP_MOVE186_ENO,)) {
    __SET_VAR(data__->,MACHINE_3,,__GET_VAR(data__->_TMP_MOVE186_OUT,));
  };
  if ((((__GET_VAR(data__->PALLETIZER,) && !(__GET_VAR(data__->PALLETIZER_GRABBING,))) && __GET_VAR(data__->PS_5,)) && __GET_VAR(data__->_TMP_EQ210_OUT,))) {
    __SET_VAR(data__->,PALLETIZER_GRABBING,,__BOOL_LITERAL(TRUE));
  };
  __SET_VAR(data__->,_TMP_MOVE273_OUT,,__TAG_DISPLAY_MOVE__INT__INT27(
    (BOOL)((!(__GET_VAR(data__->PS_6,)) && __GET_VAR(data__->WRAPPER,)) && __GET_VAR(data__->_TMP_AND353_OUT,)),
    (INT)0,
    data__));
  if (__GET_VAR(data__->_TMP_MOVE273_ENO,)) {
    __SET_VAR(data__->,BOX_COUNT,,__GET_VAR(data__->_TMP_MOVE273_OUT,));
  };

  goto __end;

__end:
  return;
} // TAG_DISPLAY_body__() 





void WIRECUT_PPM_FUNCTION_init__(WIRECUT_PPM_FUNCTION *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->WIRECUT_CPM,0,retain)
  __INIT_VAR(data__->COOKIE_WEIGHT,0,retain)
  __INIT_VAR(data__->COOKIE_COUNT,0,retain)
  __INIT_VAR(data__->WIRECUT_PPM,0,retain)
}

// Code part
void WIRECUT_PPM_FUNCTION_body__(WIRECUT_PPM_FUNCTION *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  __SET_VAR(data__->,COOKIE_COUNT,,8.0);
  __SET_VAR(data__->,COOKIE_WEIGHT,,0.3);
  __SET_VAR(data__->,WIRECUT_PPM,,(((WORD_TO_REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (WORD)__GET_VAR(data__->WIRECUT_CPM,)) * __GET_VAR(data__->COOKIE_COUNT,)) * __GET_VAR(data__->COOKIE_WEIGHT,)) / 16.0));

  goto __end;

__end:
  return;
} // WIRECUT_PPM_FUNCTION_body__() 





