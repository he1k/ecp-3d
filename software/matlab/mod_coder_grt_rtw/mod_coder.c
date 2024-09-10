/*
 * mod_coder.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "mod_coder".
 *
 * Model version              : 1.2
 * Simulink Coder version : 23.2 (R2023b) 01-Aug-2023
 * C source code generated on : Tue Sep 10 15:04:07 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objective: Debugging
 * Validation result: Not run
 */

#include "mod_coder.h"
#include <math.h>
#include "mod_coder_private.h"
#include "rt_nonfinite.h"

/* Block signals (default storage) */
B_mod_coder_T mod_coder_B;

/* Real-time model */
static RT_MODEL_mod_coder_T mod_coder_M_;
RT_MODEL_mod_coder_T *const mod_coder_M = &mod_coder_M_;

/* Model step function */
void mod_coder_step(void)
{
  /* Sin: '<Root>/Sine Wave' */
  mod_coder_B.SineWave = sin(mod_coder_P.SineWave_Freq * mod_coder_M->Timing.t[0]
    + mod_coder_P.SineWave_Phase) * mod_coder_P.SineWave_Amp +
    mod_coder_P.SineWave_Bias;

  /* Sum: '<Root>/Sum' incorporates:
   *  Constant: '<Root>/Constant'
   */
  mod_coder_B.Sum = mod_coder_B.SineWave + mod_coder_P.Constant_Value;

  /* Matfile logging */
  rt_UpdateTXYLogVars(mod_coder_M->rtwLogInfo, (mod_coder_M->Timing.t));

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.0s, 0.0s] */
    if ((rtmGetTFinal(mod_coder_M)!=-1) &&
        !((rtmGetTFinal(mod_coder_M)-mod_coder_M->Timing.t[0]) >
          mod_coder_M->Timing.t[0] * (DBL_EPSILON))) {
      rtmSetErrorStatus(mod_coder_M, "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++mod_coder_M->Timing.clockTick0)) {
    ++mod_coder_M->Timing.clockTickH0;
  }

  mod_coder_M->Timing.t[0] = mod_coder_M->Timing.clockTick0 *
    mod_coder_M->Timing.stepSize0 + mod_coder_M->Timing.clockTickH0 *
    mod_coder_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [0.001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.001, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    mod_coder_M->Timing.clockTick1++;
    if (!mod_coder_M->Timing.clockTick1) {
      mod_coder_M->Timing.clockTickH1++;
    }
  }
}

/* Model initialize function */
void mod_coder_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)mod_coder_M, 0,
                sizeof(RT_MODEL_mod_coder_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&mod_coder_M->solverInfo,
                          &mod_coder_M->Timing.simTimeStep);
    rtsiSetTPtr(&mod_coder_M->solverInfo, &rtmGetTPtr(mod_coder_M));
    rtsiSetStepSizePtr(&mod_coder_M->solverInfo, &mod_coder_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&mod_coder_M->solverInfo, (&rtmGetErrorStatus
      (mod_coder_M)));
    rtsiSetRTModelPtr(&mod_coder_M->solverInfo, mod_coder_M);
  }

  rtsiSetSimTimeStep(&mod_coder_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&mod_coder_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(mod_coder_M, &mod_coder_M->Timing.tArray[0]);
  rtmSetTFinal(mod_coder_M, 10.0);
  mod_coder_M->Timing.stepSize0 = 0.001;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    mod_coder_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(mod_coder_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(mod_coder_M->rtwLogInfo, (NULL));
    rtliSetLogT(mod_coder_M->rtwLogInfo, "tout");
    rtliSetLogX(mod_coder_M->rtwLogInfo, "");
    rtliSetLogXFinal(mod_coder_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(mod_coder_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(mod_coder_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(mod_coder_M->rtwLogInfo, 0);
    rtliSetLogDecimation(mod_coder_M->rtwLogInfo, 1);
    rtliSetLogY(mod_coder_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(mod_coder_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(mod_coder_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &mod_coder_B), 0,
                sizeof(B_mod_coder_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(mod_coder_M->rtwLogInfo, 0.0, rtmGetTFinal
    (mod_coder_M), mod_coder_M->Timing.stepSize0, (&rtmGetErrorStatus
    (mod_coder_M)));
}

/* Model terminate function */
void mod_coder_terminate(void)
{
  /* (no terminate code required) */
}
