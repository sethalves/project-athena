//
//  Haze.cpp
//  libraries/model/src/model
//
//  Created by Nissim Hadar on 9/13/2017.
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
#include <memory>
#include <gpu/Resource.h>

#include "Haze.h"

using namespace model;

Haze::Haze() {
    Parameters parameters;
    _hazeParametersBuffer = gpu::BufferView(std::make_shared<gpu::Buffer>(sizeof(Parameters), (const gpu::Byte*) &parameters));
}

enum HazeModes {
    HAZE_MODE_IS_ACTIVE                        = 1 << 0,
    HAZE_MODE_IS_ALTITUDE_BASED                = 1 << 1,
    HAZE_MODE_IS_DIRECTIONAL_LIGHT_ATTENUATED  = 1 << 2,
    HAZE_MODE_IS_MODULATE_COLOR                = 1 << 3
};

// For color modulated mode, the colour values are used as range values, which are then converted to range factors
// This is separate for each colour.
// The colour value is converted from [0.0 .. 1.0] to [5.0 .. 3000.0]
void Haze::setHazeColor(const glm::vec3 hazeColor) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.hazeColor.r != hazeColor.r) {
        _hazeParametersBuffer.edit<Parameters>().hazeColor.r = hazeColor.r;

        float range = hazeColor.r * 2995.0f + 5.0f;
        float factor = convertHazeRangeToHazeRangeFactor(range);
        _hazeParametersBuffer.edit<Parameters>().colorModulationFactor.r = factor;
    }
    if (params.hazeColor.g != hazeColor.g) {
        _hazeParametersBuffer.edit<Parameters>().hazeColor.g = hazeColor.g;

        float range = hazeColor.g * 2995.0f + 5.0f;
        float factor = convertHazeRangeToHazeRangeFactor(range);
        _hazeParametersBuffer.edit<Parameters>().colorModulationFactor.g = factor;
    }

    if (params.hazeColor.b != hazeColor.b) {
        _hazeParametersBuffer.edit<Parameters>().hazeColor.b = hazeColor.b;

        float range = hazeColor.b * 2995.0f + 5.0f;
        float factor = convertHazeRangeToHazeRangeFactor(range);
        _hazeParametersBuffer.edit<Parameters>().colorModulationFactor.b = factor;
    }
}

void Haze::setDirectionalLightBlend(const float hazeDirectionalLightBlend) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.directionalLightBlend != hazeDirectionalLightBlend) {
        _hazeParametersBuffer.edit<Parameters>().directionalLightBlend = hazeDirectionalLightBlend;
    }
}

void Haze::setDirectionalLightColor(const glm::vec3 hazeDirectionalLightColor) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.directionalLightColor.r != hazeDirectionalLightColor.r) {
        _hazeParametersBuffer.edit<Parameters>().directionalLightColor.r = hazeDirectionalLightColor.r;
    }
    if (params.directionalLightColor.g != hazeDirectionalLightColor.g) {
        _hazeParametersBuffer.edit<Parameters>().directionalLightColor.g = hazeDirectionalLightColor.g;
    }
    if (params.directionalLightColor.b != hazeDirectionalLightColor.b) {
        _hazeParametersBuffer.edit<Parameters>().directionalLightColor.b = hazeDirectionalLightColor.b;
    }
}
void Haze::setHazeActive(const bool isHazeActive) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (((params.hazeMode & HAZE_MODE_IS_ACTIVE) == HAZE_MODE_IS_ACTIVE )&& !isHazeActive) {
        _hazeParametersBuffer.edit<Parameters>().hazeMode &= ~HAZE_MODE_IS_ACTIVE;
    }
    else if (((params.hazeMode & HAZE_MODE_IS_ACTIVE) != HAZE_MODE_IS_ACTIVE) && isHazeActive) {
        _hazeParametersBuffer.edit<Parameters>().hazeMode |= HAZE_MODE_IS_ACTIVE;
    }
}

void Haze::setAltitudeBased(const bool isAltitudeBased) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (((params.hazeMode & HAZE_MODE_IS_ALTITUDE_BASED) == HAZE_MODE_IS_ALTITUDE_BASED )&& !isAltitudeBased) {
        _hazeParametersBuffer.edit<Parameters>().hazeMode &= ~HAZE_MODE_IS_ALTITUDE_BASED;
    }
    else if (((params.hazeMode & HAZE_MODE_IS_ALTITUDE_BASED) != HAZE_MODE_IS_ALTITUDE_BASED) && isAltitudeBased) {
        _hazeParametersBuffer.edit<Parameters>().hazeMode |= HAZE_MODE_IS_ALTITUDE_BASED;
    }
}

