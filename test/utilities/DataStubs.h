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


#ifndef SPIRITSENSORGATEWAY_DATASTUBS_H
#define SPIRITSENSORGATEWAY_DATASTUBS_H

#include "spirit-sensor-gateway/domain/Frame.h"
#include "spirit-sensor-gateway/domain/AWLMessage.h"
#include <vector>

using DataFlow::AWLMessage;
using DataFlow::Track;
using DataFlow::Pixel;
using DataFlow::Frame;

namespace {

    static AWLMessage createAWLMessageWithID(uint16_t id) {

        AWLMessage awlMessage = AWLMessage::returnDefaultData();
        awlMessage.id = id;
        awlMessage.data[0] = 16;
        awlMessage.data[1] = 32;
        awlMessage.data[2] = 48;
        awlMessage.data[3] = 1;
        awlMessage.data[4] = 0;
        awlMessage.data[5] = 96;
        awlMessage.data[6] = 112;
        awlMessage.data[7] = 128;
        return awlMessage;
    }

    static Track createTrack(DataFlow::TrackID trackID, DataFlow::ConfidenceLevel confidenceLevel,
                      DataFlow::Intensity intensity, DataFlow::Acceleration acceleration,
                      DataFlow::Distance distance, DataFlow::Speed speed) {
        Track track = Track(trackID, confidenceLevel, intensity);
        track.setAcceleration(acceleration);
        track.setDistance(distance);
        track.setSpeed(speed);
        return track;
    }

    static Pixel createPixel(DataFlow::PixelID pixelID, std::vector<Track> tracks) {
        Pixel pixel = Pixel(pixelID);
        for (auto track : tracks) {
            pixel.addTrack(track);
        }
        return pixel;
    }

    static Frame createFrame(DataFlow::FrameID frameID, DataFlow::SystemID systemID, std::vector<Pixel> pixels) {
        Frame frame = Frame();
        frame.setFrameID(frameID);
        frame.setSystemID(systemID);
        for (auto pixel : pixels) {
            frame.addPixel(pixel);
        }
        return frame;
    }

#endif //SPIRITSENSORGATEWAY_DATASTUBS_H

}