#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

/* ============================
   TOKEN TYPE ENUMERATION
   ============================ */

typedef enum {
    TOKEN_KEYWORD,        // Keywords (e.g. digit, check, output, show, etc.)
    TOKEN_RESERVEDWORD,   // Reserved words (e.g. int, float, if, else, ...)
    TOKEN_NOISEWORD,      // Noise words to ignore (e.g. the, a, an, to, ...)
    TOKEN_IDENTIFIER,

    /* Literals */
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,

    /* Operators */
    TOKEN_PLUS_OPERATOR,          // +
    TOKEN_MINUS_OPERATOR,         // -
    TOKEN_MULTIPLY_OPERATOR,      // *
    TOKEN_DIVIDE_OPERATOR,        // /
    TOKEN_MODULO_OPERATOR,        // %
    TOKEN_ASSIGN_OPERATOR,        // =
    TOKEN_LOGICAL_NOT_OPERATOR,   // !
    TOKEN_LESS_THAN_OPERATOR,     // <
    TOKEN_GREATER_THAN_OPERATOR,  // >

    /* Multi-character operators (we can add more later) */
    TOKEN_EQUAL_OPERATOR,            // ==
    TOKEN_PLUS_ASSIGN_OPERATOR,      // +=
    TOKEN_MINUS_ASSIGN_OPERATOR,     // -=
    TOKEN_MULTIPLY_ASSIGN_OPERATOR,  // *=
    TOKEN_DIVIDE_ASSIGN_OPERATOR,    // /=
    TOKEN_MODULO_ASSIGN_OPERATOR,    // %=
    TOKEN_NOT_EQUAL_OPERATOR,        // !=
    TOKEN_LESS_EQUAL_OPERATOR,       // <=
    TOKEN_GREATER_EQUAL_OPERATOR,    // >=
    TOKEN_LOGICAL_AND_OPERATOR,      // &&
    TOKEN_LOGICAL_OR_OPERATOR,       // ||
    TOKEN_INCREMENT_OPERATOR,        // ++
    TOKEN_DECREMENT_OPERATOR,        // --
    

    /* Delimiters */
    TOKEN_LEFT_PARENTHESIS,       // (
    TOKEN_RIGHT_PARENTHESIS,      // )
    TOKEN_LEFT_BRACE,             // {
    TOKEN_RIGHT_BRACE,            // }
    TOKEN_LEFT_BRACKET,           // [
    TOKEN_RIGHT_BRACKET,          // ]
    TOKEN_COMMA,                  // ,
    TOKEN_SEMICOLON,              // ;
   //  TOKEN_DOT,                    // .
    TOKEN_COLON,                  // :
    TOKEN_STRING_QUOTATION,       // "
   TOKEN_CHAR_QUOTATION,          // '

    /* Comments */
    TOKEN_LINE_COMMENT,           // //
    TOKEN_BLOCK_COMMENT,          // /* ... */

    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

/* ============================
   TOKEN STRUCTURE
   ============================ */

typedef struct {
    TokenType type;
    char *lexeme;
} Token;

/* ============================
   LEXER STRUCTURE
   ============================ */

typedef struct {
    const char *source;
    int position;
    char currentChar;
} Lexer;

/* ============================
   FUNCTION PROTOTYPES
   ============================ */

void initLexer(Lexer *lexer, const char *source);
void advance(Lexer *lexer);
void skipWhitespace(Lexer *lexer);
Token getNextToken(Lexer *lexer);

/* Character classification */
int isAlpha(char c);
int isDigit(char c);
int isOperator(char c);
int isDelimiter(char c);

/* Word classification */
int isKeyword(const char *lexeme);
int isReservedWord(const char *lexeme);
int isNoiseWord(const char *lexeme);

#endif