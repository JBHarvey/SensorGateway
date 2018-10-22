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

#ifndef SENSORGATEWAY_AWLDATATRANSLATORTEST_CPP
#define SENSORGATEWAY_AWLDATATRANSLATORTEST_CPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include "test/utilities/files/AWLMessagesFileManager.h"
#include "test/utilities/files/FrameFileManager.h"
#include "test/utilities/stub/AWLMessageStub.h"
#include "test/utilities/stub/FrameStub.h"
#include "test/utilities/mock/ArbitraryDataSinkMock.hpp"
#include "sensor-gateway/data-translation/AWLTranslationStrategy.h"

using TestUtilities::AWLMessagesFileManager;
using TestUtilities::FrameFileManager;
using Stub::createAWLMessageStub;
using Stub::createFrameStub;
using DataTranslation::AWLTranslationStrategy;

using FrameSinkMock = Mock::ArbitraryDataSinkMock<DataFlow::Frame>;
using FrameProcessingScheduler = DataFlow::DataProcessingScheduler<DataFlow::Frame, FrameSinkMock, 1>;

class AWLTranslationStrategyTest : public ::testing::Test {
protected:
    char const* AWLDATAS_INPUT_FILE_NAME = "AWLMessagesInputFile.txt";
    char const* EXPECTED__FRAMES_OUTPUT_FILE_NAME = "ExpectedFramesOutputFile.txt";

    virtual void SetUp() {
        auto awlMessages = createAWLMessageStub();
        auto Frames = createFrameStub();
        awlMessagesFileManager.writeFileWithMessages(awlMessages, AWLDATAS_INPUT_FILE_NAME);
        frameFileManager.writeFileWithMessages(Frames, EXPECTED__FRAMES_OUTPUT_FILE_NAME);
    }

    AWLMessagesFileManager awlMessagesFileManager;
    FrameFileManager frameFileManager;
};

TEST_F(AWLTranslationStrategyTest,
       given_someInputFileContainingValidAWLMessages_when_translatingAWLMessagesIntoFrames_then_returnCorrespondingSpiritFramesOutputFile) {
    auto ACTUAL__FRAMES_OUTPUT_FILE_NAME = "ActualFramesOutputFile.txt";
    AWLTranslationStrategy awlMessageTranslator;
    FrameSinkMock frameSinkMock(1);
    FrameProcessingScheduler scheduler(&frameSinkMock);
    awlMessageTranslator.linkConsumer(&scheduler);
    int counter = 0;

    auto messages = awlMessagesFileManager.readMessagesFromFile(AWLDATAS_INPUT_FILE_NAME);
    for (auto message : messages) {
        awlMessageTranslator.translateMessage(std::move(message));
        ++counter;
    }

    scheduler.terminateAndJoin();
    frameSinkMock.waitConsumptionToBeReached();

    auto frames = frameSinkMock.getConsumedData();

    frameFileManager.writeFileWithFrames(frames, ACTUAL__FRAMES_OUTPUT_FILE_NAME);
    ASSERT_TRUE(frameFileManager.areFilesEqual(EXPECTED__FRAMES_OUTPUT_FILE_NAME,
                                               ACTUAL__FRAMES_OUTPUT_FILE_NAME));
}

#endif //SENSORGATEWAY_AWLDATATRANSLATORTEST_CPP
