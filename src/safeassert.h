/*
 *  Safe assertion
 *  Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 *  Contact: Nokia Corporation (qt-info@nokia.com)
 *
 *  GNU Lesser General Public License Usage
 *
 *  This file may be used under the terms of the GNU Lesser
 *  General Public License version 2.1 as published by the Free Software
 *  Foundation and appearing in the file LICENSE.LGPL included in the
 *  packaging of this file.  Please review the following information to
 *  ensure the GNU Lesser General Public License version 2.1 requirements
 *  will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 */

#ifndef SAFEASSERT_H
#define SAFEASSERT_H

/*
 * This assertion is based on the QTC_ASSERT from Qt Creator.
 */

#include <QtCore/QDebug>

#define SAFE_ASSERT_STRINGIFY_INTERNAL(x) #x
#define SAFE_ASSERT_STRINGIFY(x) SAFE_ASSERT_STRINGIFY_INTERNAL(x)

// we do not use the  'do {...} while (0)' idiom here to be able to use
// 'break' and 'continue' as 'actions'.

#define SAFE_ASSERT(cond, action) \
    if(cond){}else{qDebug()<<"ASSERTION " #cond " FAILED AT " __FILE__ ":" SAFE_ASSERT_STRINGIFY(__LINE__);action;}

#endif // SAFEASSERT_H
