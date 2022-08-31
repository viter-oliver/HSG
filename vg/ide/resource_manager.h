#include "user_control_imgui.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace vg {
template <typename baseType, bool is_vector = true> struct res_container {
  using type = std::vector<std::shared_ptr<baseType>>;
};
template <typename baseType> struct res_container<baseType, false> {
  using type = std::map<std::string, std::shared_ptr<baseType>>;
};

template <typename baseType, bool is_vector = true> class res_manager {
  using resContainer = res_container<baseType, is_vector>;
  resContainer &container_host;
  std::string res_name, item_name_sel;
  using sd_base_type = std::shared_ptr<baseType>;
  sd_base_type cur_item = nullptr;
  // vector container
  void
  iterate_container(typename res_container<baseType, true>::type &container) {
    // std::cout << "it is vector" << std::endl;
    u32 idx=0;
    for (auto &sd_res_unit : container) {
      auto &res_unit = *sd_res_unit;
      auto& item_name=res_unit.name;
      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;
      if (res_unit._sel) {
        node_flags |= ImGuiTreeNodeFlags_Selected;
      }
      IconTreeNode(res_name, item_name.c_str(), node_flags);
      if (ImGui::IsItemClicked()) {
        if (cur_item) {
          cur_item->_sel = false;
        }
        cur_item = sd_res_unit;
        cur_item->_sel = true;
        cur_item->select_item(idx);
        item_name_sel=item_name;
      }
      ImGui::TreePop();
      idx++;
    }
  }
  // map container
  void
  iterate_container(typename res_container<baseType, false>::type &container) {
    // std::cout << "it is map" << std::endl;
    for(auto& it_res_unit:container){
      auto& item_name=it_res_unit.first;
      auto& res_unit=*it_res_unit.second;
      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;
      if (res_unit._sel) {
        node_flags |= ImGuiTreeNodeFlags_Selected;
      }
      IconTreeNode(res_name, item_name.c_str(), node_flags);
      if (ImGui::IsItemClicked()) {
        if (cur_item) {
          cur_item->_sel = false;
        }
        cur_item = it_res_unit.second;
        cur_item->_sel = true;
        cur_item->select_item(item_name);
        item_name_sel=item_name;
      }
      ImGui::TreePop();
      
    }
  }
  bool opened{false};
public:

  res_manager(resContainer &container_target, std::string resName)
      : container_host(container_target), res_name(resName) {}
  void menu_switch(){
    auto menu_name = res_name + " manager";
    if(ImGui::MenuItem(menu_name.c_str(),NULL,opened)){
      opened=!opened;
    }
  }
  void operator()() {
    auto window_name = res_name + " manager";
    ImGui::Begin(window_name.c_str(), &opened);
    ImGui::Columns(2);
    auto item_list = res_name + "list:";
    ImGui::Text(item_list.c_str());
    // TODO:create_item
    auto btn_cap="Create "+res_name;
    if(ImGui::Button(btn_cap.c_str())){
      ImGui::OpenPopup(btn_cap.c_str())
    }
    if(ImGui::BeginPopupModal(btn_cap.c_str())){
      create_item(container_host);
      ImGui::EndPopup();
    }
    // TODO:tree of items
    iterate_container(container_host);
    ImGui::NextColumn()
    // TODO:edit_item   
    if(cur_item){
      cur_item->edit_item();
    }
    ImGui::End()
  }
};
} // namespace vg