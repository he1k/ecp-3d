/*
 * mod_win_data.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "mod_win".
 *
 * Model version              : 1.72
 * Simulink Coder version : 23.2 (R2023b) 01-Aug-2023
 * C source code generated on : Thu Oct 10 12:38:10 2024
 *
 * Target selection: sldrt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "mod_win.h"

/* Block parameters (default storage) */
P_mod_win_T mod_win_P = {
  /* Mask Parameter: UARTRX_MaxMissedTicks
   * Referenced by: '<S1>/UART RX'
   */
  10000.0,

  /* Mask Parameter: RTSynch_MaxMissedTicks
   * Referenced by: '<S1>/RT Synch'
   */
  10000.0,

  /* Mask Parameter: StreamOutput_MaxMissedTicks
   * Referenced by: '<S1>/Stream Output'
   */
  1000.0,

  /* Mask Parameter: UARTRX_YieldWhenWaiting
   * Referenced by: '<S1>/UART RX'
   */
  0.0,

  /* Mask Parameter: RTSynch_YieldWhenWaiting
   * Referenced by: '<S1>/RT Synch'
   */
  0.0,

  /* Mask Parameter: StreamOutput_YieldWhenWaiting
   * Referenced by: '<S1>/Stream Output'
   */
  0.0,

  /* Expression: 0.1
   * Referenced by: '<Root>/Sine Wave'
   */
  0.1,

  /* Expression: 0
   * Referenced by: '<Root>/Sine Wave'
   */
  0.0,

  /* Expression: 0.25*2*pi
   * Referenced by: '<Root>/Sine Wave'
   */
  1.5707963267948966,

  /* Computed Parameter: SineWave_Hsin
   * Referenced by: '<Root>/Sine Wave'
   */
  0.0062831439655589511,

  /* Computed Parameter: SineWave_HCos
   * Referenced by: '<Root>/Sine Wave'
   */
  0.99998026085613712,

  /* Computed Parameter: SineWave_PSin
   * Referenced by: '<Root>/Sine Wave'
   */
  -0.0062831439655589511,

  /* Computed Parameter: SineWave_PCos
   * Referenced by: '<Root>/Sine Wave'
   */
  0.99998026085613712,

  /* Expression: 2.0
   * Referenced by: '<S1>/Saturation'
   */
  2.0,

  /* Expression: -2.0
   * Referenced by: '<S1>/Saturation'
   */
  -2.0
};
