#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "xnr_line.h"
#include "xnr_history.h"
#include "xnr_io.h"
#include "xnr_key.h"
#include "xnr_var.h"

#define MAX_LINE_LENGTH 1024

int getch()
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

int io_write_t(int fd, const char *buf, size_t len)
{
    return write(fd, buf, len);
}

int io_read_t(int fd, char *buf, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        buf[i] = getch();
    }
    return len;
}

int main(void)
{
    xnr_line_t le;
    xnr_history_t hs;
    xnr_io_t io;
    xnr_key_t key;
    xnr_var_tab_t var_table;
    xnr_var_t var_buf[4];
    char history_line[XNR_LINE_MAX_LENGTH] = {0};

    //变量测试，增加几个name变量
    xnr_var_init(&var_table, var_buf, 4);
    xnr_var_set(&var_table, "name1", "Sun Wukong");
    xnr_var_set(&var_table, "name2", "Zhu Bajie");
    xnr_var_set(&var_table, "name3", "Sha  Heshang");
    xnr_var_set(&var_table, "name4", "Bai Longma");
    xnr_var_set(&var_table, "name4", "nil");
    xnr_var_set(&var_table, "name5", "Tang Sanzang");

    xnr_io_init(&io, io_write_t, io_read_t, STDIN_FILENO, STDOUT_FILENO);
    xnr_key_init(&key);
    xnr_line_init(&le, &io);
    xnr_history_init(&hs, &io);
    printf("You can type: hello ${name1}\n");
    xnr_line_clear(&le);
    while (1) //功能：从命令行中读取字符串并显示
    {
        int ch = xnr_io_getc(&io);
        if (xnr_key_encode(&key, ch))
        {
            char *key_val = xnr_key_val(&key);
            if (isprint(key_val[0])) //可显示字符
            { 
                xnr_line_insert(&le, ch); //向行中插入可显示字符
            }
            else //不可显示字符，即快捷键
            {
                if (strcmp(key_val, KEY_ENTER) == 0) //回车
                {
                    printf("\n");
                    char *line = xnr_line_line(&le);
                    xnr_history_append(&hs, line);
                    xnr_var_repalce(&var_table, line, history_line, XNR_LINE_MAX_LENGTH);
                    printf("You typed: %s\n", history_line);
                    xnr_line_clear(&le);
                }
                else if (strcmp(key_val, KEY_UP) == 0) //上翻
                {
                    if (xnr_history_up(&hs, history_line, XNR_LINE_MAX_LENGTH) > 0)
                    {
                        xnr_line_clear(&le);
                        xnr_line_puts(&le, history_line);
                    }
                }
                else if (strcmp(key_val, KEY_DW) == 0) //下翻
                {
                    if (xnr_history_dw(&hs, history_line, XNR_LINE_MAX_LENGTH) > 0)
                    {
                        xnr_line_clear(&le);
                        xnr_line_puts(&le, history_line);
                    }
                    else
                    {
                        xnr_line_clear(&le);
                    }
                }
                else if (strcmp(key_val, KEY_RIGHT) == 0) //光标右移
                {
                    xnr_line_cursor_right(&le);
                }
                else if (strcmp(key_val, KEY_LEFT) == 0) //光标左移
                {
                    xnr_line_cursor_left(&le);
                }
                else if (strcmp(key_val, KEY_DELETE) == 0) //删除光标所在字符
                {
                    xnr_line_delete(&le);
                }
                else //其他按键，统一按照删除键来算
                {
                    xnr_line_delete(&le);
                }
            }
        }
    }
    return EXIT_SUCCESS;
}