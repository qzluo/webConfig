/********************************************************************
 File: udloadcgi.c
 Function: to handle the file upload from browse.
 Date: 2015-12-3 by lbh
********************************************************************/

#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <stdbool.h>
#endif

#define DEAL_BUF_LEN   1024
#define SIGN_CODE_LEN  100
#define FILENAME_LEN   256

#define LINECONTENT_LEN 1024

#define MAP_FILE_TO_PATH   "fileDirMap.dat"
#define DEFAULT_FILE_PATH  "/home/sky/ehomeserver/bin/"

enum {
    STATE_START,
    STATE_GET_SIGN_CODE,
    STATE_GET_FILE_NAME,
    STATE_GET_FILE_START,
    STATE_GET_FILE_CONTENT,
    STATE_CHECK_END,
    STATE_END,
};

/*-------------------------------------------------------------------
 Function: readSignCode(from_buf, to_buf)
 Poupose: read sign code from a source buffer to a target buffer.
 Parameters: from_buf -- [IN], source buffer preserve the http 
                               response data.
             to_buf -- [OUT], target buffer
 return: 0 -- get success, -1 -- failed
-------------------------------------------------------------------*/
int readSignCode(const char* from_buf, char* to_buf)
{
    if (from_buf == NULL || to_buf == NULL)
        return -1;

    while (*from_buf != '\r' && *from_buf != '\n' && *from_buf)
        *to_buf++ = *from_buf++;

    *to_buf = 0;

    return 0;
}

/*-------------------------------------------------------------------
 Function: getUploadFileName(from_buf, filename)
 Poupose: get upload file name from buffer.
          from buffer should have the following format:
            ... ...
            ..., filename="1.txt"
            ... ...
 Parameters: from_buf -- [IN], source buffer preserve the http 
                               response data.
             filename -- [OUT], file name
 return: 0 -- get success, -1 -- failed
-------------------------------------------------------------------*/
int getUploadFileName(const char* from_buf, char* filename)
{
    char* read_pos = NULL;
    char* write_pos = NULL;

    if (!from_buf || !filename)
        return -1;

    read_pos = (char*)strstr(from_buf, "filename=");
    if (!read_pos)
        return -1;

    read_pos += strlen("filename=");
    write_pos = filename;

    while (*read_pos != '\r' && *read_pos != '\n') {
        if (*read_pos == '/' || *read_pos == '\\')
            write_pos = filename;
        else if (*read_pos != '"')
            *write_pos++ = *read_pos;

        read_pos++;
    }

    *write_pos = 0;

    return 0;
}

