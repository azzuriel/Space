//                     NVIDIA FXAA 3.11 by TIMOTHY LOTTES
//
// COPYRIGHT (C) 2010, 2011 NVIDIA CORPORATION. ALL RIGHTS RESERVED.
//
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
// NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR
// CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR
// LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS
// INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR
// INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGES.

#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4       : enable

//=============================================================================
//                           FXAA QUALITY PRESETS
//=============================================================================

// Choose the quality preset.
// This needs to be compiled into the shader as it effects code.

#define FXAA_QUALITY__PRESET 12

// OPTIONS
// -----------------------------------------------------------------------
// 10 to 15 - default medium dither (10=fastest, 15=highest quality)
// 20 to 29 - less dither, more expensive (20=fastest, 29=highest quality)
// 39       - no dither, very expensive 
//
// NOTES
// -----------------------------------------------------------------------
// 12 = slightly faster then FXAA 3.9 and higher edge quality (default)
// 13 = about same speed as FXAA 3.9 and better than 12
// 23 = closest to FXAA 3.9 visually and performance wise
//  ^-- the lowest digit is directly related to performance
// ^--- the highest digit is directly related to style

//=============================================================================
//                  FXAA QUALITY - MEDIUM DITHER PRESETS
//=============================================================================
#if (FXAA_QUALITY__PRESET == 10)
    #define FXAA_QUALITY__PS 3
    #define FXAA_QUALITY__P0 1.5
    #define FXAA_QUALITY__P1 3.0
    #define FXAA_QUALITY__P2 12.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 11)
    #define FXAA_QUALITY__PS 4
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 3.0
    #define FXAA_QUALITY__P3 12.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 12)
    #define FXAA_QUALITY__PS 5
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 4.0
    #define FXAA_QUALITY__P4 12.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 13)
    #define FXAA_QUALITY__PS 6
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 4.0
    #define FXAA_QUALITY__P5 12.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 14)
    #define FXAA_QUALITY__PS 7
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 2.0
    #define FXAA_QUALITY__P5 4.0
    #define FXAA_QUALITY__P6 12.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 15)
    #define FXAA_QUALITY__PS 8
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 2.0
    #define FXAA_QUALITY__P5 2.0
    #define FXAA_QUALITY__P6 4.0
    #define FXAA_QUALITY__P7 12.0
#endif

//=============================================================================
//                  FXAA QUALITY - LOW DITHER PRESETS
//=============================================================================
#if (FXAA_QUALITY__PRESET == 20)
    #define FXAA_QUALITY__PS 3
    #define FXAA_QUALITY__P0 1.5
    #define FXAA_QUALITY__P1 2.0
    #define FXAA_QUALITY__P2 8.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 21)
    #define FXAA_QUALITY__PS 4
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 8.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 22)
    #define FXAA_QUALITY__PS 5
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 8.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 23)
    #define FXAA_QUALITY__PS 6
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 2.0
    #define FXAA_QUALITY__P5 8.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 24)
    #define FXAA_QUALITY__PS 7
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 2.0
    #define FXAA_QUALITY__P5 3.0
    #define FXAA_QUALITY__P6 8.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 25)
    #define FXAA_QUALITY__PS 8
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 2.0
    #define FXAA_QUALITY__P5 2.0
    #define FXAA_QUALITY__P6 4.0
    #define FXAA_QUALITY__P7 8.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 26)
    #define FXAA_QUALITY__PS 9
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 2.0
    #define FXAA_QUALITY__P5 2.0
    #define FXAA_QUALITY__P6 2.0
    #define FXAA_QUALITY__P7 4.0
    #define FXAA_QUALITY__P8 8.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 27)
    #define FXAA_QUALITY__PS 10
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 2.0
    #define FXAA_QUALITY__P5 2.0
    #define FXAA_QUALITY__P6 2.0
    #define FXAA_QUALITY__P7 2.0
    #define FXAA_QUALITY__P8 4.0
    #define FXAA_QUALITY__P9 8.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 28)
    #define FXAA_QUALITY__PS 11
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 2.0
    #define FXAA_QUALITY__P5 2.0
    #define FXAA_QUALITY__P6 2.0
    #define FXAA_QUALITY__P7 2.0
    #define FXAA_QUALITY__P8 2.0
    #define FXAA_QUALITY__P9 4.0
    #define FXAA_QUALITY__P10 8.0
