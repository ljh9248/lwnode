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

#pragma once

#include <v8.h>
#include <functional>
#include <memory>

namespace Escargot {
class ContextRef;
class ValueRef;
}  // namespace Escargot

namespace LWNode {

void InitializeProcessMethods(v8::Local<v8::Object> target,
                              v8::Local<v8::Context> context);

void IdleGC(v8::Isolate* isolate = nullptr);
void initDebugger();
bool dumpSelfMemorySnapshot();

class MessageLoop {
  using WakeupMainloopHandler = std::function<void()>;

  struct PlatformHandler {
    WakeupMainloopHandler wakeup{nullptr};
  };

 public:
  static MessageLoop* GetInstance();

  // Prepare callback is called right before polling I/O events
  void onPrepare(v8::Isolate* isolate);

  void wakeupMainloopOnce();
  void setWakeupMainloopOnceHandler(PlatformHandler handler);

 private:
  MessageLoop();

  PlatformHandler platformHandler_;

  class Internal;
  std::unique_ptr<Internal> internal_;
};

class Utils {
 public:
  static Escargot::ContextRef* ToEsContext(v8::Context* context);

  static v8::Local<v8::Value> NewLocal(v8::Isolate* isolate,
                                       Escargot::ValueRef* ptr);
};

}  // namespace LWNode
