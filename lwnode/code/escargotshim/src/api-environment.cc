/*
 * Copyright (c) 2021-present Samsung Electronics Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory>
#include "api.h"
#include "api/engine.h"
#include "api/utils/cast.h"
#include "base.h"
#include "init/v8.h"

using namespace Escargot;
using namespace EscargotShim;

namespace v8 {
// --- E n v i r o n m e n t ---

void v8::V8::InitializePlatform(Platform* platform) {
  /* NOTHING TO DO */
  internal::V8::InitializePlatform(platform);
}

void v8::V8::ShutdownPlatform() {
  EscargotShim::Platform::Dispose();
  internal::V8::ShutdownPlatform();
}

bool v8::V8::Initialize(const int build_config) {
  Engine::Initialize();

  return true;
}

bool V8::TryHandleSignal(int signum, void* info, void* context) {
  LWNODE_RETURN_FALSE;
}

bool TryHandleWebAssemblyTrapPosix(int sig_code,
                                   siginfo_t* info,
                                   void* context) {
  LWNODE_RETURN_FALSE;
}

bool V8::EnableWebAssemblyTrapHandler(bool use_v8_signal_handler) {
  LWNODE_UNIMPLEMENT_WORKAROUND;
  return false;
}

void v8::V8::SetEntropySource(EntropySource entropy_source) {
  LWNODE_CALL_TRACE();
  LWNODE_UNIMPLEMENT_IGNORED;
}

void v8::V8::SetReturnAddressLocationResolver(
    ReturnAddressLocationResolver return_address_resolver) {
  LWNODE_RETURN_VOID;
}

bool v8::V8::Dispose() {
  LWNODE_CALL_TRACE();
  Engine::Dispose();
  return true;
}

SharedMemoryStatistics::SharedMemoryStatistics()
    : read_only_space_size_(0),
      read_only_space_used_size_(0),
      read_only_space_physical_size_(0) {}

HeapStatistics::HeapStatistics()
    : total_heap_size_(0),
      total_heap_size_executable_(0),
      total_physical_size_(0),
      total_available_size_(0),
      used_heap_size_(0),
      heap_size_limit_(0),
      malloced_memory_(0),
      external_memory_(0),
      peak_malloced_memory_(0),
      does_zap_garbage_(false),
      number_of_native_contexts_(0),
      number_of_detached_contexts_(0) {}

HeapSpaceStatistics::HeapSpaceStatistics()
    : space_name_(nullptr),
      space_size_(0),
      space_used_size_(0),
      space_available_size_(0),
      physical_space_size_(0) {}

HeapObjectStatistics::HeapObjectStatistics()
    : object_type_(nullptr),
      object_sub_type_(nullptr),
      object_count_(0),
      object_size_(0) {}

HeapCodeStatistics::HeapCodeStatistics()
    : code_and_metadata_size_(0),
      bytecode_and_metadata_size_(0),
      external_script_source_size_(0) {}

bool v8::V8::InitializeICU(const char* icu_data_file) {
  LWNODE_RETURN_FALSE;
}

bool v8::V8::InitializeICUDefaultLocation(const char* exec_path,
                                          const char* icu_data_file) {
  LWNODE_RETURN_FALSE;
}

void v8::V8::InitializeExternalStartupData(const char* directory_path) {
  LWNODE_RETURN_VOID;
}

// static
void v8::V8::InitializeExternalStartupDataFromFile(const char* snapshot_blob) {
  LWNODE_RETURN_VOID;
}

const char* v8::V8::GetVersion() {
  return "@todo NOT DEFINED YET";
}

void V8::GetSharedMemoryStatistics(SharedMemoryStatistics* statistics) {
  LWNODE_RETURN_VOID;
}

Local<Context> NewContext(
    v8::Isolate* external_isolate,
    v8::ExtensionConfiguration* extensions,
    v8::MaybeLocal<ObjectTemplate> global_template,
    v8::MaybeLocal<Value> global_object,
    size_t context_snapshot_index,
    v8::DeserializeInternalFieldsCallback embedder_fields_deserializer,
    v8::MicrotaskQueue* microtask_queue) {
  LWNODE_RETURN_LOCAL(Context);
}

Local<Context> v8::Context::New(
    v8::Isolate* external_isolate,
    v8::ExtensionConfiguration* extensions,
    v8::MaybeLocal<ObjectTemplate> global_template,
    v8::MaybeLocal<Value> global_object,
    DeserializeInternalFieldsCallback internal_fields_deserializer,
    v8::MicrotaskQueue* microtask_queue) {
  if (extensions || !global_object.IsEmpty() || microtask_queue) {
    LWNODE_UNIMPLEMENT;
  }

  ObjectTemplateRef* esNewGlobalObjectTemplate = nullptr;

  if (!global_template.IsEmpty()) {
    ObjectTemplateRef* esGlobalTemplate =
        VAL(*global_template.ToLocalChecked())->otpl();

    if (esGlobalTemplate->has(StringRef::createFromASCII("constructor"))) {
      LWNODE_UNIMPLEMENT;
    }

    if (!global_object.IsEmpty()) {
      LWNODE_UNIMPLEMENT;
    }

    esNewGlobalObjectTemplate = esGlobalTemplate;
  }

  auto lwContext =
      ContextWrap::New(IsolateWrap::fromV8(external_isolate), extensions);

  if (esNewGlobalObjectTemplate) {
    auto esContext = lwContext->get();
    auto esGlobalObject = esContext->globalObject();
    auto esNewGlobalObject = esNewGlobalObjectTemplate->instantiate(esContext);

    // @note using setPrototype here is a workaround. it may right to install
    // all properties of esNewGlobalObjectTemplate or esNewGlobalObject onto
    // esGlobalObject.

    // @todo we may replace the below code once ObjectTemplatRef::installTo is
    // implemented.
    // e.g) esNewGlobalObjectTemplate->installTo(esContext, esGlobalObject);
    ObjectRefHelper::setPrototype(esContext, esGlobalObject, esNewGlobalObject);
  }

  return v8::Utils::NewLocal(external_isolate, lwContext);
}

MaybeLocal<Context> v8::Context::FromSnapshot(
    v8::Isolate* external_isolate,
    size_t context_snapshot_index,
    v8::DeserializeInternalFieldsCallback embedder_fields_deserializer,
    v8::ExtensionConfiguration* extensions,
    MaybeLocal<Value> global_object,
    v8::MicrotaskQueue* microtask_queue) {
  LWNODE_RETURN_LOCAL(Context);
}

MaybeLocal<Object> v8::Context::NewRemoteContext(
    v8::Isolate* external_isolate,
    v8::Local<ObjectTemplate> global_template,
    v8::MaybeLocal<v8::Value> global_object) {
  LWNODE_RETURN_LOCAL(Object);
}

void v8::Context::SetSecurityToken(Local<Value> token) {
  auto lwContext = VAL(this)->context();
  auto esToken = CVAL(*token)->value();
  lwContext->SetSecurityToken(esToken);
}

void v8::Context::UseDefaultSecurityToken() {
  auto lwContext = VAL(this)->context();
  lwContext->UseDefaultSecurityToken();
}

Local<Value> v8::Context::GetSecurityToken() {
  auto lwContext = VAL(this)->context();
  auto esToken = lwContext->GetSecurityToken();
  return Utils::NewLocal<Value>(lwContext->GetIsolate()->toV8(), esToken);
}

v8::Isolate* Context::GetIsolate() {
  return VAL(this)->context()->GetIsolate()->toV8();
}

v8::Local<v8::Object> Context::Global() {
  LWNODE_CALL_TRACE();
  auto lwContext = VAL(this)->context();
  GlobalObjectRef* esGlobalObject = lwContext->get()->globalObject();

  return Utils::NewLocal<Object>(lwContext->GetIsolate()->toV8(),
                                 esGlobalObject);
}

void Context::DetachGlobal() {
  LWNODE_RETURN_VOID;
}

Local<v8::Object> Context::GetExtrasBindingObject() {
  LWNODE_CALL_TRACE();
  auto lwContext = VAL(this)->context();
  return Utils::NewLocal<Object>(lwContext->GetIsolate()->toV8(),
                                 lwContext->GetExtrasBindingObject());
}

void Context::AllowCodeGenerationFromStrings(bool allow) {
  LWNODE_RETURN_VOID;
}

bool Context::IsCodeGenerationFromStringsAllowed() {
  LWNODE_RETURN_FALSE;
}

void Context::SetErrorMessageForCodeGenerationFromStrings(Local<String> error) {
  LWNODE_RETURN_VOID;
}

void Context::SetAbortScriptExecution(
    Context::AbortScriptExecutionCallback callback) {
  LWNODE_UNIMPLEMENT;
}

Local<Value> Context::GetContinuationPreservedEmbedderData() const {
  LWNODE_RETURN_LOCAL(Value);
}

void Context::SetContinuationPreservedEmbedderData(Local<Value> data) {
  LWNODE_UNIMPLEMENT;
}

i::Address* Context::GetDataFromSnapshotOnce(size_t index) {
  LWNODE_RETURN_NULLPTR;
}

void v8::ObjectTemplate::CheckCast(Data* that) {
  LWNODE_RETURN_VOID;
}

void v8::FunctionTemplate::CheckCast(Data* that) {
  LWNODE_RETURN_VOID;
}

void v8::Signature::CheckCast(Data* that) {
  LWNODE_RETURN_VOID;
}

void v8::AccessorSignature::CheckCast(Data* that) {
  LWNODE_RETURN_VOID;
}

Local<External> v8::External::New(Isolate* isolate, void* value) {
  API_ENTER_NO_EXCEPTION(isolate);

  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esObject = ObjectRefHelper::create(esContext);

  auto data = new ExternalObjectData();
  data->setInternalFieldCount(ExternalObjectData::kInternalFieldCount);
  data->setInternalField(ExternalObjectData::kValueSlot, value);

  ObjectRefHelper::setExtraData(esObject, data);

  return Utils::NewLocal<External>(lwIsolate->toV8(), esObject);
}

void* External::Value() const {
  auto esObject = CVAL(this)->value()->asObject();

  LWNODE_CHECK(ObjectRefHelper::getExtraData(esObject));
  auto externalObjectData =
      ObjectRefHelper::getExtraData(esObject)->asExternalObjectData();

  LWNODE_DCHECK(externalObjectData->internalFieldCount() == 1);

  return externalObjectData->internalField(0);
}

namespace {

static_assert(v8::String::kMaxLength == Constants::kMaxStringLength,
              "String max size is different");

}  // anonymous namespace

Local<String> String::NewFromUtf8Literal(Isolate* isolate,
                                         const char* literal,
                                         NewStringType type,
                                         int length) {
  MaybeLocal<String> result = NewFromUtf8(isolate, literal, type, length);
  return result.ToLocalChecked();
}

