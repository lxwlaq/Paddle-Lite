// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "lite/core/kernel.h"
#include "lite/core/op_registry.h"
#include "lite/operators/clip_op.h"

namespace paddle {
namespace lite {
namespace kernels {
namespace x86 {

class ClipCompute : public KernelLite<TARGET(kX86), PRECISION(kFloat)> {
 public:
  using param_t = operators::ClipParam;

  void Run() override {
    auto& param = Param<operators::ClipParam>();
    lite::Tensor* x = param.x;
    lite::Tensor* min_tensor = param.min_tensor;
    lite::Tensor* max_tensor = param.max_tensor;
    lite::Tensor* out = param.out;
    float min = param.min;
    float max = param.max;

    if (min_tensor != nullptr) {
      min = min_tensor->data<float>()[0];
    }
    if (max_tensor != nullptr) {
      max = max_tensor->data<float>()[0];
    }

    const float* x_ptr = x->data<float>();
    float* out_ptr = out->mutable_data<float>();
    int64_t num = x->numel();

    for (int i = 0; i < num; i++) {
      float tmp = x_ptr[i] > min ? x_ptr[i] : min;
      out_ptr[i] = tmp < max ? tmp : max;
    }
  }

  virtual ~ClipCompute() = default;
};

}  // namespace x86
}  // namespace kernels
}  // namespace lite
}  // namespace paddle
