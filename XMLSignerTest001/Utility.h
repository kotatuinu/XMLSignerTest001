#pragma once
#include <string>
using namespace std;

class CUtility
{
public:

	// BSTR to VARIANT(BYTE ARRAY)
	static bool convertBSTRtoVARIANT(const BSTR inputData, VARIANT &outputData);
	// VARIANT(BYTE ARRAY) to BSTR
	static bool convertVARIANTtoBSTR(const VARIANT &inputData, BSTR &outputData);

	// BSTR to BYTE ARRAY
	static bool convertBSTRtoMultiByteArray(const BSTR inputData, LPBYTE *outputData, size_t &dataSize, int codePage = CP_UTF8);

	// save to File(BSTR)
	static bool saveToFile(const string & outputFileName, const BSTR data);
	// load File(BYTE Arrya)
	static bool loadFile(const string &inputFileName, BSTR &data, size_t &dataSize);

	// Free Allocation Area
	static void freeByteArray(LPBYTE outputData);

private:
	CUtility() {};
	~CUtility() {};
};

