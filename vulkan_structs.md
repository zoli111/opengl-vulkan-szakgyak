### Vulkan struktúrák leírása
A struktúrák, és a függvények leírását (beleértve a függvényparamétereket) érdemes az interneten megkeresni

## Instance
- Ez az objektum köti össze az alkalmazás logikáját a Vulkannal, ezért általában ezt kell először létrehozni
- Meg kell adni többek közt az alkalmazás nevét, a használt Vulkan verziót, az esetlegesen használt game engine-t (ez alapján alkalmazhat egyéni optimalizációt a driver)
- Ezen keresztül lehet lekérdezni a fizikai eszközöket, és logikai eszközöket létrehozni (lásd device)
- Szintén itt lehet a támogatott instance-szintű vulkan kiterjesztéseket és layereket lekérdezni és bekapcsolni
---
- Típus: `VkInstance`
- Létrehozás: `vkCreateInstance()`
    - CreateInfo: `VkInstanceCreateInfo`
        - Mező: `pApplicationInfo`: ezt érdemes (TODO: kell?) kitölteni `VkApplicationInfo` típusú struktúrával
        - Mező: `enabledExtensionCount`: engedélyezett kiterjesztések száma
        - Mező: `ppEnabledExtensionNames`: engedélyezett kiterjesztések
- Megszüntetés: `vkDestroyInstance()`

## Physical device
- Ez az objektum reprezentálja a hardvert, amin Vulkan fut
- Lehet dedikált vagy integrált videokártya, CPU szoftveres rendereléssel, VM-ben virtuális GPU, egyéb gyorsító kártya...
- Különféle funkciók támogatottak, ezeket le lehet kérdezni, és ezek alapján lehet választani, hogy melyik eszköz legyen használva
	- Pl. hányas Vulkan verzió érhető el, mennyi a memória, elérhető függvények, mekkora képeket kezel, grafikus és/vagy más számításokat is tud...
---
- Típus: `VkPhysicalDevice`
- Listázás: `vkEnumeratePhysicalDevices()`

Tulajdonságok/funkciók lekérdezése
- Típus: `VkPhysicalDeviceProperties`
    - Lekérdezés: `vkGetPhysicalDeviceProperties()`
- Típus: `VkPhysicalDeviceFeatures`
    - Lekérdezés: `vkGetPhysicalDeviceFeatures()`

### Queue family
- A GPU-n végrehajtott műveleteket tudjuk a típusuk szerint külön kezelni
- Pl. grafikai, számítási, adattovábbítási műveletek
---
- Lekérdezés: `vkGetPhysicalDeviceQueueFamilyProperties()`

## Logical device
- Interfészt ad a fizikai eszközhöz
- Egy fizikai eszközhöz tartozhat több logikai eszköz is (hiszen több program is használhatja egyszerre a GPU-t)
---
- Típus: `VkDevice`
- Létrehozás: `vkCreateDevice`
    - CreateInfo: `VkDeviceCreateInfo`
        - Mező: `pEnabledFeatures`: engedélyezett funkciók, le kell kérni a fizikai eszköztől
            - Típus: `VkPhysicalDeviceFeatures`
            - Lekérdezés: `vkGetPhysicalDeviceFeatures()`
        - Mező: `pQueueCreateInfos`
        - Mező: `queueCreateInfoCount`

### Queue
- TODO: ide valami leírás
---
- Típus: `VkQueue`
- Lekérdezés: `vkGetDeviceQueue()`
