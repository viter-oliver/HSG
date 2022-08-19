#include <fstream>
#include <msgpack.hpp>
namespace vg {
using mpk_stream = msgpack::packer<std::ofstream>;
using mpk_obj = msgpack::v1::object;
} // namespace vg