MaybeLocal<String> String::NewFromUtf8(Isolate* isolate,
                                       const char* data,
                                       NewStringType type,
                                       int length) {  // nbytes
  MaybeLocal<String> result;

  if (NewStringType::kInternalized == type) {
    // LWNODE_UNIMPLEMENT;
    // @note this isn't supported because it leads to consuming additional
    // memory.
  }

  if (length == 0) {
    result = String::Empty(isolate);
  } else if (length > v8::String::kMaxLength) {
    result = MaybeLocal<String>();
  } else {
    if (length < 0) {
      length = strLength(data);
    }
    StringRef* esSource = StringRef::createFromUTF8(data, length);
    result = Utils::NewLocal<String>(isolate, esSource);
  }

  return result;
}

MaybeLocal<String> String::NewFromOneByte(Isolate* isolate,
                                          const uint8_t* data,
                                          NewStringType type,
                                          int length) {
  MaybeLocal<String> result;

  if (NewStringType::kInternalized == type) {
    // LWNODE_UNIMPLEMENT;
    // @note this isn't supported because it leads to consuming additional
    // memory.
  }

  if (length == 0) {
    result = String::Empty(isolate);
  } else if (length > v8::String::kMaxLength) {
    result = MaybeLocal<String>();
  } else {
    if (length < 0) {
      length = strLength(data);
    }

    StringRef* esSource = StringRef::createFromLatin1(data, length);
    result = Utils::NewLocal<String>(isolate, esSource);
  }

  return result;
}

MaybeLocal<String> String::NewFromTwoByte(Isolate* isolate,
                                          const uint16_t* data,
                                          NewStringType type,
                                          int length) {
  MaybeLocal<String> result;

  if (NewStringType::kInternalized == type) {
    // LWNODE_UNIMPLEMENT;
    // @note this isn't supported because it leads to consuming additional
    // memory.
  }

  if (length == 0) {
    result = String::Empty(isolate);
  } else if (length > v8::String::kMaxLength) {
    result = MaybeLocal<String>();
  } else {
    if (length < 0) length = strLength(data);
    StringRef* esSource = StringRef::createFromUTF16(
        reinterpret_cast<const char16_t*>(data), length);
    result = Utils::NewLocal<String>(isolate, esSource);
  }

  return result;
}

static void copyStringToTwoByteArray(char16_t* dest, StringRef* src) {
  auto bufferData = src->stringBufferAccessData();
  if (bufferData.has8BitContent) {
    for (size_t i = 0; i < bufferData.length; i++) {
      dest[i] = bufferData.uncheckedCharAtFor8Bit(i);
    }
  } else {
    memcpy(dest, bufferData.buffer, bufferData.length * sizeof(char16_t));
  }
}

Local<String> v8::String::Concat(Isolate* v8_isolate,
                                 Local<String> left,
                                 Local<String> right) {
  if (left.IsEmpty() || right.IsEmpty()) {
    return Local<String>();
  }

  auto esLeftStr = VAL(*left)->value()->asString();
  auto leftStrBufferData = esLeftStr->stringBufferAccessData();

  auto esRightStr = VAL(*right)->value()->asString();
  auto rightStrBufferData = esRightStr->stringBufferAccessData();

  size_t nchars = leftStrBufferData.length + rightStrBufferData.length;

  Local<String> result;

  if (leftStrBufferData.has8BitContent && rightStrBufferData.has8BitContent) {
    unsigned char* buffer = new unsigned char[nchars];
    memcpy(buffer, leftStrBufferData.buffer, leftStrBufferData.length);
    memcpy(buffer + leftStrBufferData.length,
           rightStrBufferData.buffer,
           rightStrBufferData.length);

    result = Utils::NewLocal<String>(
        v8_isolate, StringRef::createFromLatin1(buffer, nchars));
    delete[] buffer;
  } else {
    char16_t* buffer = new char16_t[nchars];
    copyStringToTwoByteArray(buffer, esLeftStr);
    copyStringToTwoByteArray(buffer + leftStrBufferData.length, esRightStr);

    result = Utils::NewLocal<String>(
        v8_isolate, StringRef::createFromUTF16(buffer, nchars));
    delete[] buffer;
  }

  return result;
}

MaybeLocal<String> v8::String::NewExternalTwoByte(
    Isolate* isolate, v8::String::ExternalStringResource* resource) {
  LWNODE_CHECK_NOT_NULL(resource);

  if (resource->length() > static_cast<size_t>(v8::String::kMaxLength)) {
    return MaybeLocal<String>();
  }

  if (resource->length() == 0) {
    resource->Dispose();
    return Utils::NewLocal<String>(isolate, StringRef::emptyString());
  }

  LWNODE_CHECK_NOT_NULL(resource->data());

  auto esString = StringRef::createExternalFromUTF16(
      reinterpret_cast<const char16_t*>(resource->data()), resource->length());

  return Utils::NewLocal<String>(
      isolate, ExternalStringWrap::create(esString, resource));
}

MaybeLocal<String> v8::String::NewExternalOneByte(
    Isolate* isolate, v8::String::ExternalOneByteStringResource* resource) {
  LWNODE_CHECK_NOT_NULL(resource);

  if (resource->length() > static_cast<size_t>(v8::String::kMaxLength)) {
    return MaybeLocal<String>();
  }

  if (resource->length() == 0) {
    resource->Dispose();

    return Utils::NewLocal<String>(isolate, StringRef::emptyString());
  }

  LWNODE_CHECK_NOT_NULL(resource->data());

  auto externalString = StringRef::createExternalFromLatin1(
      (const unsigned char*)(resource->data()), resource->length());

  return Utils::NewLocal<String>(
      isolate, ExternalStringWrap::create(externalString, resource));
}

bool v8::String::MakeExternal(v8::String::ExternalStringResource* resource) {
  LWNODE_RETURN_FALSE;
}

bool v8::String::MakeExternal(
    v8::String::ExternalOneByteStringResource* resource) {
  LWNODE_RETURN_FALSE;
}

bool v8::String::CanMakeExternal() {
  LWNODE_RETURN_FALSE;
}

bool v8::String::StringEquals(Local<String> that) {
  auto esSelf = VAL(this)->value()->asString();
  auto esThatStr = VAL(*that)->value()->asString();
  return esSelf->equals(esThatStr);
}

Isolate* v8::Object::GetIsolate() {
  LWNODE_ONCE(LWNODE_DLOG_WARN("@ignored/multi-isolate"));
  // NOTE: there is no API yet to acquire an Isolate this object belongs to.
  return IsolateWrap::GetCurrent()->toV8();
}

Local<v8::Object> v8::Object::New(Isolate* isolate) {
  API_ENTER_NO_EXCEPTION(isolate);
  ObjectRef* esObject =
      ObjectRefHelper::create(lwIsolate->GetCurrentContext()->get());

  return Utils::NewLocal<Object>(isolate, esObject);
}

Local<v8::Object> v8::Object::New(Isolate* isolate,
                                  Local<Value> prototype_or_null,
                                  Local<Name>* names,
                                  Local<Value>* values,
                                  size_t length) {
  LWNODE_RETURN_LOCAL(Object);
}

Local<v8::Value> v8::NumberObject::New(Isolate* isolate, double value) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto esContext = lwIsolate->GetCurrentContext()->get();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState, double value) -> ValueRef* {
        auto object = NumberObjectRef::create(esState);
        object->setPrimitiveValue(esState, ValueRef::create(value));
        return object;
      },
      value);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Value>(isolate, r.result);
}

double v8::NumberObject::ValueOf() const {
  auto esValue = CVAL(this)->value();
  LWNODE_CHECK(esValue->isNumberObject());
  return esValue->asNumberObject()->primitiveValue();
}

Local<v8::Value> v8::BigIntObject::New(Isolate* isolate, int64_t value) {
  LWNODE_RETURN_LOCAL(Value);
}

Local<v8::BigInt> v8::BigIntObject::ValueOf() const {
  LWNODE_RETURN_LOCAL(BigInt);
}

Local<v8::Value> v8::BooleanObject::New(Isolate* isolate, bool value) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto esContext = lwIsolate->GetCurrentContext()->get();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState, bool value) -> ValueRef* {
        auto object = BooleanObjectRef::create(esState);
        object->setPrimitiveValue(esState, ValueRef::create(value));
        return object;
      },
      value);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Value>(isolate, r.result);
}

bool v8::BooleanObject::ValueOf() const {
  auto esValue = CVAL(this)->value();
  LWNODE_CHECK(esValue->isBooleanObject());
  return esValue->asBooleanObject()->primitiveValue();
}

Local<v8::Value> v8::StringObject::New(Isolate* v8_isolate,
                                       Local<String> value) {
  API_ENTER_NO_EXCEPTION(v8_isolate);
  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esValue = CVAL(*value)->value()->asString();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState, StringRef* esValue) -> ValueRef* {
        auto object = StringObjectRef::create(esState);
        object->setPrimitiveValue(esState, esValue);
        return object;
      },
      esValue);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Value>(v8_isolate, r.result);
}

Local<v8::String> v8::StringObject::ValueOf() const {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto esValue = CVAL(this)->value();
  LWNODE_CHECK(esValue->isStringObject());
  return Utils::NewLocal<String>(lwIsolate->toV8(),
                                 esValue->asStringObject()->primitiveValue());
}

Local<v8::Value> v8::SymbolObject::New(Isolate* isolate, Local<Symbol> value) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esValue = CVAL(*value)->value()->asSymbol();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState, SymbolRef* esValue) -> ValueRef* {
        auto object = SymbolObjectRef::create(esState);
        object->setPrimitiveValue(esState, esValue);
        return object;
      },
      esValue);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Value>(lwIsolate->toV8(), r.result);
}

Local<v8::Symbol> v8::SymbolObject::ValueOf() const {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto esValue = CVAL(this)->value();
  LWNODE_CHECK(esValue->isSymbolObject());

  return Utils::NewLocal<Symbol>(lwIsolate->toV8(),
                                 esValue->asSymbolObject()->primitiveValue());
}

MaybeLocal<v8::Value> v8::Date::New(Local<Context> context, double time) {
  LWNODE_RETURN_LOCAL(Value);
}

double v8::Date::ValueOf() const {
  LWNODE_RETURN_0;
}

MaybeLocal<v8::RegExp> v8::RegExp::New(Local<Context> context,
                                       Local<String> pattern,
                                       Flags flags) {
  API_ENTER_WITH_CONTEXT(context, MaybeLocal<RegExp>());
  auto lwContext = lwIsolate->GetCurrentContext();
  auto lwPattern = CVAL(*pattern)->value();
  int flagsValue = (int)flags;

  auto r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* esState, ValueRef* source, int flags) -> ValueRef* {
        return RegExpObjectRef::create(
            esState, source, (RegExpObjectRef::RegExpObjectOption)flags);
      },
      lwPattern,
      flagsValue);
  API_HANDLE_EXCEPTION(r, lwIsolate, MaybeLocal<RegExp>());

  return Utils::NewLocal<RegExp>(lwIsolate->toV8(), r.result);
}

