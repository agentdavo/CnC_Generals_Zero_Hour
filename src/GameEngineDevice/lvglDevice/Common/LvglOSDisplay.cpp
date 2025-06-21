#include "Common/OSDisplay.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "GameClient/GameText.h"
#include "LvglPlatform/LvglPlatform.h"
#include <lvgl.h>

struct ButtonEventData {
    lv_obj_t *box;
    OSDisplayButtonType value;
    bool *done;
    OSDisplayButtonType *result;
};

static void btn_event_cb(lv_event_t *e)
{
    ButtonEventData *data = static_cast<ButtonEventData *>(lv_event_get_user_data(e));
    *data->result = data->value;
    *data->done = true;
    lv_msgbox_close(data->box);
}

static void close_event_cb(lv_event_t *e)
{
    ButtonEventData *data = static_cast<ButtonEventData *>(lv_event_get_user_data(e));
    if(!*data->done) {
        *data->result = data->value;
        *data->done = true;
    }
}

static const char *icon_from_flags(UnsignedInt flags)
{
    if(BitTest(flags, OSDOF_EXCLAMATIONICON)) return LV_SYMBOL_WARNING;
    if(BitTest(flags, OSDOF_INFORMATIONICON)) return LV_SYMBOL_BELL;
    if(BitTest(flags, OSDOF_ERRORICON) || BitTest(flags, OSDOF_STOPICON)) return LV_SYMBOL_CLOSE;
    return nullptr;
}

OSDisplayButtonType OSDisplayWarningBox(AsciiString p, AsciiString m, UnsignedInt buttonFlags, UnsignedInt otherFlags)
{
    if(!TheGameText)
        return OSDBT_ERROR;

    UnicodeString promptStr = TheGameText->fetch(p);
    UnicodeString mesgStr = TheGameText->fetch(m);

    AsciiString promptA, mesgA;
    promptA.translate(promptStr);
    mesgA.translate(mesgStr);

    bool modal = BitTest(otherFlags, OSDOF_SYSTEMMODAL) ||
                 BitTest(otherFlags, OSDOF_APPLICATIONMODAL) ||
                 BitTest(otherFlags, OSDOF_TASKMODAL) ||
                 otherFlags == 0; /*Win32 default*/

    lv_obj_t *parent = modal ? lv_layer_top() : lv_scr_act();

    if(modal) {
        lv_obj_add_flag(parent, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_color(parent, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_set_style_bg_opa(parent, LV_OPA_50, 0);
    }

    lv_obj_t *box = lv_msgbox_create(parent);
    lv_msgbox_add_title(box, promptA.str());
    const char *icon = icon_from_flags(otherFlags);
    if(icon)
        lv_msgbox_add_header_button(box, icon);
    lv_msgbox_add_text(box, mesgA.str());

    bool done = false;
    OSDisplayButtonType result = OSDBT_CANCEL;

    OSDisplayButtonType close_value = BitTest(buttonFlags, OSDBT_CANCEL) ? OSDBT_CANCEL : OSDBT_OK;
    ButtonEventData close_data{box, close_value, &done, &result};
    lv_obj_t *close_btn = lv_msgbox_add_close_button(box);
    lv_obj_add_event_cb(close_btn, close_event_cb, LV_EVENT_CLICKED, &close_data);

    if(BitTest(buttonFlags, OSDBT_OK)) {
        ButtonEventData ok_data{box, OSDBT_OK, &done, &result};
        lv_obj_t *btn = lv_msgbox_add_footer_button(box, "OK");
        lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, &ok_data);
    }

    if(BitTest(buttonFlags, OSDBT_CANCEL)) {
        ButtonEventData cancel_data{box, OSDBT_CANCEL, &done, &result};
        lv_obj_t *btn = lv_msgbox_add_footer_button(box, "Cancel");
        lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, &cancel_data);
    }

    lv_obj_center(box);

    while(!done) {
        LvglPlatform::poll_events();
    }

    if(modal) {
        lv_obj_remove_flag(parent, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_opa(parent, LV_OPA_TRANSP, 0);
    }

    return result;
}
