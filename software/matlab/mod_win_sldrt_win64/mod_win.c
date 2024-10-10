/*
 * mod_win.c
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
#include <math.h>
#include "rtwtypes.h"
#include <string.h>
#include "rt_nonfinite.h"
#include "mod_win_dt.h"

/* options for Simulink Desktop Real-Time board 0 */
static double SLDRTBoardOptions0[] = {
  1.2E+7,
  8.0,
  0.0,
  0.0,
  0.0,
  0.0,
  0.0,
  0.0,
};

/* list of Simulink Desktop Real-Time timers */
const int SLDRTTimerCount = 1;
const double SLDRTTimers[2] = {
  0.001, 0.0,
};

/* list of Simulink Desktop Real-Time boards */
const int SLDRTBoardCount = 1;
SLDRTBOARD SLDRTBoards[1] = {
  { "Standard_Devices/Serial_Port", 8U, 8, SLDRTBoardOptions0 },
};

/* Block signals (default storage) */
B_mod_win_T mod_win_B;

/* Block states (default storage) */
DW_mod_win_T mod_win_DW;

/* Real-time model */
static RT_MODEL_mod_win_T mod_win_M_;
RT_MODEL_mod_win_T *const mod_win_M = &mod_win_M_;
static void rate_scheduler(void);

/*
 *         This function updates active task flag for each subrate.
 *         The function is called at model base rate, hence the
 *         generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (mod_win_M->Timing.TaskCounters.TID[2])++;
  if ((mod_win_M->Timing.TaskCounters.TID[2]) > 3) {/* Sample time: [0.004s, 0.0s] */
    mod_win_M->Timing.TaskCounters.TID[2] = 0;
  }

  mod_win_M->Timing.sampleHits[2] = (mod_win_M->Timing.TaskCounters.TID[2] == 0)
    ? 1 : 0;
}

/* Model output function */
void mod_win_output(void)
{
  real_T lastSin_tmp;

  /* S-Function (sldrtsi): '<S1>/UART RX' */
  /* S-Function Block: <S1>/UART RX */
  {
    char indata[971U];
    int status;
    const char* terminators = "\n\0";
    memset(indata, 0, sizeof(indata));
    status = RTBIO_DriverIO(0, STREAMINPUT, IOREADWITHRESET, 970U, NULL, (double*)
      indata, terminators);
    mod_win_B.UARTRX_o2 = status & 0x1;/* Data Ready port */
    if (status & 0x1) {
      real32_T var0;
      real32_T var1;
      real32_T var2;
      int nscan = sscanf(indata, "%f %f %f", &var0, &var1, &var2);
      if (nscan > 0)
        mod_win_B.UARTRX_o1[0] = (real_T) var0;
      if (nscan > 1)
        mod_win_B.UARTRX_o1[1] = (real_T) var1;
      if (nscan > 2)
        mod_win_B.UARTRX_o1[2] = (real_T) var2;
    }
  }

  /* S-Function (sldrtsync): '<S1>/RT Synch' */
  /* S-Function Block: <S1>/RT Synch */
  {
    mod_win_B.RTSynch = 0;             /* Missed Ticks value is always zero */
  }

  if (mod_win_M->Timing.TaskCounters.TID[2] == 0) {
    /* Sin: '<Root>/Sine Wave' */
    if (mod_win_DW.systemEnable != 0) {
      lastSin_tmp = mod_win_P.SineWave_Freq * mod_win_M->Timing.t[2];
      mod_win_DW.lastSin = sin(lastSin_tmp);
      mod_win_DW.lastCos = cos(lastSin_tmp);
      mod_win_DW.systemEnable = 0;
    }

    /* Sin: '<Root>/Sine Wave' */
    mod_win_B.SineWave = ((mod_win_DW.lastSin * mod_win_P.SineWave_PCos +
      mod_win_DW.lastCos * mod_win_P.SineWave_PSin) * mod_win_P.SineWave_HCos +
                          (mod_win_DW.lastCos * mod_win_P.SineWave_PCos -
      mod_win_DW.lastSin * mod_win_P.SineWave_PSin) * mod_win_P.SineWave_Hsin) *
      mod_win_P.SineWave_Amp + mod_win_P.SineWave_Bias;
  }

  if (mod_win_M->Timing.TaskCounters.TID[2] == 0) {
    /* S-Function (sldrtso): '<S1>/Stream Output' */
    /* S-Function Block: <S1>/Stream Output */
    {
      int status = RTBIO_DriverIO(0, STREAMOUTPUT, IOSTATUS, 329U, NULL, NULL,
        NULL);
      mod_win_B.StreamOutput = status & 0x1;/* Data Ready port */
    }
  }

  if (mod_win_M->Timing.TaskCounters.TID[2] == 0) {
    /* Saturate: '<S1>/Saturation' */
    if (mod_win_B.SineWave > mod_win_P.Saturation_UpperSat) {
      /* Saturate: '<S1>/Saturation' */
      mod_win_B.Saturation = mod_win_P.Saturation_UpperSat;
    } else if (mod_win_B.SineWave < mod_win_P.Saturation_LowerSat) {
      /* Saturate: '<S1>/Saturation' */
      mod_win_B.Saturation = mod_win_P.Saturation_LowerSat;
    } else {
      /* Saturate: '<S1>/Saturation' */
      mod_win_B.Saturation = mod_win_B.SineWave;
    }

    /* End of Saturate: '<S1>/Saturation' */
  }
}

