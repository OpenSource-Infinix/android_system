//
// Copyright (C) 2012 The Android Open Source Project
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

#ifndef SHILL_MOCK_LINK_MONITOR_H_
#define SHILL_MOCK_LINK_MONITOR_H_

#include "shill/link_monitor.h"

#include <gmock/gmock.h>

namespace shill {

class MockLinkMonitor : public LinkMonitor {
 public:
  MockLinkMonitor();
  ~MockLinkMonitor() override;

  MOCK_METHOD0(Start, bool());
  MOCK_METHOD0(Stop, void());
  MOCK_METHOD0(OnAfterResume, void());
  MOCK_CONST_METHOD0(GetResponseTimeMilliseconds, int());
  MOCK_CONST_METHOD0(IsGatewayFound, bool());

 private:
  DISALLOW_COPY_AND_ASSIGN(MockLinkMonitor);
};

}  // namespace shill

#endif  // SHILL_MOCK_LINK_MONITOR_H_