MaybeLocal<v8::RegExp> v8::RegExp::NewWithBacktrackLimit(
    Local<Context> context,
    Local<String> pattern,
    Flags flags,
    uint32_t backtrack_limit) {
  LWNODE_RETURN_LOCAL(RegExp);
}

Local<v8::String> v8::RegExp::GetSource() const {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto lwContext = lwIsolate->GetCurrentContext();
  auto self = CVAL(this)->value();

  auto r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* esState, RegExpObjectRef* self) -> ValueRef* {
        return self->source();
      },
      self->asRegExpObject());
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<String>(lwIsolate->toV8(), r.result);
}

v8::RegExp::Flags v8::RegExp::GetFlags() const {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto lwContext = lwIsolate->GetCurrentContext();
  auto self = CVAL(this)->value();

  int flags = RegExp::Flags::kNone;
  auto r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* esState,
         RegExpObjectRef* self,
         int* flags) -> ValueRef* {
        *flags = self->option();
        return ValueRef::createNull();
      },
      self->asRegExpObject(),
      &flags);
  LWNODE_CHECK(r.isSuccessful());

  return (RegExp::Flags)flags;
}

MaybeLocal<v8::Object> v8::RegExp::Exec(Local<Context> context,
                                        Local<v8::String> subject) {
  API_ENTER_WITH_CONTEXT(context, MaybeLocal<Object>());
  auto lwContext = lwIsolate->GetCurrentContext();
  auto self = CVAL(this)->value();
  auto esSubject = CVAL(*subject)->value()->asString();

  auto r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* state,
         RegExpObjectRef* self,
         StringRef* subject) -> ValueRef* {
        RegExpObjectRef::RegexMatchResult result;
        self->match(state, subject, result, false, 0);

        if (result.m_matchResults.empty()) {
          return ValueRef::createNull();
        }

        auto vector = ValueVectorRef::create();
        for (auto tokens : result.m_matchResults) {
          for (auto token : tokens) {
            auto match = subject->substring(token.m_start, token.m_end);
            vector->pushBack(match);
          }
        }

        return ArrayObjectRef::create(state, vector);
      },
      self->asRegExpObject(),
      esSubject);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Object>(lwIsolate->toV8(), r.result);
}

Local<v8::Array> v8::Array::New(Isolate* isolate, int length) {
  API_ENTER_NO_EXCEPTION(isolate);
  return Utils::NewLocal<Array>(
      isolate,
      ArrayObjectRefHelper::create(lwIsolate->GetCurrentContext()->get(),
                                   (length >= 0) ? length : 0));
}

Local<v8::Array> v8::Array::New(Isolate* isolate,
                                Local<Value>* elements,
                                size_t length) {
  API_ENTER_NO_EXCEPTION(isolate);

  auto vector = ValueVectorRef::create();
  for (size_t i = 0; i < length; i++) {
    vector->pushBack(VAL(**(elements + i))->value());
  }

  return Utils::NewLocal<Array>(
      isolate,
      ArrayObjectRefHelper::create(lwIsolate->GetCurrentContext()->get(),
                                   vector));
}

uint32_t v8::Array::Length() const {
  return static_cast<uint32_t>(ArrayObjectRefHelper::length(
      IsolateWrap::GetCurrent()->GetCurrentContext()->get(),
      CVAL(this)->value()->asArrayObject()));
}

Local<v8::Map> v8::Map::New(Isolate* isolate) {
  EsScope scope(isolate);

  EvalResult r = Evaluator::execute(
      scope.context(), [](ExecutionStateRef* esState) -> ValueRef* {
        return MapObjectRef::create(esState);
      });
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Map>(isolate, r.result);
}

size_t v8::Map::Size() const {
  API_ENTER_NO_TERMINATION_CHECK(EsScope, nullptr);

  EvalResult r = Evaluator::execute(
      scope.context(),
      [](ExecutionStateRef* esState, MapObjectRef* esSelf) -> ValueRef* {
        return ValueRef::create(esSelf->size(esState));
      },
      scope.self()->asMapObject());
  LWNODE_CHECK(r.isSuccessful());

  return r.result->asNumber();
}

void Map::Clear() {
  API_ENTER_NO_TERMINATION_CHECK(EsScope, nullptr);

  EvalResult r = Evaluator::execute(
      scope.context(),
      [](ExecutionStateRef* esState, MapObjectRef* esSelf) -> ValueRef* {
        esSelf->clear(esState);
        return ValueRef::createNull();
      },
      scope.self()->asMapObject());
  LWNODE_CHECK(r.isSuccessful());
}

MaybeLocal<Value> Map::Get(Local<Context> context, Local<Value> key) {
  API_ENTER_AND_EXIT_IF_TERMINATING(EsScope, context, MaybeLocal<Value>());

  EvalResult r = Evaluator::execute(
      scope.context(),
      [](ExecutionStateRef* esState,
         MapObjectRef* esSelf,
         ValueRef* esKey) -> ValueRef* { return esSelf->get(esState, esKey); },
      scope.self()->asMapObject(),
      scope.asValue(key));
  API_EXIT_IF_EXCEPTION_OCCURRED(r, MaybeLocal<Value>());

  return Utils::NewLocal<Value>(scope.v8Isolate(), r.result);
}

MaybeLocal<Map> Map::Set(Local<Context> context,
                         Local<Value> key,
                         Local<Value> value) {
  API_ENTER_AND_EXIT_IF_TERMINATING(EsScope, context, MaybeLocal<Map>());

  EvalResult r = Evaluator::execute(
      scope.context(),
      [](ExecutionStateRef* esState,
         MapObjectRef* esSelf,
         ValueRef* esKey,
         ValueRef* esValue) -> ValueRef* {
        esSelf->set(esState, esKey, esValue);
        return esSelf;
      },
      scope.self()->asMapObject(),
      scope.asValue(key),
      scope.asValue(value));
  API_EXIT_IF_EXCEPTION_OCCURRED(r, MaybeLocal<Map>());

  return Utils::NewLocal<Map>(scope.v8Isolate(), r.result);
}

Maybe<bool> Map::Has(Local<Context> context, Local<Value> key) {
  API_ENTER_AND_EXIT_IF_TERMINATING(EsScope, context, Nothing<bool>());

  EvalResult r = Evaluator::execute(
      scope.context(),
      [](ExecutionStateRef* esState,
         MapObjectRef* esSelf,
         ValueRef* esKey) -> ValueRef* {
        return ValueRef::create(esSelf->has(esState, esKey));
      },
      scope.self()->asMapObject(),
      scope.asValue(key));
  API_EXIT_IF_EXCEPTION_OCCURRED(r, Nothing<bool>());

  return Just(r.result->asBoolean());
}

Maybe<bool> Map::Delete(Local<Context> context, Local<Value> key) {
  API_ENTER_AND_EXIT_IF_TERMINATING(EsScope, context, Nothing<bool>());

  EvalResult r = Evaluator::execute(
      scope.context(),
      [](ExecutionStateRef* esState,
         MapObjectRef* esSelf,
         ValueRef* esKey) -> ValueRef* {
        return ValueRef::create(esSelf->deleteOperation(esState, esKey));
      },
      scope.self()->asMapObject(),
      scope.asValue(key));
  API_EXIT_IF_EXCEPTION_OCCURRED(r, Nothing<bool>());

  return Just(r.result->asBoolean());
}

Local<Array> Map::AsArray() const {
  API_ENTER_NO_TERMINATION_CHECK(EsScope, nullptr);

  EvalResult r = Evaluator::execute(
      scope.context(),
      [](ExecutionStateRef* esState, MapObjectRef* esSelf) -> ValueRef* {
        auto done = StringRef::createFromASCII("done");
        auto value = StringRef::createFromASCII("value");
        auto zero = ValueRef::create(0);
        auto one = ValueRef::create(1);
        auto vector = ValueVectorRef::create();

        auto itr = esSelf->entries(esState);
        for (auto entry = itr->next(esState);
             entry->asObject()->get(esState, done)->isFalse();
             entry = itr->next(esState)) {
          auto keyValueArray =
              entry->asObject()->get(esState, value)->asObject();
          auto key = keyValueArray->getIndexedProperty(esState, zero);
          auto value = keyValueArray->getIndexedProperty(esState, one);

          vector->pushBack(key);
          vector->pushBack(value);
        }

        return ArrayObjectRef::create(esState, vector);
      },
      scope.self()->asMapObject());
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Array>(scope.v8Isolate(), r.result);
}

Local<v8::Set> v8::Set::New(Isolate* isolate) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto esContext = lwIsolate->GetCurrentContext()->get();

  EvalResult r = Evaluator::execute(
      esContext, [](ExecutionStateRef* esState) -> ValueRef* {
        return SetObjectRef::create(esState);
      });
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Set>(isolate, r.result);
}

size_t v8::Set::Size() const {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esSelf = CVAL(this)->value()->asSetObject();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState, SetObjectRef* esSelf) -> ValueRef* {
        return ValueRef::create(esSelf->size(esState));
      },
      esSelf);
  LWNODE_CHECK(r.isSuccessful());

  return r.result->asNumber();
}

void Set::Clear() {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esSelf = CVAL(this)->value()->asSetObject();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState, SetObjectRef* esSelf) -> ValueRef* {
        esSelf->clear(esState);
        return ValueRef::createNull();
      },
      esSelf);
  LWNODE_CHECK(r.isSuccessful());
}

MaybeLocal<Set> Set::Add(Local<Context> context, Local<Value> key) {
  API_ENTER_WITH_CONTEXT(context, MaybeLocal<Set>());
  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esSelf = CVAL(this)->value()->asSetObject();
  auto esKey = CVAL(*key)->value();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState,
         SetObjectRef* esSelf,
         ValueRef* esKey) -> ValueRef* {
        esSelf->add(esState, esKey);
        return ValueRef::createNull();
      },
      esSelf,
      esKey);
  API_HANDLE_EXCEPTION(r, lwIsolate, MaybeLocal<Set>());

  return Utils::NewLocal<Set>(lwIsolate->toV8(), esSelf);
}

Maybe<bool> Set::Has(Local<Context> context, Local<Value> key) {
  API_ENTER_WITH_CONTEXT(context, Nothing<bool>());
  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esSelf = CVAL(this)->value()->asSetObject();
  auto esKey = CVAL(*key)->value();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState,
         SetObjectRef* esSelf,
         ValueRef* esKey) -> ValueRef* {
        return ValueRef::create(esSelf->has(esState, esKey));
      },
      esSelf,
      esKey);
  API_HANDLE_EXCEPTION(r, lwIsolate, Nothing<bool>());

  return Just(r.result->asBoolean());
}

