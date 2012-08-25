#pragma once


//! Holds the information needed to morph sun into a new state. Last state is the permanent state until next morph.
struct SSunMorphState {
    D3DXVECTOR4 color; // final color
    float fMorphTime; //! speed of morph. Independent to speed of Clock.
    float vspower, pspower, vsmult; // shader vars for sun size/look.

    SSunMorphState(const D3DXVECTOR4& _color, float _fMorphTime, float _vspower, float _pspower, float _vsmult) :
        color(_color), fMorphTime(_fMorphTime), vspower(_vspower), pspower(_pspower), vsmult(_vsmult)    
    {}
    SSunMorphState() {}
};

/*! The Sun's color isn't really the color. It is the arithmetic operation you would like to
        do at the spot that makes up the son. So 120,0,0 adds 120 r at the sun location on sky dome.
        -255,-255,-255, 0 would make the sun black.

        The alpha component makes the sun location see through into the CLEARED buffer (which is white).
        */    
    /*  These variables affect the size and density of the sun. Lower is bigger except for the VSMult. */



// PREDEFINED SUNSTATES WHICH USER CAN USE TO TOGGLE.

namespace SunStates
{
    const SSunMorphState DefaultSun(D3DXVECTOR4(229.f, 130.f, 65.f, 0.f),       1.f, 8.f, 7.f, 0.6f);

    // TODO: Tweak these bitches below.
    const SSunMorphState BlackMorph(D3DXVECTOR4(-255.f,-255.f,-255.f,0.f),      5.f, 7.f, 6.f, 0.5f);
    const SSunMorphState WhiteMorph(D3DXVECTOR4(255.f,255.f,255.f,0.f),         5.f, 7.f, 6.f, 0.5f);
    const SSunMorphState BlueMorph(D3DXVECTOR4(-50.f,-50.f,255.f,0.f),          5.f, 7.f, 6.f, 0.5f);

    const SSunMorphState BlackSkyMorph(D3DXVECTOR4(-255.f,-255.f,-255.f,0.f),   5.f, 1.f, 1.f, 0.5f);
    const SSunMorphState WhiteSkyMorph(D3DXVECTOR4(255.f,255.f,255.f,0.f),      5.f, 1.f, 1.f, 0.5f);
    const SSunMorphState BlueSkyMorph(D3DXVECTOR4(0.f,0.f,255.f,0.f),           5.f, 1.f, 1.f, 0.5f);

    const SSunMorphState BlackEdgeMorph(D3DXVECTOR4(-255.f,-255.f,-255.f,0.f),  1.f, 128.f, 1.f, 0.5f);
    const SSunMorphState WhiteEdgeMorph(D3DXVECTOR4(255.f,255.f,255.f,0.f),     1.f, 128.f, 1.f, 0.5f);
}