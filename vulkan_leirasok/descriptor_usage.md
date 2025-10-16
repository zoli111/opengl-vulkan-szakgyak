# Descriptorok használata

A Vulkan dokumentáció ezt mondja:

> A descriptor is an opaque data structure representing a shader resource such as a buffer, buffer view, image view, sampler, or combined image sampler. Descriptors are organized into descriptor sets, which are bound during command recording for use in subsequent drawing commands.

Tehát descriptorokkal lehet hivatkozni különböző erőforrásokra, amiket el lehet érni a shaderben. Ebben a példában egy buffert fogunk használni.

## 1. VkDescriptorPool

A descriptor setek allokálásához a GPU memóriában egy descriptor poolra van szükség.

### Létrehozás manuálisan

TODO: megírni

### Létrehozás a VkCourse keretrendszerrel

```cpp
DescriptorPool descPool = DescriptorPool();
descPool.Create(
    // Logikai eszköz
    VkDevice device,
    // Milyen típusú descriptorból maximum hányat tárolhat, pl.:
    // {{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100}, {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100}}
    const std::unordered_map<VkDescriptorType, uint32_t>& countPerType,
    // Összesen hány descriptor*Set*et tárolhat
    uint32_t maxSetCount
);
```

[Descriptor típusok](https://registry.khronos.org/vulkan/specs/latest/man/html/VkDescriptorType.html)

## 2. VkDescriptorSetLayoutBinding

TODO: leírás hozzáadása

### Létrehozás

Egy ilyen struktúrát kell létrehozni

```cpp
typedef struct VkDescriptorSetLayoutBinding {
    // TODO: ez micsoda
    uint32_t              binding;
    // Descriptor típusa
    VkDescriptorType      descriptorType;
    // Darabszáma
    uint32_t              descriptorCount;
    // Melyik stage-ben legyen elérhető a descriptor
    // Ezeket bitenként össze kell vagyolni, ha több van, pl.:
    // VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
    VkShaderStageFlags    stageFlags;
    // TODO: nem tudom, hogy ezt használjuk-e, legyen nullptr?
    const VkSampler*      pImmutableSamplers;
} VkDescriptorSetLayoutBinding;
```

[Az elérhető stage-ek listája](https://registry.khronos.org/vulkan/specs/latest/man/html/VkShaderStageFlagBits.html)

## 3. VkDescriptorSetLayout

### Létrehozás manuálisan

TODO: leírás hozzáadása

### Létrehozás a VkCourse keretrendszerrel

A DescriptorPool wrapper osztálynak van függvénye a létrehozásához

```cpp
DescriptorPool::createLayout(
    // VkDescriptorSetLayoutBinding-ok listája
    const std::vector<VkDescriptorSetLayoutBinding>& bindings
);
```

## 4. VkDescriptorSet

### Létrehozás manuálisan

TODO: leírás hozzáadása

### Létrehozás a VkCourse keretrendszerrel

Szintén a DescriptorPool segítségével hozható létre

```cpp
DescriptorPool::createSet(
    // A korábban létrehozott VkDescriptorSetLayout-ot kell megadni
    VkDescriptorSetLayout layout
);
```
