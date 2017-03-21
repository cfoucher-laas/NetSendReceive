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
#include "udpsendwidget.h"

// Qt classes
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QUdpSocket>
#include <QCheckBox>

// NSR classes
#include "networkconfig.h"
#include "datatablewidget.h"
#include "logwidget.h"


UdpSendWidget::UdpSendWidget(QWidget* parent) :
    QWidget(parent)
{
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Config

    // Line 1

    this->labelConfig = new QLabel(this);
    gridLayout->addWidget(this->labelConfig, 0, 0, 1, -1, Qt::AlignCenter);

    // Line 2

    this->labelRemoteAddress = new QLabel(this);
    gridLayout->addWidget(this->labelRemoteAddress, 1, 0, 1, 1);

    this->textRemoteAddress = new QLineEdit(this);
    this->textRemoteAddress->setText(DEFAULT_IP_ADDR);
    gridLayout->addWidget(this->textRemoteAddress, 1, 1, 1, 1);

    this->labelRemotePort = new QLabel(this);
    gridLayout->addWidget(this->labelRemotePort, 1, 2, 1, 1);

    this->textRemotePort = new QLineEdit(this);
    this->textRemotePort->setText(DEFAULT_PORT);
    QRegularExpression portRegExp("[0-9]{0,10}");
    this->textRemotePort->setValidator(new QRegularExpressionValidator(portRegExp, 0));
    gridLayout->addWidget(this->textRemotePort, 1, 3, 1, 1);

    // Line 3

    this->checkBoxSpecifyLocalPort = new QCheckBox(this);
    gridLayout->addWidget(this->checkBoxSpecifyLocalPort, 2, 0, 1, 2);

    this->labelLocalPort = new QLabel(this);
    this->labelLocalPort->setVisible(false);
    gridLayout->addWidget(this->labelLocalPort, 2, 2, 1, 1);

    this->textLocalPort= new QLineEdit(this);
    this->textLocalPort->setText(DEFAULT_PORT);
    QRegularExpression portRegExp2("[0-9]{0,10}");
    this->textLocalPort->setValidator(new QRegularExpressionValidator(portRegExp2, 0));
    this->textLocalPort->setVisible(false);
    gridLayout->addWidget(this->textLocalPort, 2, 3, 1, 1);

    // Message

    this->labelMessageContent = new QLabel(this);
    gridLayout->addWidget(this->labelMessageContent, 3, 0, 1, -1, Qt::AlignCenter);

    this->messageContentEditor = new DataTableWidget(true, true, true, this);
    gridLayout->addWidget(this->messageContentEditor, 4, 0, 1, -1);


    this->retranslateUi();

    connect(this->messageContentEditor,     &DataTableWidget::sendRequested, this, &UdpSendWidget::sendData);
    connect(this->checkBoxSpecifyLocalPort, &QCheckBox::toggled,             this, &UdpSendWidget::setSpecifyLocalPort);
}

void UdpSendWidget::sendData(shared_ptr<QByteArray> data)
{
    bool ok;
    bool proceed = true;
	int localPort = 0;

    int remotePort = this->textRemotePort->text().toInt(&ok);

    if (ok == true)
    {
        this->labelRemotePort->setStyleSheet("");
        this->textRemotePort->setStyleSheet("");
    }
    else
    {
        this->labelRemotePort->setStyleSheet("color:red;");
        this->textRemotePort->setStyleSheet("color:red;");
        proceed = false;
    }

    QHostAddress address;
    ok = address.setAddress(this->textRemoteAddress->text());

    if (ok == true)
    {
        this->labelRemoteAddress->setStyleSheet("");
        this->textRemoteAddress->setStyleSheet("");
    }
    else
    {
        this->labelRemoteAddress->setStyleSheet("color:red;");
        this->textRemoteAddress->setStyleSheet("color:red;");
        proceed = false;
    }

    if (this->checkBoxSpecifyLocalPort->isChecked())
    {
        localPort = this->textLocalPort->text().toInt(&ok);

        if (ok == true)
        {
            this->labelLocalPort->setStyleSheet("");
            this->textLocalPort->setStyleSheet("");
        }
        else
        {
            this->labelLocalPort->setStyleSheet("color:red;");
            this->textLocalPort->setStyleSheet("color:red;");
            proceed = false;
        }
    }

    if (proceed == true)
    {
        this->labelConfig->setStyleSheet("");

        unique_ptr<QUdpSocket> emissionSocket = unique_ptr<QUdpSocket>(new QUdpSocket());
        if (this->checkBoxSpecifyLocalPort->isChecked())
        {
            emissionSocket->bind(localPort);
        }
        ok = emissionSocket->open(QIODevice::ReadWrite);

        if (ok == true)
        {
            qint64 sentBytes = emissionSocket->writeDatagram(*data.get(), address, remotePort);

            if (sentBytes < data->size())
            {
                ok = emissionSocket->waitForBytesWritten(10000);
            }
            else
            {
                ok = true;
            }

            if (ok == true)
            {
				LogWidget::addLogEntry("UDP: " + tr("Message sent."));
            }
            else
            {
				QString errorMessage = tr("Error! Unable to send UDP message.");
				errorMessage += " ";
				errorMessage += QString::number(sentBytes);
				errorMessage += " ";
				if (sentBytes <= 1)
					errorMessage += tr("byte");
				else
					errorMessage += tr("bytes");
				errorMessage += " ";
				errorMessage += tr("sent");
				errorMessage += " ";
				errorMessage += tr("out of");
				errorMessage += " ";
				errorMessage += QString::number(data->size());
				errorMessage += ".";

				LogWidget::addLogEntry(errorMessage);
            }

            emissionSocket->close();
        }
        else
        {
			LogWidget::addLogEntry(tr("Error! Unable to open UDP socket."));
        }
    }
    else
    {
        this->labelConfig->setStyleSheet("color:red;");
		LogWidget::addLogEntry(tr("Error! Incorrect UDP configuration. Message wasn't sent."));
    }
}

void UdpSendWidget::setSpecifyLocalPort(bool doSpecify)
{
    if (doSpecify == true)
    {
        this->labelLocalPort->setVisible(true);
        this->textLocalPort->setVisible(true);
    }
    else
    {
        this->labelLocalPort->setVisible(false);
        this->textLocalPort->setVisible(false);
    }
}

void UdpSendWidget::retranslateUi()
{
    this->labelConfig->setText("<b>" + tr("Configuration") + "</b>");

    this->labelRemoteAddress->setText(tr("Remote IP address"));
    this->labelRemotePort->setText(tr("Remote port"));
    this->labelLocalPort->setText(tr("Local port"));
    this->labelMessageContent->setText("<b>" + tr("Content of message to send") + "</b>");

    this->checkBoxSpecifyLocalPort->setText(tr("Specify local port used to send data"));

    this->messageContentEditor->retranslateUi();
}
