//! GlobalDefine.h
// 
//! Place Program wide constants, typedefs, and definitions in gdef namespace here
//! eg: typedef bool BOOL!
//
//

#pragma once

#define AABB_OUTSIDE      0x01
#define AABB_INSIDE       0x02
#define AABB_SPANNING     0x04


struct BoxVertColor;

//! Print to Output Panel in Visual Studio. Disappear in RELEASE build.
#if defined(DEBUG) || defined(_DEBUG)
#define dprintf gdef::debugprintf
#else
#define dprintf(...) void()
#endif

#define     PRINT_ENTER_FUNCTION_DEBUG  dprintf("Entered: %s\n", __FUNCTION__);

// TODO: Make a lprintf (log printf) to compliment the dprintf.

namespace gdef {
    //////
    ////// FUNCTIONS //////
    //////

    //! Print to Output Panel in Visual Studio. Don't print a string longer than 256 char, Overflow.
    int debugprintf(const char *fmt, ...);
    //! convert WCHAR* to char *
    UINT WCharStringToCharString(LPCWSTR fn, LPSTR pszFilename, int MAXLENGTH);

    //! linear interpolation, duh
    float lerp(float a, float b, float tween);
    VOID lerp(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pA, const D3DXVECTOR4 *pB, float tween);        

    //! test for the intersection of 2 2D rectangles
    bool IsBoxIntersect2D(const D3DXVECTOR2 &pos1, float lengthx1, float lengthz1, 
                          const D3DXVECTOR2 &pos2, float lengthx2, float lengthz2);
    bool IsBoxIntersect3D(const D3DXVECTOR3 &pos1, const D3DXVECTOR3 &length1,
                          const D3DXVECTOR3 &pos2, const D3DXVECTOR3 &length2);

    //! Returns whether the first passed cube is entirely inside the second passed cube.
    bool IsCubeInsideCube(const D3DXVECTOR3 &pos1, const D3DXVECTOR3 &length1,
                          const D3DXVECTOR3 &pos2, const D3DXVECTOR3 &length2);

    float GetAbsoluteManhattanDistance(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2);
    float GetCosAngle3D(const D3DXVECTOR3* origin, const D3DXVECTOR3* p1, const D3DXVECTOR3* p2);
    float GetCosAngle2D(float ox, float oy,
                        float p1x, float p1y,
                        float p2x, float p2y);

    float TriangleArea(D3DXVECTOR3 *p1, D3DXVECTOR3 *p2, D3DXVECTOR3 *p3);

    VOID NormalizedNegativeReciprocalY(D3DXVECTOR3 *pvOut, const D3DXVECTOR3 *pvIn);

        // Generate Corner Vectors. 
        //  4 --- 5                3 --- 2 - top
        //  |     |                |     |
        //  0 --- 1  - bottom      7 --- 6          
    //! Same as QuadTrees QTNodes, start with something stupid and boy does it perpetrate.
    //! PREREQ: outputs corners, an array of 8 D3DXVECTORS
    VOID GenerateAABBPosMiddle(D3DXVECTOR3 corners[8], 
                               const D3DXVECTOR3 &center, 
                               const D3DXVECTOR3 &displace);

    VOID MatrixTranslation(D3DXMATRIX *pmtx, const float x, const float y, const float z);            

    //! Stores indices to the corner vertices used in an optimized view frustum culling
    struct CornerIndexPair {
        int i1, i2;
        CornerIndexPair(int _i1, int _i2) : i1(_i1), i2(_i2) {}
        CornerIndexPair() {}
    };

    //! Extract planes from world matrix. Helper function for derived classes. TAKEN FROM MS SAMPLE.
    void ExtractPlanes( D3DXPLANE * p_planes, const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatProj);    
    //! Finds the 2 corners most aligned with a planes normal
    VOID FindAxisAlignedCorners( D3DXPLANE * p_planes, CornerIndexPair *pOutAlignedCornerPair );
	//! Tests the corners against the passed in Frustum Planes using the optimized CornerIndexPairs.
	UCHAR AABBFrustumTest(D3DXPLANE pFrustumPlanes[6], CornerIndexPair arrCIP[6], const D3DXVECTOR3* corners);
    //! Generates 24 BoxVertColor Vertices from the passed in 8 corners suitable for a DP Linelist call.
    VOID GenerateBoundingBox(BoxVertColor* verts, const D3DXVECTOR3* corners);

