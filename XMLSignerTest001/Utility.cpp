#include "stdafx.h"
#include "Utility.h"
#include "comdef.h"

bool CUtility::convertBSTRtoVARIANT(const BSTR inputData, VARIANT &outputData)
{
	LPBYTE lpData = NULL;
	size_t dataSize = 0;
	bool bRslt = convertBSTRtoMultiByteArray(inputData, &lpData, dataSize);

	SAFEARRAYBOUND rgb[1];
	rgb[0].cElements = dataSize;
	rgb[0].lLbound = 0;

	SAFEARRAY* psa = SafeArrayCreate(VT_UI1, 1, rgb);
	if (psa == NULL) {
		freeByteArray(lpData);
		return false;
	}

	void HUGEP *pvData;
	HRESULT hr = SafeArrayAccessData(psa, &pvData);
	if (FAILED(hr)) {
		freeByteArray(lpData);
		return false;
	}

	memcpy_s(pvData, dataSize, lpData, dataSize);
	freeByteArray(lpData);

	hr = SafeArrayUnaccessData(psa);
	if (FAILED(hr)) {
		return false;
	}

	outputData.parray = psa;
	outputData.vt = VT_ARRAY | VT_UI1;

	return true;
}

bool CUtility::convertVARIANTtoBSTR(const VARIANT &inputData, BSTR &outputData)
{
	if (inputData.vt != (VT_ARRAY | VT_UI1)) {
		return false;
	}

	long lIndexMin = 0;
	long lIndexMax = 0;
	SafeArrayGetLBound(inputData.parray, 1, &lIndexMin);
	SafeArrayGetUBound(inputData.parray, 1, &lIndexMax);

	size_t dataSizet = lIndexMax - lIndexMin;

	SAFEARRAYBOUND rgb[1];
	rgb[0].cElements = dataSizet;
	rgb[0].lLbound = 0;

	SAFEARRAY* psa = inputData.parray;

	void HUGEP *pvData;
	HRESULT hr = SafeArrayAccessData(psa, &pvData);
	if (FAILED(hr)) {
		return false;
	}
	LPBYTE lpData = new BYTE[dataSizet];

	memcpy_s(lpData, dataSizet, pvData, dataSizet);
	delete lpData;
	outputData = _bstr_t((char*)pvData).copy();

	hr = SafeArrayUnaccessData(psa);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool CUtility::convertBSTRtoMultiByteArray(const BSTR inputData, LPBYTE *outputData, size_t &dataSize, int codePage)
{
	int iRtn = WideCharToMultiByte(codePage, WC_SEPCHARS, inputData, -1, NULL, 0, NULL, NULL);
	if (iRtn == 0) {
		return false;
	}

	LPSTR lpMultiByteStr = (LPSTR)new BYTE[iRtn];		// 新しい文字列を受け取るバッファのアドレス
	ZeroMemory(lpMultiByteStr, iRtn);
	iRtn = WideCharToMultiByte(codePage, WC_SEPCHARS, inputData, iRtn, lpMultiByteStr, iRtn, NULL, NULL);
	if (iRtn == 0) {
		return false;
	}

	*outputData = (LPBYTE)lpMultiByteStr;
	dataSize = iRtn;

	return true;
}

bool CUtility::saveToFile(const string & outputFileName, const BSTR bstrData)
{
	LPBYTE convertedData = NULL;
	size_t dataSize;
	bool bRslt = convertBSTRtoMultiByteArray(bstrData, &convertedData, dataSize);
	if (!bRslt || dataSize <= 0) {
		return false;
	}

	HANDLE f = ::CreateFile(outputFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (f == INVALID_HANDLE_VALUE) {
		return false;
	}

	DWORD writeSize = 0;
	// BSTRの末尾にある'\0'は出力しない
	if (convertedData[dataSize - 1] == '\0') {
		dataSize--;
	}
	BOOL boolRslt = ::WriteFile(f, convertedData, dataSize, &writeSize, NULL);
	freeByteArray(convertedData);
	if (!boolRslt) {
		::CloseHandle(f);
		return false;
	}

	::CloseHandle(f);

	return true;
}
bool CUtility::loadFile(const string &inputFileName, BSTR &data, size_t &dataSize)
{
	HANDLE f = ::CreateFile(inputFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (f == INVALID_HANDLE_VALUE) {
		return false;
	}
	DWORD fileSize;
	DWORD fileHightSize;
	fileSize = ::GetFileSize(f, &fileHightSize);

	LPBYTE pByteData = new BYTE[fileSize + 1];
	ZeroMemory(pByteData, fileSize + 1);
	BOOL bRslt = ::ReadFile(f, pByteData, fileSize, NULL, NULL);
	if (!bRslt) {
		delete pByteData;
		::CloseHandle(f);
		return false;
	}

	data = _bstr_t((char*)pByteData).copy();
	delete pByteData;
	dataSize = fileSize;

	::CloseHandle(f);

	return true;
}


void CUtility::freeByteArray(LPBYTE outputData)
{
	delete outputData;
}

