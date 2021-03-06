// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GARNET_EXAMPLES_ESCHER_COMMON_DEMO_HARNESS_FUCHSIA_H_
#define GARNET_EXAMPLES_ESCHER_COMMON_DEMO_HARNESS_FUCHSIA_H_

#include <memory>

#include <lib/async-loop/cpp/loop.h>
#include <trace-provider/provider.h>

#include "garnet/bin/ui/input_reader/input_reader.h"
#include "garnet/examples/escher/common/demo_harness.h"
#include "lib/component/cpp/startup_context.h"
#include "lib/fidl/cpp/binding_set.h"

class DemoHarnessFuchsia : public DemoHarness,
                           fuchsia::ui::input::InputDeviceRegistry {
 public:
  DemoHarnessFuchsia(async::Loop* loop, WindowParams window_params);

  // |DemoHarness|
  void Run(Demo* demo) override;

  component::StartupContext* startup_context() {
    return startup_context_.get();
  }

 private:
  // |DemoHarness|
  // Called by Init().
  void InitWindowSystem() override;
  vk::SurfaceKHR CreateWindowAndSurface(
      const WindowParams& window_params) override;

  // |DemoHarness|
  // Called by Init() via CreateInstance().
  void AppendPlatformSpecificInstanceExtensionNames(
      InstanceParams* params) override;

  // |DemoHarness|
  // Called by Shutdown().
  void ShutdownWindowSystem() override;

  // |fuchsia::ui::input::InputDeviceRegistry|
  void RegisterDevice(fuchsia::ui::input::DeviceDescriptor descriptor,
                      fidl::InterfaceRequest<fuchsia::ui::input::InputDevice>
                          input_device) override;

  void RenderFrameOrQuit();

  // DemoHarnessFuchsia can work with a pre-existing message loop, and also
  // create its own if necessary.
  async::Loop* loop_;
  std::unique_ptr<async::Loop> owned_loop_;
  trace::TraceProvider trace_provider_;

  std::unique_ptr<component::StartupContext> startup_context_;
  mozart::InputReader input_reader_;
  fidl::BindingSet<fuchsia::ui::input::InputDevice,
                   std::unique_ptr<fuchsia::ui::input::InputDevice>>
      input_devices_;
};

#endif  // GARNET_EXAMPLES_ESCHER_COMMON_DEMO_HARNESS_FUCHSIA_H_
