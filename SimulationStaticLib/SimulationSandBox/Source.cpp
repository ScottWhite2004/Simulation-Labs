#define GLM_ENABLE_EXPERIMENTAL
#include "Application.h"
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>


//==================================================
// Vulkan ver 1.3 based
//====================================================

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <optional>
#include <set>
#include <cmath>

//Created Class Includes
#include "CameraManager.h"
#include "Vertex.h"
#include "Material.h"
#include "textureManager.h"
#include "Light.h"
#include "GraphicsPipelineBuilder.h"
#include "VulkanContext.h"
#include "QueueFamilyIndices.h"
#include "WindowManager.h"
#include "SwapChainManager.h"
#include "RenderPassManager.h"
#include "SyncManager.h"
#include "PipelineManager.h"
#include "Scenario.h"
#include "Sphere.h"
#include "ClearScenario.h"
#include "AngularDisplacement.h"
#include "AngularVelocity.h"
#include "Spring.h"
#include "PhysicsWorld.h"
#include "WorldObject.h"
#include "WorldObjectManager.h"
#include "Capsule.h"
#include "Scene.h"
#include "FlatBufferLoader.h"
#include "SpawnerSystem.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct TimeUBO
{
    float time;
    float pad0;
    float pad1;
    float pad2;
};

class HelloTriangleApplication {
public:
    HelloTriangleApplication()
        : _clearScenario(&uiClearColour)
		, _physicsWorld(gravity), _worldObjectManager(&_physicsWorld)
    {
    }

    void run();

private:
	//Management Classes
    VulkanContext _vulkanContext;
    WindowManager _windowManager;
    SwapChainManager _swapChainManager;
    RenderPassManager _renderPassManager;
    SyncManager _syncManager;
    PipelineManager _pipelineManager;
	CameraManager _cameraManager;
    textureManager _texManager;
	Image _imageHelper;
	Material _defaultMaterial;
    std::vector<VkDescriptorBufferInfo> _lightingBufferInfos;
    size_t _uploadedWorldObjectCount = 0;

	//Scenario Management
	ClearScenario _clearScenario;
	AngularDisplacement _angularDisplacementScenario;
	AngularVelocity _angularVelocityScenario;

    uint32_t currentFrame = 0;

    //Colour Controlled From ImGui
	ImVec4 uiClearColour = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 displacement = glm::vec3(0.0f);
	float displacementRadians = 0.0f;

    //Timestep controlled by imgui
    float simulationTimeStep = 0.0f;

	//Stop and starting simulation controlled by imgui
	bool simulationRunning = false;

	bool framebufferResized = false;

	float simulationTimeAculator = 0.0f;

    //Simulation Metrics
    float renderInterpolationAlpha = 1.0f;

    //Camera controls
    float cameraMoveSpeed = 4.0f;
    float cameraLookSpeed = 1.8f;

    //Scene loader from flat buffers
	Scene scene;
    FlatBufferLoader loader;

    //Depth Resources
	VkImage depthImage = VK_NULL_HANDLE;
	VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
	VkImageView depthImageView = VK_NULL_HANDLE;

    // --- Buffers and Memory ---
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    std::vector<VkBuffer> lightingUniformBuffers;
    std::vector<VkDeviceMemory> lightingUniformBuffersMemory;
    std::vector<void*> lightingUniformBuffersMapped;

    // --- Descriptors ---
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets;

    // --- Synchronization ---
    std::vector<VkCommandBuffer> commandBuffers;

    // --- ImGui ---
	VkDescriptorPool imguiDescriptorPool = VK_NULL_HANDLE;

    //Cloth Settings
	std::vector<Sphere> _clothParticles;
	std::vector<Spring> _clothSprings;
	std::vector<uint32_t> _clothIndices;
	std::vector<Vertex> _clothVertices;
	RigidBody _testRigidBody;

    //Physics
	PhysicsWorld _physicsWorld;
	WorldObjectManager _worldObjectManager;
	std::unique_ptr<SpawnerSystem> _spawnerSystem;


    //World Forces
	glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);

	int _selectedWorldObjectIndex = -1;


    // --- Main Flow ---
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void createGraphicsPipeline();

    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createCommandBuffers();

    // --- ImGui steps ---
    void initImGui();
    void cleanupImGui();
	void drawWorldObjectUI();

	//Depth resources
	void createDepthResources();

	Camera perspectiveCamera;
	Camera orthographicCamera;



    // --- Drawing and Swapchain Handling ---
    void drawFrame();
    void recreateSwapChain();
    void cleanupSwapChain();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void updateUniformBuffer(uint32_t currentImage);

    //Cloth Creation

	//void createCloth(int width, int height, float spacing, const Material& material);

    uint32_t getClothParticleIndex(int x, int y, int width) {
        return y * width + x;
	}


    // --- Helper Functions ---
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    std::vector<const char*> getRequiredExtensions();
    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    void uploadNewWorldObjects();

    // --- Callbacks ---
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    //Simulation control
    void update(float seconds);
    void updateCameraControls(float dt);
    
};

