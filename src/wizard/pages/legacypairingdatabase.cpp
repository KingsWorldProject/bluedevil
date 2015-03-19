/*****************************************************************************
 * This file is part of the KDE project                                      *
 *                                                                           *
 * Copyright (C) 2010 Alejandro Fiestas Olivares <afiestas@kde.org>          *
 * Copyright (C) 2010-2011 UFO Coders <info@ufocoders.com>                   *
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

#include "legacypairingdatabase.h"
#include "../bluewizard.h"
#include "../wizardagent.h"
#include "debug_p.h"

#include <KIconLoader>
#include <KLocalizedString>
#include <KPixmapSequence>
#include <KPixmapSequenceOverlayPainter>

#include <BluezQt/Device>
#include <BluezQt/Adapter>
#include <BluezQt/PendingCall>

LegacyPairingPageDatabase::LegacyPairingPageDatabase(BlueWizard *parent)
    : QWizardPage(parent)
    , m_wizard(parent)
    , m_success(false)
{
    setupUi(this);

    KPixmapSequenceOverlayPainter *painter = new KPixmapSequenceOverlayPainter(this);
    painter->setSequence(KIconLoader::global()->loadPixmapSequence(QStringLiteral("process-working"), 22));
    painter->setWidget(working);
    painter->start();
}

int LegacyPairingPageDatabase::nextId() const
{
    if (m_success) {
        return BlueWizard::Connect;
    }
    return BlueWizard::Fail;
}

void LegacyPairingPageDatabase::initializePage()
{
    qCDebug(WIZARD) << "Initialize Legacy Database Pairing Page";

    m_wizard->setButtonLayout(wizardButtonsLayout());

    BluezQt::DevicePtr device = m_wizard->device();
    connecting->setText(i18n("Connecting to %1...", device->name()));

    // Adapter must be pairable, otherwise pairing would fail
    BluezQt::PendingCall *call = device->adapter()->setPairable(true);
    connect(call, &BluezQt::PendingCall::finished, this, &LegacyPairingPageDatabase::setPairableFinished);
}

void LegacyPairingPageDatabase::setPairableFinished(BluezQt::PendingCall *call)
{
    Q_UNUSED(call)

    BluezQt::PendingCall *pairCall = m_wizard->device()->pair();
    connect(pairCall, &BluezQt::PendingCall::finished, this, &LegacyPairingPageDatabase::pairingFinished);
}

void LegacyPairingPageDatabase::pairingFinished(BluezQt::PendingCall *call)
{
    qCDebug(WIZARD) << "Legacy Database Pairing finished:";
    qCDebug(WIZARD) << "\t error     : " << (bool) call->error();
    qCDebug(WIZARD) << "\t errorText : " << call->errorText();

    m_success = !call->error();
    wizard()->next();
}

QList< QWizard::WizardButton > LegacyPairingPageDatabase::wizardButtonsLayout() const
{
    QList <QWizard::WizardButton> list;
    list << QWizard::Stretch;
    list << QWizard::CancelButton;
    return list;
}
