// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Included first as it collides with the X11 headers.
#include "flutter/shell/platform/linux/testing/linux_test.h"
#include "gtest/gtest.h"

#include "flutter/shell/platform/linux/fl_view_accessible.h"

class FlViewAccessibleDescriptionTest : public flutter::testing::LinuxTest {};

TEST_F(FlViewAccessibleDescriptionTest, ExposesHintAsDescription) {
  g_autoptr(FlViewAccessible) accessible = fl_view_accessible_new(engine, 456);

  FlutterSemanticsFlags flags{};
  FlutterSemanticsNode2 node{};
  node.id = 0;
  node.label = "Export";
  node.hint = "Writes the visible range to a PNG file";
  node.flags2 = &flags;

  FlutterSemanticsNode2* nodes[] = {&node};
  FlutterSemanticsUpdate2 update = {.node_count = 1, .nodes = nodes};
  fl_view_accessible_handle_update_semantics(accessible, &update);

  AtkObject* root =
      atk_object_ref_accessible_child(ATK_OBJECT(accessible), 0);
  ASSERT_NE(root, nullptr);
  EXPECT_STREQ(atk_object_get_description(root),
               "Writes the visible range to a PNG file");

  node.hint = "Writes the selected range to a PNG file";
  fl_view_accessible_handle_update_semantics(accessible, &update);
  EXPECT_STREQ(atk_object_get_description(root),
               "Writes the selected range to a PNG file");

  node.hint = "";
  fl_view_accessible_handle_update_semantics(accessible, &update);
  const gchar* description = atk_object_get_description(root);
  EXPECT_TRUE(description == nullptr || description[0] == '\0');

  g_object_unref(root);
}
