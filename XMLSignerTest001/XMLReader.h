#pragma once
#include <Windows.h>
#include <comdef.h>
#include <string>
#include <vector>

using namespace std;

class CXMLReader
{
private:
	IDispatchPtr m_XMLDOMDocument;

public:
	CXMLReader();
	~CXMLReader();
	bool load(const std::string &xmlFileName, BSTR &xmlData);
	bool loadXML(const BSTR xmlData);
	bool setParameter(const string &name, const string &value);
	bool getXPathValue(const string &XPath, vector<BSTR> &value);

private:
	IDispatchPtr getXMLDOMDocInstance();
	void setParameters();

};

