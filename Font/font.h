#pragma once
#include <d3d11.h>
#include "VertexFormat.h"

struct FontType
{
	float left, right;
	int size;
};

class Font
{
	public:
		Font();
		~Font();

		void write(char* sentence, float drawX, float drawY);

	private:
		bool LoadFontData(char*);
		FontType* m_Font;
};

