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

#ifndef SENSORGATEWAY_SERVERCOMMUNICATORTEST_CPP
#define SENSORGATEWAY_SERVERCOMMUNICATORTEST_CPP

#include <gtest/gtest.h>
#include <list>

#include "sensor-gateway/server-communication/ServerCommunicator.hpp"
#include "test/utilities/data-model/DataModelFixture.h"
#include "test/utilities/mock/ArbitraryDataSinkMock.hpp"
#include "test/utilities/mock/ErrorThrowingServerCommunicationStrategyMock.hpp"
#include "test/utilities/assertion/TimeAssertion.hpp"

using TestFunctions::DataTestUtil;

namespace ServerCommunicatorTestMock {

    using ServerCommunication::ServerCommunicationStrategy;

    class MockServerCommunicatorStrategy : public ServerCommunicationStrategy<Sensor::Test::Simple::Structures> {

    protected:

        using super = ServerCommunicationStrategy<Sensor::Test::Simple::Structures>;

        using super::Message;
        using super::RawData;

    public:

        MockServerCommunicatorStrategy() :
                openConnectionCalled(false),
                sendMessageCalled(false),
                sendRawDataCalled(false),
                closeConnectionCalled(false),
                sentMessage(Message::returnDefaultData()),
                sentRawData(RawData::returnDefaultData()) {

        }

        ~MockServerCommunicatorStrategy() noexcept override = default;

        void sendMessage(Message&& message) override {
            sendMessageCalled.store(true);
            sentMessage = message;
        }

        void sendRawData(RawData&& rawData) override {
            sendRawDataCalled.store(true);
            sentRawData = rawData;
        }

        void openConnection(std::string const& serverAddress) override {
            openConnectionCalled.store(true);
        }

        void closeConnection() override {
            closeConnectionCalled.store(true);
        }

        bool hasOpenConnectionBeenCalled() const {
            return openConnectionCalled.load();
        }

        bool hasCloseConnectionBeenCalled() const {
            return closeConnectionCalled.load();
        }

        bool hasSendMessageBeenCalled() const {
            return sendMessageCalled.load();
        }

        bool hasSendRawDataBeenCalled() const {
            return sendRawDataCalled.load();
        }

        Message getSentMessage() const {
            return sentMessage;
        }

        RawData getSentRawData() const {
            return sentRawData;
        }


        AtomicFlag openConnectionCalled;
        AtomicFlag sendMessageCalled;
        AtomicFlag sendRawDataCalled;
        AtomicFlag closeConnectionCalled;

    protected:

        Message sentMessage;
        RawData sentRawData;
    };
}

class ServerCommunicatorTest : public ::testing::Test {
public:
    using Error = ErrorHandling::SensorAccessLinkError;
    using ErrorSinkMock = Mock::ArbitraryDataSinkMock<Error>;
    using ErrorProcessingScheduler = DataFlow::DataProcessingScheduler<Error, ErrorSinkMock, 1>;
    using ThrowingServerCommunicationStrategy = Mock::ErrorThrowingServerCommunicationStrategyMock<Sensor::Test::Simple::Structures>;

protected:

    using ServerCommunicator = SensorAccessLinkElement::ServerCommunicator<Sensor::Test::Simple::Structures>;
    using Message = Sensor::Test::Simple::Structures::Message;
    using RawData = Sensor::Test::Simple::Structures::RawData;

    std::string const SERVER_ADDRESS = "I like trains";

    ServerCommunicatorTest() = default;

    virtual ~ServerCommunicatorTest() = default;

    Error formatStrategyErrorWithCorrectOrigin(Error strategyIssuedError, std::string origin) const noexcept {
        Error formattedError(origin + ErrorHandling::Message::SEPARATOR + strategyIssuedError.getOrigin(),
                             strategyIssuedError.getCategory(),
                             strategyIssuedError.getSeverity(),
                             strategyIssuedError.getErrorCode(),
                             strategyIssuedError.getMessage());
        return formattedError;
    }

