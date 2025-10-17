# Descriptorok használata

A Vulkan dokumentáció szépen összefoglalja:

> A descriptor is an opaque data structure representing a shader resource such as a buffer, buffer view, image view, sampler, or combined image sampler. Descriptors are organized into descriptor sets, which are bound during command recording for use in subsequent drawing commands.

Tehát descriptorokkal lehet hivatkozni különböző erőforrásokra, amiket el lehet érni a shaderben. Ebben a példában egy uniform buffert fogunk használni.

A descriptorok setekben vannak csoportosítva, és az ugyanolyan elrendezésű descriptor seteket gyorsan tudja váltogatni a GPU.

## 1. VkDescriptorPool

A descriptor setek allokálásához a GPU memóriában egy descriptor poolra van szükség.

### Használat a VkCourse keretrendszerrel

**Definíció**

```cpp
descPool.Create(
	VkDevice device,
	const std::unordered_map<VkDescriptorType, uint32_t>& countPerType,
	uint32_t maxSetCount
);
```

- `device`: logikai eszköz
- `countPerType`: milyen típusú descriptorból hányat szeretnénk (az összes setet beleértve, összesen)
  - [Descriptor típusok](https://registry.khronos.org/vulkan/specs/latest/man/html/VkDescriptorType.html)
- `maxSetCount`: összesen hány setet szeretnénk

**Példa**

```cpp
DescriptorPool descPool = DescriptorPool();
descPool.Create(
	device,
	{{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100}, {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100}},
	100
);
```
- Tipp (unordered) map-et lehet kapcsos zárójelekkel inicializálni
- Pl. itt lefoglalunk 100-100-at, és maximum 100 setet lehet létrehozni
  - Ez bőven több, mint amit használni fogunk

### Manuális használat

Először egy createInfo-t kell létrehozni hozzá

**Definíció**
```cpp
typedef struct VkDescriptorPoolCreateInfo {
	VkStructureType                sType;
	const void*                    pNext;
	VkDescriptorPoolCreateFlags    flags;
	uint32_t                       maxSets;
	uint32_t                       poolSizeCount;
	const VkDescriptorPoolSize*    pPoolSizes;
	} VkDescriptorPoolCreateInfo;
```
- `sType`: Vulkanban szokásos típusmeghatározás, itt legyen `VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO`
- `pNext`: nem használjuk, legyen nullptr
- `flags`: extra opciók, nem fogjuk használni
- `maxSets`: maximum hány descriptor setet tartalmazhat
- `poolSizeCount`: `VkDescriptorPoolSize` struktúrák száma
- `pPoolSizes`: `VkDescriptorPoolSize` struktúrák tömbje
	- C++-ban egyszerűbb vektort használni (lásd példa)

**Példa**
```cpp

std::vector<VkDescriptorPoolSize> descPoolSizes;
// Amennyit és amilyen típusú descriptort szeretnénk létrehozni
descPoolSizes.push_back(
	{
		.type               = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount    = 50,
	}
);
// ...

VkDescriptorPoolCreateInfo poolCreateInfoManual{
	.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.maxSets = 100,
	.poolSizeCount = static_cast<uint32_t>(descPoolSizes.size()),
	.pPoolSizes = descPoolSizes.data(),
}
```

Majd ha ez kész akkor megcsinálhatjuk magát a poolt

**Definíció**
```cpp
VkResult vkCreateDescriptorPool(
	VkDevice                             device,
	const VkDescriptorPoolCreateInfo*    pCreateInfo,
	const VkAllocationCallbacks*         pAllocator,
	VkDescriptorPool*                    pDescriptorPool
);
```

**Példa**
```cpp
VkDescriptorPool descPoolManual = VK_NULL_HANDLE;
VkResult result = vkCreateDescriptorPool(
	VkDevice                             device,
	const VkDescriptorPoolCreateInfo*    poolCreateInfoManual,
	const VkAllocationCallbacks*         nullptr,
	VkDescriptorPool*                    descPoolManual
);
```

## 2. VkDescriptorSetLayoutBinding

TODO: leírás hozzáadása

### Manuális használat

Mivel ez csak egy darab struktúra, ezt a VkCourse-ban is manuálisan kell létrehozni

**Definíció**

```cpp
typedef struct VkDescriptorSetLayoutBinding {
	uint32_t              binding;
	VkDescriptorType      descriptorType;
	uint32_t              descriptorCount;
	VkShaderStageFlags    stageFlags;
	const VkSampler*      pImmutableSamplers;
} VkDescriptorSetLayoutBinding;
```

- `binding`: a shaderben kell megadni, hogy melyik binding-ból kell az adott erőforrás
	- Pl.: `layout (set = 0, binding = 1)`
- `descriptorType`: descriptorok típusa
- `descriptorCount`: descriptorok száma
- `stageFlags`: a rajzolás melyik fázisában érhető el
	- [Stage-ek listája](https://registry.khronos.org/vulkan/specs/latest/man/html/VkShaderStageFlagBits.html)
	- Ha több is kell, akkor bitenként össze kell ÉS-elni őket (|)
- `pImmutableSamplers`: ha sampler típusú a descriptor, itt meg lehet adni olyan samplereket, amiket utána nem lehet változtatni
	- Általában ezt nem fogjuk használni (TODO: ez valóban így van?)


**Példa**

```cpp
VkDescriptorSetLayoutBinding descSetLayoutBinding{
	.binding               = 0,
	.descriptorType        = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	.descriptorCount       = 1,
	.stageFlags            = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	.pImmutableSamplers    = nullptr,
};
```

## 3. VkDescriptorSetLayout

TODO: leírás hozzáadása

### Használat a VkCourse keretrendszerrel

A `DescriptorPool` osztálynak van függvénye a létrehozásához

**Definíció**

```cpp
VkDescriptorSetLayout DescriptorPool::createLayout(
	const std::vector<VkDescriptorSetLayoutBinding>& bindings
);
```
- A DescriptorPool elmenti, hogy milyen layout bindingokat kapott, és ha van ugyanolyan, akkor azt adja vissza

**Példa**

```cpp
VkDescriptorSetLayout descSetLayout = descPool.createLayout(
	{descSetLayoutBinding}
);
```
- Tipp: itt is működik a kapcsos zárójeles inicializáció

### Használat manuálisan

**Definíció**
```cpp
typedef struct VkDescriptorSetLayoutCreateInfo {
	VkStructureType                        sType;
	const void*                            pNext;
	VkDescriptorSetLayoutCreateFlags       flags;
	uint32_t                               bindingCount;
	const VkDescriptorSetLayoutBinding*    pBindings;
} VkDescriptorSetLayoutCreateInfo;
```
- `sType`: legyen `VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO`
- `pNext`: nem használjuk, legyen nullptr
- `flags`: extra opciók, legyen 0
- `bindingCount`: hány darab VkDescriptorSetLayoutBinding jön ide
- `pBindings`: bindingok tömbje

**Példa**

```cpp
vector<VkDescriptorSetLayoutBinding> bindings;

VkDescriptorSetLayoutCreateInfo layoutInfo{
    .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext        = nullptr,
    .flags        = 0,
    .bindingCount = 1,
    .pBindings    = {descSetLayoutBinding},
};
```
- Mivel a példában nyilván egy bindingot használunk, így 1 lesz a `bindingCount`

Majd létre tudjuk hozni a layoutot

**Definíció**
```cpp
VkResult vkCreateDescriptorSetLayout(
	VkDevice                                  device,
	const VkDescriptorSetLayoutCreateInfo*    pCreateInfo,
	const VkAllocationCallbacks*              pAllocator,
	VkDescriptorSetLayout*                    pSetLayout);
```
- `device`: logikai eszköz
- `pCreateInfo`: a createInfo, amit korábban létrehoztunk
- `pAllocator`: legyen nullptr
- `pSetLayout`: referencia a változóra, ami a layout handle-t fogja tartalmazni

**Példa**
```cpp
VkDescriptorSetLayout layout = VK_NULL_HANDLE;
VkResult result = vkCreateDescriptorSetLayout(
	device,
	&layoutInfo,
	nullptr,
	&layout
);
```

## 4. VkDescriptorSet

TODO: leírás hozzáadása

### Használat a VkCourse keretrendszerrel

Szintén a DescriptorPool segítségével hozható létre

**Definíció**

```cpp
DescriptorSetMgmt DescriptorPool::createSet(
	VkDescriptorSetLayout layout
);
```
- `layout`: pl. amit korábban létrehoztunk
- DescriptorSetMgmt objektumot ad vissza, ami wrappeli a VkDescriptorSet-et

**Példa**

```cpp
DescriptorSetMgmt descSet = descPool.createSet(
	VkDescriptorSetLayout descSetLayout
);
```

### Használat manuálisan

Ellőször kell egy allocateInfo-t létrehozni

**Definíció**
```cpp
typedef struct VkDescriptorSetAllocateInfo {
	VkStructureType                 sType;
	const void*                     pNext;
	VkDescriptorPool                descriptorPool;
	uint32_t                        descriptorSetCount;
	const VkDescriptorSetLayout*    pSetLayouts;
} VkDescriptorSetAllocateInfo;
```
- `sType`: legyen VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO
- `pNext`: legyen csak nullptr
- `descriptorPool`: a használt descriptor pool
- `descriptorSetCount`: hány setet akarunk létrehozni
- `pSetLayouts`: referencia/tömb a desciptor set layout-okra

**Példa**
```cpp
VkDescriptorSetAllocateInfo allocInfo{
.sType                 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
.pNext                 = nullptr,
.descriptorPool        = descPoolManual,
.descriptorSetCount    = 1,
.pSetLayouts           = &layout,
};
```

Majd létrehozhatjuk a set(ek)et is

**Definíció**
```cpp
VkResult vkAllocateDescriptorSets(
	VkDevice                              device,
	const VkDescriptorSetAllocateInfo*    pAllocateInfo,
	VkDescriptorSet*                      pDescriptorSets
);
```
- `device`: logikai eszköz
- `pAllocateInfo`: az allocInfo, amit létrehoztunk korábban
- `pDescriptorSets`: egy (vagy egy tömbnyi) VkDescriptorSet handle


**Példa**
```cpp
VkDescriptorSet descSetManual = VK_NULL_HANDLE;
VkResult result = vkAllocateDescriptorSets(
	device,
	&allocInfo,
	&descSetManual
);
```
