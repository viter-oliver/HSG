#include "state_sets.h"
#include "easing.h"
namespace vg {
    namespace state_sets {
        using namespace std;
        using namespace chrono;
        m_sd_state_set g_state_sets;

        static char* str_play_state[en_play_state_cnt] = { "stopped", "playing a trans",
                                                          "playing a playlist" };
        AFG_EXPORT bool trans_is_playing(string trans_name) {
            const auto& istm = g_state_sets.find(trans_name);
            if (istm == g_state_sets.end()) {
                printf("invalid state manager name:%s\n", trans_name.c_str());
                return false;
            }
            auto& stm = *istm->second;
            if (stm._play_state != en_play_stop) {
                printf("state manager:%s is running in state:%s\n", trans_name.c_str(),
                    str_play_state[stm._play_state]);
                return true;
            }
            return false;
        }
        AFG_EXPORT bool play_tran(string stm_name, int from, int to,
            bool cover_from_value) {
            const auto& istm = g_state_sets.find(stm_name);
            if (istm == g_state_sets.end()) {
                printf("invalid state manager name:%s\n", stm_name.c_str());
                return false;
            }
            auto& stm = *istm->second;
            if (stm._play_state != en_play_stop) {
                printf("state manager:%s is running in state:%s\n", stm_name.c_str(),
                    str_play_state[stm._play_state]);
                return false;
            }
            const auto& itran = stm._mtrans.find({ from, to });
            if (itran == stm._mtrans.end()) {
                return false;
            }
            stm._pcur_tran = itran->second;
            stm._trans_start = steady_clock::now();
            stm._play_state = en_play_tran;
            stm._cur_from = from;
            stm._cur_to = to;
            if (cover_from_value) {
                auto& vstate_list = stm._vstate_list;
                for (auto& su : vstate_list) {
                    auto& _var = *(*su).var_bind;
                    auto var_member = _var.bind_member();
                    auto& desvalue = (*su).value_list[from];
                    var_member->assign(desvalue);
                }
            }
            return true;
        }
        AFG_EXPORT bool play_tran_playlist(string stm_name, int playlist_id) {
            const auto& istm = g_state_sets.find(stm_name);
            if (istm == g_state_sets.end()) {
                printf("invalid state manager name:%s\n", stm_name.c_str());
                return false;
            }
            auto& stm = *istm->second;
            if (stm._play_state != en_play_stop) {
                printf("state manager:%s is running in state:%s\n", stm_name.c_str(),
                    str_play_state[stm._play_state]);
                return false;
            }
            if (playlist_id >= stm._playlist_list.size()) {
                printf("for state manager:%s,%d is invalid\n", stm_name.c_str(),
                    playlist_id);
                return false;
            }
            stm._cur_playlist_id = playlist_id;
            stm._play_state = en_play_tran_playlist;
            stm._trans_start = steady_clock::now();
            stm._cur_play_trans_id = 0;
            return true;
        }

