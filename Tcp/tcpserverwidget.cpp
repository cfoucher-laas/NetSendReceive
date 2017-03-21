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
#include "tcpserverwidget.h"

// Qt classes
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTabWidget>
#include <QTcpServer>
#include <QTcpSocket>

// NSR classes
#include "networkconfig.h"
#include "tcpsendreceivewidget.h"
#include "logwidget.h"


TcpServerWidget::TcpServerWidget(QWidget* parent) :
    QWidget(parent)
{
    this->serverStatus = server_status::offline;

    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Configuration section

    this->labelConfig = new QLabel(this);
    gridLayout->addWidget(this->labelConfig, 0, 0, 1, -1, Qt::AlignCenter);

    this->labelPort = new QLabel(this);
    gridLayout->addWidget(this->labelPort, 1, 0, 1, 1);

    this->textPort = new QLineEdit(this);
    this->textPort->setText(DEFAULT_PORT);
    QRegularExpression portRegExp("[0-9]{0,10}");
    this->textPort->setValidator(new QRegularExpressionValidator(portRegExp, 0));
    gridLayout->addWidget(this->textPort, 1, 1, 1, 1);

    this->buttonStartServer = new QPushButton(this);
    this->buttonStartServer->setCheckable(true);
    gridLayout->addWidget(this->buttonStartServer, 1, 2, 1, 1);

    // Status

    this->labelServerStatusTitle = new QLabel(this);
    gridLayout->addWidget(this->labelServerStatusTitle, 2, 0, 1, -1, Qt::AlignCenter);

    this->labelServerStatus = new QLabel(this);
    gridLayout->addWidget(this->labelServerStatus, 3, 0, 1, -1, Qt::AlignCenter);

    // Tabs
	this->sendReceiveWidget = new TcpSendReceiveWidget(tr("TCP server:"), this);
    gridLayout->addWidget(this->sendReceiveWidget, 4, 0, 1, -1);

    connect(this->buttonStartServer, &QPushButton::clicked,               this, &TcpServerWidget::buttonStartServerClicked);
    connect(this->sendReceiveWidget, &TcpSendReceiveWidget::disconnected, this, &TcpServerWidget::disconnected);
}

TcpServerWidget::~TcpServerWidget()
{
    this->stopServer();
}

void TcpServerWidget::retranslateUi()
{
    this->labelConfig->setText("<b>" + tr("Configuration") + "</b>");
    this->labelPort->setText(tr("Listen on port"));

    if (this->buttonStartServer->isChecked() == false)
        this->buttonStartServer->setText(tr("Start server"));
    else
        this->buttonStartServer->setText(tr("Terminate server"));

    this->labelServerStatusTitle->setText("<b>" + tr("Server status") + "</b>");

    this->displayServerStatus();

    this->sendReceiveWidget->retranslateUi();
}

void TcpServerWidget::buttonStartServerClicked(bool checked)
{
    this->labelConfig->setStyleSheet("");

    if (checked == true)
    {
        bool serverStarted = this->startServer();

        if (serverStarted == true)
        {
            this->buttonStartServer->setText(tr("Terminate server"));
            this->textPort->setEnabled(false);
        }
        else
        {
            this->buttonStartServer->setChecked(false);
        }
    }
    else
    {
        this->stopServer();

        this->buttonStartServer->setText(tr("Start server"));
        this->textPort->setEnabled(true);

        this->serverStatus = server_status::offline;
        this->displayServerStatus();
    }
}

void TcpServerWidget::connected()
{
    disconnect(this->server.get(), &QTcpServer::newConnection, this, &TcpServerWidget::connected);
    QTcpSocket* currentConnection = this->server->nextPendingConnection();
    this->server->close();

    this->sendReceiveWidget->setSocket(currentConnection);

    this->serverStatus = server_status::connected;
    this->displayServerStatus();

	LogWidget::addLogEntry(tr("TCP server:") + " " + tr("Received and accepted connection from") + " " + currentConnection->peerAddress().toString());
}

void TcpServerWidget::disconnected()
{
    if (this->serverStatus == server_status::connected)
    {
        connect(this->server.get(), &QTcpServer::newConnection, this, &TcpServerWidget::connected);
        this->server->listen(QHostAddress::Any, this->textPort->text().toInt()); // Port already validated and text box disabled, no check required

        this->serverStatus = server_status::disconnected;
        this->displayServerStatus();

		LogWidget::addLogEntry(tr("TCP server:") + " " + tr("Remote client disconnected."));
    }
}

bool TcpServerWidget::startServer()
{
    bool ok;

    int port = this->textPort->text().toInt(&ok);

    if (ok == true)
    {
        this->labelConfig->setStyleSheet("");
        this->labelPort->setStyleSheet("");
        this->textPort->setStyleSheet("");

        this->server = shared_ptr<QTcpServer>(new QTcpServer());
        connect(this->server.get(), &QTcpServer::newConnection, this, &TcpServerWidget::connected);
        this->server->listen(QHostAddress::Any, port);

        this->serverStatus = server_status::waiting_for_connection;
        this->displayServerStatus();

		LogWidget::addLogEntry(tr("TCP server:") + " " + tr("Server launched, listening on port") + " " + QString::number(port));
    }
    else
    {
        this->labelConfig->setStyleSheet("color:red;");
        this->labelPort->setStyleSheet("color:red;");
        this->textPort->setStyleSheet("color:red;");

		LogWidget::addLogEntry(tr("TCP server:") + " " + tr("Configuration error.") + " " + tr("Cancelled server launching."));
    }

    return ok;
}

void TcpServerWidget::stopServer()
{
    this->sendReceiveWidget->closeConnection();

    if (this->server != nullptr)
    {
        this->server->close();
        this->server = nullptr;

		LogWidget::addLogEntry(tr("TCP server:") + " " + tr("Server stopped."));
    }
}

void TcpServerWidget::displayServerStatus()
{
    if (this->serverStatus == server_status::offline)
    {
        this->labelServerStatus->setText(tr("Offline"));
        this->labelServerStatus->setStyleSheet("");
    }
    else if (this->serverStatus == server_status::waiting_for_connection)
    {
        this->labelServerStatus->setText(tr("Waiting for connection..."));
        this->labelServerStatus->setStyleSheet("color:blue;");
    }
    else if (this->serverStatus == server_status::connected)
    {
        this->server->serverAddress();
        this->labelServerStatus->setText(tr("Connected")); // to") + " " + this->currentConnection->peerAddress().toString());
        this->labelServerStatus->setStyleSheet("color:green;");
    }
    else if (this->serverStatus == server_status::disconnected)
    {
        this->labelServerStatus->setText(tr("Disconnected. Waiting for new connection..."));
        this->labelServerStatus->setStyleSheet("color:red;");
    }
}