void Haze::setHazeAttenuateKeyLight(const bool isHazeAttenuateKeyLight) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (((params.hazeMode & HAZE_MODE_IS_DIRECTIONAL_LIGHT_ATTENUATED) == HAZE_MODE_IS_DIRECTIONAL_LIGHT_ATTENUATED ) && !isHazeAttenuateKeyLight) {
        _hazeParametersBuffer.edit<Parameters>().hazeMode &= ~HAZE_MODE_IS_DIRECTIONAL_LIGHT_ATTENUATED;
    }
    else if (((params.hazeMode & HAZE_MODE_IS_DIRECTIONAL_LIGHT_ATTENUATED) != HAZE_MODE_IS_DIRECTIONAL_LIGHT_ATTENUATED) && isHazeAttenuateKeyLight) {
        _hazeParametersBuffer.edit<Parameters>().hazeMode |= HAZE_MODE_IS_DIRECTIONAL_LIGHT_ATTENUATED;
    }
}

void Haze::setModulateColorActive(const bool isModulateColorActive) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (((params.hazeMode & HAZE_MODE_IS_MODULATE_COLOR) == HAZE_MODE_IS_MODULATE_COLOR ) && !isModulateColorActive) {
        _hazeParametersBuffer.edit<Parameters>().hazeMode &= ~HAZE_MODE_IS_MODULATE_COLOR;
    }
    else if (((params.hazeMode & HAZE_MODE_IS_MODULATE_COLOR) != HAZE_MODE_IS_MODULATE_COLOR) && isModulateColorActive) {
        _hazeParametersBuffer.edit<Parameters>().hazeMode |= HAZE_MODE_IS_MODULATE_COLOR;
    }
}

void Haze::setHazeRangeFactor(const float hazeRangeFactor) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.hazeRangeFactor != hazeRangeFactor) {
        _hazeParametersBuffer.edit<Parameters>().hazeRangeFactor = hazeRangeFactor;
    }
}

void Haze::setHazeAltitudeFactor(const float hazeAltitudeFactor) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.hazeAltitudeFactor != hazeAltitudeFactor) {
        _hazeParametersBuffer.edit<Parameters>().hazeAltitudeFactor = hazeAltitudeFactor;
    }
}

void Haze::setHazeKeyLightRangeFactor(const float hazeKeyLightRangeFactor) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.hazeKeyLightRangeFactor != hazeKeyLightRangeFactor) {
        _hazeParametersBuffer.edit<Parameters>().hazeKeyLightRangeFactor = hazeKeyLightRangeFactor;
    }
}

void Haze::setHazeKeyLightAltitudeFactor(const float hazeKeyLightAltitudeFactor) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.hazeKeyLightAltitudeFactor != hazeKeyLightAltitudeFactor) {
        _hazeParametersBuffer.edit<Parameters>().hazeKeyLightAltitudeFactor = hazeKeyLightAltitudeFactor;
    }
}

void Haze::setHazeBaseReference(const float hazeBaseReference) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.hazeBaseReference != hazeBaseReference) {
        _hazeParametersBuffer.edit<Parameters>().hazeBaseReference = hazeBaseReference;
    }
}

void Haze::setHazeBackgroundBlendValue(const float hazeBackgroundBlendValue) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.hazeBackgroundBlendValue != hazeBackgroundBlendValue) {
        _hazeParametersBuffer.edit<Parameters>().hazeBackgroundBlendValue = hazeBackgroundBlendValue;
    }
}

void Haze::setZoneOrientation(const glm::quat& zoneOrientation) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    glm::vec3 zoneDirection = zoneOrientation * glm::vec3(0.0f, 0.0f, -1.0f);
    if (params.zoneDirection == zoneDirection) {
        _hazeParametersBuffer.edit<Parameters>().zoneDirection = zoneDirection;
    }
}

void Haze::setZonePosition(const glm::vec3& zonePosition) {
    auto& params = _hazeParametersBuffer.get<Parameters>();

    if (params.zonePosition != zonePosition) {
        _hazeParametersBuffer.edit<Parameters>().zonePosition = zonePosition;
    }
}
