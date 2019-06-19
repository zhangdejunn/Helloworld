// raw12ToBmp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include <unistd.h>
#include <stdint.h>
#include <windows.h>
#include <iostream>
using namespace std;

int saveRawToBmp(uint8_t *pBuf, int32_t width, int32_t height, const char *pFileName);

int _tmain(int argc, _TCHAR* argv[])
{
	int32_t width	= 1928;
	int32_t height	= 1208;
	uint8_t *pBuf = new uint8_t[width * height*2];
	memset(pBuf, 0, width * height);

	FILE *pRF = fopen("D:\\savevideo\\aaa.raw", "r");
	if (NULL == pRF)
	{
		return 1;
	}
	fread(pBuf, width * height*2, sizeof(uint8_t), pRF);
	fclose(pRF);

	saveRawToBmp(pBuf, width, height, "D:\\savevideo\\bbb.bmp");


	Sleep(2000);
	return 0;
}

int saveRawToBmp(uint8_t *pBuf, int32_t width, int32_t height, const char *pFileName)
{
	char heard[54] = { \
		0x42,0x4d,0x30,0x0C,0x01,0x00,0x00,0x00,0x00,0x00,0x36,0x04,0x00,0x00,			//tagBITMAPFILEHEADER	14 byte
		0x00,0x28,0x00,0x00,0xF5,0x00,0x00,0x00,0x46,0x00,		//tagBITMAPFILEHEADER	40 byte
		0x00,0x00,0x00,0x01,0x00,0x24,0x00,0x00,0x00,0x00,	
		0xF8,0x0b,0x01,0x00,0x12,0x0b,0x00,0x00,0x12,0x0b,	
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	//char patte[1024] = {0};
	char end[2] = {0, 0};
	char color = 0;
	int32_t w = width;
	int32_t h = height;
	int32_t size = w * h;
	int32_t allsize = size + 54;
	int32_t pos = 0;
	int32_t idx = 0;
	int32_t ret = 0;
	FILE *pFp = NULL;

	do
	{
		if ((NULL == pBuf) || (NULL == pFileName))
		{
			ret = -1;
			break;
		}
		if ((width < 0) || (height < 0))
		{
			ret = -2;
			break;
		}

		heard[2] = allsize & 0xFF;	//tagBITMAPFILEHEADER bfSize 文件大小
		heard[3] = (allsize >> 8) & 0xFF;
		heard[4] = (allsize >> 16) & 0xFF;
		heard[5] = (allsize >> 24) & 0xFF;

		heard[18] = w & 0xFF;	//tagBITMAPINFOHEADER biWidth 
		heard[19] = (w >> 8) & 0xFF;
		heard[20] = (w >> 16) & 0xFF;
		heard[21] = (w >> 24) & 0xFF;

		heard[22] = h & 0xFF;	//tagBITMAPINFOHEADER biHeight
		heard[23] = (h >> 8) & 0xFF;
		heard[24] = (h >> 16)  & 0xFF;
		heard[25] = (h >> 24) & 0xFF;

		allsize -= 54;
		heard[34] = allsize & 0xFF;
		heard[35] = (allsize >> 8) & 0xFF;
		heard[36] = (allsize >>  16) & 0xFF;
		heard[37] = (allsize >> 24) &  0xFF;

		//for (idx = 0; idx < 1024; idx += 4)
		//{
		//	patte[pos++] = color;
		//	patte[pos++] = color;
		//	patte[pos++] = color;
		//	patte[pos++] = 0;
		//	color++;
		//}

		//====== Write raw data ======
		pFp = fopen(pFileName, "wb");
		if (NULL == pFp)
		{
			ret = -3;
			break;
		}

		fwrite(heard, 1, sizeof(heard), pFp);
		//fwrite(patte, 1, sizeof(patte), pFp);
#if 1
		for (idx = height - 1; idx >= 0; idx--)
		{
			fwrite(&pBuf[idx * width], sizeof(uint8_t) * width, 1, pFp);
		}
#else
		fwrite(buf, 1, size, pFp);
#endif
		fwrite(end, 1, sizeof(end), pFp);

		fflush(pFp);
		//if (0 != fsync(fileno(pFp)))
		//{
		//	ret = -4;
		//	break;
		//}
	} while(0);

	if (NULL != pFp)
	{
		fclose(pFp);
		pFp = NULL;
	}

	return ret;
}

//int saveRawToBmp(uint8_t *pBuf, int32_t width, int32_t height, const char *pFileName)	//raw8 to Graybmp
//{
// 	char heard[54] = { \
// 		0x42,0x4d,0x30,0x0C,0x01,0x00,0x00,0x00,0x00,0x00,0x36,0x04,0x00,0x00,	\		//tagBITMAPFILEHEADER	14 byte
// 		0x28,0x00,0x00,0x00,0xF5,0x00,0x00,0x00,0x46,0x00,	\	//tagBITMAPFILEHEADER	40 byte
// 		0x00,0x00,0x01,0x00,0x08,0x00,0x00,0x00,0x00,0x00,	\
// 		0xF8,0x0b,0x01,0x00,0x12,0x0b,0x00,0x00,0x12,0x0b,	\
// 		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
//	char patte[1024] = {0};
//	char end[2] = {0, 0};
//	char color = 0;
//	int32_t w = width;
//	int32_t h = height;
//	int32_t size = w * h;
//	int32_t allsize = size + 1080;
//	int32_t pos = 0;
//	int32_t idx = 0;
//	int32_t ret = 0;
//	FILE *pFp = NULL;
//
//	do
//	{
//		if ((NULL == pBuf) || (NULL == pFileName))
//		{
//			ret = -1;
//			break;
//		}
//		if ((width < 0) || (height < 0))
//		{
//			ret = -2;
//			break;
//		}
//
//		heard[2] = allsize & 0xFF;	//tagBITMAPFILEHEADER bfSize 文件大小
//		heard[3] = (allsize >> 8) & 0xFF;
//		heard[4] = (allsize >> 16) & 0xFF;
//		heard[5] = (allsize >> 24) & 0xFF;
//
//		heard[18] = w & 0xFF;	//tagBITMAPINFOHEADER biWidth 
//		heard[19] = (w >> 8) & 0xFF;
//		heard[20] = (w >> 16) & 0xFF;
//		heard[21] = (w >> 24) & 0xFF;
//
//		heard[22] = h & 0xFF;	//tagBITMAPINFOHEADER biHeight
//		heard[23] = (h >> 8) & 0xFF;
//		heard[24] = (h >> 16)  & 0xFF;
//		heard[25] = (h >> 24) & 0xFF;
//
//		allsize -= 1078;
//		heard[34] = allsize & 0xFF;
//		heard[35] = (allsize >> 8) & 0xFF;
//		heard[36] = (allsize >>  16) & 0xFF;
//		heard[37] = (allsize >> 24) &  0xFF;
//
//		for (idx = 0; idx < 1024; idx += 4)
//		{
//			patte[pos++] = color;
//			patte[pos++] = color;
//			patte[pos++] = color;
//			patte[pos++] = 0;
//			color++;
//		}
//
//		//====== Write raw data ======
//		pFp = fopen(pFileName, "wb");
//		if (NULL == pFp)
//		{
//			ret = -3;
//			break;
//		}
//
//		fwrite(heard, 1, sizeof(heard), pFp);
//		fwrite(patte, 1, sizeof(patte), pFp);
//#if 1
//		for (idx = height - 1; idx >= 0; idx--)
//		{
//			fwrite(&pBuf[idx * width], sizeof(uint8_t) * width, 1, pFp);
//		}
//#else
//		fwrite(buf, 1, size, pFp);
//#endif
//		fwrite(end, 1, sizeof(end), pFp);
//
//		fflush(pFp);
//		//if (0 != fsync(fileno(pFp)))
//		//{
//		//	ret = -4;
//		//	break;
//		//}
//	} while(0);
//
//	if (NULL != pFp)
//	{
//		fclose(pFp);
//		pFp = NULL;
//	}
//
//	return ret;
//}
