#ifndef HELLOWORLD_HELLOTRIANGLEAPPLICATION_HPP
#define HELLOWORLD_HELLOTRIANGLEAPPLICATION_HPP
#pragma once
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

class HelloTriangleApplication {

public:
  const uint32_t WIDTH = 800;
  const uint32_t HEIGHT = 600;

  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  VkInstance instance;
  void initWindow() {
    if (!glfwInit()) {
      throw std::runtime_error("failed to initialize GLFW!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    if (!window) {
      glfwTerminate();
      throw std::runtime_error("failed to create window!");
    }

    // enum all devices
    // create logical device
    VkDevice device;
    VkPhysicalDevice physicalDevice;
  }
  void initVulkan() { createInstance(); }
  void mainLoop() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      // if resize
      // recreate swapchain
    }
  }
  void cleanup() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
  }
  void createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to create instance!");
    }

    std::vector<const char *> requiredExtensions;

    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
      requiredExtensions.emplace_back(glfwExtensions[i]);
    }

    requiredExtensions.emplace_back(
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
      throw std::runtime_error("failed to create instance!");
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                           extensions.data());

    std::cout << "available extensions:\n";

    for (const auto &extension : extensions) {
      std::cout << '\t' << extension.extensionName << '\n';
    }
  }
};

#endif