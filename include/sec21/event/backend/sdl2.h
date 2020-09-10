#pragma once

#include <SDL2/SDL_events.h>

#include <sec21/event/events.h>

#include <tuple>
#include <optional>

namespace sec21::event::backend
{
   class sdl2
   {
   public:
      std::tuple<bool, event_t> poll_event()
      {
         SDL_Event event{};
         if (SDL_PollEvent(&event)) {
            return { true, to_event(event) };
         }		
         return { false, {} };
      }

      static std::optional<SDL_Event> to_event(event_t const& event) noexcept
      {
         return sec21::match(event,
            [](auto const& value) -> std::optional<SDL_Event> { return transform_event(value); },
            [](time_elapsed const&) -> std::optional<SDL_Event> { return {}; },
            [](std::monostate const&) -> std::optional<SDL_Event> { return {}; }
         );
      }

   private:
      event_t to_event(SDL_Event const& event) const noexcept
      {
         using namespace sec21::event;

         const auto to_keycode = [](auto const& k)
         {
            switch (k)
            {
               case SDLK_a:
                  return Keycode::A;
               case SDLK_b:
                  return Keycode::B;
               case SDLK_c:
                  return Keycode::C;
               case SDLK_d:
                  return Keycode::D;
               case SDLK_e:
                  return Keycode::E;
               case SDLK_f:
                  return Keycode::F;
               case SDLK_g:
                  return Keycode::G;
               case SDLK_h:
                  return Keycode::H;
               case SDLK_i:
                  return Keycode::I;
               case SDLK_j:
                  return Keycode::J;
               case SDLK_k:
                  return Keycode::K;
               case SDLK_l:
                  return Keycode::L;
               case SDLK_m:
                  return Keycode::M;
               case SDLK_n:
                  return Keycode::N;
               case SDLK_o:
                  return Keycode::O;
               case SDLK_p:
                  return Keycode::P;
               case SDLK_q:
                  return Keycode::Q;
               case SDLK_r:
                  return Keycode::R;
               case SDLK_s:
                  return Keycode::S;
               case SDLK_t:
                  return Keycode::T;
               case SDLK_u:
                  return Keycode::U;
               case SDLK_v:
                  return Keycode::V;
               case SDLK_w:
                  return Keycode::W;
               case SDLK_x:
                  return Keycode::X;
               case SDLK_y:
                  return Keycode::Y;
               case SDLK_z:
                  return Keycode::Z;
               case SDLK_0:
                  return Keycode::Num0;
               case SDLK_1:
                  return Keycode::Num1;
               case SDLK_2:
                  return Keycode::Num2;
               case SDLK_3:
                  return Keycode::Num3;
               case SDLK_4:
                  return Keycode::Num4;
               case SDLK_5:
                  return Keycode::Num5;
               case SDLK_6:
                  return Keycode::Num6;
               case SDLK_7:
                  return Keycode::Num7;
               case SDLK_8:
                  return Keycode::Num8;
               case SDLK_9:
                  return Keycode::Num9;
               case SDLK_ESCAPE:
                  return Keycode::Escape;
               case SDLK_LEFTBRACKET:
                  return Keycode::LeftBracket;
               case SDLK_RIGHTBRACKET:
                  return Keycode::RightBracket;
               case SDLK_SEMICOLON:
                  return Keycode::Semicolon;
               case SDLK_KP_COMMA:
                  return Keycode::Comma;
               case SDLK_PERIOD:
                  return Keycode::Period;
               case SDLK_QUOTE:
                  return Keycode::Quote;
               case SDLK_SLASH:
                  return Keycode::Slash;
               case SDLK_BACKSLASH:
                  return Keycode::Backslash;
               case SDLK_EQUALS:
                  return Keycode::Equal;
               case SDLK_SPACE:
                  return Keycode::Space;
               case SDLK_RETURN:
                  return Keycode::Enter;
               case SDLK_BACKSPACE:
                  return Keycode::Backspace;
               case SDLK_TAB:
                  return Keycode::Tab;
               case SDLK_PAUSE:
                  return Keycode::Pause;
               case SDLK_PAGEUP:
                  return Keycode::PageUp;
               case SDLK_PAGEDOWN:
                  return Keycode::PageDown;
               case SDLK_END:
                  return Keycode::End;
               case SDLK_HOME:
                  return Keycode::Home;
               case SDLK_INSERT:
                  return Keycode::Insert;
               case SDLK_LEFT:
                  return Keycode::Left;
               case SDLK_RIGHT:
                  return Keycode::Right;
               case SDLK_UP:
                  return Keycode::Up;
               case SDLK_DOWN:
                  return Keycode::Down;
               case SDLK_KP_0:
                  return Keycode::Numpad0;
               case SDLK_KP_1:
                  return Keycode::Numpad1;
               case SDLK_KP_2:
                  return Keycode::Numpad2;
               case SDLK_KP_3:
                  return Keycode::Numpad3;
               case SDLK_KP_4:
                  return Keycode::Numpad4;
               case SDLK_KP_5:
                  return Keycode::Numpad5;
               case SDLK_KP_6:
                  return Keycode::Numpad6;
               case SDLK_KP_7:
                  return Keycode::Numpad7;
               case SDLK_KP_8:
                  return Keycode::Numpad8;
               case SDLK_KP_9:
                  return Keycode::Numpad9;
               case SDLK_F1:
                  return Keycode::F1;
               case SDLK_F2:
                  return Keycode::F2;
               case SDLK_F3:
                  return Keycode::F3;
               case SDLK_F4:
                  return Keycode::F4;
               case SDLK_F5:
                  return Keycode::F5;
               case SDLK_F6:
                  return Keycode::F6;
               case SDLK_F7:
                  return Keycode::F7;
               case SDLK_F8:
                  return Keycode::F8;
               case SDLK_F9:
                  return Keycode::F9;
               case SDLK_F10:
                  return Keycode::F10;
               case SDLK_F11:
                  return Keycode::F11;
               case SDLK_F12:
                  return Keycode::F12;
               default:
                  return Keycode::Unknown;
            }
         };

         switch (event.type)	
         {
            case SDL_QUIT:
               return close_window{};
            case SDL_KEYDOWN:
             	return make_event<pressed<key>>(
            		(event.key.keysym.mod & KMOD_ALT), 
            		(event.key.keysym.mod & KMOD_CTRL),
            		(event.key.keysym.mod & KMOD_GUI),
            		(event.key.keysym.mod & KMOD_SHIFT),
            		to_keycode(event.key.keysym.sym));
            case SDL_KEYUP:
             	return make_event<released<key>>(
            		(event.key.keysym.mod & KMOD_ALT), 
            		(event.key.keysym.mod & KMOD_CTRL),
            		(event.key.keysym.mod & KMOD_GUI),
            		(event.key.keysym.mod & KMOD_SHIFT),
            		to_keycode(event.key.keysym.sym));                  
            case SDL_MOUSEMOTION:
               return make_event<moved<mouse>>(event.motion.x, event.motion.y);
            case SDL_MOUSEBUTTONDOWN:
   				return make_event<pressed<mouse_button>>(event.button.button, event.button.x, event.button.y);
            case SDL_MOUSEBUTTONUP:
   				return make_event<released<mouse_button>>(event.button.button, event.button.x, event.button.y);
            default:
               return std::monostate{};
         }
      }

