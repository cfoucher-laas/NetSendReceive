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

#ifndef TCPSENDRECEIVEWIDGET_H
#define TCPSENDRECEIVEWIDGET_H

// Parent class
#include <QTabWidget>

// C++ classes
#include <memory>
using namespace std;

// Qt classes
class QLabel;
class QTcpSocket;

// NSR classes
class DataTableWidget;
class DataTableWithList;


class TcpSendReceiveWidget : public QTabWidget
{
    Q_OBJECT

public:
	explicit TcpSendReceiveWidget(const QString& logHeader, QWidget* parent = nullptr);
    ~TcpSendReceiveWidget();

    void retranslateUi();

    void setSocket(QTcpSocket *newSocket);
    void closeConnection();

signals:
    void disconnected();

private slots:
    void messageReceived();
    void sendData(shared_ptr<QByteArray> data);

private:
    void updateTabs();
    void socketClosed();
    void clearSocket();

private:
    QTcpSocket* socket; // Can't use shared_ptr as we use deleteLater...

	QString logHeader;

    DataTableWidget*   sendWidget;
    DataTableWithList* receiveWidget;
    QLabel*            messageWidget;

    QWidget* sendTabContentWidget;
    QWidget* receiveTabContentWidget;
};

#endif // TCPSENDRECEIVEWIDGET_H
