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
#include "datatablewidget.h"

// Qt classes
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QRadioButton>
#include <QLabel>

// NSR classes
#include "datatableview.h"


DataTableWidget::DataTableWidget(bool enableLoad, bool enableSend, bool isEditable, QWidget* parent) :
    QWidget(parent)
{
    /* Global layout:
    ----------------------------
    - Representation -         -
    ----------------------------
    -     Table      - Buttons -
    ----------------------------
    */

    QGridLayout* globalLayout = new QGridLayout(this);
    globalLayout->setSpacing(10);
    globalLayout->setContentsMargins(0, 0, 0, 0);

    // Data representation choice
    QHBoxLayout* representationLayout = new QHBoxLayout();
    representationLayout->setContentsMargins(0, 0, 0, 0);

    this->labelRepresentation = new QLabel(this);
    representationLayout->addWidget(this->labelRepresentation);

    this->radioButtonAscii = new QRadioButton(this);
    representationLayout->addWidget(this->radioButtonAscii);

    this->radioButtonHexa = new QRadioButton(this);
    this->radioButtonHexa->setChecked(true);
    representationLayout->addWidget(this->radioButtonHexa);

    globalLayout->addLayout(representationLayout, 0, 0, 1, 1);

    // Table
    this->dataTableDisplay = new DataTableView(isEditable, this);
    globalLayout->addWidget(this->dataTableDisplay, 1, 0, 1, 1);

    // Buttons
    QVBoxLayout* buttonsLayout = new QVBoxLayout();
    buttonsLayout->setContentsMargins(0, 0, 0, 0);

    buttonsLayout->addStretch(1);

    if (isEditable == true)
    {
        this->buttonClear = new QPushButton(this);
        buttonsLayout->addWidget(this->buttonClear);

        this->buttonRemoveLast = new QPushButton(this);
        buttonsLayout->addWidget(this->buttonRemoveLast);

        buttonsLayout->addStretch(1);
    }
    else
    {
        this->buttonClear = nullptr;
        this->buttonRemoveLast = nullptr;
    }

    this->buttonSave = new QPushButton(this);
	if (isEditable == false)
	{
		this->buttonSave->setEnabled(false);
	}
    buttonsLayout->addWidget(this->buttonSave);

    if (enableLoad == true)
    {
        this->buttonLoad = new QPushButton(this);
        buttonsLayout->addWidget(this->buttonLoad);
    }
    else
    {
        this->buttonLoad = nullptr;
    }

    if (enableSend == true)
    {
        buttonsLayout->addStretch(1);
        this->buttonSend = new QPushButton(this);
        buttonsLayout->addWidget(this->buttonSend);
    }
    else
    {
        this->buttonSend = nullptr;
    }

    buttonsLayout->addStretch(5);

    globalLayout->addLayout(buttonsLayout, 1, 1, 1, 1);

    // Text

    this->retranslateUi();

    // Connections

    connect(this->radioButtonAscii, &QRadioButton::toggled, this, &DataTableWidget::setAsciiRepresentation);
    connect(this->radioButtonHexa,  &QRadioButton::toggled, this, &DataTableWidget::setHexaRepresentation);
    if (isEditable == true)
    {
        connect(this->buttonClear,      &QPushButton::clicked, this, &DataTableWidget::clearData);
        connect(this->buttonRemoveLast, &QPushButton::clicked, this, &DataTableWidget::removeLastByte);
    }
    connect(this->buttonSave, &QPushButton::clicked, this, &DataTableWidget::saveData);
    if (enableLoad == true)
    {
        connect(this->buttonLoad, &QPushButton::clicked, this, &DataTableWidget::loadData);
    }
    if (enableSend == true)
    {
        connect(this->buttonSend, &QPushButton::clicked, this, &DataTableWidget::sendData);
    }
}

void DataTableWidget::retranslateUi()
{
	this->labelRepresentation->setText(tr("Data representation:"));

    this->radioButtonAscii->setText(tr("ASCII"));
    this->radioButtonHexa->setText(tr("Hexadecimal"));

    if (this->buttonClear != nullptr)
    {
        this->buttonClear->setText(tr("Clear"));
    }
    if (this->buttonRemoveLast != nullptr)
    {
        this->buttonRemoveLast->setText(tr("Remove last byte"));
    }

    this->buttonSave->setText(tr("Save data to file"));

    if (this->buttonLoad != nullptr)
    {
        this->buttonLoad->setText(tr("Load data from file"));
    }

    if (this->buttonSend != nullptr)
    {
        this->buttonSend->setText(tr("Send data"));
    }
}

void DataTableWidget::setData(shared_ptr<QByteArray> data)
{
    this->dataTableDisplay->setData(data);

	if ( (data == nullptr) && (this->dataTableDisplay->isEditable() == false) )
	{
		this->buttonSave->setEnabled(false);
	}
	else
	{
		this->buttonSave->setEnabled(true);
	}
}

void DataTableWidget::setAsciiRepresentation(bool doSet)
{
    if (doSet == true)
    {
        this->dataTableDisplay->setDataRepresentation(TableModel::dataRepresentation::ASCII);
    }
}

void DataTableWidget::setHexaRepresentation(bool doSet)
{
    if (doSet == true)
    {
        this->dataTableDisplay->setDataRepresentation(TableModel::dataRepresentation::Hexa);
    }
}

void DataTableWidget::clearData()
{
	this->dataTableDisplay->clearData();

	if (this->dataTableDisplay->isEditable() == false)
	{
		this->buttonSave->setEnabled(false);
	}
}

void DataTableWidget::removeLastByte()
{
    this->dataTableDisplay->removeLastByte();
}

void DataTableWidget::saveData()
{
    if (this->dataTableDisplay->getData() != nullptr)
    {
		QString fileName = QFileDialog::getSaveFileName(this, tr("Choose file name"), ".", "*");

		if (fileName.isEmpty() == false)
        {
            QFile file(fileName);
            file.open(QIODevice::ReadWrite);

            shared_ptr<QByteArray> data = this->dataTableDisplay->getData();
            file.write(data->data(), data->size());
            file.close();
        }
    }
}

void DataTableWidget::sendData()
{
    emit sendRequested(this->dataTableDisplay->getData());
}

void DataTableWidget::loadData()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select file to send"), ".", "*");

	if (fileName.isEmpty() == false)
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray content = file.readAll();
        file.close();

        this->dataTableDisplay->setData(shared_ptr<QByteArray>(new QByteArray(content)));

		this->buttonSave->setEnabled(true);
    }

}

