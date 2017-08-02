#ifndef __XORSTRING_H__
#define __XORSTRING_H__

#include <string>

class CXorString
{
public:
	CXorString(const char* p = "", unsigned long xor = 0x51E64E17, bool xorInstantly = true);
	~CXorString();

	/// <summary>
	/// Sets the interal buffer to the given string
	/// </summary>
	/// <param name = "p">Value to use</param>
	void String(const char* p);

	/// <summary>
	/// Xors the string previously set by constructor or CXorString::String(const char*)
	/// </summary>
	/// <returns>Pointer to the just xor'ed string</returns>
	const char* Xor();

	std::string ToString() { return std::string(m_pBuffer); }
	const char* ToCharArray() { return (const char*)m_pBuffer; }
private:
	char* m_pBuffer;
	unsigned long m_iXorKey;
};

#endif // __XORSTRING_H__