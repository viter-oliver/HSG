#pragma once
#include "bind_base.h"
#include <chrono>
namespace vg {

// const float cal_step = 0.025;
    namespace state_sets {
        struct trans_key {
            int _from, _to;
            bool operator<(const trans_key& ttrans) const {
                if (_from != ttrans._from) {
                    return _from < ttrans._from;
                }
                else {
                    return _to < ttrans._to;
                }
            }
            bool operator==(const trans_key& ttrans) const {
                return _from == ttrans._from && _to == ttrans._to;
            }
            /*trans_key& operator =( trans_key& ttrans )
            {
                 _from = ttrans._from;
                 _to = ttrans._to;
                 return *this;
            }*/
            bool valid() { return _from != _to; }
        };
        struct state_transition {

            int _start_time;  // ms{0}
            int _duration;    // ms{250}
            int _easing_func; // {EaseLinear};

            // easing_functions _easing_out_func{ EaseLinear };
        };
        using sd_var_base = std::shared_ptr<var_base>;
        using v_sd_var_base = std::vector<sd_var_base>;
        using sp_st_trans = std::shared_ptr<state_transition>;
        using vtrans_key = std::vector<trans_key>;
        using vvtrans_key = std::vector<vtrans_key>;
        using mp_trans = std::map<trans_key, sp_st_trans>;
        // using  vstrans= vector<state_transition>;
        enum moving_state { en_state_pause, en_state_moving, en_state_out };
        enum trans_play_state {
            en_play_stop,
            en_play_tran,
            en_play_tran_playlist,
            en_play_state_cnt
        };
        struct state_unit {
            sd_bind_base var_bind;
            v_sd_var_base value_list;
        };
        DEFINE_SMART(state_unit)
            using trans_finish_handle = std::function<void(int from, int to)>;
        using st_clock = std::chrono::steady_clock;
        using st_clock_point = st_clock::time_point;
        struct state_set {
            v_sd_state_unit _vstate_list;
            state_transition _any_to_any;
            trans_finish_handle _trans_finish;
            sp_st_trans _pcur_tran{ nullptr };
            mp_trans _mtrans; //
            unsigned char _state_idx{ 0 };
            moving_state _mstate{ en_state_pause };
            trans_play_state _play_state{ en_play_stop };
            st_clock_point _trans_start;
            int _cur_from{ 0 }, _cur_to{ 0 };
            int _cur_play_trans_id{ 0 }, _cur_playlist_id{ 0 };
            vvtrans_key _playlist_list;
        };
        DEFINE_SMART(state_set)
            extern AFG_EXPORT m_sd_state_set g_state_sets;
        AFG_EXPORT bool trans_is_playing(std::string trans_name);

        AFG_EXPORT bool reg_trans_handle(std::string trans_name,
            trans_finish_handle trans_handle);
        AFG_EXPORT bool play_tran(std::string stm_name, int from, int to,
            bool cover_from_value = true);
        AFG_EXPORT bool play_tran_playlist(std::string stm_name, int playlist_id);

        AFG_EXPORT void keep_state_trans_on();

        AFG_EXPORT bool cancel_play_tran(std::string stm_name);

        AFG_EXPORT bool restore_2_state(std::string trans_name, int state_id);
    }
} // namespace vg