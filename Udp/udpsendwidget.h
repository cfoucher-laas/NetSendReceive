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

#ifndef UDPSENDWIDGET_H
#define UDPSENDWIDGET_H

// Current class parent
#include <QWidget>

// C++ classes
#include <memory>
using namespace std;

// Qt classes
class QLabel;
class QLineEdit;
class QCheckBox;

// NSR classes
class DataTableWidget;


class UdpSendWidget : public QWidget
{
	Q_OBJECT

public:
	explicit UdpSendWidget(QWidget* parent = nullptr);

	void retranslateUi();

private slots:
	void sendData(shared_ptr<QByteArray> data);
	void setSpecifyLocalPort(bool doSpecify);

private:
	QLabel*    labelConfig;
	QLabel*    labelRemoteAddress;
	QLineEdit* textRemoteAddress;
	QLabel*    labelRemotePort;
	QLineEdit* textRemotePort;
	QLabel*    labelLocalPort;
	QLineEdit* textLocalPort;
	QLabel*    labelMessageContent;

	QCheckBox* checkBoxSpecifyLocalPort;

	DataTableWidget* messageContentEditor;
};

#endif // UDPSENDWIDGET_H
