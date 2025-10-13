#pragma once
#include <iostream>
#include "GL/glew.h"
#include "stb_image/stb_image.h"

class Texture
{
private:
	unsigned int m_renderId;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_bpp;
public:
    Texture(const std::string& path)
    {
    m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);
    
    stbi_set_flip_vertically_on_load(1);
    glGenTextures(1, &m_renderId);
    glBindTexture(GL_TEXTURE_2D, m_renderId);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    if (m_localBuffer) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
    }
    else {
        std::cout << "error" << std::endl;
    }
    stbi_image_free(m_localBuffer);
}
	~Texture()
    {
        glDeleteTextures(1, &m_renderId);
    }


    void bind(unsigned int slot = 0) const
    {
        glBindTexture(GL_TEXTURE_2D, m_renderId);
    }
    void unBind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
	inline int getBPP() const { return m_bpp; }
    
};