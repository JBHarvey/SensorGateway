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
#ifndef SENSORGATEWAY_AWLMESSAGE_HPP
#define SENSORGATEWAY_AWLMESSAGE_HPP

#include "sensor-gateway/common/ConstantFunctionsDefinition.h"

namespace AWL {
    using MessageId = int64_t;
    using MessageTimestamp = uint64_t;
    using MessageLength = uint32_t;
    using MessageDataUnit = uint8_t;
    using MessageDataArray = std::array<MessageDataUnit, Sensor::AWL::NUMBER_OF_DATA_BYTES>;
}

namespace DataFlow {

    template<typename SensorMessageDefinition>
    class AWLMessage : public SensorMessageDefinition::TimeTracking {
    protected:

        using superTimeTracking = typename SensorMessageDefinition::TimeTracking;

    public:

        explicit AWLMessage(AWL::MessageId id,
                            AWL::MessageTimestamp timestamp,
                            AWL::MessageLength length,
                            AWL::MessageDataArray data) noexcept :
                id(id),
                timestamp(timestamp),
                length(length),
                data(data) {
        }


        explicit AWLMessage() : AWLMessage(returnDefaultData()) {}


        ~AWLMessage() noexcept = default;

        AWLMessage(AWLMessage const& other) :
                AWLMessage(other.id, other.timestamp, other.length, other.data) {
        }

        AWLMessage(AWLMessage&& other) noexcept :
                id(other.id),
                timestamp(other.timestamp),
                length(other.length),
                data(other.data) {

        }

        AWLMessage& operator=(AWLMessage const& other)& {
            AWLMessage temporary(other);
            swap(*this, temporary);
            return *this;
        }

        AWLMessage& operator=(AWLMessage&& other)& noexcept {
            swap(*this, other);
            return *this;
        }

        static void swap(AWLMessage& current, AWLMessage& other) noexcept;

        bool operator==(AWLMessage const& other) const {
            auto sameId = (id == other.id);
            auto sameTimestamp = (timestamp == other.timestamp);
            auto sameLength = (length == other.length);
            auto sameData = (data == other.data);
            auto messagesAreEqual = (sameId && sameTimestamp && sameLength && sameData);
            return messagesAreEqual;
        }

        bool operator!=(AWLMessage const& other) const {
            return !operator==(other);
        }

        static AWLMessage const& returnDefaultData() noexcept;

        using superTimeTracking::addTimePointForSensorWithLocation;
        using superTimeTracking::addTimePointForGatewayWithLocation;

        AWL::MessageId id;
        AWL::MessageTimestamp timestamp;
        AWL::MessageLength length;
        AWL::MessageDataArray data;
    };

    namespace Defaults {
        using DataFlow::AWLMessage;
        AWL::MessageId const DEFAULT_ID = 0;
        AWL::MessageTimestamp const DEFAULT_TIMESTAMP = 0;
        AWL::MessageLength const DEFAULT_LENGTH = 0;
        AWL::MessageDataArray const DEFAULT_AWL_DATA = AWL::MessageDataArray();

        template<class SensorMessageDefinition>
        AWLMessage<SensorMessageDefinition> const DEFAULT_AWL_MESSAGE = AWLMessage<SensorMessageDefinition>(
                DEFAULT_ID,
                DEFAULT_TIMESTAMP,
                DEFAULT_LENGTH,
                DEFAULT_AWL_DATA);
    }

    template<class SensorMessageDefinition>
    void AWLMessage<SensorMessageDefinition>::swap(AWLMessage& current, AWLMessage& other) noexcept {
        std::swap(current.id, other.id);
        std::swap(current.length, other.length);
        std::swap(current.timestamp, other.timestamp);
        std::swap(current.data, other.data);
    }

    template<class SensorMessageDefinition>
    AWLMessage<SensorMessageDefinition> const& AWLMessage<SensorMessageDefinition>::returnDefaultData() noexcept {
        return Defaults::DEFAULT_AWL_MESSAGE<SensorMessageDefinition>;
    }
}

#endif //SENSORGATEWAY_AWLMESSAGE_HPP