      static SDL_Event transform_event(moved<mouse> const& value) noexcept
      {
         // clang-format off
         return SDL_Event{ 
            .motion = SDL_MouseMotionEvent{
               .type = SDL_MOUSEMOTION,
               .timestamp = {},
               .windowID = {},
               .which = {},
               .state = {},
               .x = value.source.x,
               .y = value.source.y,
               .xrel = {},
               .yrel = {}
            }
         };
         // clang-format on
      }

      static SDL_Event transform_event(pressed<mouse_button> const& value) noexcept
      {
         // clang-format off
         return SDL_Event{ 
            .button = SDL_MouseButtonEvent{
               .type = SDL_MOUSEBUTTONDOWN,
               .timestamp = {},
               .windowID = {},
               .which = {},
               .button = value.source.button,
               .state = SDL_PRESSED,
               .clicks = {},
               .padding1 = {},
               .x = value.source.position.x,
               .y = value.source.position.y
            }
         };
         // clang-format on
      }

      static SDL_Event transform_event(released<mouse_button> const& value) noexcept
      {
         // clang-format off
         return SDL_Event{ 
            .button = SDL_MouseButtonEvent{
               .type = SDL_MOUSEBUTTONUP,
               .timestamp = {},
               .windowID = {},
               .which = {},
               .button = value.source.button,
               .state = SDL_RELEASED,
               .clicks = {},
               .padding1 = {},
               .x = value.source.position.x,
               .y = value.source.position.y
            }
         };
         // clang-format on
      }

