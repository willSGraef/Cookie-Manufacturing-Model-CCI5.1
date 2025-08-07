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
    wrapper_conveying AT %QX91.1 : BOOL := false;
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
    palletizer_position AT %QW1012 : INT := 0;
    box_count AT %QW1013 : INT := 0;
    wrapper_wrapping AT %QX92.6 : BOOL := false;
    boxing AT %QX92.7 : BOOL := false;
    taping AT %QX93.0 : BOOL := false;
    conveying_1 AT %QX93.1 : BOOL := false;
    conveying_2 AT %QX93.2 : BOOL := false;
    conveying_3 AT %QX93.3 : BOOL := false;
    conveying_4 AT %QX93.4 : BOOL := false;
    conveying_5 AT %QX93.5 : BOOL := false;
    palletizer_moving AT %QX93.6 : BOOL := false;
    bagging AT %QX93.7 : BOOL := false;
    machine_1 AT %QW500 : INT := 0;
    machine_2 AT %QW501 : INT := 0;
    machine_3 AT %QW502 : INT := 0;
    machine_4 AT %QW503 : INT := 0;
  END_VAR
  VAR
    state_0 : INT := 0;
    state_1 : INT := 1;
    state_2 : INT := 2;
    state_3 : INT := 3;
    state_4 : INT := 4;
    state_5 : INT := 5;
    count_1 : INT := 1;
    box_req : INT := 40;
    pos_0 : INT := 0;
    pos_1 : INT := 1;
    pos_2 : INT := 2;
    _TMP_ADD37_OUT : REAL;
    _TMP_ADD38_OUT : REAL;
    _TMP_ADD41_OUT : REAL;
    _TMP_ADD39_OUT : REAL;
    _TMP_ADD40_OUT : REAL;
    _TMP_ADD43_OUT : REAL;
    mixer_rpm_function0_counter : INT;
    vacuum_rpm_function0_counter : INT;
    _TMP_EQ167_OUT : BOOL;
    _TMP_MOVE174_ENO : BOOL;
    _TMP_MOVE174_OUT : INT;
    _TMP_EQ113_OUT : BOOL;
    _TMP_MOVE120_ENO : BOOL;
    _TMP_MOVE120_OUT : INT;
    _TMP_EQ125_OUT : BOOL;
    _TMP_MOVE133_ENO : BOOL;
    _TMP_MOVE133_OUT : INT;
    _TMP_EQ130_OUT : BOOL;
    _TMP_MOVE144_ENO : BOOL;
    _TMP_MOVE144_OUT : INT;
    _TMP_EQ141_OUT : BOOL;
    _TMP_MOVE156_ENO : BOOL;
    _TMP_MOVE156_OUT : INT;
    _TMP_EQ154_OUT : BOOL;
    _TMP_ADD163_ENO : BOOL;
    _TMP_ADD163_OUT : INT;
    _TMP_EQ171_OUT : BOOL;
    _TMP_ADD188_ENO : BOOL;
    _TMP_ADD188_OUT : INT;
    _TMP_EQ197_OUT : BOOL;
    _TMP_ADD205_ENO : BOOL;
    _TMP_ADD205_OUT : INT;
    _TMP_EQ210_OUT : BOOL;
    _TMP_EQ219_OUT : BOOL;
    _TMP_EQ237_OUT : BOOL;
    _TMP_AND354_OUT : BOOL;
    _TMP_ADD231_ENO : BOOL;
    _TMP_ADD231_OUT : INT;
    _TMP_EQ243_OUT : BOOL;
    _TMP_ADD281_ENO : BOOL;
    _TMP_ADD281_OUT : INT;
    _TMP_MOVE228_ENO : BOOL;
    _TMP_MOVE228_OUT : INT;
    _TMP_MOVE238_ENO : BOOL;
    _TMP_MOVE238_OUT : INT;
    _TMP_EQ264_OUT : BOOL;
    _TMP_MOVE241_ENO : BOOL;
    _TMP_MOVE241_OUT : INT;
    _TMP_ADD248_ENO : BOOL;
    _TMP_ADD248_OUT : INT;
    _TMP_MOVE252_ENO : BOOL;
    _TMP_MOVE252_OUT : INT;
    _TMP_EQ256_OUT : BOOL;
    _TMP_EQ263_OUT : BOOL;
    _TMP_AND353_OUT : BOOL;
    _TMP_MOVE224_ENO : BOOL;
    _TMP_MOVE224_OUT : INT;
    _TMP_EQ270_OUT : BOOL;
    _TMP_ADD290_ENO : BOOL;
    _TMP_ADD290_OUT : INT;
    _TMP_ADD310_ENO : BOOL;
    _TMP_ADD310_OUT : INT;
    _TMP_EQ314_OUT : BOOL;
    _TMP_MOVE321_ENO : BOOL;
    _TMP_MOVE321_OUT : INT;
    _TMP_EQ145_OUT : BOOL;
    _TMP_MOVE336_ENO : BOOL;
    _TMP_MOVE336_OUT : INT;
    _TMP_EQ347_OUT : BOOL;
    _TMP_MOVE345_ENO : BOOL;
    _TMP_MOVE345_OUT : INT;
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
  _TMP_EQ167_OUT := EQ(machine_1, state_0);
  IF box_maker AND _TMP_EQ167_OUT THEN
    boxing := TRUE; (*set*)
  END_IF;
  _TMP_MOVE174_OUT := MOVE(EN := box_maker AND _TMP_EQ167_OUT, IN := state_1, ENO => _TMP_MOVE174_ENO);
  IF _TMP_MOVE174_ENO THEN
      machine_1 := _TMP_MOVE174_OUT;
  END_IF;
  _TMP_EQ113_OUT := EQ(machine_1, state_1);
  IF conveyor_1 AND NOT(boxing) AND _TMP_EQ113_OUT THEN
    conveying_1 := TRUE; (*set*)
  END_IF;
  _TMP_MOVE120_OUT := MOVE(EN := conveyor_1 AND NOT(boxing) AND _TMP_EQ113_OUT, IN := state_2, ENO => _TMP_MOVE120_ENO);
  IF _TMP_MOVE120_ENO THEN
      machine_1 := _TMP_MOVE120_OUT;
  END_IF;
  _TMP_EQ125_OUT := EQ(machine_1, state_2);
  IF bagger AND ps_1 AND _TMP_EQ125_OUT THEN
    bagging := TRUE; (*set*)
  END_IF;
  _TMP_MOVE133_OUT := MOVE(EN := bagger AND ps_1 AND _TMP_EQ125_OUT, IN := state_3, ENO => _TMP_MOVE133_ENO);
  IF _TMP_MOVE133_ENO THEN
      machine_1 := _TMP_MOVE133_OUT;
  END_IF;
  _TMP_EQ130_OUT := EQ(machine_1, state_3);
  _TMP_MOVE144_OUT := MOVE(EN := NOT(bagging) AND conveyor_2 AND _TMP_EQ130_OUT, IN := state_4, ENO => _TMP_MOVE144_ENO);
  IF _TMP_MOVE144_ENO THEN
      machine_1 := _TMP_MOVE144_OUT;
  END_IF;
  IF NOT(bagging) AND conveyor_2 AND _TMP_EQ130_OUT THEN
    conveying_2 := TRUE; (*set*)
  END_IF;
  _TMP_EQ141_OUT := EQ(machine_1, state_5);
  _TMP_MOVE156_OUT := MOVE(EN := NOT(ps_2) AND _TMP_EQ141_OUT, IN := state_0, ENO => _TMP_MOVE156_ENO);
  IF _TMP_MOVE156_ENO THEN
      machine_1 := _TMP_MOVE156_OUT;
  END_IF;
  _TMP_EQ154_OUT := EQ(machine_2, state_0);
  IF ps_3 AND conveyor_3 AND _TMP_EQ154_OUT THEN
    conveying_3 := TRUE; (*set*)
  END_IF;
  _TMP_ADD163_OUT := ADD(EN := ps_3 AND conveyor_3 AND _TMP_EQ154_OUT, IN1 := machine_2, IN2 := count_1, ENO => _TMP_ADD163_ENO);
  IF _TMP_ADD163_ENO THEN
      machine_2 := _TMP_ADD163_OUT;
  END_IF;
  _TMP_EQ171_OUT := EQ(machine_2, state_1);
  IF ps_4 AND taper AND _TMP_EQ171_OUT THEN
    taping := TRUE; (*set*)
  END_IF;
  _TMP_ADD188_OUT := ADD(EN := ps_4 AND taper AND _TMP_EQ171_OUT, IN1 := machine_2, IN2 := count_1, ENO => _TMP_ADD188_ENO);
  IF _TMP_ADD188_ENO THEN
      machine_2 := _TMP_ADD188_OUT;
  END_IF;
  _TMP_EQ197_OUT := EQ(machine_2, state_2);
  IF conveyor_4 AND NOT(taping) AND _TMP_EQ197_OUT THEN
    conveying_4 := TRUE; (*set*)
  END_IF;
  _TMP_ADD205_OUT := ADD(EN := conveyor_4 AND NOT(taping) AND _TMP_EQ197_OUT, IN1 := machine_2, IN2 := count_1, ENO => _TMP_ADD205_ENO);
  IF _TMP_ADD205_ENO THEN
      machine_2 := _TMP_ADD205_OUT;
  END_IF;
  _TMP_EQ210_OUT := EQ(machine_2, state_3);
  IF palletizer AND NOT(palletizer_grabbing) AND NOT(palletizer_moving) AND ps_5 AND _TMP_EQ210_OUT THEN
    palletizer_moving := TRUE; (*set*)
  END_IF;
  _TMP_EQ219_OUT := EQ(machine_3, state_0);
  _TMP_EQ237_OUT := EQ(palletizer_position, pos_0);
  _TMP_AND354_OUT := AND(_TMP_EQ219_OUT, _TMP_EQ237_OUT);
  _TMP_ADD231_OUT := ADD(EN := palletizer AND NOT(palletizer_moving) AND ps_5 AND _TMP_AND354_OUT, IN1 := machine_3, IN2 := count_1, ENO => _TMP_ADD231_ENO);
  IF _TMP_ADD231_ENO THEN
      machine_3 := _TMP_ADD231_OUT;
  END_IF;
  IF palletizer AND NOT(palletizer_moving) AND ps_5 AND _TMP_AND354_OUT THEN
    palletizer_grabbing := TRUE; (*set*)
  END_IF;
  _TMP_EQ243_OUT := EQ(machine_3, state_1);
  IF palletizer AND NOT(wrapper_conveying) AND NOT(wrapper_wrapping) AND NOT(palletizer_moving) AND _TMP_EQ243_OUT THEN
    palletizer_moving := TRUE; (*set*)
  END_IF;
  _TMP_ADD281_OUT := ADD(EN := palletizer AND NOT(wrapper_conveying) AND NOT(wrapper_wrapping) AND NOT(palletizer_moving) AND _TMP_EQ243_OUT, IN1 := machine_3, IN2 := count_1, ENO => _TMP_ADD281_ENO);
  IF _TMP_ADD281_ENO THEN
      machine_3 := _TMP_ADD281_OUT;
  END_IF;
  _TMP_MOVE228_OUT := MOVE(EN := NOT(palletizer_moving) AND ps_5 AND _TMP_AND354_OUT, IN := pos_1, ENO => _TMP_MOVE228_ENO);
  IF _TMP_MOVE228_ENO THEN
      palletizer_position := _TMP_MOVE228_OUT;
  END_IF;
  _TMP_MOVE238_OUT := MOVE(EN := NOT(palletizer_moving) AND _TMP_EQ243_OUT, IN := pos_0, ENO => _TMP_MOVE238_ENO);
  IF _TMP_MOVE238_ENO THEN
      palletizer_position := _TMP_MOVE238_OUT;
  END_IF;
  _TMP_EQ264_OUT := EQ(machine_3, state_2);
  _TMP_MOVE241_OUT := MOVE(EN := NOT(palletizer_moving) AND _TMP_EQ264_OUT, IN := pos_2, ENO => _TMP_MOVE241_ENO);
  IF _TMP_MOVE241_ENO THEN
      palletizer_position := _TMP_MOVE241_OUT;
  END_IF;
  _TMP_ADD248_OUT := ADD(EN := palletizer AND NOT(palletizer_moving) AND _TMP_EQ264_OUT, IN1 := box_count, IN2 := count_1, ENO => _TMP_ADD248_ENO);
  IF _TMP_ADD248_ENO THEN
      box_count := _TMP_ADD248_OUT;
  END_IF;
  _TMP_MOVE252_OUT := MOVE(EN := palletizer AND NOT(palletizer_moving) AND _TMP_EQ264_OUT, IN := state_0, ENO => _TMP_MOVE252_ENO);
  IF _TMP_MOVE252_ENO THEN
      machine_3 := _TMP_MOVE252_OUT;
  END_IF;
  _TMP_EQ256_OUT := EQ(machine_4, state_0);
  _TMP_EQ263_OUT := EQ(box_count, box_req);
  _TMP_AND353_OUT := AND(_TMP_EQ256_OUT, _TMP_EQ263_OUT);
  IF NOT(ps_6) AND wrapper AND _TMP_AND353_OUT THEN
    wrapper_wrapping := TRUE; (*set*)
  END_IF;
  _TMP_MOVE224_OUT := MOVE(EN := palletizer AND NOT(palletizer_grabbing) AND NOT(palletizer_moving) AND ps_5 AND _TMP_EQ210_OUT, IN := state_0, ENO => _TMP_MOVE224_ENO);
  IF _TMP_MOVE224_ENO THEN
      machine_2 := _TMP_MOVE224_OUT;
  END_IF;
  _TMP_EQ270_OUT := EQ(machine_4, state_1);
  IF NOT(wrapper_wrapping) AND wrapper AND _TMP_EQ270_OUT THEN
    wrapper_conveying := TRUE; (*set*)
  END_IF;
  _TMP_ADD290_OUT := ADD(EN := NOT(ps_6) AND wrapper AND _TMP_AND353_OUT, IN1 := machine_4, IN2 := count_1, ENO => _TMP_ADD290_ENO);
  IF _TMP_ADD290_ENO THEN
      machine_4 := _TMP_ADD290_OUT;
  END_IF;
  _TMP_ADD310_OUT := ADD(EN := NOT(wrapper_wrapping) AND wrapper AND _TMP_EQ270_OUT, IN1 := machine_4, IN2 := count_1, ENO => _TMP_ADD310_ENO);
  IF _TMP_ADD310_ENO THEN
      machine_4 := _TMP_ADD310_OUT;
  END_IF;
  _TMP_EQ314_OUT := EQ(machine_4, state_2);
  IF NOT(wrapper_conveying) AND conveyor_4 AND _TMP_EQ314_OUT THEN
    conveying_4 := TRUE; (*set*)
  END_IF;
  _TMP_MOVE321_OUT := MOVE(EN := NOT(wrapper_conveying) AND conveyor_4 AND _TMP_EQ314_OUT, IN := state_3, ENO => _TMP_MOVE321_ENO);
  IF _TMP_MOVE321_ENO THEN
      machine_4 := _TMP_MOVE321_OUT;
  END_IF;
  IF palletizer AND NOT(palletizer_moving) AND ps_5 AND _TMP_AND354_OUT THEN
    palletizer_moving := TRUE; (*set*)
  END_IF;
  IF palletizer AND NOT(palletizer_moving) AND _TMP_EQ264_OUT THEN
    palletizer_grabbing := FALSE; (*reset*)
  END_IF;
  IF palletizer AND NOT(palletizer_moving) AND _TMP_EQ264_OUT THEN
    palletizer_moving := TRUE; (*set*)
  END_IF;
  IF ps_1 AND _TMP_EQ125_OUT THEN
    conveying_1 := FALSE; (*reset*)
  END_IF;
  _TMP_EQ145_OUT := EQ(machine_1, state_4);
  _TMP_MOVE336_OUT := MOVE(EN := ps_2 AND _TMP_EQ145_OUT, IN := state_5, ENO => _TMP_MOVE336_ENO);
  IF _TMP_MOVE336_ENO THEN
      machine_1 := _TMP_MOVE336_OUT;
  END_IF;
  IF ps_2 AND _TMP_EQ145_OUT THEN
    conveying_2 := FALSE; (*reset*)
  END_IF;
  IF ps_4 AND taper AND _TMP_EQ171_OUT THEN
    conveying_3 := FALSE; (*reset*)
  END_IF;
  IF ps_5 AND _TMP_EQ210_OUT THEN
    conveying_4 := FALSE; (*reset*)
  END_IF;
  _TMP_EQ347_OUT := EQ(machine_4, state_3);
  _TMP_MOVE345_OUT := MOVE(EN := ps_6 AND _TMP_EQ347_OUT, IN := state_5, ENO => _TMP_MOVE345_ENO);
  IF _TMP_MOVE345_ENO THEN
      machine_4 := _TMP_MOVE345_OUT;
  END_IF;
  IF ps_6 AND _TMP_EQ347_OUT THEN
    conveying_4 := FALSE; (*reset*)
  END_IF;
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

