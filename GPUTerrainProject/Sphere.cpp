#include "dxstdafx.h"
#include ".\sphere.h"

// ----------------------------------------------------------
// STRUCT STUFF
// ----------------------------------------------------------
const D3DVERTEXELEMENT9 CSphere::SphereVert::Decl[] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
};

CSphere::SphereVert::SphereVert(const D3DXVECTOR3& _pos, const D3DXVECTOR3& _norm, float _u, float _v) :
    pos(_pos), norm(_norm), tu(_u), tv(_v)
{
}

const D3DVERTEXELEMENT9 CSphere::SphereTanBinVert::Decl[] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },   
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    { 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },  
    { 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },  
    D3DDECL_END()
};

CSphere::SphereTanBinVert::SphereTanBinVert(const D3DXVECTOR3& _pos, 
                                            const D3DXVECTOR3& _norm, 
                                            const D3DXVECTOR3& _tan, 
                                            const D3DXVECTOR3& _bin, 
                                            float _u, float _v) :
    pos(_pos), norm(_norm), tan(_tan), binormal(_bin), tu(_u), tv(_v)
{
}

// ----------------------------------------------------------
// PUBLIC
// ----------------------------------------------------------

CSphere::CSphere(IDirect3DDevice9* pd3dDevice, int nRings, int nSegments, 
                 float fHorizontalTile, float fVerticalTile, bool bUseTangents, bool bNormalsPointIn) :

    m_pVB(NULL), m_pIB(NULL), m_pSphereTanVertDecl(NULL),
    m_bNormalsPointIn(bNormalsPointIn), m_bUseTangents(bUseTangents),
    m_nRings(nRings), m_nSegments(nSegments),
    m_fHorizontalTile(fHorizontalTile), m_fVerticalTile(fVerticalTile),
    m_nVertices(0), m_nIndices(0),
    m_nSphereTriangles(0)
{
    HRESULT hr;    

    if(m_bUseTangents)
    {
        V( pd3dDevice->CreateVertexDeclaration(SphereTanBinVert::Decl, &m_pSphereTanVertDecl) );
        V( CreateSphereWithTangents(pd3dDevice, nRings, nSegments, fHorizontalTile, fVerticalTile, bNormalsPointIn) );
    }
    else
        V( CreateSphere(pd3dDevice, nRings, nSegments, fHorizontalTile, fVerticalTile, bNormalsPointIn) );
}

CSphere::~CSphere(void)
{
    SAFE_RELEASE(m_pVB);
    SAFE_RELEASE(m_pIB);
    SAFE_RELEASE(m_pSphereTanVertDecl);    
}

HRESULT CSphere::SetDevice(IDirect3DDevice9* pd3dDevice) { 
    HRESULT hr;
    if(m_bUseTangents) {                
        V_RETURN( pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SphereTanBinVert)) );                
        V_RETURN( pd3dDevice->SetVertexDeclaration(m_pSphereTanVertDecl) );
    }
    else {
        V_RETURN( pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SphereVert)) ); 
        V_RETURN( pd3dDevice->SetFVF(SphereVert::FVF) );
    }
    V_RETURN( pd3dDevice->SetIndices(m_pIB) );
    return S_OK;
}

// ----------------------------------------------------------
// PRIVATE
// ----------------------------------------------------------

//! Gets the tangent vector for the given vector pv0 that makes a triangle with pv1 and pv2.
VOID CSphere::GetTangentBinormalVector(D3DXVECTOR3* pTangentOut, D3DXVECTOR3* pBinormalOut, const D3DXVECTOR3* pv0, const D3DXVECTOR3* pv1, const D3DXVECTOR3* pv2, 
                            float c0u, float c0v, float c1u, float c1v, float c2u, float c2v)
{
    assert(pv0 && pv1 && pv2);

    // get differences value
    D3DXVECTOR3 dv1v0 = *pv1 - *pv0;
    D3DXVECTOR3 dv2v0 = *pv2 - *pv0;

    D3DXVECTOR2 dc1c0, dc2c0;
    dc1c0.x = c1u - c0u; 
    dc1c0.y = c1v - c0v;

    dc2c0.x = c2u - c0u; 
    dc2c0.y = c2v - c0v;

    // Remember we want to find the 2 unknowns T and B. 
    // dv1v0 = dc1c0.u DOT T  +  dc1c0.v DOT B. Since we have two of these we can do a system of equations.

    float determinant = dc1c0.x * dc2c0.y - dc2c0.x * dc1c0.y;

    D3DXVECTOR3 tangent = determinant * ( dc2c0.y * dv1v0 - dc1c0.y * dv2v0 );
    D3DXVECTOR3 binormal = determinant * ( -dc2c0.x * dv1v0 + dc1c0.x * dv2v0 );        

    D3DXVec3Normalize(pTangentOut, &tangent);
    D3DXVec3Normalize(pBinormalOut, &binormal);
}