Maybe<bool> Set::Delete(Local<Context> context, Local<Value> key) {
  API_ENTER_WITH_CONTEXT(context, Nothing<bool>());
  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esSelf = CVAL(this)->value()->asSetObject();
  auto esKey = CVAL(*key)->value();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState,
         SetObjectRef* esSelf,
         ValueRef* esKey) -> ValueRef* {
        return ValueRef::create(esSelf->deleteOperation(esState, esKey));
      },
      esSelf,
      esKey);
  API_HANDLE_EXCEPTION(r, lwIsolate, Nothing<bool>());

  return Just(r.result->asBoolean());
}

Local<Array> Set::AsArray() const {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esSelf = CVAL(this)->value()->asSetObject();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState, SetObjectRef* esSelf) -> ValueRef* {
        auto done = StringRef::createFromASCII("done");
        auto value = StringRef::createFromASCII("value");
        auto vector = ValueVectorRef::create();

        auto itr = esSelf->values(esState);
        for (auto entry = itr->next(esState);
             entry->asObject()->get(esState, done)->isFalse();
             entry = itr->next(esState)) {
          vector->pushBack(entry->asObject()->get(esState, value));
        }

        return ArrayObjectRef::create(esState, vector);
      },
      esSelf);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Array>(lwIsolate->toV8(), r.result);
}

MaybeLocal<Promise::Resolver> Promise::Resolver::New(Local<Context> context) {
  API_ENTER_WITH_CONTEXT(context, MaybeLocal<Promise::Resolver>());
  auto esContext = lwIsolate->GetCurrentContext()->get();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* state,
         EscargotShim::IsolateWrap* lwIsolate) -> ValueRef* {
        auto promise = PromiseObjectRef::create(state);
        return promise;
      },
      lwIsolate);
  API_HANDLE_EXCEPTION(r, lwIsolate, MaybeLocal<Promise::Resolver>());

  return Utils::NewLocal<Promise::Resolver>(lwIsolate->toV8(), r.result);
}

Local<Promise> Promise::Resolver::GetPromise() {
  return Utils::NewLocal<Promise>(IsolateWrap::GetCurrent()->toV8(),
                                  VAL(this)->value());
}

Maybe<bool> Promise::Resolver::Resolve(Local<Context> context,
                                       Local<Value> value) {
  API_ENTER_AND_EXIT_IF_TERMINATING(EsScope, context, Nothing<bool>());
  auto self = scope.self()->asPromiseObject();
  if (self->state() != Escargot::PromiseObjectRef::PromiseState::Pending) {
    return Just(true);
  }

  auto esValueTofulfill = scope.asValue(value);
  if (esValueTofulfill->isPromiseObject()) {
    esValueTofulfill = esValueTofulfill->asPromiseObject()->promiseResult();
  }

  EvalResult r = Evaluator::execute(
      scope.context(),
      [](ExecutionStateRef* state,
         PromiseObjectRef* promise,
         ValueRef* value,
         EscargotShim::IsolateWrap* lwIsolate) -> ValueRef* {
        promise->fulfill(state, value);
        return ValueRef::createUndefined();
      },
      self,
      esValueTofulfill,
      scope.lwIsolate());
  API_EXIT_IF_EXCEPTION_OCCURRED(r, Nothing<bool>());

  return Just(true);
}

Maybe<bool> Promise::Resolver::Reject(Local<Context> context,
                                      Local<Value> value) {
  API_ENTER_AND_EXIT_IF_TERMINATING(EsScope, context, Nothing<bool>());
  auto self = scope.self()->asPromiseObject();
  if (self->state() != Escargot::PromiseObjectRef::PromiseState::Pending) {
    return Just(true);
  }

  auto esValueToReject = scope.asValue(value);
  if (esValueToReject->isPromiseObject()) {
    esValueToReject = esValueToReject->asPromiseObject()->promiseResult();
  }

  EvalResult r = Evaluator::execute(
      scope.context(),
      [](ExecutionStateRef* state,
         PromiseObjectRef* promise,
         ValueRef* esValue,
         EscargotShim::IsolateWrap* lwIsolate) -> ValueRef* {
        promise->reject(state, esValue);
        return ValueRef::createUndefined();
      },
      self,
      esValueToReject,
      scope.lwIsolate());
  API_EXIT_IF_EXCEPTION_OCCURRED(r, Nothing<bool>());

  return Just(true);
}

MaybeLocal<Promise> Promise::Catch(Local<Context> context,
                                   Local<Function> handler) {
  API_ENTER_WITH_CONTEXT(context, MaybeLocal<Promise>());
  auto esContext = lwIsolate->GetCurrentContext()->get();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* state,
         PromiseObjectRef* promise,
         FunctionObjectRef* esHandler) -> ValueRef* {
        return promise->catchOperation(state, esHandler);
      },
      CVAL(this)->value()->asPromiseObject(),
      CVAL(*handler)->value()->asFunctionObject());
  API_HANDLE_EXCEPTION(r, lwIsolate, MaybeLocal<Promise>());

  return Utils::NewLocal<Promise>(lwIsolate->toV8(), r.result);
}

MaybeLocal<Promise> Promise::Then(Local<Context> context,
                                  Local<Function> handler) {
  API_ENTER_WITH_CONTEXT(context, MaybeLocal<Promise>());
  auto esContext = lwIsolate->GetCurrentContext()->get();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* state,
         PromiseObjectRef* promise,
         FunctionObjectRef* esHandler) -> ValueRef* {
        return promise->then(state, esHandler);
      },
      CVAL(this)->value()->asPromiseObject(),
      CVAL(*handler)->value()->asFunctionObject());
  API_HANDLE_EXCEPTION(r, lwIsolate, MaybeLocal<Promise>());

  return Utils::NewLocal<Promise>(lwIsolate->toV8(), r.result);
}

MaybeLocal<Promise> Promise::Then(Local<Context> context,
                                  Local<Function> on_fulfilled,
                                  Local<Function> on_rejected) {
  API_ENTER_WITH_CONTEXT(context, MaybeLocal<Promise>());
  auto esContext = lwIsolate->GetCurrentContext()->get();

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* state,
         PromiseObjectRef* promise,
         FunctionObjectRef* esOnFulfilled,
         FunctionObjectRef* esOnRejected) -> ValueRef* {
        return promise->then(state, esOnFulfilled, esOnRejected);
      },
      CVAL(this)->value()->asPromiseObject(),
      CVAL(*on_fulfilled)->value()->asFunctionObject(),
      CVAL(*on_rejected)->value()->asFunctionObject());
  API_HANDLE_EXCEPTION(r, lwIsolate, MaybeLocal<Promise>());

  return Utils::NewLocal<Promise>(lwIsolate->toV8(), r.result);
}

bool Promise::HasHandler() {
  auto self = CVAL(this)->value()->asPromiseObject();
  return self->hasRejectHandlers() || self->hasResolveHandlers();
}

Local<Value> Promise::Result() {
  return Utils::NewLocal<Value>(
      IsolateWrap::GetCurrent()->toV8(),
      CVAL(this)->value()->asPromiseObject()->promiseResult());
}

Promise::PromiseState Promise::State() {
  auto self = CVAL(this)->value()->asPromiseObject();
  switch (self->state()) {
    case Escargot::PromiseObjectRef::PromiseState::Pending:
      return v8::Promise::PromiseState::kPending;
    case Escargot::PromiseObjectRef::PromiseState::FulFilled:
      return v8::Promise::PromiseState::kFulfilled;
    case Escargot::PromiseObjectRef::PromiseState::Rejected:
      return v8::Promise::PromiseState::kRejected;
    default:
      LWNODE_CHECK_NOT_REACH_HERE();
      break;
  }
  LWNODE_CHECK_NOT_REACH_HERE();
  return v8::Promise::PromiseState::kPending;
}

void Promise::MarkAsHandled() {
  LWNODE_UNIMPLEMENT;
}

Local<Value> Proxy::GetTarget() {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto esSelf = CVAL(this)->value()->asProxyObject();
  auto target = esSelf->target();
  if (target) {
    return Utils::NewLocal<Value>(lwIsolate->toV8(), target);
  }

  return Utils::NewLocal<Value>(lwIsolate->toV8(), ValueRef::createNull());
}

Local<Value> Proxy::GetHandler() {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto esSelf = CVAL(this)->value()->asProxyObject();

  auto handler = esSelf->handler();
  if (handler) {
    return Utils::NewLocal<Value>(lwIsolate->toV8(), handler);
  }

  return Utils::NewLocal<Value>(lwIsolate->toV8(), ValueRef::createNull());
}

bool Proxy::IsRevoked() {
  auto esSelf = CVAL(this)->value()->asProxyObject();
  return esSelf->isRevoked();
}

void Proxy::Revoke() {
  auto esSelf = CVAL(this)->value()->asProxyObject();
  esSelf->revoke();
}

MaybeLocal<Proxy> Proxy::New(Local<Context> context,
                             Local<Object> local_target,
                             Local<Object> local_handler) {
  API_ENTER_WITH_CONTEXT(context, MaybeLocal<Proxy>());
  auto lwContext = lwIsolate->GetCurrentContext();

  EvalResult r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* state,
         ValueRef* target,
         ValueRef* handler) -> ValueRef* {
        return ProxyObjectRef::create(
            state, target->asObject(), handler->asObject());
      },
      CVAL(*local_target)->value(),
      CVAL(*local_handler)->value());
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Proxy>(lwIsolate->toV8(), r.result);
}

CompiledWasmModule::CompiledWasmModule(
    std::shared_ptr<internal::wasm::NativeModule> native_module,
    const char* source_url,
    size_t url_length)
    : native_module_(std::move(native_module)) {}

OwnedBuffer CompiledWasmModule::Serialize() {
  LWNODE_UNIMPLEMENT;
  std::unique_ptr<uint8_t[]> buffer(new uint8_t[0]);
  return {std::move(buffer), 0};
}

MemorySpan<const uint8_t> CompiledWasmModule::GetWireBytesRef() {
  LWNODE_UNIMPLEMENT;
  return {nullptr, 0};
}

CompiledWasmModule WasmModuleObject::GetCompiledModule() {
  LWNODE_UNIMPLEMENT;
  return CompiledWasmModule(nullptr, nullptr, 0);
}

MaybeLocal<WasmModuleObject> WasmModuleObject::FromCompiledModule(
    Isolate* isolate, const CompiledWasmModule& compiled_module){
    LWNODE_RETURN_LOCAL(WasmModuleObject)}

WasmModuleObjectBuilderStreaming::WasmModuleObjectBuilderStreaming(
    Isolate* isolate) {}

Local<Promise> WasmModuleObjectBuilderStreaming::GetPromise() {
  LWNODE_RETURN_LOCAL(Promise);
}

void WasmModuleObjectBuilderStreaming::OnBytesReceived(const uint8_t* bytes,
                                                       size_t size) {}

void WasmModuleObjectBuilderStreaming::Finish() {
  LWNODE_UNIMPLEMENT;
}

void WasmModuleObjectBuilderStreaming::Abort(MaybeLocal<Value> exception) {}

void* v8::ArrayBuffer::Allocator::Reallocate(void* data,
                                             size_t old_length,
                                             size_t new_length) {
  LWNODE_RETURN_NULLPTR;
}

