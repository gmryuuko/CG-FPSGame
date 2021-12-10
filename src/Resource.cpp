#include "Resource.h"

#include "ObjLoader.h"
#include "stb_image.h"

using namespace std;

namespace Resource {

    unordered_map<string, Model*> models;
    unordered_map<string, Texture*> textures;

    Texture* LoadTexture(const string& path);

    // path是相对与directory的路径
    Texture* GetTexture(const string& path, const string& directory) {
        string file = directory + '/' + path;

        if (textures.find(file) != textures.end()) {
            return textures[file];
        }

        Texture* texture = LoadTexture(file);

        if (texture != nullptr) {
            textures[file] = texture;
        }

        return texture;
    }

    Model* GetModel(const string& path, const string& directory) {
        string file = directory + '/' + path;

        if (models.find(file) != models.end()) {
            return models[file];
        }

        Model* model = ObjLoader::LoadModel(file);

        if (model != nullptr) {
            models[file] = model;
        }

        return model;
    }

    Texture* LoadTexture(const string& path) {
        std::cout << "[Resource] Texture Loading: " << path << std::endl;
        stbi_set_flip_vertically_on_load(true);

        Texture *texture = new Texture();

        texture->path = path;
        glGenTextures(1, &texture->texId);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

        if (data) {
            std::cout << "\twidth:  " << width << std::endl;
            std::cout << "\theight: " << height << std::endl;
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, texture->texId);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else {
            // 加载失败后删除glTexture，Texture
            std::cerr << "[Resource] Texture failed to load at path: " << path << std::endl;
            
            glDeleteTextures(1, &texture->texId);
            delete texture;
            texture = nullptr;
            stbi_image_free(data);
        }

        return texture;
    }

}  // namespace Resource