/*
 * This file is part of the GAMS Studio project.
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
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GAMSLICENSINGDIALOG_H
#define GAMSLICENSINGDIALOG_H

#include <QDialog>

namespace gams {
namespace studio {
namespace support {

namespace Ui {
class GamsLicensingDialog;
}

class GamsLicensingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GamsLicensingDialog(const QString &title, QWidget *parent = nullptr);
    ~GamsLicensingDialog();

    static QString studioInfo();
    static QString aboutStudio();
    static QString header();
    static void createLicenseFile(QWidget *parent);

private:
    QString gamsLicense();

private slots:
    void on_copylicense_clicked();

private:
    Ui::GamsLicensingDialog *ui;
};

}
}
}
#endif // GAMSLICENSINGDIALOG_H
