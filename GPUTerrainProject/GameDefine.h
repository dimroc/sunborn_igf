//! GameDefine.h - Defines Game Constants

#pragma once

#define         RESOLUTION_WIDTH            1024
#define         RESOLUTION_HEIGHT           768

#define         DEFAULT_LOG_FILE            L"GPUTerrainProject.log" //! default log file
#define         FOV_DIVISOR                 4       //! FOV is D3DX_PI/FOV_DIVISOR. To widen ViewFrustum.
// #define         MIPMAP_TILESIZE             65      //! Tilesize for GeoMipmapping

#define         SKYCAM_HEIGHT               9000.0f
#define         GROUND_CAMERA_SPEED         120.0f
#define         FARPLANE_DISTANCE           19999.0f

#define         CHARACTER_SCALE             2.f

#define			BASINHEIGHT					50.f
#define         CLOUD_ALPHA_CUTOFF          0x0000000f

#define         WINDOWED                    1

#define			MENU_LOGO					L"..\\Media\\sunborn_logo.bmp"
#define			LIVE_ICON					L"..\\Media\\life_icon.bmp"