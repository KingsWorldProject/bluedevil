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

#include "keyboardpairing.h"
#include "wizardagent.h"
#include "../bluewizard.h"
#include "debug_p.h"

#include <KIconLoader>
#include <KPixmapSequence>
#include <KPixmapSequenceOverlayPainter>

#include <BluezQt/Device>
#include <BluezQt/Adapter>
#include <BluezQt/PendingCall>

KeyboardPairingPage::KeyboardPairingPage(BlueWizard *parent)
    : QWizardPage(parent)
    , m_wizard(parent)
    , m_success(false)
{
    setupUi(this);

    m_working = new KPixmapSequenceOverlayPainter(this);
    m_working->setSequence(KIconLoader::global()->loadPixmapSequence(QStringLiteral("process-working"), 22));
    m_working->setWidget(pinNumber);
    m_working->start();

    QFont font(pinNumber->font());
    font.setPointSize(42);
    font.setBold(true);
    pinNumber->setFont(font);
}

int KeyboardPairingPage::nextId() const
{
    if (m_success) {
        return BlueWizard::Connect;
    }
    return BlueWizard::Fail;
}

void KeyboardPairingPage::initializePage()
{
    qCDebug(WIZARD) << "Initialize Keyboard Pairing Page";

    m_wizard->setButtonLayout(wizardButtonsLayout());

    connect(m_wizard->agent(), &WizardAgent::pinRequested, this, &KeyboardPairingPage::pinRequested);

    // Adapter must be pairable, otherwise pairing would fail
    BluezQt::PendingCall *call = m_wizard->device()->adapter()->setPairable(true);
    connect(call, &BluezQt::PendingCall::finished, this, &KeyboardPairingPage::setPairableFinished);
}

void KeyboardPairingPage::setPairableFinished(BluezQt::PendingCall *call)
{
    Q_UNUSED(call)

    BluezQt::PendingCall *pairCall = m_wizard->device()->pair();
    connect(pairCall, &BluezQt::PendingCall::finished, this, &KeyboardPairingPage::pairingFinished);
}

void KeyboardPairingPage::pairingFinished(BluezQt::PendingCall *call)
{
    qCDebug(WIZARD) << "Keyboard Pairing finished:";
    qCDebug(WIZARD) << "\t error     : " << (bool) call->error();
    qCDebug(WIZARD) << "\t errorText : " << call->errorText();

    m_success = !call->error();
    wizard()->next();
}

void KeyboardPairingPage::pinRequested(const QString& pin)
{
    m_working->stop();
    pinNumber->setText(pin);
}

QList<QWizard::WizardButton> KeyboardPairingPage::wizardButtonsLayout() const
{
    QList <QWizard::WizardButton> list;
    list << QWizard::Stretch;
    list << QWizard::CancelButton;
    return list;
}
