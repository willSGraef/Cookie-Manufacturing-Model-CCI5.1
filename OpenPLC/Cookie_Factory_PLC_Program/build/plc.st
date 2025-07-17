TYPE
  LOGLEVEL : (CRITICAL, WARNING, INFO, DEBUG) := INFO;
END_TYPE

FUNCTION_BLOCK LOGGER
  VAR_INPUT
    TRIG : BOOL;
    MSG : STRING;
    LEVEL : LOGLEVEL := INFO;
  END_VAR
  VAR
    TRIG0 : BOOL;
  END_VAR

  IF TRIG AND NOT TRIG0 THEN
  {{
   LogMessage(GetFbVar(LEVEL),(char*)GetFbVar(MSG, .body),GetFbVar(MSG, .len));
  }}
  END_IF;
  TRIG0:=TRIG;
END_FUNCTION_BLOCK


FUNCTION_BLOCK mixer_rpm_function
  VAR_OUTPUT
    output_rpm : WORD;
  END_VAR
  VAR_INPUT
    valve_status : BOOL;
    hopper_weight : REAL;
    mixer_status : BOOL;
  END_VAR
  VAR
    noise : INT;
  END_VAR
  VAR_IN_OUT
    counter : INT;
  END_VAR

  counter := counter + 1;
  IF counter > 10000 THEN
    counter := 0;
  END_IF;

  noise := counter MOD 2;

  IF hopper_weight = 0.0 AND mixer_status = 1 THEN
    output_rpm := INT_TO_WORD(70 + noise);
  ELSIF hopper_weight > 0.0 AND mixer_status = 1 AND valve_status = 1 THEN
    output_rpm := INT_TO_WORD(35 + noise);
  ELSE
    output_rpm := INT_TO_WORD(0);
  END_IF;
END_FUNCTION_BLOCK

FUNCTION_BLOCK vacuum_rpm_function
  VAR_INPUT
    input_rpm : WORD;
    vacuum_status : BOOL;
  END_VAR
  VAR_OUTPUT
    output_rpm : WORD;
  END_VAR
  VAR_INPUT
    max_rpm : WORD;
    manual : BOOL := 0;
    dv : BOOL;
  END_VAR
  VAR
    noise : INT;
  END_VAR
  VAR_IN_OUT
    counter : INT;
  END_VAR

  counter := counter + 1;
  IF counter > 10000 THEN
    counter := 0;
  END_IF;

  noise := counter MOD 5;

  IF NOT vacuum_status THEN
    output_rpm := 0;
  ELSIF input_rpm > max_rpm THEN
    output_rpm := INT_TO_WORD(WORD_TO_INT(max_rpm) + noise);
  ELSIF dv = 1 AND manual = 0 THEN
    output_rpm := INT_TO_WORD(1380 + noise);
  ELSIF dv = 0 AND manual = 0 THEN
    output_rpm := INT_TO_WORD(1980 + noise);
  ELSE
    IF (counter MOD 2) = 1 THEN
      output_rpm := INT_TO_WORD(WORD_TO_INT(input_rpm) + noise);
    ELSE
      output_rpm := INT_TO_WORD(WORD_TO_INT(input_rpm) - noise);
      END_IF;
  END_IF;
END_FUNCTION_BLOCK

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

  IF Flour_weight <= 100.0 THEN
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

  IF Sugar_weight <= 100.0 THEN
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

  IF Hopper_weight <= 10.0 THEN
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

FUNCTION_BLOCK papercutter_cpm_function
  VAR
    rows : REAL;
  END_VAR
  VAR_INPUT
    paper_cutter : BOOL;
    cookie_diameter : REAL;
    cookie_margin : REAL;
    conveyor_fpm : REAL;
  END_VAR
  VAR
    float_cpm : REAL;
  END_VAR
  VAR_OUTPUT
    papercut_cpm : WORD;
  END_VAR


  IF paper_cutter THEN
    rows := 5.0;
    float_cpm := (conveyor_fpm * 12.0) / (rows * (cookie_diameter+cookie_margin));
    papercut_cpm := REAL_TO_WORD(float_cpm);
  ELSE
    papercut_cpm := 0;
  END_IF;