// --- Implementation ---

void HelloTriangleApplication::run() {
	_windowManager.initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initVulkan() {

    _vulkanContext.initializeContext(_windowManager.getWindow());
    _swapChainManager.initialize(&_vulkanContext, &_windowManager);
    _swapChainManager.createSwapChain();
    _swapChainManager.createImageViews();

    const VkExtent2D extent = _swapChainManager.getExtent();
    const float aspect = (extent.height > 0)
        ? static_cast<float>(extent.width) / static_cast<float>(extent.height)
        : 1.0f;

    perspectiveCamera = Camera(
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        90.0f,
        aspect,
        0.1f,
        300.0f,
        10.0f,
        ProjectionType::Perspective
    );

    orthographicCamera = Camera(
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        90.0f,
        aspect,
        0.1f,
        300.0f,
        10.0f,
        ProjectionType::Orthographic
    );

    _cameraManager.addCamera("Perspective", perspectiveCamera);
    _cameraManager.addCamera("Orthographic", orthographicCamera);
    _cameraManager.switchToCamera("Perspective"); // Make the first camera active
    // ---------------------------------------------

    _renderPassManager.initialize(&_vulkanContext, &_swapChainManager);
    _renderPassManager.createRenderPass();
    _pipelineManager.initialize(&_vulkanContext, &_renderPassManager, &_swapChainManager);

    VkDescriptorSetLayoutBinding uboBinding{};
    uboBinding.binding = 0;
    uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboBinding.descriptorCount = 1;
    uboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding lightingBinding{};
    lightingBinding.binding = 2;
    lightingBinding.descriptorCount = 1;
    lightingBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    lightingBinding.pImmutableSamplers = nullptr;
    lightingBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; // or BOTH depending on your shader

    std::array<VkDescriptorSetLayoutBinding, 3> bindings = { uboBinding, samplerLayoutBinding, lightingBinding };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    _vulkanContext.createDescriptorSetLayout(layoutInfo);

    std::vector<VkDescriptorSetLayout> setLayouts;
    setLayouts.push_back(_vulkanContext.getDescriptorSetLayout());
    _pipelineManager.createPipelineLayout(setLayouts);

    _imageHelper = Image(_vulkanContext.getDevice(), _vulkanContext.getPhysicalDevice(), _vulkanContext.getCommandPool(), _vulkanContext.getGraphicsQueue());

    createGraphicsPipeline();
    _vulkanContext.createCommandPool();
    _texManager.initialize(_vulkanContext.getDevice(), _vulkanContext.getPhysicalDevice(), _vulkanContext.getCommandPool(), _vulkanContext.getGraphicsQueue());
    _texManager.addTexture("default", "textures/Default.png");
    createDepthResources();
    _swapChainManager.createFramebuffers(_renderPassManager.getRenderPass(), depthImageView);
    _syncManager.initialize(&_vulkanContext, &_swapChainManager, MAX_FRAMES_IN_FLIGHT);

    _defaultMaterial = Material(glm::vec4(1.0f), 0.0f, 1.0f, _texManager.getTexture("default"));

    if (loader.loadSceneFromFile("scenes/newtonsCradle.bin", scene)) {
        scene.createWorldObjects(_worldObjectManager, _defaultMaterial);
    }

	//_worldObjectManager.addPlane("Ground", glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(10.0f, 1.0f, 10.0f), 10.0f,10.0f, _defaultMaterial, glm::vec3(0.0f), 0.0f);

    const glm::vec3 spawnerCenter(0.0f, 5.0f, 0.0f);
    const float spawnerRadius = 5.0f;
    const float containerSize = spawnerRadius * 2.0f;

    {
        WorldObject* container = _worldObjectManager.addCuboid(
            "ContainmentBox",
            spawnerCenter,
            glm::vec3(0.0f),
            glm::vec3(1.0f),
            containerSize,
            containerSize,
            containerSize,
            _defaultMaterial,
            glm::vec3(0.0f),
            1.0f);

        if (container != nullptr) {
            container->setCollisionType(CollisionType::CONTAINER);
            if (container->getRigidBody() != nullptr) {
                container->getRigidBody()->SetStatic(true);
            }
        }
    }

    // Test cylinder spawner (repeating)
    {
        SceneSpawner testSpawner;
        testSpawner.type = SpawnerType::SphereSpawner;

        testSpawner.sphere.base.name = "TestCylinderSpawner";
        testSpawner.sphere.base.start_time = 0.0f;
        testSpawner.sphere.base.spawn_type = SpawnType::RepeatingSpawn;
        testSpawner.sphere.base.repeating_spawn.max_count = 100;
        testSpawner.sphere.base.repeating_spawn.interval = 1.0f;

        testSpawner.sphere.base.location = SpawnLocation::RandomSphere;
        testSpawner.sphere.base.random_sphere.center = spawnerCenter;
        testSpawner.sphere.base.random_sphere.radius = spawnerRadius;

        testSpawner.sphere.base.linear_velocity.min = glm::vec3(0.0f);
        testSpawner.sphere.base.linear_velocity.max = glm::vec3(0.0f);
        testSpawner.sphere.base.angular_velocity.min = glm::vec3(0.0f);
        testSpawner.sphere.base.angular_velocity.max = glm::vec3(0.0f);

		testSpawner.sphere.radius_range.min = 0.5f;
		testSpawner.sphere.radius_range.max = 1.0f;

        scene.spawners.push_back(testSpawner);
    }

    _spawnerSystem = std::make_unique<SpawnerSystem>(scene, _worldObjectManager, _defaultMaterial);

    _physicsWorld.setGravity(gravity);

    createUniformBuffers();
    createDescriptorPool();

    _lightingBufferInfos.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        _lightingBufferInfos[i].buffer = lightingUniformBuffers[i];
        _lightingBufferInfos[i].offset = 0;
        _lightingBufferInfos[i].range = sizeof(LightingUBO);
    }

    _worldObjectManager.uploadWorldObjects(
        _vulkanContext,
        MAX_FRAMES_IN_FLIGHT,
        _defaultMaterial.getTextureImageView(),
        _defaultMaterial.getTextureSampler(),
        _lightingBufferInfos
    );
    _uploadedWorldObjectCount = _worldObjectManager.getWorldObjects().size();

    createDescriptorSets();
    createCommandBuffers();
    initImGui();
}

