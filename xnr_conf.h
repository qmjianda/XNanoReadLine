#ifndef XNR_CONF_H
#define XNR_CONF_H

//行缓存长度
#ifndef  XNR_LINE_MAX_LENGTH
#define XNR_LINE_MAX_LENGTH (128)
#endif

//历史记录缓存长度
#ifndef XNR_HISTORY_BUF_SZIE
#define XNR_HISTORY_BUF_SZIE (512)
#endif

//变量名长度
#ifndef XNR_VAR_NAME_LEN
#define XNR_VAR_NAME_LEN    (32)
#endif

//变量值长度
#ifndef XNR_VAR_VAR_LEN
#define XNR_VAR_VAR_LEN    (64)
#endif

//IO print缓存大小
#ifndef XNR_PRINT_BUF_MAX_LENGTH
#define XNR_PRINT_BUF_MAX_LENGTH (128)
#endif

#endif