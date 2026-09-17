#pragma once
#include "vg_type.h"
#include "vertex.h"
namespace vg {
    enum enum_usage{
        gl_stream_draw,//GL_STREAM_DRAW
        gl_stream_read,//GL_STREAM_READ
        gl_stream_copy,//GL_STREAM_COPY
        gl_static_draw,//GL_STATIC_DRAW
        gl_static_read,//GL_STATIC_READ
        gl_static_copy,//GL_STATIC_COPY
        gl_dynamic_draw,//GL_DYNAMIC_DRAW
        gl_dynamic_read,//GL_DYNAMIC_READ
        gl_dynamic_copy,//GL_DYNAMIC_COPY
        gl_usage_cnt
    };
    #if  0
    enum enum_element_type{
        gl_no_element,//0
        gl_unsigned_byte,//GL_UNSIGNED_BYTE
        gl_unsigned_short,//GL_UNSIGNED_SHORT
        gl_unsigned_int,//GL_UNSIGNED_INT
        gl_element_type_cnt
    };
    #endif
    struct vertex_file_head {
          u16 demension : 4;// bigger than 0 less than 7 
          u16 format_index : 12;
          u16 type:1;//1 interlaced,0 progressive
          u16 usage:4;//enum_usage
          //u16 element_type:2;//enum_element_type
          u16 reserverd:11;
          u32 vertex_cnt;
    };
    extern u32 mem_usage[gl_usage_cnt];
    extern const char* mem_usage_show[ gl_usage_cnt ];
    constexpr auto vertex_head_size = sizeof (vertex_file_head);
	  using vertex_buffer=std::string;
    void get_vtx_format(u16 index,vtx_format& pmf );//pmf.size()>1
    u16 get_vtx_format_index(vtx_format& pmf);
    void vertex_to_vertex_buffer(vertex &vtx, vertex_buffer &vtx_bf);
    sd_vertex get_sd_vertex_from_vertex_buffer(vertex_buffer &vtx_bf);
    }