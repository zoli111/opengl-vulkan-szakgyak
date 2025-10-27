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
  - [Descriptor típusok](https://docs.vulkan.org/refpages/latest/refpages/source/VkDescriptorType.html)
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

#### CreateInfo létrehozása

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

VkDescriptorPoolCreateInfo poolCreateInfo{
	.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.maxSets = 100,
	.poolSizeCount = static_cast<uint32_t>(descPoolSizes.size()),
	.pPoolSizes = descPoolSizes.data(),
}
```

#### DescriptorPool létrehozása

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
VkDescriptorPool descPool = VK_NULL_HANDLE;
VkResult result = vkCreateDescriptorPool(
	VkDevice                             device,
	const VkDescriptorPoolCreateInfo*    poolCreateInfo,
	const VkAllocationCallbacks*         nullptr,
	VkDescriptorPool*                    descPool
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
	- [Stage-ek listája](https://docs.vulkan.org/refpages/latest/refpages/source/VkShaderStageFlagBits.html)
	- Ha több is kell, akkor bitenként össze kell VAGY-olni őket (|)
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

#### DescriptorSetLayout létrehozása

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

#### AllocInfo létrehozása

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
.descriptorPool        = descPool,
.descriptorSetCount    = 1,
.pSetLayouts           = &layout,
};
```

#### DescriptorSetek létrehozása

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
VkDescriptorSet descSet = VK_NULL_HANDLE;
VkResult result = vkAllocateDescriptorSets(
	device,
	&allocInfo,
	&descSet
);
```

## 5. Buffer/memória

### Használat a VkCourse keretrendszerrel

#### Létrehozás

**Definíció**
```cpp
BufferInfo BufferInfo::Create(
	const VkPhysicalDevice phyDevice,
	const VkDevice device,
	VkDeviceSize size,
	VkBufferUsageFlags usageFlags
);
```
- `phyDevice`: fizikai eszköz
- `device`: logikai eszköz
- `size`: méret (bájtban), tipp: `sizeof()`
- `usageFlags`: [Flagek](https://docs.vulkan.org/refpages/latest/refpages/source/VkBufferUsageFlagBits.html)

**Példa**
```cpp
BufferInfo buffer;
buffer = BufferInfo::Create(phyDevice, device, sizeof(glm::mat4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
```

#### Feltöltés adatokkal

**Definíció**
```cpp
void Update(
	const VkDevice device,
	const void* inputPtr,
	size_t size
);
```
- `device`: logikai eszköz
- `inputPtr`: referencia vagy pointer arra a C++ objektumra, aminek az adataival töltjük fel
- `size`: adatok mérete, `sizeof()`

**Példa**
```cpp
glm::mat4 matrix = glm::mat4(1.0f); // ezt transzformálhatjuk, ahogy akarjuk
buffer.Update(device, &matrix, sizeof(matrix));
```

### Használat manuálisan

#### Buffer createInfo létrehozása

```cpp
typedef struct VkBufferCreateInfo {
	VkStructureType        sType;
	const void*            pNext;
	VkBufferCreateFlags    flags;
	VkDeviceSize           size;
	VkBufferUsageFlags     usage;
	VkSharingMode          sharingMode;
	uint32_t               queueFamilyIndexCount;
	const uint32_t*        pQueueFamilyIndices;
} VkBufferCreateInfo;
```
- `sType`: VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO
- `pNext`: extra funkciók, legyen nullptr
- `flags`: [Flagek](https://docs.vulkan.org/refpages/latest/refpages/source/VkBufferUsageFlagBits.html)
- `size`: a leendő buffer mérete
- `usage`: mire lesz használva a buffer
	- [Használat](https://docs.vulkan.org/refpages/latest/refpages/source/VkBufferUsageFlagBits.html)
	- Bitenként össze kell VAGY-olni
- `sharingMode`: [Leírás](https://docs.vulkan.org/refpages/latest/refpages/source/VkSharingMode.html)
- `queueFamilyIndexCount`: csak akkor kell, ha több queue-t használunk
- `pQueueFamilyIndices`: szintén

**Példa**
```cpp
glm::mat4 matrix = glm::mat4(1.0f);

VkBufferCreateInfo bufferCreateInfo{
	.sType                    = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	.pNext                    = nullptr,
	.flags                    = 0,
	.size                     = sizeof(matrix),
	.usage                    = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
	.sharingMode              = VK_SHARING_MODE_EXCLUSIVE,
	.queueFamilyIndexCount    = 0,
	.pQueueFamilyIndices      = nullptr,
};
```

#### Buffer létrehozása

**Definíció**
```cpp
VkResult vkCreateBuffer(
	VkDevice                        device,
	const VkBufferCreateInfo*       pCreateInfo,
	const VkAllocationCallbacks*    pAllocator,
	VkBuffer*                       pBuffer
);
```
- `device`: logikai eszköz
- `pCreateInfo`: a létrehozott createInfo
- `pAllocator`: legyen nullptr, de ha valamilyen más memóriaallokátort akarunk használni akkor ott van
- `pBuffer`: ebbe a handle-be kerül a buffer

**Példa**
```cpp
VkBuffer buffer = VK_NULL_HANDLE;
VkResult result = vkCreateBuffer(
	device,
	bufferCreateInfo,
	nullptr,
	&buffer
);
```

#### BufferMemoryRequirements megszerzése

**Definíció**
```cpp
void vkGetBufferMemoryRequirements(
	VkDevice                 device,
	VkBuffer                 buffer,
	VkMemoryRequirements*    pMemoryRequirements
);
```

**Példa**
```cpp
VkMemoryRequirements memRequirements = VK_NULL_HANDLE;
vkGetBufferMemoryRequirements(
	device,
	buffer,
	&memRequirements
);
```

#### Memória allocInfo létrehozása

**Definíció**
```cpp
typedef struct VkMemoryAllocateInfo {
	VkStructureType    sType;
	const void*        pNext;
	VkDeviceSize       allocationSize;
	uint32_t           memoryTypeIndex;
} VkMemoryAllocateInfo;
```
- `sType`: legyen `VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO`
- `pNext`: extra funkciók, maradhat nullptr
- `allocationSize`: a VkMemoryRequirements-ből le kell kérni
- `memoryTypeIndex`: szintén a VkMemoryRequirements-ből kell lekérni
	- Kicsit bonyolult, a Vulkan Tutorialból van rá példa függvény
	- [Memória típusok](https://docs.vulkan.org/refpages/latest/refpages/source/VkMemoryPropertyFlagBits.html)

**Példa**
```cpp
// memoryTypeIndex megszerzésére való függvény
// Forrás: Vulkan Tutorial
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	throw std::runtime_error("failed to find suitable memory type!");
}

VkMemoryAllocateInfo allocInfo{
	sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	pNext              = nullptr,
	allocationSize     = memRequirements.size,
	memoryTypeIndex    = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);,
};
```

#### Memória lefoglalása

**Definíció**
```cpp
VkResult vkAllocateMemory(
	VkDevice                         device,
	const VkMemoryAllocateInfo*      pAllocateInfo,
	const VkAllocationCallbacks*     pAllocator,
	VkDeviceMemory*                  pMemory
);
```
- `device`: logikai eszköz
- `pAllocateInfo`: A korábban létrehozott allocInfo
- `pAllocator`: Ha más allokátort akarunk használni, itt maradhat nullptr
- `pMemory`: A handle, amivel később hivatkozhatunk rá

**Példa**
```cpp
VkDeviceMemory memory = VK_NULL_HANDLE;
VkResult result = vkAllocateMemory(device, allocInfo, nullptr, &memory);
```

#### Adatok feltöltése a memóriába

**Definíció**
```cpp
VkResult vkMapMemory(
	VkDevice            device,
	VkDeviceMemory      memory,
	VkDeviceSize        offset,
	VkDeviceSize        size,
	VkMemoryMapFlags    flags,
	void**              ppData
);
```
- `device`: logikai eszköz
- `memory`: a memória objektum
- `offset`: offset a memória elejétől bájtban
- `size`: mennyi memóriát mappelünk
	- Pl. ha egy buffernél a buffer `createInfo`-jából le lehet kérni
- `flags`: legyen 0
- `ppData`: void* objektum, amivel elérhetjük a mappelt memóriaterületet

**Példa**
```cpp
void* data;
vkMapMemory(device, memory, 0, bufferCreateInfo.size, 0, &data);
// Most már átmásolhatjuk az adatokat
memcpy(data, &matrix, sizeof(matrix));
vkUnmapMemory(device, memory);
```

#### Buffer összekötése a memóriával

**Definíció**
```cpp
VkResult vkBindBufferMemory(
	VkDevice          device,
	VkBuffer          buffer,
	VkDeviceMemory    memory,
	VkDeviceSize      memoryOffset
);
```
- `device`: logikai eszköz
- `buffer`: a buffer, amit használni akarunk
- `memory`: a lefoglalt `VkDeviceMemory` objektum
- `memoryOffset`: offset bájtokban, a memory elejétől kezdve

**Példa**
```cpp
VkResult result = vkBindBufferMemory(device, buffer, memory, 0);
```
