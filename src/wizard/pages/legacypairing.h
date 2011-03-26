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


#ifndef LEGACYPAIRING_H
#define LEGACYPAIRING_H

#include "ui_legacypairing.h"
#include <QWizardPage>

class BlueWizard;
class KPixmapSequenceOverlayPainter;

namespace BlueDevil {
    class Device;
    class Adapter;
}

using namespace BlueDevil;

class LegacyPairingPage : public QWizardPage
, Ui::LegacyPairing
{
Q_OBJECT

public:
    LegacyPairingPage(BlueWizard* parent = 0);

    virtual void initializePage();
    virtual bool isComplete() const;
    virtual int nextId() const;

public Q_SLOTS:
    void registered(Device* device);
    void pinRequested(const QString &pin);
    void pairedChanged(bool paired);

private:
    bool                           m_triedToPair;
    bool                           m_paired;
    BlueWizard                    *m_wizard;
    KPixmapSequenceOverlayPainter *m_working;
};

#endif // LEGACYPAIRING_H