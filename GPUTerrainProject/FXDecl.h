#pragma once

//! Used to draw Bounding Boxes
struct BoxVertColor  {
    D3DXVECTOR3 Pos;
    ULONG Color;
}; 

struct BoxVertTex {
    static const UINT FVF = D3DFVF_XYZ | D3DFVF_TEX1;

    D3DXVECTOR3 pos;
    float u,v;

    BoxVertTex() {}
    BoxVertTex(D3DXVECTOR3 vec, float _u, float _v) :
        pos(vec), u(_u), v(_v)
    {} 
    VOID SetTex(float _u, float _v) { u = _u; v = _v; }
};

struct BoxVertTexColor {
    static const UINT FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE;

    D3DXVECTOR3 pos;
    ULONG color;
    float u,v;

    BoxVertTexColor() {}
    BoxVertTexColor(const D3DXVECTOR3 &vec, ULONG _color, float _u, float _v) :
        pos(vec), color(_color), u(_u), v(_v)
    {}
};

//--------------------------------------------------------------------------------------
// Various Vertex Declarations used by Effect Files
//--------------------------------------------------------------------------------------
// This is the vertex format used for the meshes.
struct MESHVERT {
    D3DXVECTOR3 pos; // position
    D3DXVECTOR3 norm;
    float tu, tv; // tex coordinate

    static const UINT FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
    static const D3DVERTEXELEMENT9 Decl[4];

    MESHVERT(const D3DXVECTOR3& _pos, const D3DXVECTOR3& _norm, float u, float v);
};


//--------------------------------------------------------------------------------------
// This is the vertex format used with the quad during post-process.
struct PPVERT
{
    float x, y, z, rhw;
    float tu, tv;       // Texcoord for post-process source
    float tu2, tv2;     // Texcoord for the original scene

    const static D3DVERTEXELEMENT9 Decl[4];
};


struct TERRAINVERT
{
    float x, y, z;      // Position
    float nx, ny, nz;   // Normal    
    float diff0, diff1; // Difference between LOD
    
    const static D3DVERTEXELEMENT9 Decl[4];

	TERRAINVERT(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _d0, float _d1) :
	x(_x), y(_y), z(_z), nx(_nx), ny(_ny), nz(_nz), diff0(_d0), diff1(_d1) {};
};
