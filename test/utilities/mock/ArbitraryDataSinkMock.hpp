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

#ifndef SENSORGATEWAY_ARBITRARYDATASINKMOCK_HPP
#define SENSORGATEWAY_ARBITRARYDATASINKMOCK_HPP

#include "test/utilities/data-model/DataModelFixture.h"
#include "sensor-gateway/common/data-flow/DataSink.hpp"

namespace Mock {

    template<class T>
    class ArbitraryDataSinkMock final : public DataFlow::DataSink<T> {

    public:

        typedef T Data;
        typedef typename std::list<T> DataList;

        explicit ArbitraryDataSinkMock(uint8_t numberOfDataToConsume) :
                actualNumberOfDataConsumed(0),
                numberOfDataToConsume(numberOfDataToConsume) {
            consumedData.clear();
        }

        ~ArbitraryDataSinkMock() noexcept = default;

        void consume(T&& data) override {
            ++actualNumberOfDataConsumed;
            if (hasBeenCalledLessOrEqualToTheExpectedNumberOfTimes()) {
                consumedData.push_back(data);
            }
            if (hasBeenCalledExpectedNumberOfTimes()) {
                consumptionReached.set_value(true);
            }
        }

        bool hasBeenCalledLessOrEqualToTheExpectedNumberOfTimes() {
            LockGuard guard(consumptionMutex);
            return actualNumberOfDataConsumed.load() <= numberOfDataToConsume.load();
        }

        bool hasBeenCalledExpectedNumberOfTimes() {
            LockGuard guard(consumptionMutex);
            return actualNumberOfDataConsumed.load() == numberOfDataToConsume.load();
        };

        int getNumberOfConsumptions() const {
            return actualNumberOfDataConsumed.load();
        }

        void waitConsumptionToBeReached() {
            LockGuard guard(goalReachedMutex);
            if (!hasBeenCalledExpectedNumberOfTimes()) {
                consumptionReached.get_future().wait();
            }
        }

        DataList getConsumedData() const noexcept {
            return consumedData;
        }

        bool hasConsumed(T const& data) const& noexcept {
            auto dataConsumed = std::any_of(consumedData.cbegin(), consumedData.cend(), SameDataInstance(data));
            return dataConsumed;
        }

    private:


        struct SameDataInstance {
            T const value;

            SameDataInstance(T const& t) : value(t) {}

            bool const operator()(T const& t) const {
                return t == value;
            }
        };

        AtomicCounter actualNumberOfDataConsumed;
        AtomicCounter numberOfDataToConsume;

        DataList consumedData;

        Mutex consumptionMutex;
        Mutex goalReachedMutex;
        mutable BooleanPromise consumptionReached;
    };
}

#endif //SENSORGATEWAY_ARBITRARYDATASINKMOCK_HPP
