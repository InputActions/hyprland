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

#include "HyprlandWindow.h"
#include <hyprland/src/desktop/view/Window.hpp>
#include <hyprland/src/managers/fullscreen/FullscreenController.hpp>

namespace InputActions
{

HyprlandWindow::HyprlandWindow(PHLWINDOW window)
    : m_window(std::move(window))
{
}

void HyprlandWindow::activate()
{
    m_window->activate(true);
}

std::optional<QString> HyprlandWindow::id()
{
    return idToString(m_window.get());
}

std::optional<pid_t> HyprlandWindow::pid()
{
    return m_window->getPID();
}

std::optional<QRectF> HyprlandWindow::geometry()
{
    const auto position = m_window->position(Desktop::View::IGeometric::GEOMETRIC_GOAL);
    const auto size = m_window->size(Desktop::View::IGeometric::GEOMETRIC_GOAL);
    return QRectF(position.x, position.y, size.x, size.y);
}

std::optional<QString> HyprlandWindow::title()
{
    return QString::fromStdString(m_window->m_title);
}

std::optional<QString> HyprlandWindow::resourceClass()
{
    return QString::fromStdString(m_window->m_class);
}

std::optional<bool> HyprlandWindow::fullscreen()
{
    return Fullscreen::controller()->isFullscreen(m_window);
}

QString HyprlandWindow::idToString(const Desktop::View::CWindow *window)
{
    return QString::number((uintptr_t)window, 16);
}

}