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
    flour_weight_high AT %QW117 : UINT;
    flour_weight_low AT %QW116 : UINT;
    sugar_weight AT %MD10 : REAL := 0.0;
    sugar_weight_high AT %QW219 : UINT;
    sugar_weight_low AT %QW218 : UINT;
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
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : tag_display;
  END_RESOURCE
END_CONFIGURATION

(*DBG:char md5[] = "7aeb466d72b6af75c532671406db63bb";*)
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
(*DBG:    {&(RES0__INSTANCE0.VACUUM_FREQ), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.VACUUM_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_FREQ), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.MIXER_RPM), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_1), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_2), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_3), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCS_4), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_1), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_2), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_3), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCF_4), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.LCH), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_WEIGHT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_WEIGHT_HIGH), UINT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FLOUR_WEIGHT_LOW), UINT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_WEIGHT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_WEIGHT_HIGH), UINT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SUGAR_WEIGHT_LOW), UINT_ENUM},*)
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
(*DBG:    {&(RES0__INSTANCE0.CFM), REAL_ENUM},*)
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
(*DBG:    {&(RES0__INSTANCE0.FREQ_FUNCTION0.EN), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FREQ_FUNCTION0.ENO), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FREQ_FUNCTION0.INPUT_FREQ), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FREQ_FUNCTION0.OUTPUT_FREQ), WORD_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FREQ_FUNCTION0.MANUAL), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.FREQ_FUNCTION0.DV), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD37_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD38_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD41_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD39_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD40_OUT), REAL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0._TMP_ADD43_OUT), REAL_ENUM},*)
(*DBG:};*)
(*DBG:*)
(*DBG:#define VAR_COUNT               124*)
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
(*DBG:    case UINT_ENUM:*)
(*DBG:        return sizeof(UINT);*)
(*DBG:    case BOOL_ENUM:*)
(*DBG:        return sizeof(BOOL);*)
(*DBG:    case WORD_ENUM:*)
(*DBG:        return sizeof(WORD);*)
(*DBG:    case REAL_ENUM:*)
(*DBG:        return sizeof(REAL);*)
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
(*DBG:    case UINT_ENUM:*)
(*DBG:        return (void *)&((__IEC_UINT_t *) ptr)->value;*)
(*DBG:    case BOOL_ENUM:*)
(*DBG:        return (void *)&((__IEC_BOOL_t *) ptr)->value;*)
(*DBG:    case WORD_ENUM:*)
(*DBG:        return (void *)&((__IEC_WORD_t *) ptr)->value;*)
(*DBG:    case REAL_ENUM:*)
(*DBG:        return (void *)&((__IEC_REAL_t *) ptr)->value;*)
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
(*DBG:        case UINT_ENUM: {*)
(*DBG:            memcpy(&((__IEC_UINT_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_UINT_t *) ptr)->value = *((UINT *) val);*)
(*DBG:            ((__IEC_UINT_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case BOOL_ENUM: {*)
(*DBG:            memcpy(&((__IEC_BOOL_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_BOOL_t *) ptr)->value = *((BOOL *) val);*)
(*DBG:            ((__IEC_BOOL_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case WORD_ENUM: {*)
(*DBG:            memcpy(&((__IEC_WORD_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_WORD_t *) ptr)->value = *((WORD *) val);*)
(*DBG:            ((__IEC_WORD_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case REAL_ENUM: {*)
(*DBG:            memcpy(&((__IEC_REAL_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_REAL_t *) ptr)->value = *((REAL *) val);*)
(*DBG:            ((__IEC_REAL_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        default:*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:    } else {*)
(*DBG:        switch (debug_vars[idx].type) {*)
(*DBG:        case UINT_ENUM:*)
(*DBG:            ((__IEC_UINT_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        case BOOL_ENUM:*)
(*DBG:            ((__IEC_BOOL_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        case WORD_ENUM:*)
(*DBG:            ((__IEC_WORD_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        case REAL_ENUM:*)
(*DBG:            ((__IEC_REAL_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
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