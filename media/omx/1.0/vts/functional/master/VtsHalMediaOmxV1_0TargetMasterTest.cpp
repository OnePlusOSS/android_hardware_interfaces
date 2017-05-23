/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "media_omx_hidl_master_test"
#include <android-base/logging.h>

#include <VtsHalHidlTargetTestBase.h>
#include <android/hardware/media/omx/1.0/IOmx.h>
#include <android/hardware/media/omx/1.0/IOmxStore.h>
#include <android/hardware/media/omx/1.0/types.h>
#include <getopt.h>
#include <log/log.h>

using ::android::hardware::media::omx::V1_0::IOmxStore;
using ::android::hardware::media::omx::V1_0::IOmx;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;

// A class for test environment setup
class ComponentTestEnvironment : public ::testing::Environment {
   public:
    virtual void SetUp() {}
    virtual void TearDown() {}

    ComponentTestEnvironment() : instance("default") {}

    void setInstance(const char* _instance) { instance = _instance; }

    const hidl_string getInstance() const { return instance; }

    int initFromOptions(int argc, char** argv) {
        static struct option options[] = {
            {"instance", required_argument, 0, 'I'}, {0, 0, 0, 0}};

        while (true) {
            int index = 0;
            int c = getopt_long(argc, argv, "I:", options, &index);
            if (c == -1) {
                break;
            }

            switch (c) {
                case 'I':
                    setInstance(optarg);
                    break;
                case '?':
                    break;
            }
        }

        if (optind < argc) {
            fprintf(stderr,
                    "unrecognized option: %s\n\n"
                    "usage: %s <gtest options> <test options>\n\n"
                    "test options are:\n\n"
                    "-I, --instance: HAL instance to test\n",
                    argv[optind ?: 1], argv[0]);
            return 2;
        }
        return 0;
    }

   private:
    hidl_string instance;
};

static ComponentTestEnvironment* gEnv = nullptr;

class MasterHidlTest : public ::testing::VtsHalHidlTargetTestBase {
   public:
    virtual void SetUp() override {
        omxStore = nullptr;
        omxStore = ::testing::VtsHalHidlTargetTestBase::getService<IOmxStore>();
        ASSERT_NE(omxStore, nullptr);
        omx = nullptr;
        omx = omxStore->getOmx(gEnv->getInstance());
        ASSERT_NE(omx, nullptr);
    }

    virtual void TearDown() override {}

    sp<IOmxStore> omxStore;
    sp<IOmx> omx;

   protected:
    static void description(const std::string& description) {
        RecordProperty("description", description);
    }
};

void displayComponentInfo(hidl_vec<IOmx::ComponentInfo>& nodeList) {
    for (size_t i = 0; i < nodeList.size(); i++) {
        printf("%s | ", nodeList[i].mName.c_str());
        for (size_t j = 0; j < ((nodeList[i]).mRoles).size(); j++) {
            printf("%s ", nodeList[i].mRoles[j].c_str());
        }
        printf("\n");
    }
}

// list service attributes
TEST_F(MasterHidlTest, ListServiceAttr) {
    description("list service attributes");
    android::hardware::media::omx::V1_0::Status status;
    hidl_vec<IOmxStore::Attribute> attributes;
    EXPECT_TRUE(omxStore
                    ->listServiceAttributes([&status, &attributes](
                        android::hardware::media::omx::V1_0::Status _s,
                        hidl_vec<IOmxStore::Attribute> const& _nl) {
                        status = _s;
                        attributes = _nl;
                    })
                    .isOk());
    if (attributes.size() == 0)
        std::cerr << "[          ] Warning ! Attribute list empty \n";
}

// get node prefix
TEST_F(MasterHidlTest, getNodePrefix) {
    description("get node prefix");
    hidl_string prefix;
    omxStore->getNodePrefix(
        [&prefix](hidl_string const& _nl) { prefix = _nl; });
    if (prefix.empty())
        std::cerr << "[          ] Warning ! Node Prefix empty \n";
}

// list roles
TEST_F(MasterHidlTest, ListRoles) {
    description("list roles");
    hidl_vec<IOmxStore::RoleInfo> roleList;
    omxStore->listRoles([&roleList](hidl_vec<IOmxStore::RoleInfo> const& _nl) {
        roleList = _nl;
    });
    if (roleList.size() == 0)
        std::cerr << "[          ] Warning ! RoleInfo list empty \n";
}

// list components and roles
TEST_F(MasterHidlTest, ListNodes) {
    description("enumerate component and roles");
    android::hardware::media::omx::V1_0::Status status;
    hidl_vec<IOmx::ComponentInfo> nodeList;
    EXPECT_TRUE(
        omx->listNodes([&status, &nodeList](
                           android::hardware::media::omx::V1_0::Status _s,
                           hidl_vec<IOmx::ComponentInfo> const& _nl) {
               status = _s;
               nodeList = _nl;
           })
            .isOk());
    if (nodeList.size() == 0)
        std::cerr << "[          ] Warning ! ComponentInfo list empty \n";
    else
        displayComponentInfo(nodeList);
}

int main(int argc, char** argv) {
    gEnv = new ComponentTestEnvironment();
    ::testing::AddGlobalTestEnvironment(gEnv);
    ::testing::InitGoogleTest(&argc, argv);
    int status = gEnv->initFromOptions(argc, argv);
    if (status == 0) {
        status = RUN_ALL_TESTS();
        ALOGI("Test result = %d", status);
    }
    return status;
}
