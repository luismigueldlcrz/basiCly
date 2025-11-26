#include "lexer.h"
#include <stdlib.h>
#include <string.h>

/* ============================
   LEXER INITIALIZATION
   ============================ */

void initLexer(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->currentChar = source[0];
}

/* ============================
   LEXER UTILITY FUNCTIONS
   ============================ */

void advance(Lexer *lexer) {
    lexer->position++;
    lexer->currentChar = lexer->source[lexer->position];
}

void skipWhitespace(Lexer *lexer) {
    while (lexer->currentChar == ' ' || lexer->currentChar == '\t' ||
           lexer->currentChar == '\n' || lexer->currentChar == '\r') {
        advance(lexer);
    }
}

/* ============================
   HELPER FUNCTIONS
   ============================ */

int isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

int isDigit(char c) {
    return (c >= '0' && c <= '9');
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
           c == '=' || c == '<' || c == '>' || c == '!' || c == '&' ||
           c == '|';
}

int isDelimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ',' || c == ';' ||
           c == '.' || c == ':' || c == '"' || c == '\'';
}

/* ============================
   WORD CLASSIFICATION
   ============================ */

int isKeyword(const char *lexeme) {
    if (!lexeme || !lexeme[0]) return 0;
    switch (lexeme[0]) {
        case 'c': /* check, cycle */
            if (lexeme[1]=='h' && lexeme[2]=='e' && lexeme[3]=='c' && lexeme[4]=='k' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='y' && lexeme[2]=='c' && lexeme[3]=='l' && lexeme[4]=='e' && lexeme[5]=='\0') return 1;
            break;
        case 'd': /* digit, during */
            if (lexeme[1]=='i' && lexeme[2]=='g' && lexeme[3]=='i' && lexeme[4]=='t' && lexeme[5]=='\0') return 1;
            break;
        case 'i': /* indiv, instead */
            if (lexeme[1]=='n' && lexeme[2]=='d' && lexeme[3]=='i' && lexeme[4]=='v' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='n' && lexeme[2]=='s' && lexeme[3]=='t' && lexeme[4]=='e' && lexeme[5]=='a' && lexeme[6]=='d' && lexeme[7]=='\0') return 1;
            break;
        case 'l': /* large, long */
            if (lexeme[1]=='a' && lexeme[2]=='r' && lexeme[3]=='g' && lexeme[4]=='e' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='o' && lexeme[2]=='n' && lexeme[3]=='g' && lexeme[4]=='\0') return 1;
            break;
        case 'o': /* otherwise */
            if (lexeme[1]=='t' && lexeme[2]=='h' && lexeme[3]=='e' && lexeme[4]=='r' && lexeme[5]=='w' && lexeme[6]=='i' && lexeme[7]=='s' && lexeme[8]=='e' && lexeme[9]=='\0') return 1;
            break;
        case 'p': /* perform */
            if (lexeme[1]=='e' && lexeme[2]=='f' && lexeme[3]=='o' && lexeme[4]=='r' && lexeme[5]=='m' && lexeme[6]=='\0') return 1;
            break;
        case 's': /* select, short, skip, stop*/
            if (lexeme[1]=='e' && lexeme[2]=='l' && lexeme[3]=='e' && lexeme[4]=='c' && lexeme[5]=='t' && lexeme[6]=='\0') return 1;
            if (lexeme[1]=='h' && lexeme[2]=='o' && lexeme[3]=='r' && lexeme[4]=='t' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='k' && lexeme[2]=='i' && lexeme[3]=='p' && lexeme[4]=='\0') return 1;
            if (lexeme[1]=='t' && lexeme[2]=='o' && lexeme[3]=='p' && lexeme[4]=='\0') return 1;
            break;
        case 't': /* tiny, test */
            if (lexeme[1]=='i' && lexeme[2]=='n' && lexeme[3]=='y' && lexeme[4]=='\0') return 1;
            break;
        case 'w': /* when */
            if (lexeme[1]=='h' && lexeme[2]=='e' && lexeme[3]=='n' && lexeme[4]=='\0') return 1;
            break;
    }
    return 0;
}