//! Creates a sphere of radius 1
HRESULT CSphere::CreateSphereWithTangents(IDirect3DDevice9* pd3dDevice, 
                              int nRings, int nSegments, 
                              float fHorTile, float fVerTile, 
                              bool bNormalsPointIn)
{
    HRESULT hr;
    // set vertex count and index count 

    DWORD dwVertices = ( nRings + 1 ) * ( nSegments + 1 ) ;
    DWORD dwIndices = 2 * nRings * ( nSegments + 1 ) ;

    m_nVertices = dwVertices;
    m_nIndices = dwIndices;

    // Create a vector of vertices and indices to look up to create tangent vectors later
    // This is done because we cannot read from the locked vb/ib s later and its easier this way.
    vector<SphereTanBinVert> vVertices;
    vector<WORD> vIndices;

    // Create the vertex buffer

    V_RETURN( pd3dDevice->CreateVertexBuffer( 
        dwVertices * sizeof ( SphereTanBinVert ), 
        D3DUSAGE_WRITEONLY, 0,  
        D3DPOOL_MANAGED, &m_pVB, NULL ) );

    // Create the index buffer

    V_RETURN( pd3dDevice->CreateIndexBuffer( 
        dwIndices * 2, 
        D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
        D3DPOOL_MANAGED, &m_pIB, NULL ) );        

    // Lock the vertex buffer

    SphereTanBinVert *pVertex, *pArrVertex ; 
    V_RETURN( m_pVB->Lock( 0, 0, (VOID**) &pVertex , 0 ) );
    pArrVertex = pVertex;

    // lock the index buffer 

    WORD *pIndices;
    V_RETURN( m_pIB->Lock( 0, 0, ( VOID ** ) &pIndices, 0 ) );

    // Establish constants used in sphere generation

    FLOAT fDeltaRingAngle = ( D3DX_PI / nRings );
    FLOAT fDeltaSegAngle = ( 2.0f * D3DX_PI / nSegments );

    WORD wVerticeIndex = 0;    
    int count =0;
    // Generate the group of rings for the sphere

    for( int ring = 0; ring < nRings + 1 ; ring++ )
    {
        FLOAT r0 = sinf ( ring * fDeltaRingAngle );
        FLOAT y0 = cosf ( ring * fDeltaRingAngle );

        // Generate the group of segments for the current ring

        for( int seg = 0; seg < nSegments + 1 ; seg++ )
        {
            FLOAT x0 = r0 * sinf( seg * fDeltaSegAngle );
            FLOAT z0 = r0 * cosf( seg * fDeltaSegAngle );

            // Add one vertices to the strip which makes up the sphere

            SphereTanBinVert tVert;
            tVert.pos.x = x0; tVert.pos.y = y0; tVert.pos.z = z0;

            if(m_bNormalsPointIn)   // point normal inside sphere            
                tVert.norm = -pVertex->pos ; // since sphere is of radius 1, it's normalized.
            else
                tVert.norm = pVertex->pos ; // since sphere is of radius 1, it's normalized.

            memset(&tVert.tan, 0, sizeof(D3DXVECTOR3)); // 0 out tangent vector to be incremented on later.
            memset(&tVert.binormal, 0, sizeof(D3DXVECTOR3)); // 0 out tangent vector to be incremented on later.
            tVert.tu = (FLOAT) seg / (FLOAT) nSegments * fHorTile; // set tex coordinates 
            tVert.tv = (FLOAT) ring / (FLOAT) nRings * fVerTile; // Multiplied by Tile Factor

            vVertices.push_back(tVert);
            *pVertex = tVert;
            pVertex ++;            

            // add two indices except for last ring 

            if ( ring != nRings ) 
            {
                *pIndices = wVerticeIndex ;                
                pIndices ++ ;
                *pIndices = wVerticeIndex + ( WORD ) ( nSegments + 1 ) ;
                pIndices ++ ;                

                vIndices.push_back(wVerticeIndex); // for tangent vectors later
                vIndices.push_back(wVerticeIndex + ( WORD ) ( nSegments + 1 )); // for tangent vectors later

                wVerticeIndex ++ ;
                count += 2;
            }
        } // end for seg 
    } // end for ring     

    m_nSphereTriangles = count - 2;

    //
    // Go Through like a drawing Triangle Strip and generate Tangent Vectors.
    //    
    vector<int> vCount;    
    vCount.resize(vVertices.size(), 0);
    
    assert(vIndices.size() > 3);
    for(size_t i=0; i<vIndices.size() - 2; i++)
    {
        D3DXVECTOR3 vTanOut, vBinOut;

        GetTangentBinormalVector(&vTanOut, &vBinOut,
                        &vVertices[vIndices[i]].pos,
                        &vVertices[vIndices[i+1]].pos,
                        &vVertices[vIndices[i+2]].pos,
                        vVertices[vIndices[i]].tu, vVertices[vIndices[i]].tv,
                        vVertices[vIndices[i+1]].tu, vVertices[vIndices[i+1]].tv,
                        vVertices[vIndices[i+2]].tu, vVertices[vIndices[i+2]].tv);

        vVertices[vIndices[i]].tan += vTanOut;
        vVertices[vIndices[i+1]].tan += vTanOut;
        vVertices[vIndices[i+2]].tan += vTanOut;
        vVertices[vIndices[i]].binormal += vBinOut;
        vVertices[vIndices[i+1]].binormal += vBinOut;
        vVertices[vIndices[i+2]].binormal += vBinOut;
                        
        vCount[vIndices[i]]++;
        vCount[vIndices[i+1]]++;
        vCount[vIndices[i+2]]++;
    }

    // average out the tanget vectors
    for(size_t i=0;i<vVertices.size(); i++)
    {
        assert(vCount[i] > 0);
        vVertices[i].tan /= (float)vCount[i];
        vVertices[i].binormal /= (float)vCount[i];
        D3DXVec3Normalize(&pArrVertex[i].tan, &vVertices[i].tan); // will this work on WRITEONLY?
        D3DXVec3Normalize(&pArrVertex[i].binormal, &vVertices[i].binormal); // will this work on WRITEONLY?
    }

    // finish and clean up

    m_pIB->Unlock();
    m_pVB->Unlock();

    return S_OK ; 
}