void HelloTriangleApplication::drawWorldObjectUI()
{
    ImGui::Begin("World Object Inspector");

    const std::vector<WorldObject*>& worldObjects = _worldObjectManager.getWorldObjects();

    if (worldObjects.empty()) {
        _selectedWorldObjectIndex = -1;
        ImGui::TextUnformatted("No world objects.");
        ImGui::End();
        return;
    }

    if (_selectedWorldObjectIndex < 0) {
        _selectedWorldObjectIndex = 0;
    }
    if (_selectedWorldObjectIndex >= static_cast<int>(worldObjects.size())) {
        _selectedWorldObjectIndex = static_cast<int>(worldObjects.size()) - 1;
    }

    ImGui::BeginChild("WorldObjectList", ImVec2(230.0f, 0.0f), true);
    for (size_t i = 0; i < worldObjects.size(); ++i) {
        const bool isSelected = (_selectedWorldObjectIndex == static_cast<int>(i));
        const std::string label = worldObjects[i]->getName() + "##" + std::to_string(i);
        if (ImGui::Selectable(label.c_str(), isSelected)) {
            _selectedWorldObjectIndex = static_cast<int>(i);
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginGroup();

    WorldObject* selected = worldObjects[_selectedWorldObjectIndex];
    RigidBody* rigidBody = selected->getRigidBody();
    Collider* collider = selected->getCollider();
    Shape* shape = selected->getShape();

    ImGui::Text("Name: %s", selected->getName().c_str());

    if (rigidBody != nullptr) {
        glm::vec3 rigidBodyPos = rigidBody->getPos();
        if (ImGui::DragFloat3("RigidBody Position", &rigidBodyPos.x, 0.05f)) {
            rigidBody->setPos(rigidBodyPos);
            if (collider != nullptr) {
                collider->GetTransform().setPosition(rigidBodyPos);
            }
            if (shape != nullptr) {
                shape->setPosition(rigidBodyPos);
            }
            selected->syncTransform();
        }

        glm::vec3 velocity = rigidBody->getVelocity();
        if (ImGui::DragFloat3("Velocity", &velocity.x, 0.05f)) {
            rigidBody->setVelocity(velocity);
        }

        glm::vec3 angularVelocity = rigidBody->getAngularVelocity();
        if (ImGui::DragFloat3("Angular Velocity", &angularVelocity.x, 0.05f)) {
            rigidBody->setAngularVelocity(angularVelocity);
        }

        float mass = rigidBody->getMass();
        if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.001f, 10000.0f, "%.3f")) {
            rigidBody->SetMass(mass);
            if (collider != nullptr) {
                rigidBody->CalculateInertia();
            }
        }

        bool isStatic = rigidBody->isStatic();
        if (ImGui::Checkbox("Static", &isStatic)) {
            rigidBody->SetStatic(isStatic);
        }

        if (ImGui::Button("Zero Velocity")) {
            rigidBody->setVelocity(glm::vec3(0.0f));
            rigidBody->setAngularVelocity(glm::vec3(0.0f));
        }
    }
    else {
        ImGui::TextUnformatted("No rigid body attached.");
    }

    if (collider != nullptr) {
        glm::vec3 colliderPos = collider->GetTransform().getPosition();
        if (ImGui::DragFloat3("Collider Position", &colliderPos.x, 0.05f)) {
            collider->GetTransform().setPosition(colliderPos);
            if (rigidBody != nullptr) {
                rigidBody->setPos(colliderPos);
            }
            if (shape != nullptr) {
                shape->setPosition(colliderPos);
            }
            selected->syncTransform();
        }
    }
    else {
        ImGui::TextUnformatted("No collider attached.");
    }

    if (shape != nullptr) {
        glm::vec3 renderPos = shape->getPos();
        if (ImGui::DragFloat3("Rendering Position", &renderPos.x, 0.05f)) {
            shape->setPosition(renderPos);
            if (rigidBody != nullptr) {
                rigidBody->setPos(renderPos);
            }
            if (collider != nullptr) {
                collider->GetTransform().setPosition(renderPos);
            }
        }
    }
    else {
        ImGui::TextUnformatted("No shape attached.");
    }

    ImGui::EndGroup();
    ImGui::End();
}


