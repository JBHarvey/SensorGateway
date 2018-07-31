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

#include "SimpleData.h"

using DataModel::SimpleData;
using DataModel::SimpleDataContent;
using Defaults::DEFAULT_SIMPLE_DATA_CONTENT;

SimpleData::SimpleData(SimpleDataContent simpleDataContent) :
        content(std::move(simpleDataContent)) {
}

SimpleData::SimpleData(SimpleData&& other) noexcept:
        content(std::move(other.content)) {
    other.content = DEFAULT_SIMPLE_DATA_CONTENT;
}

SimpleData& SimpleData::operator=(SimpleData const& other)& {
    SimpleData temporary(other);
    swap(*this, temporary);
    return *this;
}

SimpleData& SimpleData::operator=(SimpleData&& other)& noexcept {
    swap(*this, other);
    return *this;
}

void SimpleData::swap(SimpleData& current, SimpleData& other) noexcept {
    std::swap(current.content, other.content);
}

bool SimpleData::operator==(SimpleData const& other) const {
    bool sameContent = true;
    for (unsigned long contentIndex = 0; contentIndex < content.size() && sameContent; ++contentIndex) {
        sameContent = (content.at(contentIndex) == other.content.at(contentIndex));
    }
    return sameContent;
}

bool SimpleData::operator!=(SimpleData const& other) const {
    return !operator==(other);
}

std::string SimpleData::toString() const noexcept {
    std::string stringifiedContent;
    for(unsigned long i = 0; i < content.size(); ++i) {
        stringifiedContent += " " + content.at(i);
    }
    return stringifiedContent;
}

const SimpleData SimpleData::returnDefaultData() noexcept {
    return Defaults::DEFAULT_SIMPLE_DATA;
}

