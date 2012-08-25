#include "dxstdafx.h"
#include ".\iterrain.h"

using gdef::WCharStringToCharString;
using std::ifstream;
using std::ios;

//! Loads .hmp files into memory
UINT ITerrain::LoadHMP(LPCSTR fn) {
    // TODO: LOAD TEXTURE NAME FROM HEADER IN HMP
    LOG_CREATE(L"GPUTerrain");
    LOG_INFO("Loading File: " << fn);    

    //
    // Open File
    //
    ifstream fin;
    fin.open(fn, ios::in | ios::binary);
    if(!fin.is_open()) {
        LOG_DEBUG("Error opening file! " << fn);
        return EXIT_FAILURE;
    }

    //
    // Read 100 byte Header
    //
    fin.read((char*)&m_Width, 4); // 4 bytes for int
    fin.read((char*)&m_Height, 4);     
    fin.seekg(92, ios::cur); // move to 100th position, skips reserved bytes.
    
    // Ensure Terrain is a (power of 2) + 1 in each dimension and greater than min dimension
    //
    bool bWCorrect = false, bHCorrect = false;
    for(UINT i = MIN_DIMENSION-1; i < MAX_DIMENSION; i*=2) {
        if(m_Width-1 == i)  bWCorrect = true;
        if(m_Height-1 == i)  bHCorrect = true;
    }
    LOG_DEBUG("Dimensions of Terrain: " << m_Width << " * " << m_Height);
    if(!bWCorrect || !bHCorrect) {
        fin.close();
        return EXIT_FAILURE;    
    }    

    //
    // Load up HMP in UCHAR array then convert to float
    //
    UINT area = m_Width*m_Height;
    UCHAR *tHMP = new UCHAR[area];
    fin.read((char*)tHMP, area);    // Read the huge block in one go.

    m_HeightMap = new float[area];	
    for(UINT i=0;i<area;i++) 
        m_HeightMap[i] = (float)tHMP[i];

    SAFE_DELETE_ARRAY(tHMP);
    
    UINT nBytes = fin.tellg();   
    fin.close();
    
    LOG_DEBUG("Bytes Read for HMP: " << nBytes << " : For an Area of: " << area);   

    //
    // Smooth HMP if defined
    //
    SmoothHMP();
    LOG_DEBUG("Height Map Smoothed" << std::endl);
    ZeroEdges();

    LoadNormals(); // used for lighting    

	GenerateDiffMap(); // used for geomorphing

    if( (nBytes-100) == area )
        return EXIT_SUCCESS;
    return EXIT_FAILURE;
}


UINT ITerrain::GenerateDiffMap(void) {
	
    m_DiffMap[0] = new float[m_Width*m_Height];
    m_DiffMap[1] = new float[m_Width*m_Height];

	for(UINT i = 0; i < m_Width*m_Height; i ++) {
		m_DiffMap[0][i] = 0;
        m_DiffMap[1][i] = 0;
	}

	int skip = 2;
	int nSkip = 1;
	for(int res = 0; res < (int)m_LOD-1; res ++) { //constantly increase # of skipped vertices
        float *handle = m_DiffMap[res];
		//first get all the columns
		for(UINT i = (int) pow(2.f, res); i < m_Width; i += skip) {	
			for(UINT j = 0; j < m_Height; j += skip) {
				UINT loc = i + j*m_Width; 
				float val = (m_HeightMap[loc-nSkip]+m_HeightMap[loc+nSkip])/2.0f;
				handle[loc] = (val - m_HeightMap[loc])*m_HeightSpacing;
			}
		}

		//then the rows		
		for(UINT j = (int) pow(2.f, res); j < m_Height; j += skip) {
			for(UINT i = 0; i < m_Width; i += skip) {
				UINT loc = i + j*m_Width; 
				float val = (m_HeightMap[loc-nSkip*m_Width]+m_HeightMap[loc+nSkip*m_Width])/2.0f;
				handle[loc] = (val - m_HeightMap[loc])*m_HeightSpacing;
			}
		}

		//the the vertices which fell on columns and rows being removed
		for(UINT i = (int) pow(2.f, res); i < m_Width; i += skip) {
			for(UINT j = (int) pow(2.f, res); j < m_Height; j += skip) {
				UINT loc = i + j*m_Width; 
				float val = (m_HeightMap[loc+nSkip*(m_Width-1)]+m_HeightMap[loc-nSkip*(m_Width-1)])/2.0f;
				handle[loc] = (val - m_HeightMap[loc])*m_HeightSpacing;
			}
		}

		skip *= 2;
		nSkip *= 2;
	}

    // DEBUG - Print out a tile and it's 2 diffs
    /*
    FILE *f = fopen("TILE_OUTPUT.txt", "w");
    int begin = 8;
    int fin = 16;
    for(int i=begin; i<=fin; i++) {
        for(int j=begin; j<=fin; j++) {
            fprintf(f, "%02.3f ", m_HeightMap[i+j*m_Width]);            
        }
        fprintf(f, "\n");        
    }
    fprintf(f, "\n");
    fprintf(f, "\n");
    for(int i=begin; i<=fin; i++) {
        for(int j=begin; j<=fin; j++) {
            fprintf(f, "%02.3f ", m_DiffMap[0][i+j*m_Width]);            
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    fprintf(f, "\n");
    for(int i=begin; i<=fin; i++) {
        for(int j=begin; j<=fin; j++) {
            fprintf(f, "%02.3f ", m_DiffMap[1][i+j*m_Width]);            
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    fprintf(f, "\n");
    fclose(f);
    */

	return EXIT_SUCCESS;
}

