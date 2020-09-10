#pragma once

#include <sec21/event/events.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <tuple>
#include <optional>

namespace sec21::event::backend
{
   class sfml
   {
   public:	
      std::tuple<bool, event_t> poll_event(sf::RenderWindow& window)
      {
         sf::Event evt{};
         if (window.pollEvent(evt)) {
            return { true, to_event(evt) };
         }
         return { false, {} };
      }

      static sf::Time to_time(time_elapsed const& te) noexcept
      {
         return sf::microseconds(duration_cast<std::chrono::microseconds>(te.elapsed).count());
      }

      static std::optional<sf::Event> to_event(event_t const& evt) noexcept
      {
         return std::visit(overloaded{
            [](auto const& value) -> std::optional<sf::Event> { return transform_event(value); },
            [](time_elapsed const&) -> std::optional<sf::Event> { return {}; },
            [](std::monostate const&) -> std::optional<sf::Event> { return {}; },
         }, evt);
      }

   private:
      event_t to_event(sf::Event const& event) const noexcept
      {
         const auto to_keycode = [](sf::Keyboard::Key const& k)
         {
            using namespace sec21;
            switch (k)
            {
               case sf::Keyboard::Key::A:
                  return Keycode::A;
               case sf::Keyboard::Key::B:
                  return Keycode::B;
               case sf::Keyboard::Key::C:
                  return Keycode::C;
               case sf::Keyboard::Key::D:
                  return Keycode::D;
               case sf::Keyboard::Key::E:
                  return Keycode::E;
               case sf::Keyboard::Key::F:
                  return Keycode::F;
               case sf::Keyboard::Key::G:
                  return Keycode::G;
               case sf::Keyboard::Key::H:
                  return Keycode::H;
               case sf::Keyboard::Key::I:
                  return Keycode::I;
               case sf::Keyboard::Key::J:
                  return Keycode::J;
               case sf::Keyboard::Key::K:
                  return Keycode::K;
               case sf::Keyboard::Key::L:
                  return Keycode::L;
               case sf::Keyboard::Key::M:
                  return Keycode::M;
               case sf::Keyboard::Key::N:
                  return Keycode::N;
               case sf::Keyboard::Key::O:
                  return Keycode::O;
               case sf::Keyboard::Key::P:
                  return Keycode::P;
               case sf::Keyboard::Key::Q:
                  return Keycode::Q;
               case sf::Keyboard::Key::R:
                  return Keycode::R;
               case sf::Keyboard::Key::S:
                  return Keycode::S;
               case sf::Keyboard::Key::T:
                  return Keycode::T;
               case sf::Keyboard::Key::U:
                  return Keycode::U;
               case sf::Keyboard::Key::V:
                  return Keycode::V;
               case sf::Keyboard::Key::W:
                  return Keycode::W;
               case sf::Keyboard::Key::X:
                  return Keycode::X;
               case sf::Keyboard::Key::Y:
                  return Keycode::Y;
               case sf::Keyboard::Key::Z:
                  return Keycode::Z;
               case sf::Keyboard::Key::Num0:
                  return Keycode::Num0;
               case sf::Keyboard::Key::Num1:
                  return Keycode::Num1;
               case sf::Keyboard::Key::Num2:
                  return Keycode::Num2;
               case sf::Keyboard::Key::Num3:
                  return Keycode::Num3;
               case sf::Keyboard::Key::Num4:
                  return Keycode::Num4;
               case sf::Keyboard::Key::Num5:
                  return Keycode::Num5;
               case sf::Keyboard::Key::Num6:
                  return Keycode::Num6;
               case sf::Keyboard::Key::Num7:
                  return Keycode::Num7;
               case sf::Keyboard::Key::Num8:
                  return Keycode::Num8;
               case sf::Keyboard::Key::Num9:
                  return Keycode::Num9;
               case sf::Keyboard::Key::Escape:
                  return Keycode::Escape;
               case sf::Keyboard::Key::LBracket:
                  return Keycode::LeftBracket;
               case sf::Keyboard::Key::RBracket:
                  return Keycode::RightBracket;
               case sf::Keyboard::Key::Semicolon:
                  return Keycode::Semicolon;
               case sf::Keyboard::Key::Comma:
                  return Keycode::Comma;
               case sf::Keyboard::Key::Period:
                  return Keycode::Period;
               case sf::Keyboard::Key::Quote:
                  return Keycode::Quote;
               case sf::Keyboard::Key::Slash:
                  return Keycode::Slash;
               case sf::Keyboard::Key::Backslash:
                  return Keycode::Backslash;
               case sf::Keyboard::Key::Tilde:
                  return Keycode::Tilde;
               case sf::Keyboard::Key::Equal:
                  return Keycode::Equal;
               case sf::Keyboard::Key::Hyphen:
                  return Keycode::Hyphen;
               case sf::Keyboard::Key::Space:
                  return Keycode::Space;
               case sf::Keyboard::Key::Enter:
                  return Keycode::Enter;
               case sf::Keyboard::Key::Backspace:
                  return Keycode::Backspace;
               case sf::Keyboard::Key::Tab:
                  return Keycode::Tab;
               case sf::Keyboard::Key::Pause:
                  return Keycode::Pause;
               case sf::Keyboard::Key::PageUp:
                  return Keycode::PageUp;
               case sf::Keyboard::Key::PageDown:
                  return Keycode::PageDown;
               case sf::Keyboard::Key::End:
                  return Keycode::End;
               case sf::Keyboard::Key::Home:
                  return Keycode::Home;
               case sf::Keyboard::Key::Insert:
                  return Keycode::Insert;
               case sf::Keyboard::Key::Delete:
                  return Keycode::Delete;
               case sf::Keyboard::Key::Add:
                  return Keycode::Add;
               case sf::Keyboard::Key::Subtract:
                  return Keycode::Subtract;
               case sf::Keyboard::Key::Multiply:
                  return Keycode::Multiply;
               case sf::Keyboard::Key::Divide:
                  return Keycode::Divide;
               case sf::Keyboard::Key::Left:
                  return Keycode::Left;
               case sf::Keyboard::Key::Right:
                  return Keycode::Right;
               case sf::Keyboard::Key::Up:
                  return Keycode::Up;
               case sf::Keyboard::Key::Down:
                  return Keycode::Down;
               case sf::Keyboard::Key::Numpad0:
                  return Keycode::Numpad0;
               case sf::Keyboard::Key::Numpad1:
                  return Keycode::Numpad1;
               case sf::Keyboard::Key::Numpad2:
                  return Keycode::Numpad2;
               case sf::Keyboard::Key::Numpad3:
                  return Keycode::Numpad3;
               case sf::Keyboard::Key::Numpad4:
                  return Keycode::Numpad4;
               case sf::Keyboard::Key::Numpad5:
                  return Keycode::Numpad5;
               case sf::Keyboard::Key::Numpad6:
                  return Keycode::Numpad6;
               case sf::Keyboard::Key::Numpad7:
                  return Keycode::Numpad7;
               case sf::Keyboard::Key::Numpad8:
                  return Keycode::Numpad8;
               case sf::Keyboard::Key::Numpad9:
                  return Keycode::Numpad9;
               case sf::Keyboard::Key::F1:
                  return Keycode::F1;
               case sf::Keyboard::Key::F2:
                  return Keycode::F2;
               case sf::Keyboard::Key::F3:
                  return Keycode::F3;
               case sf::Keyboard::Key::F4:
                  return Keycode::F4;
               case sf::Keyboard::Key::F5:
                  return Keycode::F5;
               case sf::Keyboard::Key::F6:
                  return Keycode::F6;
               case sf::Keyboard::Key::F7:
                  return Keycode::F7;
               case sf::Keyboard::Key::F8:
                  return Keycode::F8;
               case sf::Keyboard::Key::F9:
                  return Keycode::F9;
               default:
                  return Keycode::Unknown;
            }
         };

         switch (event.type)	
         {
            case sf::Event::Closed:
               return close_window{};
            case sf::Event::MouseButtonPressed:
               return make_event<pressed<mouse_button>>(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
            case sf::Event::MouseButtonReleased:
               return make_event<released<mouse_button>>(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
            // case sf::Event::MouseEntered:
            // case sf::Event::MouseLeft:
            //! \todo 
            // case sf::Event::MouseWheelScrolled:
            case sf::Event::MouseMoved:
               return make_event<moved<mouse>>(event.mouseMove.x, event.mouseMove.y);
            case sf::Event::KeyPressed:
               return make_event<pressed<key>>(event.key.alt, event.key.control, event.key.system, event.key.shift, to_keycode(event.key.code));
            case sf::Event::KeyReleased:				
               return make_event<released<key>>(event.key.alt, event.key.control, event.key.system, event.key.shift, to_keycode(event.key.code));
            default:
               return std::monostate{};
         }
      }

      static sf::Keyboard::Key transform_key(key const& value) noexcept
      {
         //! \todo implement
         return sf::Keyboard::Key::Unknown;
      }

      static sf::Event::KeyEvent transform_event(key const& value) noexcept
      {
         //! \todo consider ALT + F4 -> seams that be wrong
         return { .code = transform_key(value), .alt = value.alt, .control = value.control, .shift = value.shift, .system = value.system };
      }

      static sf::Event::MouseMoveEvent transform_event(mouse const& value) noexcept
      { 
         return { .x = value.x, .y = value.y }; 
      }

      static sf::Event::MouseButtonEvent transform_event(mouse_button const& value) noexcept
      {
         return { .button = static_cast<sf::Mouse::Button>(value.button), .x = value.position.x, .y = value.position.y };
      }

      static sf::Event transform_event(pressed<mouse_button> const& value) noexcept
      {
         return sf::Event{ .type = sf::Event::MouseButtonPressed, .mouseButton = transform_event(value.source) };
      }

      static sf::Event transform_event(released<mouse_button> const& value) noexcept
      {
         return sf::Event{ .type = sf::Event::MouseButtonReleased, .mouseButton = transform_event(value.source) };
      }

      static sf::Event transform_event(moved<mouse> const& value) noexcept
      {
         return sf::Event{ .type = sf::Event::MouseMoved, .mouseMove = transform_event(value.source) };
      }

      static sf::Event transform_event(pressed<key> const& value) noexcept
      {
         return { .type = sf::Event::KeyPressed, .key = transform_event(value.source) };
      }

      static sf::Event transform_event(released<key> const& value) noexcept
      {
         return { .type = sf::Event::KeyReleased, .key = transform_event(value.source) };
      }

      static sf::Event transform_event(close_window const&) noexcept
      {
         return sf::Event{ .type = sf::Event::Closed, .size = {} };
      }
   };
}