// static
v8::ArrayBuffer::Allocator* v8::ArrayBuffer::Allocator::NewDefaultAllocator() {
  return new ArrayBufferAllocator();
}

bool v8::ArrayBuffer::IsExternal() const {
  LWNODE_RETURN_FALSE;
}

bool v8::ArrayBuffer::IsDetachable() const {
  auto esSelf = CVAL(this)->value()->asArrayBufferObject();
  return !esSelf->isDetachedBuffer();
}

v8::ArrayBuffer::Contents::Contents(void* data,
                                    size_t byte_length,
                                    void* allocation_base,
                                    size_t allocation_length,
                                    Allocator::AllocationMode allocation_mode,
                                    DeleterCallback deleter,
                                    void* deleter_data)
    : data_(data),
      byte_length_(byte_length),
      allocation_base_(allocation_base),
      allocation_length_(allocation_length),
      allocation_mode_(allocation_mode),
      deleter_(deleter),
      deleter_data_(deleter_data) {}

v8::ArrayBuffer::Contents v8::ArrayBuffer::Externalize() {
  return GetContents(true);
}

void v8::ArrayBuffer::Externalize(
    const std::shared_ptr<BackingStore>& backing_store) {
  LWNODE_RETURN_VOID;
}

v8::ArrayBuffer::Contents v8::ArrayBuffer::GetContents() {
  return GetContents(false);
}

v8::ArrayBuffer::Contents v8::ArrayBuffer::GetContents(bool externalize) {
  LWNODE_UNIMPLEMENT;
  return ArrayBuffer::Contents();
}

void v8::ArrayBuffer::Detach() {
  CVAL(this)->value()->asArrayBufferObject()->detachArrayBuffer();
}

size_t v8::ArrayBuffer::ByteLength() const {
  return CVAL(this)->value()->asArrayBufferObject()->byteLength();
}

Local<ArrayBuffer> v8::ArrayBuffer::New(Isolate* isolate, size_t byte_length) {
  // @note regarding Backing Store and Array Buffer:
  // https://docs.google.com/document/d/1sTc_jRL87Fu175Holm5SV0kajkseGl2r8ifGY76G35k/edit#
  API_ENTER_NO_EXCEPTION(isolate);
  auto lwContext = lwIsolate->GetCurrentContext();

  EvalResult r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* esState, size_t byteLength) -> ValueRef* {
        auto arrayBuffer = ArrayBufferObjectRef::create(esState);
        arrayBuffer->allocateBuffer(esState, byteLength);
        return arrayBuffer;
      },
      byte_length);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<ArrayBuffer>(isolate, r.result);
}

Local<ArrayBuffer> v8::ArrayBuffer::New(Isolate* isolate,
                                        void* data,
                                        size_t byte_length,
                                        ArrayBufferCreationMode mode) {
  LWNODE_RETURN_LOCAL(ArrayBuffer);
}

Local<ArrayBuffer> v8::ArrayBuffer::New(
    Isolate* isolate, std::shared_ptr<BackingStore> backing_store) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto lwContext = lwIsolate->GetCurrentContext();
  auto esBackingStore = reinterpret_cast<BackingStoreRef*>(backing_store.get());

  EvalResult r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* esState,
         BackingStoreRef* backingStore) -> ValueRef* {
        auto arrayBuffer = ArrayBufferObjectRef::create(esState);
        arrayBuffer->attachBuffer(backingStore);
        return arrayBuffer;
      },
      esBackingStore);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<ArrayBuffer>(isolate, r.result);
}

std::unique_ptr<v8::BackingStore> v8::ArrayBuffer::NewBackingStore(
    Isolate* isolate, size_t byte_length) {
  auto lwIsolate = IsolateWrap::GetCurrent();

  BackingStoreRef* esBackingStore =
      BackingStoreRef::createDefaultNonSharedBackingStore(byte_length);
  lwIsolate->addBackingStore(esBackingStore);

  return std::unique_ptr<v8::BackingStore>(
      reinterpret_cast<v8::BackingStore*>(esBackingStore));
}

std::unique_ptr<v8::BackingStore> v8::ArrayBuffer::NewBackingStore(
    void* data,
    size_t byte_length,
    v8::BackingStore::DeleterCallback deleter,
    void* deleter_data) {
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto lwContext = lwIsolate->GetCurrentContext();

  struct Params {
    BackingStore::DeleterCallback deleter{nullptr};
    void* deleter_data{nullptr};
  };

  Params* callbackData = new Params();
  callbackData->deleter = deleter;
  callbackData->deleter_data = deleter_data;

  auto callback = [](void* data, size_t length, void* callbackData) {
    Params* params = reinterpret_cast<Params*>(callbackData);
    if (data) {
      /*
        @note

        According to src/node_buffer.cc mention, V8 simply ignores the
        BackingStore deleter callback if data == nullptr.
      */
      params->deleter(data, length, params->deleter_data);
    }
    delete params;
  };

  BackingStoreRef* esBackingStore =
      BackingStoreRef::createNonSharedBackingStore(
          data, byte_length, callback, callbackData);

  lwIsolate->addBackingStore(esBackingStore);

  return std::unique_ptr<v8::BackingStore>(
      reinterpret_cast<v8::BackingStore*>(esBackingStore));
}

Local<ArrayBuffer> v8::ArrayBufferView::Buffer() {
  auto esArrayBufferObject = VAL(this)->value()->asArrayBufferView()->buffer();
  return Utils::NewLocal<ArrayBuffer>(IsolateWrap::GetCurrent()->toV8(),
                                      esArrayBufferObject);
}

size_t v8::ArrayBufferView::CopyContents(void* dest, size_t byte_length) {
  LWNODE_CALL_TRACE();

  auto esArrayBufferView = CVAL(this)->value()->asArrayBufferView();

  if (byte_length == 0 || esArrayBufferView->rawBuffer() == nullptr) {
    return 0;
  }

  LWNODE_CHECK_NOT_NULL(dest);

  size_t byteLengthToCopy =
      std::min(byte_length, esArrayBufferView->byteLength());

  memcpy(dest, esArrayBufferView->rawBuffer(), byteLengthToCopy);

  return byteLengthToCopy;
}

bool v8::ArrayBufferView::HasBuffer() const {
  ArrayBufferRef* esArrayBufferObject =
      CVAL(this)->value()->asArrayBufferView()->buffer();
  return esArrayBufferObject != nullptr;
}

size_t v8::ArrayBufferView::ByteOffset() {
  return CVAL(this)->value()->asArrayBufferView()->byteOffset();
}

size_t v8::ArrayBufferView::ByteLength() {
  return CVAL(this)->value()->asArrayBufferView()->byteLength();
}

size_t v8::TypedArray::Length() {
  return CVAL(this)->value()->asArrayBufferView()->arrayLength();
}

#define TYPED_ARRAY_NEW(Type, type, TYPE, ctype)                               \
  Local<Type##Array> Type##Array::New(                                         \
      Local<ArrayBuffer> array_buffer, size_t byte_offset, size_t length) {    \
    auto lwIsolate = IsolateWrap::GetCurrent();                                \
    auto esContext = lwIsolate->GetCurrentContext()->get();                    \
    auto esArrayBuffer = VAL(*array_buffer)->value()->asArrayBufferObject();   \
                                                                               \
    auto esArrayBufferView =                                                   \
        ArrayBufferHelper::createView<Type##ArrayObjectRef>(                   \
            esContext,                                                         \
            esArrayBuffer,                                                     \
            byte_offset,                                                       \
            length,                                                            \
            ArrayBufferHelper::ArrayType::kExternal##Type##Array);             \
                                                                               \
    LWNODE_CHECK(esArrayBufferView->is##Type##ArrayObject());                  \
                                                                               \
    return Utils::NewLocal<Type##Array>(lwIsolate->toV8(), esArrayBufferView); \
  }                                                                            \
                                                                               \
  Local<Type##Array> Type##Array::New(                                         \
      Local<SharedArrayBuffer> shared_array_buffer,                            \
      size_t byte_offset,                                                      \
      size_t length) {                                                         \
    LWNODE_RETURN_LOCAL(Type##Array);                                          \
  }

TYPED_ARRAYS(TYPED_ARRAY_NEW)
#undef TYPED_ARRAY_NEW

Local<DataView> DataView::New(Local<ArrayBuffer> array_buffer,
                              size_t byte_offset,
                              size_t byte_length) {
  LWNODE_RETURN_LOCAL(DataView);
}

Local<DataView> DataView::New(Local<SharedArrayBuffer> shared_array_buffer,
                              size_t byte_offset,
                              size_t byte_length) {
  LWNODE_RETURN_LOCAL(DataView);
}

bool v8::SharedArrayBuffer::IsExternal() const {
  LWNODE_RETURN_FALSE;
}

v8::SharedArrayBuffer::Contents::Contents(
    void* data,
    size_t byte_length,
    void* allocation_base,
    size_t allocation_length,
    Allocator::AllocationMode allocation_mode,
    DeleterCallback deleter,
    void* deleter_data)
    : data_(data),
      byte_length_(byte_length),
      allocation_base_(allocation_base),
      allocation_length_(allocation_length),
      allocation_mode_(allocation_mode),
      deleter_(deleter),
      deleter_data_(deleter_data) {
  LWNODE_UNIMPLEMENT;
}

v8::SharedArrayBuffer::Contents v8::SharedArrayBuffer::Externalize() {
  return GetContents(true);
}

void v8::SharedArrayBuffer::Externalize(
    const std::shared_ptr<BackingStore>& backing_store) {
  LWNODE_UNIMPLEMENT;
}

v8::SharedArrayBuffer::Contents v8::SharedArrayBuffer::GetContents() {
  return GetContents(false);
}

v8::SharedArrayBuffer::Contents v8::SharedArrayBuffer::GetContents(
    bool externalize) {
  return v8::SharedArrayBuffer::Contents();
}

size_t v8::SharedArrayBuffer::ByteLength() const {
  auto esSelf = CVAL(this)->value()->asSharedArrayBufferObject();
  return esSelf->byteLength();
}

Local<SharedArrayBuffer> v8::SharedArrayBuffer::New(Isolate* isolate,
                                                    size_t byte_length) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto lwContext = lwIsolate->GetCurrentContext();

  auto r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* state, size_t byteLength) -> ValueRef* {
        return SharedArrayBufferObjectRef::create(state, byteLength);
      },
      byte_length);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<SharedArrayBuffer>(isolate, r.result);
}

Local<SharedArrayBuffer> v8::SharedArrayBuffer::New(
    Isolate* isolate,
    void* data,
    size_t byte_length,
    ArrayBufferCreationMode mode) {
  // Deprecate soon
  LWNODE_RETURN_LOCAL(SharedArrayBuffer);
}