#endif
//-----------------------------------------------------------------------------
#if (FXAA_QUALITY__PRESET == 29)
    #define FXAA_QUALITY__PS 12
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.5
    #define FXAA_QUALITY__P2 2.0
    #define FXAA_QUALITY__P3 2.0
    #define FXAA_QUALITY__P4 2.0
    #define FXAA_QUALITY__P5 2.0
    #define FXAA_QUALITY__P6 2.0
    #define FXAA_QUALITY__P7 2.0
    #define FXAA_QUALITY__P8 2.0
    #define FXAA_QUALITY__P9 2.0
    #define FXAA_QUALITY__P10 4.0
    #define FXAA_QUALITY__P11 8.0
#endif

//=============================================================================
//                      FXAA QUALITY - EXTREME QUALITY
//=============================================================================
#if (FXAA_QUALITY__PRESET == 39)
    #define FXAA_QUALITY__PS 12
    #define FXAA_QUALITY__P0 1.0
    #define FXAA_QUALITY__P1 1.0
    #define FXAA_QUALITY__P2 1.0
    #define FXAA_QUALITY__P3 1.0
    #define FXAA_QUALITY__P4 1.0
    #define FXAA_QUALITY__P5 1.5
    #define FXAA_QUALITY__P6 2.0
    #define FXAA_QUALITY__P7 2.0
    #define FXAA_QUALITY__P8 2.0
    #define FXAA_QUALITY__P9 2.0
    #define FXAA_QUALITY__P10 4.0
    #define FXAA_QUALITY__P11 8.0
#endif



//=============================================================================
//                              SHADER UNIFORMS
//=============================================================================

// Input color texture.
// RGB = color in linear or perceptual color space
// A   = luma in perceptual color space (not linear)
uniform sampler2DRect Tex;

uniform vec2 RcpFrame;      // ( pixelStepX, pixelStepY )
uniform vec3 QualityParams; // ( Subpix, EdgeThreshold, EdgeThresholdMin )

// This used to be the FXAA_QUALITY__SUBPIX define.
// Choose the amount of sub-pixel aliasing removal.
// This can effect sharpness.
//   1.00 - upper limit (softer)
//   0.75 - default amount of filtering
//   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
//   0.25 - almost off
//   0.00 - completely off
#define Subpix       QualityParams.x

// This used to be the FXAA_QUALITY__EDGE_THRESHOLD define.
// The minimum amount of local contrast required to apply algorithm.
//   0.333 - too little (faster)
//   0.250 - low quality
//   0.166 - default
//   0.125 - high quality 
//   0.063 - overkill (slower)
#define EdgeThreshold    QualityParams.y

// This used to be the FXAA_QUALITY__EDGE_THRESHOLD_MIN define.
// Trims the algorithm from processing darks.
//   0.0833 - upper limit (default, the start of visible unfiltered edges)
//   0.0625 - high quality (faster)
//   0.0312 - visible limit (slower)
#define EdgeThresholdMin     QualityParams.z


//=============================================================================
//                              FXAA shader
//=============================================================================

#ifdef _VERTEX_

out vec2 pos;

void main()
{
    gl_Position = ftransform();
    pos = gl_MultiTexCoord0.xy;
}

#else

// Use noperspective interpolation here (turn off perspective interpolation).
in  vec2 pos;

//-----------------------------------------------------------------------------
#define saturate(x) clamp(x, 0.0, 1.0)
//-----------------------------------------------------------------------------

