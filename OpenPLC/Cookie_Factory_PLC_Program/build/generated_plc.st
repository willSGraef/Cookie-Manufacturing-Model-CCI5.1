FUNCTION_BLOCK real_to_words
  VAR_INPUT
    Input : REAL;
  END_VAR
  VAR_OUTPUT
    High : WORD;
    Low : WORD;
  END_VAR

  {
      IEC_REAL var_in;
      IEC_WORD var_out[2];
      
      var_in = GetFbVar(INPUT);
      memcpy(var_out, &var_in, sizeof(var_in));
      SetFbVar(HIGH, var_out[0]);
      SetFbVar(LOW, var_out[1]);
  }
      
      
END_FUNCTION_BLOCK

FUNCTION_BLOCK words_to_real
  VAR_INPUT
    High : WORD;
    Low : WORD;
  END_VAR
  VAR_OUTPUT
    Result : REAL;
  END_VAR

  {

      IEC_WORD var_in[2];

      IEC_REAL var_out;

      var_in[0] = GetFbVar(HIGH);

      var_in[1] = GetFbVar(LOW);

      memcpy(&var_out, var_in, sizeof(var_out));

      SetFbVar(RESULT, var_out);



  }
END_FUNCTION_BLOCK

FUNCTION_BLOCK flour_alarm_check
  VAR_INPUT
    Flour_weight : REAL;
  END_VAR
  VAR_OUTPUT
    Alarm : BOOL := FALSE;
  END_VAR

  IF Flour_weight <= 1000.0 THEN
    Alarm := TRUE;
  ELSE
    Alarm := FALSE;
  END_IF;
END_FUNCTION_BLOCK

FUNCTION_BLOCK sugar_alarm_check
  VAR_INPUT
    Sugar_weight : REAL;
  END_VAR
  VAR_OUTPUT
    Alarm : BOOL := FALSE;
  END_VAR

  IF Sugar_weight <= 1000.0 THEN
    Alarm := TRUE;
  ELSE
    Alarm := FALSE;
  END_IF;
END_FUNCTION_BLOCK

FUNCTION_BLOCK hopper_alarm_check
  VAR_INPUT
    Hopper_weight : REAL;
  END_VAR
  VAR_OUTPUT
    Alarm : BOOL := FALSE;
  END_VAR

  IF Hopper_weight <= 100.0 THEN
    Alarm := TRUE;
  ELSE
    Alarm := FALSE;
  END_IF;
END_FUNCTION_BLOCK

FUNCTION_BLOCK vacuum_alarm_rpm
  VAR_INPUT
    Dv : BOOL;
  END_VAR
  VAR_OUTPUT
    Cfm : REAL;
  END_VAR
  VAR_INPUT
    Vacuum_rpm : WORD;
  END_VAR
  VAR_OUTPUT
    Alarm : BOOL := FALSE;
  END_VAR



  Cfm := ((-1.275 * 0.0000001) * (WORD_TO_REAL(Vacuum_rpm) * WORD_TO_REAL(Vacuum_rpm)) + (0.7896 * WORD_TO_REAL(Vacuum_rpm)) - 365.43);

  IF Dv = 1 AND Cfm < 687.0 THEN
    Alarm := TRUE;
  ELSIF Dv = 0 AND Cfm < 1178.0 THEN
    Alarm := TRUE;
  ELSE
    Alarm := FALSE;
  END_IF;
    
END_FUNCTION_BLOCK

FUNCTION_BLOCK freq_function
  VAR_INPUT
    input_freq : WORD;
  END_VAR
  VAR_OUTPUT
    output_freq : WORD;
  END_VAR
  VAR_INPUT
    manual : BOOL;
    dv : BOOL;
  END_VAR

  IF dv = 1 AND manual = 0 THEN
    output_freq := 23;
  ELSIF dv = 0 AND manual = 0 THEN
    output_freq := 33;
  ELSE
    output_freq := input_freq;
  END_IF;
END_FUNCTION_BLOCK