Local<SharedArrayBuffer> v8::SharedArrayBuffer::New(
    Isolate* isolate, std::shared_ptr<BackingStore> backing_store) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto lwContext = lwIsolate->GetCurrentContext();

  auto esBackingStore = reinterpret_cast<BackingStoreRef*>(backing_store.get());
  auto r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* state, BackingStoreRef* bs) -> ValueRef* {
        return SharedArrayBufferObjectRef::create(state, bs);
      },
      esBackingStore);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<SharedArrayBuffer>(isolate, r.result);
}

Local<SharedArrayBuffer> v8::SharedArrayBuffer::New(
    Isolate* isolate,
    const SharedArrayBuffer::Contents& contents,
    ArrayBufferCreationMode mode) {
  LWNODE_RETURN_LOCAL(SharedArrayBuffer);
}

std::unique_ptr<v8::BackingStore> v8::SharedArrayBuffer::NewBackingStore(
    Isolate* isolate, size_t byte_length) {
  API_ENTER_NO_EXCEPTION(isolate);

  BackingStoreRef* esBackingStore =
      BackingStoreRef::createDefaultSharedBackingStore(byte_length);
  lwIsolate->addBackingStore(esBackingStore);

  return std::unique_ptr<v8::BackingStore>(
      reinterpret_cast<v8::BackingStore*>(esBackingStore));
}

std::unique_ptr<v8::BackingStore> v8::SharedArrayBuffer::NewBackingStore(
    void* data,
    size_t byte_length,
    v8::BackingStore::DeleterCallback deleter,
    void* deleter_data) {
  auto lwIsolate = IsolateWrap::GetCurrent();

  struct Params {
    BackingStore::DeleterCallback deleter{nullptr};
    void* deleterData{nullptr};
  };

  Params* callbackData = new Params();
  callbackData->deleter = deleter;
  callbackData->deleterData = deleter_data;

  auto callback = [](void* data, size_t length, void* callbackData) {
    Params* params = reinterpret_cast<Params*>(callbackData);
    if (data) {
      params->deleter(data, length, params->deleterData);
    }
    delete params;
  };

  // TODO: change to shared type.
  LWNODE_UNIMPLEMENT;
  auto esBackingStore = BackingStoreRef::createNonSharedBackingStore(
      data, byte_length, callback, callbackData);
  lwIsolate->addBackingStore(esBackingStore);

  return std::unique_ptr<v8::BackingStore>(
      reinterpret_cast<v8::BackingStore*>(esBackingStore));
}

Local<Symbol> v8::Symbol::New(Isolate* isolate, Local<String> name) {
  API_ENTER_NO_EXCEPTION(isolate);

  StringRef* esName;

  if (name.IsEmpty()) {
    esName = StringRef::emptyString();
  } else {
    LWNODE_CHECK(VAL(*name)->value()->isString());
    esName = VAL(*name)->value()->asString();
  }
  return Utils::NewLocal<Symbol>(isolate, SymbolRef::create(esName));
}

Local<Symbol> v8::Symbol::For(Isolate* isolate, Local<String> name) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto lwContext = lwIsolate->GetCurrentContext();
  auto esName = VAL(*name)->value()->asString();

  auto r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* esState,
         VMInstanceRef* esVmInstance,
         StringRef* desc) -> ValueRef* {
        return SymbolRef::fromGlobalSymbolRegistry(esVmInstance, desc);
      },
      lwIsolate->get(),
      esName);
  LWNODE_CHECK(r.isSuccessful());

  return Utils::NewLocal<Symbol>(isolate, r.result);
}

Local<Symbol> v8::Symbol::ForApi(Isolate* isolate, Local<String> name) {
  API_ENTER_NO_EXCEPTION(isolate);

  SymbolRef* esSymbol =
      lwIsolate->getApiSymbol(VAL(*name)->value()->asString());

  return Utils::NewLocal<Symbol>(isolate, esSymbol);
}

#define WELL_KNOWN_SYMBOLS(V)                                                  \
  V(AsyncIterator, async_iterator, asyncIterator)                              \
  V(HasInstance, has_instance, hasInstance)                                    \
  V(IsConcatSpreadable, is_concat_spreadable, isConcatSpreadable)              \
  V(Iterator, iterator, iterator)                                              \
  V(Match, match, match)                                                       \
  V(Replace, replace, replace)                                                 \
  V(Search, search, search)                                                    \
  V(Split, split, split)                                                       \
  V(ToPrimitive, to_primitive, toPrimitive)                                    \
  V(ToStringTag, to_string_tag, toStringTag)                                   \
  V(Unscopables, unscopables, unscopables)

#define SYMBOL_GETTER(Name, v8InternalName, esName)                            \
  Local<Symbol> v8::Symbol::Get##Name(Isolate* isolate) {                      \
    IsolateWrap* lwIsolate = IsolateWrap::fromV8(isolate);                     \
    auto lwContext = lwIsolate->GetCurrentContext();                           \
                                                                               \
    auto r = Evaluator::execute(                                               \
        lwContext->get(),                                                      \
        [](ExecutionStateRef* state, VMInstanceRef* vmInstance) -> ValueRef* { \
          return vmInstance->esName##Symbol();                                 \
        },                                                                     \
        lwIsolate->get());                                                     \
                                                                               \
    return Utils::NewLocal<Symbol>(isolate, r.result);                         \
  }

WELL_KNOWN_SYMBOLS(SYMBOL_GETTER)

#undef SYMBOL_GETTER
#undef WELL_KNOWN_SYMBOLS

Local<Private> v8::Private::New(Isolate* isolate, Local<String> name) {
  API_ENTER_NO_EXCEPTION(isolate);
  StringRef* esName;

  if (name.IsEmpty()) {
    esName = StringRef::emptyString();
  } else {
    LWNODE_CHECK(VAL(*name)->value()->isString());
    esName = VAL(*name)->value()->asString();
  }
  // @note
  // A private symbol in V8 is similar to a Symbol, except that it’s not
  // enumerable and doesn’t leak to userspace JavaScript.
  // @todo For now, we ignore the private attribute and use a normal Symbol
  // instead.

  SymbolRef* esSymbol = lwIsolate->createApiPrivateSymbol(esName);

  return Utils::NewLocal<Private>(isolate, esSymbol);
}

Local<Private> v8::Private::ForApi(Isolate* isolate, Local<String> name) {
  API_ENTER_NO_EXCEPTION(isolate);

  SymbolRef* esSymbol =
      lwIsolate->getApiPrivateSymbol(VAL(*name)->value()->asString());

  return Utils::NewLocal<Private>(isolate, esSymbol);
}

Local<Number> v8::Number::New(Isolate* isolate, double value) {
  return Utils::NewLocal<Number>(isolate, Escargot::ValueRef::create(value));
}

Local<Integer> v8::Integer::New(Isolate* isolate, int32_t value) {
  return Utils::NewLocal<Integer>(isolate, Escargot::ValueRef::create(value));
}

Local<Integer> v8::Integer::NewFromUnsigned(Isolate* isolate, uint32_t value) {
  return Utils::NewLocal<Integer>(isolate, Escargot::ValueRef::create(value));
}

Local<BigInt> v8::BigInt::New(Isolate* isolate, int64_t value) {
  LWNODE_RETURN_LOCAL(BigInt);
}

Local<BigInt> v8::BigInt::NewFromUnsigned(Isolate* isolate, uint64_t value) {
  LWNODE_RETURN_LOCAL(BigInt);
}

MaybeLocal<BigInt> v8::BigInt::NewFromWords(Local<Context> context,
                                            int sign_bit,
                                            int word_count,
                                            const uint64_t* words) {
  LWNODE_RETURN_LOCAL(BigInt);
}

uint64_t v8::BigInt::Uint64Value(bool* lossless) const {
  LWNODE_RETURN_0;
}

int64_t v8::BigInt::Int64Value(bool* lossless) const {
  LWNODE_RETURN_0;
}

int BigInt::WordCount() const {
  LWNODE_RETURN_0;
}

void BigInt::ToWordsArray(int* sign_bit,
                          int* word_count,
                          uint64_t* words) const {}

void Isolate::ReportExternalAllocationLimitReached() {}

// Node v14.x ABI compat dummy.
void Isolate::CheckMemoryPressure() {
  LWNODE_RETURN_VOID;
}

HeapProfiler* Isolate::GetHeapProfiler() {
  LWNODE_UNIMPLEMENT_IGNORED;
  return nullptr;
}

void Isolate::SetIdle(bool is_idle) {
  LWNODE_RETURN_VOID;
}

ArrayBuffer::Allocator* Isolate::GetArrayBufferAllocator() {
  return IsolateWrap::fromV8(this)->array_buffer_allocator();
}

bool Isolate::InContext() {
  return IsolateWrap::fromV8(this)->InContext();
}

void Isolate::ClearKeptObjects() {
  LWNODE_ONCE(LWNODE_UNIMPLEMENT);
}

v8::Local<v8::Context> Isolate::GetCurrentContext() {
  auto lwContext = IsolateWrap::fromV8(this)->GetCurrentContext();
  return Utils::NewLocal(this, lwContext);
}

v8::Local<v8::Context> Isolate::GetEnteredContext() {
  LWNODE_RETURN_LOCAL(Context);
}

v8::Local<v8::Context> Isolate::GetEnteredOrMicrotaskContext() {
  LWNODE_RETURN_LOCAL(Context);
}

v8::Local<v8::Context> Isolate::GetIncumbentContext() {
  LWNODE_RETURN_LOCAL(Context);
}

v8::Local<Value> Isolate::ThrowException(v8::Local<v8::Value> value) {
  LWNODE_CALL_TRACE_ID(TRYCATCH);
  auto lwIsolate = IsolateWrap::GetCurrent();
  auto esValue = value.IsEmpty() ? lwIsolate->undefined_value()->value()
                                 : CVAL(*value)->value();
  lwIsolate->ScheduleThrow(esValue);

  return Utils::NewLocal<Value>(lwIsolate->toV8(), esValue);
}

void Isolate::AddGCPrologueCallback(GCCallbackWithData callback,
                                    void* data,
                                    GCType gc_type) {
  LWNODE_RETURN_VOID;
}

void Isolate::RemoveGCPrologueCallback(GCCallbackWithData callback,
                                       void* data) {
  LWNODE_RETURN_VOID;
}

void Isolate::AddGCEpilogueCallback(GCCallbackWithData callback,
                                    void* data,
                                    GCType gc_type) {
  LWNODE_RETURN_VOID;
}

void Isolate::RemoveGCEpilogueCallback(GCCallbackWithData callback,
                                       void* data) {
  LWNODE_RETURN_VOID;
}

void Isolate::AddGCPrologueCallback(GCCallback callback, GCType gc_type) {
  LWNODE_RETURN_VOID;
}