    ::testing::AssertionResult expectedErrorHasBeenThrown(ErrorSinkMock* sink, Error expectedError) const noexcept {

        auto producedErrors = sink->getConsumedData();
        bool expectedErrorPublished = true;
        auto lastParsedError = ErrorHandling::SensorAccessLinkError::returnDefaultData();
        for (auto t = 0; t < producedErrors.size() && expectedErrorPublished; ++t) {
            lastParsedError = producedErrors.front();
            expectedErrorPublished = lastParsedError == expectedError;
            producedErrors.pop_front();
        }
        if (expectedErrorPublished) {
            return ::testing::AssertionSuccess();
        } else {
            return ::testing::AssertionFailure()
                    << "\nExpected : \n"
                    << expectedError.fetchDetailedMessage()
                    << "\nGot : \n"
                    << lastParsedError.fetchDetailedMessage()
                    << "\n";
        }
    }
};

TEST_F(ServerCommunicatorTest, given__when_connect_then_callsOpenConnectionInStrategy) {
    ServerCommunicatorTestMock::MockServerCommunicatorStrategy mockStrategy;
    ServerCommunicator serverCommunicator(&mockStrategy);

    serverCommunicator.openConnection(SERVER_ADDRESS);

    auto strategyHasBeenCalled = mockStrategy.hasOpenConnectionBeenCalled();
    ASSERT_TRUE(strategyHasBeenCalled);
}

TEST_F(ServerCommunicatorTest, given_aMessageToSend_when_consume_then_callsSendMessageInStrategy) {
    ServerCommunicatorTestMock::MockServerCommunicatorStrategy mockStrategy;
    ServerCommunicator serverCommunicator(&mockStrategy);
    auto message = DataTestUtil::createRandomSimpleMessageWithEmptyTimestamps();

    serverCommunicator.consume(std::move(message));

    auto strategyHasBeenCalled = mockStrategy.hasSendMessageBeenCalled();
    ASSERT_TRUE(strategyHasBeenCalled);
}

TEST_F(ServerCommunicatorTest, given_aMessageToSend_when_consume_then_callsSendMessageInStrategyWithTheMessage) {
    ServerCommunicatorTestMock::MockServerCommunicatorStrategy mockStrategy;
    ServerCommunicator serverCommunicator(&mockStrategy);
    auto message = DataTestUtil::createRandomSimpleMessageWithEmptyTimestamps();
    auto copy = Message(message);

    serverCommunicator.consume(std::move(message));

    auto receivedMessage = mockStrategy.getSentMessage();
    ASSERT_EQ(copy, receivedMessage);
}

/**
 * @metric-test : timestamps
 */
TEST_F(ServerCommunicatorTest, given_aMessageToSend_when_consume_then_sendsItAfterHavingTimestampedIt) {
    ServerCommunicatorTestMock::MockServerCommunicatorStrategy mockStrategy;
    ServerCommunicator serverCommunicator(&mockStrategy);
    auto message = DataTestUtil::createRandomSimpleMessageWithEmptyTimestamps();

    auto now = HighResolutionClock::now();
    serverCommunicator.consume(std::move(message));

    auto receivedMessage = mockStrategy.getSentMessage();

    auto timePoint = receivedMessage.getGatewayTimestamps().getTimePoints()[0];
    auto timePointLocation = timePoint.location;
    auto timePointTimestamp = timePoint.timestamp;

    auto sameLocation = timePointLocation == Metrics::LocationNames::SERVER_COMMUNICATOR_SENDING;
    ASSERT_TRUE(sameLocation);
    ASSERT_TRUE(Assert::timeWithinMicrosecondDelta(timePointTimestamp, now, FIVE_HUNDRED_NANO_SECONDS));
}

