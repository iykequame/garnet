#!/bin/bash
# Copyright 2017 The Fuchsia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"/../lib/vars.sh

SSH_AUTH_SOCK="" scp -F "${FUCHSIA_BUILD_DIR}/ssh-keys/ssh_config" "$@"
