#include "Vec3fFactory.h"
#include "../type/Vec3fArray.h"
#include "spdlog/spdlog.h"

namespace SF64 {
std::shared_ptr<LUS::IResource> ResourceFactoryBinaryVec3fV0::ReadResource(std::shared_ptr<LUS::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto vec = std::make_shared<Vec3fArray>(file->InitData);
    auto reader = std::get<std::shared_ptr<LUS::BinaryReader>>(file->Reader);

    auto vecCount = reader->ReadUInt32();

    SPDLOG_INFO("Vec3f Count: {}", vecCount);

    for (uint32_t i = 0; i < vecCount; i++) {
        auto x = reader->ReadFloat();
        auto y = reader->ReadFloat();
        auto z = reader->ReadFloat();
        vec->mData.emplace_back(x, y, z);
    }

    return vec;
}
} // namespace LUS