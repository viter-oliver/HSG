#include "light_scene.h"
#include "common_functions.h"
#include "user_control_imgui.h"
#include "vertex_container.h"
namespace vg {
light_scene::light_scene() {
  size() = {1500, 1500};
  _pj_pt._view_pos = {0.f, 206.f, -600.f};
  _pj_pt._center_of_prj = {0.f, 90.f, 0.f};

  _pj_pt._up = {0, 1.f, 0};
  _pj_pt._fovy = 20;
  _pj_pt._near = 20;
  _pj_pt._far = 100000.f;
  _pj_pt._test_depth = true;
  _pj_pt._bk_clr = {0.2f, 0.2f, 0.2f, 1.f};
  _pj_pt._light_pos = {0.f, -80.f, -60};
  _pj_pt._light_ambient_clr = {0.17, 0.17, 0.17};
  _pj_pt._light_diffuse_clr = {1, 1, 1};
  _pj_pt._light_specular_clr = {1, 1, 1};
}
void light_scene::release_resource() {
  if (_colorTextId)
    glDeleteTextures(1, &_colorTextId);
  if (_depthStencilTextId)
    glDeleteTextures(1, &_depthStencilTextId);
  if (_fboId) {
    glDeleteFramebuffers(1, &_fboId);
    _fboId = 0;
  }
}
light_scene::~light_scene() { release_resource(); }
void light_scene::link() {
  prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, size().x, size().y);
}
void light_scene::draw_frames() {
  if (!_fboId) {
    return;
  }
  GLint last_viewport[4];
  glGetIntegerv(GL_VIEWPORT, last_viewport);
  GLint last_fmid;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_fmid);
  glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
  glViewport(0, 0, size().x, size().y);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LESS);
  // glDisable( GL_CULL_FACE );
  if (_pj_pt._test_depth) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
  // glDisable(GL_SCISSOR_TEST);
  glClearColor(_pj_pt._bk_clr.x, _pj_pt._bk_clr.y, _pj_pt._bk_clr.z,
               _pj_pt._bk_clr.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  for (auto it : _vchilds) {
    if (it->visibility()) {
      it->draw_frames();
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, last_fmid);
  glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2],
             (GLsizei)last_viewport[3]);
  auto abpos = absolute_coordinate_of_base_pos();
  ImVec2 winpos = ImGui::GetWindowPos();
  auto drawPos = winpos + abpos;
  auto cur_cursor_pos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(drawPos);
  ImGui::Image((ImTextureID)_colorTextId, to_imvec2(size()), ImVec2(1, 1),
               ImVec2(0, 0));
  ImGui::SetCursorPos(cur_cursor_pos);
}

bool light_scene::handle_mouse() { return true; }

} // namespace vg
