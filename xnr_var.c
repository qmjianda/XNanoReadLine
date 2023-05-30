#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "xnr_var.h"

int xnr_var_init(xnr_var_tab_t *tab, xnr_var_t *var_buf, int num)
{
    if (NULL == tab || NULL == var_buf || 0 == num)
        return -1;
    tab->capacity = num;
    tab->var_buf = var_buf;
    memset(var_buf, 0, sizeof(xnr_var_t) * num);
    return 0;
}

int xnr_var_set(xnr_var_tab_t *tab, char *name, char *value)
{
    if (NULL == tab ||
        NULL == name ||
        NULL == value ||
        strlen(name) == 0 || strlen(name) >= XNR_VAR_NAME_LEN ||
        strlen(value) == 0 || strlen(value) >= XNR_VAR_VAR_LEN)
        return -1;
    xnr_var_t *var = NULL;                 // 保存需要设置的槽位
    xnr_var_t *var_empty = NULL;           // 保存空闲槽位
    for (int i = 0; i < tab->capacity; i++) // 查找name是否存在
    {
        if ((NULL == var_empty) && (tab->var_buf[i].name[0] == '\0'))
        {
            var_empty = &tab->var_buf[i];
        }
        else
        {
            if (strcmp(tab->var_buf[i].name, name) == 0)
            {
                var = &tab->var_buf[i];
            }
        }
    }
    if (NULL == var) // 获取一个空闲的var槽位
        var = var_empty;

    if (NULL == var) // 没有空余槽位
        return -1;

    if (strcmp(value, "nil") == 0) // 删除变量
    {
        var->name[0] = '\0';
        return 0;
    }
    else
    {
        strncpy(var->value, value, XNR_VAR_VAR_LEN);
        strncpy(var->name, name, XNR_VAR_NAME_LEN);
        var->value[XNR_VAR_VAR_LEN - 1] = '\0';
        var->name[XNR_VAR_NAME_LEN - 1] = '\0';
    }
    return 0;
}

char *xnr_var_value(xnr_var_tab_t *tab, char *name)
{
    if (NULL == tab || NULL == name)
        return "nil";
    xnr_var_t *var = NULL;
    for (int i = 0; i < tab->capacity; i++) // 查找name是否存在
    {
        if (strcmp(tab->var_buf[i].name, name) == 0)
        {
            var = &tab->var_buf[i];
        }
    }
    if (NULL == var)
        return "nil";
    else
        return var->value;
}

int xnr_var_repalce(xnr_var_tab_t *tab, char *in, char *out, int out_len)
{
    if (NULL == tab || NULL == in || out == NULL || 0 == out_len)
        return -1;
    int len = strlen(in);
    char name[XNR_VAR_NAME_LEN] = {0};
    int out_i = 0;
    for (int i = 0; (i < len) && (out_i < (out_len - 1)); i++)
    {
        if (in[i] == '$')
        {
            int k = 0;
            char end = ' ';
            int j = i + 1;
            if (in[j] == '{')
            {
                end = '}';
                j++;
            }
            while (isprint(in[j]) && // 可显示字符
                   in[j] != end      // 变量结尾
            )
            {
                if (k < (XNR_VAR_NAME_LEN - 1))
                {
                    name[k++] = in[j++];
                }
                else
                {
                    return -1; // name长度超限
                }
            }
            name[k] = '\0';
            char *value = xnr_var_value(tab, name); // 获取变量的值
            if (value != NULL)
            {
                int val_len = strlen(value);
                for (k = 0; (k < val_len) && (out_i < (out_len - 1)); k++)
                {
                    out[out_i++] = value[k];
                }
            }
            if (end == '}')
                i = j;
            else
                i = j - 1;
        }
        else
        {
            out[out_i++] = in[i];
        }
    }
    out[out_i] = '\0';
    return out_i;
}