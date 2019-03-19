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

#ifndef SENSORGATEWAY_SERVERCOMMUNICATIONPROTOCOLSTRATEGY_HPP
#define SENSORGATEWAY_SERVERCOMMUNICATIONPROTOCOLSTRATEGY_HPP

#include "ServerRequest.hpp"
#include "ServerResponse.hpp"

namespace ServerCommunication {

    template<class T>
    class ServerCommunicationStrategy {

    public:

        using Message = typename T::Message;
        using RawData = typename T::RawData;

        using GetParameterValueContent = std::string;
        using GetParameterValueContents = std::array<GetParameterValueContent, T::MAX_NUMBER_OF_CONCURRENT_REQUEST_OF_ONE_KIND>;

        using AllParameterNamesList = std::array<std::string, T::Parameters::NUMBER_OF_AVAILABLE_PARAMETERS>;

        using UnsignedIntegerParameterResponse = ResponseType::UnsignedIntegerParameterResponse;
        using SignedIntegerParameterResponse = ResponseType::SignedIntegerParameterResponse;
        using RealNumberParameterResponse = ResponseType::RealNumberParameterResponse;
        using BooleanParameterResponse = ResponseType::BooleanParameterResponse;
        using ParameterErrorResponse = ResponseType::ParameterErrorResponse;
        using ErrorMessageResponse = ResponseType::ErrorMessageResponse;

        using SetParameterValueBooleanContent = typename std::pair<GetParameterValueContent, bool>;

        ServerCommunicationStrategy() noexcept :
                receivedGetAllParameterNamesRequest(false),
                receivedCalibrationRequest(false),
                receivedClearCalibrationRequest(false) {
        }

        virtual ~ServerCommunicationStrategy() noexcept = default;

        virtual void openConnection(std::string const& serverAddress) = 0;

        bool hasReceivedGetAllParameterNamesRequest() noexcept {
            return checkIfRequestHasBeenReceived(&receivedGetAllParameterNamesRequest);
        }

        virtual GetParameterValueContents fetchGetParameterValueContents() = 0;

        bool hasReceivedCalibrationRequest() noexcept {
            return checkIfRequestHasBeenReceived(&receivedCalibrationRequest);
        }

        bool hasReceivedClearCalibrationRequest() noexcept {
            return checkIfRequestHasBeenReceived(&receivedClearCalibrationRequest);
        }

        virtual void sendMessage(Message&& message) = 0;

        virtual void sendRawData(RawData&& rawData) = 0;

        virtual void sendResponse(UnsignedIntegerParameterResponse&& unsignedIntegerParameterResponse) = 0;

        virtual void sendResponse(SignedIntegerParameterResponse&& signedIntegerParameterResponse) = 0;

        virtual void sendResponse(RealNumberParameterResponse&& realNumberParameterResponse) = 0;

        virtual void sendResponse(BooleanParameterResponse&& booleanParameterResponse) = 0;

        virtual void sendResponse(ParameterErrorResponse&& parameterErrorResponse) = 0;

        virtual void sendResponse(ErrorMessageResponse&& errorMessageResponse) = 0;

        virtual void closeConnection() = 0;

    protected:

        bool checkIfRequestHasBeenReceived(AtomicFlag* requestReceivedFlag) const {
            bool valueToReturn = requestReceivedFlag->load();
            requestReceivedFlag->store(false);
            return valueToReturn;
        }

        void receiveGetAllParameterNamesRequest() {
            receivedGetAllParameterNamesRequest.store(true);
        }

        void receiveCalibrationRequest() {
            receivedCalibrationRequest.store(true);
        }

        void receiveClearCalibrationRequest() {
            receivedClearCalibrationRequest.store(true);
        }

        AtomicFlag receivedGetAllParameterNamesRequest;
        AtomicFlag receivedCalibrationRequest;
        AtomicFlag receivedClearCalibrationRequest;

    };
}

#endif //SENSORGATEWAY_SERVERCOMMUNICATIONPROTOCOLSTRATEGY_HPP
