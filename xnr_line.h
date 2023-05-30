/*
 * @Author: hqm 13720409820@163.com
 * @Date: 2023-05-29 22:40:29
 * @LastEditors: hqm 13720409820@163.com
 * @LastEditTime: 2023-05-29 22:57:22
 * @FilePath: /XNanoReadLine/xnr_line.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef XNR_LINE_H /*le: line edit*/
#define XNR_LINE_H

#include "xnr_conf.h"
#include "xnr_io.h"

#ifdef __cplusplus
extern   "C" {
#endif

/*光标操作符，其中0x1B是ESC，只适用于xnr_print函数*/
#define CUU(n)      "\x1B[%dA",n		/* 光标向上	光标向上 <n> 行 */
#define CUD(n)      "\x1B[%dB",n		/* 光标向下	光标向下 <n> 行 */
#define CUF(n)      "\x1B[%dC",n		/* 光标向前	光标向前（右）<n> 行 */
#define CUB(n)      "\x1B[%dD",n		/* 光标向后	光标向后（左）<n> 行 */
#define CNL(n)      "\x1B[%dE",n		/* 光标下一行	光标从当前位置向下 <n> 行 */
#define CPL(n)      "\x1B[%dF",n		/* 光标当前行	光标从当前位置向上 <n> 行 */
#define CHA(n)      "\x1B[%dG",n		/* 绝对光标水平	光标在当前行中水平移动到第 <n> 个位置 */
#define VPA(n)      "\x1B[%dd",n		/* 绝对垂直行位置	光标在当前列中垂直移动到第 <n> 个位置 */
#define CUP(y,x)    "\x1B[%d;%dH",y,x	/* 光标位置	*光标移动到视区中的 <x>; <y> 坐标，其中 <x> 是 <y> 行的列 */
#define HVP(y,x)    "\x1B[%d;%df",y,x	/* 水平垂直位置	*光标移动到视区中的 <x>; <y> 坐标，其中 <x> 是 <y> 行的列 */

/*光标可见性*/
#define CU_START_BL "\x1B[?12h"	/* ATT160	文本光标启用闪烁	开始光标闪烁 */
#define CU_STOP_BL  "\x1B[?12l"	/* ATT160	文本光标禁用闪烁	停止闪烁光标 */
#define CU_SHOW     "\x1B[?25h"	/* DECTCEM	文本光标启用模式显示	显示光标 */
#define CU_HIDE     "\x1B[?25l"	/* DECTCEM	文本光标启用模式隐藏	隐藏光标 */

/* 字符操作 */
#define ICH(n)      "\x1B[%d@",n	/* 插入字符	在当前光标位置插入 <n> 个空格，这会将所有现有文本移到右侧。 向右溢出屏幕的文本会被删除。*/
#define DCH(n)      "\x1B[%dP",n	/* 删除字符	删除当前光标位置的 <n> 个字符，这会从屏幕右边缘以空格字符移动。*/
#define ECH(n)      "\x1B[%dX",n	/* 擦除字符	擦除当前光标位置的 <n> 个字符，方法是使用空格字符覆盖它们。*/
#define IL(n)       "\x1B[%dL",n	/* 插入行	将 <n> 行插入光标位置的缓冲区。 光标所在的行及其下方的行将向下移动。*/
#define DL(n)       "\x1B[%dM\r",n	/* 删除行	从缓冲区中删除 <n> 行，从光标所在的行开始。*/

/* 打印字体颜色设置 */
#define SET_TX_CLOLR(color, txt)     color txt TX_DEF
#define TX_DEF          "\x1b[0m"
#define TX_BLACK        "\x1b[30m"
#define TX_RED          "\x1b[31m"
#define TX_GREEN        "\x1b[32m"
#define TX_YELLOW       "\x1b[33m"
#define TX_BLUE         "\x1b[34m"
#define TX_WHITE        "\x1b[37m"

/* 打印背景颜色设置 */
#define SET_BG_CLOLR(color, txt)     (color)(txt)(BK_DEF)
#define BK_DEF          "\x1b[0m"
#define BK_BLACK        "\x1b[40m"
#define BK_RED          "\x1b[41m"
#define BK_GREEN        "\x1b[42m"
#define BK_YELLOW       "\x1b[43m"
#define BK_BLUE         "\x1b[44m"
#define BK_WHITE        "\x1b[47m"

#ifndef XNR_LINE_MAX_LENGTH
#define XNR_LINE_MAX_LENGTH (64)
#endif

typedef struct xnr_line
{
    char line[XNR_LINE_MAX_LENGTH];
    char *prompt;
    int length;
    int cursor;
    xnr_io_t *io;
} xnr_line_t;

/**
 * @description: 初始化
 * @param le：xnr_line实例，io：xnr_io实例
 * @return -1：失败，0：成功
 */
int xnr_line_init(xnr_line_t *le, xnr_io_t *io);

/**
 * @description: 在行中光标位置插入一个字符
 * @param le：xnr_line实例，c：要插入的字符
 * @return -1：失败，0：成功
 */
int xnr_line_insert(xnr_line_t *le, char c);

/**
 * @description: 删除行中光标位置的一个字符
 * @param le：xnr_line实例
 * @return -1：失败，0：成功
 */
int xnr_line_delete(xnr_line_t *le);

/**
 * @description: 获取行缓存指针
 * @param le：xnr_line实例
 * @return NULL：失败，其他：成功
 */
char *xnr_line_line(xnr_line_t *le);

/**
 * @description: 获取行缓存指针
 * @param le：xnr_line实例
 * @return 行中的字符串长度，不包含'\0'
 */
int xnr_line_length(xnr_line_t *le);

/**
 * @description: 获取行光标位置
 * @param le：xnr_line实例
 * @return 返回行光标位置
 */
int xnr_line_cursor(xnr_line_t *le);

/**
 * @description: 设置行光标位置
 * @param le：xnr_line实例，pos：光标位置
 * @return 返回行实际光标位置
 */
int xnr_line_cursor_set(xnr_line_t *le, int pos);

/**
 * @description: 光标左移
 * @param le：xnr_line实例
 * @return 返回行实际光标位置
 */
int xnr_line_cursor_left(xnr_line_t *le);

/**
 * @description: 光标右移
 * @param le：xnr_line实例
 * @return 返回行实际光标位置
 */
int xnr_line_cursor_right(xnr_line_t *le);

/**
 * @description: 仅清除屏幕显示，cls时也会显示prompt
 * @param le：xnr_line实例
 * @return 无
 */
void xnr_line_cls(xnr_line_t *le);

/**
 * @description: 仅清除屏幕显示和line buffer， clear时也会显示prompt，所以建议新行后都调用此函数
 * @param le：xnr_line实例
 * @return 无
 */
void xnr_line_clear(xnr_line_t *le);

/**
 * @description: 获取光标所在位置的字符
 * @param le：xnr_line实例
 * @return 返回光标所在位置的字符
 */
char xnr_line_current_char(xnr_line_t *le);

/**
 * @description: 向行中写字符数组
 * @param le：xnr_line实例，buf：字符数组，len：字符数组长度
 * @return 无
 */
void xnr_line_write(xnr_line_t *le, const char *buf, int len);

/**
 * @description: 向行中打印字符串
 * @param le：xnr_line实例，fmt：格式化字符串，...：参数
 * @return 无
 */
void xnr_line_print(xnr_line_t *le, const char *fmt, ...);

/**
 * @description: 向行中写字符串
 * @param le：xnr_line实例，str：字符串
 * @return 无
 */
void xnr_line_puts(xnr_line_t *le, const char *str);

/**
 * @description: 切分字符串，类似strtok
 * @param msg：输入字符串，delim：切分标记，get：切分后的结果， max_num：结果buf大小
 * @return 返回切分后的子字符串个数
 */
int xnr_line_tok(char *msg, char *delim, char *get[], int max_num);

#ifdef __cplusplus
        }
#endif

#endif