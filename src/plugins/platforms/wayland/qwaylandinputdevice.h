/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWAYLANDINPUTDEVICE_H
#define QWAYLANDINPUTDEVICE_H

#include "qwaylandwindow.h"

#include <QSocketNotifier>
#include <QObject>
#include <QtGui/QPlatformIntegration>
#include <QtGui/QPlatformScreen>
#include <QWindowSystemInterface>

#include <wayland-client.h>

QT_BEGIN_NAMESPACE

class QWaylandWindow;
class QWaylandDisplay;

class QWaylandInputDevice {
public:
    QWaylandInputDevice(QWaylandDisplay *display, uint32_t id);
    void attach(QWaylandBuffer *buffer, int x, int y);
    void handleWindowDestroyed(QWaylandWindow *window);
    struct wl_input_device *wl_input_device() const { return mInputDevice; }
    QWaylandWindow *pointerFocus() const { return mPointerFocus; }

    void setTransferDevice(struct wl_data_device *device);
    struct wl_data_device *transferDevice() const;

private:
    QWaylandDisplay *mQDisplay;
    struct wl_display *mDisplay;
    struct wl_input_device *mInputDevice;
    struct wl_data_device *mTransferDevice;
    QWaylandWindow *mPointerFocus;
    QWaylandWindow *mKeyboardFocus;
    QWaylandWindow *mTouchFocus;
    static const struct wl_input_device_listener inputDeviceListener;
    Qt::MouseButtons mButtons;
    QPoint mSurfacePos;
    QPoint mGlobalPos;
    struct xkb_desc *mXkb;
    Qt::KeyboardModifiers mModifiers;
    uint32_t mTime;

    static void inputHandleMotion(void *data,
				  struct wl_input_device *input_device,
				  uint32_t time,
				  int32_t x, int32_t y,
				  int32_t sx, int32_t sy);
    static void inputHandleButton(void *data,
				  struct wl_input_device *input_device,
				  uint32_t time, uint32_t button, uint32_t state);
    static void inputHandleKey(void *data,
			       struct wl_input_device *input_device,
			       uint32_t time, uint32_t key, uint32_t state);
    static void inputHandlePointerFocus(void *data,
					struct wl_input_device *input_device,
					uint32_t time, struct wl_surface *surface,
					int32_t x, int32_t y, int32_t sx, int32_t sy);
    static void inputHandleKeyboardFocus(void *data,
					 struct wl_input_device *input_device,
					 uint32_t time,
					 struct wl_surface *surface,
					 struct wl_array *keys);
    static void inputHandleTouchDown(void *data,
                                     struct wl_input_device *wl_input_device,
                                     uint32_t time,
                                     struct wl_surface *surface,
                                     int id,
                                     int x,
                                     int y);
    static void inputHandleTouchUp(void *data,
                                   struct wl_input_device *wl_input_device,
                                   uint32_t time,
                                   int id);
    static void inputHandleTouchMotion(void *data,
                                       struct wl_input_device *wl_input_device,
                                       uint32_t time,
                                       int id,
                                       int x,
                                       int y);
    static void inputHandleTouchFrame(void *data,
                                      struct wl_input_device *wl_input_device);
    static void inputHandleTouchCancel(void *data,
                                       struct wl_input_device *wl_input_device);

    void handleTouchPoint(int id, int x, int y, Qt::TouchPointState state);
    void handleTouchFrame();
    QList<QWindowSystemInterface::TouchPoint> mTouchPoints;
    QList<QWindowSystemInterface::TouchPoint> mPrevTouchPoints;
    QTouchDevice *mTouchDevice;

    friend class QWaylandTouchExtension;
};

QT_END_NAMESPACE

#endif