TEST_F(ServerCommunicatorTest, given_aRawDataCycleToSend_when_consume_then_callsSendRawDataInStrategy) {
    ServerCommunicatorTestMock::MockServerCommunicatorStrategy mockStrategy;
    ServerCommunicator serverCommunicator(&mockStrategy);
    auto rawData = DataTestUtil::createRandomSimpleRawData();

    serverCommunicator.consume(std::move(rawData));

    auto strategyHasBeenCalled = mockStrategy.hasSendRawDataBeenCalled();
    ASSERT_TRUE(strategyHasBeenCalled);
}

TEST_F(ServerCommunicatorTest, given_aRawDataCycleToSend_when_consume_then_callsSendRawDataInStrategyWithTheRawData) {
    ServerCommunicatorTestMock::MockServerCommunicatorStrategy mockStrategy;
    ServerCommunicator serverCommunicator(&mockStrategy);
    auto rawData = DataTestUtil::createRandomSimpleRawData();
    auto copy = RawData(rawData);

    serverCommunicator.consume(std::move(rawData));

    auto receivedRawData = mockStrategy.getSentRawData();
    ASSERT_EQ(copy, receivedRawData);
}

TEST_F(ServerCommunicatorTest, given__when_disconnect_then_callsCloseConnectionInStrategy) {
    ServerCommunicatorTestMock::MockServerCommunicatorStrategy mockStrategy;
    ServerCommunicator serverCommunicator(&mockStrategy);

    serverCommunicator.closeConnection();

    auto strategyHasBeenCalled = mockStrategy.hasCloseConnectionBeenCalled();
    ASSERT_TRUE(strategyHasBeenCalled);
}

TEST_F(ServerCommunicatorTest,
       given_anErrorThatRequiresToCloseThrownOnOpenConnectionCall_when_isCaught_then_callsCloseConnectionInStrategy) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwCloseConnectionRequiredErrorWhenOpenConnectionIsCalled();

    ServerCommunicator serverCommunicator(&throwingMockStrategy);
    serverCommunicator.openConnection(SERVER_ADDRESS);
    throwingMockStrategy.waitUntilCloseConnectionCallIsMadeAfterErrorIsThrown();

    auto closeConnectionCalled = throwingMockStrategy.hasCloseConnectionBeenCalledAfterThrowingFunction();

    ASSERT_TRUE(closeConnectionCalled);
}

TEST_F(ServerCommunicatorTest,
       given_anErrorThatRequiresToOpenThrownOnOpenConnectionCall_when_isCaught_then_callsOpenConnectionInStrategy) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwOpenConnectionRequiredErrorWhenOpenConnectionIsCalled();

    ServerCommunicator serverCommunicator(&throwingMockStrategy);
    serverCommunicator.openConnection(SERVER_ADDRESS);
    throwingMockStrategy.waitUntilOpenConnectionCallIsMadeAfterErrorIsThrown();

    auto openConnectionCalled = throwingMockStrategy.hasOpenConnectionBeenCalledAfterThrowingFunction();

    ASSERT_TRUE(openConnectionCalled);
}

TEST_F(ServerCommunicatorTest,
       given_anErrorThatRequiresToCloseThrownOnCloseConnectionCall_when_isCaught_then_callsCloseConnectionInStrategy) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwCloseConnectionRequiredErrorWhenCloseConnectionIsCalled();

    ServerCommunicator serverCommunicator(&throwingMockStrategy);
    serverCommunicator.closeConnection();
    throwingMockStrategy.waitUntilCloseConnectionCallIsMadeAfterErrorIsThrown();

    auto closeConnectionCalled = throwingMockStrategy.hasCloseConnectionBeenCalledAfterThrowingFunction();

    ASSERT_TRUE(closeConnectionCalled);
}

TEST_F(ServerCommunicatorTest,
       given_anErrorThatRequiresToOpenThrownOnCloseConnectionCall_when_isCaught_then_callsOpenConnectionInStrategy) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwOpenConnectionRequiredErrorWhenCloseConnectionIsCalled();

    ServerCommunicator serverCommunicator(&throwingMockStrategy);
    serverCommunicator.closeConnection();
    throwingMockStrategy.waitUntilOpenConnectionCallIsMadeAfterErrorIsThrown();

    auto openConnectionCalled = throwingMockStrategy.hasOpenConnectionBeenCalledAfterThrowingFunction();

    ASSERT_TRUE(openConnectionCalled);
}

