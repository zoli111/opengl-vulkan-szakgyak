## 1. VkVertexInputBindingDescription

**Definíció**

```cpp
typedef struct VkVertexInputBindingDescription {
    uint32_t             binding;
    uint32_t             stride;
    VkVertexInputRate    inputRate;
} VkVertexInputBindingDescription;
```
- `binding`: a shaderben használt binding, ami alatt elérhetőek lesznek a vertex adatok
- `stride`: hány bájtonként következik új vertex (tehát összesen hány bájtot foglalnak egy vertex adatai)
- `inputRate`: vagy `VK_VERTEX_INPUT_RATE_VERTEX`, vagy `VK_VERTEX_INPUT_RATE_INSTANCE`
	- TODO: micsodák is ezek?

**Példa**

TODO: befejezni

## 2. VkVertexInputAttributeDescription

- Ez a struktúra írja le, hogy milyen különböző adatok tartoznak egy vertexhez, és ezek formátumát
	- Pl. koordináták, színek, textúra koordináták
- Egy vertex bindinghoz lehet, hogy többet is létre kell hozni

**Definíció**

```cpp
typedef struct VkVertexInputAttributeDescription {
    uint32_t    location;
    uint32_t    binding;
    VkFormat    format;
    uint32_t    offset;
} VkVertexInputAttributeDescription;
```

- `location`: milyen location alatt lesz elérhető a shaderben
- `binding`: milyen binding alatt lesz elérhető a shaderben
- `format`: az adatok formátuma
	- Ami zavaró lehet, hogy a ugyanolyan formátumban kell megadni a többi adatot, mint a színeket
		- `float`: `VK_FORMAT_R32_SFLOAT`
		- `vec2`: `VK_FORMAT_R32G32_SFLOAT`
		- `vec3`: `VK_FORMAT_R32G32B32_SFLOAT`
		- `vec4`: `VK_FORMAT_R32G32B32A32_SFLOAT`
		- `ivec2`: `VK_FORMAT_R32G32_SINT`: 32 bites előjeles egészekből álló 2 elemű vektor
		- `uvec4`: `VK_FORMAT_R32G32B32A32_UINT`: 32 bites egészekből álló 4 elemű vektor
		- `double`: `VK_FORMAT_R64_SFLOAT`: 64 bites float
		- [Stb.](https://docs.vulkan.org/refpages/latest/refpages/source/VkFormat.html)
- `offset`: a vertex elejéhez képest mennyivel később kezdődnek az adott adatok

**Példa**

TODO: befejezni

## 3. VkPipelineVertexInputStateCreateInfo
- Ezt majd a `VkGraphicsPipelineCreateInfo` létrehozásánál kell megadni

**Definíció**
```cpp
typedef struct VkPipelineVertexInputStateCreateInfo {
    VkStructureType                             sType;
    const void*                                 pNext;
    VkPipelineVertexInputStateCreateFlags       flags;
    uint32_t                                    vertexBindingDescriptionCount;
    const VkVertexInputBindingDescription*      pVertexBindingDescriptions;
    uint32_t                                    vertexAttributeDescriptionCount;
    const VkVertexInputAttributeDescription*    pVertexAttributeDescriptions;
} VkPipelineVertexInputStateCreateInfo;
```
- `sType`: struktúra típusa, legyen `VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO`
- `pNext`: ha ki akarjuk még bővíteni valamivel, maradhat `nullptr`
- `flags`: fent van tartva jövőbeli használatra, maradjon `0`
- `vertexBindingDescriptionCount`: hány binding descriptiont hoztunk létre
- `pVertexBindingDescriptions`: a binding descriptionök tömbje (egy darabnál pointer vagy referencia)
- `vertexAttributeDescriptionCount`: az attribute descriptionök száma
- `pVertexAttributeDescriptions`: ezek tömbje

**Példa**

TODO: befejezni
