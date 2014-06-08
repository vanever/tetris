/// @file nesutil.h
/// 
/// @author original by rbbadboy
/// @version 0.1

#ifndef NES_UTIL
#define NES_UTIL 

typedef unsigned char u8;  //�޷���8λ
typedef unsigned int u16;

#include <conio.h>	//����CC65�ṩ�Ŀ�
#include <nes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true	1
#define false	0
#define NULL	0

#define bin(_a,_b,_c,_d,_e,_f,_g,_h) ((_a)<<7|(_b)<<6|(_c)<<5|(_d)<<4|(_e)<<3|(_f)<<2|(_g)<<1|(_h))	//�����Ƶı�ʾ

#define BIT0	0x01	//��0λ
#define BIT1	0x02	//��1λ
#define BIT2	0x04	//��2λ
#define BIT3	0x08	//��3λ
#define BIT4	0x10	//��4λ
#define BIT5	0x20	//��5λ
#define BIT6	0x40	//��6λ
#define BIT7	0x80	//��7λ

#define upperbyte(_value)		((u8)((_value)>>8))		//ȡ���ֽ�
#define lowerbyte(_value)		((u8)(_value))			//ȡ���ֽ�

#define address(_add)			(*((u8 *)(_add)))			//����ֱ�Ӷ��ڴ����

//�ֱ����:
#define joystick_1	0x4016				//�����ֱ���ӳ��
#define joystick_2	0x4017

#define button_A		0x80			//��ťA
#define button_B		0x40			//��ťB
#define button_SELECT	0x20			//��ťSELECT
#define button_START	0x10			//��ťSTART
#define button_UP		0x08			//��ťUP
#define button_DOWN		0x04			//��ťDOWN
#define button_UD		0x0C			//���¼�����
#define button_LEFT		0x02			//��ťLEFT
#define button_RIGHT	0x01			//��ťRIGHT
#define button_LR		0x03			//���Ҽ�����
#define button_DRT		0x0F			//���������
#define button_ANY		0xff			//���������

#define read_joy() read_joystick(joystick_1)	//Ĭ�ϵ��ֱ���ȡ����ȡ�ֱ�1��

u8 read_joystick(u16 _joystick)			//���ֱ�����
{
	u8 n=8, joy_state=0;

	address(_joystick)=01;		//��_joystickд01
	address(_joystick)=00;		//��_joystickд00

	while(n){
		joy_state=(joy_state<<1)|address(_joystick)&1;
		--n;
	}
	return joy_state;
}

#define presskey(k)	(key & (k))
#define iskey(k)	(key == (k))
#define downkey(k)	(okey!=key && (k)&key)
#define upkey(k)	(okey!=key && (k)&okey)
#define isdirkey(k) ((key&button_DRT)==(k))


//����PPU���ƼĴ���1
#define PPU_ctrl_reg_1			0x2000	//PPU���ƼĴ���1ӳ���ַ

#define REG_1_name_0			0x00	//ʹ�����ֱ�0($2000)
#define REG_1_name_1			0x01	//ʹ�����ֱ�1($2400)
#define REG_1_name_2			0x02	//ʹ�����ֱ�2($2800)
#define REG_1_name_3			0x03	//ʹ�����ֱ�3($2c00)
#define REG_1_inc_32			0x04	//��PPU��ַ�Ĵ�����ֵÿ�η��ʺ�����32
#define REG_1_SP_pattern_1		0x08	//�þ���ʹ�õ�ͼ����Ϊͼ����1($1000)
#define REG_1_IM_pattern_1		0x10	//�ñ���ʹ�õ�ͼ����Ϊͼ����1($1000)
#define REG_1_SP_bigsize		0x20	//�þ���ߴ�Ϊ8*16
#define REG_1_PPU_mask			0x40	//δʹ��
#define REG_1_VBlank_able		0x80	//��Vblankʱ�����ж�

//����PPU���ƼĴ���2
#define PPU_ctrl_reg_2			0x2001	//PPU���ƼĴ���2ӳ���ַ

#define REG_2_Unkown			0x01	//ʹ�õ�ɫ
#define REG_2_show_all_IM		0x02	//��ʾ��Ļ����8�� 
#define REG_2_show_all_SP		0x04	//��ʾ�������8�� 
#define REG_2_IM_able			0x08	//��ʾͼ��
#define REG_2_SP_able			0x10	//��ʾ����
#define REG_2_bg_color_none		0x00	//ûЧ��
#define REG_2_bg_color_blue		0x20	//ǿ����ɫ
#define REG_2_bg_color_green	0x40	//ǿ����ɫ
#define REG_2_bg_color_red		0x80	//ǿ����ɫ

