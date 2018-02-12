#include "stdafx.h"
#include "XMLReader.h"

#import <msxml6.dll>
using namespace MSXML2;

CXMLReader::CXMLReader() :
	m_XMLDOMDocument(NULL)
{
}


CXMLReader::~CXMLReader()
{
	m_XMLDOMDocument = NULL;
}

IDispatchPtr CXMLReader::getXMLDOMDocInstance()
{
	IXMLDOMDocument2Ptr pXMLDOMDoc;
	if (m_XMLDOMDocument != NULL) {
		pXMLDOMDoc = m_XMLDOMDocument;
	}
	else {
		HRESULT hr = pXMLDOMDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if (FAILED(hr)) {
			return NULL;
		}
		m_XMLDOMDocument = pXMLDOMDoc;
	}
	return pXMLDOMDoc;
}

void CXMLReader::setParameters()
{
	IXMLDOMDocument2Ptr pXMLDOMDoc = getXMLDOMDocInstance();
	pXMLDOMDoc->async = VARIANT_FALSE;
	pXMLDOMDoc->validateOnParse = VARIANT_FALSE;
	pXMLDOMDoc->resolveExternals = VARIANT_FALSE;
	pXMLDOMDoc->preserveWhiteSpace = VARIANT_TRUE;
}

bool CXMLReader::load(const std::string &xmlFileName, BSTR &xmlData)
{
	bool bRslt = false;
	IXMLDOMDocument2Ptr pXMLDOMDoc;

	try {
		pXMLDOMDoc = CXMLReader::getXMLDOMDocInstance();
		setParameters();

		VARIANT_BOOL vRslt = pXMLDOMDoc->load(_variant_t(xmlFileName.c_str()));
		if (vRslt == VARIANT_FALSE) {
			return false;
		}
		xmlData = pXMLDOMDoc->xml.copy();
		bRslt = true;
	}
	catch (_com_error &e) {
		pXMLDOMDoc = NULL;
		bRslt = false;
	}

	return bRslt;
}

bool CXMLReader::loadXML(const BSTR xmlData)
{
	bool bRslt = false;
	IXMLDOMDocument2Ptr pXMLDOMDoc;

	try {
		pXMLDOMDoc = CXMLReader::getXMLDOMDocInstance();
		setParameters();

		VARIANT_BOOL vRslt = pXMLDOMDoc->loadXML(_bstr_t(xmlData));
		bRslt = (vRslt == VARIANT_TRUE);
	}
	catch (_com_error &e) {
		pXMLDOMDoc = NULL;
		bRslt = false;
	}

	return bRslt;
}

bool CXMLReader::setParameter(const string &name, const string &value)
{
	bool bRslt = false;
	IXMLDOMDocument2Ptr pXMLDOMDoc;

	try {
		pXMLDOMDoc = CXMLReader::getXMLDOMDocInstance();
		HRESULT hr = pXMLDOMDoc->setProperty(_bstr_t(name.c_str()), _variant_t(value.c_str()));
		bRslt = SUCCEEDED(hr);
	}
	catch (_com_error &e) {
		pXMLDOMDoc = NULL;
		bRslt = false;
	}

	return bRslt;
}

bool CXMLReader::getXPathValue(const string &XPath, vector<BSTR> &value)
{
	bool bRslt = false;

	try {
		if (m_XMLDOMDocument == NULL) {
			return false;
		}

		IXMLDOMDocument2Ptr pXMLDOMDoc = m_XMLDOMDocument;
		MSXML2::IXMLDOMNodePtr node;
		MSXML2::IXMLDOMNodeListPtr nodes = pXMLDOMDoc->selectNodes(_bstr_t(XPath.c_str()));
		if (nodes != NULL) {
			value.clear();
			long len = nodes->length;
			for (long i = 0; i < len; i++) {
				_variant_t val;
				node = nodes->item[i];
				switch (node->nodeType) {
				case MSXML2::DOMNodeType::NODE_ATTRIBUTE:
					val = node->nodeValue;
					break;
				case MSXML2::DOMNodeType::NODE_ELEMENT:
					val = node->nodeName;
					break;
				case MSXML2::DOMNodeType::NODE_TEXT:
					val = node->text;
					break;
				}
				value.push_back(_bstr_t(val).copy());
			}
		}
		bRslt = true;
	}
	catch (_com_error &e) {
		bRslt = false;
	}

	return bRslt;
}
