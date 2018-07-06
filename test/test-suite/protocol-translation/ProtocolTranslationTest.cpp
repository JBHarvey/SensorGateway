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

#include <gtest/gtest.h>
#include <spirit-sensor-gateway/domain/AWLMessage.h>
#include <spirit-sensor-gateway/domain/Frame.h>

#include "spirit-sensor-gateway/protocol-translation/MessageTranslator.hpp"


/**
 * @brief Test Fixture meant to ensure correct behavior of ProtocolTranslation.
 * @note A RingBuffer is used to implement the different tested functions
 */

using DataFlow::AWLMessage;
using DataFlow::Frame;
using ProtocolTranslation::MessageTranslationStrategy;
using SensorAccessLinkElement::MessageTranslator;

class ProtocolTranslationTest : public ::testing::Test {

protected:

    AWLMessage awlMessage = givenOneMessage();

    ProtocolTranslationTest() = default;

    virtual ~ProtocolTranslationTest() = default;

    AWLMessage givenOneMessage() const noexcept;

    };

class MockMessageTranslationStrategy final : public MessageTranslationStrategy<AWLMessage, Frame> {

public:

    MockMessageTranslationStrategy() : translateBasicMessageCalled(false),
                                       receivedInputMessage(AWLMessage::returnDefaultData()) {};

    void translateBasicMessage(AWLMessage* inputMessage) override {
        receivedInputMessage = *inputMessage;
        translateBasicMessageCalled.store(true);

    };

    bool hasTranslateBasicMessageBeenCalledWithRightInputMessage(AWLMessage expectedInputMessage) const {

        return (translateBasicMessageCalled.load() && (expectedInputMessage == receivedInputMessage));

    };



private:

    AtomicFlag translateBasicMessageCalled;
    AWLMessage receivedInputMessage;

};

TEST_F(ProtocolTranslationTest,
       given_aMessageTranslationStrategy_when_consumingABasicAWLMessage_then_callsTranslateBasicMessageInStrategy) {

    MockMessageTranslationStrategy mockStrategy;
    MessageTranslator<AWLMessage,Frame> messageTranslator(mockStrategy);

    messageTranslator.consume(std::move(awlMessage));
    auto strategyHasBeenCalledWithRightParameter = mockStrategy.hasTranslateBasicMessageBeenCalledWithRightInputMessage(awlMessage);
    ASSERT_TRUE(strategyHasBeenCalledWithRightParameter);
}

AWLMessage ProtocolTranslationTest::givenOneMessage() const noexcept {
    int64_t const ARBITRARY_ID = 42;
    uint64_t const ARBITRARY_LENGTH = 7;
    int64_t const ARBITRARY_TIMESTAMP = 101010;

    AWLMessage message = AWLMessage::returnDefaultData();
    message.id = ARBITRARY_ID;
    message.length = ARBITRARY_LENGTH;
    message.timestamp = ARBITRARY_TIMESTAMP;
    return message;
}