(*DBG:char md5[] = "c6137e8f851bdeaf0c6c31fb0144d0eb";*)
(*DBG:/**)
(*DBG: * This file is part of OpenPLC Runtime*)
(*DBG: **)
(*DBG: * Copyright (C) 2023 Autonomy, GP Orcullo*)
(*DBG: * Based on the work by GP Orcullo on Beremiz for uC*)
(*DBG: **)
(*DBG: * This program is free software; you can redistribute it and/or*)
(*DBG: * modify it under the terms of the GNU General Public License*)
(*DBG: * as published by the Free Software Foundation; either version 2*)
(*DBG: * of the License, or (at your option) any later version.*)
(*DBG: **)
(*DBG: * This program is distributed in the hope that it will be useful,*)
(*DBG: * but WITHOUT ANY WARRANTY; without even the implied warranty of*)
(*DBG: * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the*)
(*DBG: * GNU General Public License for more details.*)
(*DBG: **)
(*DBG: * You should have received a copy of the GNU General Public License*)
(*DBG: * along with this program; If not, see <http://www.gnu.org/licenses/>.*)
(*DBG: **)
(*DBG: */*)
(*DBG:*)
(*DBG:#include <stdbool.h>*)
(*DBG:*)
(*DBG:#include "iec_types_all.h"*)
(*DBG:#include "POUS.h"*)
(*DBG:*)
(*DBG:#define SAME_ENDIANNESS      0*)
(*DBG:#define REVERSE_ENDIANNESS   1*)
(*DBG:*)
(*DBG:uint8_t endianness;*)
(*DBG:*)
(*DBG:*)
(*DBG:extern TAG_DISPLAY RES0__INSTANCE0;*)
(*DBG:*)
(*DBG:static const struct {*)
(*DBG:    void *ptr;*)
(*DBG:    __IEC_types_enum type;*)
(*DBG:} debug_vars[] = {*)
(*DBG:    {&(RES0__INSTANCE0.LCS_1_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_1_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_2_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_2_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_3_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_3_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_4_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_4_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_1_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_1_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_2_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_2_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_3_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_3_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_4_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_4_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCH_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCH_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCM_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCM_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.TROUGH_WEIGHT_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.TROUGH_WEIGHT_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WIRECUT_CPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUT_CPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_FPM), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_FPM_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_FPM_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_1), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_2), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_3), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_4), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_1), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_2), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_3), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_4), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCH), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCM), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.TROUGH_WEIGHT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_WEIGHT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_WEIGHT_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_WEIGHT_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_WEIGHT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_WEIGHT_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_WEIGHT_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.TUNNEL_TEMP), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.TUNNEL_TEMP_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.TUNNEL_TEMP_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.NITROGEN_VOLUME), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.NITROGEN_VOLUME_HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.NITROGEN_VOLUME_LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.RV_1), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.RV_2), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.RV_3), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.DV), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_ALARM), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_ALARM), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.HOPPER_ALARM), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_ALARM), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_MANUAL), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WIRECUTTER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.TROUGH_TRANSFER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPER_CUTTER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.GV_1), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FAN_1), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FAN_2), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FAN_3), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.EXHAUST_FAN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CFM), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.RESET), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL0.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL0.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL0.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL1.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL1.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL1.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL1.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL1.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL2.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL2.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL2.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL2.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL2.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL3.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL3.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL3.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL3.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL3.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL4.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL4.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL4.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL4.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL4.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL5.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL5.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL5.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL5.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL5.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL6.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL6.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL6.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL6.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL6.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL7.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL7.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL7.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL7.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL7.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL8.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL8.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL8.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL8.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL8.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_ALARM_CHECK0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_ALARM_CHECK0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_ALARM_CHECK0.FLOUR_WEIGHT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_ALARM_CHECK0.ALARM), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_ALARM_CHECK0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_ALARM_CHECK0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_ALARM_CHECK0.SUGAR_WEIGHT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_ALARM_CHECK0.ALARM), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.HOPPER_ALARM_CHECK0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.HOPPER_ALARM_CHECK0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.HOPPER_ALARM_CHECK0.HOPPER_WEIGHT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.HOPPER_ALARM_CHECK0.ALARM), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_ALARM_RPM0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_ALARM_RPM0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_ALARM_RPM0.DV), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_ALARM_RPM0.CFM), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_ALARM_RPM0.VACUUM_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_ALARM_RPM0.ALARM), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS0.INPUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS0.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS0.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS1.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS1.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS1.INPUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS1.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS1.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_MAX_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_MAX_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM_FUNCTION0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM_FUNCTION0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM_FUNCTION0.OUTPUT_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM_FUNCTION0.VALVE_STATUS), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM_FUNCTION0.HOPPER_WEIGHT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM_FUNCTION0.MIXER_STATUS), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM_FUNCTION0.NOISE), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM_FUNCTION0.COUNTER), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.INPUT_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.VACUUM_STATUS), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.OUTPUT_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.MAX_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.MANUAL), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.DV), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.NOISE), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0.COUNTER), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.COOKIE_DIAMETER), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.COOKIE_MARGIN), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUTTER_CPM_FUNCTION0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUTTER_CPM_FUNCTION0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUTTER_CPM_FUNCTION0.ROWS), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUTTER_CPM_FUNCTION0.PAPER_CUTTER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUTTER_CPM_FUNCTION0.COOKIE_DIAMETER), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUTTER_CPM_FUNCTION0.COOKIE_MARGIN), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUTTER_CPM_FUNCTION0.CONVEYOR_FPM), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUTTER_CPM_FUNCTION0.FLOAT_CPM), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PAPERCUTTER_CPM_FUNCTION0.PAPERCUT_CPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_FPM_FUNCTION0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_FPM_FUNCTION0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_FPM_FUNCTION0.WIRECUT_CPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_FPM_FUNCTION0.COOKIE_MARGIN), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_FPM_FUNCTION0.COOKIE_DIAMETER), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_FPM_FUNCTION0.CONVEYOR_FPM), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL9.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL9.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL9.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL9.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL9.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL10.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL10.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL10.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL10.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL10.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS2.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS2.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS2.INPUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS2.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.REAL_TO_WORDS2.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WIRECUT_CPM_FUNCTION0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WIRECUT_CPM_FUNCTION0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WIRECUT_CPM_FUNCTION0.WIRECUT_CPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WIRECUT_CPM_FUNCTION0.WIRECUTTER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL11.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL11.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL11.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL11.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL11.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL12.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL12.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL12.HIGH), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL12.LOW), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WORDS_TO_REAL12.RESULT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.BOX_MAKER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.TAPER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LABELER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PALLETIZER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PALLETIZER_GRABBING), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WRAPPER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WRAPPER_CONVEYING), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_1), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_2), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_3), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_4), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYOR_5), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PS_1), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PS_2), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PS_3), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PS_4), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PS_5), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PS_6), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.BAGGER), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PALLETIZER_POSITION), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.BOX_COUNT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.WRAPPER_WRAPPING), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.BOXING), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.TAPING), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYING_1), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYING_2), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYING_3), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYING_4), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CONVEYING_5), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PALLETIZER_MOVING), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.BAGGING), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MACHINE_1), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MACHINE_2), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MACHINE_3), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MACHINE_4), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.STATE_0), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.STATE_1), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.STATE_2), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.STATE_3), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.STATE_4), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.STATE_5), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.COUNT_1), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.BOX_REQ), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.POS_0), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.POS_1), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.POS_2), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD37_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD38_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD41_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD39_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD40_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD43_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM_FUNCTION0_COUNTER), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM_FUNCTION0_COUNTER), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ167_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE174_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE174_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ113_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE120_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE120_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ125_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE133_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE133_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ130_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE144_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE144_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ141_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE156_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE156_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ154_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD163_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD163_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ171_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD188_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD188_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ197_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD205_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD205_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ210_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ219_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ237_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_AND354_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD231_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD231_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ243_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD281_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD281_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE228_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE228_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE238_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE238_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ264_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE241_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE241_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD248_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD248_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE252_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE252_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ256_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ263_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_AND353_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE224_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE224_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ270_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD290_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD290_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD310_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD310_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ314_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE321_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE321_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ145_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE336_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE336_OUT), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_EQ347_OUT), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE345_ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_MOVE345_OUT), INT_ENUM},*)
(*DBG:};*)
(*DBG:*)
(*DBG:#define VAR_COUNT               330*)
(*DBG:*)
(*DBG:uint16_t get_var_count(void)*)
(*DBG:{*)
(*DBG:    return VAR_COUNT;*)
(*DBG:}*)
(*DBG:*)
(*DBG:size_t get_var_size(size_t idx)*)
(*DBG:{*)
(*DBG:    if (idx >= VAR_COUNT)*)
(*DBG:    {*)
(*DBG:        return 0;*)
(*DBG:    }*)
(*DBG:    switch (debug_vars[idx].type) {*)
(*DBG:    case BOOL_ENUM:*)
(*DBG:        return sizeof(BOOL);*)
(*DBG:    case INT_ENUM:*)
(*DBG:        return sizeof(INT);*)
(*DBG:    case REAL_ENUM:*)
(*DBG:        return sizeof(REAL);*)
(*DBG:    case WORD_ENUM:*)
(*DBG:        return sizeof(WORD);*)
(*DBG:    default:*)
(*DBG:        return 0;*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void *get_var_addr(size_t idx)*)
(*DBG:{*)
(*DBG:    void *ptr = debug_vars[idx].ptr;*)
(*DBG:*)
(*DBG:    switch (debug_vars[idx].type) {*)
(*DBG:    case BOOL_ENUM:*)
(*DBG:        return (void *)&((__IEC_BOOL_t *) ptr)->value;*)
(*DBG:    case INT_ENUM:*)
(*DBG:        return (void *)&((__IEC_INT_t *) ptr)->value;*)
(*DBG:    case REAL_ENUM:*)
(*DBG:        return (void *)&((__IEC_REAL_t *) ptr)->value;*)
(*DBG:    case WORD_ENUM:*)
(*DBG:        return (void *)&((__IEC_WORD_t *) ptr)->value;*)
(*DBG:    default:*)
(*DBG:        return 0;*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void force_var(size_t idx, bool forced, void *val)*)
(*DBG:{*)
(*DBG:    void *ptr = debug_vars[idx].ptr;*)
(*DBG:*)
(*DBG:    if (forced) {*)
(*DBG:        size_t var_size = get_var_size(idx);*)
(*DBG:        switch (debug_vars[idx].type) {*)
(*DBG:        case BOOL_ENUM: {*)
(*DBG:            memcpy(&((__IEC_BOOL_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_BOOL_t *) ptr)->value = *((BOOL *) val);*)
(*DBG:            ((__IEC_BOOL_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case INT_ENUM: {*)
(*DBG:            memcpy(&((__IEC_INT_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_INT_t *) ptr)->value = *((INT *) val);*)
(*DBG:            ((__IEC_INT_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case REAL_ENUM: {*)
(*DBG:            memcpy(&((__IEC_REAL_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_REAL_t *) ptr)->value = *((REAL *) val);*)
(*DBG:            ((__IEC_REAL_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case WORD_ENUM: {*)
(*DBG:            memcpy(&((__IEC_WORD_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_WORD_t *) ptr)->value = *((WORD *) val);*)
(*DBG:            ((__IEC_WORD_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        default:*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:    } else {*)
(*DBG:        switch (debug_vars[idx].type) {*)
(*DBG:        case BOOL_ENUM:*)
(*DBG:            ((__IEC_BOOL_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        case INT_ENUM:*)
(*DBG:            ((__IEC_INT_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        case REAL_ENUM:*)
(*DBG:            ((__IEC_REAL_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        case WORD_ENUM:*)
(*DBG:            ((__IEC_WORD_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        default:*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void swap_bytes(void *ptr, size_t size) *)
(*DBG:{*)
(*DBG:    uint8_t *bytePtr = (uint8_t *)ptr;*)
(*DBG:    size_t i;*)
(*DBG:    for (i = 0; i < size / 2; ++i) *)
(*DBG:    {*)
(*DBG:        uint8_t temp = bytePtr[i];*)
(*DBG:        bytePtr[i] = bytePtr[size - 1 - i];*)
(*DBG:        bytePtr[size - 1 - i] = temp;*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void trace_reset(void)*)
(*DBG:{*)
(*DBG:    for (size_t i=0; i < VAR_COUNT; i++) *)
(*DBG:    {*)
(*DBG:        force_var(i, false, 0);*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void set_trace(size_t idx, bool forced, void *val)*)
(*DBG:{*)
(*DBG:    if (idx >= 0 && idx < VAR_COUNT) *)
(*DBG:    {*)
(*DBG:        if (endianness == REVERSE_ENDIANNESS)*)
(*DBG:        {*)
(*DBG:            // Aaaaarghhhh... Stupid AVR is Big Endian.*)
(*DBG:            swap_bytes(val, get_var_size(idx));*)
(*DBG:        }*)
(*DBG:*)
(*DBG:        force_var(idx, forced, val);*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void set_endianness(uint8_t value)*)
(*DBG:{*)
(*DBG:    if (value == SAME_ENDIANNESS || value == REVERSE_ENDIANNESS)*)
(*DBG:    {*)
(*DBG:        endianness = value;*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)