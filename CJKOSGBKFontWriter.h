/**********************************************************************
** Copyright (C) 2004 MurphyTalk
**
** This file is part of the MurphTalk(AKA:DeepWater@Hi-PDA)'s CJKOS GBK Font Creator,
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
#include <time.h>
#include <stdio.h>

#include "GBKMap.h"

//
template<class T>
void reverse_byte_order(T& value,char* buf){
	char *p=(char*)&value;
	for(int i=sizeof(T)-1,k=0;i>=0;i--,k++){
		buf[k]=p[i];
	}
}

//采用template T是为了实现与提供字体点阵信息的类的松耦合
//见函数GetFontDots()的说明
//
//template U: 对每个字符都会调用U:OnChar(),见CCJKOSGBKFontWriter::OnChar()的说明
//供通知一个字符处理完毕用
template<class T,class U>
class CCJKOSGBKFontWriter : public CGBKMap
{
public:
	CCJKOSGBKFontWriter(int font_size,LPCTSTR fontname):m_font_dots(NULL),m_f(NULL){
		T::Begin(font_size,fontname);
		SetFontSize(font_size);
	}
	~CCJKOSGBKFontWriter(void){
		T::End();
		delete []m_font_dots;
	}

	//得到容纳指定点阵数的一个字所需的字节数
	inline size_t GetOneCharBytes(int font_size){
		return (size_t)(font_size*font_size)/8;
	}

	//设置字体大小（点阵数）
	//同时重新设置字体点阵缓冲区
	inline void SetFontSize(int size){
		delete []m_font_dots;
		m_font_dots=new char[GetOneCharBytes(size)];
		m_font_size = size;
	}

	//得到一个字的点阵信息：排列方式是行列顺序、每点一位。
	//T必须提供静态函数GetFontDots,参数描述见下
	//template<class T>
	void GetFontDots(const char * gbk_code){
		//convert gbk_code to unicode first
		CString unicode = convert_to_win_unicode(gbk_code);
		TRACE("GBK code %02X%02X converted to UNICODE\n",gbk_code[0],gbk_code[1]);
		T::GetFontDots(unicode,						//unicode码
					   m_font_size,					//字体点阵数
					   m_font_dots,					//点阵接收缓冲区
					   (int)GetOneCharBytes(m_font_size));//缓冲区大小
	}

	//顺序遍历字符时对每个字符调用
	//见CGBKMap::TravelMap()
	virtual void OnChar(unsigned char area_code,unsigned char pos_code,int repeat_count)
	{
		const char gbk[]={area_code,pos_code};
		//取点阵
		GetFontDots(gbk);

		//将取得的点阵写入文件
		size_t bytes=GetOneCharBytes(m_font_size);
		for(int i=0;i<repeat_count;i++){
			fwrite(m_font_dots,1,bytes,m_f);
		}

		//通知调用者已处理完毕一个字符
		U::OnChar(repeat_count);
	}

	//输出一个CJKOS的GBK字体文件
	void WriteCJKOSGBKFontFile(const wchar_t* filename)
	{
		m_f=_wfopen(filename,_T("w+b"));
		if(NULL!=m_f){
			//文件头
			WriteFileHeader();
			
			//顺序写字体点阵信息
			//字符表的建立见CGBKMap::CGBKMap()
			TravelMap();

			fclose(m_f);
			m_f=NULL;
		}
		//else
	}
private:
	//写CJKOS的GBK字体文件的文件头
	void WriteFileHeader(){
		char buf[32+1];

		////PDB header
		//DB name
		memset(buf,0,32);
		sprintf(buf,"CJK Font %02dx%02d ChineseGBK",m_font_size,m_font_size);
		fwrite(buf,1,32,m_f);
		//Flag
		buf[0]=0;buf[1]=1;
		fwrite(buf,1,2,m_f);
		//version
		fwrite(buf,1,2,m_f);


		//create date,update date,backup date
		__time32_t t;
		_time32(&t);
		reverse_byte_order(t,buf);
		for(int i=0;i<3;i++){
			fwrite(buf,1,sizeof(__time32_t),m_f);
		}
		
		//three 4 bytes values:Modified Num,App.Info Size,Sort Info Size
		memset(buf,0,12);
		fwrite(buf,1,12,m_f);

		fwrite("dFnt",1,4,m_f);
		fwrite("dCJK",1,4,m_f);

		//two 4 bytes values:Unique ID,Next Record List
		fwrite(buf,1,8,m_f);

		////Resource Header
		unsigned int chunksize,offset;
		unsigned short resnum;

		if( m_font_size == 12 ){
			resnum = 4*3;
			offset = 0xC8;
			chunksize = 0x9000;
		}
		else if( m_font_size == 16 ){
			resnum = 16*3;
			offset = 0x230;
			chunksize = 0x4000;
		}
		else{ //24
			resnum = 16*3;
			offset = 0x230;
			chunksize = 0x9000;
		}

#define write_value(v) reverse_byte_order(v,buf);fwrite(buf,1,sizeof(v),m_f);

		//num. of Records
		write_value(resnum);
		
		unsigned int off;
		for(unsigned short i = 0; i < resnum; i++ ){
		    //Resource index entry
		    fwrite("FBLK",1,4,m_f);
			
			write_value(i);
			
			off=offset + i * chunksize;
			write_value(off);
		}

		//字节对齐
		memset(buf,0,2);
		fwrite(buf,1,2,m_f);
	}
private:
	//字体大小（点阵数）
	int m_font_size;
	// 一个字的点阵信息：排列方式是行列顺序、每点一位。
    //但10点阵字体例外，需要特殊处理
	char *m_font_dots;
	//正在写入的文件
	FILE *m_f;
};

