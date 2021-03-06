/*****************************************************************************
 * This file is part of the KDE project                                      *
 *                                                                           *
 * Copyright (C) 2010-2011 Alejandro Fiestas Olivares <afiestas@kde.org>     *
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

#ifndef DISCOVERWIDGET_H
#define DISCOVERWIDGET_H

#include "ui_discover.h"

#include <QWidget>

#include <BluezQt/Types>

class KMessageWidget;

class DevicesProxyModel;

class DiscoverWidget : public QWidget, public Ui::Discover
{
    Q_OBJECT

public:
    explicit DiscoverWidget(BluezQt::Manager *manager, QWidget *parent = nullptr);

Q_SIGNALS:
    void deviceSelected(BluezQt::DevicePtr device);

private Q_SLOTS:
    void indexSelected(const QModelIndex index);

    void checkAdapters();
    void fixAdaptersError();

private:
    BluezQt::Manager *m_manager;
    DevicesProxyModel *m_model;
    KMessageWidget *m_warningWidget;
};

#endif // DISCOVERWIDGET_H
