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

#include "BIQUAD.h"
#include "BQ_2I_D16F16Css_TRC_WRA_01_Private.h"
#include "LVM_Macros.h"

/**************************************************************************
 ASSUMPTIONS:
 COEFS-
 pBiquadState->coefs[0] is A2, pBiquadState->coefs[1] is A1
 pBiquadState->coefs[2] is A0, pBiquadState->coefs[3] is -B2
 pBiquadState->coefs[4] is -B1, these are in Q15 format

 DELAYS-
 pBiquadState->pDelays[0] is x(n-1)L in Q0 format
 pBiquadState->pDelays[1] is x(n-1)R in Q0 format
 pBiquadState->pDelays[2] is x(n-2)L in Q0 format
 pBiquadState->pDelays[3] is x(n-2)R in Q0 format
 pBiquadState->pDelays[4] is y(n-1)L in Q0 format
 pBiquadState->pDelays[5] is y(n-1)R in Q0 format
 pBiquadState->pDelays[6] is y(n-2)L in Q0 format
 pBiquadState->pDelays[7] is y(n-2)R in Q0 format
***************************************************************************/
void BQ_2I_D16F16C15_TRC_WRA_01(Biquad_FLOAT_Instance_t* pInstance, LVM_FLOAT* pDataIn,
                                LVM_FLOAT* pDataOut, LVM_INT16 NrSamples) {
    LVM_FLOAT ynL, ynR;
    LVM_INT16 ii;
    PFilter_State_FLOAT pBiquadState = (PFilter_State_FLOAT)pInstance;

    for (ii = NrSamples; ii != 0; ii--) {
        /**************************************************************************
                        PROCESSING OF THE LEFT CHANNEL
        ***************************************************************************/
        // ynL=A2  * x(n-2)L
        ynL = (LVM_FLOAT)pBiquadState->coefs[0] * pBiquadState->pDelays[2];

        // ynL+=A1  * x(n-1)L
        ynL += (LVM_FLOAT)pBiquadState->coefs[1] * pBiquadState->pDelays[0];

        // ynL+=A0  * x(n)L
        ynL += (LVM_FLOAT)pBiquadState->coefs[2] * (*pDataIn);

        // ynL+= ( -B2  * y(n-2)L
        ynL += (LVM_FLOAT)pBiquadState->coefs[3] * pBiquadState->pDelays[6];

        // ynL+=( -B1  * y(n-1)L
        ynL += (LVM_FLOAT)pBiquadState->coefs[4] * pBiquadState->pDelays[4];

        /**************************************************************************
                        PROCESSING OF THE RIGHT CHANNEL
        ***************************************************************************/
        // ynR=A2  * x(n-2)R
        ynR = (LVM_FLOAT)pBiquadState->coefs[0] * pBiquadState->pDelays[3];

        // ynR+=A1  * x(n-1)R
        ynR += (LVM_FLOAT)pBiquadState->coefs[1] * pBiquadState->pDelays[1];

        // ynR+=A0  * x(n)R
        ynR += (LVM_FLOAT)pBiquadState->coefs[2] * (*(pDataIn + 1));

        // ynR+= ( -B2  * y(n-2)R  )
        ynR += (LVM_FLOAT)pBiquadState->coefs[3] * pBiquadState->pDelays[7];

        // ynR+=( -B1  * y(n-1)R  )
        ynR += (LVM_FLOAT)pBiquadState->coefs[4] * pBiquadState->pDelays[5];

        /**************************************************************************
                        UPDATING THE DELAYS
        ***************************************************************************/
        pBiquadState->pDelays[7] = pBiquadState->pDelays[5];  // y(n-2)R=y(n-1)R
        pBiquadState->pDelays[6] = pBiquadState->pDelays[4];  // y(n-2)L=y(n-1)L
        pBiquadState->pDelays[3] = pBiquadState->pDelays[1];  // x(n-2)R=x(n-1)R
        pBiquadState->pDelays[2] = pBiquadState->pDelays[0];  // x(n-2)L=x(n-1)L
        pBiquadState->pDelays[5] = ynR;                       // Update y(n-1)R
        pBiquadState->pDelays[4] = ynL;                       // Update y(n-1)L
        pBiquadState->pDelays[0] = (*pDataIn++);              // Update x(n-1)L
        pBiquadState->pDelays[1] = (*pDataIn++);              // Update x(n-1)R

        /**************************************************************************
                        WRITING THE OUTPUT
        ***************************************************************************/
        *pDataOut++ = (LVM_FLOAT)ynL;  // Write Left output
        *pDataOut++ = (LVM_FLOAT)ynR;  // Write Right output
    }
}
