// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_PROTOCOL_DRAWBOARD_H_
#define FLATBUFFERS_GENERATED_PROTOCOL_DRAWBOARD_H_

#include "flatbuffers/flatbuffers.h"

namespace Drawboard {

struct Vec2;

struct DrawBatch;

struct FullImagePayload;

struct StartDrawPayload;

struct EndDrawPayload;

struct DrawPointsPayload;

struct DeleteBatchPayload;

struct UserEnterPayload;

struct UserLeavePayload;

enum PacketType {
  PacketType_FullImage = 1,
  PacketType_StartDraw = 2,
  PacketType_EndDraw = 3,
  PacketType_DrawPoints = 4,
  PacketType_DeleteBatch = 5,
  PacketType_UserEnter = 6,
  PacketType_UserLeave = 7,
  PacketType_MIN = PacketType_FullImage,
  PacketType_MAX = PacketType_UserLeave
};

inline const PacketType (&EnumValuesPacketType())[7] {
  static const PacketType values[] = {
    PacketType_FullImage,
    PacketType_StartDraw,
    PacketType_EndDraw,
    PacketType_DrawPoints,
    PacketType_DeleteBatch,
    PacketType_UserEnter,
    PacketType_UserLeave
  };
  return values;
}

inline const char * const *EnumNamesPacketType() {
  static const char * const names[] = {
    "FullImage",
    "StartDraw",
    "EndDraw",
    "DrawPoints",
    "DeleteBatch",
    "UserEnter",
    "UserLeave",
    nullptr
  };
  return names;
}

inline const char *EnumNamePacketType(PacketType e) {
  if (e < PacketType_FullImage || e > PacketType_UserLeave) return "";
  const size_t index = static_cast<size_t>(e) - static_cast<size_t>(PacketType_FullImage);
  return EnumNamesPacketType()[index];
}

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(2) Vec2 FLATBUFFERS_FINAL_CLASS {
 private:
  int16_t x_;
  int16_t y_;

 public:
  Vec2() {
    memset(static_cast<void *>(this), 0, sizeof(Vec2));
  }
  Vec2(int16_t _x, int16_t _y)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)) {
  }
  int16_t x() const {
    return flatbuffers::EndianScalar(x_);
  }
  int16_t y() const {
    return flatbuffers::EndianScalar(y_);
  }
};
FLATBUFFERS_STRUCT_END(Vec2, 4);

struct DrawBatch FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UID = 4,
    VT_SEQUENCE_ID = 6,
    VT_COLOR = 8,
    VT_POINTS = 10
  };
  uint32_t uid() const {
    return GetField<uint32_t>(VT_UID, 0);
  }
  uint32_t sequence_id() const {
    return GetField<uint32_t>(VT_SEQUENCE_ID, 0);
  }
  uint32_t color() const {
    return GetField<uint32_t>(VT_COLOR, 0);
  }
  const flatbuffers::Vector<const Vec2 *> *points() const {
    return GetPointer<const flatbuffers::Vector<const Vec2 *> *>(VT_POINTS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_UID) &&
           VerifyField<uint32_t>(verifier, VT_SEQUENCE_ID) &&
           VerifyField<uint32_t>(verifier, VT_COLOR) &&
           VerifyOffset(verifier, VT_POINTS) &&
           verifier.VerifyVector(points()) &&
           verifier.EndTable();
  }
};

struct DrawBatchBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_uid(uint32_t uid) {
    fbb_.AddElement<uint32_t>(DrawBatch::VT_UID, uid, 0);
  }
  void add_sequence_id(uint32_t sequence_id) {
    fbb_.AddElement<uint32_t>(DrawBatch::VT_SEQUENCE_ID, sequence_id, 0);
  }
  void add_color(uint32_t color) {
    fbb_.AddElement<uint32_t>(DrawBatch::VT_COLOR, color, 0);
  }
  void add_points(flatbuffers::Offset<flatbuffers::Vector<const Vec2 *>> points) {
    fbb_.AddOffset(DrawBatch::VT_POINTS, points);
  }
  explicit DrawBatchBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  DrawBatchBuilder &operator=(const DrawBatchBuilder &);
  flatbuffers::Offset<DrawBatch> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<DrawBatch>(end);
    return o;
  }
};

inline flatbuffers::Offset<DrawBatch> CreateDrawBatch(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t uid = 0,
    uint32_t sequence_id = 0,
    uint32_t color = 0,
    flatbuffers::Offset<flatbuffers::Vector<const Vec2 *>> points = 0) {
  DrawBatchBuilder builder_(_fbb);
  builder_.add_points(points);
  builder_.add_color(color);
  builder_.add_sequence_id(sequence_id);
  builder_.add_uid(uid);
  return builder_.Finish();
}

inline flatbuffers::Offset<DrawBatch> CreateDrawBatchDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t uid = 0,
    uint32_t sequence_id = 0,
    uint32_t color = 0,
    const std::vector<Vec2> *points = nullptr) {
  auto points__ = points ? _fbb.CreateVectorOfStructs<Vec2>(*points) : 0;
  return Drawboard::CreateDrawBatch(
      _fbb,
      uid,
      sequence_id,
      color,
      points__);
}

