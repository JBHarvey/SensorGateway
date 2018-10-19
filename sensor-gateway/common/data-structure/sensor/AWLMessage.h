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
#ifndef SENSORGATEWAY_AWLMESSAGE_H
#define SENSORGATEWAY_AWLMESSAGE_H

#include "sensor-gateway/common/ConstantFunctionsDefinition.h"

namespace AWL {
    using MessageId = int64_t;
    using MessageTimestamp = uint64_t;
    using MessageLength = uint32_t;
    using MessageDataUnit = uint8_t;
    using MessageDataArray = std::array<MessageDataUnit, Sensor::AWL::NUMBER_OF_DATA_BYTES>;
}

namespace DataFlow {
    class AWLMessage {
    public:
        explicit AWLMessage(AWL::MessageId id,
                            AWL::MessageTimestamp timestamp,
                            AWL::MessageLength length,
                            AWL::MessageDataArray data) noexcept;

        explicit AWLMessage();

        ~AWLMessage() noexcept = default;

        AWLMessage(AWLMessage const& other);

        AWLMessage(AWLMessage&& other) noexcept;

        AWLMessage& operator=(AWLMessage const& other)&;

        AWLMessage& operator=(AWLMessage&& other)& noexcept;

        void swap(AWLMessage& current, AWLMessage& other) noexcept;

        bool operator==(AWLMessage const&) const;

        bool operator!=(AWLMessage const&) const;

        static AWLMessage const& returnDefaultData() noexcept;

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
        AWLMessage const DEFAULT_AWL_MESSAGE = AWLMessage(DEFAULT_ID,
                                                          DEFAULT_TIMESTAMP,
                                                          DEFAULT_LENGTH,
                                                          DEFAULT_AWL_DATA);
    }
}

#endif //SENSORGATEWAY_AWLMESSAGE_H
