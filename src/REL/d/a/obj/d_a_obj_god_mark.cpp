#include "d/a/obj/d_a_obj_god_mark.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/event_manager.h"

const char *godsMarkTypes[] = {"GodsMark_F", "GodsMark_D", "GodsMark_N"};

SPECIAL_ACTOR_PROFILE(OBJ_GOD_MARK, dAcOgodMark_c, fProfile::OBJ_GOD_MARK, 0x20A, 0, 6);

STATE_DEFINE(dAcOgodMark_c, Wait);
STATE_DEFINE(dAcOgodMark_c, Shine);

bool dAcOgodMark_c::createHeap() {
    u8 rot = params & 0xF;
    if (rot == 0xF) {
        rot = 0;
    }
    rot_copy = rot;

    mRes = nw4r::g3d::ResFile(getOarcResFile("GodsMark"));
    nw4r::g3d::ResMdl resMdl = mRes.GetResMdl("GodsMark");

    if (!mMdl.create(resMdl, &heap_allocator, 0x120, 1, nullptr)) {
        return false;
    }
    if (!mAnmTexSrt.create(resMdl, mRes.GetResAnmTexSrt("GodsMark"), &heap_allocator, nullptr, 1)) {
        return false;
    }
    if (!mAnmMatClr.create(resMdl, mRes.GetResAnmClr(godsMarkTypes[0]), &heap_allocator, nullptr, 1) == 0) {
        return false;
    }

    return true;
}

int dAcOgodMark_c::create() {
    CREATE_ALLOCATOR(dAcOgodMark_c);

    _1[10] = params >> 4;

    mSceneCallback.attach(mMdl);

    mMdl.setAnm(mAnmTexSrt);

    double nose = 4503599627370496.0;

    mAnmTexSrt.setFrame(field_0x444 - nose, 0);

    mMdl.setAnm(mAnmMatClr);

    targetFiTextId = mAnmMatClr.getFrameMax(0) - 1.0;

    updateMatrix();

    mMdl.setLocalMtx(mWorldMtx);

    bool checkFlag = SceneflagManager::sInstance->checkFlag(roomid, field_0xad);

    if (checkFlag) {
        mAnmMatClr.setFrame(targetFiTextId, 0);
        mStateMgr.changeState(StateID_Shine);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }

    boundingBox.Set(mVec3_c(-300.0f, 500.0f, -2600.0f), mVec3_c(300.0f, 1100.0f, -2200.0f));

    return SUCCEEDED;
}
int dAcOgodMark_c::doDelete() {
    return SUCCEEDED;
}
int dAcOgodMark_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
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
