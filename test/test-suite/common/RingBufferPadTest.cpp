/**
	Copyright 2014-2018 Phantom Intelligence Inc.

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#ifndef SPIRITSENSORGATEWAY_RINGBUFFERPADTEST_H
#define SPIRITSENSORGATEWAY_RINGBUFFERPADTEST_H

#include <gtest/gtest.h>

#include "spirit-sensor-gateway/common/RingBufferPad.hpp"
#include "data-model/DataModelFixture.h"

using DataFlow::RingBufferPad;
using TestFunctions::DataTestUtil;

typedef RingBufferPad<ExampleDataModel::NativeData> NativePad;

/**
 * @brief Test Fixture meant to ensure correct behavior of RingBufferPad.
 * @note A RingBuffer is used to implement the different tested functions
 */
class RingBufferPadTest : public ::testing::Test {

protected:
    RingBufferPadTest() = default;

    virtual ~RingBufferPadTest() = default;
};


TEST_F(RingBufferPadTest, given_aNewPad_when_fetchingNextPad_then_returnsSelf) {
    auto initialPad = NativePad();

    auto testedNextPad = initialPad.next();

    ASSERT_EQ(testedNextPad, &initialPad);
}


TEST_F(RingBufferPadTest, given_aNextPad_when_fetchingNextPad_then_returnsTheNextPad) {
    auto initialPad = NativePad();
    auto testedPad = NativePad();
    auto nextPadPointer = &initialPad;
    testedPad.setNext(nextPadPointer);

    auto testedNextPad = testedPad.next();

    ASSERT_EQ(testedNextPad, nextPadPointer);
}

TEST_F(RingBufferPadTest, given_aNextPad_when_attemptingToSetANewNextPad_then_theNewNextPadIsNotAdded) {
    auto initialPad = NativePad();
    auto ignoredPad = NativePad();
    auto testedPad = NativePad();
    auto nextPadPointer = &initialPad;
    testedPad.setNext(nextPadPointer);

    testedPad.setNext(&ignoredPad);

    auto testedNextPad = testedPad.next();
    ASSERT_EQ(testedNextPad, nextPadPointer);
}


TEST_F(RingBufferPadTest, given_data_when_read_then_returnsData) {
    auto testedPad = NativePad();
    auto data = DataTestUtil::initNativeData();
    testedPad.write(std::move(data));

    auto readData = testedPad.read();

    ASSERT_EQ(readData, data);
}

#endif //SPIRITSENSORGATEWAY_RINGBUFFERPADTEST_H

