#include "ResourceManager.hpp"

std::shared_ptr<ResourceManager> ResourceManager::instance_(nullptr);

void ResourceManager::Initialize(const std::string& base_dir) {
    if (instance_ == nullptr) {
        instance_ =
            std::shared_ptr<ResourceManager>(new ResourceManager(base_dir));
    }
}

std::shared_ptr<ResourceManager> ResourceManager::GetInstance() {
    return ResourceManager::instance_;
}

sf::Texture* ResourceManager::LoadTexture(const std::string& name) {
    auto it = textures_.find(name);
    if (it != textures_.end()) {
        return &(it->second);
    } else {
        sf::Texture texture;
        if (!texture.loadFromFile(name)) {
            return nullptr;
        }
        textures_[name] = std::move(texture);
        return &textures_[name];
    }

    return nullptr;
}

sf::Font* ResourceManager::LoadFont(const std::string& name) {
    auto it = fonts_.find(name);
    if (it != fonts_.end()) {
        return &(it->second);
    } else {
        sf::Font font;
        if (!font.loadFromFile(name)) {
            return nullptr;
        }
        fonts_[name] = std::move(font);
        return &fonts_[name];
    }

    return nullptr;
}
