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
    lcs_1_low : WORD;
    lcs_1_high : WORD;
    lcs_2_low : WORD;
    lcs_2_high : WORD;
    lcs_3_low : WORD;
    lcs_3_high : WORD;
    lcs_4_low : WORD;
    lcs_4_high : WORD;
    lcf_1_low : WORD;
    lcf_1_high : WORD;
    lcf_2_low : WORD;
    lcf_2_high : WORD;
    lcf_3_low : WORD;
    lcf_3_high : WORD;
    lcf_4_low : WORD;
    lcf_4_high : WORD;
    lch_high : WORD;
    lch_low : WORD;
    lcm_high : WORD;
    lcm_low : WORD;
    trough_weight_high : WORD;
    trough_weight_low : WORD;
    vacuum_rpm : WORD;
    mixer_rpm : WORD;
    wirecut_cpm : WORD := 0;
    papercut_cpm : WORD;
    conveyor_fpm : REAL;
    conveyor_fpm_high : WORD := 0;
    conveyor_fpm_low : WORD := 0;
    lcs_1 : REAL;
    lcs_2 : REAL;
    lcs_3 : REAL;
    lcs_4 : REAL;
    lcf_1 : REAL;
    lcf_2 : REAL;
    lcf_3 : REAL;
    lcf_4 : REAL;
    lch : REAL := 0.0;
    lcm : REAL := 0.0;
    trough_weight : REAL := 0.0;
    flour_weight : REAL := 0.0;
    flour_weight_high : WORD;
    flour_weight_low : WORD;
    sugar_weight : REAL := 0.0;
    sugar_weight_high : WORD;
    sugar_weight_low : WORD;
    tunnel_temp : REAL;
    tunnel_temp_high : WORD := 0;
    tunnel_temp_low : WORD := 0;
    nitrogen_volume : REAL;
    nitrogen_volume_high : WORD := 0;
    nitrogen_volume_low : WORD := 0;
    rv_1 : BOOL;
    rv_2 : BOOL;
    rv_3 : BOOL;
    dv : BOOL;
    vacuum : BOOL;
    mixer : BOOL;
    flour_alarm : BOOL := false;
    sugar_alarm : BOOL := false;
    hopper_alarm : BOOL := false;
    vacuum_alarm : BOOL := false;
    vacuum_manual : BOOL := false;
    wirecutter : BOOL := false;
    trough_transfer : BOOL := false;
    conveyor : BOOL := true;
    paper_cutter : BOOL := false;
    gv_1 : BOOL := false;
    fan_1 : BOOL := false;
    fan_2 : BOOL := false;
    fan_3 : BOOL := false;
    exhaust_fan : BOOL := false;
    CFM : REAL;
    reset : BOOL := false;
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
    box_maker : BOOL := false;
    taper : BOOL := false;
    labeler : BOOL := false;
    palletizer : BOOL := false;
    palletizer_grabbing : BOOL := false;
    wrapper : BOOL := false;
    wrapper_conveying : BOOL := false;
    conveyor_1 : BOOL := true;
    conveyor_2 : BOOL := true;
    conveyor_3 : BOOL := true;
    conveyor_4 : BOOL := true;
    conveyor_5 : BOOL := true;
    ps_1 : BOOL := false;
    ps_2 : BOOL := false;
    ps_3 : BOOL := false;
    ps_4 : BOOL := false;
    ps_5 : BOOL := false;
    ps_6 : BOOL := false;
    bagger : BOOL := false;
    palletizer_rotation : INT := 180;
    box_count : INT := 0;
    wrapper_wrapping : BOOL := false;
    boxing : BOOL := false;
    taping : BOOL := false;
    conveying_1 : BOOL := false;
    conveying_2 : BOOL := false;
    conveying_3 : BOOL := false;
    conveying_4 : BOOL := false;
    conveying_5 : BOOL := false;
    palletizer_moving : BOOL := false;
    bagging : BOOL := false;
    machine_1 : INT := 0;
    machine_2 : INT := 0;
    machine_3 : INT := 0;
    machine_4 : INT := 0;
  END_VAR
  VAR
    state_0 : INT := 0;
    state_1 : INT := 1;
    state_2 : INT := 2;
    state_3 : INT := 3;
    state_4 : INT := 4;
    state_5 : INT := 5;
    state_6 : INT := 6;
    pos_1 : INT := 225;
    pos_2 : INT := 180;
    pos_3 : INT := 130;
  END_VAR
  VAR
    roe_1 : BOOL := false;
    roe_2 : BOOL := false;
    roe_3 : BOOL := false;
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
    _TMP_MOVE163_ENO : BOOL;
    _TMP_MOVE163_OUT : INT;
    _TMP_EQ171_OUT : BOOL;
    _TMP_MOVE188_ENO : BOOL;
    _TMP_MOVE188_OUT : INT;
    _TMP_EQ197_OUT : BOOL;
    _TMP_MOVE205_ENO : BOOL;
    _TMP_MOVE205_OUT : INT;
    _TMP_EQ219_OUT : BOOL;
    _TMP_MOVE231_ENO : BOOL;
    _TMP_MOVE231_OUT : INT;
    _TMP_EQ243_OUT : BOOL;
    _TMP_MOVE281_ENO : BOOL;
    _TMP_MOVE281_OUT : INT;
    _TMP_MOVE228_ENO : BOOL;
    _TMP_MOVE228_OUT : INT;
    _TMP_EQ264_OUT : BOOL;
    _TMP_MOVE241_ENO : BOOL;
    _TMP_MOVE241_OUT : INT;
    _TMP_ADD248_ENO : BOOL;
    _TMP_ADD248_OUT : INT;
    _TMP_MOVE252_ENO : BOOL;
    _TMP_MOVE252_OUT : INT;
    _TMP_EQ256_OUT : BOOL;
    _TMP_GE263_OUT : BOOL;
    _TMP_AND353_OUT : BOOL;
    _TMP_EQ210_OUT : BOOL;
    _TMP_MOVE224_ENO : BOOL;
    _TMP_MOVE224_OUT : INT;
    _TMP_EQ270_OUT : BOOL;
    _TMP_MOVE290_ENO : BOOL;
    _TMP_MOVE290_OUT : INT;
    _TMP_MOVE310_ENO : BOOL;
    _TMP_MOVE310_OUT : INT;
    _TMP_EQ314_OUT : BOOL;
    _TMP_MOVE321_ENO : BOOL;
    _TMP_MOVE321_OUT : INT;
    _TMP_EQ145_OUT : BOOL;
    _TMP_MOVE336_ENO : BOOL;
    _TMP_MOVE336_OUT : INT;
    _TMP_EQ347_OUT : BOOL;
    _TMP_MOVE345_ENO : BOOL;
    _TMP_MOVE345_OUT : INT;
    _TMP_MOVE238_ENO : BOOL;
    _TMP_MOVE238_OUT : INT;
    _TMP_EQ204_OUT : BOOL;
    _TMP_EQ232_OUT : BOOL;
    _TMP_MOVE229_ENO : BOOL;
    _TMP_MOVE229_OUT : INT;
    _TMP_MOVE262_ENO : BOOL;
    _TMP_MOVE262_OUT : INT;
    _TMP_EQ341_OUT : BOOL;
    _TMP_MOVE303_ENO : BOOL;
    _TMP_MOVE303_OUT : INT;
    _TMP_MOVE362_ENO : BOOL;
    _TMP_MOVE362_OUT : INT;
    _TMP_MOVE186_ENO : BOOL;
    _TMP_MOVE186_OUT : INT;
    _TMP_MOVE273_ENO : BOOL;
    _TMP_MOVE273_OUT : INT;
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
  _TMP_MOVE163_OUT := MOVE(EN := ps_3 AND conveyor_3 AND _TMP_EQ154_OUT, IN := state_1, ENO => _TMP_MOVE163_ENO);
  IF _TMP_MOVE163_ENO THEN
      machine_2 := _TMP_MOVE163_OUT;
  END_IF;
  _TMP_EQ171_OUT := EQ(machine_2, state_1);
  IF taper AND ps_4 AND _TMP_EQ171_OUT THEN
    taping := TRUE; (*set*)
  END_IF;
  _TMP_MOVE188_OUT := MOVE(EN := taper AND ps_4 AND _TMP_EQ171_OUT, IN := state_2, ENO => _TMP_MOVE188_ENO);
  IF _TMP_MOVE188_ENO THEN
      machine_2 := _TMP_MOVE188_OUT;
  END_IF;
  _TMP_EQ197_OUT := EQ(machine_2, state_2);
  IF conveyor_4 AND NOT(taping) AND NOT(ps_5) AND _TMP_EQ197_OUT THEN
    conveying_4 := TRUE; (*set*)
  END_IF;
  _TMP_MOVE205_OUT := MOVE(EN := conveyor_4 AND NOT(taping) AND NOT(ps_5) AND _TMP_EQ197_OUT, IN := state_0, ENO => _TMP_MOVE205_ENO);
  IF _TMP_MOVE205_ENO THEN
      machine_2 := _TMP_MOVE205_OUT;
  END_IF;
  _TMP_EQ219_OUT := EQ(machine_3, state_2);
  _TMP_MOVE231_OUT := MOVE(EN := palletizer AND _TMP_EQ219_OUT, IN := state_3, ENO => _TMP_MOVE231_ENO);
  IF _TMP_MOVE231_ENO THEN
      machine_3 := _TMP_MOVE231_OUT;
  END_IF;
  IF palletizer AND _TMP_EQ219_OUT THEN
    palletizer_moving := TRUE; (*set*)
  END_IF;
  _TMP_EQ243_OUT := EQ(machine_3, state_4);
  IF NOT(wrapper_wrapping) AND NOT(wrapper_conveying) AND palletizer AND _TMP_EQ243_OUT THEN
    palletizer_moving := TRUE; (*set*)
  END_IF;
  _TMP_MOVE281_OUT := MOVE(EN := NOT(wrapper_wrapping) AND NOT(wrapper_conveying) AND palletizer AND _TMP_EQ243_OUT, IN := state_5, ENO => _TMP_MOVE281_ENO);
  IF _TMP_MOVE281_ENO THEN
      machine_3 := _TMP_MOVE281_OUT;
  END_IF;
  _TMP_MOVE228_OUT := MOVE(EN := palletizer AND _TMP_EQ219_OUT, IN := pos_2, ENO => _TMP_MOVE228_ENO);
  IF _TMP_MOVE228_ENO THEN
      palletizer_rotation := _TMP_MOVE228_OUT;
  END_IF;
  _TMP_EQ264_OUT := EQ(machine_3, state_6);
  _TMP_MOVE241_OUT := MOVE(EN := _TMP_EQ264_OUT, IN := pos_2, ENO => _TMP_MOVE241_ENO);
  IF _TMP_MOVE241_ENO THEN
      palletizer_rotation := _TMP_MOVE241_OUT;
  END_IF;
  _TMP_ADD248_OUT := ADD(EN := palletizer AND _TMP_EQ264_OUT, IN1 := box_count, IN2 := 1, ENO => _TMP_ADD248_ENO);
  IF _TMP_ADD248_ENO THEN
      box_count := _TMP_ADD248_OUT;
  END_IF;
  _TMP_MOVE252_OUT := MOVE(EN := palletizer AND _TMP_EQ264_OUT, IN := state_3, ENO => _TMP_MOVE252_ENO);
  IF _TMP_MOVE252_ENO THEN
      machine_3 := _TMP_MOVE252_OUT;
  END_IF;
  _TMP_EQ256_OUT := EQ(machine_4, state_0);
  _TMP_GE263_OUT := GE(box_count, 40);
  _TMP_AND353_OUT := AND(_TMP_EQ256_OUT, _TMP_GE263_OUT);
  IF NOT(ps_6) AND wrapper AND _TMP_AND353_OUT THEN
    wrapper_wrapping := TRUE; (*set*)
  END_IF;
  _TMP_EQ210_OUT := EQ(machine_3, state_0);
  _TMP_MOVE224_OUT := MOVE(EN := palletizer AND NOT(palletizer_grabbing) AND ps_5 AND _TMP_EQ210_OUT, IN := state_1, ENO => _TMP_MOVE224_ENO);
  IF _TMP_MOVE224_ENO THEN
      machine_3 := _TMP_MOVE224_OUT;
  END_IF;
  _TMP_EQ270_OUT := EQ(machine_4, state_1);
  IF NOT(wrapper_wrapping) AND wrapper AND _TMP_EQ270_OUT THEN
    wrapper_conveying := TRUE; (*set*)
  END_IF;
  _TMP_MOVE290_OUT := MOVE(EN := NOT(ps_6) AND wrapper AND _TMP_AND353_OUT, IN := state_1, ENO => _TMP_MOVE290_ENO);
  IF _TMP_MOVE290_ENO THEN
      machine_4 := _TMP_MOVE290_OUT;
  END_IF;
  _TMP_MOVE310_OUT := MOVE(EN := NOT(wrapper_wrapping) AND wrapper AND _TMP_EQ270_OUT, IN := state_2, ENO => _TMP_MOVE310_ENO);
  IF _TMP_MOVE310_ENO THEN
      machine_4 := _TMP_MOVE310_OUT;
  END_IF;
  _TMP_EQ314_OUT := EQ(machine_4, state_2);
  IF NOT(wrapper_conveying) AND conveyor_5 AND _TMP_EQ314_OUT THEN
    conveying_5 := TRUE; (*set*)
  END_IF;
  _TMP_MOVE321_OUT := MOVE(EN := NOT(wrapper_conveying) AND conveyor_5 AND _TMP_EQ314_OUT, IN := state_3, ENO => _TMP_MOVE321_ENO);
  IF _TMP_MOVE321_ENO THEN
      machine_4 := _TMP_MOVE321_OUT;
  END_IF;
  IF palletizer AND _TMP_EQ264_OUT THEN
    palletizer_grabbing := FALSE; (*reset*)
  END_IF;
  IF palletizer AND _TMP_EQ264_OUT THEN
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
  IF ps_4 AND _TMP_EQ171_OUT THEN
    conveying_3 := FALSE; (*reset*)
  END_IF;
  IF ps_5 AND _TMP_EQ210_OUT THEN
    conveying_4 := FALSE; (*reset*)
  END_IF;
  _TMP_EQ347_OUT := EQ(machine_4, state_3);
  _TMP_MOVE345_OUT := MOVE(EN := ps_6 AND _TMP_EQ347_OUT, IN := state_0, ENO => _TMP_MOVE345_ENO);
  IF _TMP_MOVE345_ENO THEN
      machine_4 := _TMP_MOVE345_OUT;
  END_IF;
  IF ps_6 AND _TMP_EQ347_OUT THEN
    conveying_5 := FALSE; (*reset*)
  END_IF;
  IF palletizer AND NOT(palletizer_grabbing) AND ps_5 AND _TMP_EQ210_OUT THEN
    palletizer_moving := TRUE; (*set*)
  END_IF;
  _TMP_MOVE238_OUT := MOVE(EN := palletizer AND NOT(palletizer_grabbing) AND ps_5 AND _TMP_EQ210_OUT, IN := pos_1, ENO => _TMP_MOVE238_ENO);
  IF _TMP_MOVE238_ENO THEN
      palletizer_rotation := _TMP_MOVE238_OUT;
  END_IF;
  _TMP_EQ204_OUT := EQ(machine_3, state_1);
  IF roe_1 AND _TMP_EQ204_OUT THEN
    palletizer_moving := FALSE; (*reset*)
  END_IF;
  _TMP_EQ232_OUT := EQ(machine_3, state_3);
  _TMP_MOVE229_OUT := MOVE(EN := palletizer_grabbing AND roe_2 AND _TMP_EQ232_OUT, IN := state_4, ENO => _TMP_MOVE229_ENO);
  IF _TMP_MOVE229_ENO THEN
      machine_3 := _TMP_MOVE229_OUT;
  END_IF;
  IF roe_2 AND _TMP_EQ232_OUT THEN
    palletizer_moving := FALSE; (*reset*)
  END_IF;
  _TMP_MOVE262_OUT := MOVE(EN := NOT(wrapper_wrapping) AND NOT(wrapper_conveying) AND palletizer AND _TMP_EQ243_OUT, IN := pos_3, ENO => _TMP_MOVE262_ENO);
  IF _TMP_MOVE262_ENO THEN
      palletizer_rotation := _TMP_MOVE262_OUT;
  END_IF;
  _TMP_EQ341_OUT := EQ(machine_3, state_5);
  _TMP_MOVE303_OUT := MOVE(EN := roe_3 AND _TMP_EQ341_OUT, IN := state_6, ENO => _TMP_MOVE303_ENO);
  IF _TMP_MOVE303_ENO THEN
      machine_3 := _TMP_MOVE303_OUT;
  END_IF;
  IF roe_3 AND _TMP_EQ341_OUT THEN
    palletizer_moving := FALSE; (*reset*)
  END_IF;
  _TMP_MOVE362_OUT := MOVE(EN := NOT(palletizer_grabbing) AND roe_2 AND _TMP_EQ232_OUT, IN := state_0, ENO => _TMP_MOVE362_ENO);
  IF _TMP_MOVE362_ENO THEN
      machine_3 := _TMP_MOVE362_OUT;
  END_IF;
  _TMP_MOVE186_OUT := MOVE(EN := roe_1 AND _TMP_EQ204_OUT, IN := state_2, ENO => _TMP_MOVE186_ENO);
  IF _TMP_MOVE186_ENO THEN
      machine_3 := _TMP_MOVE186_OUT;
  END_IF;
  IF palletizer AND NOT(palletizer_grabbing) AND ps_5 AND _TMP_EQ210_OUT THEN
    palletizer_grabbing := TRUE; (*set*)
  END_IF;
  _TMP_MOVE273_OUT := MOVE(EN := NOT(ps_6) AND wrapper AND _TMP_AND353_OUT, IN := 0, ENO => _TMP_MOVE273_ENO);
  IF _TMP_MOVE273_ENO THEN
      box_count := _TMP_MOVE273_OUT;
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
