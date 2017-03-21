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
#include "udpreceivewidget.h"

// Qt classes
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QUdpSocket>
#include <QTime>

// NSR classes
#include "networkconfig.h"
#include "datatablewithlist.h"
#include "logwidget.h"


UdpReceiveWidget::UdpReceiveWidget(QWidget* parent) :
	QWidget(parent)
{
	QGridLayout* gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(10);
	gridLayout->setContentsMargins(0, 0, 0, 0);

	this->labelConfig = new QLabel(this);
	gridLayout->addWidget(this->labelConfig, 0, 0, 1, -1, Qt::AlignCenter);

	this->labelPort = new QLabel(this);
	gridLayout->addWidget(this->labelPort, 1, 0, 1, 1);

	this->textPort = new QLineEdit(this);
	this->textPort->setText(DEFAULT_PORT);
	QRegularExpression portRegExp("[0-9]{0,10}");
	this->textPort->setValidator(new QRegularExpressionValidator(portRegExp, 0));
	gridLayout->addWidget(this->textPort, 1, 1, 1, 1);

	this->buttonReceive = new QPushButton(this);
	this->buttonReceive->setCheckable(true);
	gridLayout->addWidget(this->buttonReceive, 1, 2, 1, 1);

	this->receivedMessagesDisplay = new DataTableWithList(this);
	gridLayout->addWidget(this->receivedMessagesDisplay, 2, 0, 1, -1);

	this->retranslateUi();

	connect(this->buttonReceive, &QPushButton::clicked, this, &UdpReceiveWidget::setReceive);
}

UdpReceiveWidget::~UdpReceiveWidget()
{
	if (this->udpReceptionSocket != nullptr)
	{
		disconnect(this->udpReceptionSocket.get(), &QUdpSocket::readyRead, this, &UdpReceiveWidget::messageReceived);
		this->udpReceptionSocket->close();
		this->udpReceptionSocket = nullptr;
	}
}

void UdpReceiveWidget::setReceive(bool doReceive)
{
	if (doReceive == true)
	{
		bool ok;

		int port = this->textPort->text().toInt(&ok);

		if (ok == true)
		{
			this->labelConfig->setStyleSheet("");
			this->labelPort->setStyleSheet("");
			this->textPort->setStyleSheet("");

			this->buttonReceive->setText(tr("End listening"));

			this->textPort->setEnabled(false);

			this->udpReceptionSocket = shared_ptr<QUdpSocket>(new QUdpSocket());
			this->udpReceptionSocket->bind(QHostAddress::Any, port);
			this->udpReceptionSocket->open(QIODevice::ReadWrite);
			connect(this->udpReceptionSocket.get(), &QUdpSocket::readyRead, this, &UdpReceiveWidget::messageReceived);

			LogWidget::addLogEntry("UDP: " + tr("Began listening on port") + " " + QString::number(port) + ".");
		}
		else
		{
			this->labelConfig->setStyleSheet("color:red;");
			this->labelPort->setStyleSheet("color:red;");
			this->textPort->setStyleSheet("color:red;");

			// Cancel button checking
			this->buttonReceive->setChecked(false);
		}
	}
	else
	{
		disconnect(this->udpReceptionSocket.get(), &QUdpSocket::readyRead, this, &UdpReceiveWidget::messageReceived);
		this->udpReceptionSocket->close();
		this->udpReceptionSocket = nullptr;

		this->buttonReceive->setText(tr("Begin listening"));
		this->textPort->setEnabled(true);

		LogWidget::addLogEntry("UDP: " + tr("Stopped listening."));
	}
}

void UdpReceiveWidget::messageReceived()
{
	qint64 bytesAvailable = this->udpReceptionSocket->bytesAvailable();

	char data[bytesAvailable];
	QHostAddress senderAddr;
	qint64 readBytes = this->udpReceptionSocket->readDatagram(data, bytesAvailable, &senderAddr);

	QString messageText;
	messageText  = tr("received") + " @" + QTime::currentTime().toString();
	messageText += " ";
	messageText += tr("from") + " " + senderAddr.toString();
	messageText += " ; ";
	messageText += tr("size") + " = " + QString::number(readBytes) + " ";
	if (readBytes == 1)
	{
		messageText += tr("byte");
	}
	else
	{
		messageText += tr("bytes");
	}

	this->receivedMessagesDisplay->addData(shared_ptr<QByteArray>(new QByteArray(data, readBytes)), messageText);

	LogWidget::addLogEntry("UDP: " + tr("Message received."));
}

void UdpReceiveWidget::retranslateUi()
{
	this->labelConfig->setText("<b>" + tr("Configuration") + "</b>");
	this->labelPort->setText(tr("Listen on port"));

	if (this->buttonReceive->isChecked() == true)
		this->buttonReceive->setText(tr("End listening"));
	else
		this->buttonReceive->setText(tr("Begin listening"));

	this->receivedMessagesDisplay->retranslateUi();
}