TEST_F(ServerCommunicatorTest,
       given_anErrorThatRequiresToCloseThrownOnSendMessageCall_when_isCaught_then_callsCloseConnectionInStrategy) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwCloseConnectionRequiredErrorWhenSendMessageIsCalled();

    ServerCommunicator serverCommunicator(&throwingMockStrategy);
    auto message = DataTestUtil::createRandomSimpleMessageWithEmptyTimestamps();
    serverCommunicator.consume(std::move(message));
    throwingMockStrategy.waitUntilCloseConnectionCallIsMadeAfterErrorIsThrown();

    auto closeConnectionCalled = throwingMockStrategy.hasCloseConnectionBeenCalledAfterThrowingFunction();

    ASSERT_TRUE(closeConnectionCalled);
}

TEST_F(ServerCommunicatorTest,
       given_anErrorThatRequiresToOpenThrownOnSendMessageCall_when_isCaught_then_callsOpenConnectionInStrategy) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwOpenConnectionRequiredErrorWhenSendMessageIsCalled();

    ServerCommunicator serverCommunicator(&throwingMockStrategy);
    auto message = DataTestUtil::createRandomSimpleMessageWithEmptyTimestamps();
    serverCommunicator.consume(std::move(message));
    throwingMockStrategy.waitUntilOpenConnectionCallIsMadeAfterErrorIsThrown();

    auto openConnectionCalled = throwingMockStrategy.hasOpenConnectionBeenCalledAfterThrowingFunction();

    ASSERT_TRUE(openConnectionCalled);
}

TEST_F(ServerCommunicatorTest,
       given_anErrorThatRequiresToCloseThrownOnSendRawDataCall_when_isCaught_then_callsCloseConnectionInStrategy) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwCloseConnectionRequiredErrorWhenSendRawDataIsCalled();

    ServerCommunicator serverCommunicator(&throwingMockStrategy);
    auto rawData = DataTestUtil::createRandomSimpleRawData();
    serverCommunicator.consume(std::move(rawData));
    throwingMockStrategy.waitUntilCloseConnectionCallIsMadeAfterErrorIsThrown();

    auto closeConnectionCalled = throwingMockStrategy.hasCloseConnectionBeenCalledAfterThrowingFunction();

    ASSERT_TRUE(closeConnectionCalled);
}

TEST_F(ServerCommunicatorTest,
       given_anErrorThatRequiresToOpenThrownOnSendRawDataCall_when_isCaught_then_callsOpenConnectionInStrategy) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwOpenConnectionRequiredErrorWhenSendRawDataIsCalled();

    ServerCommunicator serverCommunicator(&throwingMockStrategy);
    auto rawData = DataTestUtil::createRandomSimpleRawData();
    serverCommunicator.consume(std::move(rawData));
    throwingMockStrategy.waitUntilOpenConnectionCallIsMadeAfterErrorIsThrown();

    auto openConnectionCalled = throwingMockStrategy.hasOpenConnectionBeenCalledAfterThrowingFunction();

    ASSERT_TRUE(openConnectionCalled);
}

TEST_F(ServerCommunicatorTest,
       given_aStrategyThrowingErrorOnOpenConnectionCall_when_errorIsCaught_then_producesTheErrorWithCorrectOrigin) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwErrorWhenOpenConnectionIsCalled();
    Error expectedError = throwingMockStrategy.expectedErrorWhenOpenConnectionIsCalled();
    expectedError = formatStrategyErrorWithCorrectOrigin(expectedError,
                                                         ErrorHandling::Origin::SERVER_COMMUNICATOR_OPEN_CONNECTION);

    auto numberOfErrorToReceive = 1;
    ErrorSinkMock sink(numberOfErrorToReceive);
    ErrorProcessingScheduler scheduler(&sink);
    ServerCommunicator serverCommunicator(&throwingMockStrategy);

    serverCommunicator.linkConsumer(&scheduler);

    serverCommunicator.openConnection(SERVER_ADDRESS);
    sink.waitConsumptionToBeReached();

    scheduler.terminateAndJoin();

    ASSERT_TRUE(expectedErrorHasBeenThrown(&sink, expectedError));
}

