/*
    Input Actions - Input handler that executes user-defined actions
    Copyright (C) 2024-2026 Marcin Woźniak

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "HyprlandVirtualKeyboard.h"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/managers/KeybindManager.hpp>
#include <hyprland/src/managers/input/InputManager.hpp>
#undef HANDLE
#include <libinputactions/input/backends/InputBackend.h>

namespace InputActions
{

HyprlandVirtualKeyboard::HyprlandVirtualKeyboard()
    : m_device(makeShared<Device>())
{
    g_pInputManager->newKeyboard(m_device);
}

HyprlandVirtualKeyboard::~HyprlandVirtualKeyboard()
{
    reset();
    m_device->events.destroy.emit();
}

void HyprlandVirtualKeyboard::keyboardKey(KeyboardKey key, bool state)
{
    g_inputBackend->setIgnoreEvents(true);

    m_device->events.key.emit(Aquamarine::IKeyboard::SKeyEvent{
        .key = key.scanCode(),
        .pressed = state,
    });
    VirtualKeyboard::keyboardKey(key, state);

    if (const auto modifier = g_pKeybindManager->keycodeToModifier(key.scanCode() + 8)) {
        if (state) {
            m_modifiers |= modifier;
        } else {
            m_modifiers &= ~modifier;
        }
        m_device->events.modifiers.emit(Aquamarine::IKeyboard::SModifiersEvent{
            .depressed = m_modifiers,
        });
    }

    g_inputBackend->setIgnoreEvents(false);
}

const std::string &HyprlandVirtualKeyboard::Device::getName()
{
    static const std::string name = "inputactions-virtual-keyboard";
    return name;
}

}