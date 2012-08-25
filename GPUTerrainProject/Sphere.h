#pragma once

using std::vector;

class CSphere
{
    //
    // SPHERE OF RADIUS 1.
    //
private:
    int m_nVertices, m_nIndices;
    int m_nRings, m_nSegments;
    int m_nSphereTriangles;

    float m_fHorizontalTile, m_fVerticalTile;

    LPDIRECT3DVERTEXBUFFER9         m_pVB;
    LPDIRECT3DINDEXBUFFER9          m_pIB;
    LPDIRECT3DVERTEXDECLARATION9    m_pSphereTanVertDecl; // Vertex declartion if needing tangent vector.    

    bool m_bNormalsPointIn;
    bool m_bUseTangents;

private:
    struct SphereVert {
        D3DXVECTOR3 pos; // position
        D3DXVECTOR3 norm;
        float tu, tv; // tex coordinate

        static const UINT FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
        static const D3DVERTEXELEMENT9 Decl[4];

        SphereVert() {}
        SphereVert(const D3DXVECTOR3& _pos, const D3DXVECTOR3& _norm, float u, float v);
    };
    struct SphereTanBinVert {
        D3DXVECTOR3 pos; // position
        D3DXVECTOR3 norm;        
        float tu, tv; // tex coordinate
        D3DXVECTOR3 tan;
        D3DXVECTOR3 binormal;

        // static const UINT FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
        static const D3DVERTEXELEMENT9 Decl[6];

        SphereTanBinVert() {}
        SphereTanBinVert(const D3DXVECTOR3& _pos, const D3DXVECTOR3& _norm, const D3DXVECTOR3& _tan, const D3DXVECTOR3& _bin, float u, float v);
    };

private:
    //! Creates the triangle strip/fan of the bubble
    HRESULT CreateSphere(IDirect3DDevice9* pd3dDevice, 
                            int nRings, int nSegments, 
                            float fHorTile, float fVerTile, 
                            bool bNormalsPointIn);

    //! Creates the triangle strip/fan of the bubble
    HRESULT CreateSphereWithTangents(IDirect3DDevice9* pd3dDevice, 
                            int nRings, int nSegments, 
                            float fHorTile, float fVerTile, 
                            bool bNormalsPointIn);

    VOID GetTangentBinormalVector(D3DXVECTOR3* pTangentOut, D3DXVECTOR3* pBinormalOut, const D3DXVECTOR3* pv0, const D3DXVECTOR3* pv1, const D3DXVECTOR3* pv2, 
                            float c0u, float c0v, float c1u, float c1v, float c2u, float c2v);


public:
    //! Creates the Sphere slicing up the resolution of the sphere into nRings down, and nSegments down.
    //  The texture coordinates are altered by the tile factor sent in.
    CSphere(IDirect3DDevice9* pd3dDevice, 
            int nRings = 10, int nSegments = 10, 
            float fHorizontalTile = 1.f, float fVerticalTile = 1.f, 
            bool bUseTangents = false, bool bNormalsPointIn = false);

    ~CSphere(void);

    //! Places the VB/IB on the device.
    HRESULT SetDevice(IDirect3DDevice9* pd3dDevice);

    //! PREREQ: Must SetStreamSource by calling SetupDevice. Calls the DrawPrimitive. 
    HRESULT Draw(IDirect3DDevice9* pd3dDevice) {
        return pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertices, 0, m_nSphereTriangles );
    }

    const D3DVERTEXELEMENT9* GetDecl() { return SphereVert::Decl; }
    UINT GetFVF() { return SphereVert::FVF; } 
    bool IsUsingTangents() { return m_bUseTangents; }
};
