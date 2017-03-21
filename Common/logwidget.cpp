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
#include "logwidget.h"

// Qt classes
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>


LogWidget* LogWidget::mainLogWidget = nullptr;


LogWidget::LogWidget(QWidget* parent) :
	QWidget(parent)
{
	QHBoxLayout* hlayout = new QHBoxLayout(this);
	hlayout->setSpacing(10);
	hlayout->setContentsMargins(0, 0, 0, 0);

	this->logList = new QListWidget();
	hlayout->addWidget(this->logList);

	this->clearButton = new QPushButton();
	hlayout->addWidget(this->clearButton);

	this->retranslateUi();

	connect(this->clearButton, &QPushButton::clicked, this, &LogWidget::clearLog);
}

void LogWidget::setMainLogWidget(LogWidget* mainLogWidget)
{
	if (LogWidget::mainLogWidget == nullptr)
	{
		LogWidget::mainLogWidget = mainLogWidget;
	}
}

void LogWidget::addLogEntry(const QString& entry)
{
	if (LogWidget::mainLogWidget != nullptr)
	{
		LogWidget::mainLogWidget->addLogEntryPrivate(entry);
	}
}

void LogWidget::retranslateUi()
{
	this->clearButton->setText(tr("Clear log"));
}

void LogWidget::clearLog()
{
	this->logList->clear();
}

void LogWidget::addLogEntryPrivate(const QString& entry)
{
	this->logList->addItem(entry);
	this->logList->scrollToBottom();
}
