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
    writeFileLineWithContentLabelAndValue(file, 0, FRAME_ID_LABEL.c_str(), message.getFrameID());
    writeFileLineWithContentLabelAndValue(file, 0, SYSTEM_ID_LABEL.c_str(), message.getSystemID());

    //TODO: remove unordered map and ordered map structures once the domain class has been updated.
    std::unordered_map<SpiritProtocol::PixelID, SpiritProtocol::Pixel> unorderedPixels = message.getPixels();
    std::map<SpiritProtocol::PixelID, SpiritProtocol::Pixel> orderedPixels(unorderedPixels.begin(), unorderedPixels.end());
    writeFileLineWithContentLabel(file, 0, PIXELS_LABEL.c_str());
    for (auto pixel : orderedPixels) {
        writeFileLineWithContentLabelAndValue(file, 1, PIXEL_ID_LABEL.c_str(), pixel.second.getID());
        std::unordered_map<SpiritProtocol::TrackID, SpiritProtocol::Track> tracks = pixel.second.getTracks();
        for (auto track : tracks) {
            writeFileLineWithContentLabel(file, 2, TRACKS_LABEL.c_str());
            writeFileLineWithContentLabelAndValue(file, 3, TRACK_ID_LABEL.c_str(), track.second.getID());
            //TODO: remove comment line once getAcceleration return the right value
//            writeFileLineWithContentLabelAndValue(file, 3, ACCELERATION_LABEL.c_str(), track.second.getAcceleration());
            //TODO: remove comment line once getDistance return the right value
//            writeFileLineWithContentLabelAndValue(file, 3, DISTANCE_LABEL.c_str(), track.second.getDistance());
            writeFileLineWithContentLabelAndValue(file, 3, INTENSITY_LABEL.c_str(), track.second.getIntensity());
            writeFileLineWithContentLabelAndValue(file, 3, CONFIDENCE_LEVEL_LABEL.c_str(), track.second.getConfidenceLevel());
            //TODO: remove comment line once getSpeed return the right value
//            writeFileLineWithContentLabelAndValue(file, 3, SPEED_LABEL.c_str(), track.second.getSpeed());
        }
    }
    std::fprintf(file, "%s\n", MESSAGES_SEPARATOR.c_str());
}

void TestUtilities::SpiritFramesFileManager::writeFileLineWithContentLabel(std::FILE* file,
                                                                           unsigned int numberOfTabulator,
                                                                           char const* contentLabel){
    for (auto tabulatorNumber = 0; tabulatorNumber < numberOfTabulator; tabulatorNumber++){
        std::fprintf(file, "%s", MESSAGE_CONTENT_TABULATOR.c_str());
    }
    std::fprintf(file, "%s%s\n", contentLabel, MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str());
}

void TestUtilities::SpiritFramesFileManager::writeFileLineWithContentLabelAndValue(std::FILE* file,
                                                                                   unsigned int numberOfTabulator,
                                                                                   char const* contentLabel,
                                                                                   unsigned int contentValue) {
    for (auto tabulatorNumber = 0; tabulatorNumber < numberOfTabulator; tabulatorNumber++){
        std::fprintf(file, "%s", MESSAGE_CONTENT_TABULATOR.c_str());
    }
    std::fprintf(file, "%s%s%d\n", contentLabel, MESSAGE_LABEL_VALUE_ASSOCIATOR.c_str(), contentValue);
}
