namespace vg{
  using namespace shader;
  void add_shader_base_by_key(shader_base_key& key, sd_shader_base& s_sh){
      shader_base_assist_._sd_o->add_new_ele(key, s_sh);
  }
  void add_vertex_by_key(vertex_key& key, sd_vertex& s_vtx){
    vertex_assist_._sd_o->add_new_ele( key,s_vtx );
  }
  void add_texture_by_key(texture_key& key, sd_texture& s_txt){
    texture_assist_._sd_o->add_new_ele(key, s_txt);
  }
}