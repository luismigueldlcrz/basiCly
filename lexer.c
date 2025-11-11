#include "lexer.h"

void initLexer(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer-> currentChar = source[0];
}

void advance(Lexer *lexer) {
    lexer->position++;
    if(lexer->source[lexer->position] != '\0') {
        lexer->currentChar = lexer->source[lexer->position];
    } else {
        lexer->currentChar = '\0'; // End of source
    }
}

void skipWhitespace(Lexer *lexer) {
    while(lexer->currentChar == ' ' || lexer->currentChar == '\t' || lexer->currentChar == '\n' || lexer->currentChar == '\r') {
        advance(lexer);
    }
}

int isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

int isDigit(char c) {
    return (c >= '0' && c <= '9');
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
int isKeyword(const char *lexeme) {
    switch (lexeme[0])
    {
        case 'c':
        if(lexeme[1] == 'h' && lexeme[2] == 'e' && lexeme[3] == 'c' && lexeme[4] == 'k' && lexeme[5] == '\0') return 1; // check
        if(lexeme[1] == 'y' && lexeme[2] == 'c' && lexeme[3] == 'l' && lexeme[4] == 'e' && lexeme[5] == '\0') return 1; // cycle
        break;

        case 'd':
        if(lexeme[1] == 'i' && lexeme[2] == 'g' && lexeme[3] == 'i' && lexeme[4] == 't' && lexeme[5] == '\0') return 1; // digit
        if(lexeme[1] == 'u' && lexeme[2] == 'r' && lexeme[3] == 'i' && lexeme[4] == 'n' && lexeme[5] == 'g' && lexeme[6] == '\0') return 1; // during
        break;

        case 'i':
        if(lexeme[1] == 'n' && lexeme[2] == 'd' && lexeme[3] == 'i' && lexeme[4] == 'v' && lexeme[5] == '\0') return 1; // indiv
        if(lexeme[1] == 'n' && lexeme[2] == 's' && lexeme[3] == 't' && lexeme[4] == 'e' && lexeme[5] == 'a' && lexeme[6] == 'd' && lexeme[7] == '\0') return 1; // instead
        break;

        case 'l':
        if(lexeme[1] == 'a' && lexeme[2] == 'r' && lexeme[3] == 'g' && lexeme[4] == 'e' && lexeme[5] == '\0') return 1; // large
        if(lexeme[1] == 'o' && lexeme[2] == 'n' && lexeme[3] == 'g' && lexeme[4] == '\0') return 1; // long
        break;

        case 'o':
        if(lexeme[1] == 't' && lexeme[2] == 'h' && lexeme[3] == 'e' && lexeme[4] == 'r' && lexeme[5] == 'w' && lexeme[6] == 'i' && lexeme[7] == 's' && lexeme[8] == 'e' && lexeme[9] == '\0') return 1; // otherwise
        break;

        case 'p':
        if(lexeme[1] == 'e' && lexeme[2] == 'r' && lexeme[3] == 'f' && lexeme[4] == 'o' && lexeme[5] == 'r' && lexeme[6] == 'm' && lexeme[7] == '\0') return 1; // perform
        break;

        case 's':
        if(lexeme[1] == 'e' && lexeme[2] == 'l' && lexeme[3] == 'e' && lexeme[4] == 'c' && lexeme[5] == 't' && lexeme[6] == '\0') return 1; // select
        if(lexeme[1] == 'k' && lexeme[2] == 'i' && lexeme[3] == 'p' && lexeme[4] == '\0') return 1; // skip
        if(lexeme[1] == 'h' && lexeme[2] == 'o' && lexeme[3] == 'r' && lexeme[4] == 't' && lexeme[5] == '\0') return 1; // short
        if(lexeme[1] == 't' && lexeme[2] == 'o' && lexeme[3] == 'p' && lexeme[4] == '\0') return 1; // stop
        break;

        case 't':
        if(lexeme[1] == 'i' && lexeme[2] == 'n' && lexeme[3] == 'y' && lexeme[4] == '\0') return 1; // tiny
        break;
        
        case 'w':
        if(lexeme[1] == 'h' && lexeme[2] == 'e' && lexeme[3] == 'n' && lexeme[4] == '\0') return 1; // when
        break;
    }
    return 0;
}

Token getNextToken(Lexer *lexer) {
    Token token;
    token.lexeme = NULL;

    skipWhitespace(lexer);

    if(lexer->currentChar == '\0') {
        token.type = TOKEN_EOF;
        token.lexeme = strdup("EOF");
        return token;
    }

    if(isAlpha(lexer->currentChar) || isDigit(lexer)) {
        
    }

}