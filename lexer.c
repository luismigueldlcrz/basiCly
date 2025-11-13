#include "lexer.h"
#include <stdlib.h>
#include <string.h>

/* small helper to copy len chars and null-terminate (no strdup/strndup/strcmp) */
static char *copyLexeme(const char *src, int len) {
    char *p = (char *)malloc(len + 1);
    if (!p) return NULL;
    if (len > 0) memcpy(p, src, len);
    p[len] = '\0';
    return p;
}

/* helpers for UTF-8 curly single quotes detection (file-scope, no nested functions) */
static int utf8_has_three_bytes(const unsigned char *s, int p) {
    return s[p] != '\0' && s[p+1] != '\0' && s[p+2] != '\0';
}
static int utf8_left_single_at(const unsigned char *s, int p) {
    /* U+2018 = E2 80 98 */
    return utf8_has_three_bytes(s, p) && s[p] == 0xE2 && s[p+1] == 0x80 && s[p+2] == 0x98;
}
static int utf8_right_single_at(const unsigned char *s, int p) {
    /* U+2019 = E2 80 99 */
    return utf8_has_three_bytes(s, p) && s[p] == 0xE2 && s[p+1] == 0x80 && s[p+2] == 0x99;
}

void initLexer(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->currentChar = source[0];
}

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

int isAlpha(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc >= 'a' && uc <= 'z') || (uc >= 'A' && uc <= 'Z') || (uc == '_');
}

int isDigit(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc >= '0' && uc <= '9');
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
           c == '=' || c == '!' || c == '<' || c == '>' ||
           c == '&' || c == '|';
}

int isDelimiter(char c) {
    return c == ';' || c == ',' || c == '.' || c == '(' || c == ')' ||
           c == '{' || c == '}' || c == '[' || c == ']';
}

/* keyword detection using switch-based char-by-char matching */
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
        case 'i': /* indiv */
            if (lexeme[1]=='n' && lexeme[2]=='d' && lexeme[3]=='i' && lexeme[4]=='v' && lexeme[5]=='\0') return 1;
            break;
        case 'o': /* output */
            if (lexeme[1]=='u' && lexeme[2]=='t' && lexeme[3]=='p' && lexeme[4]=='u' && lexeme[5]=='t' && lexeme[6]=='\0') return 1;
            break;
        case 's': /* show */
            if (lexeme[1]=='h' && lexeme[2]=='o' && lexeme[3]=='w' && lexeme[4]=='\0') return 1;
            break;
    }
    return 0;
}

/* reserved words (e.g., data types and control structures) */
int isReservedWord(const char *lexeme) {
    if (!lexeme || !lexeme[0]) return 0;
    switch (lexeme[0]) {
        case 'i': /* int, if */
            if (lexeme[1]=='n' && lexeme[2]=='t' && lexeme[3]=='\0') return 1;
            if (lexeme[1]=='f' && lexeme[2]=='\0') return 1;
            break;
        case 'f': /* float, for */
            if (lexeme[1]=='l' && lexeme[2]=='o' && lexeme[3]=='a' && lexeme[4]=='t' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='o' && lexeme[2]=='r' && lexeme[3]=='\0') return 1;
            break;
        case 'c': /* char */
            if (lexeme[1]=='h' && lexeme[2]=='a' && lexeme[3]=='r' && lexeme[4]=='\0') return 1;
            break;
        case 'v': /* void */
            if (lexeme[1]=='o' && lexeme[2]=='i' && lexeme[3]=='d' && lexeme[4]=='\0') return 1;
            break;
        case 'r': /* return */
            if (lexeme[1]=='e' && lexeme[2]=='t' && lexeme[3]=='u' && lexeme[4]=='r' && lexeme[5]=='n' && lexeme[6]=='\0') return 1;
            break;
        case 'e': /* else */
            if (lexeme[1]=='l' && lexeme[2]=='s' && lexeme[3]=='e' && lexeme[4]=='\0') return 1;
            break;
        case 'w': /* while */
            if (lexeme[1]=='h' && lexeme[2]=='i' && lexeme[3]=='l' && lexeme[4]=='e' && lexeme[5]=='\0') return 1;
            break;
    }
    return 0;
}

