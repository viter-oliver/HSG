#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include <Commdlg.h>
#include <ShlObj.h>
#include <locale.h>
#include <windows.h>
#include "TextureHelper.h"
#include "resource.h"
#include "user_control_imgui.h"
#include "vg_texture.h"
#include "ifile_open.hpp"
extern HINSTANCE g_cur_instance;
namespace vg{
    using namespace std;
    using json=nlohmann::json;
    static bool isChked;
    UINT_PTR CALLBACK ofnHookProc(HWND hDlg, UINT uMsg, WPARAM wParam,
        LPARAM lParam) {
        OFNOTIFY* pofNotify;
        switch (uMsg) {
        case WM_NOTIFY:
            pofNotify = (OFNOTIFY*)lParam;
            switch (pofNotify->hdr.code) {
            case CDN_FILEOK:
                isChked = IsDlgButtonChecked(hDlg, IDC_CHK_MIPMAP);
            }
        default:
            break;
        }
        return FALSE;
    }
    template<>
    bool create_new_pair<texture>(texture_key& key,sd_texture& sd_txt){
        bool be_sucessful=false;
        static string key_str;
        static string format_str;
        if(ImGui::Button("New texture...")){
          key_str.clear();
          format_str.clear();
          isChked = false;
          ImGui::OpenPopup("New texture");
        }
        if (ImGui::BeginPopupModal("New texture")) {
          auto image_name = key_str.substr(key_str.find_last_of('\\') + 1);
          if (image_name.empty()) {
            image_name = "?";
          }
          ImGui::Text("Image:%s", image_name.c_str());
          ImGui::SameLine();
          if (ImGui::Button("Load new image...")) {
            OPENFILENAME ofn = {sizeof(OPENFILENAME)};
            // ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = GetForegroundWindow();
            ofn.lpstrFilter =
                "image "
                "file:\0*.png;*.bmp;*.jpg;*.jpeg;*.gif;*.dds;*.tga;*.psd;*.hdr\0\0";
            //ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
             //           OFN_HIDEREADONLY | OFN_ENABLETEMPLATE | OFN_ENABLEHOOK;
            //ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                        OFN_HIDEREADONLY | OFN_ENABLETEMPLATE ;

            ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_MIPMAP);
            ofn.hInstance = g_cur_instance;// GetModuleHandle(NULL);
            ofn.lpfnHook = ofnHookProc;
            char strFileName[MAX_PATH] = {0};
            ofn.nFilterIndex = 1;
            ofn.lpstrFile = strFileName;
            ofn.nMaxFile = sizeof(strFileName);
            ofn.lpstrTitle = "Loading image...";
            if (GetOpenFileName(&ofn)) {
              key_str = strFileName;
            }
          }
          auto format_name =
              format_str.substr(format_str.find_last_of('\\') + 1);
          if (format_name.empty()) {
            format_name = "?";
          }
          ImGui::Text("Image Format:%s", format_name.c_str());
          ImGui::SameLine();
          if (ImGui::Button("Image format...")) {
            OPENFILENAME ofn = {sizeof(OPENFILENAME)};
            ofn.hwndOwner = GetForegroundWindow();
            ofn.lpstrFilter = "image format:\0*.json\0\0";
            char strFileName[MAX_PATH] = {0};
            ofn.nFilterIndex = 1;
            ofn.lpstrFile = strFileName;
            ofn.nMaxFile = sizeof(strFileName);
            ofn.lpstrTitle = "Loading image format...";
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            if (GetOpenFileName(&ofn)) {
              format_str = strFileName;
            }
          }
          if (ImGui::Button("Ok")) {
            auto read_format = [](string &file_name, v_s_sub_txt &vsub) {
              ifstream iformat(file_name);
              if (!iformat.is_open()) {
                return;
              }
              json jdata = json::parse(iformat);
              auto &meta = jdata["meta"];
              auto &jsize = meta["size"];
              float w = jsize["w"];
              float h = jsize["h"];
              auto &frames = jdata["frames"];
              if (frames.is_array()) {
                auto frame_sz = frames.size();
                for (int ix = 0; ix < frame_sz; ix++) {
                  auto &frame_unit = frames[ix];
                  auto &frame = frame_unit["frame"];
                  string filename = frame_unit["filename"];
                  bool rotated = frame_unit["rotated"];
                  int x0 = frame["x"];
                  int y0 = frame["y"];
                  int x1 = x0 + frame["w"];
                  int y1 = y0 + frame["h"];
                  if (rotated) {
                    x1 = x0 + frame["h"];
                    y1 = y0 + frame["w"];
                  }
                  auto s_st = make_shared<sub_txt>();
                  *s_st = {filename, (float)x0, (float)y0, (float)x1,
                           (float)y1};
                  vsub.emplace_back(s_st);
                }
              } else { // is object
                for (auto &frame_unit : frames.items()) {
                  auto &junit = frame_unit.value();
                  auto &frame = junit["frame"];
                  string filename = junit["filename"];
                  bool rotated = junit["rotated"];
                  int x0 = frame["x"];
                  int y0 = frame["y"];
                  int x1 = x0 + frame["w"];
                  int y1 = y0 + frame["h"];
                  if (rotated) {
                    x1 = x0 + frame["h"];
                    y1 = y0 + frame["w"];
                  }
                  auto s_st = make_shared<sub_txt>();
                  *s_st = {filename, (float)x0, (float)y0, (float)x1,
                           (float)y1};
                  vsub.emplace_back(s_st);
                }
              }
            };
            int image_w, image_h;
            auto itxt =
                TextureHelper::load2DTexture(key_str.c_str(), image_w, image_h,
                              isChked);
            if (itxt > 0) {
              key.get_str_value( image_name);
              sd_txt = make_shared<texture>();
              sd_txt->_txt_id = itxt;
              sd_txt->_width = image_w;
              sd_txt->_height = image_h;
              read_format(format_str, sd_txt->_v_sd_sub_txt);
              be_sucessful = true;
            }
            ImGui::CloseCurrentPopup();
          }
          ImGui::SameLine(0,100);
          if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
          }
          ImGui::EndPopup();
        }
        return be_sucessful;
    }
    void ShowTextureMarker(const char *desc) {
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
          ImGui::TextUnformatted(desc);
          ImGui::PopTextWrapPos();
          ImGui::EndTooltip();
        }
    }
    template<>
    void res_edit<texture>(texture&tx){
        ImGui::Text("Texture width:%d", tx._width);
        ImGui::Text("Texture height:%d", tx._height);
        ImGui::Checkbox("mipmap", &tx.mipmap);
        if (tx._v_sd_sub_txt.size() > 0) {
          const int max_width = 500,seprate_width=10;
          int draw_width = 0, draw_height = 0;
          for (int ix = 0; ix < tx._v_sd_sub_txt.size(); ix++) {
            auto &sub = *tx._v_sd_sub_txt[ix];
            ImVec2 draw_size(sub.width(), sub.height());
            ImVec2 uv0(sub._x0 / tx._width,
                       sub._y0 / tx._height);
            ImVec2 uv1(sub._x1 / tx._width,
                       sub._y1 / tx._height);
            ImGui::Image((ImTextureID)tx._txt_id, draw_size,
                         uv0, uv1,
                         ImColor(255, 255, 255, 255),
                         ImColor(255, 255, 255, 128));
            ShowTextureMarker(sub.item_name.c_str());
            draw_width += draw_size.x;
            if (draw_width < max_width) {
              ImGui::SameLine(0, seprate_width);
              draw_width += seprate_width;
              if (draw_size.y > draw_height)
                draw_height = draw_size.y;
            } else {
              draw_width = 0;
              draw_height = 0;
            }
          }
        } else {
          ImGui::Image((ImTextureID)tx._txt_id, ImVec2(tx._width, tx._height),
                       ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255),
                       ImColor(255, 255, 255, 128));
        }
    }
    REG_MAP_COPONENT_EDIT_CONTAINER(texture)

    struct open_texture :public IFileOpener {
        bool tryOpen(const std::string& path) {
            size_t pos = path.find_last_of('.');
            if (pos == std::string::npos) {
                return false;
            }
            std::string ext = path.substr(pos + 1);
            const char* valid_exts[] = {
                "png", "bmp", "jpg", "jpeg", "gif", "dds", "tga", "psd", "hdr",nullptr
			};
            for (int ix = 0; valid_exts[ix] != nullptr; ix++) {
                if (ext == valid_exts[ix]) {
                    std::string img_file_name = path.substr(path.find_last_of('\\') + 1);
                    sd_texture sd_txt = make_shared<texture>();
                    int image_w, image_h;
                    auto itxt =TextureHelper::load2DTexture(path.c_str(), image_w, image_h, false);
                    if (itxt <= 0) {
                        return false;
                    }
                    sd_txt->_txt_id = itxt;
                    sd_txt->_width = image_w;
                    sd_txt->_height = image_h;
                    texture_key tkey;
                    tkey.get_str_value(img_file_name);
					texture_assist_._sd_o->add_new_ele(tkey, sd_txt);
                    return  true;
                }
            }
            return false;
        }
	};
}