int isReservedWord(const char *lexeme) {
    if (!lexeme || !lexeme[0]) return 0;
    switch (lexeme[0]) {
        case 'a': /* alias */
            if (lexeme[1]=='l' && lexeme[2]=='i' && lexeme[3]=='a' && lexeme[4]=='s' && lexeme[5]=='\0') return 1;
            break;
        case 'b': /* blank, bothsign */
            if (lexeme[1]=='l' && lexeme[2]=='a' && lexeme[3]=='n' && lexeme[4]=='k' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='o' && lexeme[2]=='t' && lexeme[3]=='h' && lexeme[4]=='s' && lexeme[5]=='i' && lexeme[6]=='g' && lexeme[7]=='n' && lexeme[8]=='\0') return 1;
            break;
        case 'f': /* false, fixed */
            if (lexeme[1]=='a' && lexeme[2]=='l' && lexeme[3]=='s' && lexeme[4]=='e' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='i' && lexeme[2]=='x' && lexeme[3]=='e' && lexeme[4]=='d' && lexeme[5]=='\0') return 1;
            break;
        case 'g': /* group */
            if (lexeme[1]=='r' && lexeme[2]=='o' && lexeme[3]=='u' && lexeme[4]=='p' && lexeme[5]=='\0') return 1;
            break;
        case 'j': /* jumpto */
            if (lexeme[1]=='u' && lexeme[2]=='m' && lexeme[3]=='p' && lexeme[4]=='t' && lexeme[5]=='o' && lexeme[6]=='\0') return 1;
            break;
        case 'k': /* keep */
            if (lexeme[1]=='e' && lexeme[2]=='e' && lexeme[3]=='p' && lexeme[4]=='\0') return 1;
            break;
        case 'l': /* link, live, local */
            if (lexeme[1]=='i' && lexeme[2]=='n' && lexeme[3]=='k' && lexeme[4]=='\0') return 1;
            if (lexeme[1]=='i' && lexeme[2]=='v' && lexeme[3]=='e' && lexeme[4]=='\0') return 1;
            if (lexeme[1]=='o' && lexeme[2]=='c' && lexeme[3]=='a' && lexeme[4]=='l' && lexeme[5]=='\0') return 1;
            break;
        case 'o': /* output */
            if (lexeme[1]=='u' && lexeme[2]=='t' && lexeme[3]=='p' && lexeme[4]=='u' && lexeme[5]=='t' && lexeme[6]=='\0') return 1;
            break;
        case 'r': /* register */
            if (lexeme[1]=='e' && lexeme[2]=='g' && lexeme[3]=='i' && lexeme[4]=='s' && lexeme[5]=='t' && lexeme[6]=='e' && lexeme[7]=='r' && lexeme[8]=='\0') return 1;
            break;
        case 's': /* set, shared, size */
            if (lexeme[1]=='e' && lexeme[2]=='t' && lexeme[3]=='\0') return 1;
            if (lexeme[1]=='h' && lexeme[2]=='a' && lexeme[3]=='r' && lexeme[4]=='e' && lexeme[5]=='d' && lexeme[6]=='\0') return 1;
            if (lexeme[1]=='i' && lexeme[2]=='z' && lexeme[3]=='e' && lexeme[4]=='\0') return 1;
            break;
        case 't': /* true */
            if (lexeme[1]=='r' && lexeme[2]=='u' && lexeme[3]=='e' && lexeme[4]=='\0') return 1;
            break;
        case 'w': /* whole */
            if (lexeme[1]=='h' && lexeme[2]=='o' && lexeme[3]=='l' && lexeme[4]=='e' && lexeme[5]=='\0') return 1;
            break;
    }
    return 0;
}

int isNoiseWord(const char *lexeme) {
    if (!lexeme || !lexeme[0]) return 0;
    switch (lexeme[0]) {
        case 'f': /* from */
            if (lexeme[1]=='r' && lexeme[2]=='o' && lexeme[3]=='m' && lexeme[4]=='\0') return 1;
            break;
        case 't': /* then */
            if (lexeme[1]=='h' && lexeme[2]=='e' && lexeme[3]=='n' && lexeme[4]=='\0') return 1;
            break;
        case 'w': /* with */
            if (lexeme[1]=='i' && lexeme[2]=='t' && lexeme[3]=='h' && lexeme[4]=='\0') return 1;
            break;
    }
    return 0;
}

/* ============================
   GET NEXT TOKEN
   ============================ */
   
