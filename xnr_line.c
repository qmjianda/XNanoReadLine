#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "xnr_line.h"

static char *_strpbrk(char *s, const char *delim) // 返回s1中第一个满足条件的字符的指针, 并且保留""号内的源格式
{
    char flag = 0;
    for (int i = 0; s[i]; i++)
    {
        for (int j = 0; delim[j]; j++)
        {
            if (0 == flag)
            {
                if (s[i] == '\"')
                {
                    for (int k = i; s[k]; k++)
                    {
                        s[k] = s[k + 1];
                    }
                    flag = 1;
                    continue;
                }
            }

            if (flag)
            {
                if (s[i] == '\"')
                {
                    for (int k = i; s[k]; k++)
                    {
                        s[k] = s[k + 1];
                    }
                    flag = 0;
                }
                else
                {
                    continue;
                }
            }

            if (s[i] == delim[j])
            {
                return &s[i];
            }
        }
    }
    return NULL;
}

/* 字符串切割函数，使用:
        char s[] = "-abc-=-def";
        char *sp;
        x = strtok_r(s, "-", &sp);      // x = "abc", sp = "=-def"
        x = strtok_r(NULL, "-=", &sp);  // x = "def", sp = NULL
        x = strtok_r(NULL, "=", &sp);   // x = NULL
                // s = "abc/0-def/0"
*/
static char *_strtok(char *s, const char *delim, char **save_ptr)
{
    char *token;

    if (s == NULL)
        s = *save_ptr;

    /* Scan leading delimiters.  */
    s += strspn(s, delim); // 返回字符串中第一个不在指定字符串中出现的字符下标，去掉了以空字符开头的
                           // 字串的情况
    if (*s == '\0')
        return NULL;

    /* Find the end of the token.  */
    token = s;
    s = _strpbrk(token, delim); //   返回s1中第一个满足条件的字符的指针
    if (s == NULL)
        /* This token finishes the string.  */
        *save_ptr = strchr(token, '\0');
    else
    {
        /* Terminate the token and make *SAVE_PTR point past it.  */
        *s = '\0';
        *save_ptr = s + 1;
    }
    return token;
}

int xnr_line_init(xnr_line_t *le, xnr_io_t *io)
{
    if (NULL == le || NULL == io)
        return -1;
    le->length = 0;
    le->cursor = 0;
    le->prompt = "->";
    le->line[0] = '\0';
    le->io = io;
    return 0;
}

int xnr_line_insert(xnr_line_t *le, char c)
{
    if (NULL == le)
        return -1;

    if (le->length < XNR_LINE_MAX_LENGTH - 1) // 需要保留一个位置存放'\0'
    {
        for (int i = le->length; i > le->cursor; i--)
        {
            le->line[i] = le->line[i - 1];
        }
        ++le->length;
        le->line[le->length] = '\0';
        le->line[le->cursor++] = c;
        xnr_io_printf(le->io, ICH(1));  // 在当前光标位置插入空格
        xnr_io_printf(le->io, "%c", c); // 将空格替换成字符
        return 0;
    }
    return -1;
}

int xnr_line_delete(xnr_line_t *le)
{
    if (NULL == le)
        return -1;
    if (le->cursor > 0)
    {
        for (int i = le->cursor - 1; i < le->length - 1; i++)
        {
            le->line[i] = le->line[i + 1];
        }
        --le->length;
        --le->cursor;
        le->line[le->length] = '\0';
        xnr_io_printf(le->io, CUB(1));
        xnr_io_printf(le->io, DCH(1));
        return 0;
    }
    return -1;
}

char *xnr_line_line(xnr_line_t *le)
{
    if (NULL == le)
        return NULL;
    return le->line;
}

int xnr_line_cursor(xnr_line_t *le)
{
    if (NULL == le)
        return 0;
    return le->cursor;
}

int xnr_line_length(xnr_line_t *le) // 不包含'\0'
{
    if (NULL == le)
        return 0;
    return le->length;
}

int xnr_line_cursor_left(xnr_line_t *le)
{
    if (NULL == le)
        return 0;
    int pos = xnr_line_cursor(le);
    if (pos > 0)
        xnr_line_cursor_set(le, --pos);
    return pos;
}

int xnr_line_cursor_right(xnr_line_t *le)
{
    if (NULL == le)
        return 0;
    int pos = xnr_line_cursor(le);
    if (pos < xnr_line_length(le))
        xnr_line_cursor_set(le, ++pos);
    return pos;
}

int xnr_line_cursor_set(xnr_line_t *le, int pos)
{
    if (NULL == le)
        return 0;
    if (pos > le->length)
    {
        pos = le->length;
    }
    le->cursor = pos;
    pos = le->cursor + strlen(le->prompt) + 1;
    xnr_io_printf(le->io, CHA(pos));
    return pos;
}

void xnr_line_cls(xnr_line_t *le) // 仅清除屏幕显示，clear时也会显示prompt
{
    if (NULL == le)
        return;
    xnr_io_printf(le->io, DL(0));
    xnr_io_printf(le->io, "%s", le->prompt);
    le->length = 0;
    le->cursor = 0;
}

void xnr_line_clear(xnr_line_t *le) // 仅清除屏幕显示和le buffer， clear时也会显示prompt，所以建议新行后都调用此函数
{
    if (NULL == le)
        return;
    xnr_line_cls(le);
    le->line[0] = '\0';
}

char xnr_line_current_char(xnr_line_t *le)
{
    if (NULL == le)
        return '\0';
    if (le->cursor < le->length)
    {
        return le->line[le->cursor];
    }
    return '\0';
}

void xnr_line_write(xnr_line_t *le, const char *buf, int len)
{
    if (NULL == le)
        return;
    if (len > XNR_LINE_MAX_LENGTH - 1)
    {
        len = XNR_LINE_MAX_LENGTH - 1;
    }
    for (int i = 0; i < len; i++)
    {
        xnr_line_insert(le, buf[i]);
    }
}

void xnr_line_puts(xnr_line_t *le, const char *str)
{
    if (NULL == le)
        return;
    xnr_line_write(le, str, strlen(str));
}

void xnr_line_print(xnr_line_t *le, const char *fmt, ...)
{
    if (NULL == le)
        return;
    char ucstring[XNR_LINE_MAX_LENGTH] = {0};
    va_list arg;
    va_start(arg, fmt);
    vsnprintf(ucstring, XNR_LINE_MAX_LENGTH, fmt, arg);
    va_end(arg);
    xnr_line_write(le, ucstring, strlen(ucstring));
}

int xnr_line_tok(char *msg, char *delim, char *get[], int max_num)
{
    int i, ret;
    char *ptr = NULL;
    char *sp = NULL;
    ptr = _strtok(msg, delim, &sp);
    for (i = 0; ptr != NULL && i < max_num; i++)
    {
        get[i] = ptr;
        ptr = _strtok(NULL, delim, &sp);
    }
    ret = i;
    return ret;
}