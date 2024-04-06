#include "HitboxFactory.h"
#include "../type/Hitbox.h"
#include "spdlog/spdlog.h"

namespace SF64 {
std::shared_ptr<LUS::IResource> ResourceFactoryBinaryHitboxV0::ReadResource(std::shared_ptr<LUS::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto hitbox = std::make_shared<Hitbox>(file->InitData);
    auto reader = std::get<std::shared_ptr<LUS::BinaryReader>>(file->Reader);

    auto count = reader->ReadUInt32();

    for (uint32_t i = 0; i < count; i++) {
        hitbox->mHitbox.push_back(reader->ReadFloat());
    }

    return hitbox;
}
} // namespace LUS
