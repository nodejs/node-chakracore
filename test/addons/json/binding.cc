#include <node.h>
#include <v8.h>

namespace {

void Stringify(const v8::FunctionCallbackInfo<v8::Value>& args) {
  auto result = v8::JSON::Stringify(args.GetIsolate()->GetCurrentContext(),
                                    args[0].As<v8::Object>(),
                                    args[1].As<v8::String>());
  args.GetReturnValue().Set(result.ToLocalChecked());
}

void Parse(const v8::FunctionCallbackInfo<v8::Value>& args) {
  auto result = v8::JSON::Parse(args.GetIsolate()->GetCurrentContext(),
                                args[0].As<v8::String>());
  args.GetReturnValue().Set(result.ToLocalChecked());
}

inline void Initialize(v8::Local<v8::Object> binding) {
  NODE_SET_METHOD(binding, "stringify", Stringify);
  NODE_SET_METHOD(binding, "parse", Parse);
}

NODE_MODULE(binding, Initialize)

}  // anonymous namespace