//����PPU״̬�Ĵ���
#define PPU_status_reg			0x2002	//PPU״̬�Ĵ���ӳ���ַ

#define hit_flag				0x40	//0�ž�����ײ��־
#define VBlank_flag				0x80	//Vblank��־

//�����Ĵ���ӳ��
#define SCR_scroll_offset		0x2005	//��Ļ����ƫ��
#define PPU_memory_add			0x2006	//VRAM��ַ
#define PPU_memory_dat			0x2007	//VRAM����

#define SP_memory_add			0x2003	//����RAM��ַ
#define SP_memory_dat			0x2004	//����RAM����
#define SP_DMA_access			0x4014	//DMA���ʾ���RAM


//VRAM�ڴ����
#define VRAM_pattern_0			0x0000	//ͼ����0
#define VRAM_pattern_1			0x1000	//ͼ����1
#define pattern_length			0x1000	//ͼ������
#define all_pat_len				0x2000	//ȫ��ͼ������

#define VRAM_name_0				0x2000	//���ֱ�������0
#define VRAM_name_1				0x2400	//���ֱ�1
#define VRAM_name_2				0x2800	//���ֱ�2
#define VRAM_name_3				0x2c00	//���ֱ�3
#define name_length				0x03c0	//���ֱ���


#define VRAM_attr_0				0x23c0	//���Ա�0
#define VRAM_attr_1				0x27c0	//���Ա�1
#define VRAM_attr_2				0x2bc0	//���Ա�2
#define VRAM_attr_3				0x2fc0	//���Ա�3
#define attr_length				0x0040	//���Ա���

#define full_name_l				0x0400	//���������ĵ����ֱ�����������
#define all_name_l				0x0800	//ȫ���������������ĵ����ֱ�����������

#define col_black					0x0f		//��
#define col_blue					0x02		//��
#define col_green					0x2a		//��
#define col_red						0x16		//��
#define col_white					0x30		//��

#define BG_palette				0x3f00	//������ɫ��
#define SP_palette				0x3f10	//�����ɫ��
#define palette_len				0x0010	//������ɫ�峤��
#define all_pal_len				0x0020	//ȫ������������ɫ�峤��
#define VRAM_palette			0x3f00	//��ɫ����ʼλ��

//�������
#define sprite_length			0x04	//�������鳤�ȣ�4���ֽڣ�
#define sprite_all_len			0xff	//���þ���ĳ���
#define	sprite_start			0x00	//������SPRAM�е���ʼλ��
#define SP_color				0x03	//��ɫ����λ
#define SP_behindbg				0x20	//�����ڱ�����
#define SP_hor					0x40	//ˮƽ��ת
#define SP_ver					0x80	//��ֱ��ת


//����
#define screen_width			32		//�����
#define screen_height			30		//�����


#define wait_VBlank()			while(!((address(PPU_status_reg) & VBlank_flag)))	//�ȴ�VBlank
#define ishit()					(address(PPU_status_reg) & hit_flag)		//�ж�0�ž����Ƿ�����ײ

#define set_VRAM_add(_add)		address(PPU_memory_add)=upperbyte(_add);\
									address(PPU_memory_add)=lowerbyte(_add)		//����VRAM��ַ
#define set_cREG_1(_v)			address(PPU_ctrl_reg_1)=(_v)	//���ÿ��ƼĴ���1
#define set_cREG_2(_v)			address(PPU_ctrl_reg_2)=(_v)	//���ÿ��ƼĴ���2


void fill_RAM(u8 *p,u16 len,u8 value)		//���CPU��RAM
{
	while(len){
		*p=value;
		++p;
		--len;
	}
}


void fill_VRAM(u16 start,u16 len,u8 value)	//���PPU��VRAM
{
	set_VRAM_add(start);
	while(len){
		address(PPU_memory_dat)=value;
		--len;
	}
}


void fill_SPRAM(u8 start,u8 len,u8 value)	//��侫��RAM
{
	address(SP_memory_add)=start;
	while(len){
		address(SP_memory_dat)=value;
		--len;
	}
}

void copy_RAM_to_VRAM(u8 *st_r,u16 st_v,u16 len)	//��RAM�����ݵ�VRAM
{
	set_VRAM_add(st_v);
	while(len){
		address(PPU_memory_dat)=*st_r;
		++st_r;
		--len;
	}
}

void copy_RAM_to_SPRAM(u8 *st_r,u8 st_s,u8 len)		//��RAM�����ݵ�SPRAM
{
	address(SP_memory_add)=st_s;
	while(len){
		address(SP_memory_dat)=*st_r;
		++st_r;
		--len;
	}
}

