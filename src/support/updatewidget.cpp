/*
 *
 * Copyright (c) 2017-2023 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2023 GAMS Development Corp. <support@gams.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "updatewidget.h"
#include "ui_updatewidget.h"
#include "settings.h"

#include <QDate>

namespace gams {
namespace studio {
namespace support {

UpdateWidget::UpdateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UpdateWidget)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, [this]{ hide(); });
    connect(ui->settingsButton, &QPushButton::clicked, this, &UpdateWidget::openSettings);
    connect(ui->remindButton, &QPushButton::clicked, this, &UpdateWidget::remindMeLater);
}

UpdateWidget::~UpdateWidget()
{
    delete ui;
}

void UpdateWidget::setText(const QString &text)
{
    ui->label->setText(text);
}

void UpdateWidget::remindMeLater()
{
    Settings::settings()->setDate(skNextUpdateCheckDate, QDate::currentDate().addDays(1));
    hide();
}

}
}
}
