#pragma once


// A structure for our custom vertex type
struct SVertex
{
    float x, y, z, rhw; // The transformed position for the vertex
    unsigned long color;        // The vertex color
};
 struct SColor
 {
	int r, g, b;

	SColor() : r(0), g(0), b(0) { }
	SColor(int _r, int _g, int _b) : r(_r), g(_g), b(_b) { }
 };

class CGUIObject {
protected:
	int						m_nX, m_nY;				//! screen coordinates
	int						m_nHeight, m_nWidth;	//! dimensions

	CGUIObject(): m_nX(0), m_nY(0), m_nHeight(0), m_nWidth(0){ }
	CGUIObject(int x, int y, int h, int w) : m_nX(x), m_nY(y), m_nHeight(h), m_nWidth(w){ }

public:
	int GetX() { return m_nX; }
	int GetY() { return m_nY; }
	int GetWidth() { return m_nWidth; }
	int GetHeight() { return m_nHeight; }

	void SetHeight(int h) { m_nHeight = h; }
	void SetWidth(int w) { m_nWidth = w; }
	void SetX(int x) { m_nX = x; }
	void SetY(int y) { m_nY = y; }

	bool CheckInBounds(int x, int y){
		if (m_nX <= x && x <= m_nX + m_nWidth &&
			m_nY <= y && y <= m_nY + m_nHeight)
			return true;
		return false;
	}

};
