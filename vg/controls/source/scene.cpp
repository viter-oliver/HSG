#include "common_functions.h"
#include "scene.h"
#include "user_control_imgui.h"
namespace vg {
scene::scene() : _fboId(0), _colorTextId(0), _depthStencilTextId(0) {
  size() = {640, 480};
  _sn_pt._bk_clr = {0.2f, 0.2f, 0.5f, 0.5f};
#if defined(WITHIN_IDE)
  
#endif
}
void scene::release_resource() {
  if (_colorTextId)
    glDeleteTextures(1, &_colorTextId);
  if (_depthStencilTextId)
    glDeleteTextures(1, &_depthStencilTextId);
  if (_fboId) {
    glDeleteFramebuffers(1, &_fboId);
    _fboId = 0;
  }
}
scene::~scene() { release_resource(); }
void scene::link() {
  prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, size().x,
              size().y);
}

void scene::draw() {
  if (!_fboId) {
    vg_print("fail to draw for scene");
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
  if (_sn_pt._test_depth) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
  // glDisable(GL_SCISSOR_TEST);
  glClearColor(_sn_pt._bk_clr.x, _sn_pt._bk_clr.y, _sn_pt._bk_clr.z,
               _sn_pt._bk_clr.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  for (auto it : _vchilds) {
    if (it->visibility()) {
      it->draw();
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
  ImGui::Image((ImTextureID)_colorTextId, to_imvec2(size()), ImVec2(1, 1), ImVec2(0, 0));
  ImGui::SetCursorPos(cur_cursor_pos);
}

} // namespace vg
