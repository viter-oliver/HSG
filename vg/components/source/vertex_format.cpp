#include <map>
#if defined(_GLFW_WIN32)
#include <GL/gl3w.h>
#else
#include "glad/glad.h"
#endif
#include "vertex_format.h"
namespace vg {
#include "vertex_format_table.h"
u32 mem_usage[gl_usage_cnt]={
  GL_STREAM_DRAW,
  GL_STREAM_READ,
  GL_STREAM_COPY,
  GL_STATIC_DRAW,
  GL_STATIC_READ,
  GL_STATIC_COPY,
  GL_DYNAMIC_DRAW,
  GL_DYNAMIC_READ,
  GL_DYNAMIC_COPY,
};
std::map<u32, enum_usage> mem_usage_id = {
  {GL_STREAM_DRAW, gl_stream_draw},
  {GL_STREAM_READ, gl_stream_read},
  {GL_STREAM_COPY, gl_stream_copy}, 
  {GL_STATIC_DRAW, gl_static_draw},
  {GL_STATIC_READ, gl_stream_read}, 
  {GL_STATIC_COPY, gl_static_copy},
  {GL_DYNAMIC_DRAW, gl_dynamic_draw}, 
  {GL_DYNAMIC_READ, gl_dynamic_read}, 
  {GL_DYNAMIC_COPY, gl_dynamic_copy},
};
const char *mem_usage_show[gl_usage_cnt] = {
"GL_STREAM_DRAW",
"GL_STREAM_READ", 
"GL_STREAM_COPY", 
"GL_STATIC_DRAW", 
"GL_STATIC_READ", 
"GL_STATIC_COPY", 
"GL_DYNAMIC_DRAW", 
"GL_DYNAMIC_READ", 
"GL_DYNAMIC_COPY",
};

const u16 ptm_indx_tab[] = {
	4,
	32,
	192,
	1024,
	5120,
	24576,
};
#if 0
u32 ele_type[gl_element_type_cnt]={
0,
GL_UNSIGNED_BYTE,
GL_UNSIGNED_SHORT,
GL_UNSIGNED_INT,
};
u8 ele_size[ gl_element_type_cnt ] = {
  0,
  sizeof( u8 ),
  sizeof( u16 ),
  sizeof( u32 )
};
#endif
void get_vtx_format(u16 index, vtx_format& pmf) { 
  auto demension = pmf.size();
  if (demension < 1) return;
  u8* phead = ptm_table;
  auto span_index = demension - 1;
  u16 span = 0;
  for (int ix = 0; ix < span_index; ix++) {
    span += ptm_indx_tab[ix];
  }
  phead += span;
  auto cur_span = index * demension;
  phead += cur_span;
  for (auto& pm : pmf) {
    pm = *phead++;
  }
}
u16 get_vtx_format_index(vtx_format& pmf) {
  auto ix = pmf.size() - 1;
  u16 index = 0;
  auto pow_v = [](u8 b, u8 cnt) {
    u16 vb = 1;
    for (u16 i = 0; i < cnt; i++) {
      vb *= b;
    }
    return vb;
  };
  for (u16 i = 0; i < pmf.size(); ix--, i++) {
    auto pu = pmf[ix] - 1;
    auto pv = pu * pow_v(4, i);
    index += pv;
  }
  return index;
}
void vertex_to_vertex_buffer(vertex &vtx, vertex_buffer &vtx_bf) {
  auto std_len = vtx.stride();
  //USHRT_MAX
  auto vertex_buffer_len = vtx._vertex_count * std_len * sizeof(float);
  auto ele_buffer_len = vtx._ele_count * sizeof(u32);
  auto file_buffur_len = vertex_head_size + vertex_buffer_len + ele_buffer_len;
  vtx_bf.resize(file_buffur_len);
  char *pdata = & vtx_bf[0];
  vertex_file_head *phead = (vertex_file_head *)pdata;
  phead->demension = vtx._format.size();
  phead->format_index= get_vtx_format_index(vtx._format);
  phead->type =vtx._interlaced;
  phead->usage = mem_usage_id[vtx._mem_usage];
  phead->vertex_cnt = vtx._vertex_count;
  pdata += vertex_head_size;
  if (vtx._interlaced) {
    glBindBuffer(GL_ARRAY_BUFFER, vtx._vbos[0]);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, vertex_buffer_len, pdata);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
  } else {
    for (int ix = 0; ix < vtx._format.size(); ix++) {
      glBindBuffer(GL_ARRAY_BUFFER, vtx._vbos[ix]);
      auto vtx_ele_buffer_len =vtx._format[ix] * vtx._vertex_count * sizeof(float);
      glGetBufferSubData(GL_ARRAY_BUFFER, 0, vtx_ele_buffer_len, pdata);
      pdata += vtx_ele_buffer_len;
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }
  if (ele_buffer_len > 0) {
      pdata += vertex_buffer_len;
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vtx._ebo);
      glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ele_buffer_len, pdata);
      //FIXME      
      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }

}
sd_vertex get_sd_vertex_from_vertex_buffer(vertex_buffer &vtx_bf) {
   char *pdata = &vtx_bf[0];
   vertex_file_head *phead = (vertex_file_head *)pdata;
   vtx_format vtx_fmt;
   vtx_fmt.resize(phead->demension);
   get_vtx_format(phead->format_index, vtx_fmt);
   u8 std_len = 0;
   for (auto el : vtx_fmt) {
      std_len += el;
   }

   auto vtx_buffer_len = phead->vertex_cnt * std_len * sizeof(float);
   u32 uf_sz = vtx_bf.size();
   auto ele_buffer_len = uf_sz - vertex_head_size - vtx_buffer_len;
   u32 ele_cnt = ele_buffer_len / sizeof(u32);
   pdata += vertex_head_size;
   u32 *pele = 0;
   if (ele_cnt > 0) {
      pele = (u32 *)(pdata + vtx_buffer_len);
   }
   auto sd_vtx = std::make_shared<vertex>(
       vtx_fmt, (float *)pdata, phead->vertex_cnt, mem_usage[phead->usage],
       (bool)phead->type, pele, ele_cnt);
   return sd_vtx;
}
}