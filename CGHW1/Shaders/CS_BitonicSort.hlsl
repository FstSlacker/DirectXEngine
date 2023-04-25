cbuffer cData : register(b0)
{
    float4 GroupDimY_ElementsCount_MaxStage_CurrentStage;
};

RWStructuredBuffer<float2> Data : register(u0);

#define BLOCK_SIZE 256
#define THREAD_IN_GROUP_TOTAL 256


[numthreads(BLOCK_SIZE, 1, 1)]
void CSMain(
    uint3 groupID : SV_GroupID,
    uint3 groupThreadID : SV_GroupThreadID,
    uint3 dispatchThreadID : SV_DispatchThreadID,
    uint groupIndex : SV_GroupIndex
)
{
    int i = groupID.x * THREAD_IN_GROUP_TOTAL + groupID.y * THREAD_IN_GROUP_TOTAL * GroupDimY_ElementsCount_MaxStage_CurrentStage.x + groupIndex;
    
    int elementsCount = (int)GroupDimY_ElementsCount_MaxStage_CurrentStage.y;
    int maxStage = (int)GroupDimY_ElementsCount_MaxStage_CurrentStage.z;
    int currentStage = (int)GroupDimY_ElementsCount_MaxStage_CurrentStage.w;
    
    [flattern]
    if (i >= (elementsCount >> 1))
        return;
    
    int offset = (currentStage - 1);

    int ind = (i + ((i >> offset) << offset));

    bool dir = (bool)((i >> (maxStage - 1)) & 1);

    float2 v1 = Data[ind];
    float2 v2 = Data[ind + (1 << offset)];
    
    if ((v1.y < v2.y) == dir)
    {
        Data[ind] = v2;
        Data[ind + (1 << offset)] = v1;
    }
}