//void HelloTriangleApplication::createCloth(int width, int height, float spacing, const Material& material)
//{
//    if (width <= 1 || height <= 1 || spacing <= 0.0f) {
//        return;
//    }
//
//    for (auto& particle : _clothParticles) {
//        particle.destroy(_vulkanContext);
//    }
//
//    _clothParticles.clear();
//    _clothSprings.clear();
//
//    const size_t particleCount = static_cast<size_t>(width) * static_cast<size_t>(height);
//
//    const size_t structuralCount =
//        static_cast<size_t>(width - 1) * static_cast<size_t>(height) +
//        static_cast<size_t>(width) * static_cast<size_t>(height - 1);
//
//    const size_t shearCount =
//        2ull * static_cast<size_t>(width - 1) * static_cast<size_t>(height - 1);
//
//    // Axial bend + diagonal bend (flexion)
//    const size_t bendAxialCount =
//        static_cast<size_t>(std::max(0, width - 2)) * static_cast<size_t>(height) +
//        static_cast<size_t>(width) * static_cast<size_t>(std::max(0, height - 2));
//
//    const size_t bendDiagonalCount =
//        2ull * static_cast<size_t>(std::max(0, width - 2)) * static_cast<size_t>(std::max(0, height - 2));
//
//    _clothParticles.reserve(particleCount);
//    _clothSprings.reserve(structuralCount + shearCount + bendAxialCount + bendDiagonalCount);
//
//    const glm::vec3 origin(
//        -0.5f * static_cast<float>(width - 1) * spacing,
//        4.0f,
//        0.0f);
//
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            const glm::vec3 p = origin + glm::vec3(x * spacing, -y * spacing, 0.0f);
//
//            Sphere particle(
//                p,
//                glm::vec3(0.0f),
//                glm::vec3(0.0f),
//                SphereCollider(glm::vec3(0.0f), 0.07f),
//                0.1f,
//                material,
//                0.07f);
//
//            particle.create();
//
//            if ((y == 0 && x == 0) || (y == 0 && x == width - 1)) {
//                particle.SetStatic(true);
//            }
//
//            _clothParticles.emplace_back(std::move(particle));
//        }
//    }
//
//    auto addSpring = [&](uint32_t a, uint32_t b, float k, float d) {
//        const float rest = glm::distance(_clothParticles[a].getPos(), _clothParticles[b].getPos());
//        _clothSprings.emplace_back(&_clothParticles[a], &_clothParticles[b], rest, k, d);
//        };
//
//    // Tunables
//    const float kStructural = 3.0f;
//    const float dStructural = 1.2f;
//    const float kShear = 2.0f;
//    const float dShear = 1.0f;
//    const float kFlexion = 1.0f; // lower than structural
//    const float dFlexion = 0.7f;
//
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            const uint32_t i = getClothParticleIndex(x, y, width);
//
//            // Structural
//            if (x + 1 < width)  addSpring(i, getClothParticleIndex(x + 1, y, width), kStructural, dStructural);
//            if (y + 1 < height) addSpring(i, getClothParticleIndex(x, y + 1, width), kStructural, dStructural);
//
//            // Shear
//            if (x + 1 < width && y + 1 < height) addSpring(i, getClothParticleIndex(x + 1, y + 1, width), kShear, dShear);
//            if (x - 1 >= 0 && y + 1 < height)    addSpring(i, getClothParticleIndex(x - 1, y + 1, width), kShear, dShear);
//
//            // Flexion (axial 2-hop)
//            if (x + 2 < width)  addSpring(i, getClothParticleIndex(x + 2, y, width), kFlexion, dFlexion);
//            if (y + 2 < height) addSpring(i, getClothParticleIndex(x, y + 2, width), kFlexion, dFlexion);
//
//            // Flexion (diagonal 2-hop) - improves fold resistance
//            if (x + 2 < width && y + 2 < height) addSpring(i, getClothParticleIndex(x + 2, y + 2, width), kFlexion, dFlexion);
//            if (x - 2 >= 0 && y + 2 < height)    addSpring(i, getClothParticleIndex(x - 2, y + 2, width), kFlexion, dFlexion);
//        }
//    }
//}

