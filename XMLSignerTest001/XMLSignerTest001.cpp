// XMLSignerTest001.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "XMLSigner.h"
#include "XMLReader.h"
#include "Utility.h"

int main(int argc, char *argv[])
{
	bool bRslt = false;

	if (argc != 5) {
		fprintf(
			stderr, 
			"Invalid Argments.\n"
			" Arg1:PKCS12 file.\n"
			" Arg2:PKCS12 file Password.\n"
			" Arg3:Input XMLfile.\n"
			" Arg4:output SignedXMLfile.\n"
			);
		return -1;
	}

	HRESULT hr = CoInitialize(NULL);

	BSTR xmlData;
	BSTR xmlSignedData;

	std::vector<BSTR> xmlValues;

	CXMLReader *xmlReader = new CXMLReader();
	bRslt = xmlReader->load(argv[3], xmlData);
	bRslt = xmlReader->getXPathValue("/*/@id", xmlValues);
	delete xmlReader;
	if (xmlValues.size() != 1) {
		return -1;
	}

	_bstr_t nodeid = xmlValues[0];

	CXMLSigner *signer = new CXMLSigner();
	bRslt = signer->SetCertification(string(argv[1]), (argv[2]));
	bRslt = signer->Sign(xmlData, nodeid, "test", xmlSignedData);
	delete signer;

	bRslt = CUtility::saveToFile(argv[4], xmlSignedData);
	SysFreeString(xmlData);
	SysFreeString(xmlSignedData);

	CoUninitialize();

	return 0;
}

