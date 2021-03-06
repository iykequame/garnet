// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package typestest

import (
	"encoding/json"
	"io/ioutil"
	"path/filepath"
	"runtime"
	"strconv"
	"strings"
	"testing"

	"fidl/compiler/backend/types"

	"github.com/google/go-cmp/cmp"
)

var basePath = func() string {
	for i := 0; i < 10; i++ {
		_, file, _, ok := runtime.Caller(i)
		if !ok {
			break
		}
		if strings.HasSuffix(file, "testutil.go") {
			path, _ := filepath.Split(file)
			return path
		}
	}
	panic("")
}()

// GetExample retrieves an example by filename, and parses it.
func GetExample(filename string) types.Root {
	var (
		data = GetGolden(filename)
		fidl types.Root
	)
	if err := json.Unmarshal(data, &fidl); err != nil {
		panic(err)
	}
	return fidl
}

// GetGolden retrieves a golden example by filename, and returns the raw
// content.
func GetGolden(filename string) []byte {
	data, err := ioutil.ReadFile(filepath.Join(basePath, filename))
	if err != nil {
		panic(err)
	}
	return data
}

// AssertCodegenCmp assert that the actual codegen matches the expected codegen.
func AssertCodegenCmp(t *testing.T, expected, actual []byte) {
	var (
		splitExpected = strings.Split(strings.TrimSpace(string(expected)), "\n")
		splitActual   = strings.Split(strings.TrimSpace(string(actual)), "\n")
	)
	if diff := cmp.Diff(splitActual, splitExpected); diff != "" {
		t.Errorf("unexpected impl.go: %s\ngot: %s", diff, string(actual))
	}
}

func NumericLiteral(value int) types.Constant {
	return types.Constant{
		Kind: types.LiteralConstant,
		Literal: types.Literal{
			Kind:  types.NumericLiteral,
			Value: strconv.Itoa(value),
		},
	}
}

func BoolLiteral(val bool) types.Constant {
	var kind types.LiteralKind
	if val {
		kind = types.TrueLiteral
	} else {
		kind = types.FalseLiteral
	}
	return types.Constant{
		Kind: types.LiteralConstant,
		Literal: types.Literal{
			Kind: kind,
		},
	}
}

func PrimitiveType(kind types.PrimitiveSubtype) types.Type {
	return types.Type{
		Kind:             types.PrimitiveType,
		PrimitiveSubtype: kind,
	}
}

func ArrayType(elementType types.Type, elementCount int) types.Type {
	return types.Type{
		Kind:         types.ArrayType,
		ElementType:  &elementType,
		ElementCount: &elementCount,
	}
}

func VectorType(elementType types.Type, elementCount *int) types.Type {
	return types.Type{
		Kind:         types.VectorType,
		ElementType:  &elementType,
		ElementCount: elementCount,
	}
}

func StringType(elementCount *int) types.Type {
	return types.Type{
		Kind:         types.StringType,
		ElementCount: elementCount,
	}
}

func HandleType() types.Type {
	return types.Type{
		Kind: types.HandleType,
	}
}

func Nullable(t types.Type) types.Type {
	t.Nullable = true
	return t
}
