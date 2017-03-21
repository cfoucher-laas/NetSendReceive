/*
 * Copyright © 2017 LAAS/CNRS
 * Author: Clément Foucher
 *
 * Distributed under the GNU GPL v2. For full terms see the file LICENSE.txt.
 *
 *
 * This file is part of Network Send / Receive (NSR).
 *
 * NSR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * NSR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with NSR. If not, see <http://www.gnu.org/licenses/>.
 */

// Current class header
#include "tcpwidget.h"

// Qt cleasses
#include <QVBoxLayout>

// NSR classes
#include "tcpclientwidget.h"
#include "tcpserverwidget.h"


TcpWidget::TcpWidget(QWidget* parent) :
    QTabWidget(parent)
{
    // Tab client
    this->clientTab = new TcpClientWidget();
    QWidget* clientContentWidget = new QWidget();
    QVBoxLayout* clientLayout = new QVBoxLayout(clientContentWidget);
    clientLayout->setContentsMargins(10, 10, 10, 10);
    clientLayout->addWidget(this->clientTab);
    this->addTab(clientContentWidget, QString());

    // Tab server
    this->serverTab = new TcpServerWidget();
    QWidget* serverContentWidget = new QWidget();
    QVBoxLayout* serverLayout = new QVBoxLayout(serverContentWidget);
    serverLayout->addWidget(this->serverTab);
    serverLayout->setContentsMargins(10, 10, 10, 10);
    this->addTab(serverContentWidget, QString());

    this->setCurrentIndex(0);

    this->retranslateUi();
}

void TcpWidget::retranslateUi()
{
    this->setTabText(0, tr("Client"));
    this->setTabText(1, tr("Server"));

    this->clientTab->retranslateUi();
    this->serverTab->retranslateUi();
}
