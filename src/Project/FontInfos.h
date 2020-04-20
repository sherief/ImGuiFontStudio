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

struct GlyphsRange
{
//	std::set<ImWchar> datas;
	int rangeStart = 0;
	int rangeEnd = 0;
};

struct HeaderFileCPPStruct
{
	bool isOk = false;
	std::string prefix;
	std::string bufferName;
	size_t bufferSize = 0;
	size_t rangeMin = 0;
	size_t rangeMax = 0;
	std::map<std::string, size_t> database; // glyph name, codepoint
};
struct SourceFileCPPStruct
{
	bool isOk = false;
	std::string prefix;
	std::string bufferName;
	std::string buffer;
};

class ProjectFile;
class FontInfos : public conf::ConfigAbstract
{
public: // not to save
	ImFontAtlas m_ImFontAtlas;
	std::vector<std::string> m_GlyphNames;
	std::map<ImWchar, std::string> m_GlyphCodePointNames;
	char m_SearchBuffer[1024] = "\0";
	ImFontConfig m_FontConfig;
	bool m_NeedFilePathResolve = false; // the path is not found, need resolve for not lost glyphs datas
	std::map<ImWchar, std::vector<GlyphInfos*>> m_GlyphsOrderedByCodePoints;
	std::map<std::string, std::vector<GlyphInfos*>> m_GlyphsOrderedByGlyphName;
	int m_Ascent = 0;
	int m_Descent = 0;
	int m_LineGap = 0;
	ct::ivec4 m_BoundingBox;
	float m_Point = 0.0f;
	std::string m_FontFileName;
	
public: // not to save // errors => will full or partially block generation
	bool m_NameInDoubleFound = false;
	bool m_CodePointInDoubleFound = false;

public: // to save
	std::map<ImWchar, GlyphInfos> m_SelectedGlyphs;
	std::string m_FontPrefix; // peut servir pour la generation par lot
	std::string m_FontFilePathName;
	int m_Oversample = 1;
	int m_FontSize = 17;
	std::set<std::string> m_Filters; // use map just for have binary tree search
	
public: // callable
	bool LoadFont(ProjectFile *vProjectFile, const std::string& vFontFilePathName);
	void Clear();
	std::string GetGlyphName(ImWchar vCodePoint);
	void DrawInfos();

private: // Loader
	bool Load_TTF_OTF_FontFile(ProjectFile *vProjectFile, const std::string& vFontFilePathName);
	bool Load_CPP_Source_File(ProjectFile *vProjectFile, const std::string& vFontFilePathName);

private: // Cpp / h Helper
	HeaderFileCPPStruct ParseHeaderFile_CPP(const std::string& vHeaderFile);
	SourceFileCPPStruct ParseSourceFile_CPP(const std::string& vSourceFile, HeaderFileCPPStruct vHeader);
	size_t ParseCodePoint(const std::string vBuffer, size_t vLastLoc, const std::string& vKeyToSearch,
		std::string *vTag, size_t *vNum, bool vConvertUTF8);

private: // Glyph Names Extraction / DB
	void FillGlyphNames();
	void GenerateCodePointToGlypNamesDB();
	void GetInfos();

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


