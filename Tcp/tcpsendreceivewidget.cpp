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
#include "tcpsendreceivewidget.h"

// Qt classes
#include <QLabel>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QHostAddress>
#include <QTime>

// NSR classes
#include "datatablewidget.h"
#include "datatablewithlist.h"
#include "logwidget.h"


TcpSendReceiveWidget::TcpSendReceiveWidget(const QString& logHeader, QWidget* parent) :
	QTabWidget(parent)
{
	this->socket = nullptr;

	this->logHeader = logHeader + " ";

	this->sendWidget    = new DataTableWidget(true, true, true);
	this->receiveWidget = new DataTableWithList();

	this->messageWidget = new QLabel();
	this->messageWidget->setAlignment(Qt::AlignCenter);

	this->sendTabContentWidget = new QWidget();
	QVBoxLayout* sendTabLayout = new QVBoxLayout(this->sendTabContentWidget);
	sendTabLayout->setContentsMargins(10, 10, 10, 10);
	sendTabLayout->addWidget(this->sendWidget);

	this->receiveTabContentWidget = new QWidget();
	QVBoxLayout* receiveTabLayout = new QVBoxLayout(this->receiveTabContentWidget);
	receiveTabLayout->setContentsMargins(10, 10, 10, 10);
	receiveTabLayout->addWidget(this->receiveWidget);

	this->addTab(this->messageWidget,           QString());
	this->addTab(this->receiveTabContentWidget, QString());

	this->retranslateUi();

	connect(this->sendWidget, &DataTableWidget::sendRequested, this, &TcpSendReceiveWidget::sendData);
}

TcpSendReceiveWidget::~TcpSendReceiveWidget()
{
	this->clearSocket();
}

void TcpSendReceiveWidget::retranslateUi()
{
	this->setTabText(0, tr("Send"));
	this->setTabText(1, tr("Receive"));

	this->messageWidget->setText(tr("Waiting for connection."));

	this->receiveWidget->retranslateUi();
	this->sendWidget->retranslateUi();
}

void TcpSendReceiveWidget::setSocket(QTcpSocket* newSocket)
{
	if (this->socket != nullptr)
	{
		this->clearSocket();

		if (newSocket == nullptr)
		{
			emit disconnected();
		}
	}

	this->socket = newSocket;

	if (this->socket !=  nullptr)
	{
		connect(this->socket, &QTcpSocket::disconnected, this, &TcpSendReceiveWidget::socketClosed);
		connect(this->socket, &QTcpSocket::readyRead,    this, &TcpSendReceiveWidget::messageReceived);
	}

	this->updateTabs();
}

void TcpSendReceiveWidget::closeConnection()
{
	if (this->socket != nullptr)
	{
		this->clearSocket();
		this->updateTabs();
	}
}

void TcpSendReceiveWidget::messageReceived()
{
	shared_ptr<QByteArray> data(new QByteArray(this->socket->readAll()));

	QString messageText;
	messageText  = tr("received") + " @" + QTime::currentTime().toString();
	messageText += " ";
	messageText += tr("from") + " " + this->socket->peerAddress().toString();
	messageText += " ; ";
	messageText += tr("size") + " = " + QString::number(data->size()) + " ";
	if (data->size() == 1)
	{
		messageText += tr("byte");
	}
	else
	{
		messageText += tr("bytes");
	}

	this->receiveWidget->addData(data, messageText);

	LogWidget::addLogEntry(this->logHeader + tr("Message received."));
}

void TcpSendReceiveWidget::sendData(shared_ptr<QByteArray> data)
{
	bool ok;
	qint64 sentBytes = this->socket->write(*data.get(), data->size());

	if (sentBytes < data->size())
	{
		ok = this->socket->waitForBytesWritten(10000);
	}
	else
	{
		ok = true;
	}

	if (ok == true)
	{
		LogWidget::addLogEntry(this->logHeader + tr("Message sent containing") + " " + QString::number(sentBytes) + " " + tr("bytes of data"));
	}
	else
	{
		LogWidget::addLogEntry(this->logHeader + tr("Error! Unable to send data."));
	}
}

void TcpSendReceiveWidget::updateTabs()
{
	int currentTab = this->currentIndex();

	this->removeTab(0);

	if (this->socket == nullptr)
	{
		this->insertTab(0, this->messageWidget, QString());
	}
	else
	{
		this->insertTab(0, this->sendTabContentWidget, QString());
	}

	this->setCurrentIndex(currentTab);
	this->retranslateUi();
}

void TcpSendReceiveWidget::socketClosed()
{
	this->socket->deleteLater(); // Only way to remove it as we are in socket's disconnected event
	this->socket = nullptr;
	this->updateTabs();
	emit disconnected();
}

void TcpSendReceiveWidget::clearSocket()
{
	if (this->socket != nullptr)
	{
		disconnect(this->socket, &QTcpSocket::readyRead,    this, &TcpSendReceiveWidget::messageReceived);
		disconnect(this->socket, &QTcpSocket::disconnected, this, &TcpSendReceiveWidget::socketClosed);
		this->socket->close();
		delete this->socket;
		this->socket = nullptr;
	}
}
