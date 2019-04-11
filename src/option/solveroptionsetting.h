/*
 * This file is part of the GAMS Studio project.
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
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
#ifndef SOLVEROPTIONSETTING_H
#define SOLVEROPTIONSETTING_H

#include <QWidget>

namespace gams {
namespace studio {
namespace option {

namespace Ui {
class SolverOptionSetting;
}

class SolverOptionSetting : public QWidget
{
    Q_OBJECT

public:
    explicit SolverOptionSetting(QString eolchars, QString separator, QString stringquote, QWidget *parent = nullptr);
    ~SolverOptionSetting();

    QChar getDefaultEOLCharacter() const;
//    QChar getDefaultSeparatorCharacter() const;

signals:
    void addCommentAboveChanged(int checkState);
    void addOptionDescriptionAsComment(int checkState);
    void EOLCharChanged(QChar ch);
//    void separatorCharChanged(QChar ch);

private slots:
    void on_addCommentAboveCheckBox_stateChanged(int checkState);
    void on_addEOLCommentCheckBox_stateChanged(int checkState);

private:
    Ui::SolverOptionSetting *ui;
    QString mEOLChars;
    QChar mDefaultEOLChar;
    QString mDefaultSeparator;
    QString mDefaultStringQute;
};

}
}
}

#endif // SOLVEROPTIONSETTING_H