/* Model update function */
void mod_win_update(void)
{
  real_T HoldSine;
  if (mod_win_M->Timing.TaskCounters.TID[2] == 0) {
    /* Update for Sin: '<Root>/Sine Wave' */
    HoldSine = mod_win_DW.lastSin;
    mod_win_DW.lastSin = mod_win_DW.lastSin * mod_win_P.SineWave_HCos +
      mod_win_DW.lastCos * mod_win_P.SineWave_Hsin;
    mod_win_DW.lastCos = mod_win_DW.lastCos * mod_win_P.SineWave_HCos - HoldSine
      * mod_win_P.SineWave_Hsin;

    /* Update for S-Function (sldrtso): '<S1>/Stream Output' */

    /* S-Function Block: <S1>/Stream Output */
    {
      char_T outstring[329U];
      int n = snprintf(outstring, 329U, "%f\n", (real_T) mod_win_B.Saturation);
      RTBIO_DriverIO(0, STREAMOUTPUT, IOWRITE, n, NULL, (double*) outstring,
                     NULL);
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
  if (!(++mod_win_M->Timing.clockTick0)) {
    ++mod_win_M->Timing.clockTickH0;
  }

  mod_win_M->Timing.t[0] = mod_win_M->Timing.clockTick0 *
    mod_win_M->Timing.stepSize0 + mod_win_M->Timing.clockTickH0 *
    mod_win_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [0.001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++mod_win_M->Timing.clockTick1)) {
      ++mod_win_M->Timing.clockTickH1;
    }

    mod_win_M->Timing.t[1] = mod_win_M->Timing.clockTick1 *
      mod_win_M->Timing.stepSize1 + mod_win_M->Timing.clockTickH1 *
      mod_win_M->Timing.stepSize1 * 4294967296.0;
  }

  if (mod_win_M->Timing.TaskCounters.TID[2] == 0) {
    /* Update absolute timer for sample time: [0.004s, 0.0s] */
    /* The "clockTick2" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick2"
     * and "Timing.stepSize2". Size of "clockTick2" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick2 and the high bits
     * Timing.clockTickH2. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++mod_win_M->Timing.clockTick2)) {
      ++mod_win_M->Timing.clockTickH2;
    }

    mod_win_M->Timing.t[2] = mod_win_M->Timing.clockTick2 *
      mod_win_M->Timing.stepSize2 + mod_win_M->Timing.clockTickH2 *
      mod_win_M->Timing.stepSize2 * 4294967296.0;
  }

  rate_scheduler();
}

/* Model initialize function */
void mod_win_initialize(void)
{
  /* Start for S-Function (sldrtso): '<S1>/Stream Output' */

  /* S-Function Block: <S1>/Stream Output */
  RTBIO_DriverIO(0, STREAMOUTPUT, IOWRITE, sizeof("B\n")-1, NULL, (double*)
                 "B\n", NULL);

  /* Enable for Sin: '<Root>/Sine Wave' */
  mod_win_DW.systemEnable = 1;
}

/* Model terminate function */
void mod_win_terminate(void)
{
  /* Terminate for S-Function (sldrtso): '<S1>/Stream Output' */
  RTBIO_DriverIO(0, STREAMOUTPUT, IOWRITE, sizeof("S\n")-1, NULL, (double*)
                 "S\n", NULL);
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  mod_win_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  mod_win_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  mod_win_initialize();
}

void MdlTerminate(void)
{
  mod_win_terminate();
}

/* Registration function */
RT_MODEL_mod_win_T *mod_win(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)mod_win_M, 0,
                sizeof(RT_MODEL_mod_win_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&mod_win_M->solverInfo, &mod_win_M->Timing.simTimeStep);
    rtsiSetTPtr(&mod_win_M->solverInfo, &rtmGetTPtr(mod_win_M));
    rtsiSetStepSizePtr(&mod_win_M->solverInfo, &mod_win_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&mod_win_M->solverInfo, (&rtmGetErrorStatus(mod_win_M)));
    rtsiSetRTModelPtr(&mod_win_M->solverInfo, mod_win_M);
  }

  rtsiSetSimTimeStep(&mod_win_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&mod_win_M->solverInfo,"FixedStepDiscrete");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = mod_win_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    mdlTsMap[2] = 2;

    /* polyspace +2 MISRA2012:D4.1 [Justified:Low] "mod_win_M points to
       static memory which is guaranteed to be non-NULL" */
    mod_win_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    mod_win_M->Timing.sampleTimes = (&mod_win_M->Timing.sampleTimesArray[0]);
    mod_win_M->Timing.offsetTimes = (&mod_win_M->Timing.offsetTimesArray[0]);

    /* task periods */
    mod_win_M->Timing.sampleTimes[0] = (0.0);
    mod_win_M->Timing.sampleTimes[1] = (0.001);
    mod_win_M->Timing.sampleTimes[2] = (0.004);

    /* task offsets */
    mod_win_M->Timing.offsetTimes[0] = (0.0);
    mod_win_M->Timing.offsetTimes[1] = (0.0);
    mod_win_M->Timing.offsetTimes[2] = (0.0);
  }

  rtmSetTPtr(mod_win_M, &mod_win_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = mod_win_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    mdlSampleHits[2] = 1;
    mod_win_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(mod_win_M, 5.0);
  mod_win_M->Timing.stepSize0 = 0.001;
  mod_win_M->Timing.stepSize1 = 0.001;
  mod_win_M->Timing.stepSize2 = 0.004;

  /* External mode info */
  mod_win_M->Sizes.checksums[0] = (1584481799U);
  mod_win_M->Sizes.checksums[1] = (4055867794U);
  mod_win_M->Sizes.checksums[2] = (1376265793U);
  mod_win_M->Sizes.checksums[3] = (1187357407U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    mod_win_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(mod_win_M->extModeInfo,
      &mod_win_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(mod_win_M->extModeInfo, mod_win_M->Sizes.checksums);
    rteiSetTPtr(mod_win_M->extModeInfo, rtmGetTPtr(mod_win_M));
  }

  mod_win_M->solverInfoPtr = (&mod_win_M->solverInfo);
  mod_win_M->Timing.stepSize = (0.001);
  rtsiSetFixedStepSize(&mod_win_M->solverInfo, 0.001);
  rtsiSetSolverMode(&mod_win_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  mod_win_M->blockIO = ((void *) &mod_win_B);
  (void) memset(((void *) &mod_win_B), 0,
                sizeof(B_mod_win_T));

  /* parameters */
  mod_win_M->defaultParam = ((real_T *)&mod_win_P);

  /* states (dwork) */
  mod_win_M->dwork = ((void *) &mod_win_DW);
  (void) memset((void *)&mod_win_DW, 0,
                sizeof(DW_mod_win_T));

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    mod_win_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 25;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  mod_win_M->Sizes.numContStates = (0);/* Number of continuous states */
  mod_win_M->Sizes.numY = (0);         /* Number of model outputs */
  mod_win_M->Sizes.numU = (0);         /* Number of model inputs */
  mod_win_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  mod_win_M->Sizes.numSampTimes = (3); /* Number of sample times */
  mod_win_M->Sizes.numBlocks = (12);   /* Number of blocks */
  mod_win_M->Sizes.numBlockIO = (6);   /* Number of block outputs */
  mod_win_M->Sizes.numBlockPrms = (15);/* Sum of parameter "widths" */
  return mod_win_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
