#include "MessageLookupFactory.h"
#include "../type/Message.h"
#include "spdlog/spdlog.h"

#include "resourcebridge.h"

namespace SF64 {
std::shared_ptr<LUS::IResource> ResourceFactoryBinaryMessageLookupV0::ReadResource(std::shared_ptr<LUS::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto table = std::make_shared<MessageLookup>(file->InitData);
    auto reader = std::get<std::shared_ptr<LUS::BinaryReader>>(file->Reader);
    auto count = reader->ReadUInt32();

    for (uint32_t i = 0; i < count; i++) {
        SPDLOG_INFO("Reading message lookup table entry {}", i);
        auto id = reader->ReadInt32();
        uint16_t* ptr = static_cast<uint16_t*>(ResourceGetDataByCrc(reader->ReadUInt64()));
        table->mLookupTable.push_back({ id, ptr });
    }

    return table;
}
} // namespace LUS
