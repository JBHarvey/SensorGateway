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


#ifndef SPIRITSENSORGATEWAY_CONSTANTSIZEDPOINTERLIST_HPP
#define SPIRITSENSORGATEWAY_CONSTANTSIZEDPOINTERLIST_HPP

#include "AbstractPointerArray.hpp"

namespace Processing {

    template<class T, std::size_t SIZE>
    class ConstantSizedPointerList final : AbstractPointerArray<T, SIZE> {

        using super = AbstractPointerArray<T, SIZE>;
        using MarkerPosition = typename super::IteratorMarker;

    public:

        using super::AbstractPointerArray;
        using super::empty;
        using super::full;

        void store(T* newPointer) override {
            LockGuard guard(contentMutex);
            super::validateNotFull();
            placeProducerMarkerOnNextUnoccupiedPosition();
            super::storePointerAtProducerMarker(newPointer);
        }

        auto consumeNext() -> T* override {
            LockGuard guard(contentMutex);
            super::validateNotEmpty();
            placeConsumerMarkerOnNextOccupiedPosition();
            T* contentToReturn = super::consumePointerAtConsumerMarker();
            return contentToReturn;
        }

        auto readNext() -> T* {
            LockGuard guard(contentMutex);
            super::validateNotEmpty();
            placeConsumerMarkerOnNextOccupiedPosition();
            T* contentToReturn = super::readPointerAtConsumerMarker();
            return contentToReturn;
        }

        bool contains(T* pointerToFind) const noexcept {
            LockGuard guard(contentMutex);
            auto position = findPositionOf(pointerToFind);
            return position != END;
        }

        void remove(T* pointerToRemove) {
            LockGuard guard(contentMutex);
            auto position = findPositionOf(pointerToRemove);
            if (position == END) {
                throwIllegalRemovalException();
            }
            *position = nullptr;
            --numberOfPointersHeld;
        }

        uint16_t getNumberOfPointerHeld() const noexcept {
            return numberOfPointersHeld.load();
        }

    private:
        void placeProducerMarkerOnNextUnoccupiedPosition() {
            while (*producerMarker != nullptr) {
                producerMarker = super::advanceMarker(producerMarker);
            }
        }

        void placeConsumerMarkerOnNextOccupiedPosition() {
            while (*consumerMarker == nullptr) {
                consumerMarker = super::advanceMarker(consumerMarker);
            }
        }

        MarkerPosition findPositionOf(T* pointerToFind) const noexcept {
            return std::find(BEGIN, END, pointerToFind);
        }

        [[noreturn]] void throwIllegalRemovalException() const {
            super::throwIllegalActionException(
                    ExceptionMessage::CONSTANT_SIZED_POINTER_LIST_ILLEGAL_REMOVAL_POINTER_NOT_CONTAINED);
        }

        using super::pointers;
        using super::contentMutex;

        using super::producerMarker;
        using super::consumerMarker;
        using super::BEGIN;
        using super::END;

        using super::numberOfPointersHeld;
    };
}


#endif //SPIRITSENSORGATEWAY_CONSTANTSIZEDPOINTERLIST_HPP