/* noise words to ignore in final output */
int isNoiseWord(const char *lexeme) {
    if (!lexeme || !lexeme[0]) return 0;
    switch (lexeme[0]) {
        case 'a':
            if (lexeme[1]=='\0') return 1; /* "a" */
            if (lexeme[1]=='n' && lexeme[2]=='\0') return 1; /* "an" */
            break;
        case 't':
            if (lexeme[1]=='h' && lexeme[2]=='e' && lexeme[3]=='\0') return 1; /* the */
            if (lexeme[1]=='o' && lexeme[2]=='\0') return 1; /* to */
            break;
        case 'o':
            if (lexeme[1]=='f' && lexeme[2]=='\0') return 1; /* of */
            break;
        case 'i':
            if (lexeme[1]=='s' && lexeme[2]=='\0') return 1; /* is */
            if (lexeme[1]=='n' && lexeme[2]=='\0') return 1; /* in */
            break;
        case 'b':
            if (lexeme[1]=='y' && lexeme[2]=='\0') return 1; /* by */
            break;
        case 'f':
            if (lexeme[1]=='o' && lexeme[2]=='r' && lexeme[3]=='\0') return 1; /* for */
            break;
        case 'w':
            if (lexeme[1]=='i' && lexeme[2]=='t' && lexeme[3]=='h' && lexeme[4]=='\0') return 1; /* with */
            break;
    }
    return 0;
}

/* check two-character operator pairs without strcmp */
static int matchTwoCharOp(char a, char b) {
    /* list of supported two-char operators */
    const char pairs[][2] = {
        {'=', '='}, {'!', '='}, {'<', '='}, {'>', '='},
        {'&', '&'}, {'|', '|'}, {'+', '+'}, {'-', '-'},
        {'+', '='}, {'-', '='}, {'*', '='}, {'/', '='}, {'%', '='}
    };
    for (size_t i = 0; i < sizeof(pairs)/sizeof(pairs[0]); ++i) {
        if (pairs[i][0] == a && pairs[i][1] == b) return 1;
    }
    return 0;
}

