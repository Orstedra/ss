#include "d/a/obj/d_a_obj_god_mark.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_stage_mgr.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/event_manager.h"

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
    mStateMgr.executeState();
    return SUCCEEDED;
}
int dAcOgodMark_c::actorExecuteInEvent() {
    switch (mEventRelated.getCurrentEventCommand()) {
        case 'act0': {
            mEventRelated.advanceNext();
        } break;
        case 'act1': {
            actorExecute();
            if (field_0x444 == mAnmMatClr.getFrame(0)) {
                mEventRelated.advanceNext();
            }
        } break;
        case '????': {
            actorExecute();
        } break;
        default: {
            mEventRelated.advanceNext();
        }
    }
    return SUCCEEDED;
}

void dAcOgodMark_c::initializeState_Wait() {
    mAnmMatClr.setFrame(0.0f, 0);
    return;
}
void dAcOgodMark_c::executeState_Wait() {
    if (SceneflagManager::sInstance->checkFlag(roomid & 0xffff, field_0xad)) {
        void *zevDat = getOarcZev("GodsMark");
        Event event = Event("GodsMark", zevDat, 1, 0x100001, nullptr, nullptr);
        mEventRelated.scheduleEvent(event, 0);

        if (EventManager::isInEvent(this, "GodsMark")) {
            mStateMgr.changeState(StateID_Shine);
        }
    }
    return;
}

void dAcOgodMark_c::finalizeState_Wait() {}

void dAcOgodMark_c::initializeState_Shine() {}

void dAcOgodMark_c::executeState_Shine() {
    if (mAnmMatClr.getFrame(0) == field_0x444) {
        return;
    } else {
        mAnmMatClr.play();
    }
}
void dAcOgodMark_c::finalizeState_Shine() {}
