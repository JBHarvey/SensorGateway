#ifndef SPIRITSENSORGATEWAY_PIXELTEST_CPP
#define SPIRITSENSORGATEWAY_PIXELTEST_CPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "spirit-sensor-gateway/domain/Pixel.h"

using namespace DataFlow;

class PixelTest : public ::testing::Test {

};

TEST_F(PixelTest,given_anExistingTrackInAPixel_when_checkingIfTheTrackExists_then_returnsTrue) {
    Track track(8,0,0);
    Pixel pixel(1);
    pixel.addTrack(track);

    ASSERT_TRUE(pixel.doesTrackExist(track.getID()));
}

TEST_F(PixelTest,given_anExistingTrackInAPixel_when_fetchingTheTracByItsID_then_returnsAPointerToThisTrack){
    Track track(8,0,0);
    Pixel pixel(1);
    pixel.addTrack(track);

    auto actualTrackPointer = pixel.fetchTrackByID(track.getID());
    Track* expectedTrackPointer = &pixel.getTracks()->at(0);

    ASSERT_EQ(expectedTrackPointer, actualTrackPointer);
}

TEST_F(PixelTest,given_anEmptyPixelAndATrack_when_addingTheTrackToThePixel_then_trackIsAddedCorrectly){
    Track track(8,0,0);
    Pixel pixel(1);
    pixel.addTrack(track);

    ASSERT_EQ(track, pixel.getTracks()->at(0));
}

TEST_F(PixelTest,given_aPixelWithAFullArrayOfTrack_when_addingOneMoreTrack_then_throwsATrackArrayIllegalStoreFullException){
    Pixel pixel(1);
    Track track(8,0,0);
    for(int i; i < NUMBER_OF_TRACKS_IN_PIXEL; ++i){
        pixel.addTrack(track);
    }

    ASSERT_THROW(pixel.addTrack(track),std::runtime_error);
}

TEST_F(PixelTest,given_twoPixelsWithTheSameAttributes_when_checkingIfTheyAreEquals_then_returnsTrue){
    Pixel firstPixel(18);
    Pixel secondPixel(18);

    auto pixelsAreEqual = (firstPixel==secondPixel);

    ASSERT_TRUE(pixelsAreEqual);
}

TEST_F(PixelTest,given_twoPixelsWithDifferentAttributes_when_checkingIfTheyAreEquals_then_returnsFalse){
    Pixel firstPixel(18);
    Pixel secondPixel(19);

    auto pixelsAreEqual = (firstPixel==secondPixel);

    ASSERT_FALSE(pixelsAreEqual);
}

#endif //SPIRITSENSORGATEWAY_PIXELTEST_CPP