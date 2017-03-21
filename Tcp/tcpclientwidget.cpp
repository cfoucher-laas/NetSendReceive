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
#include "tcpclientwidget.h"

// Qt classes
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTabWidget>
#include <QCheckBox>
#include <QTcpSocket>
#include <QHostAddress>

// NSR classes
#include "networkconfig.h"
#include "tcpsendreceivewidget.h"
#include "logwidget.h"


TcpClientWidget::TcpClientWidget(QWidget* parent) :
    QWidget(parent)
{
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Configuration section

    // Line 1

    this->labelConfig = new QLabel(this);
    gridLayout->addWidget(this->labelConfig, 0, 0, 1, -1, Qt::AlignCenter);

    // Line 2
    this->labelAddress = new QLabel(this);
    gridLayout->addWidget(this->labelAddress, 1, 0, 1, 1);

    this->textAddress = new QLineEdit(this);
    this->textAddress->setText(DEFAULT_IP_ADDR);
    gridLayout->addWidget(this->textAddress, 1, 1, 1, 1);

    this->labelPort = new QLabel(this);
    gridLayout->addWidget(this->labelPort, 1, 2, 1, 1);

    this->textPort = new QLineEdit(this);
    this->textPort->setText(DEFAULT_PORT);
    QRegularExpression portRegExp("[0-9]{0,10}");
    this->textPort->setValidator(new QRegularExpressionValidator(portRegExp, 0));
    gridLayout->addWidget(this->textPort, 1, 3, 1, 1);

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

    // Cross line

    this->buttonConnect = new QPushButton(this);
    this->buttonConnect->setCheckable(true);
    gridLayout->addWidget(this->buttonConnect, 1, 4, 2, 1);


    // Tabs

	this->sendReceiveWidget = new TcpSendReceiveWidget(tr("TCP client:"), this);
    gridLayout->addWidget(this->sendReceiveWidget, 3, 0, 1, -1);

    this->retranslateUi();

    connect(this->buttonConnect,            &QPushButton::clicked,               this, &TcpClientWidget::buttonConnectToggled);
    connect(this->checkBoxSpecifyLocalPort, &QCheckBox::toggled,                 this, &TcpClientWidget::setSpecifyLocalPort);
    connect(this->sendReceiveWidget,        &TcpSendReceiveWidget::disconnected, this, &TcpClientWidget::disconnected);
}

TcpClientWidget::~TcpClientWidget()
{
    this->stopConnection();
}

void TcpClientWidget::retranslateUi()
{
    this->labelConfig->setText("<b>" + tr("Configuration") + "</b>");
    this->labelAddress->setText(tr("Server IP address"));
    this->labelPort->setText(tr("Server port"));

    if (this->buttonConnect->isChecked() == false)
        this->buttonConnect->setText(tr("Connect to server"));
    else
        this->buttonConnect->setText(tr("Disconnect from server"));

    this->checkBoxSpecifyLocalPort->setText(tr("Use a different port for client"));
    this->labelLocalPort->setText(tr("Client port"));

    this->sendReceiveWidget->retranslateUi();
}

void TcpClientWidget::buttonConnectToggled(bool checked)
{
    this->labelConfig->setStyleSheet("");

    if (checked == true)
    {
        bool ok = this->startConnection();

        if (ok == true)
        {
			this->buttonConnect->setText(tr("Disconnect from server"));

            this->textAddress->setEnabled(false);
            this->textPort->setEnabled(false);
            this->checkBoxSpecifyLocalPort->setEnabled(false);
            this->textLocalPort->setEnabled(false);
        }
        else
        {
            this->buttonConnect->setChecked(false);
        }
    }
    else
    {
        this->stopConnection();

        this->buttonConnect->setText(tr("Connect to server"));

        this->textAddress->setEnabled(true);
        this->textPort->setEnabled(true);
        this->checkBoxSpecifyLocalPort->setEnabled(true);
        this->textLocalPort->setEnabled(true);

		LogWidget::addLogEntry(tr("TCP client:") + " " + tr("Disconnected from server."));
    }
}

void TcpClientWidget::setSpecifyLocalPort(bool doSpecify)
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

void TcpClientWidget::disconnected()
{
    this->buttonConnect->setChecked(false);
    this->buttonConnect->setText(tr("Connect to server"));

    this->textAddress->setEnabled(true);
    this->textPort->setEnabled(true);
    this->checkBoxSpecifyLocalPort->setEnabled(true);
    this->textLocalPort->setEnabled(true);

	LogWidget::addLogEntry(tr("TCP client:") + " " + tr("Disconnected by remote server."));
}

bool TcpClientWidget::startConnection()
{
    bool ok;
    bool proceed = true;
	int localPort = 0;

    int port = this->textPort->text().toInt(&ok);

    if (ok == true)
    {
        this->textPort->setStyleSheet("");
    }
    else
    {
        this->textPort->setStyleSheet("color:red;");
        proceed = false;
    }

    QHostAddress address;
    ok = address.setAddress(this->textAddress->text());

    if (ok == true)
    {
        this->textAddress->setStyleSheet("");
    }
    else
    {
        this->textAddress->setStyleSheet("color:red;");
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

        QTcpSocket* socket = new QTcpSocket();

        if (this->checkBoxSpecifyLocalPort->isChecked())
        {
            socket->bind(localPort);
        }

        socket->connectToHost(address, port, QIODevice::ReadWrite);

        ok = socket->waitForConnected(1000);

        if (ok == true)
        {
			LogWidget::addLogEntry(tr("TCP client:") + " " + tr("Connected to server"));
            this->sendReceiveWidget->setSocket(socket);
        }
        else
        {
			LogWidget::addLogEntry(tr("TCP client:") + " " + tr("Unable to connect to server"));
        }
    }
    else
    {
        this->labelConfig->setStyleSheet("color:red;");
		LogWidget::addLogEntry(tr("TCP client:") + " " + tr("Configuration error.") + " " + tr("Cancelled connection atempt."));
    }

    return ok;
}

void TcpClientWidget::stopConnection()
{
    this->sendReceiveWidget->closeConnection();
}
