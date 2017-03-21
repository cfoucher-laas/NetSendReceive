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
#include "mainwindow.h"

// Qt classes
#include <QLabel>
#include <QRadioButton>
#include <QTranslator>
#include <QVBoxLayout>
#include <QApplication>

// NSR classes
#include "udpwidget.h"
#include "tcpwidget.h"
#include "logwidget.h"


MainWindow::MainWindow(shared_ptr<QApplication> application, QWidget *parent) :
    QMainWindow(parent)
{
    this->application = application;

    this->resize(1000, 800);
    this->setWindowTitle("Network Send / Receive");

    this->translator = shared_ptr<QTranslator>(new QTranslator());
    this->translator->load(":/translations/français/french");

	// UI

    QWidget* centralWidget = new QWidget(this);

    QVBoxLayout* verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setContentsMargins(10, 10, 10, 10);

    this->tabWidgetProtocol = new QTabWidget();

    // UDP tab
    this->udpWidget = new UdpWidget();
    QWidget* udpTabContentWidget = new QWidget();
    QVBoxLayout* udpTabLayout = new QVBoxLayout(udpTabContentWidget);
    udpTabLayout->setContentsMargins(10, 10, 10, 10);
    udpTabLayout->addWidget(this->udpWidget);
    this->tabWidgetProtocol->addTab(udpTabContentWidget, QString("UDP"));

    // TCP tab
    this->tcpWidget = new TcpWidget();
    QWidget* tcpTabContentWidget = new QWidget();
    QVBoxLayout* tcpTabLayout = new QVBoxLayout(tcpTabContentWidget);
    tcpTabLayout->setContentsMargins(10, 10, 10, 10);
    tcpTabLayout->addWidget(this->tcpWidget);
    this->tabWidgetProtocol->addTab(tcpTabContentWidget, QString("TCP"));

    // Spacer tab
    QWidget* foo = new QWidget();
    this->tabWidgetProtocol->addTab(foo, QString());
    this->tabWidgetProtocol->setTabEnabled(2, false);

    // Options tab
	this->buildOptionTab();
    this->tabWidgetProtocol->addTab(this->optionsWidget, QString());

    // About tab
	this->buildAboutTab();
    this->tabWidgetProtocol->addTab(this->aboutWidget, QString());

    // Log list
    this->logWidget = new LogWidget(this);
	LogWidget::setMainLogWidget(this->logWidget);

	// Whole window
    verticalLayout->addWidget(this->tabWidgetProtocol);
    verticalLayout->addWidget(this->logWidget);

    this->tabWidgetProtocol->setCurrentIndex(0);
    this->setCentralWidget(centralWidget);

    this->retranslateUi();
}

void MainWindow::retranslateUi()
{
    this->tabWidgetProtocol->setTabText(3, tr("Options"));
    this->tabWidgetProtocol->setTabText(4, tr("About"));

    this->labelOptionsLang->setText(tr("Choose language"));

	QString text;

	text  = tr("Version");
	text += " " + QString(NSR_VERSION);
	this->labelAboutVersion->setText(text);

	text  = "Copyright © 2017 <a href=\"https://www.laas.fr\">LAAS/CNRS</a>, ";
	text += tr("developed by");
	text += " <a href=\"https://homepages.laas.fr/cfoucher\">Clément Foucher</a>.";
	this->labelAboutCopyright->setText(text);

	text  = tr("Distributed under the terms of the");
	text += " <a href=\"https://www.gnu.org/licenses/gpl-2.0.html\">GNU GPL V2</a> ";
	text += tr("license agreement.");
	this->labelAboutLicense->setText(text);

	text  = tr("Contact:");
	text += " <a href=\"mailto:clement.foucher@outlook.fr\">clement.foucher@outlook.fr</a>";
	this->labelAboutContact->setText(text);

    this->tcpWidget->retranslateUi();
    this->udpWidget->retranslateUi();
	this->logWidget->retranslateUi();
}

void MainWindow::setLangEnglish()
{
    this->application->removeTranslator(translator.get());
    this->retranslateUi();
}

void MainWindow::setLangFrench()
{
    this->application->installTranslator(translator.get());
	this->retranslateUi();
}

void MainWindow::buildOptionTab()
{
	this->optionsWidget = new QWidget();
	QVBoxLayout* optionTabLayout = new QVBoxLayout(this->optionsWidget);
	optionTabLayout->setContentsMargins(10, 10, 10, 10);
	optionTabLayout->setSpacing(10);

	optionTabLayout->addStretch(10);

	this->labelOptionsLang = new QLabel();
	optionTabLayout->addWidget(this->labelOptionsLang, 0, Qt::AlignCenter);

	optionTabLayout->addStretch(1);

	QRadioButton* langEnglish = new QRadioButton("English");
	langEnglish->setChecked(true);
	connect(langEnglish, &QRadioButton::pressed, this, &MainWindow::setLangEnglish);
	optionTabLayout->addWidget(langEnglish, 0, Qt::AlignCenter);

	QRadioButton* langFrench  = new QRadioButton("Français");
	connect(langFrench, &QRadioButton::pressed, this, &MainWindow::setLangFrench);
	optionTabLayout->addWidget(langFrench, 0, Qt::AlignCenter);

	optionTabLayout->addStretch(10);
}

void MainWindow::buildAboutTab()
{
	this->aboutWidget = new QWidget();
	QVBoxLayout* aboutTabLayout = new QVBoxLayout(this->aboutWidget);
	aboutTabLayout->setContentsMargins(10, 10, 10, 10);
	aboutTabLayout->setSpacing(10);

	aboutTabLayout->addStretch(10);

	this->labelAboutHeader = new QLabel("<b>Network Send / Receive</b>");
	this->labelAboutHeader->setAlignment(Qt::AlignCenter);
	aboutTabLayout->addWidget(this->labelAboutHeader);

	this->labelAboutVersion = new QLabel();
	this->labelAboutVersion->setAlignment(Qt::AlignCenter);
	aboutTabLayout->addWidget(this->labelAboutVersion);

	aboutTabLayout->addStretch(2);

	this->labelAboutCopyright = new QLabel();
	this->labelAboutCopyright->setAlignment(Qt::AlignCenter);
	this->labelAboutCopyright->setTextInteractionFlags(Qt::TextBrowserInteraction);
	this->labelAboutCopyright->setOpenExternalLinks(true);
	aboutTabLayout->addWidget(this->labelAboutCopyright);

	this->labelAboutLicense = new QLabel();
	this->labelAboutLicense->setAlignment(Qt::AlignCenter);
	this->labelAboutLicense->setTextInteractionFlags(Qt::TextBrowserInteraction);
	this->labelAboutLicense->setOpenExternalLinks(true);
	aboutTabLayout->addWidget(this->labelAboutLicense);

	aboutTabLayout->addStretch(1);

	this->labelAboutContact = new QLabel();
	this->labelAboutContact->setAlignment(Qt::AlignCenter);
	this->labelAboutContact->setTextInteractionFlags(Qt::TextBrowserInteraction);
	this->labelAboutContact->setOpenExternalLinks(true);
	aboutTabLayout->addWidget(this->labelAboutContact);

	aboutTabLayout->addStretch(10);
}