struct FullImagePayload FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_BATCHES = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<DrawBatch>> *batches() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<DrawBatch>> *>(VT_BATCHES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_BATCHES) &&
           verifier.VerifyVector(batches()) &&
           verifier.VerifyVectorOfTables(batches()) &&
           verifier.EndTable();
  }
};

struct FullImagePayloadBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_batches(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<DrawBatch>>> batches) {
    fbb_.AddOffset(FullImagePayload::VT_BATCHES, batches);
  }
  explicit FullImagePayloadBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  FullImagePayloadBuilder &operator=(const FullImagePayloadBuilder &);
  flatbuffers::Offset<FullImagePayload> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<FullImagePayload>(end);
    return o;
  }
};

inline flatbuffers::Offset<FullImagePayload> CreateFullImagePayload(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<DrawBatch>>> batches = 0) {
  FullImagePayloadBuilder builder_(_fbb);
  builder_.add_batches(batches);
  return builder_.Finish();
}

inline flatbuffers::Offset<FullImagePayload> CreateFullImagePayloadDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<DrawBatch>> *batches = nullptr) {
  auto batches__ = batches ? _fbb.CreateVector<flatbuffers::Offset<DrawBatch>>(*batches) : 0;
  return Drawboard::CreateFullImagePayload(
      _fbb,
      batches__);
}

struct StartDrawPayload FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UID = 4,
    VT_COLOR = 6,
    VT_SEQUENCE_ID = 8
  };
  uint32_t uid() const {
    return GetField<uint32_t>(VT_UID, 0);
  }
  uint32_t color() const {
    return GetField<uint32_t>(VT_COLOR, 0);
  }
  uint32_t sequence_id() const {
    return GetField<uint32_t>(VT_SEQUENCE_ID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_UID) &&
           VerifyField<uint32_t>(verifier, VT_COLOR) &&
           VerifyField<uint32_t>(verifier, VT_SEQUENCE_ID) &&
           verifier.EndTable();
  }
};

struct StartDrawPayloadBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_uid(uint32_t uid) {
    fbb_.AddElement<uint32_t>(StartDrawPayload::VT_UID, uid, 0);
  }
  void add_color(uint32_t color) {
    fbb_.AddElement<uint32_t>(StartDrawPayload::VT_COLOR, color, 0);
  }
  void add_sequence_id(uint32_t sequence_id) {
    fbb_.AddElement<uint32_t>(StartDrawPayload::VT_SEQUENCE_ID, sequence_id, 0);
  }
  explicit StartDrawPayloadBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  StartDrawPayloadBuilder &operator=(const StartDrawPayloadBuilder &);
  flatbuffers::Offset<StartDrawPayload> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<StartDrawPayload>(end);
    return o;
  }
};

inline flatbuffers::Offset<StartDrawPayload> CreateStartDrawPayload(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t uid = 0,
    uint32_t color = 0,
    uint32_t sequence_id = 0) {
  StartDrawPayloadBuilder builder_(_fbb);
  builder_.add_sequence_id(sequence_id);
  builder_.add_color(color);
  builder_.add_uid(uid);
  return builder_.Finish();
}

struct EndDrawPayload FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UID = 4
  };
  uint32_t uid() const {
    return GetField<uint32_t>(VT_UID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_UID) &&
           verifier.EndTable();
  }
};

struct EndDrawPayloadBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_uid(uint32_t uid) {
    fbb_.AddElement<uint32_t>(EndDrawPayload::VT_UID, uid, 0);
  }
  explicit EndDrawPayloadBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  EndDrawPayloadBuilder &operator=(const EndDrawPayloadBuilder &);
  flatbuffers::Offset<EndDrawPayload> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<EndDrawPayload>(end);
    return o;
  }
};

inline flatbuffers::Offset<EndDrawPayload> CreateEndDrawPayload(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t uid = 0) {
  EndDrawPayloadBuilder builder_(_fbb);
  builder_.add_uid(uid);
  return builder_.Finish();
}

struct DrawPointsPayload FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UID = 4,
    VT_SEQUENCE_ID = 6,
    VT_POINTS = 8
  };
  uint32_t uid() const {
    return GetField<uint32_t>(VT_UID, 0);
  }
  uint32_t sequence_id() const {
    return GetField<uint32_t>(VT_SEQUENCE_ID, 0);
  }
  const flatbuffers::Vector<const Vec2 *> *points() const {
    return GetPointer<const flatbuffers::Vector<const Vec2 *> *>(VT_POINTS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_UID) &&
           VerifyField<uint32_t>(verifier, VT_SEQUENCE_ID) &&
           VerifyOffset(verifier, VT_POINTS) &&
           verifier.VerifyVector(points()) &&
           verifier.EndTable();
  }
};

