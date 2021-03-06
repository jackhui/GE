#include <iostream>
#include <iomanip>
#include <fstream>
#include "Font.h"
#include "../Graphics/D3D11Renderer.h"

Font::Font()
{
	if (!m_Font && !LoadFontData("../Font/font_data.txt")) {
		OutputDebugStringW(L"Load font data error");
	}
}

void Font::write(char* sentence, float drawX, float drawY) {
	// Get the number of letters in the sentence.
	const int iNumLetters = (int) strlen(sentence);
	const int iNumVerts = iNumLetters * 6;
	const int iNumIndices = iNumVerts;

	const float textSpacing = 0.05f;
	const float textHeight = 1.0f;
	float textWidth = 0.0f;

	Vertex1P1UV* pVertices = new Vertex1P1UV[iNumVerts];
	unsigned int* pIndices = new unsigned int[iNumIndices];

	// Draw each letter onto a quad.
	for (int i = 0, letter = 0, index = 0; i < iNumLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;
		textWidth = m_Font[letter].size / (16.0f / textHeight);
		
		// If the letter is a space then just move over three pixels.
		if (letter == 0) {
			drawX = drawX - textWidth;
		} else {
			// First triangle in quad
			// Top left
			pVertices[index].m_pos = Vector3(drawX, drawY, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].left;
			pVertices[index].m_UV[1] = 0.0f;
			index++;

			// Bottom right
			pVertices[index].m_pos = Vector3(drawX - textWidth, drawY - textHeight, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].right;
			pVertices[index].m_UV[1] = 0.9f;
			index++;

			// Bottom left
			pVertices[index].m_pos = Vector3(drawX, drawY - textHeight, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].left;
			pVertices[index].m_UV[1] = 0.9f;
			index++;
		
			// Second triangle in quad
			// Top left
			pVertices[index].m_pos = Vector3(drawX, drawY, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].left;
			pVertices[index].m_UV[1] = 0.0f;
			index++;
			
			// Top right
			pVertices[index].m_pos = Vector3(drawX - textWidth, drawY, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].right;
			pVertices[index].m_UV[1] = 0.0f;
			index++;

			// Bottom right
			pVertices[index].m_pos = Vector3(drawX - textWidth, drawY - textHeight, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].right;
			pVertices[index].m_UV[1] = 0.9f;
			index++;

			// Update the x location for drawing by the size of the letter and one pixel
			drawX = drawX - (textWidth + textSpacing);
		}
	}

	for (int i = 0, j = 0; i < iNumIndices;) {
		pIndices[i++] = j++;
		pIndices[i++] = j++;
		pIndices[i++] = j++;
		pIndices[i++] = j++;
		pIndices[i++] = j++;
		pIndices[i++] = j++;
	}

	D3D11Renderer::GetInstance()->GetMeshInstanceList().push_back(
		new MeshInstance(pVertices, iNumVerts, pIndices, iNumIndices, RenderType::V1P1UV, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, "../Font/font.dds")
	);
}

bool Font::LoadFontData(char* filename) {
	std::ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	m_Font = new FontType[95];
	if(!m_Font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for(i=0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

Font::~Font()
{
	delete m_Font;
}
