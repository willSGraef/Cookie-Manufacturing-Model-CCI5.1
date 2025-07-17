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





static inline INT __TAG_DISPLAY_ADD__INT__INT1(BOOL EN,
  UINT __PARAM_COUNT,
  INT IN1,
  INT IN2,
  TAG_DISPLAY *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_ADD161_ENO,);
  __res = ADD__INT__INT(EN,
    &__TMP_ENO,
    __PARAM_COUNT,
    IN1,
    IN2);
  __SET_VAR(,data__->_TMP_ADD161_ENO,,__TMP_ENO);
  return __res;
}

void TAG_DISPLAY_init__(TAG_DISPLAY *data__, BOOL retain) {
  __INIT_LOCATED(WORD,__QW100,data__->LCS_1_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCS_1_LOW,0)
  __INIT_LOCATED(WORD,__QW101,data__->LCS_1_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCS_1_HIGH,0)
  __INIT_LOCATED(WORD,__QW102,data__->LCS_2_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCS_2_LOW,0)
  __INIT_LOCATED(WORD,__QW103,data__->LCS_2_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCS_2_HIGH,0)
  __INIT_LOCATED(WORD,__QW104,data__->LCS_3_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCS_3_LOW,0)
  __INIT_LOCATED(WORD,__QW105,data__->LCS_3_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCS_3_HIGH,0)
  __INIT_LOCATED(WORD,__QW106,data__->LCS_4_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCS_4_LOW,0)
  __INIT_LOCATED(WORD,__QW107,data__->LCS_4_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCS_4_HIGH,0)
  __INIT_LOCATED(WORD,__QW108,data__->LCF_1_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCF_1_LOW,0)
  __INIT_LOCATED(WORD,__QW109,data__->LCF_1_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCF_1_HIGH,0)
  __INIT_LOCATED(WORD,__QW110,data__->LCF_2_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCF_2_LOW,0)
  __INIT_LOCATED(WORD,__QW111,data__->LCF_2_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCF_2_HIGH,0)
  __INIT_LOCATED(WORD,__QW112,data__->LCF_3_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCF_3_LOW,0)
  __INIT_LOCATED(WORD,__QW113,data__->LCF_3_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCF_3_HIGH,0)
  __INIT_LOCATED(WORD,__QW114,data__->LCF_4_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCF_4_LOW,0)
  __INIT_LOCATED(WORD,__QW115,data__->LCF_4_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCF_4_HIGH,0)
  __INIT_LOCATED(WORD,__QW99,data__->LCH_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCH_HIGH,0)
  __INIT_LOCATED(WORD,__QW98,data__->LCH_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCH_LOW,0)
  __INIT_LOCATED(WORD,__QW97,data__->LCM_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->LCM_HIGH,0)
  __INIT_LOCATED(WORD,__QW96,data__->LCM_LOW,retain)
  __INIT_LOCATED_VALUE(data__->LCM_LOW,0)
  __INIT_LOCATED(WORD,__QW95,data__->TROUGH_WEIGHT_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->TROUGH_WEIGHT_HIGH,0)
  __INIT_LOCATED(WORD,__QW94,data__->TROUGH_WEIGHT_LOW,retain)
  __INIT_LOCATED_VALUE(data__->TROUGH_WEIGHT_LOW,0)
  __INIT_LOCATED(WORD,__QW1001,data__->VACUUM_RPM,retain)
  __INIT_LOCATED_VALUE(data__->VACUUM_RPM,0)
  __INIT_LOCATED(WORD,__QW1003,data__->MIXER_RPM,retain)
  __INIT_LOCATED_VALUE(data__->MIXER_RPM,0)
  __INIT_LOCATED(WORD,__QW1006,data__->WIRECUT_CPM,retain)
  __INIT_LOCATED_VALUE(data__->WIRECUT_CPM,0)
  __INIT_LOCATED(WORD,__QW1009,data__->PAPERCUT_CPM,retain)
  __INIT_LOCATED_VALUE(data__->PAPERCUT_CPM,0)
  __INIT_LOCATED(REAL,__MD13,data__->CONVEYOR_FPM,retain)
  __INIT_LOCATED_VALUE(data__->CONVEYOR_FPM,0)
  __INIT_LOCATED(WORD,__QW121,data__->CONVEYOR_FPM_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->CONVEYOR_FPM_HIGH,0)
  __INIT_LOCATED(WORD,__QW120,data__->CONVEYOR_FPM_LOW,retain)
  __INIT_LOCATED_VALUE(data__->CONVEYOR_FPM_LOW,0)
  __INIT_LOCATED(REAL,__MD0,data__->LCS_1,retain)
  __INIT_LOCATED_VALUE(data__->LCS_1,0)
  __INIT_LOCATED(REAL,__MD1,data__->LCS_2,retain)
  __INIT_LOCATED_VALUE(data__->LCS_2,0)
  __INIT_LOCATED(REAL,__MD2,data__->LCS_3,retain)
  __INIT_LOCATED_VALUE(data__->LCS_3,0)
  __INIT_LOCATED(REAL,__MD3,data__->LCS_4,retain)
  __INIT_LOCATED_VALUE(data__->LCS_4,0)
  __INIT_LOCATED(REAL,__MD4,data__->LCF_1,retain)
  __INIT_LOCATED_VALUE(data__->LCF_1,0)
  __INIT_LOCATED(REAL,__MD5,data__->LCF_2,retain)
  __INIT_LOCATED_VALUE(data__->LCF_2,0)
  __INIT_LOCATED(REAL,__MD6,data__->LCF_3,retain)
  __INIT_LOCATED_VALUE(data__->LCF_3,0)
  __INIT_LOCATED(REAL,__MD7,data__->LCF_4,retain)
  __INIT_LOCATED_VALUE(data__->LCF_4,0)
  __INIT_LOCATED(REAL,__MD8,data__->LCH,retain)
  __INIT_LOCATED_VALUE(data__->LCH,0.0)
  __INIT_LOCATED(REAL,__MD11,data__->LCM,retain)
  __INIT_LOCATED_VALUE(data__->LCM,0.0)
  __INIT_LOCATED(REAL,__MD12,data__->TROUGH_WEIGHT,retain)
  __INIT_LOCATED_VALUE(data__->TROUGH_WEIGHT,0.0)
  __INIT_LOCATED(REAL,__MD9,data__->FLOUR_WEIGHT,retain)
  __INIT_LOCATED_VALUE(data__->FLOUR_WEIGHT,0.0)
  __INIT_LOCATED(WORD,__QW117,data__->FLOUR_WEIGHT_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->FLOUR_WEIGHT_HIGH,0)
  __INIT_LOCATED(WORD,__QW116,data__->FLOUR_WEIGHT_LOW,retain)
  __INIT_LOCATED_VALUE(data__->FLOUR_WEIGHT_LOW,0)
  __INIT_LOCATED(REAL,__MD10,data__->SUGAR_WEIGHT,retain)
  __INIT_LOCATED_VALUE(data__->SUGAR_WEIGHT,0.0)
  __INIT_LOCATED(WORD,__QW219,data__->SUGAR_WEIGHT_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->SUGAR_WEIGHT_HIGH,0)
  __INIT_LOCATED(WORD,__QW218,data__->SUGAR_WEIGHT_LOW,retain)
  __INIT_LOCATED_VALUE(data__->SUGAR_WEIGHT_LOW,0)
  __INIT_LOCATED(REAL,__MD15,data__->TUNNEL_TEMP,retain)
  __INIT_LOCATED_VALUE(data__->TUNNEL_TEMP,0)
  __INIT_LOCATED(WORD,__QW303,data__->TUNNEL_TEMP_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->TUNNEL_TEMP_HIGH,0)
  __INIT_LOCATED(WORD,__QW302,data__->TUNNEL_TEMP_LOW,retain)
  __INIT_LOCATED_VALUE(data__->TUNNEL_TEMP_LOW,0)
  __INIT_LOCATED(REAL,__MD14,data__->NITROGEN_VOLUME,retain)
  __INIT_LOCATED_VALUE(data__->NITROGEN_VOLUME,0)
  __INIT_LOCATED(WORD,__QW301,data__->NITROGEN_VOLUME_HIGH,retain)
  __INIT_LOCATED_VALUE(data__->NITROGEN_VOLUME_HIGH,0)
  __INIT_LOCATED(WORD,__QW300,data__->NITROGEN_VOLUME_LOW,retain)
  __INIT_LOCATED_VALUE(data__->NITROGEN_VOLUME_LOW,0)
  __INIT_LOCATED(BOOL,__QX87_4,data__->RV_1,retain)
  __INIT_LOCATED_VALUE(data__->RV_1,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX87_5,data__->RV_2,retain)
  __INIT_LOCATED_VALUE(data__->RV_2,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX87_6,data__->RV_3,retain)
  __INIT_LOCATED_VALUE(data__->RV_3,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX87_7,data__->DV,retain)
  __INIT_LOCATED_VALUE(data__->DV,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX88_0,data__->VACUUM,retain)
  __INIT_LOCATED_VALUE(data__->VACUUM,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX88_1,data__->MIXER,retain)
  __INIT_LOCATED_VALUE(data__->MIXER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX88_2,data__->FLOUR_ALARM,retain)
  __INIT_LOCATED_VALUE(data__->FLOUR_ALARM,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX88_3,data__->SUGAR_ALARM,retain)
  __INIT_LOCATED_VALUE(data__->SUGAR_ALARM,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX88_4,data__->HOPPER_ALARM,retain)
  __INIT_LOCATED_VALUE(data__->HOPPER_ALARM,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX88_5,data__->VACUUM_ALARM,retain)
  __INIT_LOCATED_VALUE(data__->VACUUM_ALARM,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX88_6,data__->VACUUM_MANUAL,retain)
  __INIT_LOCATED_VALUE(data__->VACUUM_MANUAL,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX88_7,data__->WIRECUTTER,retain)
  __INIT_LOCATED_VALUE(data__->WIRECUTTER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX89_1,data__->TROUGH_TRANSFER,retain)
  __INIT_LOCATED_VALUE(data__->TROUGH_TRANSFER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX89_4,data__->CONVEYOR,retain)
  __INIT_LOCATED_VALUE(data__->CONVEYOR,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX89_5,data__->PAPER_CUTTER,retain)
  __INIT_LOCATED_VALUE(data__->PAPER_CUTTER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX89_6,data__->GV_1,retain)
  __INIT_LOCATED_VALUE(data__->GV_1,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX89_7,data__->FAN_1,retain)
  __INIT_LOCATED_VALUE(data__->FAN_1,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX90_0,data__->FAN_2,retain)
  __INIT_LOCATED_VALUE(data__->FAN_2,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX90_1,data__->FAN_3,retain)
  __INIT_LOCATED_VALUE(data__->FAN_3,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX90_2,data__->EXHAUST_FAN,retain)
  __INIT_LOCATED_VALUE(data__->EXHAUST_FAN,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(REAL,__MD20,data__->CFM,retain)
  __INIT_LOCATED_VALUE(data__->CFM,0)
  __INIT_LOCATED(BOOL,__QX125_0,data__->RESET,retain)
  __INIT_LOCATED_VALUE(data__->RESET,__BOOL_LITERAL(FALSE))
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
  __INIT_LOCATED(BOOL,__QX90_3,data__->BOX_MAKER,retain)
  __INIT_LOCATED_VALUE(data__->BOX_MAKER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX90_4,data__->TAPER,retain)
  __INIT_LOCATED_VALUE(data__->TAPER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX90_5,data__->LABELER,retain)
  __INIT_LOCATED_VALUE(data__->LABELER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX90_6,data__->PALLETIZER,retain)
  __INIT_LOCATED_VALUE(data__->PALLETIZER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX90_7,data__->PALLETIZER_GRABBING,retain)
  __INIT_LOCATED_VALUE(data__->PALLETIZER_GRABBING,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX91_0,data__->WRAPPER,retain)
  __INIT_LOCATED_VALUE(data__->WRAPPER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX91_1,data__->WRAPPER_CONVEYOR,retain)
  __INIT_LOCATED_VALUE(data__->WRAPPER_CONVEYOR,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX91_2,data__->CONVEYOR_1,retain)
  __INIT_LOCATED_VALUE(data__->CONVEYOR_1,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX91_3,data__->CONVEYOR_2,retain)
  __INIT_LOCATED_VALUE(data__->CONVEYOR_2,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX91_4,data__->CONVEYOR_3,retain)
  __INIT_LOCATED_VALUE(data__->CONVEYOR_3,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX91_5,data__->CONVEYOR_4,retain)
  __INIT_LOCATED_VALUE(data__->CONVEYOR_4,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX91_6,data__->CONVEYOR_5,retain)
  __INIT_LOCATED_VALUE(data__->CONVEYOR_5,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX91_7,data__->PS_1,retain)
  __INIT_LOCATED_VALUE(data__->PS_1,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX92_0,data__->PS_2,retain)
  __INIT_LOCATED_VALUE(data__->PS_2,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX92_1,data__->PS_3,retain)
  __INIT_LOCATED_VALUE(data__->PS_3,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX92_2,data__->PS_4,retain)
  __INIT_LOCATED_VALUE(data__->PS_4,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX92_3,data__->PS_5,retain)
  __INIT_LOCATED_VALUE(data__->PS_5,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX92_4,data__->PS_6,retain)
  __INIT_LOCATED_VALUE(data__->PS_6,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX92_5,data__->BAGGER,retain)
  __INIT_LOCATED_VALUE(data__->BAGGER,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(WORD,__QW1012,data__->PALLETIZER_POSITION,retain)
  __INIT_LOCATED_VALUE(data__->PALLETIZER_POSITION,0)
  __INIT_LOCATED(WORD,__QW1013,data__->BOX_COUNT,retain)
  __INIT_LOCATED_VALUE(data__->BOX_COUNT,0)
  __INIT_LOCATED(BOOL,__QX92_6,data__->WRAPPER_WRAPPING,retain)
  __INIT_LOCATED_VALUE(data__->WRAPPER_WRAPPING,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX92_7,data__->BOX_MAKER_BOXING,retain)
  __INIT_LOCATED_VALUE(data__->BOX_MAKER_BOXING,__BOOL_LITERAL(FALSE))
  __INIT_VAR(data__->_TMP_ADD37_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD38_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD41_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD39_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD40_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD43_OUT,0,retain)
  __INIT_VAR(data__->MIXER_RPM_FUNCTION0_COUNTER,0,retain)
  __INIT_VAR(data__->VACUUM_RPM_FUNCTION0_COUNTER,0,retain)
  __INIT_VAR(data__->_TMP_EQ140_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_AND143_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_WORD_TO_INT162_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD161_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_ADD161_OUT,0,retain)
  __INIT_VAR(data__->_TMP_INT_TO_WORD163_OUT,0,retain)
  __INIT_VAR(data__->_TMP_EQ103_OUT,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_AND152_OUT,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void TAG_DISPLAY_body__(TAG_DISPLAY *data__) {
  // Initialise TEMP variables

  __SET_VAR(data__->WORDS_TO_REAL0.,HIGH,,__GET_LOCATED(data__->LCF_1_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL0.,LOW,,__GET_LOCATED(data__->LCF_1_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL0);
  __SET_LOCATED(data__->,LCF_1,,__GET_VAR(data__->WORDS_TO_REAL0.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL1.,HIGH,,__GET_LOCATED(data__->LCF_2_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL1.,LOW,,__GET_LOCATED(data__->LCF_2_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL1);
  __SET_LOCATED(data__->,LCF_2,,__GET_VAR(data__->WORDS_TO_REAL1.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL2.,HIGH,,__GET_LOCATED(data__->LCF_3_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL2.,LOW,,__GET_LOCATED(data__->LCF_3_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL2);
  __SET_LOCATED(data__->,LCF_3,,__GET_VAR(data__->WORDS_TO_REAL2.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL3.,HIGH,,__GET_LOCATED(data__->LCF_4_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL3.,LOW,,__GET_LOCATED(data__->LCF_4_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL3);
  __SET_LOCATED(data__->,LCF_4,,__GET_VAR(data__->WORDS_TO_REAL3.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL4.,HIGH,,__GET_LOCATED(data__->LCS_1_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL4.,LOW,,__GET_LOCATED(data__->LCS_1_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL4);
  __SET_LOCATED(data__->,LCS_1,,__GET_VAR(data__->WORDS_TO_REAL4.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL5.,HIGH,,__GET_LOCATED(data__->LCS_2_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL5.,LOW,,__GET_LOCATED(data__->LCS_2_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL5);
  __SET_LOCATED(data__->,LCS_2,,__GET_VAR(data__->WORDS_TO_REAL5.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL6.,HIGH,,__GET_LOCATED(data__->LCS_3_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL6.,LOW,,__GET_LOCATED(data__->LCS_3_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL6);
  __SET_LOCATED(data__->,LCS_3,,__GET_VAR(data__->WORDS_TO_REAL6.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL7.,HIGH,,__GET_LOCATED(data__->LCS_4_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL7.,LOW,,__GET_LOCATED(data__->LCS_4_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL7);
  __SET_LOCATED(data__->,LCS_4,,__GET_VAR(data__->WORDS_TO_REAL7.RESULT,));
  __SET_VAR(data__->,_TMP_ADD37_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_LOCATED(data__->LCF_1,),
    (REAL)__GET_LOCATED(data__->LCF_2,)));
  __SET_VAR(data__->,_TMP_ADD38_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_LOCATED(data__->LCF_3,),
    (REAL)__GET_LOCATED(data__->LCF_4,)));
  __SET_VAR(data__->,_TMP_ADD41_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_VAR(data__->_TMP_ADD37_OUT,),
    (REAL)__GET_VAR(data__->_TMP_ADD38_OUT,)));
  __SET_LOCATED(data__->,FLOUR_WEIGHT,,__GET_VAR(data__->_TMP_ADD41_OUT,));
  __SET_VAR(data__->FLOUR_ALARM_CHECK0.,FLOUR_WEIGHT,,__GET_LOCATED(data__->FLOUR_WEIGHT,));
  FLOUR_ALARM_CHECK_body__(&data__->FLOUR_ALARM_CHECK0);
  __SET_LOCATED(data__->,FLOUR_ALARM,,__GET_VAR(data__->FLOUR_ALARM_CHECK0.ALARM,));
  __SET_VAR(data__->,_TMP_ADD39_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_LOCATED(data__->LCS_1,),
    (REAL)__GET_LOCATED(data__->LCS_2,)));
  __SET_VAR(data__->,_TMP_ADD40_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_LOCATED(data__->LCS_3,),
    (REAL)__GET_LOCATED(data__->LCS_4,)));
  __SET_VAR(data__->,_TMP_ADD43_OUT,,ADD__REAL__REAL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (REAL)__GET_VAR(data__->_TMP_ADD39_OUT,),
    (REAL)__GET_VAR(data__->_TMP_ADD40_OUT,)));
  __SET_LOCATED(data__->,SUGAR_WEIGHT,,__GET_VAR(data__->_TMP_ADD43_OUT,));
  __SET_VAR(data__->WORDS_TO_REAL8.,HIGH,,__GET_LOCATED(data__->LCH_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL8.,LOW,,__GET_LOCATED(data__->LCH_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL8);
  __SET_LOCATED(data__->,LCH,,__GET_VAR(data__->WORDS_TO_REAL8.RESULT,));
  __SET_VAR(data__->HOPPER_ALARM_CHECK0.,HOPPER_WEIGHT,,__GET_LOCATED(data__->LCH,));
  HOPPER_ALARM_CHECK_body__(&data__->HOPPER_ALARM_CHECK0);
  __SET_LOCATED(data__->,HOPPER_ALARM,,__GET_VAR(data__->HOPPER_ALARM_CHECK0.ALARM,));
  __SET_VAR(data__->VACUUM_ALARM_RPM0.,DV,,__GET_LOCATED(data__->DV,));
  __SET_VAR(data__->VACUUM_ALARM_RPM0.,VACUUM_RPM,,__GET_LOCATED(data__->VACUUM_RPM,));
  VACUUM_ALARM_RPM_body__(&data__->VACUUM_ALARM_RPM0);
  __SET_LOCATED(data__->,VACUUM_ALARM,,__GET_VAR(data__->VACUUM_ALARM_RPM0.ALARM,));
  __SET_LOCATED(data__->,CFM,,__GET_VAR(data__->VACUUM_ALARM_RPM0.CFM,));
  __SET_VAR(data__->REAL_TO_WORDS0.,INPUT,,__GET_LOCATED(data__->FLOUR_WEIGHT,));
  REAL_TO_WORDS_body__(&data__->REAL_TO_WORDS0);
  __SET_LOCATED(data__->,FLOUR_WEIGHT_HIGH,,__GET_VAR(data__->REAL_TO_WORDS0.HIGH,));
  __SET_LOCATED(data__->,FLOUR_WEIGHT_LOW,,__GET_VAR(data__->REAL_TO_WORDS0.LOW,));
  __SET_VAR(data__->REAL_TO_WORDS1.,INPUT,,__GET_LOCATED(data__->SUGAR_WEIGHT,));
  REAL_TO_WORDS_body__(&data__->REAL_TO_WORDS1);
  __SET_LOCATED(data__->,SUGAR_WEIGHT_HIGH,,__GET_VAR(data__->REAL_TO_WORDS1.HIGH,));
  __SET_LOCATED(data__->,SUGAR_WEIGHT_LOW,,__GET_VAR(data__->REAL_TO_WORDS1.LOW,));
  __SET_VAR(data__->SUGAR_ALARM_CHECK0.,SUGAR_WEIGHT,,__GET_LOCATED(data__->SUGAR_WEIGHT,));
  SUGAR_ALARM_CHECK_body__(&data__->SUGAR_ALARM_CHECK0);
  __SET_LOCATED(data__->,SUGAR_ALARM,,__GET_VAR(data__->SUGAR_ALARM_CHECK0.ALARM,));
  __SET_VAR(data__->,MIXER_RPM_FUNCTION0_COUNTER,,__GET_VAR(data__->MIXER_RPM_FUNCTION0.COUNTER,));
  __SET_VAR(data__->MIXER_RPM_FUNCTION0.,VALVE_STATUS,,__GET_LOCATED(data__->RV_3,));
  __SET_VAR(data__->MIXER_RPM_FUNCTION0.,HOPPER_WEIGHT,,__GET_LOCATED(data__->LCH,));
  __SET_VAR(data__->MIXER_RPM_FUNCTION0.,MIXER_STATUS,,__GET_LOCATED(data__->MIXER,));
  __SET_VAR(data__->MIXER_RPM_FUNCTION0.,COUNTER,,__GET_VAR(data__->MIXER_RPM_FUNCTION0_COUNTER,));
  MIXER_RPM_FUNCTION_body__(&data__->MIXER_RPM_FUNCTION0);
  __SET_VAR(data__->,MIXER_RPM_FUNCTION0_COUNTER,,__GET_VAR(data__->MIXER_RPM_FUNCTION0.COUNTER));
  __SET_LOCATED(data__->,MIXER_RPM,,__GET_VAR(data__->MIXER_RPM_FUNCTION0.OUTPUT_RPM,));
  __SET_VAR(data__->,VACUUM_RPM_FUNCTION0_COUNTER,,__GET_VAR(data__->VACUUM_RPM_FUNCTION0.COUNTER,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,INPUT_RPM,,__GET_LOCATED(data__->VACUUM_RPM,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,VACUUM_STATUS,,__GET_LOCATED(data__->VACUUM,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,MAX_RPM,,__GET_VAR(data__->VACUUM_MAX_RPM,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,MANUAL,,__GET_LOCATED(data__->VACUUM_MANUAL,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,DV,,__GET_LOCATED(data__->DV,));
  __SET_VAR(data__->VACUUM_RPM_FUNCTION0.,COUNTER,,__GET_VAR(data__->VACUUM_RPM_FUNCTION0_COUNTER,));
  VACUUM_RPM_FUNCTION_body__(&data__->VACUUM_RPM_FUNCTION0);
  __SET_VAR(data__->,VACUUM_RPM_FUNCTION0_COUNTER,,__GET_VAR(data__->VACUUM_RPM_FUNCTION0.COUNTER));
  __SET_LOCATED(data__->,VACUUM_RPM,,__GET_VAR(data__->VACUUM_RPM_FUNCTION0.OUTPUT_RPM,));
  __SET_VAR(data__->WORDS_TO_REAL9.,HIGH,,__GET_LOCATED(data__->TROUGH_WEIGHT_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL9.,LOW,,__GET_LOCATED(data__->TROUGH_WEIGHT_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL9);
  __SET_LOCATED(data__->,TROUGH_WEIGHT,,__GET_VAR(data__->WORDS_TO_REAL9.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL10.,HIGH,,__GET_LOCATED(data__->LCM_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL10.,LOW,,__GET_LOCATED(data__->LCM_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL10);
  __SET_LOCATED(data__->,LCM,,__GET_VAR(data__->WORDS_TO_REAL10.RESULT,));
  __SET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.,PAPER_CUTTER,,__GET_LOCATED(data__->PAPER_CUTTER,));
  __SET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.,COOKIE_DIAMETER,,__GET_VAR(data__->COOKIE_DIAMETER,));
  __SET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.,COOKIE_MARGIN,,__GET_VAR(data__->COOKIE_MARGIN,));
  __SET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.,CONVEYOR_FPM,,__GET_LOCATED(data__->CONVEYOR_FPM,));
  PAPERCUTTER_CPM_FUNCTION_body__(&data__->PAPERCUTTER_CPM_FUNCTION0);
  __SET_LOCATED(data__->,PAPERCUT_CPM,,__GET_VAR(data__->PAPERCUTTER_CPM_FUNCTION0.PAPERCUT_CPM,));
  __SET_VAR(data__->CONVEYOR_FPM_FUNCTION0.,WIRECUT_CPM,,__GET_LOCATED(data__->WIRECUT_CPM,));
  __SET_VAR(data__->CONVEYOR_FPM_FUNCTION0.,COOKIE_MARGIN,,__GET_VAR(data__->COOKIE_MARGIN,));
  __SET_VAR(data__->CONVEYOR_FPM_FUNCTION0.,COOKIE_DIAMETER,,__GET_VAR(data__->COOKIE_DIAMETER,));
  CONVEYOR_FPM_FUNCTION_body__(&data__->CONVEYOR_FPM_FUNCTION0);
  __SET_LOCATED(data__->,CONVEYOR_FPM,,__GET_VAR(data__->CONVEYOR_FPM_FUNCTION0.CONVEYOR_FPM,));
  __SET_VAR(data__->REAL_TO_WORDS2.,INPUT,,__GET_LOCATED(data__->CONVEYOR_FPM,));
  REAL_TO_WORDS_body__(&data__->REAL_TO_WORDS2);
  __SET_LOCATED(data__->,CONVEYOR_FPM_HIGH,,__GET_VAR(data__->REAL_TO_WORDS2.HIGH,));
  __SET_LOCATED(data__->,CONVEYOR_FPM_LOW,,__GET_VAR(data__->REAL_TO_WORDS2.LOW,));
  __SET_VAR(data__->WIRECUT_CPM_FUNCTION0.,WIRECUT_CPM,,__GET_LOCATED(data__->WIRECUT_CPM,));
  __SET_VAR(data__->WIRECUT_CPM_FUNCTION0.,WIRECUTTER,,__GET_LOCATED(data__->WIRECUTTER,));
  WIRECUT_CPM_FUNCTION_body__(&data__->WIRECUT_CPM_FUNCTION0);
  __SET_LOCATED(data__->,WIRECUT_CPM,,__GET_VAR(data__->WIRECUT_CPM_FUNCTION0.WIRECUT_CPM));
  __SET_LOCATED(data__->,WIRECUT_CPM,,__GET_VAR(data__->WIRECUT_CPM_FUNCTION0.WIRECUT_CPM,));
  __SET_VAR(data__->WORDS_TO_REAL11.,HIGH,,__GET_LOCATED(data__->NITROGEN_VOLUME_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL11.,LOW,,__GET_LOCATED(data__->NITROGEN_VOLUME_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL11);
  __SET_LOCATED(data__->,NITROGEN_VOLUME,,__GET_VAR(data__->WORDS_TO_REAL11.RESULT,));
  __SET_VAR(data__->WORDS_TO_REAL12.,HIGH,,__GET_LOCATED(data__->TUNNEL_TEMP_HIGH,));
  __SET_VAR(data__->WORDS_TO_REAL12.,LOW,,__GET_LOCATED(data__->TUNNEL_TEMP_LOW,));
  WORDS_TO_REAL_body__(&data__->WORDS_TO_REAL12);
  __SET_LOCATED(data__->,TUNNEL_TEMP,,__GET_VAR(data__->WORDS_TO_REAL12.RESULT,));
  __SET_LOCATED(data__->,BOX_MAKER_BOXING,,((__GET_LOCATED(data__->BOX_MAKER,) && !(__GET_LOCATED(data__->PS_1,))) && !(__GET_LOCATED(data__->BOX_MAKER_BOXING,))));
  __SET_LOCATED(data__->,CONVEYOR_1,,((!(__GET_LOCATED(data__->PS_1,)) && !(__GET_LOCATED(data__->BOX_MAKER_BOXING,))) && !(__GET_LOCATED(data__->CONVEYOR_2,))));
  __SET_LOCATED(data__->,CONVEYOR_2,,((((__GET_LOCATED(data__->BOX_MAKER_BOXING,) && __GET_LOCATED(data__->BAGGER,)) && !(__GET_LOCATED(data__->PS_2,))) && !(__GET_LOCATED(data__->PS_1,))) && !(__GET_LOCATED(data__->CONVEYOR_1,))));
  __SET_LOCATED(data__->,CONVEYOR_3,,((__GET_LOCATED(data__->PS_4,) && !(__GET_LOCATED(data__->PS_5,))) && __GET_LOCATED(data__->PS_3,)));
  __SET_LOCATED(data__->,PS_3,,!(((__GET_LOCATED(data__->PS_4,) && !(__GET_LOCATED(data__->PS_5,))) && __GET_LOCATED(data__->PS_3,))));
  __SET_VAR(data__->,_TMP_EQ140_OUT,,EQ__BOOL__WORD(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (WORD)__GET_LOCATED(data__->PALLETIZER_POSITION,),
    (WORD)2));
  __SET_VAR(data__->,_TMP_AND143_OUT,,AND__BOOL__BOOL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (BOOL)__GET_LOCATED(data__->PALLETIZER_GRABBING,),
    (BOOL)__GET_VAR(data__->_TMP_EQ140_OUT,)));
  __SET_LOCATED(data__->,PALLETIZER_GRABBING,,!(__GET_VAR(data__->_TMP_AND143_OUT,)));
  __SET_VAR(data__->,_TMP_WORD_TO_INT162_OUT,,WORD_TO_INT(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (WORD)__GET_LOCATED(data__->BOX_COUNT,)));
  __SET_VAR(data__->,_TMP_ADD161_OUT,,__TAG_DISPLAY_ADD__INT__INT1(
    (BOOL)__GET_VAR(data__->_TMP_AND143_OUT,),
    (UINT)2,
    (INT)1,
    (INT)__GET_VAR(data__->_TMP_WORD_TO_INT162_OUT,),
    data__));
  __SET_VAR(data__->,_TMP_INT_TO_WORD163_OUT,,INT_TO_WORD(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (INT)__GET_VAR(data__->_TMP_ADD161_OUT,)));
  __SET_LOCATED(data__->,BOX_COUNT,,__GET_VAR(data__->_TMP_INT_TO_WORD163_OUT,));
  __SET_VAR(data__->,_TMP_EQ103_OUT,,EQ__BOOL__WORD(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (WORD)__GET_LOCATED(data__->BOX_COUNT,),
    (WORD)40));
  __SET_VAR(data__->,_TMP_AND152_OUT,,AND__BOOL__BOOL(
    (BOOL)__BOOL_LITERAL(TRUE),
    NULL,
    (UINT)2,
    (BOOL)__GET_LOCATED(data__->WRAPPER,),
    (BOOL)__GET_VAR(data__->_TMP_EQ103_OUT,)));
  __SET_LOCATED(data__->,WRAPPER_WRAPPING,,__GET_VAR(data__->_TMP_AND152_OUT,));
  __SET_LOCATED(data__->,CONVEYOR_5,,__GET_LOCATED(data__->PS_6,));

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





