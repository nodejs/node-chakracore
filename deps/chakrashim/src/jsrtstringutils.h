// Copyright Microsoft. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

namespace jsrt {
// Internal class for converting strings to Wide Chars and vice versa
class StringConvert {
 public:

// xplat-todo: The string utilities in this file uses Windows only APIs. Need to
// implement differently for cross-platform.
#ifdef _WIN32
  static JsErrorCode ToChar(const wchar_t *str,
                            const size_t length,
                            char* buffer,
                            const size_t size) {
    return InternalToChar(str, length, CP_ACP, buffer, size);
  }

  static JsErrorCode ToUTF8Char(const wchar_t *str,
                                const size_t length,
                                char* buffer,
                                const size_t size,
                                size_t *bytesWritten = nullptr,
                                size_t *charsWrittern = nullptr) {
    return InternalToChar(
      str, length, CP_UTF8, buffer, size, bytesWritten, charsWrittern);
  }

  static JsErrorCode ToWChar(const char *str,
                             const size_t length,
                             wchar_t* buffer,
                             const size_t size,
                             size_t *charsWritten) {
    return InternalToWChar(str, length, CP_UTF8, buffer, size, charsWritten);
  }

  static JsErrorCode UTF8CharLength(
      const wchar_t *str, const size_t length, size_t *utf8Length) {
    return GetCharLength(str, length, CP_UTF8, utf8Length);
  }
#endif  // _WIN32

  template <class SrcChar, class DstChar>
  static JsErrorCode CopyRaw(const SrcChar* src,
                             size_t length,
                             DstChar* dst,
                             size_t size,
                             size_t *charsWrittern = nullptr) {
    size_t count = min(length, size);
    InternalCopyRaw(src, dst, count);
    if (charsWrittern != nullptr) {
      *charsWrittern = count;
    }
    return JsNoError;
  }

 private:
  template <class SrcChar, class DstChar>
  static DstChar CastRaw(SrcChar ch) {
    return static_cast<DstChar>(ch);
  }

  template <class SrcChar, class DstChar>
  static void InternalCopyRaw(const SrcChar* src, DstChar* dst, size_t count) {
    const SrcChar* end = src + count;
    while (src < end) {
      *dst++ = CastRaw<SrcChar, DstChar>(*src++);
    }
  }

  static JsErrorCode GetCharLength(const wchar_t *str,
                                   const size_t length,
                                   const int code,
                                   size_t *utf8Length);
  static JsErrorCode InternalToChar(const wchar_t *str,
                                    const size_t length,
                                    const int code,
                                    char* buffer,
                                    size_t bufferSize,
                                    size_t *bytesWritten = nullptr,
                                    size_t *charsWritten = nullptr);
  static JsErrorCode InternalToWChar(const char *str,
                                     const size_t length,
                                     const int code,
                                     wchar_t* buffer,
                                     size_t size,
                                     size_t *charsWritten);
};

template <>
inline wchar_t StringConvert::CastRaw<char, wchar_t>(char ch) {
  return static_cast<wchar_t>(static_cast<uint8_t>(ch));
}

template <>
inline void StringConvert::InternalCopyRaw<wchar_t, wchar_t>(
    const wchar_t* src, wchar_t* dst, size_t count) {
  memmove(dst, src, sizeof(wchar_t) * count);
}

template <>
inline void StringConvert::InternalCopyRaw<char, char>(
    const char* src, char* dst, size_t count) {
  memmove(dst, src, sizeof(char) * count);
}

};