END_FUNCTION_BLOCK

FUNCTION_BLOCK conveyor_fpm_function
  VAR_INPUT
    wirecut_cpm : WORD;
    cookie_margin : REAL;
    cookie_diameter : REAL;
  END_VAR
  VAR_OUTPUT
    conveyor_fpm : REAL;
  END_VAR

  conveyor_fpm := (WORD_TO_REAL(wirecut_cpm) * (cookie_margin+cookie_diameter)) / 12.0;
END_FUNCTION_BLOCK

FUNCTION_BLOCK wirecut_cpm_function
  VAR_IN_OUT
    wirecut_cpm : WORD;
  END_VAR
  VAR_INPUT
    wirecutter : BOOL;
  END_VAR

  IF wirecutter THEN
    wirecut_cpm := 200;
  ELSE
    wirecut_cpm := 0;
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
    lcm_high AT %QW97 : WORD;
    lcm_low AT %QW96 : WORD;
    trough_weight_high AT %QW95 : WORD;
    trough_weight_low AT %QW94 : WORD;
    vacuum_rpm AT %QW1001 : WORD;
    mixer_rpm AT %QW1003 : WORD;
    wirecut_cpm AT %QW1006 : WORD := 0;
    papercut_cpm AT %QW1009 : WORD;
    conveyor_fpm AT %MD13 : REAL;
    conveyor_fpm_high AT %QW121 : WORD := 0;
    conveyor_fpm_low AT %QW120 : WORD := 0;
    lcs_1 AT %MD0 : REAL;
    lcs_2 AT %MD1 : REAL;
    lcs_3 AT %MD2 : REAL;
    lcs_4 AT %MD3 : REAL;
    lcf_1 AT %MD4 : REAL;
    lcf_2 AT %MD5 : REAL;
    lcf_3 AT %MD6 : REAL;
    lcf_4 AT %MD7 : REAL;
    lch AT %MD8 : REAL := 0.0;
    lcm AT %MD11 : REAL := 0.0;
    trough_weight AT %MD12 : REAL := 0.0;
    flour_weight AT %MD9 : REAL := 0.0;
    flour_weight_high AT %QW117 : WORD;
    flour_weight_low AT %QW116 : WORD;
    sugar_weight AT %MD10 : REAL := 0.0;
    sugar_weight_high AT %QW219 : WORD;
    sugar_weight_low AT %QW218 : WORD;
    tunnel_temp AT %MD15 : REAL;
    tunnel_temp_high AT %QW303 : WORD := 0;
    tunnel_temp_low AT %QW302 : WORD := 0;
    nitrogen_volume AT %MD14 : REAL;
    nitrogen_volume_high AT %QW301 : WORD := 0;
    nitrogen_volume_low AT %QW300 : WORD := 0;
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
    wirecutter AT %QX88.7 : BOOL := false;
    trough_transfer AT %QX89.1 : BOOL := false;
    conveyor AT %QX89.4 : BOOL := false;
    paper_cutter AT %QX89.5 : BOOL := false;
    gv_1 AT %QX89.6 : BOOL := false;
    fan_1 AT %QX89.7 : BOOL := false;
    fan_2 AT %QX90.0 : BOOL := false;
    fan_3 AT %QX90.1 : BOOL := false;
    exhaust_fan AT %QX90.2 : BOOL := false;
    CFM AT %MD20 : REAL;
    reset AT %QX125.0 : BOOL := false;
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
    real_to_words0 : real_to_words;
    real_to_words1 : real_to_words;
    vacuum_max_rpm : WORD := 3600;
    mixer_max_rpm : WORD := 80;
    mixer_rpm_function0 : mixer_rpm_function;
    vacuum_rpm_function0 : vacuum_rpm_function;
    cookie_diameter : REAL := 1.5;
    cookie_margin : REAL := 0.5;
    papercutter_cpm_function0 : papercutter_cpm_function;
    conveyor_fpm_function0 : conveyor_fpm_function;
    words_to_real9 : words_to_real;
    words_to_real10 : words_to_real;
    real_to_words2 : real_to_words;
    wirecut_cpm_function0 : wirecut_cpm_function;
    words_to_real11 : words_to_real;
    words_to_real12 : words_to_real;
  END_VAR
  VAR
    box_maker AT %QX90.3 : BOOL := false;
    taper AT %QX90.4 : BOOL := false;
    labeler AT %QX90.5 : BOOL := false;
    palletizer AT %QX90.6 : BOOL := false;
    palletizer_grabbing AT %QX90.7 : BOOL := false;
    wrapper AT %QX91.0 : BOOL := false;
    wrapper_conveyor AT %QX91.1 : BOOL := false;
    conveyor_1 AT %QX91.2 : BOOL := false;
    conveyor_2 AT %QX91.3 : BOOL := false;
    conveyor_3 AT %QX91.4 : BOOL := false;
    conveyor_4 AT %QX91.5 : BOOL := false;
    conveyor_5 AT %QX91.6 : BOOL := false;
    ps_1 AT %QX91.7 : BOOL := false;
    ps_2 AT %QX92.0 : BOOL := false;
    ps_3 AT %QX92.1 : BOOL := false;
    ps_4 AT %QX92.2 : BOOL := false;
    ps_5 AT %QX92.3 : BOOL := false;
    ps_6 AT %QX92.4 : BOOL := false;
    bagger AT %QX92.5 : BOOL := false;
    palletizer_position AT %QW1012 : WORD := 0;
    box_count AT %QW1013 : WORD := 0;
    wrapper_wrapping AT %QX92.6 : BOOL := false;
    box_maker_boxing AT %QX92.7 : BOOL := false;
  END_VAR
  VAR
    _TMP_ADD37_OUT : REAL;
    _TMP_ADD38_OUT : REAL;
    _TMP_ADD41_OUT : REAL;
    _TMP_ADD39_OUT : REAL;
    _TMP_ADD40_OUT : REAL;
    _TMP_ADD43_OUT : REAL;
    mixer_rpm_function0_counter : INT;
    vacuum_rpm_function0_counter : INT;
    _TMP_EQ140_OUT : BOOL;
    _TMP_AND143_OUT : BOOL;
    _TMP_WORD_TO_INT162_OUT : INT;
    _TMP_ADD161_ENO : BOOL;
    _TMP_ADD161_OUT : INT;
    _TMP_INT_TO_WORD163_OUT : WORD;
    _TMP_EQ103_OUT : BOOL;
    _TMP_AND152_OUT : BOOL;
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
  words_to_real8(High := lch_high, Low := lch_low);
  lch := words_to_real8.Result;
  hopper_alarm_check0(Hopper_weight := lch);
  hopper_alarm := hopper_alarm_check0.Alarm;
  vacuum_alarm_rpm0(Dv := dv, Vacuum_rpm := vacuum_rpm);
  vacuum_alarm := vacuum_alarm_rpm0.Alarm;
  CFM := vacuum_alarm_rpm0.Cfm;
  real_to_words0(Input := flour_weight);
  flour_weight_high := real_to_words0.High;
  flour_weight_low := real_to_words0.Low;
  real_to_words1(Input := sugar_weight);
  sugar_weight_high := real_to_words1.High;
  sugar_weight_low := real_to_words1.Low;
  sugar_alarm_check0(Sugar_weight := sugar_weight);
  sugar_alarm := sugar_alarm_check0.Alarm;
  mixer_rpm_function0_counter := mixer_rpm_function0.counter;
  mixer_rpm_function0(valve_status := rv_3, hopper_weight := lch, mixer_status := mixer, counter := mixer_rpm_function0_counter);
  mixer_rpm := mixer_rpm_function0.output_rpm;
  vacuum_rpm_function0_counter := vacuum_rpm_function0.counter;
  vacuum_rpm_function0(input_rpm := vacuum_rpm, vacuum_status := vacuum, max_rpm := vacuum_max_rpm, manual := vacuum_manual, dv := dv, counter := vacuum_rpm_function0_counter);
  vacuum_rpm := vacuum_rpm_function0.output_rpm;
  words_to_real9(High := trough_weight_high, Low := trough_weight_low);
  trough_weight := words_to_real9.Result;
  words_to_real10(High := lcm_high, Low := lcm_low);
  lcm := words_to_real10.Result;
  papercutter_cpm_function0(paper_cutter := paper_cutter, cookie_diameter := cookie_diameter, cookie_margin := cookie_margin, conveyor_fpm := conveyor_fpm);
  papercut_cpm := papercutter_cpm_function0.papercut_cpm;
  conveyor_fpm_function0(wirecut_cpm := wirecut_cpm, cookie_margin := cookie_margin, cookie_diameter := cookie_diameter);
  conveyor_fpm := conveyor_fpm_function0.conveyor_fpm;
  real_to_words2(Input := conveyor_fpm);
  conveyor_fpm_high := real_to_words2.High;
  conveyor_fpm_low := real_to_words2.Low;
  wirecut_cpm_function0(wirecut_cpm := wirecut_cpm, wirecutter := wirecutter);
  wirecut_cpm := wirecut_cpm_function0.wirecut_cpm;
  words_to_real11(High := nitrogen_volume_high, Low := nitrogen_volume_low);
  nitrogen_volume := words_to_real11.Result;
  words_to_real12(High := tunnel_temp_high, Low := tunnel_temp_low);
  tunnel_temp := words_to_real12.Result;
  box_maker_boxing := box_maker AND NOT(ps_1) AND NOT(box_maker_boxing);
  conveyor_1 := NOT(ps_1) AND NOT(box_maker_boxing) AND NOT(conveyor_2);
  conveyor_2 := box_maker_boxing AND bagger AND NOT(ps_2) AND NOT(ps_1) AND NOT(conveyor_1);
  conveyor_3 := ps_4 AND NOT(ps_5) AND ps_3;
  ps_3 := NOT(ps_4 AND NOT(ps_5) AND ps_3);
  _TMP_EQ140_OUT := EQ(palletizer_position, 2);
  _TMP_AND143_OUT := AND(palletizer_grabbing, _TMP_EQ140_OUT);
  palletizer_grabbing := NOT(_TMP_AND143_OUT);
  _TMP_WORD_TO_INT162_OUT := WORD_TO_INT(box_count);
  _TMP_ADD161_OUT := ADD(EN := _TMP_AND143_OUT, IN1 := 1, IN2 := _TMP_WORD_TO_INT162_OUT, ENO => _TMP_ADD161_ENO);
  _TMP_INT_TO_WORD163_OUT := INT_TO_WORD(_TMP_ADD161_OUT);
  box_count := _TMP_INT_TO_WORD163_OUT;
  _TMP_EQ103_OUT := EQ(box_count, 40);
  _TMP_AND152_OUT := AND(wrapper, _TMP_EQ103_OUT);
  wrapper_wrapping := _TMP_AND152_OUT;
  conveyor_5 := ps_6;
END_PROGRAM

FUNCTION_BLOCK wirecut_ppm_function
  VAR_INPUT
    wirecut_cpm : WORD;
  END_VAR
  VAR
    cookie_weight : REAL;
    cookie_count : REAL;
  END_VAR
  VAR_OUTPUT
    wirecut_ppm : REAL;
  END_VAR

  cookie_count := 8.0;
  cookie_weight := 0.3;
  wirecut_ppm := (WORD_TO_REAL(wirecut_cpm) * cookie_count * cookie_weight)/16.0;
END_FUNCTION_BLOCK


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : tag_display;
  END_RESOURCE
END_CONFIGURATION