PROGRAM tag_display
  VAR
    lcs_1_low AT %QW100 : WORD;
    lcs_1_high AT %QW101 : WORD;
    lcs_2_low AT %QW102 : WORD;
    lcs_2_high AT %QW103 : WORD;
    lcs_3_low AT %QW104 : WORD;
    lcs_3_high AT %QW105 : WORD;
    lcs_4_low AT %QW106 : WORD;
    lcs_4_high AT %QW107 : WORD;
    lcf_1_low AT %QW108 : WORD;
    lcf_1_high AT %QW109 : WORD;
    lcf_2_low AT %QW110 : WORD;
    lcf_2_high AT %QW111 : WORD;
    lcf_3_low AT %QW112 : WORD;
    lcf_3_high AT %QW113 : WORD;
    lcf_4_low AT %QW114 : WORD;
    lcf_4_high AT %QW115 : WORD;
    lch_high AT %QW99 : WORD;
    lch_low AT %QW98 : WORD;
    vacuum_freq AT %QW1000 : WORD := 0;
    vacuum_rpm AT %QW1001 : WORD;
    mixer_freq AT %QW1002 : WORD := 0;
    mixer_rpm AT %QW1003 : WORD;
    lcs_1 AT %MD0 : REAL;
    lcs_2 AT %MD1 : REAL;
    lcs_3 AT %MD2 : REAL;
    lcs_4 AT %MD3 : REAL;
    lcf_1 AT %MD4 : REAL;
    lcf_2 AT %MD5 : REAL;
    lcf_3 AT %MD6 : REAL;
    lcf_4 AT %MD7 : REAL;
    lch AT %MD8 : REAL := 0.0;
    flour_weight AT %MD9 : REAL := 0.0;
    flour_weight_high AT %QW117 : WORD;
    flour_weight_low AT %QW116 : WORD;
    sugar_weight AT %MD10 : REAL := 0.0;
    sugar_weight_high AT %QW219 : WORD;
    sugar_weight_low AT %QW218 : WORD;
    rv_1 AT %QX87.4 : BOOL;
    rv_2 AT %QX87.5 : BOOL;
    rv_3 AT %QX87.6 : BOOL;
    dv AT %QX87.7 : BOOL;
    vacuum AT %QX88.0 : BOOL;
    mixer AT %QX88.1 : BOOL;
    flour_alarm AT %QX88.2 : BOOL := false;
    sugar_alarm AT %QX88.3 : BOOL := false;
    hopper_alarm AT %QX88.4 : BOOL := false;
    vacuum_alarm AT %QX88.5 : BOOL := false;
    vacuum_manual AT %QX88.6 : BOOL := false;
    CFM AT %MD11 : REAL;
  END_VAR
  VAR
    words_to_real0 : words_to_real;
    words_to_real1 : words_to_real;
    words_to_real2 : words_to_real;
    words_to_real3 : words_to_real;
    words_to_real4 : words_to_real;
    words_to_real5 : words_to_real;
    words_to_real6 : words_to_real;
    words_to_real7 : words_to_real;
    words_to_real8 : words_to_real;
    flour_alarm_check0 : flour_alarm_check;
    sugar_alarm_check0 : sugar_alarm_check;
    hopper_alarm_check0 : hopper_alarm_check;
    vacuum_alarm_rpm0 : vacuum_alarm_rpm;
    freq_function0 : freq_function;
    real_to_words0 : real_to_words;
    real_to_words1 : real_to_words;
    _TMP_ADD37_OUT : REAL;
    _TMP_ADD38_OUT : REAL;
    _TMP_ADD41_OUT : REAL;
    _TMP_ADD39_OUT : REAL;
    _TMP_ADD40_OUT : REAL;
    _TMP_ADD43_OUT : REAL;
  END_VAR

  words_to_real0(High := lcf_1_high, Low := lcf_1_low);
  lcf_1 := words_to_real0.Result;
  words_to_real1(High := lcf_2_high, Low := lcf_2_low);
  lcf_2 := words_to_real1.Result;
  words_to_real2(High := lcf_3_high, Low := lcf_3_low);
  lcf_3 := words_to_real2.Result;
  words_to_real3(High := lcf_4_high, Low := lcf_4_low);
  lcf_4 := words_to_real3.Result;
  words_to_real4(High := lcs_1_high, Low := lcs_1_low);
  lcs_1 := words_to_real4.Result;
  words_to_real5(High := lcs_2_high, Low := lcs_2_low);
  lcs_2 := words_to_real5.Result;
  words_to_real6(High := lcs_3_high, Low := lcs_3_low);
  lcs_3 := words_to_real6.Result;
  words_to_real7(High := lcs_4_high, Low := lcs_4_low);
  lcs_4 := words_to_real7.Result;
  _TMP_ADD37_OUT := ADD(lcf_1, lcf_2);
  _TMP_ADD38_OUT := ADD(lcf_3, lcf_4);
  _TMP_ADD41_OUT := ADD(_TMP_ADD37_OUT, _TMP_ADD38_OUT);
  flour_weight := _TMP_ADD41_OUT;
  flour_alarm_check0(Flour_weight := flour_weight);
  flour_alarm := flour_alarm_check0.Alarm;
  _TMP_ADD39_OUT := ADD(lcs_1, lcs_2);
  _TMP_ADD40_OUT := ADD(lcs_3, lcs_4);
  _TMP_ADD43_OUT := ADD(_TMP_ADD39_OUT, _TMP_ADD40_OUT);
  sugar_weight := _TMP_ADD43_OUT;
  sugar_alarm_check0(Sugar_weight := sugar_weight);
  sugar_alarm := sugar_alarm_check0.Alarm;
  words_to_real8(High := lch_high, Low := lch_low);
  lch := words_to_real8.Result;
  hopper_alarm_check0(Hopper_weight := lch);
  hopper_alarm := hopper_alarm_check0.Alarm;
  vacuum_alarm_rpm0(Dv := dv, Vacuum_rpm := vacuum_rpm);
  vacuum_alarm := vacuum_alarm_rpm0.Alarm;
  CFM := vacuum_alarm_rpm0.Cfm;
  freq_function0(input_freq := vacuum_freq, manual := vacuum_manual, dv := dv);
  vacuum_freq := freq_function0.output_freq;
  real_to_words0(Input := flour_weight);
  flour_weight_high := real_to_words0.High;
  flour_weight_low := real_to_words0.Low;
  real_to_words1(Input := sugar_weight);
  sugar_weight_high := real_to_words1.High;
  sugar_weight_low := real_to_words1.Low;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : tag_display;
  END_RESOURCE
END_CONFIGURATION