void HelloTriangleApplication::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    // Create descriptor pool for ImGui (image samplers)
    VkDescriptorPoolSize poolSizes[] = {
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000;
    poolInfo.poolSizeCount = static_cast<uint32_t>(sizeof(poolSizes) / sizeof(poolSizes[0]));
    poolInfo.pPoolSizes = poolSizes;

    if (vkCreateDescriptorPool(_vulkanContext.getDevice(), &poolInfo, nullptr, &imguiDescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create ImGui descriptor pool!");
    }

    ImGui_ImplGlfw_InitForVulkan(_windowManager.getWindow(), true);

    ImGui_ImplVulkan_InitInfo initInfo{};
    initInfo.ApiVersion = VK_API_VERSION_1_3;
    initInfo.Instance = _vulkanContext.getInstance();
    initInfo.PhysicalDevice = _vulkanContext.getPhysicalDevice();
    initInfo.Device = _vulkanContext.getDevice();
    initInfo.QueueFamily = _vulkanContext.findQueueFamilies(_vulkanContext.getPhysicalDevice()).graphicsFamily.value();
    initInfo.Queue = _vulkanContext.getGraphicsQueue();
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = imguiDescriptorPool;
    initInfo.DescriptorPoolSize = 0; // because we provided DescriptorPool
    initInfo.MinImageCount = static_cast<uint32_t>(_swapChainManager.getImageCount());
    initInfo.ImageCount = static_cast<uint32_t>(_swapChainManager.getImageCount());
    initInfo.Allocator = nullptr;
    initInfo.CheckVkResultFn = nullptr;

    // Use classic render pass instead of dynamic rendering
    initInfo.PipelineInfoMain.RenderPass = _renderPassManager.getRenderPass();
    initInfo.PipelineInfoMain.Subpass = 0;
    // Optionally: initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&initInfo);
}

void HelloTriangleApplication::cleanupImGui() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (imguiDescriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(_vulkanContext.getDevice(), imguiDescriptorPool, nullptr);
        imguiDescriptorPool = VK_NULL_HANDLE;
    }
}

void HelloTriangleApplication::update(float seconds)
{
    if (_spawnerSystem)
    {
		_spawnerSystem->update(seconds);
		uploadNewWorldObjects();
    }
    _physicsWorld.step(seconds);
	_worldObjectManager.capturePhysicsState();
}

void HelloTriangleApplication::updateCameraControls(float dt)
{
    if (dt <= 0.0f) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    GLFWwindow* window = _windowManager.getWindow();

    // --- Keyboard movement (WASD + QE) ---
    if (!io.WantCaptureKeyboard) {
        const float moveStep = cameraMoveSpeed * dt;

        float right = 0.0f;
        float forward = 0.0f;
        float up = 0.0f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) forward += moveStep;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) forward -= moveStep;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) right += moveStep;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) right -= moveStep;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) up += moveStep;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) up -= moveStep;

        if (right != 0.0f || forward != 0.0f || up != 0.0f) {
            _cameraManager.panCurrentCamera(right, forward, up);
        }
    }

    // --- Mouse rotation (hold Right Mouse Button) ---
    static bool rotating = false;
    static double lastX = 0.0;
    static double lastY = 0.0;

    if (!io.WantCaptureMouse && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double mouseX = 0.0;
        double mouseY = 0.0;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        if (!rotating) {
            rotating = true;
            lastX = mouseX;
            lastY = mouseY;
        }
        else {
            const double deltaX = mouseX - lastX;
            const double deltaY = mouseY - lastY;

            // Sensitivity: tune as needed.
            const float mouseLookSensitivity = 0.0025f * cameraLookSpeed;
            const float yaw = static_cast<float>(-deltaX) * mouseLookSensitivity;
            const float pitch = static_cast<float>(-deltaY) * mouseLookSensitivity;

            if (yaw != 0.0f || pitch != 0.0f) {
                _cameraManager.rotateCurrentCamera(yaw, pitch);
            }

            lastX = mouseX;
            lastY = mouseY;
        }
    }
    else {
        rotating = false;
    }
}

void HelloTriangleApplication::mainLoop() {
    using clock = std::chrono::high_resolution_clock;
    auto previousTime = clock::now();

    while (!glfwWindowShouldClose(_windowManager.getWindow())) {
        glfwPollEvents();
        auto currentTime = clock::now();
        float frameSeconds =
            std::chrono::duration<float>(currentTime - previousTime).count();
        previousTime = currentTime;

        updateCameraControls(frameSeconds);

        float dt = simulationTimeStep > 0.0f ? simulationTimeStep : frameSeconds;

        if (simulationRunning && simulationTimeStep > 0.0f) {
            simulationTimeAculator += frameSeconds;

            const float maxAccumulatedTime = simulationTimeStep * 8.0f;
            if (simulationTimeAculator > maxAccumulatedTime) {
                simulationTimeAculator = maxAccumulatedTime;
            }

            const float maxSubstep = 1.0f / 120.0f;
            const float fixedStep = simulationTimeStep;
            const int subSteps = std::max(1, static_cast<int>(std::ceil(fixedStep / maxSubstep)));
            const float subStep = fixedStep / static_cast<float>(subSteps);

            while (simulationTimeAculator >= fixedStep) {
                if(_spawnerSystem) {
                    _spawnerSystem->update(fixedStep);
					uploadNewWorldObjects();
				}
                for (int step = 0; step < subSteps; ++step) {
                    _physicsWorld.step(subStep);
                }
                _worldObjectManager.capturePhysicsState();
                simulationTimeAculator -= fixedStep;
            }

            renderInterpolationAlpha = simulationTimeAculator / fixedStep;
        }
        else if (simulationRunning && simulationTimeStep <= 0.0f) {
            update(dt);
            renderInterpolationAlpha = 1.0f;
        }
        else {
            renderInterpolationAlpha = 1.0f;
        }

        if (renderInterpolationAlpha < 0.0f) {
            renderInterpolationAlpha = 0.0f;
        }
        if (renderInterpolationAlpha > 1.0f) {
            renderInterpolationAlpha = 1.0f;
        }

        drawFrame();
    }
    vkDeviceWaitIdle(_vulkanContext.getDevice());
}

