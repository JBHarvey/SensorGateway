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

#ifndef SENSORGATEWAY_CONTROLMESSAGE_HPP
#define SENSORGATEWAY_CONTROLMESSAGE_HPP

#include "sensor-gateway/common/data-structure/time-tracking/TimeTracking.hpp"

namespace Sensor {
    /**
     * @brief This CommandDefinition struct serves to declare types used by <SensorName>Command classes
     * @tparam T the type of an individual payload data
     * @tparam N the max number of payload data in a command
     */
    template<typename T, std::size_t N>
    struct ControlMessageDefinition {
        static std::size_t const SIZE = N;
        using ValueType = T;
        using Payload = std::array<ValueType, SIZE>;
    };

    namespace ControlMessagePayloadTypes {
        typedef Byte AWL;
        typedef Byte GUARDIAN;
    }

}

namespace DataFlow {


    template<typename ControlMessageDefinition>
    class ControlMessage {

    public:

        enum ControlMessageCode {
            NOOP = 0,
            GET_VALUE = 10,
            BULK_GET_VALUE = 11,
            SET_VALUE = 20,
            BULK_SET_VALUE = 21,
        };

        using Definitions = ControlMessageDefinition;
        using Payload = typename Definitions::Payload;

        explicit ControlMessage(ControlMessageCode const& commandCode, Payload payload) noexcept :
                controlMessageCode(commandCode),
                payload(payload) {}

        explicit ControlMessage() noexcept :
                ControlMessage(ControlMessage::returnDefaultData()) {}

        ~ControlMessage() noexcept = default;

        ControlMessage(ControlMessage const& other) :
                ControlMessage(other.controlMessageCode, other.payload) {}

        ControlMessage(ControlMessage&& other) noexcept :
                ControlMessage(std::move(other.controlMessageCode),
                        std::move(other.payload)) {}

        ControlMessage& operator=(ControlMessage const& other)& {
            ControlMessage temporary(other);
            swap(*this, temporary);
            return *this;
        }

        ControlMessage& operator=(ControlMessage&& other)& noexcept {
            swap(*this, other);
            return *this;
        }

        void swap(ControlMessage& current, ControlMessage& other) noexcept {
            std::swap(current.controlMessageCode, other.controlMessageCode);
            std::swap(current.payload, other.payload);
        }

        bool operator==(ControlMessage const& other) const {
            auto sameCommandCode = (controlMessageCode == other.controlMessageCode);
            auto samePayload = (payload == other.payload);
            auto messagesAreEqual = (sameCommandCode && samePayload);
            return messagesAreEqual;
        }

        bool operator!=(ControlMessage const& other) const {
            return !operator==(other);
        }

        static ControlMessage const& returnDefaultData() noexcept;

    private:
        ControlMessageCode controlMessageCode;
        Payload payload;
    };

    namespace Defaults {
        using DataFlow::ControlMessage;

        template<typename CommandDefinition>
        typename ControlMessage<CommandDefinition>::ControlMessageCode const DEFAULT_CONTROL_MESSAGE_CODE = ControlMessage<CommandDefinition>::ControlMessageCode::NOOP;

        template<typename CommandDefinition>
        typename CommandDefinition::Payload const DEFAULT_CONTROL_MESSAGE_PAYLOAD{};

        template<typename CommandDefinition>
        ControlMessage<CommandDefinition> const DEFAULT_CONTROL_MESSAGE = ControlMessage<CommandDefinition>(
                DEFAULT_CONTROL_MESSAGE_CODE<CommandDefinition>,
                DEFAULT_CONTROL_MESSAGE_PAYLOAD<CommandDefinition>);
    }

    template<typename CommandDefinition>
    ControlMessage<CommandDefinition> const& ControlMessage<CommandDefinition>::returnDefaultData() noexcept {
        return Defaults::DEFAULT_CONTROL_MESSAGE<CommandDefinition>;
    }

}

#endif //SENSORGATEWAY_CONTROLMESSAGE_HPP