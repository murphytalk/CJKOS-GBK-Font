/**********************************************************************
** Copyright (C) 2004 MurphyTalk
**
** This file is part of the MurphyTalk PinYin Chinese input method,
** which is for the Qtopia Environment.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
**
** murphytalk@gmail.com
***********************************************************************/
#pragma once
#include <vector>
class CGBKMap
{
	//字符表的一个入口
	struct CGBKMapEntry{
		//起始区码
		unsigned char area_code_start;
		//终止区码
		unsigned char area_code_end;
		//各区起始位码
		unsigned char pos_code_start;
		//各区终止位码
		unsigned char pos_code_end;

		//容纳字符数
		//当上述成员值全为0时有效
		unsigned int chars_count;

		CGBKMapEntry(unsigned char ac1,unsigned char ac2,
			unsigned char pc1,unsigned char pc2)
			:area_code_start(ac1),area_code_end(ac2),pos_code_start(pc1),pos_code_end(pc2),
			 chars_count(0)
		{}

		CGBKMapEntry(unsigned int count)
			:area_code_start(0),area_code_end(0),pos_code_start(0),pos_code_end(0),
			 chars_count(count)
		{}
	};

	inline void AppendEntry(unsigned char area_code1,unsigned char area_code2,
		unsigned char pos_code1,unsigned char pos_code2){
			m_chars.push_back(CGBKMapEntry(area_code1,area_code2,
										   pos_code1,pos_code2));
	}

	inline void AppendEntry(unsigned int count){
			m_chars.push_back(CGBKMapEntry(count));
	}
public:
	CGBKMap(void);
	virtual ~CGBKMap(void);

	CString convert_to_win_unicode(const char*  gbk_code);
	
	//从头至尾按照CJKOS GBK 字库的格式的顺序遍历所有字符
	void TravelMap();
	
	//顺序遍历字符时对每个字符调用
	virtual void OnChar(unsigned char area_code,unsigned char pos_code,int repeat_count)=0;

	//字符总数
	unsigned int GetCharsCount();
private:
	//字符表
	typedef std::vector<CGBKMapEntry> CharMap;
	CharMap m_chars;
};
