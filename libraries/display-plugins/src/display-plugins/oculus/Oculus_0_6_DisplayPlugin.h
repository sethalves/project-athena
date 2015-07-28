//
//  Created by Bradley Austin Davis on 2015/05/29
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
#pragma once

#include "OculusBaseDisplayPlugin.h"

#include <QTimer>

#if (OVR_MAJOR_VERSION == 6)

class OffscreenGlCanvas;
struct SwapFramebufferWrapper;
struct MirrorFramebufferWrapper;

using SwapFboPtr = QSharedPointer<SwapFramebufferWrapper>;
using MirrorFboPtr = QSharedPointer<MirrorFramebufferWrapper>;

class Oculus_0_6_DisplayPlugin : public OculusBaseDisplayPlugin {
public:
    virtual bool isSupported() const override;
    virtual const QString & getName() const override;

    virtual void activate(PluginContainer * container) override;
    virtual void deactivate(PluginContainer* container) override;


    virtual bool eventFilter(QObject* receiver, QEvent* event) override;

protected:
    virtual void display(GLuint finalTexture, const glm::uvec2& sceneSize) override;
    virtual void customizeContext(PluginContainer * container) override;
    // Do not perform swap in finish
    virtual void finishFrame() override;

private:

    static const QString NAME;

    ovrLayerEyeFov& getSceneLayer();

    SwapFboPtr          _sceneFbo;
    MirrorFboPtr        _mirrorFbo;
    ovrLayerEyeFov      _sceneLayer;
};

#endif