void main()
{
    vec2 posM;
    posM.x = pos.x;
    posM.y = pos.y;

    // alpha channel of Tex must be a pixel luma
    vec4  rgbyM = texture2DRect(Tex, posM);
    float lumaS = texture2DRectOffset(Tex, posM, ivec2( 0, 1)).w;
    float lumaE = texture2DRectOffset(Tex, posM, ivec2( 1, 0)).w;
    float lumaN = texture2DRectOffset(Tex, posM, ivec2( 0,-1)).w;
    float lumaW = texture2DRectOffset(Tex, posM, ivec2(-1, 0)).w;

    float maxSM  = max(lumaS, rgbyM.w);
    float minSM  = min(lumaS, rgbyM.w);
    float maxESM = max(lumaE, maxSM);
    float minESM = min(lumaE, minSM);
    float maxWN  = max(lumaN, lumaW);
    float minWN  = min(lumaN, lumaW);
    float rangeMax = max(maxWN, maxESM);
    float rangeMin = min(minWN, minESM);
    float rangeMaxScaled = rangeMax * EdgeThreshold;
    float range = rangeMax - rangeMin;

    if (range >= max(EdgeThresholdMin, rangeMaxScaled))
    {
        // compute antialiased color

        float lumaNW = texture2DRectOffset(Tex, posM, ivec2(-1,-1)).w;
        float lumaSE = texture2DRectOffset(Tex, posM, ivec2( 1, 1)).w;
        float lumaNE = texture2DRectOffset(Tex, posM, ivec2( 1,-1)).w;
        float lumaSW = texture2DRectOffset(Tex, posM, ivec2(-1, 1)).w;

        float lumaNS = lumaN + lumaS;
        float lumaWE = lumaW + lumaE;
        float subpixRcpRange = 1.0/range;
        float subpixNSWE = lumaNS + lumaWE;
        float edgeHorz1 = (-2.0 * rgbyM.w) + lumaNS;
        float edgeVert1 = (-2.0 * rgbyM.w) + lumaWE;

        float lumaNESE = lumaNE + lumaSE;
        float lumaNWNE = lumaNW + lumaNE;
        float edgeHorz2 = (-2.0 * lumaE) + lumaNESE;
        float edgeVert2 = (-2.0 * lumaN) + lumaNWNE;

        float lumaNWSW = lumaNW + lumaSW;
        float lumaSWSE = lumaSW + lumaSE;
        float edgeHorz4 = (abs(edgeHorz1) * 2.0) + abs(edgeHorz2);
        float edgeVert4 = (abs(edgeVert1) * 2.0) + abs(edgeVert2);
        float edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;
        float edgeVert3 = (-2.0 * lumaS) + lumaSWSE;
        float edgeHorz = abs(edgeHorz3) + edgeHorz4;
        float edgeVert = abs(edgeVert3) + edgeVert4;

        float subpixNWSWNESE = lumaNWSW + lumaNESE;
        float lengthSign = RcpFrame.x;
        bool  horzSpan = edgeHorz >= edgeVert;
        float subpixA = subpixNSWE * 2.0 + subpixNWSWNESE;

        if (!horzSpan) lumaN = lumaW;
        if (!horzSpan) lumaS = lumaE;
        if (horzSpan) lengthSign = RcpFrame.y;
        float subpixB = (subpixA * (1.0/12.0)) - rgbyM.w;

        float gradientN = lumaN - rgbyM.w;
        float gradientS = lumaS - rgbyM.w;
        float lumaNN = lumaN + rgbyM.w;
        float lumaSS = lumaS + rgbyM.w;
        bool pairN = abs(gradientN) >= abs(gradientS);
        float gradient = max(abs(gradientN), abs(gradientS));
        if (pairN) lengthSign = -lengthSign;
        float subpixC = saturate(abs(subpixB) * subpixRcpRange);

        vec2 posB;
        posB.x = posM.x;
        posB.y = posM.y;
        vec2 offNP;
        offNP.x = (!horzSpan) ? 0.0 : RcpFrame.x;
        offNP.y = ( horzSpan) ? 0.0 : RcpFrame.y;
        if (!horzSpan) posB.x += lengthSign * 0.5;
        if ( horzSpan) posB.y += lengthSign * 0.5;

        vec2 posN;
        posN.x = posB.x - offNP.x * FXAA_QUALITY__P0;
        posN.y = posB.y - offNP.y * FXAA_QUALITY__P0;
        vec2 posP;
        posP.x = posB.x + offNP.x * FXAA_QUALITY__P0;
        posP.y = posB.y + offNP.y * FXAA_QUALITY__P0;
        float subpixD = ((-2.0)*subpixC) + 3.0;
        float lumaEndN = texture2DRect(Tex, posN).w;
        float subpixE = subpixC * subpixC;
        float lumaEndP = texture2DRect(Tex, posP).w;

        if (!pairN) lumaNN = lumaSS;
        float gradientScaled = gradient * 1.0/4.0;
        float lumaMM = rgbyM.w - lumaNN * 0.5;
        float subpixF = subpixD * subpixE;
        bool lumaMLTZero = lumaMM < 0.0;

        lumaEndN -= lumaNN * 0.5;
        lumaEndP -= lumaNN * 0.5;
        bool doneN = abs(lumaEndN) >= gradientScaled;
        bool doneP = abs(lumaEndP) >= gradientScaled;
        if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P1;
        if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P1;
        bool doneNP = (!doneN) || (!doneP);
        if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P1;
        if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P1;

        if (doneNP)
        {
            if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
            if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
            if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
            if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
            doneN = abs(lumaEndN) >= gradientScaled;
            doneP = abs(lumaEndP) >= gradientScaled;
            if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P2;
            if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P2;
            doneNP = (!doneN) || (!doneP);
            if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P2;
            if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P2;

            #if (FXAA_QUALITY__PS > 3)
            if (doneNP)
            {
                if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                doneN = abs(lumaEndN) >= gradientScaled;
                doneP = abs(lumaEndP) >= gradientScaled;
                if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P3;
                if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P3;
                doneNP = (!doneN) || (!doneP);
                if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P3;
                if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P3;

                #if (FXAA_QUALITY__PS > 4)
                if (doneNP)
                {
                    if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                    if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                    if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                    if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                    doneN = abs(lumaEndN) >= gradientScaled;
                    doneP = abs(lumaEndP) >= gradientScaled;
                    if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P4;
                    if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P4;
                    doneNP = (!doneN) || (!doneP);
                    if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P4;
                    if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P4;

                    #if (FXAA_QUALITY__PS > 5)
                    if (doneNP)
                    {
                        if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                        if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                        if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                        if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                        doneN = abs(lumaEndN) >= gradientScaled;
                        doneP = abs(lumaEndP) >= gradientScaled;
                        if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P5;
                        if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P5;
                        doneNP = (!doneN) || (!doneP);
                        if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P5;
                        if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P5;

                        #if (FXAA_QUALITY__PS > 6)
                        if (doneNP)
                        {
                            if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                            if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                            if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                            if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                            doneN = abs(lumaEndN) >= gradientScaled;
                            doneP = abs(lumaEndP) >= gradientScaled;
                            if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P6;
                            if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P6;
                            doneNP = (!doneN) || (!doneP);
                            if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P6;
                            if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P6;

                            #if (FXAA_QUALITY__PS > 7)
                            if (doneNP)
                            {
                                if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                                if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                                if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                                if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                                doneN = abs(lumaEndN) >= gradientScaled;
                                doneP = abs(lumaEndP) >= gradientScaled;
                                if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P7;
                                if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P7;
                                doneNP = (!doneN) || (!doneP);
                                if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P7;
                                if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P7;

                                #if (FXAA_QUALITY__PS > 8)
                                if (doneNP)
                                {
                                    if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                                    if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                                    if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                                    if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                                    doneN = abs(lumaEndN) >= gradientScaled;
                                    doneP = abs(lumaEndP) >= gradientScaled;
                                    if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P8;
                                    if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P8;
                                    doneNP = (!doneN) || (!doneP);
                                    if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P8;
                                    if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P8;

                                    #if (FXAA_QUALITY__PS > 9)
                                    if (doneNP)
                                    {
                                        if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                                        if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                                        if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                                        if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                                        doneN = abs(lumaEndN) >= gradientScaled;
                                        doneP = abs(lumaEndP) >= gradientScaled;
                                        if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P9;
                                        if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P9;
                                        doneNP = (!doneN) || (!doneP);
                                        if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P9;
                                        if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P9;

                                        #if (FXAA_QUALITY__PS > 10)
                                        if (doneNP)
                                        {
                                            if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                                            if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                                            if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                                            if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                                            doneN = abs(lumaEndN) >= gradientScaled;
                                            doneP = abs(lumaEndP) >= gradientScaled;
                                            if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P10;
                                            if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P10;
                                            doneNP = (!doneN) || (!doneP);
                                            if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P10;
                                            if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P10;

                                            #if (FXAA_QUALITY__PS > 11)
                                            if (doneNP)
                                            {
                                                if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                                                if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                                                if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                                                if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                                                doneN = abs(lumaEndN) >= gradientScaled;
                                                doneP = abs(lumaEndP) >= gradientScaled;
                                                if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P11;
                                                if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P11;
                                                doneNP = (!doneN) || (!doneP);
                                                if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P11;
                                                if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P11;

                                                #if (FXAA_QUALITY__PS > 12)
                                                if (doneNP)
                                                {
                                                    if (!doneN) lumaEndN = texture2DRect(Tex, posN.xy).w;
                                                    if (!doneP) lumaEndP = texture2DRect(Tex, posP.xy).w;
                                                    if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                                                    if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                                                    doneN = abs(lumaEndN) >= gradientScaled;
                                                    doneP = abs(lumaEndP) >= gradientScaled;
                                                    if (!doneN) posN.x -= offNP.x * FXAA_QUALITY__P12;
                                                    if (!doneN) posN.y -= offNP.y * FXAA_QUALITY__P12;
                                                    doneNP = (!doneN) || (!doneP);
                                                    if (!doneP) posP.x += offNP.x * FXAA_QUALITY__P12;
                                                    if (!doneP) posP.y += offNP.y * FXAA_QUALITY__P12;
                                                }
                                                #endif
                                            }
                                            #endif
                                        }
                                        #endif
                                    }
                                    #endif
                                }
                                #endif
                            }
                            #endif
                        }
                        #endif
                    }
                    #endif
                }
                #endif
            }
            #endif
        }

        float dstN = posM.x - posN.x;
        float dstP = posP.x - posM.x;
        if (!horzSpan) dstN = posM.y - posN.y;
        if (!horzSpan) dstP = posP.y - posM.y;

        bool goodSpanN = (lumaEndN < 0.0) != lumaMLTZero;
        float spanLength = (dstP + dstN);
        bool goodSpanP = (lumaEndP < 0.0) != lumaMLTZero;
        float spanLengthRcp = 1.0/spanLength;

        bool directionN = dstN < dstP;
        float dst = min(dstN, dstP);
        bool goodSpan = directionN ? goodSpanN : goodSpanP;
        float subpixG = subpixF * subpixF;
        float pixelOffset = (dst * (-spanLengthRcp)) + 0.5;
        float subpixH = subpixG * Subpix;

        float pixelOffsetGood = goodSpan ? pixelOffset : 0.0;
        float pixelOffsetSubpix = max(pixelOffsetGood, subpixH);
        if (!horzSpan) posM.x += pixelOffsetSubpix * lengthSign;
        if ( horzSpan) posM.y += pixelOffsetSubpix * lengthSign;

        gl_FragColor = vec4(texture2DRect(Tex, posM).xyz, rgbyM.w);
    }
    else
    {
        // no antialiasing required
        gl_FragColor = rgbyM;
    }
}

//-----------------------------------------------------------------------------

#endif
