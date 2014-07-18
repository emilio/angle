//
// Copyright (c) 2013-2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// shadervars.h:
//  Types to represent GL variables (varyings, uniforms, etc)
//

#ifndef COMMON_SHADERVARIABLE_H_
#define COMMON_SHADERVARIABLE_H_

#include <string>
#include <vector>
#include <algorithm>
#include "GLSLANG/ShaderLang.h"

namespace sh
{

// Varying interpolation qualifier, see section 4.3.9 of the ESSL 3.00.4 spec
enum InterpolationType
{
    INTERPOLATION_SMOOTH,
    INTERPOLATION_CENTROID,
    INTERPOLATION_FLAT
};

// Uniform block layout qualifier, see section 4.3.8.3 of the ESSL 3.00.4 spec
enum BlockLayoutType
{
    BLOCKLAYOUT_STANDARD,
    BLOCKLAYOUT_PACKED,
    BLOCKLAYOUT_SHARED
};

// Base class for all variables defined in shaders, including Varyings, Uniforms, etc
struct ShaderVariable
{
    ShaderVariable()
        : type(0),
          precision(0),
          arraySize(0),
          staticUse(false)
    {}

    ShaderVariable(GLenum typeIn, unsigned int arraySizeIn)
        : type(typeIn),
          precision(0),
          arraySize(arraySizeIn),
          staticUse(false)
    {}

    bool isArray() const { return arraySize > 0; }
    unsigned int elementCount() const { return std::max(1u, arraySize); }

    GLenum type;
    GLenum precision;
    std::string name;
    std::string mappedName;
    unsigned int arraySize;
    bool staticUse;
};

struct Uniform : public ShaderVariable
{
    Uniform() {}

    bool isStruct() const { return !fields.empty(); }

    std::vector<Uniform> fields;
};

struct Attribute : public ShaderVariable
{
    Attribute()
        : location(-1)
    {}

    int location;
};

struct InterfaceBlockField : public ShaderVariable
{
    InterfaceBlockField()
        : isRowMajorMatrix(false)
    {}

    bool isStruct() const { return !fields.empty(); }

    bool isRowMajorMatrix;
    std::vector<InterfaceBlockField> fields;
};

struct Varying : public ShaderVariable
{
    Varying()
        : interpolation(INTERPOLATION_SMOOTH)
    {}

    bool isStruct() const { return !fields.empty(); }

    InterpolationType interpolation;
    std::vector<Varying> fields;
    std::string structName;
};

struct InterfaceBlock
{
    InterfaceBlock()
        : arraySize(0),
          layout(BLOCKLAYOUT_PACKED),
          isRowMajorLayout(false),
          staticUse(false)
    {}

    std::string name;
    std::string mappedName;
    unsigned int arraySize;
    BlockLayoutType layout;
    bool isRowMajorLayout;
    bool staticUse;
    std::vector<InterfaceBlockField> fields;
};

}

#endif // COMMON_SHADERVARIABLE_H_
