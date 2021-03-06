/*****************************************************************************
 * This file is part of the KDE project                                      *
 *                                                                           *
 * Copyright (C) 2010 Alejandro Fiestas Olivares <afiestas@kde.org>          *
 * Copyright (C) 2010-2011 UFO Coders <info@ufocoders.com>                   *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>                         *
 *                                                                           *
 * This library is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU Library General Public               *
 * License as published by the Free Software Foundation; either              *
 * version 2 of the License, or (at your option) any later version.          *
 *                                                                           *
 * This library is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public License *
 * along with this library; see the file COPYING.LIB.  If not, write to      *
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 * Boston, MA 02110-1301, USA.                                               *
 *****************************************************************************/

#include "success.h"
#include "../bluewizard.h"
#include "debug_p.h"

#include <QIcon>

#include <BluezQt/Device>

#include <KNotification>

SuccessPage::SuccessPage(BlueWizard *parent)
    : QWizardPage(parent)
    , m_wizard(parent)
{

}

int SuccessPage::nextId() const
{
    return -1;
}

void SuccessPage::initializePage()
{
    qCDebug(WIZARD) << "Sending Success notification";

    BluezQt::DevicePtr device = m_wizard->device();

    KNotification *notification = new KNotification(QStringLiteral("SetupFinished"),
                                                    KNotification::CloseOnTimeout, this);
    notification->setComponentName(QStringLiteral("bluedevil"));
    notification->setTitle(i18n("Setup Finished"));
    if (device->name().isEmpty()) {
        notification->setText(i18n("The device has been set up and can now be used."));
    } else {
        notification->setText(i18nc("Placeholder is device name",
                                    "The device '%1' has been set up and can now be used.", device->name()));
    }
    // Mark as response to explicit user action ("pairing the device")
    notification->setHint(QStringLiteral("x-kde-user-action-feedback"), true);
    notification->sendEvent();

    setFinalPage(true);
}
