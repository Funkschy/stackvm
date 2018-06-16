// Copyright (c) 2018 Felix Schoeller
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef CALC_PARAMETERS_H
#define CALC_PARAMETERS_H

#define DEBUG_TRACE_GC 0
#define DEBUG_TRACE_EXECUTION 1
#define DEBUG_SHOW_DISASSEMBLY 1

// 500 KB
// TODO find good value
#define INITIAL_GC_THRESHOLD 512000

#define STACK_MAX 256
#define SCOPES_MAX 256

#endif //CALC_PARAMETERS_H
