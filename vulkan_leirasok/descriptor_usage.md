# Descriptorok használata

A Vulkan dokumentáció ezt mondja:

> A descriptor is an opaque data structure representing a shader resource such as a buffer, buffer view, image view, sampler, or combined image sampler. Descriptors are organized into descriptor sets, which are bound during command recording for use in subsequent drawing commands.

Tehát descriptorokkal lehet hivatkozni különböző erőforrásokra, amiket el lehet érni a shaderben. Ebben a példában egy uniform buffert fogunk használni.

A descriptorok setekben vannak csoportosítva, és az ugyanolyan elrendezésű descriptor seteket gyorsan tudja váltogatni a GPU.

## 1. VkDescriptorPool

A descriptor setek allokálásához a GPU memóriában egy descriptor poolra van szükség.

### Manuális használat

TODO: megírni

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

## 2. VkDescriptorSetLayoutBinding

TODO: leírás hozzáadása

### Manuális használat

Mivel ez csak egy darab struktúra, ezt a VkCourse-ban is manuálisan kell létrehozni

**Definíció**

```cpp
typedef struct VkDescriptorSetLayoutBinding {
	uint32_t           binding;
	VkDescriptorType   descriptorType;
	uint32_t           descriptorCount;
	VkShaderStageFlags stageFlags;
	const VkSampler*   pImmutableSamplers;
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
	.binding            = 0,
	.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	.descriptorCount    = 1,
	.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	.pImmutableSamplers = nullptr,
};
```

## 3. VkDescriptorSetLayout

### Használat manuálisan

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

## 4. VkDescriptorSet

TODO: leírás hozzáadása

### Használat manuálisan

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