u8 *copy_VRAM_to_RAM(u16 st_v,u8 *st_r,u16 len)		//��VRAM�����ݵ�RAM
{
	u8 *p=st_r;
	set_VRAM_add(st_v);
	0|address(0x2007);	//��ȡ�ĵ�һ�ֽڱ����ԣ����Զ��һ��
	while(len){
		*st_r=address(PPU_memory_dat);
		++st_r;
		--len;
	}
	return p;
}

u8 *copy_SPRAM_to_RAM(u8 st_s,u8 *st_r,u8 len)		//��SPRAM�����ݵ�RAM
{
	u8 *p=st_r;
	address(SP_memory_add)=st_s;
	while(len){
		*st_r=address(SP_memory_dat);
		++st_r;
		--len;
	}
	return p;
}

void SP_DMA(u8 page,u8 start)	//�����DMA���䷽ʽ
{
	address(SP_memory_add)=start;
	address(SP_DMA_access)=page;
}

u8 read_VRAM(u16 addr)		//��ȡVRAM��ֵ
{
	set_VRAM_add(addr);
	0|address(0x2007);	//��ȡ�ĵ�һ�ֽڱ����ԣ����Զ��һ��
	return address(PPU_memory_dat);
}

void write_VRAM(u16 addr,u8 value)	//дVRAM��ֵ
{

	set_VRAM_add(addr);
	address(PPU_memory_dat)=value;
}

u8 read_SPRAM(u8 addr)			//��ȡSPRAM��ֵ
{

	address(SP_memory_add)=addr;
	return address(SP_memory_dat);
}

void write_SPRAM(u8 addr,u8 value)	//дSPRAM��ֵ
{
	address(SP_memory_add)=addr;
	address(SP_memory_dat)=value;
}

void puts_xy(u16 _nametable,u8 _x,u8 _y,char *s)	//�����ֱ�_nametable��(x,y)λ��д�봮s дʱ�������
{
	u16 u;
	u=(_nametable)+(_y)*32+(_x);
	set_VRAM_add(u);
	while(*s!=NULL){
		address(PPU_memory_dat)=*s;
		s++;
	}
}

#define putc_xy(_nametable,_x,_y,_c)	write_VRAM((_nametable)+(_y)*32+(_x),_c)	//��_nametable��(x,y)λ��д��c
#define getc_xy(_nametable,_x,_y)			read_VRAM((_nametable)+(_y)*32+(_x))	//��_nametable��(x,y)λ�õ�ֵ

#define set_scroll(_h,_v)	address(SCR_scroll_offset)=_h;\
													address(SCR_scroll_offset)=_v		//������Ļ����
													
//ͼ�������
#define load_pattern_0(_ptr)		copy_RAM_to_VRAM(_ptr,VRAM_pattern_0,pattern_length)	//����ͼ����0
#define load_pattern_1(_ptr)		copy_RAM_to_VRAM(_ptr,VRAM_pattern_1,pattern_length)	//����ͼ����0											
#define load_all_pattern(_ptr)	copy_RAM_to_VRAM(_ptr,VRAM_pattern_1,all_pat_len)		//����ȫ����������ͼ����
													
//��ɫ�����
#define load_SP_palette(_ptr)		copy_RAM_to_VRAM(_ptr,SP_palette,palette_len)	//���뾫���ɫ��
#define load_BG_palette(_ptr) 	copy_RAM_to_VRAM(_ptr,BG_palette,palette_len)	//���뱳����ɫ��
#define load_all_palette(_ptr)	copy_RAM_to_VRAM(_ptr,VRAM_palette,all_pal_len)	//����ȫ��(����)��ɫ��
#define load_palette_n(_ptr,_n)	copy_RAM_to_VRAM(_ptr,VRAM_palette+(_n)*4,4)	//�����n��ɫ��(������ʼ������һ��������ɫ��Ϊ0,��һ�������ɫ��Ϊ4)

#define load_one_color(_n,_col)	write_VRAM(VRAM_palette+(_n),_col)	//���õ�_n��(0��31)��ɫΪ_col
								
#define save_SP_palette(_ptr)		copy_VRAM_to_RAM(SP_palette,_ptr,palette_len)	//���澫���ɫ��
#define save_BG_palette(_ptr)		copy_VRAM_to_RAM(BG_palette,_ptr,palette_len)	//���汳����ɫ��
#define save_all_palette(_ptr)	copy_VRAM_to_RAM(VRAM_palette,_ptr,all_pal_len)	//����ȫ��(����)��ɫ��

