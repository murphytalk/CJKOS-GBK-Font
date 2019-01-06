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
#include "StdAfx.h"
#include "gbkmap.h"
#include "iconv/iconv.h"

CGBKMap::CGBKMap(void)
{
	//建立字符表
	//按照CJKOS GBK字库文件的字符排列顺序

	//GBK/1,GBK/2 -> GB2312字符
	//区码0xA1~0xF7，各区内位码0xA1~0xFE
	AppendEntry(0xA1,0xF7,0xA1,0xFE);

	//1337个空白字
	AppendEntry(1337);

	//GBK/5 -> GBK扩展符号
	//区码0xA8~0xA9，各区内位码0x40~0xA0
	AppendEntry(0xA8,0xA9,0x40,0xA0);

	//GBK/4 -> GBK扩展字
	//区码0xAA~0xFE，各区内位码0x40~0xA0
	AppendEntry(0xAA,0xFE,0x40,0xA0);

	//GBK/3 -> GBK扩展字
	//区码0x81~0xA0，各区内位码0x40~0xFE
	AppendEntry(0x81,0xA0,0x40,0xFE);
}

CGBKMap::~CGBKMap(void)
{
}

//转换一个GBK码到windows所采用的UCS-2LE码
//也可以用windows的MCB和WChar间转换的API
//但那需要用户事先安装GBK的CodePage（不过大概几乎每个用这个工具的人都已经安装了)
//所以还是用了gnu的iconv
CString CGBKMap::convert_to_win_unicode(const char* gbk_code)
{
	CString result;

	iconv_t cd=iconv_open("UCS-2LE","GBK");
	if(cd!=(iconv_t)-1){
		const char *pGBK=gbk_code;
		char* dest=new char[4];
		char* dest_save=dest;
		memset(dest,0,4);
		size_t in_size  = 2;
		size_t out_size = 2;
		size_t convert = iconv(cd,&pGBK,&in_size,&dest,&out_size);
		//dest would be moved in the calling to iconv()
		
		wchar_t *unicode=reinterpret_cast<wchar_t*>(dest_save);
		result=CString(unicode);

		delete []dest_save;
		
		iconv_close(cd);
	}

	return result;
}

//从头至尾按照CJKOS GBK 字库的格式的顺序遍历所有字符
void CGBKMap::TravelMap()
{
	for(CharMap::iterator pos=m_chars.begin();pos!=m_chars.end();pos++){
		CGBKMapEntry& entry=*pos;

        

		if(entry.area_code_start==0 && entry.area_code_end==0){
			//重复个数
			OnChar(0,0,entry.chars_count);
		}
		else{
			for(unsigned char area_code=entry.area_code_start;area_code<=entry.area_code_end;area_code++){
				for(unsigned char pos_code=entry.pos_code_start;pos_code<=entry.pos_code_end;pos_code++){
					OnChar(area_code,pos_code,1);
				}
			}
		}
	}
}

//字符总数
unsigned int CGBKMap::GetCharsCount(){
	unsigned int count=0;
	for(CharMap::iterator pos=m_chars.begin();pos!=m_chars.end();pos++){
		CGBKMapEntry& entry=*pos;
		
		if(entry.area_code_start==0 && entry.area_code_end==0){
			//重复个数
			count+=entry.chars_count;
		}
		else{
			count+=(entry.area_code_end-entry.area_code_start+1)*(entry.pos_code_end-entry.pos_code_start+1);
		}
	}
	
	return count;
}
