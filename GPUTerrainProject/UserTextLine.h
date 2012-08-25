#pragma once

#include "dxstdafx.h"

using std::string;

#define MAX_STRING_SIZE 64

class CUserTextLine
{
private:
	string					m_strText;

public:
	CUserTextLine() { };
	CUserTextLine(LPCSTR text) { m_strText.assign(text); }

	// Adds the character c to the end of this line
	// Does nothing if the string has already reached its max length
	void AddChar(UINT c) 
	{
		if (m_strText.size() < MAX_STRING_SIZE)
			m_strText.push_back((char)c);
	}

	// Adds the string to the end of this line
	void Append(LPCSTR text){
		m_strText.append(text);
	}

	// Erases the last character in the string
	// Does nothing if the string is empty
	void Backspace() { 
		if (m_strText.size() < 1)
			return;
		m_strText.erase(m_strText.size() - 1); 

	}

	// Clears the string
	void Clear() { m_strText.clear(); }
	// Returns the string
	LPCSTR GetText() { return m_strText.c_str(); }
	
	// Sets the string to the passed in parameter
	// Old string is lost
	void SetText(LPCSTR text) { 
		m_strText.clear();
		m_strText.append(text);
	}
	// Returns the length
	UINT Length() { return (UINT) m_strText.length(); }
	
	// Returns true if length = 0
	bool Empty() { return m_strText.length() == 0; }
};