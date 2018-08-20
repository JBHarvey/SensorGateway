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

#ifndef SENSORGATEWAY_AWLTRANSLATIONSTRATEGY_H
#define SENSORGATEWAY_AWLTRANSLATIONSTRATEGY_H

#include "sensor-gateway/domain/Frame.h"
#include "sensor-gateway/domain/AWLMessage.h"
#include "MessageTranslationStrategy.hpp"

namespace MessageTranslation {

    using DataFlow::AWLMessage;
    using DataFlow::Frame;
    using DataFlow::PixelID;

    class AWLTranslationStrategy final : public MessageTranslationStrategy<AWLMessage, Frame> {
    protected:
        using super = MessageTranslationStrategy<AWLMessage, Frame>;
        using super::INPUT;
        using super::OUTPUT;
    public:

        AWLTranslationStrategy();

        ~ AWLTranslationStrategy() = default;

        void translateMessage(INPUT&& inputMessage) override;

    private:

        using super::currentOutputMessage;

        void addTrackInPixel(AWLMessage&& awlMessage, PixelID pixelID);

        Track* fetchTrack(DataFlow::TrackID const& trackID);

        void translateDetectionTrackMessage(AWLMessage&& awlMessage);

        void translateDetectionVelocityMessage(AWLMessage&& awlMessage);

        void translateEndOfFrameMessage(AWLMessage&& awlMessage);
    };
}
#endif //SENSORGATEWAY_AWLTRANSLATIONSTRATEGY_H