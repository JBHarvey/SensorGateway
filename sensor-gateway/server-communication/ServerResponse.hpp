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

#ifndef SERVERGATEWAY_SERVERRESPONSE_HPP
#define SERVERGATEWAY_SERVERRESPONSE_HPP

#include "ServerRequestAssembler.hpp"

namespace ServerCommunication {

    template<typename ResponsePayload, typename Request>
    class ServerResponse {

    public:

        explicit ServerResponse(ResponsePayload const& payload, Request&& request) noexcept :
                payload(payload), request(std::forward<Request>(request)) {}

        explicit ServerResponse() noexcept :
                ServerResponse(ServerResponse::returnDefaultData()) {}

        ~ServerResponse() noexcept = default;

        ServerResponse(ServerResponse const& other) :
                ServerResponse(other.payload, other.request) {}

        ServerResponse(ServerResponse&& other) noexcept :
                ServerResponse(std::move(other.payload), std::move(other.request)) {}

        ServerResponse& operator=(ServerResponse const& other)& {
            ServerResponse temporary(other);
            swap(*this, temporary);
            return *this;
        }

        ServerResponse& operator=(ServerResponse&& other)& noexcept {
            swap(*this, other);
            return *this;
        }

        void swap(ServerResponse& current, ServerResponse& other) noexcept {
            std::swap(current.payload, other.payload);
            std::swap(current.request, other.request);
        }

        bool operator==(ServerResponse const& other) const {
            auto samePayload = (payload == other.payload);
            auto sameRequest = (request == other.request);
            bool requestsAreEqual = (samePayload &&
                                     sameRequest);
            return requestsAreEqual;
        }

        bool operator!=(ServerResponse const& other) const {
            return !operator==(other);
        }

        static ServerResponse const& returnDefaultData() noexcept;

    private:

        ResponsePayload payload;
        Request request;

    };

    namespace Defaults {
        using ServerCommunication::ServerResponse;

        template<typename ResultPayload>
        ResultPayload const DEFAULT_SERVER_RESULT_PAYLOAD{};

        template<typename Request>
        Request const DEFAULT_SERVER_RESULT_REQUEST{};

        template<typename ResultPayload, typename Request>
        ServerResponse<ResultPayload, Request> const DEFAULT_SERVER_RESULT = ServerResponse<ResultPayload, Request>(
                DEFAULT_SERVER_RESULT_PAYLOAD<ResultPayload>,
                DEFAULT_SERVER_RESULT_REQUEST<Request>);
    }

    template<typename ResultPayload, typename Request>
    ServerResponse<ResultPayload, Request> const& ServerResponse<ResultPayload, Request>::returnDefaultData() noexcept {
        return Defaults::DEFAULT_SERVER_RESULT<ResultPayload, Request>;
    }

}


#endif //SERVERGATEWAY_SERVERRESPONSE_HPP