TEST_F(ServerCommunicatorTest,
       given_aStrategyThrowingErrorOnCloseConnectionCall_when_errorIsCaught_then_producesTheErrorWithCorrectOrigin) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwErrorWhenCloseConnectionIsCalled();
    Error expectedError = throwingMockStrategy.expectedErrorWhenCloseConnectionIsCalled();
    expectedError = formatStrategyErrorWithCorrectOrigin(expectedError,
                                                         ErrorHandling::Origin::SERVER_COMMUNICATOR_CLOSE_CONNECTION);
    auto numberOfErrorToReceive = 1;
    ErrorSinkMock sink(numberOfErrorToReceive);
    ErrorProcessingScheduler scheduler(&sink);
    ServerCommunicator serverCommunicator(&throwingMockStrategy);

    serverCommunicator.linkConsumer(&scheduler);

    serverCommunicator.closeConnection();
    sink.waitConsumptionToBeReached();

    scheduler.terminateAndJoin();

    ASSERT_TRUE(expectedErrorHasBeenThrown(&sink, expectedError));
}

TEST_F(ServerCommunicatorTest,
       given_aStrategyThrowingErrorOnFetchMessagesCall_when_errorIsCaught_then_producesTheErrorWithCorrectOrigin) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwErrorWhenSendMessageIsCalled();
    Error expectedError = throwingMockStrategy.expectedErrorWhenSendMessageIsCalled();
    expectedError = formatStrategyErrorWithCorrectOrigin(expectedError,
                                                         ErrorHandling::Origin::SERVER_COMMUNICATOR_SEND_MESSAGE);
    auto numberOfErrorToReceive = 1;
    ErrorSinkMock sink(numberOfErrorToReceive);
    ErrorProcessingScheduler scheduler(&sink);
    ServerCommunicator serverCommunicator(&throwingMockStrategy);

    serverCommunicator.linkConsumer(&scheduler);

    auto message = DataTestUtil::createRandomSimpleMessageWithEmptyTimestamps();
    serverCommunicator.consume(std::move(message));
    sink.waitConsumptionToBeReached();

    scheduler.terminateAndJoin();

    ASSERT_TRUE(expectedErrorHasBeenThrown(&sink, expectedError));
}

TEST_F(ServerCommunicatorTest,
       given_aStrategyThrowingErrorOnFetchRawDataCyclesCall_when_errorIsCaught_then_producesTheErrorWithCorrectOrigin) {
    ThrowingServerCommunicationStrategy throwingMockStrategy;
    throwingMockStrategy.throwErrorWhenSendRawDataIsCalled();
    Error expectedError = throwingMockStrategy.expectedErrorWhenSendRawDataIsCalled();
    expectedError = formatStrategyErrorWithCorrectOrigin(expectedError,
                                                         ErrorHandling::Origin::SERVER_COMMUNICATOR_SEND_RAWDATA);
    auto numberOfErrorToReceive = 1;
    ErrorSinkMock sink(numberOfErrorToReceive);
    ErrorProcessingScheduler scheduler(&sink);
    ServerCommunicator serverCommunicator(&throwingMockStrategy);

    serverCommunicator.linkConsumer(&scheduler);

    auto rawData = DataTestUtil::createRandomSimpleRawData();
    serverCommunicator.consume(std::move(rawData));
    sink.waitConsumptionToBeReached();

    scheduler.terminateAndJoin();

    ASSERT_TRUE(expectedErrorHasBeenThrown(&sink, expectedError));
}

#endif //SENSORGATEWAY_SERVERCOMMUNICATORTEST_CPP

