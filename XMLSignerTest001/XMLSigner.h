#pragma once
#include <Windows.h>
#include <string>
#include <comdef.h>

using namespace std;

class CXMLSigner
{
private:
	IDispatchPtr m_CLCXtxSigner;

public:
	CXMLSigner();
	~CXMLSigner();

	bool SetCertification(string pkcs12File, string pwd);
	bool Sign(const BSTR XMLData, const _bstr_t &bstrNodeId, const _bstr_t &bstrUserId, BSTR &signedXMLData);
};

