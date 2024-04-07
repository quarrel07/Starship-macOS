#include "GenericArrayFactory.h"
#include "../type/GenericArray.h"
#include "spdlog/spdlog.h"

namespace SF64 {
std::shared_ptr<LUS::IResource> ResourceFactoryBinaryGenericArrayV0::ReadResource(std::shared_ptr<LUS::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto arr = std::make_shared<GenericArray>(file->InitData);
    auto reader = std::get<std::shared_ptr<LUS::BinaryReader>>(file->Reader);

    auto type = reader->ReadUInt32();

    SPDLOG_INFO("GenericArray Type Num: {}", type);

    auto count = reader->ReadUInt32();

    SPDLOG_INFO("GenericArray Count: {}", count);

    for (uint32_t i = 0; i < count; i++) {
        switch (static_cast<ArrayType>(type)) {
            case ArrayType::u8: {
                auto x = reader->ReadUByte();
                arr->mData.emplace_back(x);
                break;
            }
            case ArrayType::s8: {
                auto x = reader->ReadInt8();
                arr->mData.emplace_back(x);
                break;
            }
            case ArrayType::u16: {
                auto x = reader->ReadUInt16();
                arr->mData.emplace_back(x);
                break;
            }
            case ArrayType::s16: {
                auto x = reader->ReadInt16();
                arr->mData.emplace_back(x);
                break;
            }
            case ArrayType::u32: {
                auto x = reader->ReadUInt32();
                arr->mData.emplace_back(x);
                break;
            }
            case ArrayType::s32: {
                auto x = reader->ReadInt32();
                arr->mData.emplace_back(x);
                break;
            }
            case ArrayType::u64: {
                auto x = reader->ReadUInt64();
                arr->mData.emplace_back(x);
                break;
            }
            case ArrayType::f32: {
                auto x = reader->ReadFloat();
                arr->mData.emplace_back(x);
                break;
            }
            case ArrayType::f64: {
                auto x = reader->ReadDouble();
                arr->mData.emplace_back(x);
                break;
            }
            case ArrayType::Vec2f: {
                auto x = reader->ReadFloat();
                auto y = reader->ReadFloat();
                arr->mData.emplace_back(Vec2f(x, y));
                break;
            }
            case ArrayType::Vec3f: {
                auto x = reader->ReadFloat();
                auto y = reader->ReadFloat();
                auto z = reader->ReadFloat();
                arr->mData.emplace_back(Vec3f(x, y, z));
                break;
            }
            case ArrayType::Vec3s: {
                auto x = reader->ReadInt16();
                auto y = reader->ReadInt16();
                auto z = reader->ReadInt16();
                arr->mData.emplace_back(Vec3s(x, y, z));
                break;
            }
            case ArrayType::Vec3i: {
                auto x = reader->ReadInt32();
                auto y = reader->ReadInt32();
                auto z = reader->ReadInt32();
                arr->mData.emplace_back(Vec3i(x, y, z));
                break;
            }
            case ArrayType::Vec4f: {
                auto x = reader->ReadFloat();
                auto y = reader->ReadFloat();
                auto z = reader->ReadFloat();
                auto w = reader->ReadFloat();
                arr->mData.emplace_back(Vec4f(x, y, z, w));
                break;
            }
            case ArrayType::Vec4s: {
                auto x = reader->ReadInt16();
                auto y = reader->ReadInt16();
                auto z = reader->ReadInt16();
                auto w = reader->ReadInt16();
                arr->mData.emplace_back(Vec4s(x, y, z, w));
                break;
            }
        }
    }

    return arr;
}
} // namespace LUS
