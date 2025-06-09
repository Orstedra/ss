#ifndef D_A_OBJ_GOD_MARK_H
#define D_A_OBJ_GOD_MARK_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/stage_render_stuff.h"

class dAcOgodMark_c : public dAcObjBase_c {
public:
    dAcOgodMark_c() : mStateMgr(*this, sStateID::null), mSceneCallback(this), mEventRelated(*this, nullptr) {}
    virtual ~dAcOgodMark_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;

    STATE_FUNC_DECLARE(dAcOgodMark_c, Wait);
    STATE_FUNC_DECLARE(dAcOgodMark_c, Shine);

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 maybe 0xa0? */ m3d::smdl_c mMdl;
    /* 0x??? */ STATE_MGR_DECLARE(dAcOgodMark_c);
    /* 0x138 */ ActorEventRelated mEventRelated;
    m3d::anmTexSrt_c mAnmTexSrt;
    /* Should be 0x408 (1032) */ m3d::anmMatClr_c mAnmMatClr;
    dScnCallback_c mSceneCallback;
    u8 field_0xad;
    u8 mFlag;
    f32 field_0x444;
};

#endif
