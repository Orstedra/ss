#include "d/a/obj/d_a_obj_god_mark.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

#include <cstddef>

SPECIAL_ACTOR_PROFILE(OBJ_GOD_MARK, dAcOgodMark_c, fProfile::OBJ_GOD_MARK, 0x20A, 0, 6);

STATE_DEFINE(dAcOgodMark_c, Wait);
STATE_DEFINE(dAcOgodMark_c, Shine);

int dAcOgodMark_c::create() {
    CREATE_ALLOCATOR(dAcOgodMark_c);

    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    boundingBox.Set(min, max);

    return SUCCEEDED;
}
int dAcOgodMark_c::doDelete() {
    return SUCCEEDED;
}
int dAcOgodMark_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
bool dAcOgodMark_c::createHeap() {
    rot_copy = params & 0xf;
    if ((params & 0xf) == 0xf) {
        rot_copy = 0;
    }

    mRes = nw4r::g3d::ResFile(getOarcResFile("GodsMark"));

    nw4r::g3d::ResMdl resMdl = mRes.GetResMdl("GodsMark");

    if (!mMdl.create(resMdl, &heap_allocator, 0x120, 1, nullptr)) {
        return false;
    } else {
        nw4r::g3d::ResAnmTexSrt resAnmTexSrt = mRes.GetResAnmTexSrt("GodsMark");
        if (!mAnmTexSrt.create(resMdl, resAnmTexSrt, &heap_allocator, nullptr, 1)) {
            nw4r::g3d::ResAnmClr anmClr = mRes.GetResAnmClr("GodsMark_F");
            if (mAnmMatClr.create(resMdl, anmClr, &heap_allocator, nullptr, 1) == 0) {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}
int dAcOgodMark_c::actorExecute() {
    return SUCCEEDED;
}
int dAcOgodMark_c::actorExecuteInEvent() {
    return SUCCEEDED;
}

void dAcOgodMark_c::initializeState_Wait() {}
void dAcOgodMark_c::executeState_Wait() {}
void dAcOgodMark_c::finalizeState_Wait() {}
void dAcOgodMark_c::initializeState_Shine() {}
void dAcOgodMark_c::executeState_Shine() {}
void dAcOgodMark_c::finalizeState_Shine() {}
