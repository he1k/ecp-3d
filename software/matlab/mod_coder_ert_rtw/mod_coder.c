/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: mod_coder.c
 *
 * Code generated for Simulink model 'mod_coder'.
 *
 * Model version                  : 1.3
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Tue Sep 10 15:29:32 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "mod_coder.h"
#include "mod_coder_private.h"

/* Real-time model */
static RT_MODEL_mod_coder_T mod_coder_M_;
RT_MODEL_mod_coder_T *const mod_coder_M = &mod_coder_M_;

/* Model step function */
void mod_coder_step(void)
{
  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  mod_coder_M->Timing.t[0] =
    ((time_T)(++mod_coder_M->Timing.clockTick0)) * mod_coder_M->Timing.stepSize0;

  {
    /* Update absolute timer for sample time: [0.001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.001, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     */
    mod_coder_M->Timing.clockTick1++;
  }
}

/* Model initialize function */
void mod_coder_initialize(void)
{
  /* Registration code */
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
  mod_coder_M->Timing.stepSize0 = 0.001;
}

/* Model terminate function */
void mod_coder_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
