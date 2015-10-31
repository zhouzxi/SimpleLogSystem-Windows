/**********************************************************************
* 版权所有 (C)2015, Zhou Zhaoxiong。
*
* 文件名称：WriteLog.c
* 文件标识：无
* 内容摘要：演示日志信息的打印方法
* 其它说明：无
* 当前版本：V1.0
* 作    者：Zhou Zhaoxiong
* 完成日期：20150225
*
**********************************************************************/
#include <stdio.h>
#include <windows.h>

// 重定义数据类型
typedef signed   int    INT32;
typedef unsigned int    UINT32;
typedef unsigned char   UINT8;

// 全局变量
UINT32  g_iLogLevel      = 0;    // 日志等级
UINT8   g_szLogFile[100] = {0};  // 带路径的日志文件名

// 宏定义
#define LOG_FATAL       0     // 严重错误
#define LOG_ERROR       1     // 一般错误
#define LOG_WARN        2     // 警告
#define LOG_INFO        3     // 一般信息
#define LOG_TRACE       4     // 跟踪信息
#define LOG_DEBUG       5     // 调试信息
#define LOG_ALL         6     // 所有信息


// 函数声明
void   WriteLogFile(UINT32 iLogLevel, UINT8 *pszContent);
UINT8 *LogLevel(UINT32 iLogLevel);
void   GetTime(UINT8 *pszTimeStr);


/**********************************************************************
* 功能描述：主函数
* 输入参数：无
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期        版本号      修改人            修改内容
* -------------------------------------------------------------------
* 20150225        V1.0     Zhou Zhaoxiong        创建
***********************************************************************/
INT32 main()
{
    UINT8  szConfigFile[128]  = {0};
    UINT8  szLogContent[1024] = {0};
    UINT8  szLogDir[128]      = {0};
    UINT32 iLoopFlag          = 0;

    // 获取配置文件全路径(包括文件名)
    GetCurrentDirectory(sizeof(szConfigFile)-1, szConfigFile);
    strcat(szConfigFile, "\\");
    strcat(szConfigFile, "Config.ini");

    // 日志等级
    g_iLogLevel = GetPrivateProfileInt("LOG", "LogLevel", 3, szConfigFile);

    // 日志文件存放目录
    GetPrivateProfileString("LOG", "LogDir", "", szLogDir, sizeof(szLogDir), szConfigFile);
    _snprintf(g_szLogFile, sizeof(g_szLogFile)-1, "%s\\WriteLog.log", szLogDir);
    
    // 打印第一条日志
    _snprintf(szLogContent, sizeof(szLogContent)-1, "The first log info!");
    WriteLogFile(LOG_INFO, szLogContent);

    // 打印第二条日志
    _snprintf(szLogContent, sizeof(szLogContent)-1, "The second log info!");
    WriteLogFile(LOG_DEBUG, szLogContent);

    // 打印0到9的10个数
    for (iLoopFlag=0; iLoopFlag<10; iLoopFlag++)
    {
	    _snprintf(szLogContent, sizeof(szLogContent)-1, "The value is: %d", iLoopFlag);
	    WriteLogFile(LOG_INFO, szLogContent);
    }

    return 0;                  // main函数执行成功返回0
}


/**********************************************************************
 * 功能描述： 将内容写到日志文件中
 * 输入参数： iLogLevel-日志等级
              *pszContent-每条日志的具体内容
 * 输出参数： 无
 * 返 回 值： 无
 * 其它说明： 无
 * 修改日期        版本号        修改人        修改内容
 * -------------------------------------------------------------------
 * 20150225        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/    
void WriteLogFile(UINT32 iLogLevel, UINT8 *pszContent) 
{
    FILE  *fp                 = NULL;
    UINT8  szLogContent[2048] = {0};
    UINT8  szTimeStr[128]     = {0};

    if (pszContent == NULL)
    {
        return;
    }

    // 过滤日志等级
    if (iLogLevel > g_iLogLevel)
    {
        return;
    }

    fp = fopen(g_szLogFile, "at+");      // 打开文件, 每次写入的时候在后面追加
    if (fp == NULL)
    {
        return;
    }

    // 写入日志时间
    GetTime(szTimeStr);
    fputs(szTimeStr, fp);

    // 写入日志内容
    // 在原内容中添加日志等级标识
    _snprintf(szLogContent, sizeof(szLogContent)-1, "[WriteLog.c][%s]%s\n", LogLevel(iLogLevel), pszContent);
    fputs(szLogContent, fp);
    
    fflush(fp);     // 刷新文件
    fclose(fp);     // 关闭文件
    fp = NULL;      // 将文件指针置为空

    return;
}


/**********************************************************************
 * 功能描述： 获取对应的日志等级
 * 输入参数： iLogLevel-日志等级
 * 输出参数： 无
 * 返 回 值： 日志等级信息字符串
 * 其它说明： 无
 * 修改日期        版本号        修改人        修改内容
 * ------------------------------------------------------------------
 * 20150225        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/ 
UINT8 *LogLevel(UINT32 iLogLevel)
{
    switch (iLogLevel)
    {
        case LOG_FATAL:
	{
	    return "FATAL";
      	}

        case LOG_ERROR:   
	{
	    return "ERROR";
      	}

        case LOG_WARN :
	{
	    return "WARN";
	}

        case LOG_INFO :
	{
	    return "INFO";
        }

        case LOG_TRACE:   
	{
	    return "TRACE";
	}

        case LOG_DEBUG:   
	{
	    return "DEBUG";
	}

	case LOG_ALL:   
	{
	    return "ALL";
	}

        default: 
	{
	    return "OTHER";
        }
    }
}


/**********************************************************************
 * 功能描述： 获取时间串
 * 输入参数： 无
 * 输出参数： pszTimeStr-时间串
 * 返 回 值： 无
 * 其它说明： 无
 * 修改日期        版本号        修改人        修改内容
 * -------------------------------------------------------------------
 * 20150225        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/ 
void GetTime(UINT8 *pszTimeStr)
{
    SYSTEMTIME  tSysTime = {0};

    GetLocalTime(&tSysTime);
    sprintf(pszTimeStr, "[%04d.%02d.%02d %02d:%02d:%02d.%03d]", 
        tSysTime.wYear, tSysTime.wMonth, tSysTime.wDay,
        tSysTime.wHour, tSysTime.wMinute, tSysTime.wSecond,
        tSysTime.wMilliseconds);

    return;
}
