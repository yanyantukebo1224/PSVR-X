#ifdef _WIN32

#include <windows.h>
#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wrl/client.h>

#include <iomanip>
#include <iostream>
#include <string>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "ole32.lib")

using Microsoft::WRL::ComPtr;

static std::wstring read_string(IMFAttributes* attributes, REFGUID key) {
    WCHAR* value = nullptr;
    UINT32 length = 0;
    if (FAILED(attributes->GetAllocatedString(key, &value, &length))) {
        return L"(unknown)";
    }
    std::wstring result(value, length);
    CoTaskMemFree(value);
    return result;
}

static std::wstring subtype_name(const GUID& subtype) {
    if (subtype == MFVideoFormat_YUY2) return L"YUY2";
    if (subtype == MFVideoFormat_NV12) return L"NV12";
    if (subtype == MFVideoFormat_MJPG) return L"MJPG";
    if (subtype == MFVideoFormat_RGB32) return L"RGB32";
    if (subtype == MFVideoFormat_RGB24) return L"RGB24";
    if (subtype == MFVideoFormat_UYVY) return L"UYVY";
    return L"(other GUID)";
}

static void print_media_types(IMFMediaSource* source) {
    ComPtr<IMFAttributes> reader_attributes;
    HRESULT hr = MFCreateAttributes(&reader_attributes, 1);
    if (FAILED(hr)) {
        std::wcerr << L"MFCreateAttributes failed: 0x" << std::hex << hr << L"\n";
        return;
    }

    ComPtr<IMFSourceReader> reader;
    hr = MFCreateSourceReaderFromMediaSource(source, reader_attributes.Get(), &reader);
    if (FAILED(hr)) {
        std::wcerr << L"MFCreateSourceReaderFromMediaSource failed: 0x"
                   << std::hex << hr << L"\n";
        return;
    }

    std::wcout << L"  Native media types:\n";
    for (DWORD index = 0;; ++index) {
        ComPtr<IMFMediaType> type;
        hr = reader->GetNativeMediaType(
            static_cast<DWORD>(MF_SOURCE_READER_FIRST_VIDEO_STREAM), index, &type);
        if (hr == MF_E_NO_MORE_TYPES) break;
        if (FAILED(hr)) {
            std::wcerr << L"    GetNativeMediaType failed: 0x"
                       << std::hex << hr << L"\n";
            break;
        }

        GUID subtype{};
        UINT32 width = 0, height = 0;
        UINT32 fps_num = 0, fps_den = 1;
        type->GetGUID(MF_MT_SUBTYPE, &subtype);
        MFGetAttributeSize(type.Get(), MF_MT_FRAME_SIZE, &width, &height);
        MFGetAttributeRatio(type.Get(), MF_MT_FRAME_RATE, &fps_num, &fps_den);

        const double fps = fps_den
            ? static_cast<double>(fps_num) / static_cast<double>(fps_den)
            : 0.0;

        std::wcout << L"    [" << index << L"] "
                   << width << L"x" << height
                   << L"  " << subtype_name(subtype)
                   << L"  " << fps_num << L"/" << fps_den
                   << L" (" << std::fixed << std::setprecision(3)
                   << fps << L" fps)\n";
    }
}

int wmain() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeEx failed: 0x" << std::hex << hr << L"\n";
        return 1;
    }

    hr = MFStartup(MF_VERSION);
    if (FAILED(hr)) {
        std::wcerr << L"MFStartup failed: 0x" << std::hex << hr << L"\n";
        CoUninitialize();
        return 1;
    }

    ComPtr<IMFAttributes> attributes;
    hr = MFCreateAttributes(&attributes, 1);
    if (SUCCEEDED(hr)) {
        hr = attributes->SetGUID(
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
            MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    }

    IMFActivate** devices = nullptr;
    UINT32 count = 0;
    if (SUCCEEDED(hr)) {
        hr = MFEnumDeviceSources(attributes.Get(), &devices, &count);
    }

    if (FAILED(hr)) {
        std::wcerr << L"Camera enumeration failed: 0x" << std::hex << hr << L"\n";
        MFShutdown();
        CoUninitialize();
        return 1;
    }

    std::wcout << L"Media Foundation video devices: " << count << L"\n";
    for (UINT32 i = 0; i < count; ++i) {
        std::wcout << L"\n[" << i << L"] "
                   << read_string(devices[i], MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME)
                   << L"\n";
        std::wcout << L"  Symbolic link: "
                   << read_string(devices[i], MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK)
                   << L"\n";

        ComPtr<IMFMediaSource> source;
        hr = devices[i]->ActivateObject(IID_PPV_ARGS(&source));
        if (SUCCEEDED(hr)) {
            print_media_types(source.Get());
            source->Shutdown();
        } else {
            std::wcerr << L"  ActivateObject failed: 0x" << std::hex << hr << L"\n";
        }
        devices[i]->Release();
    }

    CoTaskMemFree(devices);
    MFShutdown();
    CoUninitialize();
    return 0;
}

#else
#error This diagnostic tool is Windows-only.
#endif
