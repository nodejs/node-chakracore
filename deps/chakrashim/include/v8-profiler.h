// Copyright 2010 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <v8.h>

namespace v8 {

struct HeapStatsUpdate;

// NOT IMPLEMENTED
class V8_EXPORT CpuProfiler {
 public:
  // void SetSamplingInterval(int us);
  // void StartProfiling(Handle<String> title, bool record_samples = false);
  // CpuProfile* StopProfiling(Handle<String> title);
  void SetIdle(bool is_idle) {}
};

class V8_EXPORT OutputStream {  // NOLINT
 public:
  enum WriteResult {
    kContinue = 0,
    kAbort = 1
  };
  virtual ~OutputStream() {}
  virtual void EndOfStream() = 0;
  virtual int GetChunkSize() { return 1024; }
  virtual WriteResult WriteAsciiChunk(char* data, int size) = 0;
  virtual WriteResult WriteHeapStatsChunk(HeapStatsUpdate* data, int count) {
    return kAbort;
  }
};

// NOT IMPLEMENTED
class V8_EXPORT HeapSnapshot {
 public:
  enum SerializationFormat {
    kJSON = 0  // See format description near 'Serialize' method.
  };

  void Delete() { delete this; }
  void Serialize(OutputStream* stream,
                 SerializationFormat format = kJSON) const {}
};

class V8_EXPORT ActivityControl {  // NOLINT
 public:
  enum ControlOption {
    kContinue = 0,
    kAbort = 1
  };
  virtual ~ActivityControl() {}
  virtual ControlOption ReportProgressValue(int done, int total) = 0;
};

// NOT IMPLEMENTED
class V8_EXPORT HeapProfiler {
 public:
  typedef RetainedObjectInfo *(*WrapperInfoCallback)(
    uint16_t class_id, Handle<Value> wrapper);

  class ObjectNameResolver {
   public:
    virtual const char* GetName(Local<Object> object) = 0;

   protected:
    virtual ~ObjectNameResolver() {}
  };

  const HeapSnapshot* TakeHeapSnapshot(
      ActivityControl* control = NULL,
      ObjectNameResolver* global_object_name_resolver = NULL) {
    return new HeapSnapshot();
  }

  void SetWrapperClassInfoProvider(
    uint16_t class_id, WrapperInfoCallback callback) {}
  void StartTrackingHeapObjects(bool track_allocations = false) {}
};

// NOT IMPLEMENTED
class V8_EXPORT RetainedObjectInfo {
 public:
  virtual void Dispose() = 0;
  virtual bool IsEquivalent(RetainedObjectInfo *other) = 0;
  virtual intptr_t GetHash() = 0;
  virtual const char *GetLabel() = 0;
  virtual const char *GetGroupLabel() { return nullptr; }
  virtual intptr_t GetElementCount() { return 0; }
  virtual intptr_t GetSizeInBytes() { return 0; }
};

struct HeapStatsUpdate {
  HeapStatsUpdate(uint32_t index, uint32_t count, uint32_t size)
    : index(index), count(count), size(size) { }
  uint32_t index;  // Index of the time interval that was changed.
  uint32_t count;  // New value of count field for the interval with this index.
  uint32_t size;  // New value of size field for the interval with this index.
};

}  // namespace v8
