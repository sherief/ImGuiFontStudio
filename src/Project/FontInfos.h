/*
 * Copyright 2020 Stephane Cuillerdier (aka Aiekick)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <ConfigAbstract.h>
#include "tinyxml2/tinyxml2.h"

#include "GlyphInfos.h"

#include <imgui.h>
#include <string>
#include <set>
#include <map>
#include <unordered_map>

enum RasterizerEnum
{
	RASTERIZER_STB = 0,
	RASTERIZER_FREETYPE,
	RASTERIZER_Count
};

struct GlyphsRange
{
//	std::set<uint32_t> datas;
	int rangeStart = 0;
	int rangeEnd = 0;
};

class ProjectFile;
class FontInfos : public conf::ConfigAbstract
{
public:
	static RasterizerEnum rasterizerMode;
	static uint32_t freeTypeFlag;
	static float fontsMultiply;
	static int32_t fontsPadding;

public: // not to save
	ImFontAtlas m_ImFontAtlas;
	std::vector<std::string> m_GlyphNames;
	std::unordered_map<uint32_t, bool> m_GlyphHaveColor; // unordered_map because just for query
	std::unordered_map<uint32_t, std::string> m_GlyphCodePointToName; // unordered_map because just for query
	std::unordered_map<uint32_t, uint32_t> m_GlyphCodePointToGlyphIndex; // unordered_map because just for query
	char m_SearchBuffer[1024] = "\0";
	ImFontConfig m_FontConfig;
	bool m_NeedFilePathResolve = false; // the path is not found, need resolve for not lost glyphs datas
	bool m_NameInDoubleFound = false;
	bool m_CodePointInDoubleFound = false;
	std::map<uint32_t, std::vector<GlyphInfos*>> m_GlyphsOrderedByCodePoints; // map because need alpha order
	std::map<std::string, std::vector<GlyphInfos*>> m_GlyphsOrderedByGlyphName; // map because need alpha order
	int m_Ascent = 0;
	int m_Descent = 0;
	int m_LineGap = 0;
	ct::ivec4 m_BoundingBox;
	float m_Point = 0.0f;
	std::string m_FontFileName;
	
public: // to save
	std::unordered_map<uint32_t, GlyphInfos> m_SelectedGlyphs; // unordered_map because just for query
	std::string m_FontPrefix; // peut servir pour la generation par lot
	std::string m_FontFilePathName;
	int m_Oversample = 1;
	int m_FontSize = 17;
	std::set<std::string> m_Filters;
	
public: // callable
	bool LoadFont(ProjectFile *vProjectFile, const std::string& vFontFilePathName);
	void Clear();
	std::string GetGlyphName(uint32_t vCodePoint);
	void DrawInfos();

private: // Glyph Names Extraction / DB
	void FillGlyphNames();
	void GenerateCodePointToGlypNamesDB();
	void GetInfos();

private: // glyph color check
	void CheckIfGlyphHaveColor();

private: // Opengl Texture
	void CreateFontTexture();
	void DestroyFontTexture();

public: // Configuration
	std::string getXml(const std::string& vOffset);
	void setFromXml(tinyxml2::XMLElement* vElem, tinyxml2::XMLElement* vParent);

public: // Cons/Des tructors
	FontInfos();
	~FontInfos();
};


