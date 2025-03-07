/*
 * Copyright (C) 2004-2010 NXP Software
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LVC_MIXER_H__
#define __LVC_MIXER_H__

#include "LVM_Types.h"

/**********************************************************************************
   INSTANCE MEMORY TYPE DEFINITION
***********************************************************************************/

/* LVMixer3_st structure stores Instance parameters for one audio stream */
typedef struct {
    LVM_FLOAT PrivateParams[3]; /* Private Instance params for \
                                   Audio Stream shift parameter */
    LVM_INT16 CallbackSet;      /* Boolean.  Should be set by calling application \
                                   each time the target value is updated */
    LVM_INT16 CallbackParam;    /* Parameter that will be used in the calback function */
    void* pCallbackHandle;      /* Pointer to the instance of the callback function */
    void* pGeneralPurpose;      /* Pointer for general purpose usage */
    LVM_Callback pCallBack;     /* Pointer to the callback function */
} LVMixer3_FLOAT_st;
typedef struct {
    LVMixer3_FLOAT_st MixerStream[1]; /* Instance Params for one Audio Stream */
} LVMixer3_1St_FLOAT_st;
typedef struct {
    LVMixer3_FLOAT_st MixerStream[2]; /* Instance Params for two Audio Streams */
} LVMixer3_2St_FLOAT_st;
/**********************************************************************************
   FUNCTION PROTOTYPES (HIGH LEVEL FUNCTIONS)
***********************************************************************************/

/* Function names should be unique within first 16 characters  */
#define LVMixer3_MixSoft_1St_D16C31_SAT LVMixer3_1St_D16C31_SAT_MixSoft
#define LVMixer3_MixInSoft_D16C31_SAT LVMixer3_D16C31_SAT_MixInSoft
#define LVMixer3_MixSoft_2St_D16C31_SAT LVMixer3_2St_D16C31_SAT_MixSoft
#define LVMixer3_MixSoft_3St_D16C31_SAT LVMixer3_3St_D16C31_SAT_MixSoft

/*** General functions ************************************************************/

/**********************************************************************************/
/* This time constant calculation function assumes the mixer will be called with  */
/* large block sizes. When the block size is small, especially if less than 4,    */
/* then the calculation will give an incorrect value for alpha, see the mixer     */
/* documentation for further details.                                             */
/* ********************************************************************************/
void LVC_Mixer_SetTarget(LVMixer3_FLOAT_st* pStream, LVM_FLOAT TargetGain);
LVM_FLOAT LVC_Mixer_GetTarget(LVMixer3_FLOAT_st* pStream);

LVM_FLOAT LVC_Mixer_GetCurrent(LVMixer3_FLOAT_st* pStream);

void LVC_Mixer_Init(LVMixer3_FLOAT_st* pStream, LVM_FLOAT TargetGain, LVM_FLOAT CurrentGain);

void LVC_Mixer_SetTimeConstant(LVMixer3_FLOAT_st* pStream, LVM_INT32 Tc_millisec, LVM_Fs_en Fs,
                               LVM_INT16 NumChannels);

void LVC_Mixer_VarSlope_SetTimeConstant(LVMixer3_FLOAT_st* pStream, LVM_INT32 Tc_millisec,
                                        LVM_Fs_en Fs, LVM_INT16 NumChannels);

/*** 16 bit functions *************************************************************/

void LVC_MixSoft_1St_D16C31_SAT(LVMixer3_1St_FLOAT_st* pInstance, const LVM_FLOAT* src,
                                LVM_FLOAT* dst, LVM_INT16 n);
void LVC_MixSoft_Mc_D16C31_SAT(LVMixer3_1St_FLOAT_st* pInstance, const LVM_FLOAT* src,
                               LVM_FLOAT* dst, LVM_INT16 NrFrames, LVM_INT16 NrChannels);

void LVC_MixInSoft_D16C31_SAT(LVMixer3_1St_FLOAT_st* pInstance, const LVM_FLOAT* src,
                              LVM_FLOAT* dst, LVM_INT16 n);
void LVC_MixInSoft_Mc_D16C31_SAT(LVMixer3_1St_FLOAT_st* pInstance, const LVM_FLOAT* src,
                                 LVM_FLOAT* dst, LVM_INT16 NrFrames, LVM_INT16 NrChannels);

void LVC_MixSoft_2St_D16C31_SAT(LVMixer3_2St_FLOAT_st* pInstance, const LVM_FLOAT* src1,
                                const LVM_FLOAT* src2,
                                LVM_FLOAT* dst, /* dst cannot be equal to src2 */
                                LVM_INT16 n);
void LVC_MixSoft_2Mc_D16C31_SAT(LVMixer3_2St_FLOAT_st* pInstance, const LVM_FLOAT* src1,
                                const LVM_FLOAT* src2,
                                LVM_FLOAT* dst, /* dst cannot be equal to src2 */
                                LVM_INT16 NrFrames, LVM_INT16 NrChannels);
/**********************************************************************************/
/* For applying different gains to Left and right chennals                        */
/* MixerStream[0] applies to Left channel                                         */
/* MixerStream[1] applies to Right channel                                        */
/* Gain values should not be more that 1.0                                        */
/**********************************************************************************/
void LVC_MixSoft_1St_MC_float_SAT(LVMixer3_2St_FLOAT_st* pInstance, const LVM_FLOAT* src,
                                  LVM_FLOAT* dst, /* dst can be equal to src */
                                  LVM_INT16 NrFrames, LVM_INT32 NrChannels, LVM_INT32 ChMask);
void LVC_MixSoft_1St_2i_D16C31_SAT(LVMixer3_2St_FLOAT_st* pInstance, const LVM_FLOAT* src,
                                   LVM_FLOAT* dst, /* dst can be equal to src */
                                   LVM_INT16 n);   /* Number of stereo samples */

/**********************************************************************************/

#endif  //#ifndef __LVC_MIXER_H__
