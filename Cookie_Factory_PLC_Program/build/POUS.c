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

  if ((__GET_VAR(data__->FLOUR_WEIGHT,) <= 1000.0)) {
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

  if ((__GET_VAR(data__->SUGAR_WEIGHT,) <= 1000.0)) {
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

  if ((__GET_VAR(data__->HOPPER_WEIGHT,) <= 100.0)) {
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





void FREQ_FUNCTION_init__(FREQ_FUNCTION *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->INPUT_FREQ,0,retain)
  __INIT_VAR(data__->OUTPUT_FREQ,0,retain)
  __INIT_VAR(data__->MANUAL,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->DV,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void FREQ_FUNCTION_body__(FREQ_FUNCTION *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  if (((__GET_VAR(data__->DV,) == 1) && (__GET_VAR(data__->MANUAL,) == 0))) {
    __SET_VAR(data__->,OUTPUT_FREQ,,23);
  } else if (((__GET_VAR(data__->DV,) == 0) && (__GET_VAR(data__->MANUAL,) == 0))) {
    __SET_VAR(data__->,OUTPUT_FREQ,,33);
  } else {
    __SET_VAR(data__->,OUTPUT_FREQ,,__GET_VAR(data__->INPUT_FREQ,));
  };

  goto __end;

__end:
  return;
} // FREQ_FUNCTION_body__() 





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
  __INIT_LOCATED(WORD,__QW1000,data__->VACUUM_FREQ,retain)
  __INIT_LOCATED_VALUE(data__->VACUUM_FREQ,0)
  __INIT_LOCATED(WORD,__QW1001,data__->VACUUM_RPM,retain)
  __INIT_LOCATED_VALUE(data__->VACUUM_RPM,0)
  __INIT_LOCATED(WORD,__QW1002,data__->MIXER_FREQ,retain)
  __INIT_LOCATED_VALUE(data__->MIXER_FREQ,0)
  __INIT_LOCATED(WORD,__QW1003,data__->MIXER_RPM,retain)
  __INIT_LOCATED_VALUE(data__->MIXER_RPM,0)
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
  __INIT_LOCATED(REAL,__MD11,data__->CFM,retain)
  __INIT_LOCATED_VALUE(data__->CFM,0)
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
  FREQ_FUNCTION_init__(&data__->FREQ_FUNCTION0,retain);
  REAL_TO_WORDS_init__(&data__->REAL_TO_WORDS0,retain);
  REAL_TO_WORDS_init__(&data__->REAL_TO_WORDS1,retain);
  __INIT_VAR(data__->_TMP_ADD37_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD38_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD41_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD39_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD40_OUT,0,retain)
  __INIT_VAR(data__->_TMP_ADD43_OUT,0,retain)
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
  __SET_VAR(data__->SUGAR_ALARM_CHECK0.,SUGAR_WEIGHT,,__GET_LOCATED(data__->SUGAR_WEIGHT,));
  SUGAR_ALARM_CHECK_body__(&data__->SUGAR_ALARM_CHECK0);
  __SET_LOCATED(data__->,SUGAR_ALARM,,__GET_VAR(data__->SUGAR_ALARM_CHECK0.ALARM,));
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
  __SET_VAR(data__->FREQ_FUNCTION0.,INPUT_FREQ,,__GET_LOCATED(data__->VACUUM_FREQ,));
  __SET_VAR(data__->FREQ_FUNCTION0.,MANUAL,,__GET_LOCATED(data__->VACUUM_MANUAL,));
  __SET_VAR(data__->FREQ_FUNCTION0.,DV,,__GET_LOCATED(data__->DV,));
  FREQ_FUNCTION_body__(&data__->FREQ_FUNCTION0);
  __SET_LOCATED(data__->,VACUUM_FREQ,,__GET_VAR(data__->FREQ_FUNCTION0.OUTPUT_FREQ,));
  __SET_VAR(data__->REAL_TO_WORDS0.,INPUT,,__GET_LOCATED(data__->FLOUR_WEIGHT,));
  REAL_TO_WORDS_body__(&data__->REAL_TO_WORDS0);
  __SET_LOCATED(data__->,FLOUR_WEIGHT_HIGH,,__GET_VAR(data__->REAL_TO_WORDS0.HIGH,));
  __SET_LOCATED(data__->,FLOUR_WEIGHT_LOW,,__GET_VAR(data__->REAL_TO_WORDS0.LOW,));
  __SET_VAR(data__->REAL_TO_WORDS1.,INPUT,,__GET_LOCATED(data__->SUGAR_WEIGHT,));
  REAL_TO_WORDS_body__(&data__->REAL_TO_WORDS1);
  __SET_LOCATED(data__->,SUGAR_WEIGHT_HIGH,,__GET_VAR(data__->REAL_TO_WORDS1.HIGH,));
  __SET_LOCATED(data__->,SUGAR_WEIGHT_LOW,,__GET_VAR(data__->REAL_TO_WORDS1.LOW,));

  goto __end;

__end:
  return;
} // TAG_DISPLAY_body__() 