void HelloTriangleApplication::cleanup() {
    cleanupSwapChain();
	
    _renderPassManager.destroyRenderPass();

	cleanupImGui();

    for (auto& particle : _clothParticles) {
		particle.destroy(_vulkanContext);
	}

    _worldObjectManager.destroyWorldObjects(_vulkanContext);


    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(_vulkanContext.getDevice(), uniformBuffers[i], nullptr);
        vkFreeMemory(_vulkanContext.getDevice(), uniformBuffersMemory[i], nullptr);

        vkDestroyBuffer(_vulkanContext.getDevice(), lightingUniformBuffers[i], nullptr);
        vkFreeMemory(_vulkanContext.getDevice(), lightingUniformBuffersMemory[i], nullptr);
    }
    
    vkDestroyDescriptorPool(_vulkanContext.getDevice(), _vulkanContext.getDescriptorPool(), nullptr);
	vkDestroyDescriptorSetLayout(_vulkanContext.getDevice(), _vulkanContext.getDescriptorSetLayout(), nullptr);
	
    _syncManager.cleanup();

	vkDestroyCommandPool(_vulkanContext.getDevice(), _vulkanContext.getCommandPool(), nullptr);
	_windowManager.destroyWindow();
}

void HelloTriangleApplication::createDepthResources()
{
    VkFormat depthFormat = findDepthFormat();
	_imageHelper.createImage(_swapChainManager.getExtent().width, _swapChainManager.getExtent().height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = _imageHelper.createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void HelloTriangleApplication::createGraphicsPipeline() {
    auto vertShaderCode = readFile("shaders/vert.spv");
    auto fragShaderCode = readFile("shaders/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    // Optional: if PipelineManager builds input state for you, you may not need this.
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // Use PipelineManager; name the pipeline "Phong"
    _pipelineManager.createGraphicsPipeline(
        "Pipeline",
        vertShaderModule,
        fragShaderModule,
        &vertexInputInfo,
        /*enableDepth*/ true,
        /*enableBlending*/ false);

    vkDestroyShaderModule(_vulkanContext.getDevice(), fragShaderModule, nullptr);
    vkDestroyShaderModule(_vulkanContext.getDevice(), vertShaderModule, nullptr);
}

void HelloTriangleApplication::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    VkDeviceSize lightBufferSize = sizeof(LightingUBO);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    lightingUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    lightingUniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    lightingUniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        // Transform uniform buffer
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
        vkMapMemory(_vulkanContext.getDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);

        // Lighting uniform buffer
        createBuffer(lightBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, lightingUniformBuffers[i], lightingUniformBuffersMemory[i]);
        vkMapMemory(_vulkanContext.getDevice(), lightingUniformBuffersMemory[i], 0, lightBufferSize, 0, &lightingUniformBuffersMapped[i]);
    }
}

void HelloTriangleApplication::createDescriptorPool() {
    std::array<VkDescriptorPoolSize, 2> poolSizes{};

    // We need enough Uniform Buffers and Samplers for our scene objects
    // Bumping limits to comfortably support the triangle and the sphere
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 1000000);

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 1000000);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();

    // Give the pool plenty of available sets
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 1000000);
    _vulkanContext.createDescriptorPool(poolInfo);
}

void HelloTriangleApplication::createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, _vulkanContext.getDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _vulkanContext.getDescriptorPool();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(_vulkanContext.getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo transformBufferInfo{};
        transformBufferInfo.buffer = uniformBuffers[i];
        transformBufferInfo.offset = 0;
        transformBufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo samplerInfo{};
        samplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        samplerInfo.imageView = _defaultMaterial.getTextureImageView();
        samplerInfo.sampler = _defaultMaterial.getTextureSampler();

        VkDescriptorBufferInfo lightingBufferInfo{};
        lightingBufferInfo.buffer = lightingUniformBuffers[i];
        lightingBufferInfo.offset = 0;
        lightingBufferInfo.range = sizeof(LightingUBO);

        std::array<VkWriteDescriptorSet, 3> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &transformBufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &samplerInfo;

        descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[2].dstSet = descriptorSets[i];
        descriptorWrites[2].dstBinding = 2;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pBufferInfo = &lightingBufferInfo;

        vkUpdateDescriptorSets(
            _vulkanContext.getDevice(),
            static_cast<uint32_t>(descriptorWrites.size()),
            descriptorWrites.data(),
            0,
            nullptr);
    }
}

