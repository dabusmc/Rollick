#include "Arch/i386/Input/PS2Scancode.h"

namespace Kernel
{
    namespace
    {
        bool DecodeBase(uint8_t scancode, KeyCode& key)
        {
            switch (scancode)
            {
            case 0x01: key = KeyCode::Escape; break;
            case 0x02: key = KeyCode::Num1; break;
            case 0x03: key = KeyCode::Num2; break;
            case 0x04: key = KeyCode::Num3; break;
            case 0x05: key = KeyCode::Num4; break;
            case 0x06: key = KeyCode::Num5; break;
            case 0x07: key = KeyCode::Num6; break;
            case 0x08: key = KeyCode::Num7; break;
            case 0x09: key = KeyCode::Num8; break;
            case 0x0A: key = KeyCode::Num9; break;
            case 0x0B: key = KeyCode::Num0; break;
            case 0x0C: key = KeyCode::Minus; break;
            case 0x0D: key = KeyCode::Equals; break;
            case 0x0E: key = KeyCode::Backspace; break;
            case 0x0F: key = KeyCode::Tab; break;

            case 0x10: key = KeyCode::Q; break;
            case 0x11: key = KeyCode::W; break;
            case 0x12: key = KeyCode::E; break;
            case 0x13: key = KeyCode::R; break;
            case 0x14: key = KeyCode::T; break;
            case 0x15: key = KeyCode::Y; break;
            case 0x16: key = KeyCode::U; break;
            case 0x17: key = KeyCode::I; break;
            case 0x18: key = KeyCode::O; break;
            case 0x19: key = KeyCode::P; break;
            case 0x1A: key = KeyCode::LeftBracket; break;
            case 0x1B: key = KeyCode::RightBracket; break;
            case 0x1C: key = KeyCode::Enter; break;
            case 0x1D: key = KeyCode::LeftCtrl; break;

            case 0x1E: key = KeyCode::A; break;
            case 0x1F: key = KeyCode::S; break;
            case 0x20: key = KeyCode::D; break;
            case 0x21: key = KeyCode::F; break;
            case 0x22: key = KeyCode::G; break;
            case 0x23: key = KeyCode::H; break;
            case 0x24: key = KeyCode::J; break;
            case 0x25: key = KeyCode::K; break;
            case 0x26: key = KeyCode::L; break;
            case 0x27: key = KeyCode::Semicolon; break;
            case 0x28: key = KeyCode::Apostrophe; break;
            case 0x29: key = KeyCode::Grave; break;
            case 0x2A: key = KeyCode::LeftShift; break;
            case 0x2B: key = KeyCode::Backslash; break;

            case 0x2C: key = KeyCode::Z; break;
            case 0x2D: key = KeyCode::X; break;
            case 0x2E: key = KeyCode::C; break;
            case 0x2F: key = KeyCode::V; break;
            case 0x30: key = KeyCode::B; break;
            case 0x31: key = KeyCode::N; break;
            case 0x32: key = KeyCode::M; break;
            case 0x33: key = KeyCode::Comma; break;
            case 0x34: key = KeyCode::Period; break;
            case 0x35: key = KeyCode::Slash; break;
            case 0x36: key = KeyCode::RightShift; break;
            case 0x37: key = KeyCode::KeypadMultiply; break;
            case 0x38: key = KeyCode::LeftAlt; break;
            case 0x39: key = KeyCode::Space; break;
            case 0x3A: key = KeyCode::CapsLock; break;

            case 0x3B: key = KeyCode::F1; break;
            case 0x3C: key = KeyCode::F2; break;
            case 0x3D: key = KeyCode::F3; break;
            case 0x3E: key = KeyCode::F4; break;
            case 0x3F: key = KeyCode::F5; break;
            case 0x40: key = KeyCode::F6; break;
            case 0x41: key = KeyCode::F7; break;
            case 0x42: key = KeyCode::F8; break;
            case 0x43: key = KeyCode::F9; break;
            case 0x44: key = KeyCode::F10; break;
            case 0x45: key = KeyCode::NumLock; break;
            case 0x46: key = KeyCode::ScrollLock; break;

            case 0x47: key = KeyCode::Keypad7; break;
            case 0x48: key = KeyCode::Keypad8; break;
            case 0x49: key = KeyCode::Keypad9; break;
            case 0x4A: key = KeyCode::KeypadMinus; break;
            case 0x4B: key = KeyCode::Keypad4; break;
            case 0x4C: key = KeyCode::Keypad5; break;
            case 0x4D: key = KeyCode::Keypad6; break;
            case 0x4E: key = KeyCode::KeypadPlus; break;
            case 0x4F: key = KeyCode::Keypad1; break;
            case 0x50: key = KeyCode::Keypad2; break;
            case 0x51: key = KeyCode::Keypad3; break;
            case 0x52: key = KeyCode::Keypad0; break;
            case 0x53: key = KeyCode::KeypadDecimal; break;

            case 0x56: key = KeyCode::NonUSBackslash; break;
            case 0x57: key = KeyCode::F11; break;
            case 0x58: key = KeyCode::F12; break;

            default:
                return false;
            }

            return true;
        }

        bool DecodeExtended(uint8_t scancode, KeyCode& key)
        {
            switch (scancode)
            {
            case 0x1C: key = KeyCode::KeypadEnter; break;
            case 0x1D: key = KeyCode::RightCtrl; break;
            case 0x35: key = KeyCode::KeypadDivide; break;
            case 0x38: key = KeyCode::RightAlt; break;

            case 0x47: key = KeyCode::Home; break;
            case 0x48: key = KeyCode::ArrowUp; break;
            case 0x49: key = KeyCode::PageUp; break;
            case 0x4B: key = KeyCode::ArrowLeft; break;
            case 0x4D: key = KeyCode::ArrowRight; break;
            case 0x4F: key = KeyCode::End; break;
            case 0x50: key = KeyCode::ArrowDown; break;
            case 0x51: key = KeyCode::PageDown; break;
            case 0x52: key = KeyCode::Insert; break;
            case 0x53: key = KeyCode::Delete; break;

            case 0x5B: key = KeyCode::LeftSuper; break;
            case 0x5C: key = KeyCode::RightSuper; break;
            case 0x5D: key = KeyCode::Menu; break;

            default:
                return false;
            }

            return true;
        }
    }

    bool PS2Scancode::Decode(uint8_t scancode, KeyEvent& event)
    {
        if (scancode == 0xE0)
        {
            m_Extended = true;
            return false;
        }

        bool released = (scancode & 0x80) != 0;
        scancode &= 0x7F;

        KeyCode key;

        bool valid = m_Extended
            ? DecodeExtended(scancode, key)
            : DecodeBase(scancode, key);

        m_Extended = false;

        if (!valid)
            return false;

        event = {
            key,
            released ? KeyState::Released : KeyState::Pressed
        };

        return true;
    }
}