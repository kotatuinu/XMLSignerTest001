// XMLSignerTest001.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "XMLSigner.h"
#include "XMLReader.h"
#include "Utility.h"
#include <string.h>
#include <thread>

#include <process.h>

// s-kitaの日記「Win32API セキュリティ属性を指定してファイルを作成する」
// http://d.hatena.ne.jp/s-kita/20110310/1299763856
// EternalWindows「アクセスコントロール / DACLの設定」
// http://eternalwindows.jp/security/accesscontrol/accesscontrol05.html
#include <aclapi.h>
DWORD GetAclSize(PSID *ppSid, int nSidCount);
BOOL ConvertNameToSid(LPTSTR lpszName, PSID *ppSid);

// 署名モジュールの証明書設定と署名付与を同じサブスレッドで実行してみる：メモリが解放される？1万件でダメっぽいような・・・。
int signProcess(const string &pkcs12File, const string &pkcs12Pwd, const string &xmlFile);

// 署名モジュールの証明書設定と署名付与を別のスレッドで実行してみる：署名付与でメモリが解放されていないで、メモリ使用量が増加して減らない。
void signThread(CXMLSigner *signer, const BSTR &xmlData, const _bstr_t &nodeid);	// std::thread用
unsigned __stdcall signThread0(void *args);	// _beginthreadex用
void signThread1(void *args);	// _beginthread用


typedef struct {
	CXMLSigner *signer;
	BSTR xmlData;
	_bstr_t nodeid;
} SIGN_TREAD_ARG;

int main(int argc, char *argv[])
{
	if (argc != 4) {
		fprintf(
			stderr,
			"Invalid Argments.\n"
			" Arg1:PKCS12 file.\n"
			" Arg2:PKCS12 file Password.\n"
			" Arg3:Input XMLfile.\n"
			//" Arg4:output SignedXMLfile.\n"
			);
		return -1;
	}

	//CoInitialize(NULL);←入れ子になっていると、署名モジュールはメモリリークするクサイ。
	string pkcs12File = argv[1];
	string pkcs12Pwd = argv[2];
	string xmlFile = argv[3];
	int iRslt = 0;

	// シングルスレッドで証明書読込・署名付与を一括実行
	//iRslt = signProcess(pkcs12File, pkcs12Pwd, xmlFile);

	// 別スレッドで証明書読込・署名付与を一括実行
	std::thread th(signProcess, pkcs12File, pkcs12Pwd, xmlFile);
	th.join();

	//CoUninitialize();
	return iRslt;
}

