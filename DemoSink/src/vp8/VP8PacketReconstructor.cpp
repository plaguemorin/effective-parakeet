

#include "VP8PacketReconstructor.h"


namespace {
    union PayloadDescriptor {
        struct {
            uint8_t PID:3;
            bool _Reseved2:1;
            bool Start:1;
            bool NonReferenceFrame:1;
            bool _Reseved1:1;
            bool Extended:1; // struct ExtendedControlBitsField is present
        };
        uint8_t raw_value;
    };

    union ExtendedControlBitsField {
        struct {
            bool KEYIDXPresent:1; // struct TIDKEYIDXField is present
            bool TIDPresent:1; // struct TIDKEYIDXField is present
            bool TL0PICIDXPresent:1; // struct TL0PICIDXField is present
            bool PictureIDPresent:1; // struct PictureIdField is present
            uint8_t _Reserved1:4;
        };
        uint8_t raw_value;
    };

    union PictureIdField {
        struct {
            uint8_t PictureID:7;
            bool Extension:1; // struct PictureIdExtensionField is present directly after this one
        };
        uint8_t raw_value;
    };

    union PictureIdExtensionField {
        struct {
            uint8_t ExtendedPictureId;
        };
        uint8_t raw_value;
    };

    union TL0PICIDXField {
        struct {
            uint8_t TL0PICIDX;
        };
        uint8_t raw_value;
    };

    union TIDKEYIDXField {
        struct {
            uint8_t KEYIDX:5;
            bool Y:1;
            uint8_t TID:2;
        };
        uint8_t raw_value;
    };

    union PayloadHeader {
        struct {
            bool interframe:1;
            uint8_t ver:3;
            bool for_display:1;
            uint32_t siz0:19;
        };
        uint8_t raw_value[3];
    };

    union KeyframeHeader {
        struct {
            uint32_t startCode : 24;

            uint16_t horiz : 14;
            uint8_t horizScale : 2;

            uint16_t vert : 14;
            uint8_t vertScale : 2;
        } __attribute__((packed));
        uint8_t raw_value[7];
    };
}

VP8PacketReconstructor::VP8PacketReconstructor(uint8_t pt) : rtp::packetization::StreamReconstructor(pt) {

}

VP8PacketReconstructor::~VP8PacketReconstructor() {
}

bool VP8PacketReconstructor::ProcessPacketizedData(const rtp::packetization::PayloadDescriptor &data, const std::vector<uint8_t> &payload) {
  PayloadDescriptor payloadDescriptor = {0};
  ExtendedControlBitsField extendedControlBitsField = {0};
  PictureIdField pictureIdField = {0};
  PictureIdExtensionField pictureIdExtensionField = {0};
  TL0PICIDXField tl0PICIDXField = {0};
  TIDKEYIDXField tidkeyidxField = {0};

  auto p = payload.data();

  payloadDescriptor.raw_value = *p++;

  if (payloadDescriptor.Extended) {
    extendedControlBitsField.raw_value = *p++;
    if (extendedControlBitsField.PictureIDPresent) {
      pictureIdField.raw_value = *p++;
      if (pictureIdField.Extension) {
        pictureIdExtensionField.raw_value = *p++;
      }
    }
    if (extendedControlBitsField.TL0PICIDXPresent) {
      tl0PICIDXField.raw_value = *p++;
    }
    if (extendedControlBitsField.TIDPresent || extendedControlBitsField.KEYIDXPresent) {
      tidkeyidxField.raw_value = *p++;
    }
  }

  const auto frame_start = p;

  // Check if the packet says this should be a new frame
  if (payloadDescriptor.Start) {
    // Does the RTP layer agree ?
    if (!data.newFrame) {
      // RTP and VP8 don't agree - now that's not good!
      return false;
    }

    // Payload header is only present in the first frame
    PayloadHeader payloadHeader = {0};
    payloadHeader.raw_value[0] = *p++;
    payloadHeader.raw_value[1] = *p++;
    payloadHeader.raw_value[2] = *p++;

    // Before we allocate a frame, make sure we have valid data
    if (payloadHeader.interframe && !seen_one_keyframe) {
      // We have an interframe but we didn't have any keyframes yet!
      return false;
    }

    // Allocate memory for our raw frame
    next_frame_ = std::unique_ptr<VP8EncodedImage>(new VP8EncodedImage());
    next_frame_->timestamp = data.rtpTimestamp;
    next_frame_->rtpPayloadType = PayloadType();

    payloadHeader.for_display;
    payloadHeader.ver;
    next_frame_->payload.reserve(payloadHeader.siz0);
    next_frame_->keyFrame = !payloadHeader.interframe;

    /* if it's a keyframe, then we have a keyframe header */
    if (!payloadHeader.interframe) {
      KeyframeHeader keyframeHeader = {0};
      keyframeHeader.raw_value[0] = *p++;
      keyframeHeader.raw_value[1] = *p++;
      keyframeHeader.raw_value[2] = *p++;
      keyframeHeader.raw_value[3] = *p++;
      keyframeHeader.raw_value[4] = *p++;
      keyframeHeader.raw_value[5] = *p++;
      keyframeHeader.raw_value[6] = *p++;

      next_frame_->width = (keyframeHeader.horizScale << 14) | keyframeHeader.horiz;
      next_frame_->height = (keyframeHeader.vertScale << 14) | keyframeHeader.vert;

      seen_one_keyframe = true;
    } else {
      // use the last width/height we had
      next_frame_->width = last_width;
      next_frame_->height = last_height;
    }
  }

  if (!next_frame_) {
    // We need a place to send the data to!
    return false;
  }

  // We have to copy the payload
  next_frame_->payload.insert(
      next_frame_->payload.end(),
      frame_start,
      payload.data() + data.payloadDataLength
  );

  // Marker bit marks the frame as completed
  if (data.marker) {
    QueueCompletedFrame(std::move(next_frame_));
  }

  return true;
}
