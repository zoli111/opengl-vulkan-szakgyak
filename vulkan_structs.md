### Vulkan struktúrák leírása

A struktúrák, és a függvények leírását (beleértve a függvényparamétereket) érdemes az interneten megkeresni

## Instance

- Ez az objektum köti össze az alkalmazás logikáját a Vulkannal, ezért általában ezt kell először létrehozni
- Meg kell adni többek közt az alkalmazás nevét, a használt Vulkan verziót, az esetlegesen használt game engine-t (ez alapján alkalmazhat egyéni optimalizációt a driver)
- Ezen keresztül lehet lekérdezni a fizikai eszközöket, és logikai eszközöket létrehozni (lásd device)
- Szintén itt lehet a támogatott instance-szintű vulkan kiterjesztéseket és layereket lekérdezni és bekapcsolni

---

- Típus: `VkInstance`
- Létrehozás: [vkCreateInstance()](https://registry.khronos.org/vulkan/specs/latest/man/html/vkCreateInstance.html)
	- CreateInfo: [VkInstanceCreateInfo](https://registry.khronos.org/vulkan/specs/latest/man/html/VkInstanceCreateInfo.html)
		- Mező: `pApplicationInfo`: ezt kell kitölteni [VkApplicationInfo](https://registry.khronos.org/vulkan/specs/latest/man/html/VkApplicationInfo.html) típusú struktúrával
		- Mező: `enabledExtensionCount`: engedélyezett kiterjesztések száma
		- Mező: `ppEnabledExtensionNames`: engedélyezett kiterjesztések
- Megszüntetés: [vkDestroyInstance()](https://registry.khronos.org/vulkan/specs/latest/man/html/vkDestroyInstance.html)

```cpp
// Az adott operációs rendszertől függően más extensionök kellenek
// Ezt szerencsére automatikusan le tudja kérni a glfw
uint32_t glfwExtensionCount = 0;
const char** glfwExtensions;
glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
```

## Physical device

- Ez az objektum reprezentálja a hardvert, amin Vulkan fut
- Lehet dedikált vagy integrált videokártya, CPU szoftveres rendereléssel, VM-ben virtuális GPU, egyéb gyorsítókártya...
- Már csak azok az eszközök kerülnek listázásra, amelyek támogatják az instance-ban megadott Vulkan verziót
- Különféle funkciók támogatottak, ezeket le lehet kérdezni, és ezek alapján lehet választani, hogy melyik eszköz legyen használva
	- Pl. mennyi a memória, elérhető függvények, mekkora képeket kezel, grafikus és/vagy más számításokat is tud...

---

- Típus: `VkPhysicalDevice`
- Listázás: [vkEnumeratePhysicalDevices()](https://registry.khronos.org/vulkan/specs/latest/man/html/vkEnumeratePhysicalDevices.html)
- Tulajdonságok/funkciók lekérdezése
	- Típus: [VkPhysicalDeviceProperties](https://registry.khronos.org/vulkan/specs/latest/man/html/VkPhysicalDeviceProperties.html)
		- Lekérdezés: [vkGetPhysicalDeviceProperties()](https://registry.khronos.org/vulkan/specs/latest/man/html/vkGetPhysicalDeviceProperties.html)
	- Típus: [VkPhysicalDeviceFeatures](https://registry.khronos.org/vulkan/specs/latest/man/html/VkPhysicalDeviceFeatures.html)
		- Lekérdezés: [vkGetPhysicalDeviceFeatures()](https://registry.khronos.org/vulkan/specs/latest/man/html/vkGetPhysicalDeviceFeatures.html)

```cpp
// vkEnumeratePhysicalDevices() használata
// Egy nullpointerrel lekérjük az eszközök számát
uint32_t deviceCount = 0;
vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
// Majd egy előre allokált helyre mentjük az eszközöket
std::vector<VkPhysicalDevice> devices(deviceCount);
vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
// Utána végigiterálhatunk rajtuk, és választhatunk megfelelőt
```

### Queue family

- A GPU-n végrehajtott műveleteket tudjuk a típusuk szerint külön kezelni
- Pl. grafikai, számítási, adattovábbítási műveletek

---

- Lekérdezés: [vkGetPhysicalDeviceQueueFamilyProperties()](https://registry.khronos.org/vulkan/specs/latest/man/html/vkGetPhysicalDeviceQueueFamilyProperties.html)

## Logical device

- Mindig létre kell hozni
- Interfészt ad a fizikai eszközhöz
- Egy fizikai eszközhöz tartozhat több logikai eszköz is (hiszen több program is használhatja egyszerre a GPU-t)

---

- Típus: `VkDevice`
- Létrehozás: [vkCreateDevice](https://registry.khronos.org/vulkan/specs/latest/man/html/vkCreateDevice.html)
	- CreateInfo: [VkDeviceCreateInfo](https://registry.khronos.org/vulkan/specs/latest/man/html/VkDeviceCreateInfo.html)
		- Mező: `pEnabledFeatures`: engedélyezett funkciók, le kell kérni a fizikai eszköztől
			- Típus: [VkPhysicalDeviceFeatures](https://registry.khronos.org/vulkan/specs/latest/man/html/VkPhysicalDeviceFeatures.html)
			- Lekérdezés: [vkGetPhysicalDeviceFeatures()](https://registry.khronos.org/vulkan/specs/latest/man/html/vkGetPhysicalDeviceFeatures.html)
		- Mező: `pQueueCreateInfos`
		- Mező: `queueCreateInfoCount`

```cpp
// A synchronization 2 és a dynamic rendering nagyban leegyszerűsíti a grafikus pipeline létrehozását, ezért kapcsoljuk be
VkPhysicalDeviceSynchronization2Features syncFeatures = {
	.sType              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
	.pNext              = nullptr,
	.synchronization2   = VK_TRUE,
};
VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRendering = {
	.sType              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
	.pNext              = &syncFeatures,
	.dynamicRendering   = VK_TRUE,
};
// Majd amikor létrehozzuk a device-t
VkDeviceCreateInfo createInfo = {
	.pNext = &dynamicRendering
	// ...
}
```

```cpp
// Gyorsaság miatt nagyon minimális hibakezelés van Vulkanban, ezért ha szeretnénk többet megtudni,
// hogy miért nem működik a program, engedélyeznünk kell a validation layereket
const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"
};
// A device létrehozásánál
VkDeviceCreateInfo createInfo = {
	.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	.ppEnabledLayerNames = validationLayers.data();
	// ...
}
```

### Queue

- TODO: ide valami leírás

---

- Típus: `VkQueue`
- Lekérdezés: [vkGetDeviceQueue()](https://registry.khronos.org/vulkan/specs/latest/man/html/vkGetDeviceQueue.html)
