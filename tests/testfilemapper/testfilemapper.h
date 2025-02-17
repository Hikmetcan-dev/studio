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
#ifndef TESTFILEMAPPER_H
#define TESTFILEMAPPER_H

#include "editors/filemapper.h"
#include <QtTest/QTest>

using gams::studio::FileMapper;

class TestFileMapper : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testFile();
    void testReadChunk0();
    void testReadChunk1();
    void testMoveBackAChunk();
    void testFetchBeyondChunk();
    void testReadLines();
    void testUpdateLineCounting();
    void testPeekChunkLineNrs();
    void testLineNrEstimation();
    void testPosAndAnchor();

private:
    FileMapper *mMapper;
    QDir mCurrentPath;
};

#endif // TESTFILEMAPPER_H
