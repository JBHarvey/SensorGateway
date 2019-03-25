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

#ifndef SERVERGATEWAY_SERVERREQUESTASSEMBLER_HPP
#define SERVERGATEWAY_SERVERREQUESTASSEMBLER_HPP

#include "ServerRequest.hpp"
#include "RequestHandlingErrorFactory.h"

namespace Assemble {

    /**
     * @note This is not tested by purpose, it is essentially a Factory.
     */
    class ServerRequestAssembler {

    protected:

        using MessagePayload = ServerCommunication::PayloadTypes::MessagePayload;
        using GetParameterValueRequest = ServerCommunication::RequestTypes::GetParameterValue;

        using SetUnsignedIntegerParameterValueRequest = ServerCommunication::RequestTypes::SetUnsignedIntegerParameterValue;
        using UnsignedIntegerRequestContent = typename ServerCommunication::ParameterPayloadContent<SetUnsignedIntegerParameterValueRequest>;

        using SetSignedIntegerParameterValueRequest = ServerCommunication::RequestTypes::SetSignedIntegerParameterValue;
        using SignedIntegerRequestContent = typename ServerCommunication::ParameterPayloadContent<SetSignedIntegerParameterValueRequest>;

        using SetRealNumberParameterValueRequest = ServerCommunication::RequestTypes::SetRealNumberParameterValue;
        using RealNumberRequestContent = typename ServerCommunication::ParameterPayloadContent<SetRealNumberParameterValueRequest>;

        using SetBooleanParameterValueRequest = ServerCommunication::RequestTypes::SetBooleanParameterValue;
        using BooleanRequestContent = typename ServerCommunication::ParameterPayloadContent<SetBooleanParameterValueRequest>;

        template<typename RequestedParameterContent>
        static constexpr auto getParameterName(RequestedParameterContent requestedParameterContent) noexcept ->
        std::tuple_element_t<0, RequestedParameterContent> {
            return std::get<0>(requestedParameterContent);
        }

        template<typename RequestedParameterContent>
        static constexpr auto getParameterValue(RequestedParameterContent requestedParameterContent) noexcept ->
        std::tuple_element_t<1, RequestedParameterContent> {
            return std::get<1>(requestedParameterContent);
        }

        using IrrelevantForRequest = StringLiteral<decltype(""_ToString)>;

    public:

        explicit ServerRequestAssembler() = delete;

        ~ServerRequestAssembler() = delete;

        static auto getParameterValueRequest(std::string const& payloadContent) -> GetParameterValueRequest {
            MessagePayload const payload{payloadContent};
            GetParameterValueRequest request(payload);
            return request;
        }

        template<typename RequestedParameterContent>
        static auto setUnsignedIntegerParameterValueRequest(RequestedParameterContent const& requestedParameterContent)
        -> SetUnsignedIntegerParameterValueRequest {
            using Payload = typename ServerCommunication::ParameterPayload<SetUnsignedIntegerParameterValueRequest>;
            using PayloadContent = typename ServerCommunication::ParameterPayloadContent<SetUnsignedIntegerParameterValueRequest>;
            using PayloadContentUnitType = typename ServerCommunication::SetParameterUnit<SetUnsignedIntegerParameterValueRequest>;
            auto parameterName = getParameterName(requestedParameterContent);
            auto parameterValue = static_cast<PayloadContentUnitType>(getParameterValue(requestedParameterContent));
            PayloadContent setParameterRequestContent = std::make_tuple(parameterName,
                                                                        parameterValue,
                                                                        IrrelevantForRequest::toString()
            );

            Payload payload{setParameterRequestContent};

            SetUnsignedIntegerParameterValueRequest request(payload);
            return request;
        }

        template<typename RequestedParameterContent>
        static auto setSignedIntegerParameterValueRequest(RequestedParameterContent const& requestedParameterContent)
        -> SetSignedIntegerParameterValueRequest {
            using Payload = typename ServerCommunication::ParameterPayload<SetSignedIntegerParameterValueRequest>;
            using PayloadContent = typename ServerCommunication::ParameterPayloadContent<SetSignedIntegerParameterValueRequest>;
            using PayloadContentUnitType = typename ServerCommunication::SetParameterUnit<SetSignedIntegerParameterValueRequest>;
            auto parameterName = getParameterName(requestedParameterContent);
            auto parameterValue = static_cast<PayloadContentUnitType>(getParameterValue(requestedParameterContent));
            PayloadContent setParameterRequestContent = std::make_tuple(parameterName,
                                                                        parameterValue,
                                                                        IrrelevantForRequest::toString()
            );

            Payload payload{setParameterRequestContent};

            SetSignedIntegerParameterValueRequest request(payload);
            return request;
        }

        template<typename RequestedParameterContent>
        static auto setRealNumberParameterValueRequest(RequestedParameterContent const& requestedParameterContent)
        -> SetRealNumberParameterValueRequest {
            using Payload = typename ServerCommunication::ParameterPayload<SetRealNumberParameterValueRequest>;
            using PayloadContent = typename ServerCommunication::ParameterPayloadContent<SetRealNumberParameterValueRequest>;
            using PayloadContentUnitType = typename ServerCommunication::SetParameterUnit<SetRealNumberParameterValueRequest>;
            auto parameterName = getParameterName(requestedParameterContent);
            auto parameterValue = static_cast<PayloadContentUnitType>(getParameterValue(requestedParameterContent));
            PayloadContent setParameterRequestContent = std::make_tuple(parameterName,
                                                                        parameterValue,
                                                                        IrrelevantForRequest::toString()
            );

            Payload payload{setParameterRequestContent};

            SetRealNumberParameterValueRequest request(payload);
            return request;
        }

        template<typename RequestedParameterContent>
        static auto setBooleanParameterValueRequest(RequestedParameterContent const& requestedParameterContent)
        -> SetBooleanParameterValueRequest {
            using Payload = typename ServerCommunication::ParameterPayload<SetBooleanParameterValueRequest>;
            using PayloadContent = typename ServerCommunication::ParameterPayloadContent<SetBooleanParameterValueRequest>;
            using PayloadContentUnitType = typename ServerCommunication::SetParameterUnit<SetBooleanParameterValueRequest>;
            auto parameterName = getParameterName(requestedParameterContent);
            auto parameterValue = static_cast<PayloadContentUnitType>(getParameterValue(requestedParameterContent));
            PayloadContent setParameterRequestContent = std::make_tuple(parameterName,
                                                                        parameterValue,
                                                                        IrrelevantForRequest::toString()
            );

            Payload payload{setParameterRequestContent};

            SetBooleanParameterValueRequest request(payload);
            return request;
        }
    };
}


#endif //SERVERGATEWAY_SERVERREQUESTASSEMBLER_HPP
