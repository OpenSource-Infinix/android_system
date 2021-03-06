//
// Copyright (C) 2011 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_COMMON_MOCK_CERTIFICATE_CHECKER_H_
#define UPDATE_ENGINE_COMMON_MOCK_CERTIFICATE_CHECKER_H_

#include <gmock/gmock.h>
#include <openssl/ssl.h>

#include "update_engine/common/certificate_checker.h"

namespace chromeos_update_engine {

class MockOpenSSLWrapper : public OpenSSLWrapper {
 public:
  MOCK_CONST_METHOD4(GetCertificateDigest,
                     bool(X509_STORE_CTX* x509_ctx,
                          int* out_depth,
                          unsigned int* out_digest_length,
                          uint8_t* out_digest));
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_MOCK_CERTIFICATE_CHECKER_H_