      static auto to_keymod(key const& value) noexcept 
      {
         Uint16 result{0};

         if (value.alt)
            result |= KMOD_ALT;
         if (value.control)
            result |= KMOD_CTRL;
         if (value.system)
            result |= KMOD_GUI;
         if (value.shift)
            result |= KMOD_SHIFT;
         
         return result;
      }

      static auto to_scancode(key const& value) noexcept
      {
         switch (value.code)
         {
            case Keycode::A:
               return SDL_SCANCODE_A;
            case Keycode::B:
               return SDL_SCANCODE_B;
            case Keycode::C:
               return SDL_SCANCODE_C;
            case Keycode::D:
               return SDL_SCANCODE_D;
            case Keycode::E:
               return SDL_SCANCODE_E;
            case Keycode::F:
               return SDL_SCANCODE_F;
            case Keycode::G:
               return SDL_SCANCODE_G;
            case Keycode::H:
               return SDL_SCANCODE_H;
            case Keycode::I:
               return SDL_SCANCODE_I;
            case Keycode::J:
               return SDL_SCANCODE_J;
            case Keycode::K:
               return SDL_SCANCODE_K;
            case Keycode::L:
               return SDL_SCANCODE_L;
            case Keycode::M:
               return SDL_SCANCODE_M;
            case Keycode::N:
               return SDL_SCANCODE_N;
            case Keycode::O:
               return SDL_SCANCODE_O;
            case Keycode::P:
               return SDL_SCANCODE_P;
            case Keycode::Q:
               return SDL_SCANCODE_Q;
            case Keycode::R:
               return SDL_SCANCODE_R;
            case Keycode::S:
               return SDL_SCANCODE_S;
            case Keycode::T:
               return SDL_SCANCODE_T;
            case Keycode::U:
               return SDL_SCANCODE_U;
            case Keycode::V:
               return SDL_SCANCODE_V;
            case Keycode::W:
               return SDL_SCANCODE_W;
            case Keycode::X:
               return SDL_SCANCODE_X;
            case Keycode::Y:
               return SDL_SCANCODE_Y;
            case Keycode::Z:
               return SDL_SCANCODE_Z;
            case Keycode::Num0:
               return SDL_SCANCODE_0;
            case Keycode::Num1:
               return SDL_SCANCODE_1;
            case Keycode::Num2:
               return SDL_SCANCODE_2;
            case Keycode::Num3:
               return SDL_SCANCODE_3;
            case Keycode::Num4:
               return SDL_SCANCODE_4;
            case Keycode::Num5:
               return SDL_SCANCODE_5;
            case Keycode::Num6:
               return SDL_SCANCODE_6;
            case Keycode::Num7:
               return SDL_SCANCODE_7;
            case Keycode::Num8:
               return SDL_SCANCODE_8;
            case Keycode::Num9:
               return SDL_SCANCODE_9;
            case Keycode::Escape:
               return SDL_SCANCODE_ESCAPE;
            case Keycode::LeftBracket:
               return SDL_SCANCODE_LEFTBRACKET;
            case Keycode::RightBracket:
               return SDL_SCANCODE_RIGHTBRACKET;
            case Keycode::Semicolon:
               return SDL_SCANCODE_SEMICOLON;
            case Keycode::Comma:
               return SDL_SCANCODE_COMMA;
            case Keycode::Period:
               return SDL_SCANCODE_PERIOD;
            case Keycode::Quote:
               return SDL_SCANCODE_APOSTROPHE;
            case Keycode::Slash:
               return SDL_SCANCODE_SLASH;
            case Keycode::Backslash:
               return SDL_SCANCODE_BACKSLASH;
            // case Keycode::Tilde:
            case Keycode::Equal:
               return SDL_SCANCODE_EQUALS;
            // case Keycode::Hyphen:
            case Keycode::Space:
               return SDL_SCANCODE_SPACE;
            case Keycode::Enter:
               return SDL_SCANCODE_RETURN;
            case Keycode::Backspace:
               return SDL_SCANCODE_BACKSPACE;
            case Keycode::Tab:
               return SDL_SCANCODE_TAB;
            case Keycode::Pause:
               return SDL_SCANCODE_PAUSE; 
            case Keycode::PageUp:
               return SDL_SCANCODE_PAGEUP;
            case Keycode::PageDown:
               return SDL_SCANCODE_PAGEDOWN;
            case Keycode::End:
               return SDL_SCANCODE_END;
            case Keycode::Home:
               return SDL_SCANCODE_HOME;
            case Keycode::Insert:
               return SDL_SCANCODE_INSERT;
            case Keycode::Delete:
               return SDL_SCANCODE_DELETE;
            // case Keycode::Add:
            // case Keycode::Subtract:
            // case Keycode::Multiply:
            // case Keycode::Divide:
            case Keycode::Left:
               return SDL_SCANCODE_LEFT;
            case Keycode::Right:
               return SDL_SCANCODE_RIGHT;
            case Keycode::Up:
               return SDL_SCANCODE_UP;
            case Keycode::Down:
               return SDL_SCANCODE_DOWN;
            case Keycode::Numpad0:
               return SDL_SCANCODE_KP_0;
            case Keycode::Numpad1:
               return SDL_SCANCODE_KP_1;
            case Keycode::Numpad2:
               return SDL_SCANCODE_KP_2;
            case Keycode::Numpad3:
               return SDL_SCANCODE_KP_3;
            case Keycode::Numpad4:
               return SDL_SCANCODE_KP_4;
            case Keycode::Numpad5:
               return SDL_SCANCODE_KP_5;
            case Keycode::Numpad6:
               return SDL_SCANCODE_KP_6;
            case Keycode::Numpad7:
               return SDL_SCANCODE_KP_7;
            case Keycode::Numpad8:
               return SDL_SCANCODE_KP_8;
            case Keycode::Numpad9:
               return SDL_SCANCODE_KP_9;
            case Keycode::F1:
               return SDL_SCANCODE_F1;
            case Keycode::F2:
               return SDL_SCANCODE_F2;
            case Keycode::F3:
               return SDL_SCANCODE_F3;
            case Keycode::F4:
               return SDL_SCANCODE_F4;
            case Keycode::F5:
               return SDL_SCANCODE_F5;
            case Keycode::F6:
               return SDL_SCANCODE_F6;
            case Keycode::F7:
               return SDL_SCANCODE_F7;
            case Keycode::F8:
               return SDL_SCANCODE_F8;
            case Keycode::F9:
               return SDL_SCANCODE_F9;
            case Keycode::F10:
               return SDL_SCANCODE_F10;
            case Keycode::F11:
               return SDL_SCANCODE_F11;
            case Keycode::F12:
               return SDL_SCANCODE_F12;
            default:
               return SDL_SCANCODE_UNKNOWN;
         }
      }