void Isolate::RemoveGCPrologueCallback(GCCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::AddGCEpilogueCallback(GCCallback callback, GCType gc_type) {
  LWNODE_RETURN_VOID;
}

void Isolate::RemoveGCEpilogueCallback(GCCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetEmbedderHeapTracer(EmbedderHeapTracer* tracer) {
  LWNODE_RETURN_VOID;
}

EmbedderHeapTracer* Isolate::GetEmbedderHeapTracer() {
  LWNODE_RETURN_NULLPTR;
}

void Isolate::SetGetExternallyAllocatedMemoryInBytesCallback(
    GetExternallyAllocatedMemoryInBytesCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::TerminateExecution() {
  LWNODE_RETURN_VOID;
}

bool Isolate::IsExecutionTerminating() {
  LWNODE_RETURN_FALSE;
}

void Isolate::CancelTerminateExecution() {
  LWNODE_RETURN_VOID;
}

void Isolate::RequestInterrupt(InterruptCallback callback, void* data) {
  LWNODE_RETURN_VOID;
}

void Isolate::RequestGarbageCollectionForTesting(GarbageCollectionType type) {
  IsolateWrap::GetCurrent()->CollectGarbage(GarbageCollectionReason::kTesting);
}

Isolate* Isolate::GetCurrent() {
  return IsolateWrap::GetCurrent()->toV8();
}

// static
Isolate* Isolate::Allocate() {
  Engine::current()->initializeThread();
  return IsolateWrap::toV8(IsolateWrap::New());
}

// static
// This is separate so that tests can provide a different |isolate|.
void Isolate::Initialize(Isolate* isolate,
                         const v8::Isolate::CreateParams& params) {
  IsolateWrap::fromV8(isolate)->Initialize(params);
}

Isolate* Isolate::New(const Isolate::CreateParams& params) {
  LWNODE_CALL_TRACE();
  Isolate* isolate = Allocate();
  Initialize(isolate, params);
  return isolate;
}

void Isolate::Dispose() {
  IsolateWrap::fromV8(this)->Dispose();
  Engine::current()->finalizeThread();
}

void Isolate::DumpAndResetStats() {
  LWNODE_RETURN_VOID;
}

void Isolate::DiscardThreadSpecificMetadata() {
  LWNODE_RETURN_VOID;
}

void Isolate::Enter() {
  LWNODE_CALL_TRACE();
  IsolateWrap::fromV8(this)->Enter();
}

void Isolate::Exit() {
  LWNODE_CALL_TRACE();
  IsolateWrap::fromV8(this)->Exit();
}

void Isolate::SetAbortOnUncaughtExceptionCallback(
    AbortOnUncaughtExceptionCallback callback) {
  IsolateWrap::fromV8(this)->SetAbortOnUncaughtExceptionCallback(callback);
}

void Isolate::SetHostCleanupFinalizationGroupCallback(
    HostCleanupFinalizationGroupCallback callback) {
  LWNODE_RETURN_VOID;
}

Maybe<bool> FinalizationGroup::Cleanup(
    Local<FinalizationGroup> finalization_group) {
  LWNODE_RETURN_MAYBE(bool);
}

void Isolate::SetHostImportModuleDynamicallyCallback(
    HostImportModuleDynamicallyCallback callback) {
  LWNODE_ONCE(LWNODE_UNIMPLEMENT);
}

void Isolate::SetHostInitializeImportMetaObjectCallback(
    HostInitializeImportMetaObjectCallback callback) {
  LWNODE_ONCE(LWNODE_UNIMPLEMENT);
}

void Isolate::SetPrepareStackTraceCallback(PrepareStackTraceCallback callback) {
  IsolateWrap::fromV8(this)->SetPrepareStackTraceCallback(callback);
}

Isolate::DisallowJavascriptExecutionScope::DisallowJavascriptExecutionScope(
    Isolate* isolate,
    Isolate::DisallowJavascriptExecutionScope::OnFailure on_failure)
    : on_failure_(on_failure) {
  LWNODE_ONCE(LWNODE_UNIMPLEMENT);
}

Isolate::DisallowJavascriptExecutionScope::~DisallowJavascriptExecutionScope() {
  LWNODE_ONCE(LWNODE_UNIMPLEMENT);
}

Isolate::AllowJavascriptExecutionScope::AllowJavascriptExecutionScope(
    Isolate* isolate) {
  LWNODE_UNIMPLEMENT;
}

Isolate::AllowJavascriptExecutionScope::~AllowJavascriptExecutionScope() {
  LWNODE_UNIMPLEMENT;
}

Isolate::SuppressMicrotaskExecutionScope::SuppressMicrotaskExecutionScope(
    Isolate* isolate, MicrotaskQueue* microtask_queue)
    : isolate_(reinterpret_cast<i::Isolate*>(isolate)),
      microtask_queue_(nullptr) {
  LWNODE_UNIMPLEMENT;
}

Isolate::SuppressMicrotaskExecutionScope::~SuppressMicrotaskExecutionScope() {}

Isolate::SafeForTerminationScope::SafeForTerminationScope(v8::Isolate* isolate)
    : isolate_(reinterpret_cast<i::Isolate*>(isolate)), prev_value_(nullptr) {
  LWNODE_UNIMPLEMENT;
}

Isolate::SafeForTerminationScope::~SafeForTerminationScope() {
  LWNODE_UNIMPLEMENT;
}

i::Address* Isolate::GetDataFromSnapshotOnce(size_t index) {
  LWNODE_RETURN_NULLPTR;
}

void Isolate::GetHeapStatistics(HeapStatistics* heap_statistics) {
  LWNODE_RETURN_VOID;
}

size_t Isolate::NumberOfHeapSpaces() {
  LWNODE_RETURN_0;
}

bool Isolate::GetHeapSpaceStatistics(HeapSpaceStatistics* space_statistics,
                                     size_t index) {
  LWNODE_RETURN_FALSE;
}

size_t Isolate::NumberOfTrackedHeapObjectTypes() {
  LWNODE_RETURN_0;
}

bool Isolate::GetHeapObjectStatisticsAtLastGC(
    HeapObjectStatistics* object_statistics, size_t type_index) {
  LWNODE_RETURN_FALSE;
}

bool Isolate::GetHeapCodeAndMetadataStatistics(
    HeapCodeStatistics* code_statistics) {
  LWNODE_RETURN_FALSE;
}

v8::MaybeLocal<v8::Promise> Isolate::MeasureMemory(
    v8::Local<v8::Context> context, MeasureMemoryMode mode) {
  LWNODE_RETURN_LOCAL(Promise);
}

bool Isolate::MeasureMemory(std::unique_ptr<MeasureMemoryDelegate> delegate,
                            MeasureMemoryExecution execution) {
  LWNODE_RETURN_FALSE;
}

std::unique_ptr<MeasureMemoryDelegate> MeasureMemoryDelegate::Default(
    Isolate* isolate,
    Local<Context> context,
    Local<Promise::Resolver> promise_resolver,
    MeasureMemoryMode mode) {
  LWNODE_RETURN_NULLPTR;
}

void Isolate::GetStackSample(const RegisterState& state,
                             void** frames,
                             size_t frames_limit,
                             SampleInfo* sample_info) {
  LWNODE_RETURN_VOID;
}

size_t Isolate::NumberOfPhantomHandleResetsSinceLastCall() {
  LWNODE_RETURN_0;
}

void Isolate::SetEventLogger(LogEventCallback that) {
  LWNODE_RETURN_VOID;
}

void Isolate::AddBeforeCallEnteredCallback(BeforeCallEnteredCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::RemoveBeforeCallEnteredCallback(
    BeforeCallEnteredCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::AddCallCompletedCallback(CallCompletedCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::RemoveCallCompletedCallback(CallCompletedCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::AtomicsWaitWakeHandle::Wake() {
  LWNODE_RETURN_VOID;
}

void Isolate::SetAtomicsWaitCallback(AtomicsWaitCallback callback, void* data) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetPromiseHook(PromiseHook hook) {
  IsolateWrap::fromV8(this)->SetPromiseHook(hook);
}

void Isolate::SetPromiseRejectCallback(PromiseRejectCallback callback) {
  IsolateWrap::fromV8(this)->SetPromiseRejectCallback(callback);
}

void Isolate::PerformMicrotaskCheckpoint() {
  IsolateWrap::fromV8(this)->PerformMicrotaskCheckpoint();
}

void Isolate::EnqueueMicrotask(Local<Function> v8_function) {
  LWNODE_RETURN_VOID;
}

void Isolate::EnqueueMicrotask(MicrotaskCallback callback, void* data) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetMicrotasksPolicy(MicrotasksPolicy policy) {
  LWNODE_CALL_TRACE();
}

MicrotasksPolicy Isolate::GetMicrotasksPolicy() const {
  LWNODE_UNIMPLEMENT;
  return MicrotasksPolicy();
}

void Isolate::AddMicrotasksCompletedCallback(
    MicrotasksCompletedCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::AddMicrotasksCompletedCallback(
    MicrotasksCompletedCallbackWithData callback, void* data) {
  LWNODE_RETURN_VOID;
}

void Isolate::RemoveMicrotasksCompletedCallback(
    MicrotasksCompletedCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::RemoveMicrotasksCompletedCallback(
    MicrotasksCompletedCallbackWithData callback, void* data) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetUseCounterCallback(UseCounterCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetCounterFunction(CounterLookupCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetCreateHistogramFunction(CreateHistogramCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetAddHistogramSampleFunction(
    AddHistogramSampleCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetAddCrashKeyCallback(AddCrashKeyCallback callback) {
  LWNODE_RETURN_VOID;
}

bool Isolate::IdleNotificationDeadline(double deadline_in_seconds) {
  LWNODE_RETURN_FALSE;
}

void Isolate::LowMemoryNotification() {
  LWNODE_RETURN_VOID;
}

int Isolate::ContextDisposedNotification(bool dependant_context) {
  LWNODE_RETURN_0;
}

void Isolate::IsolateInForegroundNotification() {
  LWNODE_RETURN_VOID;
}

void Isolate::IsolateInBackgroundNotification() {
  LWNODE_RETURN_VOID;
}

void Isolate::MemoryPressureNotification(MemoryPressureLevel level) {
  LWNODE_RETURN_VOID;
}

void Isolate::EnableMemorySavingsMode() {
  LWNODE_RETURN_VOID;
}

void Isolate::DisableMemorySavingsMode() {
  LWNODE_RETURN_VOID;
}

void Isolate::SetRAILMode(RAILMode rail_mode) {
  LWNODE_RETURN_VOID;
}

void Isolate::IncreaseHeapLimitForDebugging() {
  // No-op.
  LWNODE_RETURN_VOID;
}

void Isolate::RestoreOriginalHeapLimit() {
  // No-op.
  LWNODE_RETURN_VOID;
}

bool Isolate::IsHeapLimitIncreasedForDebugging() {
  LWNODE_RETURN_FALSE;
}

void Isolate::SetJitCodeEventHandler(JitCodeEventOptions options,
                                     JitCodeEventHandler event_handler) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetStackLimit(uintptr_t stack_limit) {
  LWNODE_RETURN_VOID;
}

void Isolate::GetCodeRange(void** start, size_t* length_in_bytes) {
  LWNODE_RETURN_VOID;
}

UnwindState Isolate::GetUnwindState() {
  UnwindState unwind_state;
  LWNODE_UNIMPLEMENT;

  return unwind_state;
}

JSEntryStubs Isolate::GetJSEntryStubs() {
  JSEntryStubs entry_stubs;

  LWNODE_UNIMPLEMENT;

  return entry_stubs;
}

size_t Isolate::CopyCodePages(size_t capacity, MemoryRange* code_pages_out) {
  LWNODE_RETURN_0;
}

#define CALLBACK_SETTER(ExternalName, Type, InternalName)                      \
  void Isolate::Set##ExternalName(Type callback) { LWNODE_UNIMPLEMENT_IGNORED; }

// CALLBACK_SETTER(FatalErrorHandler, FatalErrorCallback, exception_behavior)
void Isolate::SetFatalErrorHandler(FatalErrorCallback that) {
  IsolateWrap::fromV8(this)->SetFatalErrorHandler(that);
}

CALLBACK_SETTER(OOMErrorHandler, OOMErrorCallback, oom_behavior)
CALLBACK_SETTER(AllowCodeGenerationFromStringsCallback,
                AllowCodeGenerationFromStringsCallback,
                allow_code_gen_callback)
CALLBACK_SETTER(ModifyCodeGenerationFromStringsCallback,
                ModifyCodeGenerationFromStringsCallback,
                modify_code_gen_callback)
CALLBACK_SETTER(AllowWasmCodeGenerationCallback,
                AllowWasmCodeGenerationCallback,
                allow_wasm_code_gen_callback)

CALLBACK_SETTER(WasmModuleCallback, ExtensionCallback, wasm_module_callback)
CALLBACK_SETTER(WasmInstanceCallback, ExtensionCallback, wasm_instance_callback)

CALLBACK_SETTER(WasmStreamingCallback,
                WasmStreamingCallback,
                wasm_streaming_callback)

CALLBACK_SETTER(WasmThreadsEnabledCallback,
                WasmThreadsEnabledCallback,
                wasm_threads_enabled_callback)

CALLBACK_SETTER(WasmLoadSourceMapCallback,
                WasmLoadSourceMapCallback,
                wasm_load_source_map_callback)

CALLBACK_SETTER(WasmSimdEnabledCallback,
                WasmSimdEnabledCallback,
                wasm_simd_enabled_callback)

void Isolate::AddNearHeapLimitCallback(v8::NearHeapLimitCallback callback,
                                       void* data) {
  LWNODE_RETURN_VOID;
}

void Isolate::RemoveNearHeapLimitCallback(v8::NearHeapLimitCallback callback,
                                          size_t heap_limit) {
  LWNODE_RETURN_VOID;
}

void Isolate::AutomaticallyRestoreInitialHeapLimit(double threshold_percent) {
  LWNODE_RETURN_VOID;
}

bool Isolate::IsDead() {
  LWNODE_RETURN_FALSE;
}

bool Isolate::AddMessageListener(MessageCallback that, Local<Value> data) {
  LWNODE_RETURN_FALSE;
}

bool Isolate::AddMessageListenerWithErrorLevel(MessageCallback that,
                                               int message_levels,
                                               Local<Value> data) {
  LWNODE_CALL_TRACE();
  // @note: node use this only once with
  // Isolate::MessageErrorLevel::kMessageError |
  // Isolate::MessageErrorLevel::kMessageWarning
  IsolateWrap::fromV8(this)->AddMessageListenerWithErrorLevel(that);

  return true;
}

void Isolate::RemoveMessageListeners(MessageCallback that) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetFailedAccessCheckCallbackFunction(
    FailedAccessCheckCallback callback) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetCaptureStackTraceForUncaughtExceptions(
    bool capture, int frame_limit, StackTrace::StackTraceOptions options) {
  LWNODE_RETURN_VOID;
}

void Isolate::VisitExternalResources(ExternalResourceVisitor* visitor) {
  LWNODE_RETURN_VOID;
}

bool Isolate::IsInUse() {
  LWNODE_RETURN_FALSE;
}

void Isolate::VisitHandlesWithClassIds(PersistentHandleVisitor* visitor) {
  LWNODE_RETURN_VOID;
}

void Isolate::VisitWeakHandles(PersistentHandleVisitor* visitor) {
  LWNODE_RETURN_VOID;
}

void Isolate::SetAllowAtomicsWait(bool allow) {
  LWNODE_RETURN_VOID;
}

void v8::Isolate::DateTimeConfigurationChangeNotification(
    TimeZoneDetection time_zone_detection) {
  LWNODE_RETURN_VOID;
}

void v8::Isolate::LocaleConfigurationChangeNotification() {
  LWNODE_RETURN_VOID;
}

// static
std::unique_ptr<MicrotaskQueue> MicrotaskQueue::New(Isolate* isolate,
                                                    MicrotasksPolicy policy) {
  LWNODE_UNIMPLEMENT;
  return std::unique_ptr<MicrotaskQueue>();
}

MicrotasksScope::MicrotasksScope(Isolate* isolate, MicrotasksScope::Type type)
    : MicrotasksScope(isolate, nullptr, type) {}

MicrotasksScope::MicrotasksScope(Isolate* isolate,
                                 MicrotaskQueue* microtask_queue,
                                 MicrotasksScope::Type type)
    : isolate_(nullptr), microtask_queue_(nullptr), run_(false) {
  LWNODE_UNIMPLEMENT;
}

MicrotasksScope::~MicrotasksScope() {
  LWNODE_UNIMPLEMENT;
}

void MicrotasksScope::PerformCheckpoint(Isolate* v8_isolate) {
  auto lwIsolate = IsolateWrap::fromV8(v8_isolate);
  auto vmInstance = lwIsolate->vmInstance();

  GCHeap::ProcessingHoldScope scope;

  while (vmInstance->hasPendingJob()) {
    auto r = vmInstance->executePendingJob();
    if (!r.isSuccessful()) {
      __DLOG_EVAL_EXCEPTION(r);
      lwIsolate->SetTerminationOnExternalTryCatch();
      return;
    }
  }
}

int MicrotasksScope::GetCurrentDepth(Isolate* v8_isolate) {
  LWNODE_RETURN_0;
}

bool MicrotasksScope::IsRunningMicrotasks(Isolate* v8_isolate) {
  LWNODE_RETURN_FALSE;
}

String::Utf8Value::Utf8Value(v8::Isolate* isolate, v8::Local<v8::Value> obj)
    : str_(nullptr), length_(0) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto lwContext = lwIsolate->GetCurrentContext();

  TryCatch try_catch(isolate);

  auto r = Evaluator::execute(
      lwContext->get(),
      [](ExecutionStateRef* state, ValueRef* value) -> ValueRef* {
        return value->toString(state);
      },
      VAL(*obj)->value());

  if (!r.isSuccessful()) {
    return;
  }

  auto esString = r.result->asString();
  auto bufferData = esString->stringBufferAccessData();
  std::string str;
  if (bufferData.has8BitContent) {
    length_ = bufferData.length;
  } else {
    str = esString->toStdUTF8String();
    length_ = str.size();
  }

  str_ = new char[length_ + 1];

  if (bufferData.has8BitContent) {
    strncpy(str_, reinterpret_cast<const char*>(bufferData.buffer), length_);
  } else {
    strncpy(str_, str.data(), length_);
  }

  str_[length_] = '\0';
}

String::Utf8Value::~Utf8Value() {
  delete[] str_;
}

String::Value::Value(v8::Isolate* isolate, v8::Local<v8::Value> obj)
    : str_(nullptr), length_(0) {
  Local<String> str;
  if (!obj->ToString(isolate->GetCurrentContext()).ToLocal(&str)) {
    return;
  }

  length_ = str->Length();
  str_ = new u_int16_t[length_ + 1];
  str->Write(isolate, str_);
}

String::Value::~Value() {
  delete[] str_;
}

#define DEFINE_UNIMPLEMENTED_ERROR(NAME, name)                                 \
  Local<Value> Exception::NAME(v8::Local<v8::String> raw_message) {            \
    LWNODE_RETURN_LOCAL(Value);                                                \
  }

#define DEFINE_ERROR(NAME, ES_ERROR_CODE)                                      \
  Local<Value> Exception::NAME(v8::Local<v8::String> message) {                \
    auto lwIsolate = IsolateWrap::GetCurrent();                                \
    auto esError = ExceptionHelper::createErrorObject(                         \
        lwIsolate->GetCurrentContext()->get(),                                 \
        ErrorObjectRef::Code::ES_ERROR_CODE,                                   \
        VAL(*message)->value()->asString());                                   \
    return Utils::NewLocal<Value>(lwIsolate->toV8(), esError);                 \
  }

DEFINE_ERROR(RangeError, RangeError)
DEFINE_ERROR(ReferenceError, ReferenceError)
DEFINE_ERROR(SyntaxError, SyntaxError)
DEFINE_ERROR(TypeError, TypeError)
DEFINE_ERROR(Error, None)
DEFINE_UNIMPLEMENTED_ERROR(WasmCompileError, wasm_compile_error)
DEFINE_UNIMPLEMENTED_ERROR(WasmLinkError, wasm_link_error)
DEFINE_UNIMPLEMENTED_ERROR(WasmRuntimeError, wasm_runtime_error)

#undef DEFINE_UNIMPLEMENTED_ERROR
#undef DEFINE_ERROR

Local<Message> Exception::CreateMessage(Isolate* isolate,
                                        Local<Value> exception) {
  API_ENTER_NO_EXCEPTION(isolate);
  auto esContext = lwIsolate->GetCurrentContext()->get();
  auto esException = CVAL(*exception)->value();

  StringRef* messageString = StringRef::emptyString();
  if (esException->isString()) {
    messageString = esException->asString();
  }

  EvalResult r = Evaluator::execute(
      esContext,
      [](ExecutionStateRef* esState, StringRef* messageString) -> ValueRef* {
        auto object = StringObjectRef::create(esState);
        object->setPrimitiveValue(esState, messageString);
        return object;
      },
      messageString);
  LWNODE_CHECK(r.isSuccessful());

  if (esException->isObject()) {
    auto esExtraData = ObjectRefHelper::getExtraData(esException->asObject());
    if (esExtraData) {
      auto esExceptionData = esExtraData->asExceptionObjectData();
      ExtraDataHelper::setExtraData(r.result->asObject(), esExceptionData);
    } else {
      // FIXME: Check if missing extradata is ok. We print a warning
      // here until this issue is investigated
      LWNODE_LOG_WARN("esException does not have an extraData\n");
    }
  }

  return Utils::NewLocal<Message>(isolate, r.result);
}

Local<StackTrace> Exception::GetStackTrace(Local<Value> exception) {
  LWNODE_RETURN_LOCAL(StackTrace);
}
}  // namespace v8
