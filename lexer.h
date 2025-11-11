#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
    TOKEN_KEYWORD,  // Keywords (e.g. digit, shortdecimal, longdecimal, check, etc.)
    TOKEN_RESERVEDWORD, // Reserved words (e.g. , etc.)
    TOKEN_NOISEWORD,    // Noise words (e.g. , etc.)
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,  // Arithmetic operators (e.g. +, -, *, /, %), Relational operators (e.g. ==, !=, <, >, <=, >=), Logical operators (e.g. &&, ||, !), Assignment operator (e.g. =, +=, -=, /=, %=)
    TOKEN_DELIMITER,    // Delimiters (e.g. ;, ,, ., (), {}, [])
    TOKEN_COMMENT,   // Comments (e.g. // comment, /* comment */)
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Token Structure
typedef struct {
    TokenType type;
    char *lexeme;
} Token;

// Lexeme Structure
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

#endif