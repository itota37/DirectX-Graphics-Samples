//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "RayTracingHlslCompat.h"

namespace GlobalRootSignature {
    namespace Slot {
        enum Enum {
            OutputView = 0,
            AccelerationStructure,
            SceneConstant,
            AABBattributeBuffer,
            VertexBuffers,
            Count
        };
    }
}

namespace LocalRootSignature {
    namespace Type {
        enum Enum {
            Triangle = 0,
            AABB,
    #if USE_NON_NULL_LOCAL_ROOT_SIG
            Empty,
    #endif
            Count
        };
    }
}

namespace LocalRootSignature {
    namespace Triangle {
        namespace Slot {
            enum Enum {
                MaterialConstant = 0,
                Count
            };
        }
        struct RootArguments {
            PrimitiveConstantBuffer materialCb;
        };
    }
}

namespace LocalRootSignature {
    namespace AABB {
        namespace Slot {
            enum Enum {
                MaterialConstant = 0,
                GeometryIndex,
                VertexBuffers,
                Count
            };
        }
        struct RootArguments {
            PrimitiveConstantBuffer materialCb;
            PrimitiveInstanceConstantBuffer aabbCB;
            UINT32 padding; // 4byte padding to align on 8 byte boundary 
                            // since ShaderIdentifierSize is 12 and next GPU
                            // handle variable requires 8 byte alignement.
            // Now we specify a GPU handle to the vertex buffer.
            // However instead of using the handle struct with 8 byte
            // ptr itself, we use 4 byte variables to avoid alignment
            // of the whole RootArguments struct to 8 bytes. This is because
            // it would throw alignment off in the final shader record
            // with 12 byte shaderidentifiersize up front.
             UINT32 vertexBufferGPUHandle;   
             UINT32 padding2;
        };
    }
}

namespace LocalRootSignature {
    inline UINT MaxRootArgumentsSize()
    {
        return max(sizeof(Triangle::RootArguments), sizeof(AABB::RootArguments));
    }
}

namespace GeometryType {
    enum Enum {
        Triangle = 0,
        AABB,       // Procedural geometry with an application provided AABB.
        Count
    };
}

// Bottom-level acceleration structures (BottomLevelASType).
// This sample uses two BottomLevelASType, one for AABB and one for Triangle geometry.
// Mixing of geometry types within a BLAS is not supported.
namespace BottomLevelASType = GeometryType;


namespace IntersectionShaderType {
    enum Enum {
        AnalyticPrimitive = 0,
        VolumetricPrimitive,
        SignedDistancePrimitive,
        Count
    };
    inline UINT PerPrimitiveTypeCount(Enum type)
    {
        switch (type)
        {
        case AnalyticPrimitive: return AnalyticPrimitive::Count;
        case VolumetricPrimitive: return VolumetricPrimitive::Count;
        case SignedDistancePrimitive: return SignedDistancePrimitive::Count;
        }
        return 0;
    }
    static const UINT MaxPerPrimitiveTypeCount =
        max(AnalyticPrimitive::Count, max(VolumetricPrimitive::Count, SignedDistancePrimitive::Count));
    static const UINT TotalPrimitiveCount =
        AnalyticPrimitive::Count + VolumetricPrimitive::Count + SignedDistancePrimitive::Count;
}

