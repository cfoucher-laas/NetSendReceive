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

#ifndef TCPCLIENTWIDGET_H
#define TCPCLIENTWIDGET_H

// Current class parent
#include <QWidget>

// C++ classes
#include <memory>
using namespace std;

// Qt classes
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;

// NSR classes
class TcpSendReceiveWidget;


class TcpClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClientWidget(QWidget* parent = nullptr);
    ~TcpClientWidget();

    void retranslateUi();

private slots:
    void buttonConnectToggled(bool checked);
    void setSpecifyLocalPort(bool doSpecify);
    void disconnected();

private:
    bool startConnection();
    void stopConnection();

private:
    QLabel*      labelConfig;
    QLabel*      labelAddress;
    QLineEdit*   textAddress;
    QLabel*      labelPort;
    QLineEdit*   textPort;
    QLabel*      labelLocalPort;
    QLineEdit*   textLocalPort;
    QPushButton* buttonConnect;

    TcpSendReceiveWidget* sendReceiveWidget;

    QCheckBox* checkBoxSpecifyLocalPort;
};

#endif // TCPCLIENTWIDGET_H
