/**
	Copyright 2014-2019 Phantom Intelligence Inc.

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
#include <sensor-gateway/application/SensorAccessLink.hpp>

#include "sensor-gateway/data-translation/DataTranslator.hpp"
#include "test/utilities/data-model/DataModelFixture.h"

#include "test/utilities/mock/ArbitraryDataSinkMock.hpp"
#include "test/utilities/mock/DataTranslationStrategyMock.hpp"
#include "test/utilities/mock/ErrorThrowingDataTranslationStrategyMock.hpp"

class DataTranslatorTest : public ::testing::Test {

public :

    using DataStructures = Sensor::Test::RealisticImplementation::Structures;
    using GatewayStructures = GatewayStructuresFor<DataStructures>;

protected:

    using DataTestUtil = TestFunctions::DataTestUtil;
    using DataTranslator = SensorAccessLinkElement::DataTranslator<DataStructures, GatewayStructures>;

    using Error = ErrorHandling::SensorAccessLinkError;
    using ErrorSinkMock = Mock::ArbitraryDataSinkMock<Error>;
    using ThrowingDataTranslationStrategy = Mock::ErrorThrowingDataTranslationStrategyMock<DataStructures>;

    DataTranslatorTest() = default;

    ~DataTranslatorTest() override = default;

};

namespace DataTranslatorTestMock {

    using DataStructures = DataTranslatorTest::DataStructures;
    using GatewayStructures = DataTranslatorTest::GatewayStructures;
    using DataTranslationStrategy = DataTranslation::DataTranslationStrategy<DataStructures, GatewayStructures>;

    class MockDataTranslationStrategy final :
            public DataTranslationStrategy {

    protected:

        using super = DataTranslationStrategy;
        using super::SensorMessage;
        using super::SensorRawData;

        using super::MessageSource;
        using super::RawDataSource;

    public:

        MockDataTranslationStrategy() :
                translateMessageCalled(false),
                translateRawDataCalled(false),
                receivedSensorMessage(DataStructures::Message::returnDefaultData()),
                receivedSensorRawData(DataStructures::RawData::returnDefaultData()) {
        };

        ~MockDataTranslationStrategy() noexcept = default;

        void translateMessage(super::SensorMessage&& sensorMessage) override {
            receivedSensorMessage = sensorMessage;
            translateMessageCalled.store(true);
        };

        void translateRawData(super::SensorRawData&& sensorRawData) override {
            receivedSensorRawData = sensorRawData;
            translateRawDataCalled.store(true);
        }

        super::SensorMessage
        translateControlMessageToSensorMessageRequest(
                super::SensorControlMessage&& sensorControlMessage) override {
            return super::SensorMessage::returnDefaultData();
        }

        super::SensorControlMessage
        translateSensorMessageToControlMessageResult(super::SensorMessage&& sensorMessage) override {
            return super::SensorControlMessage::returnDefaultData();
        }

        bool hasTranslateMessageBeenCalledWithRightSensorMessage(
                super::SensorMessage const& expectedSensorMessage) const {
            return (translateMessageCalled.load() && (expectedSensorMessage == receivedSensorMessage));
        };

        bool hasTranslateRawDataBeenCalledWithRightSensorRawData(
                super::SensorRawData const& expectedSensorRawData) const {
            return (translateRawDataCalled.load() && (expectedSensorRawData == receivedSensorRawData));

        };

    private:

        AtomicFlag translateMessageCalled;
        AtomicFlag translateRawDataCalled;

        super::SensorMessage receivedSensorMessage;
        super::SensorRawData receivedSensorRawData;

    };
}

TEST_F(DataTranslatorTest,
       given_aTranslationStrategy_when_consumeMessage_then_callsTranslateMessageInStrategyWithTheConsumedMessage) {
    auto message = DataTestUtil::createRandomRealisticMessageWithEmptyTimestamps();
    auto copy = DataStructures::Message(message);

    DataTranslatorTestMock::MockDataTranslationStrategy mockStrategy;
    DataTranslator dataTranslator(&mockStrategy);

    dataTranslator.consume(std::move(message));
    auto strategyCalledWithTheRightMessage = mockStrategy.hasTranslateMessageBeenCalledWithRightSensorMessage(copy);

    ASSERT_TRUE(strategyCalledWithTheRightMessage);
}

TEST_F(DataTranslatorTest,
       given_aTranslationStrategy_when_consumeRawData_then_callsTranslateRawDataInStrategyWithTheConsumedRawData) {
    auto data = DataTestUtil::createRandomRealisticRawData();
    auto copy = DataStructures::RawData(data);

    DataTranslatorTestMock::MockDataTranslationStrategy mockStrategy;
    DataTranslator dataTranslator(&mockStrategy);

    dataTranslator.consume(std::move(data));
    auto strategyCalledWithTheRightRawData = mockStrategy.hasTranslateRawDataBeenCalledWithRightSensorRawData(copy);

    ASSERT_TRUE(strategyCalledWithTheRightRawData);
}

TEST_F(DataTranslatorTest,
       given_aThrowingTranslationStrategy_when_consumeMessage_then_producesAnErrorCorrectlyFormatted) {
    auto numberOfErrorToReceive = 1;
    Mock::ArbitraryDataSinkMock<Error> sink(numberOfErrorToReceive);
    DataFlow::DataProcessingScheduler<Error, ErrorSinkMock, ONLY_ONE_PRODUCER> scheduler(&sink);

    ThrowingDataTranslationStrategy throwingMockStrategy;
    DataTranslator dataTranslator(&throwingMockStrategy);

    dataTranslator.linkConsumer(&scheduler);
    auto data = DataTestUtil::createRandomRealisticMessageWithEmptyTimestamps();
    dataTranslator.consume(std::move(data));
    sink.waitConsumptionToBeReached();

    scheduler.terminateAndJoin();

    auto producedErrors = sink.getConsumedData();

    Error expectedError = ErrorHandling::SensorAccessLinkError(
            ErrorHandling::Origin::TRANSLATE_MESSAGE + ErrorHandling::Message::SEPARATOR + throwingMockStrategy.ORIGIN,
            ErrorHandling::Category::TRANSLATION_ERROR,
            ErrorHandling::Severity::ERROR,
            throwingMockStrategy.ERROR_CODE,
            throwingMockStrategy.MESSAGE_ERROR_MESSAGE);

    for (auto t = 0; t < numberOfErrorToReceive; ++t) {
        ASSERT_EQ(producedErrors.front(), expectedError);
        producedErrors.pop_front();
    }
}

TEST_F(DataTranslatorTest,
       given_aThrowingTranslationStrategy_when_consumeRawData_then_producesAnErrorCorrectlyFormatted) {
    auto numberOfErrorToReceive = 1;
    Mock::ArbitraryDataSinkMock<Error> sink(numberOfErrorToReceive);
    DataFlow::DataProcessingScheduler<Error, ErrorSinkMock, ONLY_ONE_PRODUCER> scheduler(&sink);

    ThrowingDataTranslationStrategy throwingMockStrategy;
    DataTranslator dataTranslator(&throwingMockStrategy);

    Error expectedError = ErrorHandling::SensorAccessLinkError(
            ErrorHandling::Origin::TRANSLATE_RAWDATA + ErrorHandling::Message::SEPARATOR + throwingMockStrategy.ORIGIN,
            ErrorHandling::Category::TRANSLATION_ERROR,
            ErrorHandling::Severity::ERROR,
            throwingMockStrategy.ERROR_CODE,
            throwingMockStrategy.RAW_DATA_ERROR_MESSAGE);

    auto data = DataTestUtil::createRandomRealisticRawData();

    dataTranslator.linkConsumer(&scheduler);
    data = DataTestUtil::createRandomRealisticRawData();
    dataTranslator.consume(std::move(data));
    sink.waitConsumptionToBeReached();

    scheduler.terminateAndJoin();

    auto producedErrors = sink.getConsumedData();

    for (auto t = 0; t < numberOfErrorToReceive; ++t) {
        ASSERT_EQ(producedErrors.front(), expectedError);
        producedErrors.pop_front();
    }
}

TEST_F(DataTranslatorTest,
       given_aSensorControlMessage_when_translateAndSendToSensor_then_callsTranslationStrategyWithSensorControlMessage) {
    using AvailableParameters = Sensor::FakeParameter::FruitBasketParameters;
    AvailableParameters availableParameters;
    auto validParameterName = Given::aValidParameterName<AvailableParameters>();
    auto sensorControlMessage = availableParameters.createGetParameterValueControlMessageFor(validParameterName);
    auto messageCopy = decltype(availableParameters.createGetParameterValueControlMessageFor(validParameterName))(
            sensorControlMessage);

    using LoopBackDataTranslationStrategyMock = Mock::DataTranslationStrategyMock<DataStructures, GatewayStructures>;
    using LoopBackDataTranslator = SensorAccessLinkElement::DataTranslator<DataStructures, GatewayStructures>;
    LoopBackDataTranslationStrategyMock dataTranslationStrategyMock;

    LoopBackDataTranslator dataTranslator(&dataTranslationStrategyMock);

    dataTranslator.translateAndSendToSensor(std::move(sensorControlMessage));

    dataTranslationStrategyMock.waitUntilTranslateControlMessageToSensorMessageRequestInvocation();

    auto strategyCalled = dataTranslationStrategyMock.hasTranslateControlMessageToSensorMessageRequestBeenCalledWith(
            messageCopy);

    ASSERT_TRUE(strategyCalled);
}


// Medium test
TEST_F(DataTranslatorTest,
       given_aSensorControlMessage_when_translateAndSendToSensor_then_producesTheResultOfTranslation) {
    using AvailableParameters = Sensor::FakeParameter::FruitBasketParameters;
    AvailableParameters availableParameters;
    auto validParameterName = Given::aValidParameterName<AvailableParameters>();
    auto sensorControlMessage = availableParameters.createGetParameterValueControlMessageFor(validParameterName);
    auto messageCopy = decltype(availableParameters.createGetParameterValueControlMessageFor(validParameterName))(
            sensorControlMessage);

    using LoopBackDataTranslationStrategyMock = Mock::DataTranslationStrategyMock<DataStructures, GatewayStructures>;
    using LoopBackDataTranslator = SensorAccessLinkElement::DataTranslator<DataStructures, GatewayStructures>;
    LoopBackDataTranslationStrategyMock dataTranslationStrategyMock;
    using FakeSensorMessage = typename DataStructures::Message;

    LoopBackDataTranslator dataTranslator(&dataTranslationStrategyMock);

    using FakeSensorMessageSinkMock =Mock::ArbitraryDataSinkMock<FakeSensorMessage>;
    FakeSensorMessageSinkMock sink(1);
    DataFlow::DataProcessingScheduler<FakeSensorMessage, FakeSensorMessageSinkMock, ONLY_ONE_PRODUCER, ONLY_ONE_CONSUMER, DataStructures::ASYNC_REQUEST_BUFFER_SIZE_BEFORE_TRANSMISSION_TO_SENSOR> scheduler(
            &sink);

    dataTranslator.linkConsumer(&scheduler);

    // TODO : generate a better "random" fakeTranslationResult, depending on the `Structure::Message` type
    FakeSensorMessage fakeTranslationResult;
    fakeTranslationResult.messageId = 42;
    fakeTranslationResult.sensorId = 15;

    dataTranslationStrategyMock
            .onTranslateControlMessageToSensorMessageRequest(std::move(messageCopy))
            ->returnThisSensorMessage(fakeTranslationResult);


    dataTranslator.translateAndSendToSensor(std::move(sensorControlMessage));

    sink.waitConsumptionToBeReached();
    scheduler.terminateAndJoin();

    auto strategyCalled = sink.hasConsumed(fakeTranslationResult);

    ASSERT_TRUE(strategyCalled);
}
