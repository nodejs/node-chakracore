#ifndef SRC_NODE_BUFFER_H_
#define SRC_NODE_BUFFER_H_

#include "node.h"
#include "v8.h"

namespace node {

extern bool zero_fill_all_buffers;

namespace Buffer {

static const unsigned int kMaxLength =
    sizeof(int32_t) == sizeof(intptr_t) ? 0x3fffffff : 0x7fffffff;

NODE_EXTERN typedef void (*FreeCallback)(char* data, void* hint);

NODE_EXTERN bool HasInstance(v8::Local<v8::Value> val);
NODE_EXTERN bool HasInstance(v8::Local<v8::Object> val);
NODE_EXTERN char* Data(v8::Local<v8::Value> val);
NODE_EXTERN char* Data(v8::Local<v8::Object> val);
#if ENABLE_TTD_NODE
NODE_EXTERN void TTDAsyncModRegister(v8::Local<v8::Object> val, byte* initialModPosition);
NODE_EXTERN void TTDAsyncModNotify(byte* finalModPosition);
NODE_EXTERN void TTDSyncDataModNotify(v8::Local<v8::Object> val, UINT32 index, UINT32 count);
#endif
NODE_EXTERN size_t Length(v8::Local<v8::Value> val);
NODE_EXTERN size_t Length(v8::Local<v8::Object> val);
#if ENABLE_TTD_NODE
//Notify us that a native buffer access (which we don't currently understand/support) happened.
#define TTD_NATIVE_BUFFER_ACCESS_NOTIFY(X) fprintf(stderr, "Unsupported raw buffer access %s -- investigate this!!!\n", X)
#else
#define TTD_NATIVE_BUFFER_ACCESS_NOTIFY()
#endif

// public constructor - data is copied
NODE_EXTERN v8::MaybeLocal<v8::Object> Copy(v8::Isolate* isolate,
                                            const char* data,
                                            size_t len);

// public constructor
NODE_EXTERN v8::MaybeLocal<v8::Object> New(v8::Isolate* isolate, size_t length);

// public constructor from string
NODE_EXTERN v8::MaybeLocal<v8::Object> New(v8::Isolate* isolate,
                                           v8::Local<v8::String> string,
                                           enum encoding enc = UTF8);

// public constructor - data is used, callback is passed data on object gc
NODE_EXTERN v8::MaybeLocal<v8::Object> New(v8::Isolate* isolate,
                                           char* data,
                                           size_t length,
                                           FreeCallback callback,
                                           void* hint);

// public constructor - data is used.
NODE_EXTERN v8::MaybeLocal<v8::Object> New(v8::Isolate* isolate,
                                           char* data,
                                           size_t len);

// This is verbose to be explicit with inline commenting
static inline bool IsWithinBounds(size_t off, size_t len, size_t max) {
  // Asking to seek too far into the buffer
  // check to avoid wrapping in subsequent subtraction
  if (off > max)
    return false;

  // Asking for more than is left over in the buffer
  if (max - off < len)
    return false;

  // Otherwise we're in bounds
  return true;
}

}  // namespace Buffer
}  // namespace node

#endif  // SRC_NODE_BUFFER_H_