void HelloTriangleApplication::uploadNewWorldObjects()
{
    const std::vector<WorldObject*>& worldObjects = _worldObjectManager.getWorldObjects();
    if (worldObjects.size() <= _uploadedWorldObjectCount) {
        return;
    }

    for (size_t i = _uploadedWorldObjectCount; i < worldObjects.size(); ++i) {
        WorldObject* obj = worldObjects[i];
        Shape* shape = (obj != nullptr) ? obj->getShape() : nullptr;
        if (shape != nullptr) {
            shape->upload(
                _vulkanContext,
                MAX_FRAMES_IN_FLIGHT,
                _defaultMaterial.getTextureImageView(),
                _defaultMaterial.getTextureSampler(),
                _lightingBufferInfos);
        }
    }

    _uploadedWorldObjectCount = worldObjects.size();
}

void HelloTriangleApplication::createCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _vulkanContext.getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
    if (vkAllocateCommandBuffers(_vulkanContext.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

void HelloTriangleApplication::drawFrame() {

	_syncManager.waitForFrame(currentFrame);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_vulkanContext.getDevice(), _swapChainManager.getSwapChain(), UINT64_MAX, _syncManager.getImageAvailableSemaphores()[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _windowManager.isFramebufferResized()) {
        _windowManager.resetFramebufferResized();
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    _syncManager.waitForFrame(currentFrame);
    
	_syncManager.markImageInFlight(imageIndex, currentFrame);
	_syncManager.resetFrameFence(currentFrame);

	_worldObjectManager.applyInterpolation(renderInterpolationAlpha);
    updateUniformBuffer(currentFrame);

    // Start ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Scenario"))
        {

			ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Colour"))
        {
			ImGui::ColorPicker4("Clear Colour", (float*)&uiClearColour);
			ImGui::EndMenu();
        }
        const std::string cameraMenuLabel = "Camera: " + _cameraManager.getCurrentCameraName();
        if (ImGui::BeginMenu(cameraMenuLabel.c_str()))
        {
            if (ImGui::MenuItem("Reset Local Camera"))
            {
				_cameraManager.resetLocalCamera();
            }
            const std::unordered_map<std::string, Camera>& cameras = _cameraManager.getCameras();
            const std::string& currentName = _cameraManager.getCurrentCameraName();

            for (std::unordered_map<std::string, Camera>::const_iterator it = cameras.begin(); it != cameras.end(); ++it)
            {
                const std::string& name = it->first;
                const bool isSelected = (name == currentName);

                if (ImGui::MenuItem(name.c_str(), nullptr, isSelected))
                {
                    _cameraManager.switchToCamera(name);
                }
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Material"))
        {
			ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Simulation"))
        {
            if (ImGui::Button(simulationRunning ? "Stop" : "Start"))
            {
				simulationRunning = !simulationRunning;
            }
			ImGui::SameLine();
			if(ImGui::Button("Step Forward"))
            {
                if(simulationTimeStep > 0.0f)
                {
                    update(simulationTimeStep);
				}
            }
			ImGui::InputFloat("timestep", &simulationTimeStep);

            ImGui::Separator();
            ImGui::TextUnformatted("Physics");

            if (ImGui::DragFloat3("Gravity", &gravity.x, 0.1f, -50.0f, 50.0f, "%.2f"))
            {
                _physicsWorld.setGravity(gravity);
            }

            if (ImGui::Button("Reset Gravity"))
            {
                gravity = glm::vec3(0.0f, -9.8f, 0.0f);
                _physicsWorld.setGravity(gravity);
            }
            ImGui::EndMenu();
        }
		drawWorldObjectUI();
		ImGui::EndMainMenuBar();
    }

	ImGui::Render();

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { _syncManager.getImageAvailableSemaphores()[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = { _syncManager.getImagePresentSemaphores()[imageIndex] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(_vulkanContext.getGraphicsQueue(), 1, &submitInfo, _syncManager.getInFlightFences()[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { _swapChainManager.getSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(_vulkanContext.getGraphicsQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApplication::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(_windowManager.getWindow(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(_windowManager.getWindow(), &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(_vulkanContext.getDevice());
    cleanupSwapChain();

    _swapChainManager.createSwapChain();

    // inside HelloTriangleApplication::recreateSwapChain(), after _swapChainManager.createSwapChain()
    const VkExtent2D extent = _swapChainManager.getExtent();
    if (extent.height > 0) {
        _cameraManager.setCurrentCameraAspect(static_cast<float>(extent.width) / static_cast<float>(extent.height));
    }
    _windowManager.resetFramebufferResized();
    _swapChainManager.createImageViews();

    _renderPassManager.createRenderPass();
	createDepthResources();
	_swapChainManager.createFramebuffers(_renderPassManager.getRenderPass(),depthImageView);
    _syncManager.createPerImageSemaphores();
    createCommandBuffers();
    // Recreate descriptor pools before allocating sets
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 10);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 10);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 10);

    _vulkanContext.createDescriptorPool(poolInfo);
    createDescriptorPool();
    createDescriptorSets();

	cleanupImGui();
	initImGui();
}

void HelloTriangleApplication::cleanupSwapChain() {
    
    if (depthImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(_vulkanContext.getDevice(), depthImageView, nullptr);
        depthImageView = VK_NULL_HANDLE;
    }
    if (depthImage != VK_NULL_HANDLE) {
        vkDestroyImage(_vulkanContext.getDevice(), depthImage, nullptr);
        depthImage = VK_NULL_HANDLE;
    }
    if (depthImageMemory != VK_NULL_HANDLE) {
        vkFreeMemory(_vulkanContext.getDevice(), depthImageMemory, nullptr);
        depthImageMemory = VK_NULL_HANDLE;
    }

    if (!commandBuffers.empty()) {
        vkFreeCommandBuffers(
            _vulkanContext.getDevice(),
            _vulkanContext.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    _swapChainManager.cleanup();
}

void HelloTriangleApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // or VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT if desired
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkClearValue clearValues[2]{};
    clearValues[0].color = { { uiClearColour.x, uiClearColour.y, uiClearColour.z, uiClearColour.w } };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _renderPassManager.getRenderPass();
    renderPassInfo.framebuffer = _swapChainManager.getSwapChainFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = _swapChainManager.getExtent();
    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Set viewport/scissor (can be static or dynamic)
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_swapChainManager.getExtent().width);
    viewport.height = static_cast<float>(_swapChainManager.getExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = _swapChainManager.getExtent();
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineManager.getPipeline("Pipeline"));
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        _pipelineManager.getPipelineLayout(),
        0, 1, &descriptorSets[currentFrame],
        0, nullptr);

 //   for(auto & particle : _clothParticles)
 //   {
 //       particle.draw(commandBuffer, _pipelineManager.getPipeline("Pipeline"), _pipelineManager.getPipelineLayout(), currentFrame);
	//}
    _worldObjectManager.drawWorldObjects(
        commandBuffer,
        _pipelineManager.getPipeline("Pipeline"),
        _pipelineManager.getPipelineLayout(),
        currentFrame
    );

    // Render ImGui inside the same render pass
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer, VK_NULL_HANDLE);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float>(currentTime - startTime).count();

    Camera localCamera = _cameraManager.getLocalCamera();
    UniformBufferObject ubo{};
	ubo.model = glm::mat4(1.0f);
    ubo.view = localCamera.getViewMatrix();
    ubo.proj = localCamera.getProjectionMatrix();
    ubo.proj[1][1] *= -1;

    LightingUBO lightUBO{};
    lightUBO.lightCount = 1;
    lightUBO.lights[0].type = static_cast<uint32_t>(LightType::Directional);
    lightUBO.lights[0].direction = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
    lightUBO.lights[0].color = glm::vec3(1.0f, 1.0f, 1.0f);
    lightUBO.lights[0].ambient = 0.5f;
    lightUBO.lights[0].specular = 1.0f;
    lightUBO.viewPosWorld = localCamera.getEye();
    lightUBO.shininess = 32.0f;

    memcpy(lightingUniformBuffersMapped[currentImage], &lightUBO, sizeof(lightUBO));

 //   for(auto & particle : _clothParticles)
 //   {
	//	const glm::mat4 translation = glm::translate(glm::mat4(1.0f), particle.getPos());
 //      const glm::mat4 rotation = glm::mat4_cast(glm::normalize(particle.getRot()));
	//	const glm::mat4 model = translation * rotation;
 //       particle.updateUniformBuffer(currentImage, model, ubo.view, ubo.proj);
	//}
    _worldObjectManager.updateUniformBuffers(currentImage, ubo.view, ubo.proj);

}

// --- Helper Implementations ---

std::vector<char> HelloTriangleApplication::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

VkShaderModule HelloTriangleApplication::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(_vulkanContext.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
}

void HelloTriangleApplication::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(_vulkanContext.getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_vulkanContext.getDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(_vulkanContext.getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(_vulkanContext.getDevice(), buffer, bufferMemory, 0);
}

void HelloTriangleApplication::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = _vulkanContext.getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_vulkanContext.getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_vulkanContext.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_vulkanContext.getGraphicsQueue());
    vkFreeCommandBuffers(_vulkanContext.getDevice(), _vulkanContext.getCommandPool(), 1, &commandBuffer);
}

uint32_t HelloTriangleApplication::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_vulkanContext.getPhysicalDevice(), &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

VkFormat HelloTriangleApplication::findDepthFormat() {
    return findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkFormat HelloTriangleApplication::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(_vulkanContext.getPhysicalDevice(), format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }
    throw std::runtime_error("failed to find supported format!");
}


// --- Callback Implementations ---

void HelloTriangleApplication::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}


int main() {
    HelloTriangleApplication app;
    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