/*-------------------------------------------------------------------
 Function: getMapDirNameFromFile(filename, destdir)
 Poupose: Get mapped dir of a file. The map table is list in file
          "fileDirMap.dat"
          with the format: 
             xxx(filename):xxxx(dir)
 Parameters: filename -- [IN], file name to update
             destdir -- [OUT], path to save the file updated
 return: 0 -- get success, -1 -- failed
-------------------------------------------------------------------*/
int getMapDirNameFromFile(const char* filename, char* destdir)
{
    FILE* fp = NULL;
    char linecontent[LINECONTENT_LEN] = {0};
    char line_filename[FILENAME_LEN] = {0};
    char line_filepath[FILENAME_LEN] = {0};
    fp = fopen(MAP_FILE_TO_PATH, "r");
    if (!fp)
        return -1;

    while (fgets(linecontent, LINECONTENT_LEN, fp)) {
        sscanf(linecontent, "%[^:]%*c%s", line_filename, line_filepath);
        if (strcmp(filename, line_filename) == 0) {
            strcpy(destdir, line_filepath);
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);

    return -1;
}

/*-------------------------------------------------------------------
 Function: getFileStart(from_buf)
 Poupose: find file data start posion from buffer input.
          The input buffer has the format:
          .... /r/n/r/n {the data position start from here}
          ...  
 Parameters: from_buf -- [IN], source buffer preserve the http 
                               response data.
 return: file data position
-------------------------------------------------------------------*/
char* getFileStart(char* from_buf)
{
    bool bIsDone = false;
    bool bIsCF = false;

    while (!bIsDone) {
        switch (*from_buf++) {
            case '\r':
                break;

            case '\n':
                if (bIsCF)
                    bIsDone = true;

                bIsCF = true;
                break;

            case '\0':
                return NULL;

            default:
                bIsCF = false;            
        }
    }

    return from_buf;
}

static void ShowErrorInfo(char * error)
{
    printf("Content-Type:text/html;charset=UTF-8\n\n");
    printf("<center><font color='red'>%s</font></center>" , error );
}

int main(int argc, char* argv[])
{
    int state = STATE_START;
    int readLen = 0;
    int leftLen = 0;
    int signCodeLen = 0;
    int tempSignCodeLen = 0;
    int ret = 0;
    char* p_readPos = NULL;
    char* p_posTemp = NULL;
    char dealBuf[DEAL_BUF_LEN] = {0};
    char signCode[SIGN_CODE_LEN] = {0};
    char temp_signCode[SIGN_CODE_LEN] = {0};
    char fileName[FILENAME_LEN] = {0};
    char filePath[FILENAME_LEN] = {0};
    FILE* fp = NULL;

    readLen = fread(dealBuf, sizeof(char), DEAL_BUF_LEN, stdin);
    while (readLen > 0) {
        p_readPos = dealBuf;
        leftLen = readLen + tempSignCodeLen;
        tempSignCodeLen = 0;
        while (leftLen > 0) {
            switch (state) {
                case STATE_START:
                    state = STATE_GET_SIGN_CODE;
                    break;

                case STATE_GET_SIGN_CODE:
                    ret = readSignCode(p_readPos, signCode);
                    if (ret == 0) {
                        signCodeLen = strlen(signCode);
                        leftLen -= signCodeLen;
                        p_readPos += signCodeLen;
                        state = STATE_GET_FILE_NAME;
                    }
                    else      //no signcode is found
                        leftLen = 0;      

                    break;

                case STATE_GET_FILE_NAME:
                    ret = getUploadFileName(p_readPos, fileName);
                    if (ret == 0) {
                        state = STATE_GET_FILE_START;

                        p_posTemp = strstr(p_readPos, fileName);
                        p_posTemp += strlen(fileName);    
                        leftLen -= (p_posTemp - p_readPos);
                        p_readPos = p_posTemp;

                        if (getMapDirNameFromFile(fileName, filePath))
                            strcpy(filePath, DEFAULT_FILE_PATH);

                        if (filePath[strlen(filePath) - 1] != '/')
                            strcat(filePath, "/");

                        strcat(filePath, fileName);

                        //open file
                        fp = fopen(filePath, "w");
                        if (!fp) {
                            ShowErrorInfo("Upload failed");
                            return -1;
                        }
                    }
                    else 
                        leftLen = 0;

                    break;

                case STATE_GET_FILE_START:
                    p_posTemp = getFileStart(p_readPos);
                    if (p_posTemp) {
                        leftLen -= (p_posTemp - p_readPos);
                        p_readPos = p_posTemp;
                        state = STATE_GET_FILE_CONTENT;
                    }
                    else
                        leftLen = 0;

                    break;

                case STATE_GET_FILE_CONTENT:
                    if (*p_readPos != '\r' && *p_readPos != '\n') {
                        fputc(*p_readPos, fp);
                        p_readPos++;
                        leftLen--;
                    }
                    else if (*p_readPos == '\r') {
                        if (leftLen >= signCodeLen + 2) {
                            if (strncmp(p_readPos + 2, signCode, signCodeLen) == 0) {
                                state = STATE_END;
                            }
                            else {
                                fputc(*p_readPos, fp);
                                p_readPos++;
                                leftLen--;
                            }
                        }
                        else {       //保存最后的一段数据到临时缓冲区,留待下次读完一起处理
                            memcpy(temp_signCode, p_readPos, leftLen);
                            memcpy(dealBuf, temp_signCode, leftLen);                            
                            tempSignCodeLen = leftLen;
                            leftLen = 0;
                        }
                    }
                    else {//*p_readPos == '\n'
                        if (leftLen >= signCodeLen + 1) {
                            if (strncmp(p_readPos + 1, signCode, signCodeLen) == 0) {
                                state = STATE_END;
                            }
                            else {
                                fputc(*p_readPos, fp);
                                p_readPos++;
                                leftLen--;
                            }
                        }
                        else {       //保存最后的一段数据到临时缓冲区,留待下次读完一起处理
                            memcpy(temp_signCode, p_readPos, leftLen);
                            memcpy(dealBuf, temp_signCode, leftLen);                            
                            tempSignCodeLen = leftLen;
                            leftLen = 0;
                        }
                    }

                    break;

                case STATE_END:
                    leftLen = 0;
                    break;

                default:
                    break;
            }
        }

        readLen = fread(dealBuf + tempSignCodeLen, 
            sizeof(char), DEAL_BUF_LEN - tempSignCodeLen, stdin);
    }
    if (fp) {        
        fclose(fp);
        fp = NULL;
    }

    ShowErrorInfo("Upload success!");
    return 0;
}