      static auto to_keycode(key const& value) noexcept
      {
         switch (value.code)
         {
            case Keycode::A:
               return SDLK_a;
            case Keycode::B:
               return SDLK_b;
            case Keycode::C:
               return SDLK_c;
            case Keycode::D:
               return SDLK_d;
            case Keycode::E:
               return SDLK_e;
            case Keycode::F:
               return SDLK_f;
            case Keycode::G:
               return SDLK_g;
            case Keycode::H:
               return SDLK_h;
            case Keycode::I:
               return SDLK_i;
            case Keycode::J:
               return SDLK_j;
            case Keycode::K:
               return SDLK_k;
            case Keycode::L:
               return SDLK_l;
            case Keycode::M:
               return SDLK_m;
            case Keycode::N:
               return SDLK_n;
            case Keycode::O:
               return SDLK_o;
            case Keycode::P:
               return SDLK_p;
            case Keycode::Q:
               return SDLK_q;
            case Keycode::R:
               return SDLK_r;
            case Keycode::S:
               return SDLK_s;
            case Keycode::T:
               return SDLK_t;
            case Keycode::U:
               return SDLK_u;
            case Keycode::V:
               return SDLK_v;
            case Keycode::W:
               return SDLK_w;
            case Keycode::X:
               return SDLK_x;
            case Keycode::Y:
               return SDLK_y;
            case Keycode::Z:
               return SDLK_z;
            case Keycode::Num0:
               return SDLK_0;
            case Keycode::Num1:
               return SDLK_1;
            case Keycode::Num2:
               return SDLK_2;
            case Keycode::Num3:
               return SDLK_3;
            case Keycode::Num4:
               return SDLK_4;
            case Keycode::Num5:
               return SDLK_5;
            case Keycode::Num6:
               return SDLK_6;
            case Keycode::Num7:
               return SDLK_7;
            case Keycode::Num8:
               return SDLK_8;
            case Keycode::Num9:
               return SDLK_9;
            case Keycode::Escape:
               return SDLK_ESCAPE;
            case Keycode::LeftBracket:
               return SDLK_LEFTBRACKET;
            case Keycode::RightBracket:
               return SDLK_RIGHTBRACKET;
            case Keycode::Semicolon:
               return SDLK_SEMICOLON;
            case Keycode::Comma:
               return SDLK_COMMA;
            case Keycode::Period:
               return SDLK_PERIOD;
            case Keycode::Quote:
               return SDLK_QUOTE;
            case Keycode::Slash:
               return SDLK_SLASH;
            case Keycode::Backslash:
               return SDLK_BACKSLASH;
            // case Keycode::Tilde:
            case Keycode::Equal:
               return SDLK_EQUALS;
            // case Keycode::Hyphen:
            case Keycode::Space:
               return SDLK_SPACE;
            case Keycode::Enter:
               return SDLK_RETURN;
            case Keycode::Backspace:
               return SDLK_BACKSPACE;
            case Keycode::Tab:
               return SDLK_TAB;
            case Keycode::Pause:
               return SDLK_PAUSE; 
            case Keycode::PageUp:
               return SDLK_PAGEUP;
            case Keycode::PageDown:
               return SDLK_PAGEDOWN;
            case Keycode::End:
               return SDLK_END;
            case Keycode::Home:
               return SDLK_HOME;
            case Keycode::Insert:
               return SDLK_INSERT;
            case Keycode::Delete:
               return SDLK_DELETE;
            case Keycode::Add:
               return SDLK_PLUS;
            case Keycode::Subtract:
               return SDLK_MINUS;
            case Keycode::Multiply:
               return SDLK_ASTERISK;
            case Keycode::Divide:
               return SDLK_SLASH;
            case Keycode::Left:
               return SDLK_LEFT;
            case Keycode::Right:
               return SDLK_RIGHT;
            case Keycode::Up:
               return SDLK_UP;
            case Keycode::Down:
               return SDLK_DOWN;
            case Keycode::Numpad0:
               return SDLK_KP_0;
            case Keycode::Numpad1:
               return SDLK_KP_1;
            case Keycode::Numpad2:
               return SDLK_KP_2;
            case Keycode::Numpad3:
               return SDLK_KP_3;
            case Keycode::Numpad4:
               return SDLK_KP_4;
            case Keycode::Numpad5:
               return SDLK_KP_5;
            case Keycode::Numpad6:
               return SDLK_KP_6;
            case Keycode::Numpad7:
               return SDLK_KP_7;
            case Keycode::Numpad8:
               return SDLK_KP_8;
            case Keycode::Numpad9:
               return SDLK_KP_9;
            case Keycode::F1:
               return SDLK_F1;
            case Keycode::F2:
               return SDLK_F2;
            case Keycode::F3:
               return SDLK_F3;
            case Keycode::F4:
               return SDLK_F4;
            case Keycode::F5:
               return SDLK_F5;
            case Keycode::F6:
               return SDLK_F6;
            case Keycode::F7:
               return SDLK_F7;
            case Keycode::F8:
               return SDLK_F8;
            case Keycode::F9:
               return SDLK_F9;
            case Keycode::F10:
               return SDLK_F10;
            case Keycode::F11:
               return SDLK_F11;
            case Keycode::F12:
               return SDLK_F12;
            default:
               return SDLK_UNKNOWN;
         }
      }

