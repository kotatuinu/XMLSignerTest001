#include "stdafx.h"
#include "CppUnitTest.h"
#include "XMLSigner.h"
#include "XMLReader.h"
#include "Utility.h"
#include "comutil.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	//bool SetCertification(string pkcs12File, string pwd);
	TEST_CLASS(UnitTest_CXMLSigner_SetCertification)
	{
	public:
		TEST_METHOD_INITIALIZE(method_init) {
			CoInitialize(NULL);
		}
		TEST_METHOD_CLEANUP(method_cleanup) {
			CoUninitialize();
		}

		TEST_METHOD(証明書読込_正常ケース)
		{
			bool bRslt = false;
			try {
				std::string pkcs12 = "my-identity.p12";
				std::string pwd = "1234";
				CXMLSigner *obj = new CXMLSigner();
				bRslt = obj->SetCertification(pkcs12, pwd);
				Assert::IsTrue(bRslt);

				delete obj;
			}
			catch (...) {
				Assert::Fail();
			}
		}
	};
	//bool Sign(const BSTR XMLData, const _bstr_t &bstrNodeId, const _bstr_t &bstrUserId, BSTR &signedXMLData);
	TEST_CLASS(UnitTest_CXMLSigner_Sign)
	{
	public:
		TEST_METHOD_INITIALIZE(method_init) {
			CoInitialize(NULL);
		}
		TEST_METHOD_CLEANUP(method_cleanup) {
			CoUninitialize();
		}

		TEST_METHOD(署名付与_正常ケース)
		{
			bool bRslt = false;
			try {
				std::string pkcs12 = "my-identity.p12";
				std::string pwd = "1234";
				CXMLSigner *obj = new CXMLSigner();
				bRslt = obj->SetCertification(pkcs12, pwd);

				BSTR xmlData = L"<A id=\"a\"><B id=\"b\">test</B></A>";
				BSTR signedXmlData;
				bRslt = obj->Sign(xmlData, "a", "test", signedXmlData);
				Assert::IsTrue(bRslt);

				delete obj;

				vector<BSTR> value;
				CXMLReader *xmlReader = new CXMLReader();
				bRslt = xmlReader->loadXML(signedXmlData);
				Assert::IsTrue(bRslt);
				xmlReader->setParameter("SelectionNamespaces", "xmlns:dsig=\"http://www.w3.org/2000/09/xmldsig#\"");
				bRslt = xmlReader->getXPathValue("/A/dsig:Signature", value);
				Assert::IsTrue(value.size() == 1);
				Assert::IsTrue(wcscmp(value[0], L"dsig:Signature") == 0);
				for (auto ite = value.begin(); ite != value.end(); ite++) {
					SysFreeString(*ite);
				}
				value.clear();


				delete xmlReader;

			}
			catch (...) {
				Assert::Fail();
			}
		}
	};
}