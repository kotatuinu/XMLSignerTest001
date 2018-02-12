#include "stdafx.h"
#include "XMLSigner.h"
#include "Utility.h"

//#import "C:\\Program Files\\etax\\CLXtxSigner.dll"	// Windows 32bit
#import "C:\\Program Files (x86)\\etax\\CLXtxSigner.dll"	// Windows 64bit
using namespace CLXTXSIGNERLib;


CXMLSigner::CXMLSigner() :
	m_CLCXtxSigner(NULL)
{
}


CXMLSigner::~CXMLSigner()
{
	m_CLCXtxSigner = NULL;
}

bool CXMLSigner::SetCertification(string pkcs12File, string pwd)
{
	bool bRslt = false;

	CLISignature2Ptr pSigner;
	try {
		HRESULT hr = pSigner.CreateInstance(__uuidof(CLCXtxSigner));

		CLICertificateInfo2Ptr pCert = pSigner->SetCertificate(_bstr_t(pkcs12File.c_str()), _bstr_t(pwd.c_str()));

		_variant_t vRslt = pCert->IsValid();
		bRslt = (vRslt.boolVal == VARIANT_TRUE);
		if (bRslt) {
			m_CLCXtxSigner = (IDispatchPtr)pSigner;
		}
	}
	catch (_com_error &e) {
		bRslt = false;
	}

	return bRslt;
}

bool CXMLSigner::Sign(const BSTR XMLData, const _bstr_t &bstrNodeId, const _bstr_t &bstrUserId, BSTR &signedXMLData)
{
	bool bRslt = false;
	if (m_CLCXtxSigner == NULL) {
		return false;
	}
	CLIXtxSigner2Ptr pSigner = NULL;
	try {
		pSigner = m_CLCXtxSigner;
		_variant_t varCertRegData;
		VARIANT varSignedCertRegData;

		VariantInit(&varSignedCertRegData);
		bRslt = CUtility::convertBSTRtoVARIANT(XMLData, varCertRegData);

		HRESULT hr = pSigner->SignFromData(varCertRegData, &varSignedCertRegData, bstrNodeId, bstrUserId);
		if (FAILED(hr)) {
			return false;
		}

		BSTR signedXml;
		CUtility::convertVARIANTtoBSTR(varSignedCertRegData, signedXml);

		signedXMLData = signedXml;
		bRslt = true;
	}
	catch (_com_error &e) {
		bRslt = false;
	}

	return bRslt;
}