Token getNextToken(Lexer *lexer) {
    skipWhitespace(lexer);

    Token token;
    token.lexeme = NULL;
    token.type = TOKEN_UNKNOWN;

    char c = lexer->currentChar;

    if (c == '\0') {
        token.type = TOKEN_EOF;
        token.lexeme = (char *)malloc(4);
        strcpy(token.lexeme, "EOF");
        return token;
    }

    if (c == '/') {
        char nextChar = lexer->source[lexer->position + 1];
        if (nextChar == '/') {
            // Line comment
            advance(lexer); advance(lexer); // skip "//"
            int start = lexer->position;
            while (lexer->currentChar != '\0' && lexer->currentChar != '\n') advance(lexer);
            int len = lexer->position - start;
            token.lexeme = (char *)malloc(len + 1);
            for (int i = 0; i < len; i++) token.lexeme[i] = lexer->source[start + i];
            token.lexeme[len] = '\0';
            token.type = TOKEN_LINE_COMMENT;
            return token;
        } else if (nextChar == '*') {
            // Block comment
            advance(lexer); advance(lexer); // skip "/*"
            int start = lexer->position;
            while (!(lexer->currentChar == '*' && lexer->source[lexer->position + 1] == '/') && lexer->currentChar != '\0') advance(lexer);
            int len = lexer->position - start;
            token.lexeme = (char *)malloc(len + 1);
            for (int i = 0; i < len; i++) token.lexeme[i] = lexer->source[start + i];
            token.lexeme[len] = '\0';
            token.type = TOKEN_BLOCK_COMMENT;
            if (lexer->currentChar != '\0') { advance(lexer); advance(lexer); } // skip "*/"
            return token;
        }
    }
    if (c == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    /* Identifiers / Keywords / Reserved / Noise */
    if (isAlpha(c)) {
        int start = lexer->position;
        while (isAlpha(lexer->currentChar) || isDigit(lexer->currentChar)) {
            advance(lexer);
        }
        int len = lexer->position - start;
        token.lexeme = (char *)malloc(len + 1);
        for (int i = 0; i < len; i++) {
            token.lexeme[i] = lexer->source[start + i];
        }
        token.lexeme[len] = '\0';

        if (isKeyword(token.lexeme)) token.type = TOKEN_KEYWORD;
        else if (isReservedWord(token.lexeme)) token.type = TOKEN_RESERVEDWORD;
        else if (isNoiseWord(token.lexeme)) token.type = TOKEN_NOISEWORD;
        else token.type = TOKEN_IDENTIFIER;

        return token;
    }

    /* Numbers */
    if (isDigit(c)) {
        int start = lexer->position;
        int hasDot = 0;
        while (isDigit(lexer->currentChar) || (lexer->currentChar=='.' && !hasDot)) {
            if (lexer->currentChar=='.') hasDot = 1;
            advance(lexer);
        }
        int len = lexer->position - start;
        token.lexeme = (char *)malloc(len + 1);
        for (int i=0; i<len; i++) token.lexeme[i] = lexer->source[start + i];
        token.lexeme[len] = '\0';
        token.type = hasDot ? TOKEN_FLOAT_LITERAL : TOKEN_INT_LITERAL;
        return token;
    }

    /* Strings */
    if (c == '"') {
        advance(lexer); // skip "
        int start = lexer->position;
        while (lexer->currentChar != '"' && lexer->currentChar != '\0') advance(lexer);
        int len = lexer->position - start;
        token.lexeme = (char *)malloc(len + 1);
        for (int i=0; i<len; i++) token.lexeme[i] = lexer->source[start + i];
        token.lexeme[len] = '\0';
        token.type = TOKEN_STRING_LITERAL;
        advance(lexer); // skip closing "
        return token;
    }

    /* Delimiters */
    if (isDelimiter(c)) {
        advance(lexer);
        token.lexeme = (char *)malloc(2);
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';

        switch(c) {
            case '(': token.type = TOKEN_LEFT_PARENTHESIS; break;
            case ')': token.type = TOKEN_RIGHT_PARENTHESIS; break;
            case '{': token.type = TOKEN_LEFT_BRACE; break;
            case '}': token.type = TOKEN_RIGHT_BRACE; break;
            case '[': token.type = TOKEN_LEFT_BRACKET; break;
            case ']': token.type = TOKEN_RIGHT_BRACKET; break;
            case ',': token.type = TOKEN_COMMA; break;
            case ';': token.type = TOKEN_SEMICOLON; break;
            // case '.': token.type = TOKEN_DOT; break;
            case ':': token.type = TOKEN_COLON; break;
            case '"': token.type = TOKEN_STRING_QUOTATION; break;
            case '\'': token.type = TOKEN_CHAR_QUOTATION; break;
        }
        return token;
    }

    /* Operators (single-char) */
    if (isOperator(c)) {
    char nextChar = lexer->source[lexer->position + 1];

    token.lexeme = (char *)malloc(3);
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';

    int twoChar = 0; // flag for two-character operator

    /* Detect two-character operators */
    switch (c) {
        case '+':
            if (nextChar == '+' || nextChar == '=') twoChar = 1;
            break;

        case '-':
            if (nextChar == '-' || nextChar == '=') twoChar = 1;
            break;

        case '*':
            if (nextChar == '=') twoChar = 1;
            break;

        case '/':
            if (nextChar == '=') twoChar = 1;
            break;

        case '%':
            if (nextChar == '=') twoChar = 1;
            break;

        case '=':
            if (nextChar == '=') twoChar = 1;
            break;

        case '!':
            if (nextChar == '=') twoChar = 1;
            break;

        case '<':
            if (nextChar == '=') twoChar = 1;
            break;

        case '>':
            if (nextChar == '=') twoChar = 1;
            break;
        case '&':
            if (nextChar == '&') twoChar = 1;
            break;
        case '|':
            if (nextChar == '|') twoChar = 1;
            break;
    }

    /* Build lexeme if two-character operator */
    if (twoChar) {
        token.lexeme[1] = nextChar;
        token.lexeme[2] = '\0';
        advance(lexer); // consume second character
    }

    advance(lexer); // consume first character

    /* Assign token type */
    if (c == '+' && nextChar == '+') token.type = TOKEN_INCREMENT_OPERATOR;
    else if (c == '-' && nextChar == '-') token.type = TOKEN_DECREMENT_OPERATOR;
    else if (c == '+' && nextChar == '=') token.type = TOKEN_PLUS_ASSIGN_OPERATOR;
    else if (c == '-' && nextChar == '=') token.type = TOKEN_MINUS_ASSIGN_OPERATOR;
    else if (c == '*' && nextChar == '=') token.type = TOKEN_MULTIPLY_ASSIGN_OPERATOR;
    else if (c == '/' && nextChar == '=') token.type = TOKEN_DIVIDE_ASSIGN_OPERATOR;
    else if (c == '%' && nextChar == '=') token.type = TOKEN_MODULO_ASSIGN_OPERATOR;
    else if (c == '=' && nextChar == '=') token.type = TOKEN_EQUAL_OPERATOR;
    else if (c == '!' && nextChar == '=') token.type = TOKEN_NOT_EQUAL_OPERATOR;
    else if (c == '<' && nextChar == '=') token.type = TOKEN_LESS_EQUAL_OPERATOR;
    else if (c == '>' && nextChar == '=') token.type = TOKEN_GREATER_EQUAL_OPERATOR;
    else if (c == '&' && nextChar == '&') token.type = TOKEN_LOGICAL_AND_OPERATOR;
    else if (c == '|' && nextChar == '|') token.type = TOKEN_LOGICAL_OR_OPERATOR;

    /* Single-character operators */
    else {
        switch (c) {
            case '+': token.type = TOKEN_PLUS_OPERATOR; break;
            case '-': token.type = TOKEN_MINUS_OPERATOR; break;
            case '*': token.type = TOKEN_MULTIPLY_OPERATOR; break;
            case '/': token.type = TOKEN_DIVIDE_OPERATOR; break;
            case '%': token.type = TOKEN_MODULO_OPERATOR; break;
            case '=': token.type = TOKEN_ASSIGN_OPERATOR; break;
            case '!': token.type = TOKEN_LOGICAL_NOT_OPERATOR; break;
            case '<': token.type = TOKEN_LESS_THAN_OPERATOR; break;
            case '>': token.type = TOKEN_GREATER_THAN_OPERATOR; break;
        }
    }

    return token;
}

    /* Unknown character */
    token.lexeme = (char *)malloc(2);
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    advance(lexer);
    token.type = TOKEN_UNKNOWN;
    return token;
}