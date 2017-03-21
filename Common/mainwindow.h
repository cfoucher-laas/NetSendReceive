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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Current class parent
#include <QMainWindow>

// C++ classes
#include <memory>
using namespace std;

// Qt classes
class QWidget;
class QTabWidget;
class QLabel;
class QTranslator;
class QApplication;

// NSR classes
class UdpWidget;
class TcpWidget;
class LogWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(shared_ptr<QApplication> application, QWidget* parent = nullptr);

    void retranslateUi();

private slots:
    void setLangEnglish();
    void setLangFrench();

private:
	void buildOptionTab();
	void buildAboutTab();

private:
    shared_ptr<QApplication> application;
    shared_ptr<QTranslator>  translator;

    // Tabs
    QTabWidget* tabWidgetProtocol;

    UdpWidget* udpWidget;
    TcpWidget* tcpWidget;
    QWidget*   optionsWidget;
    QWidget*   aboutWidget;

    LogWidget* logWidget;

	// Options
    QLabel* labelOptionsLang;

	// About
	QLabel* labelAboutHeader;
	QLabel* labelAboutVersion;
	QLabel* labelAboutLicense;
	QLabel* labelAboutCopyright;
	QLabel* labelAboutContact;


};

#endif // MAINWINDOW_H
