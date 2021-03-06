/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the config.tests of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWAYLANDSHELLSURFACE_H
#define QWAYLANDSHELLSURFACE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QSize>
#include <QObject>
#include <QPoint>
#include <QtWaylandClient/qtwaylandclientglobal.h>
#include <QtCore/private/qglobal_p.h>

struct wl_surface;

QT_BEGIN_NAMESPACE

class QVariant;
class QWindow;
class QPlatformWindow;

namespace QtWaylandClient {

class QWaylandWindow;
class QWaylandInputDevice;

class Q_WAYLAND_CLIENT_EXPORT QWaylandShellSurface : public QObject
{
    Q_OBJECT
public:
    explicit QWaylandShellSurface(QWaylandWindow *window);
    ~QWaylandShellSurface() override {}
    virtual bool resize(QWaylandInputDevice *, Qt::Edges) { return false; }
    virtual bool move(QWaylandInputDevice *) { return false; }
    virtual bool showWindowMenu(QWaylandInputDevice *seat) { Q_UNUSED(seat); return false; }
    virtual void setTitle(const QString & /*title*/) {}
    virtual void setAppId(const QString & /*appId*/) {}

    virtual void setWindowFlags(Qt::WindowFlags flags);

    virtual bool isExposed() const { return true; }
    virtual bool handleExpose(const QRegion &) { return false; }

    virtual void raise() {}
    virtual void lower() {}
    virtual void setContentOrientationMask(Qt::ScreenOrientations orientation) { Q_UNUSED(orientation); }

    virtual void sendProperty(const QString &name, const QVariant &value);

    virtual void applyConfigure() {}
    virtual void requestWindowStates(Qt::WindowStates states) {Q_UNUSED(states);}
    virtual bool wantsDecorations() const { return false; }
    virtual QMargins serverSideFrameMargins() const { return QMargins(); }

    virtual void propagateSizeHints() {}

    virtual void setWindowGeometry(const QRect &rect) { Q_UNUSED(rect); }
    virtual void setWindowPosition(const QPoint &position) { Q_UNUSED(position); }

    virtual bool requestActivate() { return false; }
    virtual void setXdgActivationToken(const QString &token);
    virtual void requestXdgActivationToken(quint32 serial);

    inline QWaylandWindow *window() { return m_window; }
    QPlatformWindow *platformWindow();
    struct wl_surface *wlSurface();

protected:
    void resizeFromApplyConfigure(const QSize &sizeWithMargins, const QPoint &offset = {0, 0});
    void repositionFromApplyConfigure(const QPoint &position);
    void setGeometryFromApplyConfigure(const QPoint &globalPosition, const QSize &sizeWithMargins);
    void applyConfigureWhenPossible();
    void handleActivationChanged(bool activated);

    static uint32_t getSerial(QWaylandInputDevice *inputDevice);

private:
    QWaylandWindow *m_window = nullptr;
    friend class QWaylandWindow;
};

}

QT_END_NAMESPACE

#endif // QWAYLANDSHELLSURFACE_H
