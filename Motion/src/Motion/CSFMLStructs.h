////////////////////////////////////////////////////////////
// In order to comply with the below license...
// This is a compilation of CSFML struct types created by Zachariah Brown.
// This is not the original headers and is therefore marked as a modified version.
////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef MOTION_SFMLSTRUCTS_H
#define MOTION_SFMLSTRUCTS_H

#include <cstdint>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/BlendMode.hpp>

#include <Motion/Types.h>

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} sfColor;

typedef struct
{
    int x;
    int y;
} sfVector2i;

typedef struct
{
    unsigned int x;
    unsigned int y;
} sfVector2u;

typedef struct
{
    float x;
    float y;
} sfVector2f;

typedef struct
{
    int64_t microseconds;
} sfTime;

struct sfView
{
    sf::View This;
};

struct sfRenderWindow
{
    sf::RenderWindow This;
    sfView           DefaultView;
    sfView           CurrentView;
};

struct sfRenderTexture
{
    sf::RenderTexture This;
    const sfTexture*  Target;
    sfView            DefaultView;
    sfView            CurrentView;
};

struct sfTexture
{
    sf::Texture* This;
    bool OwnInstance;
};

struct sfShader
{
    sf::Shader This;
};

struct sfImage
{
    sf::Image This;
};

typedef enum
{
    sfBlendFactorZero,
    sfBlendFactorOne,
    sfBlendFactorSrcColor,
    sfBlendFactorOneMinusSrcColor,
    sfBlendFactorDstColor,
    sfBlendFactorOneMinusDstColor,
    sfBlendFactorSrcAlpha,
    sfBlendFactorOneMinusSrcAlpha,
    sfBlendFactorDstAlpha,
    sfBlendFactorOneMinusDstAlpha
} sfBlendFactor;

typedef enum
{
    sfBlendEquationAdd,
    sfBlendEquationSubtract
} sfBlendEquation;

typedef struct
{
    sfBlendFactor colorSrcFactor;
    sfBlendFactor colorDstFactor;
    sfBlendEquation colorEquation;
    sfBlendFactor alphaSrcFactor;
    sfBlendFactor alphaDstFactor;
    sfBlendEquation alphaEquation;
} sfBlendMode;

typedef struct
{
    float matrix[9];
} sfTransform;

typedef struct
{
    sfBlendMode blendMode;
    sfTransform transform;
    const sfTexture* texture;
    const sfShader* shader;
} sfRenderStates;

inline sf::Transform convertTransform(const sfTransform& transform)
{
    const float* m = transform.matrix;
    return sf::Transform(m[0], m[1], m[2],
        m[3], m[4], m[5],
        m[6], m[7], m[8]);
}

inline sf::RenderStates convertRenderStates(const sfRenderStates* states)
{
    sf::RenderStates sfmlStates;

    if (states)
    {
        sfmlStates.blendMode.colorSrcFactor = static_cast<sf::BlendMode::Factor>(states->blendMode.colorSrcFactor);
        sfmlStates.blendMode.colorDstFactor = static_cast<sf::BlendMode::Factor>(states->blendMode.colorDstFactor);
        sfmlStates.blendMode.colorEquation = static_cast<sf::BlendMode::Equation>(states->blendMode.colorEquation);
        sfmlStates.blendMode.alphaSrcFactor = static_cast<sf::BlendMode::Factor>(states->blendMode.alphaSrcFactor);
        sfmlStates.blendMode.alphaDstFactor = static_cast<sf::BlendMode::Factor>(states->blendMode.alphaDstFactor);
        sfmlStates.blendMode.alphaEquation = static_cast<sf::BlendMode::Equation>(states->blendMode.alphaEquation);
        sfmlStates.transform = convertTransform(states->transform);
        sfmlStates.texture = states->texture ? states->texture->This : NULL;
        sfmlStates.shader = states->shader ? &states->shader->This : NULL;
    }

    return sfmlStates;
}

#endif
