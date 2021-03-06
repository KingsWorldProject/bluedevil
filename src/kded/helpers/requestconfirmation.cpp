/***************************************************************************
 *   Copyright (C) 2010 Alejandro Fiestas Olivares <alex@eyeos.org>        *
 *   Copyright (C) 2010 Eduardo Robles Elvira <edulix@gmail.com>           *
 *   Copyright (C) 2010 UFO Coders <info@ufocoders.com>                    *
 *   Copyright (C) 2014-2015 David Rosca <nowrep@gmail.com>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#include "requestconfirmation.h"
#include "debug_p.h"

#include <KNotification>
#include <KLocalizedString>

RequestConfirmation::RequestConfirmation(BluezQt::DevicePtr device, const QString &pin, QObject *parent)
    : QObject(parent)
    , m_device(device)
    , m_pin(pin)
{
    KNotification *notification = new KNotification(QStringLiteral("RequestConfirmation"),
                                                    KNotification::Persistent, this);

    notification->setComponentName(QStringLiteral("bluedevil"));
    notification->setTitle(QStringLiteral("%1 (%2)").arg(m_device->name().toHtmlEscaped(), m_device->address()));
    notification->setText(i18nc("The text is shown in a notification to know if the PIN is correct,"
                                "%1 is the remote bluetooth device and %2 is the pin",
                                "%1 is asking if the PIN is correct: %2", m_device->name().toHtmlEscaped(), m_pin));

    QStringList actions;
    actions.append(i18nc("Notification button to know if the pin is correct or not", "PIN correct"));
    actions.append(i18nc("Notification button to say that the PIN is wrong", "PIN incorrect"));

    notification->setActions(actions);

    connect(notification, &KNotification::action1Activated, this, &RequestConfirmation::pinCorrect);
    connect(notification, &KNotification::action2Activated, this, &RequestConfirmation::pinWrong);
    connect(notification, &KNotification::closed, this, &RequestConfirmation::pinWrong);
    connect(notification, &KNotification::ignored, this, &RequestConfirmation::pinWrong);
    connect(parent, SIGNAL(agentCanceled()), this, SLOT(pinWrong()));

    notification->sendEvent();
}

void RequestConfirmation::pinCorrect()
{
    qCDebug(BLUEDAEMON) << "PIN correct:" << m_device->name() << m_device->address();

    deleteLater();
    Q_EMIT done(Accept);
}

void RequestConfirmation::pinWrong()
{
    qCDebug(BLUEDAEMON) << "PIN wrong:" << m_device->name() << m_device->address();

    deleteLater();
    Q_EMIT done(Deny);
}
