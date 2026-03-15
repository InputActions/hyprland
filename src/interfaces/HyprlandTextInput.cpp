
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

#include "HyprlandTextInput.h"
#include <hyprland/src/desktop/state/FocusState.hpp>
#include <hyprland/src/managers/SessionLockManager.hpp>
#include <hyprland/src/protocols/core/Compositor.hpp>
#undef HANDLE

namespace InputActions
{

HyprlandTextInput::HyprlandTextInput()
{
    // FIXME: Text input list is private, inputs added before the plugin is loaded will not work
    m_newTextInputV3Listener = PROTO::textInputV3->m_events.newTextInput.listen([this](const auto &textInput) {
        onNewTextInputV3(textInput);
    });
}

void HyprlandTextInput::deleteSurroundingText(uint32_t beforeLength, uint32_t afterLength)
{
    if (auto *v3 = currentV3()) {
        v3->deleteSurroundingText(beforeLength, afterLength);
        v3->sendDone();
    }
}

std::optional<QString> HyprlandTextInput::surroundingText()
{
    if (auto *v3 = currentV3()) {
        return QString::fromStdString(v3->m_current.surrounding.text);
    }
    return {};
}

std::optional<uint32_t> HyprlandTextInput::surroundingTextCursorPosition()
{
    if (auto *v3 = currentV3()) {
        return v3->m_current.surrounding.cursor;
    }
    return {};
}

void HyprlandTextInput::writeText(const QString &text)
{
    if (auto *v3 = currentV3()) {
        v3->preeditString({}, 0, 0);
        v3->commitString(text.toStdString());
        v3->sendDone();
    }
}

CTextInputV3 *HyprlandTextInput::currentV3() const
{
    if (!Desktop::focusState()->window()) {
        return {};
    }

    const auto *client = Desktop::focusState()->surface()->client();
    for (const auto &[v3, _] : m_v3TextInputs) {
        if (v3->client() == client && v3->good() && v3->m_current.enabled.value) {
            return v3.get();
        }
    }
    return {};
}

void HyprlandTextInput::onNewTextInputV3(const WP<CTextInputV3> &textInput)
{
    m_v3TextInputs.emplace_back(textInput, textInput->m_events.destroy.listen([this, textInput]() {
        std::erase_if(m_v3TextInputs, [textInput](const auto &input) {
            return input.first == textInput;
        });
    }));
}

}