#pragma once

#include <nlohmann/json.hpp>

#include <string_view>

namespace sec21::event
{
   enum class Keycode
   {
      Unknown = -1,
      A = 0,        
      B,            
      C,            
      D,            
      E,            
      F,            
      G,            
      H,            
      I,            
      J,            
      K,            
      L,            
      M,            
      N,            
      O,            
      P,            
      Q,            
      R,            
      S,            
      T,            
      U,            
      V,            
      W,            
      X,            
      Y,            
      Z,   
      Num0,        // 0 
      Num1,        // 1 
      Num2,         
      Num3,         
      Num4,         
      Num5,         
      Num6,         
      Num7,         
      Num8,         
      Num9,         
      Escape,       // Escape 
      LeftBracket,  // [ 
      RightBracket, // ] 
      Semicolon,    // ; 
      Comma,        // , 
      Period,       // . 
      Quote,        // ' 
      Slash,        // / 
      Backslash,
      Tilde,        // ~ 
      Equal,        // = 
      Hyphen,       // -  (hyphen)
      Space,        // Space 
      Enter,        // Enter / Return
      Backspace,    // Backspace 
      Tab,          
      Pause,        
      PageUp,   
      PageDown,
      End,          // End key
      Home,         // Home key
      Insert,       // Insert key
      Delete,       // Delete key
      Add,          // + key
      Subtract,     // - key (minus, usually from numpad)
      Multiply,     // * key
      Divide,       // / key
      Left,         // Left arrow
      Right,        // Right arrow
      Up,           // Up arrow
      Down,         // Down arrow
      Numpad0,      // numpad 0 key
      Numpad1,      // numpad 1 key
      Numpad2,      // numpad 2 key
      Numpad3,      // numpad 3 key
      Numpad4,      // numpad 4 key
      Numpad5,      // numpad 5 key
      Numpad6,      // numpad 6 key
      Numpad7,      // numpad 7 key
      Numpad8,      // numpad 8 key
      Numpad9,      // numpad 9 key
      F1,           
      F2,           
      F3,           
      F4,           
      F5,           
      F6,           
      F7,           
      F8,           
      F9,           
      F10,          
      F11,          
      F12,          
      // Keycode count
      KeyCount
   };

   NLOHMANN_JSON_SERIALIZE_ENUM(Keycode, {
      { Keycode::Unknown, nullptr},
      { Keycode::A, "A" },
      { Keycode::B, "B" },
      { Keycode::C, "C" },
      { Keycode::D, "D" },
      { Keycode::E, "E" },
      { Keycode::F, "F" },
      { Keycode::G, "G" },
      { Keycode::H, "H" },
      { Keycode::I, "I" },
      { Keycode::J, "J" },
      { Keycode::K, "K" },
      { Keycode::L, "L" },
      { Keycode::M, "M" },
      { Keycode::N, "N" },
      { Keycode::O, "O" },
      { Keycode::P, "P" },
      { Keycode::Q, "Q" },
      { Keycode::R, "R" },
      { Keycode::S, "S" },
      { Keycode::T, "T" },
      { Keycode::U, "U" },
      { Keycode::V, "V" },
      { Keycode::W, "W" },
      { Keycode::X, "X" },
      { Keycode::Y, "Y" },
      { Keycode::Z, "Z" },
      { Keycode::Num0, "Num0" },
      { Keycode::Num1, "Num1" },
      { Keycode::Num2, "Num2" },
      { Keycode::Num3, "Num3" },
      { Keycode::Num4, "Num4" },
      { Keycode::Num5, "Num5" },
      { Keycode::Num6, "Num6" },
      { Keycode::Num7, "Num7" },
      { Keycode::Num8, "Num8" },
      { Keycode::Num9, "Num9" },
      { Keycode::Escape, "Escape" },
      { Keycode::LeftBracket, "LeftBracket" },
      { Keycode::RightBracket, "RightBracket" },
      { Keycode::Semicolon, "Semicolon" },
      { Keycode::Comma, "Comma" },
      { Keycode::Period, "Period" },
      { Keycode::Quote, "Quote" },
      { Keycode::Slash, "Slash" },
      { Keycode::Backslash, "Backslash" },
      { Keycode::Tilde, "Tilde" },
      { Keycode::Equal, "Equal" },
      { Keycode::Hyphen, "Hyphen" },
      { Keycode::Space, "Space" },
      { Keycode::Enter, "Enter" },
      { Keycode::Backspace, "Backspace" },
      { Keycode::Tab, "Tab" },
      { Keycode::Pause, "Pause" },
      { Keycode::PageUp, "PageUp" },
      { Keycode::PageDown, "PageDown" },
      { Keycode::End, "End" },
      { Keycode::Home, "Home" },
      { Keycode::Insert, "Insert" },
      { Keycode::Delete, "Delete" },
      { Keycode::Add, "Add" },
      { Keycode::Subtract, "Subtract" },
      { Keycode::Multiply, "Multiply" },
      { Keycode::Divide, "Divide" },
      { Keycode::Left, "Left" },
      { Keycode::Right, "Right" },
      { Keycode::Up, "Up" },
      { Keycode::Down, "Down" },
      { Keycode::Numpad0, "Numpad0" },
      { Keycode::Numpad1, "Numpad1" },
      { Keycode::Numpad2, "Numpad2" },
      { Keycode::Numpad3, "Numpad3" },
      { Keycode::Numpad4, "Numpad4" },
      { Keycode::Numpad5, "Numpad5" },
      { Keycode::Numpad6, "Numpad6" },
      { Keycode::Numpad7, "Numpad7" },
      { Keycode::Numpad8, "Numpad8" },
      { Keycode::Numpad9, "Numpad9" },      
      { Keycode::F1, "F1" },
      { Keycode::F2, "F2" },
      { Keycode::F3, "F3" },
      { Keycode::F4, "F4" },
      { Keycode::F5, "F5" },
      { Keycode::F6, "F6" },
      { Keycode::F7, "F7" },
      { Keycode::F8, "F8" },
      { Keycode::F9, "F9" },
      { Keycode::F10, "F10" },
      { Keycode::F11, "F11" },
      { Keycode::F12, "F12" },
   })

   struct key
   {
      static constexpr std::string_view name{ "key" };

      bool alt{ false };
      bool control{ false };
      bool system{ false };
      bool shift{ false };
      Keycode code{ Keycode::Unknown };
   };

   void to_json(nlohmann::json& j, key const& obj)
   {
      j = nlohmann::json{
         { "alt",  obj.alt },
         { "control",  obj.control },
         { "system",  obj.system },
         { "shift",  obj.shift },
         { "keycode",  obj.code },
      };
   }
   void from_json(nlohmann::json const& j, key& obj)
   {
      j.at("alt").get_to(obj.alt);
      j.at("control").get_to(obj.control);
      j.at("system").get_to(obj.system);
      j.at("shift").get_to(obj.shift);
      j.at("keycode").get_to(obj.code);
   }
}
