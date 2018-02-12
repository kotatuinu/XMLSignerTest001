#include "stdafx.h"
#include "CppUnitTest.h"
#include "XMLReader.h"
#include "Utility.h"
#include "comutil.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	//bool load(const std::string &xmlFileName, BSTR &xmlData);
	TEST_CLASS(UnitTest_CXMLReader_load)
	{
	public:
		TEST_METHOD_INITIALIZE(method_init) {
			CoInitialize(NULL);
		}
		TEST_METHOD_CLEANUP(method_cleanup) {
			CoUninitialize();
		}

		TEST_METHOD(正常ケース_読込実行1回)
		{
			bool bRslt = false;
			try {
				BSTR result;
				BSTR xmlData = L"<A id=\"a\"></A>";
				char xmlFile[] = "unittest.xml";

				CUtility::saveToFile(xmlFile, xmlData);

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->load(xmlFile, result);
				Assert::IsTrue(bRslt);
				Assert::IsTrue(wcscmp(xmlData, result) == 0);

				SysFreeString(result);
				delete obj;

				DeleteFile(xmlFile);
			}
			catch (...) {
				Assert::Fail();
			}
		}
		TEST_METHOD(正常ケース_読込実行2回実行)
		{
			bool bRslt = false;
			try {
				BSTR result;
				BSTR xmlData = L"<A id=\"a\"></A>";
				char xmlFile[] = "unittest.xml";

				CUtility::saveToFile(xmlFile, xmlData);

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->load(xmlFile, result);
				Assert::IsTrue(bRslt);
				Assert::IsTrue(wcscmp(xmlData, result) == 0);
				bRslt = obj->load(xmlFile, result);
				Assert::IsTrue(bRslt);
				Assert::IsTrue(wcscmp(xmlData, result) == 0);

				SysFreeString(result);
				delete obj;

				DeleteFile(xmlFile);
			}
			catch (...) {
				Assert::Fail();
			}
		}
	};
	//bool loadXML(const BSTR xmlData);
	TEST_CLASS(UnitTest_CXMLReader_loadXML)
	{
	public:
		TEST_METHOD_INITIALIZE(method_init) {
			CoInitialize(NULL);
		}
		TEST_METHOD_CLEANUP(method_cleanup) {
			CoUninitialize();
		}

		TEST_METHOD(正常ケース_読込実行1回)
		{
			bool bRslt = false;
			try {
				BSTR xmlData = L"<A id=\"a\"></A>";

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->loadXML(xmlData);
				Assert::IsTrue(bRslt);

				delete obj;
			}
			catch (...) {
				Assert::Fail();
			}
		}
		TEST_METHOD(正常ケース_読込実行2回実行)
		{
			bool bRslt = false;
			try {
				BSTR xmlData = L"<A id=\"a\"></A>";

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->loadXML(xmlData);
				Assert::IsTrue(bRslt);
				bRslt = obj->loadXML(xmlData);
				Assert::IsTrue(bRslt);

				delete obj;
			}
			catch (...) {
				Assert::Fail();
			}
		}
	};
	//bool getXPathValue(const string &XPath, vector<BSTR> &value);
	TEST_CLASS(UnitTest_CXMLReader_getXPathValue)
	{
	public:
		TEST_METHOD_INITIALIZE(method_init) {
			CoInitialize(NULL);
		}
		TEST_METHOD_CLEANUP(method_cleanup) {
			CoUninitialize();
		}

		TEST_METHOD(ファイル読込_ノード名取得_正常ケース)
		{
			bool bRslt = false;
			try {
				BSTR result;
				BSTR xmlData = L"<A id=\"a\"><B id=\"b\">test</B></A>";
				char xmlFile[] = "unittest.xml";
				vector<BSTR> value;

				CUtility::saveToFile(xmlFile, xmlData);

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->load(xmlFile, result);
				bRslt = obj->getXPathValue("//*", value);
				Assert::IsTrue(value.size() == 2);
				Assert::IsTrue(wcscmp(value[0], L"A") == 0);
				Assert::IsTrue(wcscmp(value[1], L"B") == 0);

				for (auto ite = value.begin(); ite != value.end(); ite++) {
					SysFreeString(*ite);
				}
				value.clear();
				SysFreeString(result);
				delete obj;

				DeleteFile(xmlFile);
			}
			catch (...) {
				Assert::Fail();
			}
		}
		TEST_METHOD(ファイル読込_値取得正常ケース)
		{
			bool bRslt = false;
			try {
				BSTR result;
				BSTR xmlData = L"<A id=\"a\"><B id=\"b\">test</B></A>";
				char xmlFile[] = "unittest.xml";
				vector<BSTR> value;

				CUtility::saveToFile(xmlFile, xmlData);

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->load(xmlFile, result);
				bRslt = obj->getXPathValue("//B/node()", value);
				Assert::IsTrue(value.size() == 1);
				Assert::IsTrue(wcscmp(value[0], L"test") == 0);

				for (auto ite = value.begin(); ite != value.end(); ite++) {
					SysFreeString(*ite);
				}
				value.clear();
				SysFreeString(result);
				delete obj;

				DeleteFile(xmlFile);
			}
			catch (...) {
				Assert::Fail();
			}
		}
		TEST_METHOD(ファイル読込_属性値取得正常ケース)
		{
			bool bRslt = false;
			try {
				BSTR result;
				BSTR xmlData = L"<A id=\"a\"><B id=\"b\">test</B></A>";
				char xmlFile[] = "unittest.xml";
				vector<BSTR> value;

				CUtility::saveToFile(xmlFile, xmlData);

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->load(xmlFile, result);
				bRslt = obj->getXPathValue("//@id", value);
				Assert::IsTrue(value.size() == 2);
				Assert::IsTrue(wcscmp(value[0], L"a") == 0);
				Assert::IsTrue(wcscmp(value[1], L"b") == 0);

				for (auto ite = value.begin(); ite != value.end(); ite++) {
					SysFreeString(*ite);
				}
				value.clear();
				SysFreeString(result);
				delete obj;

				DeleteFile(xmlFile);
			}
			catch (...) {
				Assert::Fail();
			}
		}

		TEST_METHOD(XMLデータ読込_ノード名取得_正常ケース)
		{
			bool bRslt = false;
			try {
				BSTR xmlData = L"<A id=\"a\"><B id=\"b\">test</B></A>";
				vector<BSTR> value;

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->loadXML(xmlData);
				bRslt = obj->getXPathValue("//*", value);
				Assert::IsTrue(value.size() == 2);
				Assert::IsTrue(wcscmp(value[0], L"A") == 0);
				Assert::IsTrue(wcscmp(value[1], L"B") == 0);

				for (auto ite = value.begin(); ite != value.end(); ite++) {
					SysFreeString(*ite);
				}
				value.clear();
				delete obj;
			}
			catch (...) {
				Assert::Fail();
			}
		}
		TEST_METHOD(XMLデータ読込_値取得正常ケース)
		{
			bool bRslt = false;
			try {
				BSTR xmlData = L"<A id=\"a\"><B id=\"b\">test</B></A>";
				vector<BSTR> value;

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->loadXML(xmlData);
				bRslt = obj->getXPathValue("//B/node()", value);
				Assert::IsTrue(value.size() == 1);
				Assert::IsTrue(wcscmp(value[0], L"test") == 0);

				for (auto ite = value.begin(); ite != value.end(); ite++) {
					SysFreeString(*ite);
				}
				value.clear();
				delete obj;
			}
			catch (...) {
				Assert::Fail();
			}
		}
		TEST_METHOD(XMLデータ読込_属性値取得正常ケース)
		{
			bool bRslt = false;
			try {
				BSTR xmlData = L"<A id=\"a\"><B id=\"b\">test</B></A>";
				vector<BSTR> value;

				CXMLReader *obj = new CXMLReader();
				bRslt = obj->loadXML(xmlData);
				bRslt = obj->getXPathValue("//@id", value);
				Assert::IsTrue(value.size() == 2);
				Assert::IsTrue(wcscmp(value[0], L"a") == 0);
				Assert::IsTrue(wcscmp(value[1], L"b") == 0);

				for (auto ite = value.begin(); ite != value.end(); ite++) {
					SysFreeString(*ite);
				}
				value.clear();
				delete obj;
			}
			catch (...) {
				Assert::Fail();
			}
		}

	};
}