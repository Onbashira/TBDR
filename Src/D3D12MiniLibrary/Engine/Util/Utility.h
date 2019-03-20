#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>

namespace K3D12::Util{

	//256バイトアライメント
	inline size_t Alignment256Bytes(size_t size) {
		if (size == 256) {
			return 256;
		}
		size_t ret = (size&(~0xff))+0x0100;
		return ret;
	};

	//128バイトアライメント
	inline size_t Alignment128Bytes(size_t size) {

	};

	//16バイトアライメント
	inline size_t Alignment16Bytes(size_t size) {

	};

	//4バイトアライメント
	inline size_t Alignment4Bytes(size_t size) {

	};

	//2バイトアライメント
	inline size_t Alignmentt2Bytes(size_t size) {

	};

	//引数の文字列から拡張子抽出
	inline std::string ExtensionExtruction(std::string str)
	{
		size_t  dotPos = str.rfind('.');
		size_t cutNum = str.length() - dotPos;
		std::string ext = str.substr(dotPos + 1, cutNum);
		return ext;
	}

	//std::wstring -> std::string
	inline std::string WStringToString(std::wstring wstr) {
		// wstring → SJIS
		int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str()
			, -1, (char *)NULL, 0, NULL, NULL);

		// バッファの取得
		char* cpMultiByte = new char[iBufferSize];

		// wstring → SJIS
		WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, cpMultiByte
			, iBufferSize, NULL, NULL);

		// stringの生成
		std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

		// バッファの破棄
		delete[] cpMultiByte;

		// 変換結果を返す
		return(oRet);
	
	};

	//std::string -> std::wstring
	inline std::wstring StringToWString(const std::string& refSrc)
	{
		std::vector<wchar_t> buffer(MultiByteToWideChar(CP_ACP, 0, refSrc.c_str(), -1, nullptr, 0));
		MultiByteToWideChar(CP_ACP, 0, refSrc.c_str(), -1, &buffer.front(), static_cast<int>(buffer.size()));
		return std::wstring(buffer.begin(), buffer.end());
	}

	//現在モデルパスからTextureパスまでの相対パスを取得
	static std::string GetRelativeTexturePath(std::string & modelPath, std::string texturePath)
	{
		size_t pathIndex1 = modelPath.rfind('/');

		std::string textureTempPath = modelPath.substr(0, pathIndex1);
		textureTempPath += '/';
		textureTempPath += texturePath;

		return textureTempPath;
	}

	//レガシーなFopen・Fclose・Fread
	namespace FileStream {

		//ファイルを開く
		inline HRESULT FileOpen(std::string path, std::string mode, FILE** stream) {
			errno_t error = 0;
			error = fopen_s(stream, path.c_str(), mode.c_str());
			if (error != 0) {
				char* errorStr = {};
				strerror_s(errorStr, 256, error);
				if (errorStr != nullptr) {
					printf_s("print error string by strerror  : %s\n", errorStr);
				}
				return E_FAIL;
			}
			return S_OK;
		};

		//ファイルを閉じる
		inline void FileClose(FILE* stream) {
			fclose(stream);
		};

		//ファイルを読み込む
		template < typename T > inline HRESULT FileRead(void* buffer, size_t elementNum, FILE* stream) {
			size_t refCount = fread_s(buffer, sizeof(&buffer), sizeof(T), elementNum, stream);
			if (refCount < 0)
				return E_FAIL;
			return S_OK;
		};

		//ファイルを読み込む
		template < typename T > inline HRESULT FileRead(void* buffer, size_t bufferSize, size_t elementNum, FILE* stream) {
			size_t refCount = fread_s(buffer, bufferSize, sizeof(T), elementNum, stream);
			if (refCount < 0)
				return E_FAIL;
			return S_OK;
		};

		template <class T = void*> size_t FileRead(void* buffer, FILE* stream, size_t size = 0, size_t readSize = 1) {

			size_t inSize = sizeof(T);
			size_t returnValue = -1;
			FILE&  streamRef = *stream;

			if (size == 0) {
				returnValue = std::fread(buffer, sizeof(T), readSize, &streamRef);
			}
			else {
				returnValue = std::fread(buffer, size, readSize, &streamRef);
			}
			return returnValue;
		}
	}

}	


