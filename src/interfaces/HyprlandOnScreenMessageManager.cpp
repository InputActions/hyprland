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

#include "HyprlandOnScreenMessageManager.h"

#ifdef HYPRLAND_0_55_OR_GREATER
#include <hyprland/src/notification/NotificationOverlay.hpp>
#else
#include <hyprland/src/debug/HyprNotificationOverlay.hpp>
#endif

namespace InputActions
{

void HyprlandOnScreenMessageManager::showMessage(const QString &message)
{
    const auto color =
#ifdef HYPRLAND_0_55_OR_GREATER
        Notification::
#endif
            ICONS_COLORS[1];

#ifdef HYPRLAND_0_55_OR_GREATER
    Notification::overlay()
#else
    g_pHyprNotificationOverlay
#endif
        ->addNotification(QString("[InputActions] %1").arg(message).toStdString().c_str(), color, 5000);
}

void HyprlandOnScreenMessageManager::hideMessage()
{
#ifdef HYPRLAND_0_55_OR_GREATER
    Notification::overlay()
#else
    g_pHyprNotificationOverlay
#endif
        ->dismissNotifications(1);
}

}