//! Smooths the y values by averaging the values in a 3x3 kernel.
// PREREQ: m_HeightMap is populated and m_Width and m_Height are set.
VOID ITerrain::SmoothHMP() {
    int area = m_Width*m_Height;
    float *temp = new float[area];
    
    for(int i=0; i<(int)m_Height; i++) {
        for(int j=0; j<(int)m_Width; j++) {            
            int n = 0;
            float avg = 0;
            int curPos = i*m_Width + j;
            
            for(int k = i-1; k <= i+1; k++) { // cycle through kernel
                for(int l = j-1; l <= j+1; l++) {                    
                    if( k >= 0 && k < (int)m_Height && l >= 0 && l < (int)m_Width)
                    { // if in bounds
                        avg += m_HeightMap[k*m_Width+l];
                        n++;                        
                    } // end of if
                }
            } // end of cycling thru kernel
            temp[curPos] = avg / n;
        }
    }    
    SAFE_DELETE_ARRAY(m_HeightMap);
    m_HeightMap = temp;
}

void ITerrain::ZeroEdges() 
{
    UINT displace = (m_Height-1) * m_Width;
    for(UINT i=0; i<m_Width; i++) 
    {   // top and bottom row
        m_HeightMap[i] = 0.f;
        m_HeightMap[i+displace] = 0.f;
    }

    for(UINT i=0; i<m_Height; i++)  
    {   // left and right column
        m_HeightMap[i*m_Width] = 0.f;
        m_HeightMap[i*m_Width + m_Width-1] = 0.f;
    }        
}

//! Generates the normals after LoadHMP is called through normal averaging of the triangles.
VOID ITerrain::LoadNormals() {
    // This function computes the normal for every vertex by finding the normals
    // of the 4 neighboring squares (built from 2 triangles). If at corner, etc averages with 0,1,0
    // The ijth square is the one bottom right of vertex.   *---.
    //                                                      |   |

    // NB: The square 2 triangles make is often referred to as a quad, here it will simply be a square.
    SAFE_DELETE_ARRAY(m_NormalMap);
    m_NormalMap = new D3DXVECTOR3[m_Width*m_Height];

    for(int i=0; i<(int)m_Height; i++) {
        for(int j=0; j<(int)m_Width; j++) {
            D3DXVECTOR3 n1 = GetSquareNormal(i-1, j-1);
            D3DXVECTOR3 n2 = GetSquareNormal(i-1, j); // above
            D3DXVECTOR3 n3 = GetSquareNormal(i, j-1);
            D3DXVECTOR3 n4 = GetSquareNormal(i, j); // current (bottom right square)

            D3DXVECTOR3 t = (n1 + n2 + n3 + n4) / 4;
            D3DXVec3Normalize(&t, &t);
            
            m_NormalMap[i*m_Width + j] = t;
        }
    }
}

D3DXVECTOR3 ITerrain::GetSquareNormal(int i, int j) {
    // Return up if on edge of heightmap
    if(i < 0 || j < 0 || i >= (int)(m_Height-1) || j >= (int)(m_Width-1) )
        return D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    int xpos = i*m_Width;
    float h0 = m_HeightMap[xpos + j] * m_HeightSpacing;
    float h1 = m_HeightMap[xpos + j + 1] * m_HeightSpacing;
    float h2 = m_HeightMap[xpos + m_Width + j] * m_HeightSpacing;
    float h3 = m_HeightMap[xpos + m_Width + j + 1] * m_HeightSpacing;

    //     e1
    // e2       e3
    //     e4
    D3DXVECTOR3 e1((float)m_CellSpacing, h1-h0, 0.0f);
    D3DXVECTOR3 e2(0.0f, h2-h0, (float)(-m_CellSpacing) );

    D3DXVECTOR3 e3(0, h1-h3, (float)m_CellSpacing);
    D3DXVECTOR3 e4((float)-m_CellSpacing, h2-h3, 0.0f);

    D3DXVECTOR3 n1;
    D3DXVec3Cross(&n1, &e1, &e2);
    D3DXVec3Normalize(&n1, &n1);

    D3DXVECTOR3 n2;
    D3DXVec3Cross(&n2, &e4, &e3);
    D3DXVec3Normalize(&n2, &n2);

    return ((n1 + n2) * 0.5f);
}

