#if defined(_WIN32)

#include "Engine.h"

/*
 */
void CCaptureTask::SetPath(const char *pszPath)	{ 
	m_strPath = pszPath; 
}

/*
 */
bool CCaptureTask::Start() {

	m_pAVIStream = NULL;
	m_strPath = "capture.avi";
	m_pAVICompressed = NULL;
	m_pAVIFile = NULL;
	m_nFrames = 0;
	AVIFileInit();

	return true;
}

/*
 */
void CCaptureTask::Update() {

	PROFILE("CCaptureTask::Update()", 1);

	if (m_nFrames == 0)
		OpenAVI();

	HBITMAP hBitmap = CaptureScreen();

	int nSize = CVideoTask::GetPtr()->GetWidth() * CVideoTask::GetPtr()->GetHeight() * 4;
	unsigned char *pBuffer = new unsigned char[nSize];
	if (!GetBitmapBits(hBitmap, nSize, pBuffer)) {
		char szBuffer[8192];
		LogError(FormatLastError(GetLastError(), szBuffer));
	}

	int nSize2 = CVideoTask::GetPtr()->GetWidth() * CVideoTask::GetPtr()->GetHeight() * 3;
	unsigned char *pBuffer2 = new unsigned char[nSize2];

	int n = 0;
	for (int y = CVideoTask::GetPtr()->GetHeight() - 1; y >= 0; y--) {
		unsigned char *pSrc = pBuffer + y * CVideoTask::GetPtr()->GetWidth() * 4;
		for (int x = 0; x<CVideoTask::GetPtr()->GetWidth(); x++) {
			pBuffer2[n++] = *pSrc++;
			pBuffer2[n++] = *pSrc++;
			pBuffer2[n++] = *pSrc++;
			pSrc++;
		}
	}

	HRESULT hResult = AVIStreamWrite(m_pAVICompressed, m_nFrames, 1, pBuffer2, nSize2, AVIIF_KEYFRAME, NULL, NULL);
	delete pBuffer;

	DeleteObject(hBitmap);
	m_nFrames++;
}

/*
 */
void CCaptureTask::Stop() {

	CloseAVI();
	AVIFileExit();
}

/*
 */
HBITMAP CCaptureTask::CaptureScreen() {

	int nWidth = CVideoTask::GetPtr()->GetWidth();
	int nHeight = CVideoTask::GetPtr()->GetHeight();

	HDC hdcScreen = wglGetCurrentDC();
	HDC hdcCompatible = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
	HGDIOBJ hOld = SelectObject(hdcCompatible, hbmScreen);
	BitBlt(hdcCompatible, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);

	SelectObject(hdcCompatible, hOld);
	DeleteDC(hdcCompatible);

	return hbmScreen;
}

/*
 */
void CCaptureTask::OpenAVI() {

	BITMAPINFOHEADER bih;
	memset(&bih, 0, sizeof(BITMAPINFOHEADER));
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = CVideoTask::GetPtr()->GetWidth();
	bih.biHeight = CVideoTask::GetPtr()->GetHeight();
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = CVideoTask::GetPtr()->GetWidth() * CVideoTask::GetPtr()->GetHeight() * 3;

	AVISTREAMINFO strhdr;
	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType = streamtypeVIDEO;
	strhdr.dwScale = 1;
	strhdr.dwRate = 30;
	strhdr.rcFrame.right = bih.biWidth;
	strhdr.rcFrame.bottom = bih.biHeight;
	strhdr.dwQuality = -1;
	CTimerTask::GetPtr()->LockFrameRate((int)(1000.0f / 30.0f + 0.5f));

	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS *pOpts = &opts;
	memset(&opts, 0, sizeof(AVICOMPRESSOPTIONS));

	DeleteFile(m_strPath.c_str());
	HRESULT hResult = AVIFileOpen(&m_pAVIFile, m_strPath.c_str(), OF_WRITE | OF_CREATE, NULL);
	hResult = AVIFileCreateStream(m_pAVIFile, &m_pAVIStream, &strhdr);
	BOOL b = AVISaveOptions(NULL, ICMF_CHOOSE_KEYFRAME, 1, &m_pAVIStream, &pOpts);
	hResult = AVIMakeCompressedStream(&m_pAVICompressed, m_pAVIStream, &opts, NULL);
	hResult = AVIStreamSetFormat(m_pAVICompressed, 0, &bih, sizeof(bih));
}

/*
 */
void CCaptureTask::CloseAVI() {

	if (m_pAVIStream) AVIStreamRelease(m_pAVIStream);
	m_pAVIStream = NULL;
	
	if (m_pAVICompressed) AVIStreamRelease(m_pAVICompressed);
	m_pAVICompressed = NULL;
	
	if (m_pAVIFile) AVIFileRelease(m_pAVIFile);
	m_pAVIFile = NULL;
	
	CTimerTask::GetPtr()->UnlockFrameRate();
}

#endif /* _WIN32 */