    // a RECT with floating point left,top,right,bottom values
    struct RECTF {
        float left;
        float top;
        float right;
        float bottom;
        RECTF() {};
        RECTF(const float l, const float t, const float r, const float b) :
        left(l), top(t), right(r), bottom(b) {}
    };

    struct COLOR
    {
        UCHAR b, g, r, a; // order necessary when dumping color into ULONG for d3d.
        COLOR() {}
        COLOR(UCHAR _a, UCHAR _r, UCHAR _g, UCHAR _b) :
            a(_a), r(_r), g(_g), b(_b)
        {}
        COLOR& operator= (const COLOR &c) {
            a = c.a; r = c.r; g = c.g; b = c.b;
        }
        operator DWORD() const {
            DWORD temp;
            memcpy(&temp, this, sizeof(COLOR));
            return temp;
        }
        operator unsigned int() const {
            unsigned int temp;
            memcpy(&temp, this, sizeof(COLOR));
            return temp;
        }
    };      

    struct Matrix3x3
    {
	    float vals[9];
        Matrix3x3();
	    Matrix3x3(float _11, float _12, float _13, float _21, float _22, float _23, float _31, float _32, float _33);    	
        Matrix3x3& operator= (const Matrix3x3& m);
    };

    struct SSpring {  //! Holds the camera target destination and speed. Moves like a spring.        
        float fInertialDist;    //! Spring at rest position
        float fKCoef;   //! coefficient k for spring
    };

    //! Finds the force exerted by spring when the endpoints are at src a and dest b.
    float ForceMovingAlongSpring(const SSpring* pspring, 
                                 const D3DXVECTOR3* pA, 
                                 const D3DXVECTOR3* pB, 
                                 float SPRING_TOLERANCE = 0.00005f);


    static const int     MIN_INT    =                             (int)(-2147483647 - 1);
    static const int     MAX_INT    =                             2147483647;
    
    static const UINT    MIN_UINT   =                             0; // duh
    static const UINT    MAX_UINT   =                             4294967295;
    
    static const float   MIN_FLOAT  =                             1.175494351e-38F;
    static const float   MAX_FLOAT  =                             3.402823466e+38F;

    static const float   EPSILON    =                             0.00005f; // float epsilon
}


#define DIM_GET_ACCESSOR(x,y)           inline x Get##y() const { return m_##y; }
#define DIM_SET_ACCESSOR(x,y)           inline void Set##y(x t) { m_##y = t; }
#define GET_SET_MACRO(x,y)              DIM_GET_ACCESSOR(x,y) DIM_SET_ACCESSOR(x,y)

// Logger setup Macro
#ifndef _NOLOGGING         
//!
//! MS VISUAL 2005 Broke all this shit because of the new CLR (not used in log4cplus). Don't use in Visual 2005.
//! 

#pragma warning( disable : 4512 )  // remove failure to generate assignment operate warning. This is on purpose.
#pragma warning( disable : 4127 )  // conditional expressions is constant

#include "log4cplus/logger.h"
#include "log4cplus/configurator.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/layout.h"
#include "log4cplus/ndc.h"
#include "log4cplus/helpers/loglog.h"

#pragma warning( default : 4512 ) // reset

using namespace log4cplus;

//! Use the following below includes at the top of file
//! just use LOG_CREATE_FEAULT
#define LOG_CREATE(x)            static Logger _logger = Logger::getInstance(x);
#define LOG_CREATE_DEFAULT       LOG_CREATE(L"GPUTerrainProject")

#if defined(DEBUG) || defined(_DEBUG)

