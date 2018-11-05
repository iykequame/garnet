// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package templates

const TestBase = `
{{- define "GenerateTestBasePreamble" -}}
// WARNING: This file is machine generated by fidlgen.

#pragma once

{{ range .Headers -}}
#include <{{ . }}>
{{ end -}}

{{- range .Library }}
namespace {{ . }} {
{{- end }}
namespace testing {
{{ end }}

{{- define "GenerateTestBasePostamble" -}}
}  // namespace testing
{{- range .LibraryReversed }}
}  // namespace {{ . }}
{{- end }}
{{ end }}
`