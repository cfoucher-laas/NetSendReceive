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

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

// Parent class
#include <QWidget>

// Qt classes
class QListWidget;
class QPushButton;


class LogWidget : public QWidget
{
    Q_OBJECT

public:
	explicit LogWidget(QWidget* parent = nullptr);

	static void setMainLogWidget(LogWidget* mainLogWidget);
	static void addLogEntry(const QString& entry);

	void retranslateUi();

private slots:
	void clearLog();

private:
	void addLogEntryPrivate(const QString& entry);

private:
	static LogWidget* mainLogWidget;

private:
	QListWidget* logList;
	QPushButton* clearButton;


};

#endif // LOGWIDGET_H
