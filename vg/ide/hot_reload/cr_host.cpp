#include "from_lanchor.h"
#define CR_HOST CR_UNSAFE
#include "cr.h"
#ifdef _DEBUG
#define TARGET_NAME "HSG_TARGETd"
#else
#define TARGET_NAME "HSG_TARGET"
#endif // DEBUG

const char *plugin = RUNNING_PATH CMAKE_INTDIR "/" CR_PLUGIN(TARGET_NAME);
const char *usr_plugin = "hsg_plugin//HSG_TARGET_usr.dll";
inline bool exists_test(const std::string &name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}
int main(int argc, char **argv) {
    static from_lanchor to_gust;
    static std::string user_plugin;
    to_gust.executable = argv[0];
    if(argc>1){
      to_gust.project_file = argv[1];
      auto &target_path = to_gust.project_file;
      auto project_dir=target_path.substr(0, target_path.find_last_of('\\') + 1);
      user_plugin = project_dir + usr_plugin;
    }
    cr_plugin ctx;
    ctx.userdata = &to_gust;
    int rt = 0;
  restart:
    if (exists_test(user_plugin)) {
        cr_plugin_open(ctx, user_plugin.c_str());
    } else {
      cr_plugin_open(ctx,plugin);
    }
    while (true) {
        auto rt=cr_plugin_update(ctx);
        if(rt==1||rt==2){
            break;
        }
    }
    cr_plugin_close(ctx);
    if (rt == 2) {
        goto restart;
    }
    return 0;
}