int signProcess(const string &pkcs12File, const string &pkcs12Pwd, const string &xmlFile)
{
	printf("signProcess START.\n");

	bool bRslt = false;
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		fprintf(stderr, "CoInitialize failed [%08x]\n", hr);
		return -1;
	}
	BSTR xmlData;
	std::vector<BSTR> xmlValues;

	CXMLReader *xmlReader = new CXMLReader();
	bRslt = xmlReader->load(xmlFile, xmlData);
	bRslt = xmlReader->getXPathValue("/*/@id", xmlValues);
	delete xmlReader;
	if (xmlValues.size() != 1) {
		return -1;
	}

	_bstr_t nodeid = xmlValues[0];

	CXMLSigner *signer = new CXMLSigner();
	bRslt = signer->SetCertification(pkcs12File, pkcs12Pwd);

	// シングルスレッドで起動
	signThread(signer, xmlData, nodeid);

	// **** 別スレッドで起動すると、メモリが解放されない ****
	// std::threadからの起動
	//std::thread th(signThread, signer, xmlData, nodeid);
	//th.join();

	// _beginthreadからの起動
	//SIGN_TREAD_ARG args = {
	//	signer, xmlData, nodeid
	//};
	//HANDLE h = (HANDLE)_beginthread(signThread1, 0, (void *)&args);
	//WaitForSingleObject(h, INFINITE);
	//CloseHandle(h);

	// _beginthreadexからの起動
	//SIGN_TREAD_ARG args = {
	//	signer, xmlData, nodeid
	//};
	//unsigned thrdaddr = 0;
	//TCHAR szAccountName[256];
	//DWORD dwAccessMask = GENERIC_READ;
	//DWORD dwDaclSize;
	//DWORD dwSize;
	//PSID  pSid;
	//PACL  pDacl;
	//dwSize = sizeof(szAccountName) / sizeof(TCHAR);
	//GetUserName(szAccountName, &dwSize);
	//ConvertNameToSid(szAccountName, &pSid);
	//dwDaclSize = GetAclSize(&pSid, 1);
	//pDacl = (PACL)LocalAlloc(LPTR, dwDaclSize);
	//InitializeAcl(pDacl, dwDaclSize, ACL_REVISION);
	//AddAccessAllowedAce(pDacl, ACL_REVISION, dwAccessMask, pSid);
	//SECURITY_DESCRIPTOR sd = {};
	//InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	//SetSecurityDescriptorDacl(&sd, TRUE, pDacl, FALSE);
	//SECURITY_ATTRIBUTES sa = {};
	//sa.nLength = sizeof(sa);
	//sa.bInheritHandle = FALSE;
	//sa.lpSecurityDescriptor = &sd;
	//sa.lpSecurityDescriptor = NULL;
	//HANDLE h = (HANDLE)_beginthreadex(&sa, 0, signThread0, (void *)&args, 0, &thrdaddr);
	//WaitForSingleObject(h, INFINITE);
	//CloseHandle(h);
	//LocalFree(pSid);
	//LocalFree(pDacl);

	SysFreeString(xmlData);
	delete signer;

	CoUninitialize();

	printf("signProcess END.\n");
	return 0;
}

void signThread(CXMLSigner *signer, const BSTR &xmlData, const _bstr_t &nodeid)
{
	bool bRslt = false;
	BSTR xmlSignedData;

	//CoInitialize(NULL);

	for (long l = 1; l <= 10000; l++) {
		bRslt = signer->Sign(xmlData, nodeid, "test", xmlSignedData);
		char currentDir[_MAX_PATH] = {};
		char fileName[_MAX_PATH] = {};
		GetCurrentDirectory(sizeof(currentDir), currentDir);
		sprintf_s(fileName, sizeof(fileName), "%s\\result\\%04d.xml", currentDir, l);
		printf(fileName);
		printf("\n");

		bRslt = CUtility::saveToFile(fileName, xmlSignedData);
		SysFreeString(xmlSignedData);
	}
	//CoUninitialize();
}

unsigned __stdcall signThread0(void *args)
{
	SIGN_TREAD_ARG *signerArgs = (SIGN_TREAD_ARG *)args;
	signThread(signerArgs->signer, signerArgs->xmlData, signerArgs->nodeid);

	return 0;
}
void signThread1(void *args)
{
	SIGN_TREAD_ARG *signerArgs = (SIGN_TREAD_ARG *)args;
	signThread(signerArgs->signer, signerArgs->xmlData, signerArgs->nodeid);

}

DWORD GetAclSize(PSID *ppSid, int nSidCount)
{
	int   i;
	DWORD dwAclSize = 0;

	for (i = 0; i < nSidCount; i++) {
		dwAclSize += GetLengthSid(ppSid[i]);
		dwAclSize += sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD);
	}

	dwAclSize += sizeof(ACL);

	return dwAclSize;
}

BOOL ConvertNameToSid(LPTSTR lpszName, PSID *ppSid)
{
	TCHAR        szDomainName[256];
	DWORD        dwSizeDomain = sizeof(szDomainName) / sizeof(TCHAR);
	DWORD        dwSizeSid = 0;
	SID_NAME_USE sidName;

	LookupAccountName(NULL, lpszName, NULL, &dwSizeSid, szDomainName, &dwSizeDomain, &sidName);

	*ppSid = (PSID)LocalAlloc(LPTR, dwSizeSid);

	return LookupAccountName(NULL, lpszName, *ppSid, &dwSizeSid, szDomainName, &dwSizeDomain, &sidName);
}