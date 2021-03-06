#!/bin/bash
# Copyright 2018 The Fuchsia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

set -e

source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"/vars.sh

# We want to give the user time to accept Darwin's firewall dialog.
if [[ "$(uname -s)" = "Darwin" ]]; then
  "${ZIRCON_TOOLS_DIR}/netaddr" "$@"
else
  "${ZIRCON_TOOLS_DIR}/netaddr" --nowait "$@"
fi
