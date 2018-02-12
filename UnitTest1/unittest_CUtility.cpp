#include "stdafx.h"
#include "CppUnitTest.h"
#include "Utility.h"
#include "comutil.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest_CUtility_convertBSTRtoVARIANT)
	{
	public:

		TEST_METHOD(BSTR－VARIANT型BYTE配列変換_正常ケース)
		{
			bool bRslt = false;
			BSTR inputData = L"test";
			VARIANT outputData;
			VariantInit(&outputData);

			bRslt = CUtility::convertBSTRtoVARIANT(inputData, outputData);
			Assert::IsTrue(bRslt);

			long lIndexMin = 0;
			long lIndexMax = 0;
			SafeArrayGetLBound(outputData.parray, 1, &lIndexMin);
			SafeArrayGetUBound(outputData.parray, 1, &lIndexMax);
			Assert::IsTrue(lIndexMin == 0);
			Assert::IsTrue(lIndexMax == 4);

			SAFEARRAY *psa = outputData.parray;
			void HUGEP *pvData;
			BYTE rslt[5] = {};
			HRESULT hr = SafeArrayAccessData(psa, &pvData);
			memcpy_s(rslt, sizeof(rslt), (LPBYTE)pvData, (lIndexMax - lIndexMin + 1));
			hr = SafeArrayUnaccessData(psa);
			Assert::IsTrue(rslt[0] == 't');
			Assert::IsTrue(rslt[1] == 'e');
			Assert::IsTrue(rslt[2] == 's');
			Assert::IsTrue(rslt[3] == 't');
			Assert::IsTrue(rslt[4] == '\0');

			VariantClear(&outputData);
		}
	};
	TEST_CLASS(UnitTest_CUtility_convertVARIANTtoBSTR)
	{
		public:
			TEST_METHOD(VARIANT型BYTE配列－BSTR変換_正常ケース)
		{
			VARIANT inputData;
			VariantInit(&inputData);
			SAFEARRAYBOUND rgb[1];
			rgb[0].cElements = 4;
			rgb[0].lLbound = 0;

			SAFEARRAY* psa = SafeArrayCreate(VT_UI1, 1, rgb);
			void HUGEP *pvData;
			HRESULT hr = SafeArrayAccessData(psa, &pvData);
			memcpy_s(pvData, 5, "test", 5);
			hr = SafeArrayUnaccessData(psa);

			inputData.parray = psa;
			inputData.vt = VT_ARRAY | VT_UI1;

			BSTR outputData;
			bool bRslt = CUtility::convertVARIANTtoBSTR(inputData, outputData);
			Assert::IsTrue(bRslt);

			Assert::IsTrue(wcscmp(outputData, L"test") == 0);

			VariantClear(&inputData);
			SysFreeString(outputData);
		}

	};
	TEST_CLASS(UnitTest_CUtility_convertBSTRtoMultiByteArray)
	{
	public:
		TEST_METHOD(BSTR－BYTE配列変換_正常ケース)
		{
			BSTR inputData = L"test";
			LPBYTE outputData = NULL;
			size_t dataSize = 0;
			bool bRslt = CUtility::convertBSTRtoMultiByteArray(inputData, &outputData, dataSize);

			Assert::IsTrue(bRslt);
			Assert::IsTrue(dataSize == 5);
			Assert::IsTrue(outputData[0] == 't');
			Assert::IsTrue(outputData[1] == 'e');
			Assert::IsTrue(outputData[2] == 's');
			Assert::IsTrue(outputData[3] == 't');
			Assert::IsTrue(outputData[4] == '\0');

			CUtility::freeByteArray(outputData);
		}

	};
	TEST_CLASS(UnitTest_CUtility_saveToFile)
	{
	public:
		// save to File(BSTR)
		TEST_METHOD(ファイル保存_正常ケース)
		{
			TCHAR currentDir[_MAX_PATH] = {};
			DWORD dwRslt = GetCurrentDirectory(sizeof(currentDir), currentDir);

			std::string fileName = currentDir;
			fileName += "\\test.txt";
			BSTR data = L"test";
			bool bRslt = CUtility::saveToFile(fileName, data);

			Assert::IsTrue(bRslt);
			HANDLE f = ::CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			Assert::IsTrue(f != INVALID_HANDLE_VALUE);
			DWORD fileSize = 0;
			dwRslt = ::GetFileSize(f, &fileSize);
			char readData[5] = {};
			BOOL boolRslt = ::ReadFile(f, readData, sizeof(readData), &fileSize, NULL);
			CloseHandle(f);
			Assert::IsTrue(dwRslt == 4);
			Assert::IsTrue(fileSize == 4);
			Assert::IsTrue(boolRslt == TRUE);
			Assert::IsTrue(strcmp(readData, "test") == 0);

		}
	};
	TEST_CLASS(UnitTest_CUtility_loadFile)
	{
	public:
		// load File(BYTE Arrya)
		TEST_METHOD(ファイル読込_正常ケース)
		{
			TCHAR currentDir[_MAX_PATH] = {};
			DWORD dwRslt = GetCurrentDirectory(sizeof(currentDir), currentDir);

			std::string fileName = currentDir;
			fileName += "\\test_loadtest.txt";
			BSTR data = L"";
			size_t dataSize = 0;
			bool bRslt = CUtility::loadFile(fileName, data, dataSize);
			Assert::IsTrue(bRslt);
			Assert::IsTrue(wcscmp(data, L"test") == 0);
			SysFreeString(data);
			Assert::IsTrue(dataSize == 4);
		}
		// Free Allocation Area
		TEST_METHOD(freeByteArray)
		{
			// TODO: テスト コードをここに挿入します
		}
	};
}