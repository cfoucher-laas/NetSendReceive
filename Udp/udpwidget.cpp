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
#include "udpwidget.h"

// Qt classes
#include <QVBoxLayout>
#include <QTabWidget>

// NSR classes
#include "udpreceivewidget.h"
#include "udpsendwidget.h"


UdpWidget::UdpWidget(QWidget *parent) :
    QTabWidget(parent)
{
    // Tab send
    this->udpSendTab = new UdpSendWidget();
    QWidget* sendTabContentWidget = new QWidget();
    QVBoxLayout* sendTabLayout = new QVBoxLayout(sendTabContentWidget);
    sendTabLayout->setContentsMargins(10, 10, 10, 10);
    sendTabLayout->addWidget(this->udpSendTab);
    this->addTab(sendTabContentWidget, QString());

    // Tab receive
    this->udpReceiveTab = new UdpReceiveWidget();
    QWidget* receiveTabContentWidget = new QWidget();
    QVBoxLayout* receiveTabLayout = new QVBoxLayout(receiveTabContentWidget);
    receiveTabLayout->setContentsMargins(10, 10, 10, 10);
    receiveTabLayout->addWidget(this->udpReceiveTab);
    this->addTab(receiveTabContentWidget, QString());

    this->setCurrentIndex(0);

    this->retranslateUi();
}

void UdpWidget::retranslateUi()
{
    this->setTabText(0, tr("Send"));
    this->setTabText(1, tr("Receive"));

    this->udpReceiveTab->retranslateUi();
    this->udpSendTab->retranslateUi();
}

