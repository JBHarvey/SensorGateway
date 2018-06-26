#include "AWLMessageTranslator.h"
#include "UnknownMessageException.h"


    AWLMessageTranslator::AWLMessageTranslator() {
        currentSpiritFrame = new SpiritFrame();
    }

    AWLMessageTranslator::~AWLMessageTranslator() {

    }

    void AWLMessageTranslator::translateBasicMessage(AWLMessage *awlMessage) {
        switch (awlMessage->id) {
            case FRAME_DONE:
                translateEndOfFrameMessage(awlMessage);
                break;
            case DETECTION_TRACK :
                translateDetectionTrackMessage(awlMessage);
                break;
            case DETECTION_VELOCITY :
                translateDetectionVelocityMessage(awlMessage);
                break;
            default:
                throw UnknownMessageException(awlMessage);
        }
    }

    std::vector<SpiritFrame> AWLMessageTranslator::getSpiritFrames() const{
        return spiritFrames;
    };

    void AWLMessageTranslator::translateEndOfFrameMessage(AWLMessage *awlMessage) {
        currentSpiritFrame->setFrameID(awlMessage->data[0]);
        currentSpiritFrame->setSystemID(awlMessage->data[2]);
        spiritFrames.push_back(*currentSpiritFrame);
        currentSpiritFrame = new SpiritFrame();
    }

    void AWLMessageTranslator::translateDetectionTrackMessage(AWLMessage *awlMessage) {
        std::cout << "Before track creation"<< std::endl;
        uint16_t spiritPixelID = convertTwoBytesToBigEndian(awlMessage->data[3],awlMessage->data[4]);
        SpiritPixel spiritPixel = SpiritPixel(spiritPixelID);
        currentSpiritFrame->addPixel(spiritPixel);
        std::cout << "ID of Pixel : "<< spiritPixelID << std::endl;
        addTrackInPixel(awlMessage, spiritPixelID);
        std::cout << "After track creation" << std::endl <<std::endl;

    }

    void AWLMessageTranslator::translateDetectionVelocityMessage(AWLMessage *awlMessage) {
        std::cout << "Before track update" << std::endl;
        SpiritTrack spiritTrack = fetchSpiritTrack(awlMessage);
        spiritTrack.setDistance(convertTwoBytesToBigEndian(awlMessage->data[2],awlMessage->data[3]));
        spiritTrack.setSpeed(convertTwoBytesToBigEndian(awlMessage->data[4],awlMessage->data[5]));
        spiritTrack.setAcceleration(convertTwoBytesToBigEndian(awlMessage->data[6],awlMessage->data[7]));
        std::cout << "After track update"<< std::endl << std::endl;
    }


    SpiritTrack AWLMessageTranslator::fetchSpiritTrack(AWLMessage *awlMessage) {
        SpiritTrack * fetchedSpiritTrack = nullptr;
        uint16_t spiritTrackID = convertTwoBytesToBigEndian(awlMessage->data[0], awlMessage->data[1]);
        for (int pixelNumber = 0; pixelNumber < currentSpiritFrame->getPixels().size(); pixelNumber++) {
            SpiritPixel spiritPixel = currentSpiritFrame->getPixels()[pixelNumber];
            std::cout << "Pixel ID :"<<spiritPixel.getId() << std::endl;
            SpiritTrack  spiritTrack = spiritPixel.getTrackById(spiritTrackID);
            if (&spiritTrack != nullptr){
                fetchedSpiritTrack =  &spiritTrack;
            }
        }
        std::cout << "Updating track: "<<fetchedSpiritTrack->getTrackID() << std::endl;
        return *fetchedSpiritTrack;
    }


    void AWLMessageTranslator::addTrackInPixel(AWLMessage *awlMessage, uint16_t spiritPixelID){
        uint16_t spiritTrackID = convertTwoBytesToBigEndian(awlMessage->data[0],awlMessage->data[1]);
        uint8_t spiritTrackConfidenceLevel = awlMessage->data[5];
        uint16_t spiritTrackIntensity = convertTwoBytesToBigEndian(awlMessage->data[6],awlMessage->data[7]);
        SpiritTrack spiritTrack = SpiritTrack(spiritTrackID, spiritTrackConfidenceLevel, spiritTrackIntensity);
        SpiritPixel * spiritPixel = currentSpiritFrame-> getPixelByID(spiritPixelID);
        std::cout << "Number of tracks in pixel "<< spiritPixelID << ": " <<spiritPixel->getTracks().size()<< std::endl;
        std::cout << "Adding track: " <<spiritTrackID<< std::endl;
        spiritPixel->addTrack(spiritTrack);
        std::cout << "Number of tracks in pixel "<< spiritPixelID << ": " <<spiritPixel->getTracks().size()<< std::endl;
    };