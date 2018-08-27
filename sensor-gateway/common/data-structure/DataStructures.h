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

#ifndef SENSORGATEWAY_DATASTRUCTURES_H
#define SENSORGATEWAY_DATASTRUCTURES_H

#include "sensor-gateway/common/ConstantFunctionsDefinition.h"

namespace Sensor {

    /**
     * @brief This RawDataContent struct serves to declare types used by <SensorName>RawData classes
     * @tparam T the type of an individual data
     * @tparam N the number of data per raw data cycle
     */
    template<typename T, std::size_t N>
    struct RawDataContent {
        static std::size_t const SIZE = N;
        using ValueType = T;
        using Data = std::array<ValueType, SIZE>;
    };

    namespace RawDataTypes {
        typedef int16_t AWL;
        typedef int16_t GUARDIAN;
    }

    namespace Communication {
        class DataStructures {
        public:

            explicit DataStructures() = delete;

            ~DataStructures() noexcept = delete;

            DataStructures(DataStructures const& other) = delete;

            DataStructures(DataStructures&& other) noexcept = delete;

            DataStructures& operator=(DataStructures const& other)& = delete;

            DataStructures& operator=(DataStructures&& other)& noexcept = delete;

            void swap(DataStructures& current, DataStructures& other) noexcept = delete;
        };
    }
}

#endif //SENSORGATEWAY_DATASTRUCTURES_H
