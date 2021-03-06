// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <stdint.h>

#include <functional>
#include <optional>

#include "garnet/bin/zxdb/client/client_object.h"
#include "garnet/bin/zxdb/symbols/symbol_data_provider.h"
#include "garnet/public/lib/fxl/macros.h"
#include "garnet/public/lib/fxl/memory/weak_ptr.h"

namespace zxdb {

class ExprEvalContext;
class Location;
class Thread;

// Represents one stack frame.
//
// See also FrameFingerprint (the getter for a fingerprint is on Thread).
class Frame : public ClientObject {
 public:
  explicit Frame(Session* session);
  virtual ~Frame();

  fxl::WeakPtr<Frame> GetWeakPtr();

  // Guaranteed non-null.
  virtual Thread* GetThread() const = 0;

  // Returns the location of the stack frame code. This will be symbolized.
  virtual const Location& GetLocation() const = 0;

  // Returns the program counter of this frame. This may be faster than
  // GetLocation().address() since it doesn't need to be symbolized.
  virtual uint64_t GetAddress() const = 0;

  // Returns the value of the base pointer register computed by the backend.
  // Most callers will want to use the GetBasePointer() call below which
  // takes into account symbol information that may redirect the logical base
  // pointer to somewhere else. This function specifically returns the CPU
  // value.
  virtual uint64_t GetBasePointerRegister() const = 0;

  // The frame base pointer.
  //
  // This is not necessarily the "BP" register. The symbols can specify
  // an arbitrary frame base for a location and this value will reflect that.
  // For unsymbolized code or if the symbols do not declare a frame base, this
  // will default to the CPU register.
  //
  // In most cases the frame base is available synchronously (when it's in
  // a register which is the common case), but symbols can declare any DWARF
  // expression to compute the frame base.
  //
  // The synchronous version will return the base pointer if possible. If it
  // returns no value, code that can handle async calls can call the
  // asynchronous version to be notified when the value is available.
  virtual std::optional<uint64_t> GetBasePointer() const = 0;
  virtual void GetBasePointerAsync(std::function<void(uint64_t bp)> cb) = 0;

  // Returns the stack pointer at this location.
  virtual uint64_t GetStackPointer() const = 0;

  // Returns the SymbolDataProvider that can be used to evaluate symbols
  // in the context of this frame.
  virtual fxl::RefPtr<SymbolDataProvider> GetSymbolDataProvider() const = 0;

  // Returns the ExprEvalContext that can be used to evaluate expressions in
  // the context of this frame.
  virtual fxl::RefPtr<ExprEvalContext> GetExprEvalContext() const = 0;

 private:
  fxl::WeakPtrFactory<Frame> weak_factory_;

  FXL_DISALLOW_COPY_AND_ASSIGN(Frame);
};

}  // namespace zxdb
