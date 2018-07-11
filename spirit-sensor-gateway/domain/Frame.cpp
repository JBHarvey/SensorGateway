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

#include "Frame.h"

using DataFlow::Frame;
using DataFlow::FrameID;
using Defaults::Frame::DEFAULT_FRAME;
using Defaults::Frame::DEFAULT_FRAME_ID;
using Defaults::Frame::DEFAULT_PIXELS_ARRAY;
using Defaults::Frame::DEFAULT_SYSTEM_ID;
using DataFlow::PixelsArray;
using DataFlow::SystemID;

Frame::Frame(FrameID frameID, SystemID systemID, PixelsArray pixels): frameID(frameID), systemID(systemID),
                                                                      pixels(pixels) {

};

Frame::Frame():systemID(DEFAULT_SYSTEM_ID),
               frameID(DEFAULT_FRAME_ID),
               pixels(DEFAULT_PIXELS_ARRAY){

};

Frame::Frame(Frame const& other): Frame(other.frameID, other.systemID, other.pixels){

};

Frame::Frame(Frame&& other) noexcept: frameID(std::move(other.frameID)), systemID(std::move(systemID)),
                                      pixels(std::move(other.pixels)){

};

Frame& Frame::operator=(Frame const& other)& {
    Frame temporary(std::move(other));
    swap(*this, temporary);
    return *this;
};

Frame& Frame::operator=(Frame&& other)& noexcept {
    swap(*this, other);
    return *this;
};

void Frame::swap(Frame& current, Frame& other) noexcept {
    std::swap(current.frameID, other.frameID);
    std::swap(current.systemID, other.systemID);
    std::swap(current.pixels, other.pixels);
};

bool Frame::operator==(Frame const& other) const {
    bool sameFrameID = (frameID == other.frameID);
    bool sameSystemID = (systemID == other.systemID);
    auto samePixels = true;
    for (auto i = 0; i < NUMBER_OF_PIXELS_IN_FRAME && samePixels; ++i) {
        samePixels = (pixels[i] == other.pixels[i]);
    }
    return (sameFrameID && sameSystemID && samePixels);
}

bool Frame::operator!=(Frame const& other) const {
    return !(operator==(other));
}

void Frame::addPixel(Pixel pixel) {
    pixels[pixel.getID()] = pixel;
}

Pixel* Frame::fetchPixelByID(PixelID pixelID) {
    return &pixels[pixelID];
}

FrameID Frame::getFrameID() const {
    return frameID;
};

SystemID Frame::getSystemID() const {
    return systemID;
};

PixelsArray* Frame::getPixels() {
    return &pixels;
};

void Frame::setFrameID(FrameID const& frameID) {
    this->frameID = frameID;
}

void Frame::setSystemID(SystemID const& systemID) {
    this->systemID = systemID;
}

Frame const& Frame::returnDefaultData() noexcept {
    return DEFAULT_FRAME;
}

