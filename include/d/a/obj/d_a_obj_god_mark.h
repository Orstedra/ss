#ifndef D_A_OBJ_GOD_MARK_H
#define D_A_OBJ_GOD_MARK_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_sph_chk.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOgodMark_c : public dAcObjBase_c {
public:
    dAcOgodMark_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOgodMark_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;

    STATE_FUNC_DECLARE(dAcOgodMark_c, Wait);
    STATE_FUNC_DECLARE(dAcOgodMark_c, Shine);

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    m3d::anmTexSrt_c mAnmTexSrt;
    m3d::anmMatClr_c mAnmMatClr;
    dBgS_SphChk mSphChk;
    /* 0x??? */ STATE_MGR_DECLARE(dAcOgodMark_c);
};

#endif