//! Creates a sphere of radius 1
HRESULT CSphere::CreateSphere(IDirect3DDevice9* pd3dDevice, 
                              int nRings, int nSegments, 
                              float fHorTile, float fVerTile, 
                              bool bNormalsPointIn)
{
    HRESULT hr;
    // set vertex count and index count 

    DWORD dwVertices = ( nRings + 1 ) * ( nSegments + 1 ) ;
    DWORD dwIndices = 2 * nRings * ( nSegments + 1 ) ;

    m_nVertices = dwVertices;
    m_nIndices = dwIndices;

    // Create the vertex buffer

    V_RETURN( pd3dDevice->CreateVertexBuffer( 
        dwVertices * sizeof ( SphereVert ), 
        D3DUSAGE_WRITEONLY, 0,  
        D3DPOOL_MANAGED, &m_pVB, NULL ) );

    // Create the index buffer

    V_RETURN( pd3dDevice->CreateIndexBuffer( 
        dwIndices * 2, 
        D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
        D3DPOOL_MANAGED, &m_pIB, NULL ) );        

    // Lock the vertex buffer

    SphereVert *pVertex ; 
    V_RETURN( m_pVB->Lock( 0, 0, (VOID**) &pVertex , 0 ) );

    // lock the index buffer 

    WORD *pIndices;
    V_RETURN( m_pIB->Lock( 0, 0, ( VOID ** ) &pIndices, 0 ) );

    // Establish constants used in sphere generation

    FLOAT fDeltaRingAngle = ( D3DX_PI / nRings );
    FLOAT fDeltaSegAngle = ( 2.0f * D3DX_PI / nSegments );

    WORD wVerticeIndex = 0;    
    int count =0;
    // Generate the group of rings for the sphere

    for( int ring = 0; ring < nRings + 1 ; ring++ )
    {
        FLOAT r0 = sinf ( ring * fDeltaRingAngle );
        FLOAT y0 = cosf ( ring * fDeltaRingAngle );

        // Generate the group of segments for the current ring

        for( int seg = 0; seg < nSegments + 1 ; seg++ )
        {
            FLOAT x0 = r0 * sinf( seg * fDeltaSegAngle );
            FLOAT z0 = r0 * cosf( seg * fDeltaSegAngle );

            // Add one vertices to the strip which makes up the sphere

            pVertex->pos = D3DXVECTOR3(x0,y0,z0); // normalized 

            if(m_bNormalsPointIn)   // point normal inside sphere            
                pVertex->norm = -pVertex->pos ; // since sphere is of radius 1, it's normalized.
            else
                pVertex->norm = pVertex->pos ; // since sphere is of radius 1, it's normalized.

            // pVertex->color = 0xffffffff ; 
            pVertex->tu = (FLOAT) seg / (FLOAT) nSegments * fHorTile; // set tex coordinates 
            pVertex->tv = (FLOAT) ring / (FLOAT) nRings * fVerTile; // Multiplied by Tile Factor

            pVertex ++;            

            // add two indices except for last ring 

            if ( ring != nRings ) 
            {
                *pIndices = wVerticeIndex ;
                pIndices ++ ;
                *pIndices = wVerticeIndex + ( WORD ) ( nSegments + 1 ) ;
                pIndices ++ ;
                wVerticeIndex ++ ;

                count += 2;
            }
        } // end for seg 
    } // end for ring     

    m_nSphereTriangles = count - 2;

    m_pIB->Unlock();
    m_pVB->Unlock();

    return S_OK ; 
}


