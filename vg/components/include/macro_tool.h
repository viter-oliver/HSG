#include <memory>
#include <string>
#include <map>
#include <vector>
#define DEFINE_SMART(cname)\
using sd_##cname = std::shared_ptr<cname>;\
using v_sd_##cname =std::vector<sd_##cname>;\
using m_sd_##cname=std::map<std::string,sd_##cname>;