Token getNextToken(Lexer *lexer) {
    Token token;
    token.lexeme = NULL;
    token.type = TOKEN_UNKNOWN;

    skipWhitespace(lexer);

    /* EOF */
    if (lexer->currentChar == '\0') {
        token.type = TOKEN_EOF;
        token.lexeme = copyLexeme("EOF", 3);
        return token;
    }

    /* Comments */
    if (lexer->currentChar == '#') { /* single-line comment until newline */
        int start = lexer->position;
        advance(lexer);
        while (lexer->currentChar != '\0' && lexer->currentChar != '\n') advance(lexer);
        int len = lexer->position - start;
        token.lexeme = copyLexeme(lexer->source + start, len);
        token.type = TOKEN_COMMENT;
        return token;
    }
    if (lexer->currentChar == '~' && lexer->source[lexer->position + 1] == '^') { /* multi-line ~^ ... ^~ */
        int start = lexer->position;
        advance(lexer); /* ~ */
        advance(lexer); /* ^ */
        while (lexer->currentChar != '\0') {
            if (lexer->currentChar == '^' && lexer->source[lexer->position + 1] == '~') {
                advance(lexer); /* ^ */
                advance(lexer); /* ~ */
                break;
            }
            advance(lexer);
        }
        int len = lexer->position - start;
        token.lexeme = copyLexeme(lexer->source + start, len);
        token.type = TOKEN_COMMENT;
        return token;
    }

    /* String literal (double quotes) */
    if (lexer->currentChar == '"') {
        int start = lexer->position;
        advance(lexer); /* consume opening " */
        while (lexer->currentChar != '\0') {
            if (lexer->currentChar == '\\') { /* escape sequence: include next char */
                advance(lexer);
                if (lexer->currentChar == '\0') break;
                advance(lexer);
                continue;
            }
            if (lexer->currentChar == '"') {
                advance(lexer); /* consume closing " */
                break;
            }
            advance(lexer);
        }
        int len = lexer->position - start;
        token.lexeme = copyLexeme(lexer->source + start, len);
        token.type = TOKEN_LITERAL;
        return token;
    }

    /* Single-quoted char literal (accept ASCII ' and curly quotes U+2018/U+2019 in UTF-8) */
    {
        const unsigned char *src = (const unsigned char *)lexer->source;
        int pos = lexer->position;

        if ( (src[pos] == (unsigned char)'\'' ) || utf8_left_single_at(src, pos) ) {
            int start = pos;

            /* consume opening quote: ASCII (1 byte) or UTF-8 left single (3 bytes) */
            if (src[pos] == (unsigned char)'\'' ) {
                advance(lexer);
            } else { /* UTF-8 left single */
                advance(lexer); advance(lexer); advance(lexer);
            }

            /* collect until matching closing quote */
            while (lexer->currentChar != '\0') {
                /* ASCII close */
                if ((unsigned char)lexer->currentChar == (unsigned char)'\'' ) {
                    advance(lexer);
                    break;
                }
                /* UTF-8 right single close? */
                if (utf8_right_single_at(src, lexer->position)) {
                    advance(lexer); advance(lexer); advance(lexer);
                    break;
                }
                /* escape */
                if (lexer->currentChar == '\\') {
                    advance(lexer);
                    if (lexer->currentChar == '\0') break;
                    advance(lexer);
                    continue;
                }
                advance(lexer);
            }

            int len = lexer->position - start;
            token.lexeme = copyLexeme(lexer->source + start, len);
            token.type = TOKEN_LITERAL;
            return token;
        }
    }

    /* Identifier / Keyword / Reserved / Noise */
    if (isAlpha(lexer->currentChar)) {
        int start = lexer->position;
        while (isAlpha(lexer->currentChar) || isDigit(lexer->currentChar)) advance(lexer);
        int len = lexer->position - start;
        token.lexeme = copyLexeme(lexer->source + start, len);
        if (isNoiseWord(token.lexeme)) {
            token.type = TOKEN_NOISEWORD;
        } else if (isKeyword(token.lexeme)) {
            token.type = TOKEN_KEYWORD;
        } else if (isReservedWord(token.lexeme)) {
            token.type = TOKEN_RESERVEDWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    /* Number */
    if (isDigit(lexer->currentChar) || (lexer->currentChar == '-' && isDigit(lexer->source[lexer->position + 1]))) {
        int start = lexer->position;
        if (lexer->currentChar == '-') advance(lexer); /* include negative sign */
        while (isDigit(lexer->currentChar)) advance(lexer);
        /* optional fractional part */
        if (lexer->currentChar == '.') {
            advance(lexer);
            while (isDigit(lexer->currentChar)) advance(lexer);
        }
        int len = lexer->position - start;
        token.lexeme = copyLexeme(lexer->source + start, len);
        token.type = TOKEN_NUMBER;
        return token;
    }

    /* Operator (single or two-char) */
    if (isOperator(lexer->currentChar)) {
        char c = lexer->currentChar;
        char next = lexer->source[lexer->position + 1];
        if (next != '\0' && matchTwoCharOp(c, next)) {
            token.lexeme = (char *)malloc(3);
            token.lexeme[0] = c;
            token.lexeme[1] = next;
            token.lexeme[2] = '\0';
            token.type = TOKEN_OPERATOR;
            advance(lexer); /* consume first */
            advance(lexer); /* consume second */
            return token;
        } else {
            token.lexeme = (char *)malloc(2);
            token.lexeme[0] = c;
            token.lexeme[1] = '\0';
            token.type = TOKEN_OPERATOR;
            advance(lexer);
            return token;
        }
    }

    /* Delimiter */
    if (isDelimiter(lexer->currentChar)) {
        token.lexeme = (char *)malloc(2);
        token.lexeme[0] = lexer->currentChar;
        token.lexeme[1] = '\0';
        token.type = TOKEN_DELIMITER;
        advance(lexer);
        return token;
    }

    /* Unknown single character */
    token.lexeme = (char *)malloc(2);
    token.lexeme[0] = lexer->currentChar;
    token.lexeme[1] = '\0';
    token.type = TOKEN_UNKNOWN;
    advance(lexer);
    return token;
}