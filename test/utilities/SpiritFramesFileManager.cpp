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

#include <map>
#include "SpiritFramesFileManager.h"

using TestUtilities::SpiritFramesFileManager;

SpiritProtocol::Frame SpiritFramesFileManager::readMessageFromFileBlock(std::string const& fileBlock) {
    //TODO: update method when testing SpiritFrames -> AWLMessages
    return SpiritProtocol::Frame();
}

void SpiritFramesFileManager::writeFileBlockWithMessage(SpiritProtocol::Frame const& message, std::FILE* file) {
    std::fprintf(file, "%s%s%d\n", FRAME_ID_LABEL.c_str(), MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str(), message.getFrameID());
    std::fprintf(file, "%s%s%d\n", SYSTEM_ID_LABEL.c_str(), MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str(), message.getSystemID());

    //TODO: remove unordered map and ordered map structures once the domain class has been updated.
    std::unordered_map<SpiritProtocol::PixelID, SpiritProtocol::Pixel> unorderedPixels = message.getPixels();
    std::map<SpiritProtocol::PixelID, SpiritProtocol::Pixel> orderedPixels(unorderedPixels.begin(), unorderedPixels.end());
    std::fprintf(file, "%s%s\n", PIXELS_LABEL.c_str(), MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str());
    for (auto pixel : orderedPixels) {
        std::fprintf(file, "%s%s%s%d\n", MESSAGE_CONTENT_TABULATOR.c_str(), PIXEL_ID_LABEL.c_str(),
                     MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str(), pixel.second.getID());
        std::unordered_map<SpiritProtocol::TrackID, SpiritProtocol::Track> tracks = pixel.second.getTracks();
        for (auto track : tracks) {
            std::fprintf(file, "%s%s%s%s\n", MESSAGE_CONTENT_TABULATOR.c_str(), MESSAGE_CONTENT_TABULATOR.c_str(),
                         TRACKS_LABEL.c_str(), MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str());
            std::fprintf(file, "%s%s%s%s%s%d \n", MESSAGE_CONTENT_TABULATOR.c_str(), MESSAGE_CONTENT_TABULATOR.c_str(),
                         MESSAGE_CONTENT_TABULATOR.c_str(), TRACK_ID_LABEL.c_str(),
                         MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str(), track.second.getID());
            //TODO: remove comment line once getAcceleration return the right value
//            std::fprintf(file, "%s%s%s%s%s%d \n", MESSAGE_CONTENT_TABULATOR.c_str(), MESSAGE_CONTENT_TABULATOR.c_str(),
//                         MESSAGE_CONTENT_TABULATOR.c_str(), ACCELERATION_LABEL.c_str(),
//                         MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str(), track.second.getAcceleration());
            //TODO: remove comment line once getDistance return the right value
//            std::fprintf(file, "%s%s%s%s%s%d \n", MESSAGE_CONTENT_TABULATOR.c_str(), MESSAGE_CONTENT_TABULATOR.c_str(),
//                         MESSAGE_CONTENT_TABULATOR.c_str(), DISTANCE_LABEL.c_str(),
//                         MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str(), track.second.getDistance());
            std::fprintf(file, "%s%s%s%s%s%d \n", MESSAGE_CONTENT_TABULATOR.c_str(), MESSAGE_CONTENT_TABULATOR.c_str(),
                         MESSAGE_CONTENT_TABULATOR.c_str(), INTENSITY_LABEL.c_str(),
                         MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str(), track.second.getIntensity());
            std::fprintf(file, "%s%s%s%s%s%d \n", MESSAGE_CONTENT_TABULATOR.c_str(), MESSAGE_CONTENT_TABULATOR.c_str(),
                         MESSAGE_CONTENT_TABULATOR.c_str(), CONFIDENCE_LEVEL_LABEL.c_str(),
                         MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str(), track.second.getConfidenceLevel());
            //TODO: remove comment line once getSpeed return the right value
            //std::fprintf(file, "......Speed : %d \n", track.second.getSpeed() );
        }
    }
    std::fprintf(file, "%s\n", MESSAGES_SEPARATOR.c_str());
}
