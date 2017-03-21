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

#ifndef UDPRECEIVEWIDGET_H
#define UDPRECEIVEWIDGET_H

// Current class parent
#include <QWidget>

// C++ classes
#include <memory>
using namespace std;

// Qt classes
class QLabel;
class QLineEdit;
class QPushButton;
class QUdpSocket;

// NSR classes
class DataTableWithList;


class UdpReceiveWidget : public QWidget
{
	Q_OBJECT

public:
	explicit UdpReceiveWidget(QWidget* parent = nullptr);
	~UdpReceiveWidget();

	void retranslateUi();

private slots:
	void setReceive(bool doReceive);
	void messageReceived();

private:
	shared_ptr<QUdpSocket> udpReceptionSocket;

	QLabel*       labelConfig;
	QLabel*       labelPort;
	QLineEdit*    textPort;
	QPushButton*  buttonReceive;

	DataTableWithList* receivedMessagesDisplay;
};

#endif // UDPRECEIVEWIDGET_H
