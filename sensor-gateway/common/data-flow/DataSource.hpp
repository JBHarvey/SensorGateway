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

#ifndef SENSORGATEWAY_DATASOURCE_HPP
#define SENSORGATEWAY_DATASOURCE_HPP

#include "ThreadSafeRingBuffer.hpp"

namespace DataFlow {

    template<class T, size_t N = RING_BUFFER_DEFAULT_SIZE, size_t C = NUMBER_OF_CONSUMER_PER_BUFFER>
    class DataSource {

    public:

        virtual ~DataSource() noexcept = default;

        void linkConsumer(ConsumerLink<T, N, C>* consumer) {
            consumer->linkWith(&outputBuffer);
        }

        virtual void produce(T&& data) {
            outputBuffer.write(std::forward<T>(data));
        }

    protected:

        DataFlow::RingBuffer<T, N, C> outputBuffer;

    };
}

#endif //SENSORGATEWAY_DATASOURCE_HPP
