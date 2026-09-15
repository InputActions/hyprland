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

#include "HyprlandVirtualMouse.h"
#include <hyprland/src/managers/input/InputManager.hpp>
#undef HANDLE
#include <libinputactions/input/backends/InputBackend.h>

namespace InputActions
{

HyprlandVirtualMouse::HyprlandVirtualMouse()
    : m_device(makeShared<Device>())
{
    g_pInputManager->newMouse(m_device);
}

HyprlandVirtualMouse::~HyprlandVirtualMouse()
{
    reset();
    g_pInputManager->destroyPointer(m_device);
}

void HyprlandVirtualMouse::doMouseButton(MouseButton button, bool state)
{
    m_device->m_pointerEvents.button.emit(IPointer::SButtonEvent{
        .button = button.scanCode(),
        .state = state ? WL_POINTER_BUTTON_STATE_PRESSED : WL_POINTER_BUTTON_STATE_RELEASED,
        .mouse = true,
    });
    m_device->m_pointerEvents.frame.emit();
}

void HyprlandVirtualMouse::doMouseMotion(const PointF &pos)
{
    const Vector2D delta(pos.x(), pos.y());
    m_device->m_pointerEvents.motion.emit(IPointer::SMotionEvent{
        .delta = delta,
        .unaccel = delta,
        .device = m_device,
    });
    m_device->m_pointerEvents.frame.emit();
}

void HyprlandVirtualMouse::doMouseWheel(const PointF &delta)
{
    if (const auto x = delta.x()) {
        m_device->m_pointerEvents.axis.emit(IPointer::SAxisEvent{
            .source = WL_POINTER_AXIS_SOURCE_WHEEL,
            .axis = WL_POINTER_AXIS_HORIZONTAL_SCROLL,
            .delta = x,
            .deltaDiscrete = static_cast<int32_t>(x),
            .mouse = true,
        });
    }
    if (const auto y = delta.y()) {
        m_device->m_pointerEvents.axis.emit(IPointer::SAxisEvent{
            .source = WL_POINTER_AXIS_SOURCE_WHEEL,
            .axis = WL_POINTER_AXIS_VERTICAL_SCROLL,
            .delta = y,
            .deltaDiscrete = static_cast<int32_t>(y),
            .mouse = true,
        });
    }
    m_device->m_pointerEvents.frame.emit();
}

HyprlandVirtualMouse::Device::Device()
{
    m_deviceName = "inputactions-virtual-mouse";
}

}