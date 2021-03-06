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

#ifndef SENSORGATEWAY_DATAMODELFIXTURE_H
#define SENSORGATEWAY_DATAMODELFIXTURE_H

#include "TestDataStructures.h"

namespace TestFunctions {

    class DataTestUtil {

    public:

        static const std::string createRandomStringOfLength(size_t const& stringLength) noexcept {
            auto const lengthOfDataToCreate = 42;
            typedef std::array<char, 62> CharArray;
            auto charSet = CharArray({'0', '1', '2', '3', '4',
                                      '5', '6', '7', '8', '9',
                                      'A', 'B', 'C', 'D', 'E', 'F',
                                      'G', 'H', 'I', 'J', 'K',
                                      'L', 'M', 'N', 'O', 'P',
                                      'Q', 'R', 'S', 'T', 'U',
                                      'V', 'W', 'X', 'Y', 'Z',
                                      'a', 'b', 'c', 'd', 'e', 'f',
                                      'g', 'h', 'i', 'j', 'k',
                                      'l', 'm', 'n', 'o', 'p',
                                      'q', 'r', 's', 't', 'u',
                                      'v', 'w', 'x', 'y', 'z'});

            std::default_random_engine randomEngine(std::random_device{}());
            std::uniform_int_distribution<> distribution(0, charSet.size() - 1);
            auto randomCharFunction = [charSet, &distribution, &randomEngine]() {
                return charSet[distribution(randomEngine)];
            };

            std::string randomString = drawRandomString(lengthOfDataToCreate, randomCharFunction);
            return randomString;
        }

        static const DataModel::SimpleMessage createRandomSimpleMessageWithEmptyTimestamps() {
            auto const lengthOfDataToCreate = 42;

            DataModel::SimpleMessage::Content content;
            auto numberOfStringToCreate = content.size();
            for (unsigned long i = 0; i < numberOfStringToCreate; ++i) {
                content.at(i) = createRandomStringOfLength(lengthOfDataToCreate);
            }
            DataModel::SimpleMessage randomSimpleMessage(content, DataModel::Defaults::DEFAULT_SERVICE_TIMESTAMPS);

            return randomSimpleMessage;
        }

        static const DataModel::SimpleRawData createRandomSimpleRawData() {
            auto const maximalValue = 9001;

            std::default_random_engine randomEngine(std::random_device{}());
            std::uniform_int_distribution<unsigned int> distribution(0, maximalValue);

            Sensor::Test::Simple::Structures::RawData::RawDataDefinition::Data content;
            auto const numberOfDataToCreate = content.size();
            for (auto i = 0u; i < numberOfDataToCreate; ++i) {
                content.at(i) = distribution(randomEngine);
            }
            DataModel::SimpleRawData randomSimpleRawData(content);

            return randomSimpleRawData;
        }

    private:

        static std::string drawRandomString(size_t length, std::function<char(void)> const& pickRandomChar) {
            std::string randomString(length, 0);
            std::generate_n(randomString.begin(), length, pickRandomChar);
            return randomString;
        }

    };

}

#endif //SENSORGATEWAY_DATAMODELFIXTURE_H
