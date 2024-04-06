#include "LimbFactory.h"

#include "Context.h"
#include "resourcebridge.h"
#include "../type/Limb.h"
#include "ResourceUtil.h"

namespace SF64 {

std::shared_ptr<LUS::IResource> ResourceFactoryBinaryLimbV0::ReadResource(std::shared_ptr<LUS::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto limb = std::make_shared<Limb>(file->InitData);
    auto reader = std::get<std::shared_ptr<LUS::BinaryReader>>(file->Reader);

    limb->mData.dList = LoadChild<Gfx*>(reader->ReadUInt64());
    limb->mData.trans.x = reader->ReadFloat();
    limb->mData.trans.y = reader->ReadFloat();
    limb->mData.trans.z = reader->ReadFloat();

    limb->mData.rot.x = reader->ReadInt16();
    limb->mData.rot.y = reader->ReadInt16();
    limb->mData.rot.z = reader->ReadInt16();

    limb->mData.sibling = LoadChild<LimbData*>(reader->ReadUInt64());
    limb->mData.child = LoadChild<LimbData*>(reader->ReadUInt64());

    return limb;
}
} // namespace LUS