#define load_SP(_ptr,_st,_n)		copy_RAM_to_SPRAM(_ptr,4*(_st),4*(_n))	//������Ϊn��������_ptr���͵�SPRAM�е�_st��ʼ��
#define clear_SP(_st,_n)				fill_SPRAM(4*(_st),4*(_n),0)	//������Ϊn��������_ptr���͵�SPRAM�е�_st��ʼ��								
											//about name table
#define load_name_table_0(_ptr)	copy_RAM_to_VRAM(_ptr,VRAM_name_0,name_length)	//�������ֱ�0
#define load_name_table_1(_ptr)	copy_RAM_to_VRAM(_ptr,VRAM_name_1,name_length)	//�������ֱ�1
#define load_full_name_0(_ptr)	copy_RAM_to_VRAM(_ptr,VRAM_name_0,full_name_l)	//�������������ֱ�0(�������Ա�0)
#define load_full_name_1(_ptr)	copy_RAM_to_VRAM(_ptr,VRAM_name_1,full_name_l)	//�������������ֱ�1(�������Ա�1)
#define load_all_name(_ptr)			copy_RAM_to_VRAM(_ptr,VRAM_name_0,all_name_l)	//����ȫ�������ֱ�(�������ֱ���������Ա�)

#define cls_name_table_0()			fill_VRAM(VRAM_name_0,name_length,0)
#define cls_name_table_1()			fill_VRAM(VRAM_name_1,name_length,0)
#define cls_all_name()					fill_VRAM(VRAM_name_0,all_name_l,0)
#define fill_name_0(_v)					fill_VRAM(VRAM_name_0,name_length,_v)
#define fill_name_1(_v)					fill_VRAM(VRAM_name_1,name_length,_v)

void disable_graphics()		//��Ļ��ʾ��
{
	address(PPU_ctrl_reg_1) = 0;
	address(PPU_ctrl_reg_2) = 0;
}
//�Ĵ���1:0-1:�������ַ 2:PPU��ַ������ʽ 3:����ͼ�����ַ 4:��Ļͼ�����ַ 5:����ߴ� 6:�� 7:Vblankʹ��
//�Ĵ���2:0:ɫ��ģʽ 1:�������� 2:�������� 3:��Ļʹ�� 4:����ʹ�� 5-7:����ɫ
void enable_graphics()		//��Ļ��ʾ��
{//������Լ��޸���δ�����ʵ�������Ҫ��������޸ĵ����������Ҫ���࿴�������ļ�ϵͳ˵����
																//7 6 5 4 3 2 1 0	
	address(PPU_ctrl_reg_1) = bin(	1,0,0,0,0,0,0,0);
	address(PPU_ctrl_reg_2) = bin(	0,0,0,1,1,1,1,0);
}
//ֱ�Ӵ��뾫��
#define putSP(n,x,y,t,a) 	address(SP_memory_add)=n*4;\
							address(SP_memory_dat)=y;\
							address(SP_memory_dat)=t;\
							address(SP_memory_dat)=a;\
							address(SP_memory_dat)=x

typedef struct		//���徫��ṹ
{
	u8 y;			//y����
	u8 tile;		//Tile������
	u8 attr;		//����:vhp000cc(��ֱ��ת ˮƽ��ת ��������Ȩ 000 ��ɫ�ĸ�2λ)
	u8 x;			//x����
}SPRITE,*pSPRITE;

//��������(Ŀǰֻ֧��ǰ��������)
#define sound_chn_0    0x4000 //����0������1��
#define sound_chn_1    0x4004 //����1������2��
#define sound_chn_2    0x4008 //����2
#define sound_chn_3    0x400c //����3
#define sound_ctrl_reg 0x4015	//����ʹ�ܿ���

#define sound_init()	address(sound_ctrl_reg) = 0x0f//������ʼ��

void sound(u16 ch, u8 v0, u8 v1, u8 v2, u8 v3)	//�����������ĸ�����
{
	address(ch++)=v0;
	address(ch++)=v1;
	address(ch++)=v2;
	address(ch++)=v3;
}


//������ǵ���
void nesdebug(u8 i)	//���ڳ������ͣ������0����ʾ�κ����ݣ���0��ʾ����
{
	if(i){
		cprintf("Debug mode !");
		gotoxy(0,wherey()+1);
		cprintf("Press START go on....");
	}
	while(read_joy()!=button_START){}	//�ȴ�start����
}

//v1.1����
#define rnd(n)	(rand()%(n))	//����������n�������

#endif
