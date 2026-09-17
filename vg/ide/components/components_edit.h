#pragma once
#include <vector>
#include <concepts>
#include "user_control_imgui.h"
#include "common_functions.h"
#include<string>
#include <algorithm>
namespace vg{
  extern std::string g_sdk_path;

  #if 0
  template <typename T>
  concept IsAResKey=std::is_base_of<base_key,T>::value;
  template <IsAResKey key,class ele> 
  bool create_new_pair(key&key_,std::shared_ptr<ele>& sd_ele){
    return true;
  }
  #endif
  template <class ele>
  bool create_new_pair(key_t<ele>& key_,std::shared_ptr<ele>& sd_ele){
    return true;
  }
  template <class ele>
  void res_edit(ele& ele_){}
  class base_component_edit
  {
  protected:
    bool _visible{false};   
    std::string _name; 
   public:
      base_component_edit(std::string& cp_name)
      :_name(cp_name){}
      virtual void show()=0;
      virtual void clear() = 0;
      bool visible(){
        return _visible;
      }
      void menu_item(){
        if(ImGui::MenuItem(_name.c_str(),NULL,_visible)){
          _visible=!_visible;
        }
      }
  };
  using sd_base_component_edit=std::shared_ptr<base_component_edit>;
  using v_sd_base_component_edit=std::vector<sd_base_component_edit>;
  inline v_sd_base_component_edit v_sd_base_component_edit_;
 #if 0
  enum { en_max_cp_id,};
#define GEN_ID(cp_ele) enum { cp_ele##_id = en_max_cp_id, en_max_cp_id };
#endif
#define REG_MAP_COPONENT_EDIT_CONTAINER(cp_ele)   inline struct cp_ele##_assist{\
    std::shared_ptr<component_edit<cp_ele>> _sd_o;\
    inline cp_ele##_assist(){\
      _sd_o=std::make_shared<component_edit<cp_ele>>(cp_ele##_map_,#cp_ele);\
      v_sd_base_component_edit_.emplace_back(_sd_o);\
    }\
  } cp_ele##_assist_;
#define REG_SIMPLE_COPONENT_EDIT_CONTAINER(cp_ele_simple)   inline struct cp_ele_simple##_assist{\
    std::string name=#cp_ele_simple;\
    cp_ele_simple##_assist(){\
      auto sd_o=std::make_shared<cp_ele_simple>(name);\
      v_sd_base_component_edit_.emplace_back(sd_o);\
    }\
  } cp_ele_simple##_assist_;
  template <class ele>
  class component_edit:public base_component_edit{
     using sd_ele=std::shared_ptr<ele>;
     using key_type=key_t<ele>;
     using map_type=std::map<key_type,sd_ele>;  
     using menu_handle = std::function<void(ele&,key_type&)>;
     using mp_menu_handle = std::map<std::string, menu_handle>;
     mp_menu_handle _map_mhl;
     map_type& _map_type; 
     sd_ele _sel{nullptr};  
     key_type _key;
  public:
       component_edit( map_type& mtype, std::string cp_name )
            :_map_type( mtype ), base_component_edit( cp_name )
       {
       }
       auto &operator ()(std::string menu, menu_handle mdl) {
         _map_mhl[menu] = mdl;
         return *this;
       }
       void show()
       {
            if( _visible )
            {
                 ImGui::Begin( _name.c_str(), &_visible );
                 ImGui::Columns( 2 );
                 draw_lists();
                 ImGui::NextColumn();
                 draw_item();
                 ImGui::End();
            }
       }
       void clear()
       {
            _map_type.clear();
       }
       void add_new_ele(key_type& key_, sd_ele sd_ele_) {
           while (_map_type.find(key_) != _map_type.end())
           {
               //gray@2024.08.28 comment::modify rename func 
               int suffix = 1;
               std::string base_name(key_.value);
               std::string cur_name{};
               do {
                   cur_name = base_name + "_(" + std::to_string(suffix) + ")";
                   suffix++;
               } while (std::any_of(_map_type.begin(), _map_type.end(),
                   [&cur_name](const auto& pair) {
                       return pair.first.value == cur_name;
                   }));

               strncpy(key_.value, cur_name.data(), cur_name.size());
               key_.value[cur_name.size()] = '\0';
           }
           _map_type[key_] = sd_ele_;
       }
  private:
    void draw_lists(){
      key_type key_;
      sd_ele sd_ele_=nullptr;
      if(create_new_pair<ele>(key_, sd_ele_)){
		  add_new_ele(key_, sd_ele_);
      }
      
      std::string res_count=_name+"("+std::to_string(_map_type.size())+")";
      //if (IconTreeNode(_name.c_str(), res_count.c_str(),ImGuiTreeNodeFlags_DefaultOpen)) 
      internal_image(_name.c_str());
      ImGui::SameLine(30);
      if (ImGui::TreeNode(_name.c_str(),res_count.c_str()))
      {
        for(auto& pair_item:_map_type){
          auto& key_name=pair_item.first;
          auto& item=pair_item.second;
          ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;
          if( _sel ==item){
            node_flags |= ImGuiTreeNodeFlags_Selected;
          }
          //IconTreeNode(_name.c_str(),key_name.value,node_flags);

          internal_image(_name.c_str());
          ImGui::SameLine(30);
          ImGui::TreeNodeEx(key_name.value, node_flags);
          if (ImGui::IsItemHovered()) {
            node_flags != ImGuiTreeNodeFlags_Framed;
          }
          if(ImGui::IsItemClicked()){
            _sel =item;
            _key = key_name;
          }
          if (_sel&&ImGui::BeginDragDropSource()) {
            internal_image(_name.c_str());
            ImGui::SameLine(0,20);
            ImGui::Text(_key.value);
            ImGui::SetDragDropPayload(_name.c_str(), _key.value,
                                      sizeof(key_type));
            ImGui::EndDragDropSource();
          }
          /**
          accepted window:
          if (ImGui::BeginDragDropTarget())
          {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_name.c_str())){
                
            }
            ImGui::EndDragDropTarget();
          } 
          */
         ImGui::TreePop();
        }
        ImGui::TreePop();
      }
      if (_sel && ImGui::BeginPopupContextWindow()) {
        if (ImGui::MenuItem("Delete", NULL, false,
                            _sel.use_count() == 2)) {
          auto item_del = _map_type.find(_key);
          _map_type.erase(item_del);
          _sel = nullptr;
        }
        if (ImGui::MenuItem("Copy key", NULL, false)) {
          std::string key_str(_key.value);
          store_to_clipboard(key_str);
        }
        for (const auto &[key, value] : _map_mhl) {
          if (ImGui::MenuItem(key.c_str(), NULL, false)) {
            value(*_sel, _key);
          }
        }
        ImGui::EndPopup();
      }
    }
    void draw_item(){
      if( _sel ){
        res_edit(*_sel );
      }
    }
  };
}