      static SDL_Event transform_event(pressed<key> const& value) noexcept
      {
         // clang-format off
         return SDL_Event{ 
            .key = SDL_KeyboardEvent{
               .type = SDL_KEYDOWN,
               .timestamp = {},
               .windowID = {},
               .state = SDL_PRESSED,
               .repeat = 0,
               .padding2 = {},
               .padding3 = {},
               .keysym = SDL_Keysym{
                  .scancode = to_scancode(value.source),
                  .sym = to_keycode(value.source),
                  .mod = to_keymod(value.source),
                  .unused = {}
               }
            }
         };
         // clang-format on
      }

      static SDL_Event transform_event(released<key> const& value) noexcept
      {
         // clang-format off
         return SDL_Event{ 
            .key = SDL_KeyboardEvent{
               .type = SDL_KEYUP,
               .timestamp = {},
               .windowID = {},
               .state = SDL_RELEASED,
               .repeat = 0,
               .padding2 = {},
               .padding3 = {},
               .keysym = SDL_Keysym{
                  .scancode = to_scancode(value.source),
                  .sym = to_keycode(value.source),
                  .mod = to_keymod(value.source),
                  .unused = {}
               }
            }
         };
         // clang-format on
      }      

      static SDL_Event transform_event(close_window const&) noexcept
      {
         return SDL_Event{ .type = SDL_QUIT };
      }	
   };

}
