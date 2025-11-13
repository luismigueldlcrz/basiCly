#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
    TOKEN_KEYWORD,        // Keywords (e.g. digit, check, output, show, etc.)
    TOKEN_RESERVEDWORD,   // Reserved words (e.g. int, float, if, else, ...)
    TOKEN_NOISEWORD,      // Noise words to ignore (e.g. the, a, an, to, ...)
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,         // Integer and floating-point numbers
    TOKEN_LITERAL,        // String literal (e.g. "hello")
    TOKEN_OPERATOR,       // Operators
    TOKEN_DELIMITER,      // Delimiters (e.g. ; , . ( ) { } [ ])
    TOKEN_COMMENT,        // Comments (single-line and multi-line)
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Token Structure
typedef struct {
    TokenType type;
    char *lexeme;
} Token;

// Lexer state
typedef struct {
    const char *source;
    int position;
    char currentChar;
} Lexer;

// Function Prototypes
void initLexer(Lexer *lexer, const char *source);
void advance(Lexer *lexer);
void skipWhitespace(Lexer *lexer);
Token getNextToken(Lexer *lexer);

// Helper Functions
int isAlpha(char c);
int isDigit(char c);
int isOperator(char c);
int isDelimiter(char c);
int isKeyword(const char *lexeme);
int isReservedWord(const char *lexeme);
int isNoiseWord(const char *lexeme);

#endif