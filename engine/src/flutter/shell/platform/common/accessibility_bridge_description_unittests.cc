// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "accessibility_bridge.h"

#include "gtest/gtest.h"

#include "test_accessibility_bridge.h"

namespace flutter {
namespace testing {

TEST(AccessibilityBridgeDescriptionTest, ExposesHintAsDescription) {
  std::shared_ptr<TestAccessibilityBridge> bridge =
      std::make_shared<TestAccessibilityBridge>();

  FlutterSemanticsFlags flags{};
  FlutterSemanticsNode2 node{};
  node.id = 0;
  node.label = "Export";
  node.hint = "Writes the visible range to a PNG file";
  node.flags2 = &flags;

  bridge->AddFlutterSemanticsNodeUpdate(node);
  bridge->CommitUpdates();

  auto platform_node = bridge->GetFlutterPlatformNodeDelegateFromID(0).lock();
  ASSERT_TRUE(platform_node);
  EXPECT_EQ(platform_node->GetData().GetStringAttribute(
                ax::mojom::StringAttribute::kDescription),
            "Writes the visible range to a PNG file");

  node.hint = "Writes the selected range to a PNG file";
  bridge->AddFlutterSemanticsNodeUpdate(node);
  bridge->CommitUpdates();

  platform_node = bridge->GetFlutterPlatformNodeDelegateFromID(0).lock();
  ASSERT_TRUE(platform_node);
  EXPECT_EQ(platform_node->GetData().GetStringAttribute(
                ax::mojom::StringAttribute::kDescription),
            "Writes the selected range to a PNG file");

  node.hint = "";
  bridge->AddFlutterSemanticsNodeUpdate(node);
  bridge->CommitUpdates();

  platform_node = bridge->GetFlutterPlatformNodeDelegateFromID(0).lock();
  ASSERT_TRUE(platform_node);
  EXPECT_TRUE(platform_node->GetData()
                  .GetStringAttribute(ax::mojom::StringAttribute::kDescription)
                  .empty());
}

}  // namespace testing
}  // namespace flutter
