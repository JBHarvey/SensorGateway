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

#ifndef SENSORGATEWAY_REQUESTHANDLER_HPP
#define SENSORGATEWAY_REQUESTHANDLER_HPP

#include "ResponseWriter.hpp"

namespace SensorAccessLinkElement {

    template<class SENSOR_STRUCTURES, class SERVER_STRUCTURES>
    class RequestHandler : public DataFlow::DataSource<ErrorHandling::SensorAccessLinkError> {

    protected:

        using DataTranslator = SensorAccessLinkElement::DataTranslator<SENSOR_STRUCTURES, SERVER_STRUCTURES>;
        using ResponseWriter = SensorAccessLinkElement::ResponseWriter<SENSOR_STRUCTURES, SERVER_STRUCTURES>;

        using ControlMessage = typename SENSOR_STRUCTURES::ControlMessage;
        using AvailableParameters = typename SERVER_STRUCTURES::Parameters;  // TODO: Extract this in a `Sensor` or `SensorParameters` class

        using ErrorSource = DataFlow::DataSource<ErrorHandling::SensorAccessLinkError>;

    public:
        explicit RequestHandler(ResponseWriter* responseWriter,
                                DataTranslator* dataTranslator) :
                responseWriter(responseWriter),
                dataTranslator(dataTranslator) {}

        ~RequestHandler() noexcept {};

        RequestHandler(RequestHandler const& other) = delete;

        RequestHandler(RequestHandler&& other) noexcept = delete;

        RequestHandler& operator=(RequestHandler const& other)& = delete;

        RequestHandler& operator=(RequestHandler&& other)& noexcept = delete;

        template<class Request>
        void handleIncomingServerRequest(Request&& request) {
            /**
             *  TODO:
             *  - Give pointer to *this* to ServerCommunicator
             *  - have SensorCommunicator call this function in `handleServerRequest`
             *  - Story 3 -> Give `Request` to ResponseWriter and stop
             *  - Story 4 -> Add Request parsing and send result to `DataTranslator`
             */
        }

        using ErrorSource::linkConsumer;

    private:

        ResponseWriter* responseWriter;
        DataTranslator* dataTranslator;

        AvailableParameters parameters;
    };
}

#endif //SENSORGATEWAY_REQUESTHANDLER_HPP
