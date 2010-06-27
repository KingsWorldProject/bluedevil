/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2009 by Alex Fiestas <alex@eyeos.org>                         *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#include "audio.h"
#include "audio_interface.h"

#include <KPluginFactory>
#include <KNotification>
#include <KIcon>
#include <KLocalizedString>

#include <QDBusConnection>
#include <bluedevil/bluedevildevice.h>

BLUEDEVILSERVICE_PLUGIN_EXPORT(AudioPlugin)

AudioPlugin::AudioPlugin(QObject* parent, const QVariantList& args)
    : ServicePlugin(parent)
{}

void AudioPlugin::connectService()
{
    OrgBluezAudioInterface *interface = new OrgBluezAudioInterface("org.bluez", device()->UBI(), QDBusConnection::systemBus());
    interface->Connect();

    QString desc = device()->alias();
    if (device()->alias() != device()->name() && !device()->name().isEmpty()) {
        desc.append(" ("+device()->name()+")");
    }
    desc.append(i18n(" Audio device connected and configured"));

    KNotification::event(
        KNotification::Notification,
        desc,
        KIcon(device()->icon()).pixmap(48,48)
    )->sendEvent();

    emit finished();
}