struct DrawPointsPayloadBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_uid(uint32_t uid) {
    fbb_.AddElement<uint32_t>(DrawPointsPayload::VT_UID, uid, 0);
  }
  void add_sequence_id(uint32_t sequence_id) {
    fbb_.AddElement<uint32_t>(DrawPointsPayload::VT_SEQUENCE_ID, sequence_id, 0);
  }
  void add_points(flatbuffers::Offset<flatbuffers::Vector<const Vec2 *>> points) {
    fbb_.AddOffset(DrawPointsPayload::VT_POINTS, points);
  }
  explicit DrawPointsPayloadBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  DrawPointsPayloadBuilder &operator=(const DrawPointsPayloadBuilder &);
  flatbuffers::Offset<DrawPointsPayload> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<DrawPointsPayload>(end);
    return o;
  }
};

inline flatbuffers::Offset<DrawPointsPayload> CreateDrawPointsPayload(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t uid = 0,
    uint32_t sequence_id = 0,
    flatbuffers::Offset<flatbuffers::Vector<const Vec2 *>> points = 0) {
  DrawPointsPayloadBuilder builder_(_fbb);
  builder_.add_points(points);
  builder_.add_sequence_id(sequence_id);
  builder_.add_uid(uid);
  return builder_.Finish();
}

inline flatbuffers::Offset<DrawPointsPayload> CreateDrawPointsPayloadDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t uid = 0,
    uint32_t sequence_id = 0,
    const std::vector<Vec2> *points = nullptr) {
  auto points__ = points ? _fbb.CreateVectorOfStructs<Vec2>(*points) : 0;
  return Drawboard::CreateDrawPointsPayload(
      _fbb,
      uid,
      sequence_id,
      points__);
}

struct DeleteBatchPayload FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UID = 4,
    VT_SEQUENCE_ID = 6
  };
  uint32_t uid() const {
    return GetField<uint32_t>(VT_UID, 0);
  }
  uint32_t sequence_id() const {
    return GetField<uint32_t>(VT_SEQUENCE_ID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_UID) &&
           VerifyField<uint32_t>(verifier, VT_SEQUENCE_ID) &&
           verifier.EndTable();
  }
};

struct DeleteBatchPayloadBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_uid(uint32_t uid) {
    fbb_.AddElement<uint32_t>(DeleteBatchPayload::VT_UID, uid, 0);
  }
  void add_sequence_id(uint32_t sequence_id) {
    fbb_.AddElement<uint32_t>(DeleteBatchPayload::VT_SEQUENCE_ID, sequence_id, 0);
  }
  explicit DeleteBatchPayloadBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  DeleteBatchPayloadBuilder &operator=(const DeleteBatchPayloadBuilder &);
  flatbuffers::Offset<DeleteBatchPayload> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<DeleteBatchPayload>(end);
    return o;
  }
};

inline flatbuffers::Offset<DeleteBatchPayload> CreateDeleteBatchPayload(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t uid = 0,
    uint32_t sequence_id = 0) {
  DeleteBatchPayloadBuilder builder_(_fbb);
  builder_.add_sequence_id(sequence_id);
  builder_.add_uid(uid);
  return builder_.Finish();
}

struct UserEnterPayload FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UID = 4,
    VT_COLOR = 6
  };
  uint32_t uid() const {
    return GetField<uint32_t>(VT_UID, 0);
  }
  uint32_t color() const {
    return GetField<uint32_t>(VT_COLOR, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_UID) &&
           VerifyField<uint32_t>(verifier, VT_COLOR) &&
           verifier.EndTable();
  }
};

struct UserEnterPayloadBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_uid(uint32_t uid) {
    fbb_.AddElement<uint32_t>(UserEnterPayload::VT_UID, uid, 0);
  }
  void add_color(uint32_t color) {
    fbb_.AddElement<uint32_t>(UserEnterPayload::VT_COLOR, color, 0);
  }
  explicit UserEnterPayloadBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  UserEnterPayloadBuilder &operator=(const UserEnterPayloadBuilder &);
  flatbuffers::Offset<UserEnterPayload> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UserEnterPayload>(end);
    return o;
  }
};

inline flatbuffers::Offset<UserEnterPayload> CreateUserEnterPayload(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t uid = 0,
    uint32_t color = 0) {
  UserEnterPayloadBuilder builder_(_fbb);
  builder_.add_color(color);
  builder_.add_uid(uid);
  return builder_.Finish();
}

struct UserLeavePayload FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UID = 4
  };
  uint32_t uid() const {
    return GetField<uint32_t>(VT_UID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_UID) &&
           verifier.EndTable();
  }
};

struct UserLeavePayloadBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_uid(uint32_t uid) {
    fbb_.AddElement<uint32_t>(UserLeavePayload::VT_UID, uid, 0);
  }
  explicit UserLeavePayloadBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  UserLeavePayloadBuilder &operator=(const UserLeavePayloadBuilder &);
  flatbuffers::Offset<UserLeavePayload> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UserLeavePayload>(end);
    return o;
  }
};

inline flatbuffers::Offset<UserLeavePayload> CreateUserLeavePayload(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t uid = 0) {
  UserLeavePayloadBuilder builder_(_fbb);
  builder_.add_uid(uid);
  return builder_.Finish();
}

}  // namespace Drawboard

#endif  // FLATBUFFERS_GENERATED_PROTOCOL_DRAWBOARD_H_
