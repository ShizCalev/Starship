#include <libultraship.h>

#include "Engine.h"
#include "DisplayList.h"
#include "Array.h"

extern "C" int GameEngine_OTRSigCheck(const char* data);

extern "C" void gSPDisplayList(Gfx* pkt, Gfx* dl) {
    char* imgData = (char*)dl;

    if (GameEngine_OTRSigCheck(imgData) == 1) {
        auto resource = LUS::Context::GetInstance()->GetResourceManager()->LoadResource(imgData);
        auto res = std::static_pointer_cast<LUS::DisplayList>(resource);
        dl = &res->Instructions[0];
    }

    __gSPDisplayList(pkt, dl);
}

extern "C" void gSPVertex(Gfx* pkt, uintptr_t v, int n, int v0) {

    if (GameEngine_OTRSigCheck((char*)v) == 1) {
        v = (uintptr_t) ResourceGetDataByName((char *) v);
    }

    __gSPVertex(pkt, v, n, v0);
}

extern "C" void gSPInvalidateTexCache(Gfx* pkt, uintptr_t texAddr) {
    char* imgData = (char*)texAddr;

    if (texAddr != 0 && GameEngine_OTRSigCheck(imgData)) {
        auto res = LUS::Context::GetInstance()->GetResourceManager()->LoadResource(imgData);

        if (res->GetInitData()->Type == LUS::ResourceType::DisplayList)
            texAddr = (uintptr_t)&((std::static_pointer_cast<LUS::DisplayList>(res))->Instructions[0]);
        else if (res->GetInitData()->Type == LUS::ResourceType::Array)
            texAddr = (uintptr_t)(std::static_pointer_cast<LUS::Array>(res))->Vertices.data();
        else {
            texAddr = (uintptr_t) res->GetRawPointer();
        }
    }
   __gSPInvalidateTexCache(pkt, texAddr);
}