        void keep_state_trans_on() {
            for (auto& istm : g_state_sets) {
                auto& stm = *istm.second;
                if (!stm._play_state) {
                    continue;
                }
                if (stm._play_state == en_play_tran) {
                    if (!stm._pcur_tran) {
                        continue;
                    }
                    assert(stm._cur_from != stm._cur_to && "invalid from or to");
                    auto cur_clk = steady_clock::now();
                    auto& play_clk = stm._trans_start;
                    auto dur_mills = duration_cast<milliseconds>(cur_clk - play_clk);
                    auto& cur_trans = *stm._pcur_tran;
                    auto delta_tm =
                        dur_mills.count() -
                        cur_trans
                        ._start_time; // note��ostart_time2��2?��?����??��?��???��??������o��??o��?a��?
                    if (delta_tm > 0) {
                        double tm_pt_mill = (double)delta_tm / cur_trans._duration;
                        auto& easing_fun = easingFun[cur_trans._easing_func];
                        double value_scale = easing_fun(tm_pt_mill);
                        if (delta_tm >= cur_trans._duration) {
                            stm._play_state = en_play_stop;
                            value_scale = 1.0;
                            if (stm._trans_finish) {
                                stm._trans_finish(stm._cur_from, stm._cur_to);
                            }
                        }

                        auto& vstate_list = stm._vstate_list;
                        for (auto& su : vstate_list) {
                            auto& _var = *(*su).var_bind;
                            auto var_member = _var.bind_member();
                            auto& from_value = (*su).value_list[stm._cur_from];
                            auto& to_value = (*su).value_list[stm._cur_to];
                            var_member->scale(from_value, to_value, value_scale);
                        }
                    }
                }
                else {
                    assert(stm._cur_playlist_id < stm._playlist_list.size() &&
                        "invalid playlist id?");
                    auto& playlist = stm._playlist_list[stm._cur_playlist_id];
                    auto& cur_pid = stm._cur_play_trans_id;
                    assert(cur_pid < playlist.size() && "invalid play id?");
                    auto cur_clk = steady_clock::now();
                    auto& play_clk = stm._trans_start;
                    auto dur_mills = duration_cast<milliseconds>(cur_clk - play_clk);
                    auto& cur_trankey = playlist[cur_pid];
                    auto& itrans = stm._mtrans.find(cur_trankey);
                    assert(itrans != stm._mtrans.end() && "invalid trans key?");
                    auto& cur_trans = *itrans->second;
                    auto delta_tm = dur_mills.count() - cur_trans._start_time;
                    if (delta_tm > 0) {
                        double tm_pt_mill = (double)delta_tm / cur_trans._duration;
                        auto& easing_fun = easingFun[cur_trans._easing_func];
                        double value_scale = easing_fun(tm_pt_mill);
                        if (delta_tm >= cur_trans._duration) {
                            value_scale = 1.0;
                            auto last_id = playlist.size() - 1;
                            if (cur_pid == last_id) {
                                stm._play_state = en_play_stop;
                                cur_pid = 0;
                                stm._state_idx = cur_trankey._to;
                                if (stm._trans_finish) {
                                    stm._trans_finish(cur_trankey._from, cur_trankey._to);
                                }
                            }
                            else {
                                cur_pid++;
                                stm._trans_start = steady_clock::now();
                            }
                        }
                        auto& vstate_list = stm._vstate_list;
                        for (auto& su : vstate_list) {
                            auto& _var = *(*su).var_bind;
                            auto var_member = _var.bind_member();
                            auto& from_value = (*su).value_list[cur_trankey._from];
                            auto& to_value = (*su).value_list[cur_trankey._to];
                            var_member->scale(from_value, to_value, value_scale);
                        }
                    }
                }
            }
        }
        AFG_EXPORT bool restore_2_state(std::string trans_name, int state_id) {
            const auto& itrans = g_state_sets.find(trans_name);
            if (itrans == g_state_sets.end()) {
                printf("invalid sate name:%s\n", trans_name.c_str());
                return false;
            }
            auto& trans = *itrans->second;
            auto& vstate_list = trans._vstate_list;
            if (vstate_list.size() == 0) {
                printf("state:%s is empty!\n", trans_name.c_str());
                return false;
            }

            if (state_id >= vstate_list[0]->value_list.size()) {
                printf("invalid sate_id:%d\n", state_id);
                return false;
            }
            for (auto& su : vstate_list) {
                auto& _var = *(*su).var_bind;
                auto var_member = _var.bind_member();
                auto& desvalue = (*su).value_list[state_id];
                var_member->assign(desvalue);
            }
        }

        AFG_EXPORT bool cancel_play_tran(string stm_name) {
            const auto& istm = g_state_sets.find(stm_name);
            if (istm == g_state_sets.end()) {
                printf("invalid state manager name:%s\n", stm_name.c_str());
                return false;
            }
            auto& stm = *istm->second;
            stm._play_state = en_play_stop;
            return true;
        }

        AFG_EXPORT bool reg_trans_handle(string trans_name, trans_finish_handle trans_handle) {
            const auto& itrans = g_state_sets.find(trans_name);
            if (itrans == g_state_sets.end()) {
                return false;
            }
            auto& trans = *itrans->second;
            trans._trans_finish = trans_handle;
            return true;
        }
    }
} // namespace vg