// Use in InitApp just once throughout program to tell logger instance which file to use.
// Then you can just call LOG_CREATE and the log later on.
// Log files are 1 MB ( 1024*1024 )
#define LOG_INITFILE(x)        {    SharedAppenderPtr _append_1(new RollingFileAppender(x, 1024*1024) );   \
                                    Logger::getRoot().addAppender(_append_1); \
                                    _logger.setLogLevel(DEBUG_LOG_LEVEL);   \
                                    LOG4CPLUS_INFO(_logger, "**********************");   \
                                    LOG4CPLUS_INFO(_logger, "NEWEXECUTION - DATE OF COMPILATION: " << __TIMESTAMP__);    \
                                    LOG4CPLUS_INFO(_logger, "**********************") }

#else

#define LOG_INITFILE(x)        {    SharedAppenderPtr _append_1(new RollingFileAppender(x, 1024*1024) );   \
                                    Logger::getRoot().addAppender(_append_1); \
                                    _logger.setLogLevel(INFO_LOG_LEVEL);   \
                                    LOG4CPLUS_INFO(_logger, "**********************");   \
                                    LOG4CPLUS_INFO(_logger, "NEWEXECUTION - DATE OF COMPILATION: " << __TIMESTAMP__);    \
                                    LOG4CPLUS_INFO(_logger, "**********************") }
#endif

//! Various Logging Functions
#define LOG_TRACE(x)  LOG4CPLUS_TRACE(_logger, __LINE__ << ":" << __FUNCTION__ << " - " << x) 
#define LOG_DEBUG(x)  LOG4CPLUS_DEBUG(_logger, __LINE__ << ":" << __FUNCTION__ << " - " << x)
#define LOG_INFO(x)   LOG4CPLUS_INFO(_logger, __LINE__ << ":" << __FUNCTION__ << " - " << x) 
#define LOG_WARN(x)   LOG4CPLUS_WARN(_logger, __LINE__ << ":" << __FUNCTION__ << " - " << x) 
#define LOG_ERROR(x)  LOG4CPLUS_ERROR(_logger, __LINE__ << ":" << __FUNCTION__ << " - " << x) 
#define LOG_FATAL(x)  LOG4CPLUS_FATAL(_logger, __LINE__ << ":" << __FUNCTION__ << " - " << x) 

#define LOG_SETLEVEL_TRACE  _logger.setLogLevel(TRACE_LOG_LEVEL); 
#define LOG_SETLEVEL_DEBUG  _logger.setLogLevel(DEBUG_LOG_LEVEL); 
#define LOG_SETLEVEL_INFO   _logger.setLogLevel(INFO_LOG_LEVEL); 
#define LOG_SETLEVEL_WARN   _logger.setLogLevel(WARN_LOG_LEVEL); 
#define LOG_SETLEVEL_ERROR  _logger.setLogLevel(ERROR_LOG_LEVEL); 
#define LOG_SETLEVEL_FATAL  _logger.setLogLevel(FATAL_LOG_LEVEL); 
                                    

#else   // else of #ifndef _NOLOGGING. Therefore no logging.

#define LOG_CREATE_DEFAULT
#define LOG_CREATE(x)
#define LOG_INITFILE(x)      

#define LOG_TRACE(x) 
#define LOG_DEBUG(x) 
#define LOG_INFO(x) 
#define LOG_WARN(x) 
#define LOG_ERROR(x) 
#define LOG_FATAL(x) 

#define LOG_SETLEVEL_TRACE 
#define LOG_SETLEVEL_DEBUG 
#define LOG_SETLEVEL_INFO 
#define LOG_SETLEVEL_WARN 
#define LOG_SETLEVEL_ERROR 
#define LOG_SETLEVEL_FATAL 

#endif                       

//
// These assume functions return 0 on success ( EXIT_SUCCESS )
//
// MUST HAVE UINT r declared.

#ifndef D
#define D(x)           { r = x; if( r ) { LOG_DEBUG(L"Function Failed Returning: " << r) } }
#endif
#ifndef D_RETURN
#define D_RETURN(x)    { r = x; if( r ) { LOG_ERROR(L"Function Failed Returning: " << r) return r;} }
#endif
