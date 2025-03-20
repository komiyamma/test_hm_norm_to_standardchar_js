#include <windows.h>
#include <string>
#include <vector>

// SJISに変換して、元へともどした時、同じ文字なら、SJISに収まる文字

extern "C" __declspec(dllexport) BOOL CanConvertCP932(wchar_t* text) {
    // Shift_JISに変換
    int requiredSize = WideCharToMultiByte(932 /* CP932 */, 0, text, -1, NULL, 0, NULL, NULL);
    if (requiredSize == 0) {
        return false; // 変換失敗
    }

    std::vector<char> encodedBytes(requiredSize);
    int bytesConverted = WideCharToMultiByte(932 /* CP932 */, 0, text, -1, &encodedBytes[0], requiredSize, NULL, NULL);
    if (bytesConverted == 0) {
        return false; // 変換失敗
    }

    // Shift_JISからUTF-16LEに変換
    requiredSize = MultiByteToWideChar(932 /* CP932 */, 0, &encodedBytes[0], bytesConverted, NULL, 0);
    if (requiredSize == 0) {
        return false; // 変換失敗
    }

    std::wstring decodedText(requiredSize - 1, L'\0'); // null終端文字分を引く
    int charsConverted = MultiByteToWideChar(932 /* CP932 */, 0, &encodedBytes[0], bytesConverted, &decodedText[0], requiredSize);
    if (charsConverted == 0) {
        return false; // 変換失敗
    }

    // 元のテキストと変換したテキストを比較
    return text == decodedText;
}