//! TODO: Take into account height spacing when deciding maxErrorVertex?
// Checks LOD 0's error
void ITerrain::GetMaxErrorVertexAtTile(D3DXVECTOR3 v, vector<D3DXVECTOR3> &maxPoints, int &maxErrorIndex) {
	UINT maxX = min(m_Width-1, (UINT)v.x + MIPMAP_TILESIZE-1);
	UINT maxZ = min(m_Width-1, (UINT)v.z + MIPMAP_TILESIZE-1);
    UINT LODcheck = 0;
	float maxError = 0;
	// float *maxErrorVal = new float[m_LOD];

	for(UINT it = 0; it < m_LOD; it ++) {
		maxErrorIndex = (int) (v.x+v.z*m_Width);
		// maxErrorVal[it] = 0;
	}

	//check all vertices that will be removed when changing from highest resolution
	//basically all vertices with an odd number
	for(UINT i = (UINT) v.x; i < maxX; i ++) { // finds maxErrorIndex
		for(UINT k = (UINT) v.z; k < maxZ; k ++) {
			if(i%2 || k%2)
			{	if(maxError < abs(m_DiffMap[LODcheck][i+k*m_Width]) )// finds the absolute max difference, made huge difference.
				{	maxErrorIndex = i+k*m_Width;
					maxError = abs(m_DiffMap[LODcheck][i+k*m_Width]) ;
				}
			}
		}
	}

	//now we have the max error vertex, create its position at all LODs
	int indexVal = maxErrorIndex;
	int x = indexVal%m_Width;
	int z = indexVal/m_Width;
	
	//first one is at original position
    maxPoints.push_back( D3DXVECTOR3((float)x, m_HeightMap[indexVal], (float)z) );
	for(UINT i = 1, res = 2; i < m_LOD; i ++, res *= 2) {
		int prevX, nextX;
		prevX = (x/res) * res;		//integer division to round
		nextX = prevX + res;
        float yVal;

		if(z%res) {			//entire row removed, must do something different

			//check value of neighbours after they have been removed
			float y1 = m_HeightMap[prevX + z*m_Width];
			y1 += m_DiffMap[i-1][prevX + z*m_Width];//should make sure it is + and not -
			
			float y2 = m_HeightMap[nextX + z*m_Width];
			y2 += m_DiffMap[i-1][nextX + z*m_Width];

			yVal = gdef::lerp(y1, y2, (x%res)/(float)res);
		}
		else {
			float y1 = m_HeightMap[prevX + z*m_Width];
			float y2 = m_HeightMap[nextX + z*m_Width];

			yVal = gdef::lerp(y1, y2, (x%res)/(float)res);
		}
        maxPoints.push_back( D3DXVECTOR3((float)x, yVal, (float)z) );
	}

    // displace all maxErrorVertices by cell/height spacing.
    for(UINT i = 0; i < m_LOD; i++) {
        maxPoints[i].x *= m_CellSpacing;
        maxPoints[i].z *= m_CellSpacing;
        maxPoints[i].y *= m_HeightSpacing;
    }
    // SAFE_DELETE_ARRAY(maxErrorVal);
}

//! Gets the minimum/maximum height of the terrain tile, used for culling later.
VOID ITerrain::GetMinMaxHeight(D3DXVECTOR3 v, float &min, float &max) 
{
    // this is before cell/height spacing expansion
    min = v.y;
    max = v.y;

    int xindex = (int)v.x;
    int zindex = (int)v.z;   

    int zmax = zindex + MIPMAP_TILESIZE-1;
    int xmax = xindex + MIPMAP_TILESIZE-1;
    
    for(int i=zindex; i<zmax; i++) {
        for(int j=xindex; j<xmax; j++) {
            int pos = j+i*m_Width;
            if(min > m_HeightMap[pos])
                min = m_HeightMap[pos] - 1;
            if(max < m_HeightMap[pos])
                max = m_HeightMap[pos] + 1;
        }
    }
}

UINT ITerrain::GetNumberOfLODs(void)
{	return m_LOD;
}
