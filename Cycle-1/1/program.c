#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DELIMITERS " +-*\\/;,><=()[]{}"
#define OPERATORS "+-/*<>="
bool isDelimiter(char ch)
{
    return strchr(DELIMITERS, ch) != NULL;
}

bool isOperator(char ch)
{
    return strchr(OPERATORS, ch) != NULL;
}

bool validIdentifier(const char *str)
{
    if (!str || !*str || isdigit(*str))
        return false;
    while (*str)
    {
        if (!isalnum(*str) && *str != '_')
            return false;
        str++;
    }
    return true;
}

bool isKeyword(const char *str)
{
    static const char *keywords[] = {
        "if", "else", "while", "do", "break", "continue", "int", "double", "float",
        "return", "char", "case", "sizeof", "long", "short", "typedef", "switch",
        "unsigned", "void", "static", "struct", "goto", NULL};
    for (const char **keyword = keywords; *keyword; keyword++)
    {
        if (strcmp(str, *keyword) == 0)
            return true;
    }
    return false;
}

bool isInteger(char *str)
{
    if (*str == '-' || *str == '+')
        str++;
    if (!*str)
        return false;
    while (*str)
    {
        if (!isdigit(*str))
            return false;
        str++;
    }
    return true;
}

bool isRealNumber(char *str)
{
    bool hasDecimal = false;
    if (*str == '-' || *str == '+')
        str++;
    if (!*str)
        return false;
    while (*str)
    {
        if (*str == '.')
        {
            if (hasDecimal)
                return false;
            hasDecimal = true;
        }
        else if (!isdigit(*str))
        {
            return false;
        }
        str++;
    }
    return hasDecimal;
}

char *subString(const char *str, int left, int right)
{
    int length = right - left + 1;
    char *subStr = (char *)malloc(length + 1);
    if (subStr)
    {
        memcpy(subStr, str + left, length);
        subStr[length] = '\0';
    }
    return subStr;
}

void parse(const char *str)
{
    int left = 0, right = 0;
    int len = strlen(str);
    while (right <= len)
    {
        if (!isDelimiter(str[right]) && right < len)
        {
            right++;
        }
        else
        {
            if (left < right)
            {
                char *subStr = subString(str, left, right - 1);
                if (isKeyword(subStr))
                {
                    printf("Keyword            :'%s'\n", subStr);
                }
                else if (isInteger(subStr))
                {
                    printf("Integer        : '%s' \n", subStr);
                }
                else if (isRealNumber(subStr))
                {
                    printf("Real Number        :'%s'\n", subStr);
                }
                else if (validIdentifier(subStr))
                {
                    printf("Valid Identifier   :'%s'\n", subStr);
                }
                else
                {
                    printf("Invalid Identifier :'%s'  \n", subStr);
                }
                free(subStr);
            }
            if (right < len && isOperator(str[right]))
            {
                printf("Operator           :'%c' \n", str[right]);
            }
            right++;
            left = right;
        }
    }
}

int main()
{
    FILE *file = fopen("test.txt", "r");
    if (!file)
    {
        perror("Error opening file");
        return 1;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        parse(line);
    }
    fclose(file);
    return 0;
}
