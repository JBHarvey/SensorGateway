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

DataFlow::Frame SpiritFramesFileManager::readMessageFromFileBlock(std::string const& fileBlock) {
    //TODO: update method when testing SpiritFrames -> AWLMessages
    return DataFlow::Frame();
}

void SpiritFramesFileManager::writeFileBlockWithMessage(DataFlow::Frame message, std::FILE* file) {
    writeFileLineWithContentLabelAndValue(file, 0, FRAME_ID_LABEL.c_str(), message.getFrameID());
    writeFileLineWithContentLabelAndValue(file, 0, SYSTEM_ID_LABEL.c_str(), message.getSystemID());

    auto pixels = message.getPixels();
    writeFileLineWithContentLabel(file, 0, PIXELS_LABEL.c_str());
    for (auto pixel : *pixels) {
        writeFileLineWithContentLabelAndValue(file, 1, PIXEL_ID_LABEL.c_str(), pixel.getID());
        writeFileLineWithContentLabel(file, 2, TRACKS_LABEL.c_str());
        auto tracks = pixel.getTracks();
        for (auto track : *tracks) {
            writeFileLineWithContentLabelAndValue(file, 3, TRACK_ID_LABEL.c_str(), track.ID);
            writeFileLineWithContentLabelAndValue(file, 4, ACCELERATION_LABEL.c_str(), track.acceleration);
            writeFileLineWithContentLabelAndValue(file, 4, DISTANCE_LABEL.c_str(), track.distance);
            writeFileLineWithContentLabelAndValue(file, 4, INTENSITY_LABEL.c_str(), track.intensity);
            writeFileLineWithContentLabelAndValue(file, 4, CONFIDENCE_LEVEL_LABEL.c_str(), track.confidenceLevel);
            writeFileLineWithContentLabelAndValue(file, 4, SPEED_LABEL.c_str(), track.speed);
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
