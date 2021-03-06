// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "garnet/drivers/bluetooth/lib/common/device_class.h"

#include "gtest/gtest.h"

namespace btlib {
namespace common {
namespace {

struct TestPayload {
  uint8_t arg0;
  DeviceClass class_of_device;
} __attribute__((packed));

TEST(DeviceClassTest, CastFromBytes) {
  std::array<uint8_t, 4> bytes{{10, 0x06, 0x02, 0x02}};
  EXPECT_EQ(bytes.size(), sizeof(TestPayload));

  auto* test_payload = reinterpret_cast<TestPayload*>(bytes.data());
  EXPECT_EQ(10, test_payload->arg0);
  EXPECT_EQ(DeviceClass::MajorClass::kPhone,
            test_payload->class_of_device.major_class());
}

TEST(DeviceClassTest, ToString) {
  DeviceClass device;

  EXPECT_EQ("Unspecified", device.ToString());

  device = DeviceClass({0x06, 0x02, 0x02});

  EXPECT_EQ("Phone", device.ToString());
}

TEST(DeviceClassTest, Comparison) {
  DeviceClass class1(DeviceClass::MajorClass::kPhone);
  DeviceClass class2(DeviceClass::MajorClass::kPhone);
  DeviceClass class3(DeviceClass::MajorClass::kComputer);
  EXPECT_EQ(class1, class2);
  EXPECT_NE(class2, class3);
}

}  // namespace
}  // namespace common
}  // namespace btlib
