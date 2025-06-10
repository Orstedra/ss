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

static const char *godsMarkTypes[] = {"GodsMark_F", "GodsMark_D", "GodsMark_N"};

static const char name[] = "GodsMark";

SPECIAL_ACTOR_PROFILE(OBJ_GOD_MARK, dAcOgodMark_c, fProfile::OBJ_GOD_MARK, 0x20A, 0, 6);

STATE_DEFINE(dAcOgodMark_c, Wait);
STATE_DEFINE(dAcOgodMark_c, Shine);

bool dAcOgodMark_c::createHeap() {
    u8 markParam = params & 0xF;
    mMarkType = markParam;
    if (markParam == 0xF) {
        mMarkType = 0;
    }

    mRes = nw4r::g3d::ResFile(getOarcResFile(name));
    nw4r::g3d::ResMdl resMdl = mRes.GetResMdl(name);

    if (!mMdl.create(resMdl, &heap_allocator, 0x32c, 1, nullptr)) {
        return false;
    }
    if (!mAnmTexSrt.create(resMdl, mRes.GetResAnmTexSrt(name), &heap_allocator, nullptr, 1)) {
        return false;
    }
    if (!mAnmMatClr.create(resMdl, mRes.GetResAnmClr(godsMarkTypes[mMarkType]), &heap_allocator, nullptr, 1)) {
        return false;
    }
    return true;
}

int dAcOgodMark_c::create() {
    CREATE_ALLOCATOR(dAcOgodMark_c);

    mFlag = params >> 4;

    mSceneCallback.attach(mMdl);

    mMdl.setAnm(mAnmTexSrt);

    mAnmTexSrt.setFrame(static_cast<f32>(mMarkType), 0);

    mMdl.setAnm(mAnmMatClr);

    field_0x444 = mAnmMatClr.getFrameMax(0) - 1.0f;

    updateMatrix();

    mMdl.setLocalMtx(mWorldMtx);

    bool checkFlag = SceneflagManager::sInstance->checkFlag(roomid, mFlag);

    if (checkFlag) {
        mAnmMatClr.setFrame(field_0x444, 0);
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
    bool checkFlag = SceneflagManager::sInstance->checkFlag(roomid & 0xffff, mFlag);
    if (checkFlag) {
        void *zevDat = getOarcZev(name);
        Event event = Event(name, zevDat, 1, 0x100001, nullptr, nullptr);
        mEventRelated.scheduleEvent(event, 0);
    }
    if (EventManager::isInEvent(this, name)) {
        mStateMgr.changeState(StateID_Shine);
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
