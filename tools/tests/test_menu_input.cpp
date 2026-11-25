#include <cassert>
#include <array>
#include <vector>
#include "recompinput/menu_input_policy.h"
#include "sssv_menu_input.h"
#undef main
using namespace recompinput;
int main() {
 for (auto type : {InputType::Mouse, InputType::ControllerDigital}) {
  std::array<std::vector<InputField>, num_game_inputs> mappings{};
  InputField click{type,1}, normalA{type,2};
  mappings[(size_t)GameInput::A]={normalA}; mappings[(size_t)GameInput::B]={click};
  auto read=[&](const InputField& f){return f==click;};
  assert(menu_button_down(mappings,GameInput::B,true,read));
  mappings[(size_t)GameInput::ACCEPT_MENU]={click};
  assert(menu_button_down(mappings,GameInput::A,true,read));
  assert(!menu_button_down(mappings,GameInput::B,true,read));
  assert(menu_button_down(mappings,GameInput::B,false,read));
  assert(!menu_button_down(mappings,GameInput::A,false,read));
  mappings[(size_t)GameInput::ACCEPT_MENU].clear();
  assert(menu_button_down(mappings,GameInput::A,true,[&](const InputField& f){return f==normalA;}));
 }
 std::vector<uint8_t> memory(0x800000);
 auto byte=[&](uint32_t a,uint8_t v){memory[(a&0x7fffff)^3]=v;};
 assert(!sssv::menu::active(memory.data()));
 byte(0x803f2d10,2);assert(sssv::menu::active(memory.data())); // bank/zone
 byte(0x803f2d10,0);byte(0x803f6680,1);assert(sssv::menu::active(memory.data())); // pause
 byte(0x80152e90,1);byte(0x80299dfc,3);assert(sssv::menu::title(memory.data()));
 assert(!sssv::menu::active(memory.data()));
 byte(0x80299dfc,1);assert(!sssv::menu::title(memory.data()));
 assert(sssv::menu::startup(memory.data()));
 uint16_t a=0x8000;sssv::menu::skip_startup(a);assert(a&0x1000);
 uint16_t bOnly=0x4000;sssv::menu::skip_startup(bOnly);assert(!(bOnly&0x1000));
 byte(0x80152e90,0);byte(0x803f2d38,0);byte(0x803f2d39,36);
 assert(sssv::menu::startup(memory.data()));
 byte(0x803f2d39,1);assert(!sssv::menu::startup(memory.data()));
 byte(0x803f6680,0);byte(0x803f6681,0);
 assert(sssv::menu::new_game_intro(memory.data()));
 byte(0x8023f297,1);assert(!sssv::menu::new_game_intro(memory.data()));
 for (uint16_t b : {0x8000,0x4000}) {sssv::menu::advance_title(b);assert(b&0x1000);}
}
