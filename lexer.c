#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* add if missing */

/* ============================
   TOKEN DEFINITIONS
   ============================ */
typedef enum {
    // End / Unknown
    TOKEN_EOF,
    TOKEN_UNKNOWN,

    // Identifiers & words
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,       // draft detection (you fill in isKeyword)
    TOKEN_RESERVEDWORD,  // implemented below
    TOKEN_NOISEWORD,     // implemented below

    // Literals
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,

    // Comments
    TOKEN_LINE_COMMENT,
    TOKEN_BLOCK_COMMENT,

    // Delimiters
    TOKEN_LEFT_PARENTHESIS,
    TOKEN_RIGHT_PARENTHESIS,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_DOT,

    // Quote delimiter tokens (explicit)
    TOKEN_LEFT_STRING_QUOTATION,   // opening double-quote: "
    TOKEN_RIGHT_STRING_QUOTATION,  // closing double-quote: "
    TOKEN_LEFT_CHAR_QUOTATION,     // opening single-quote: '
    TOKEN_RIGHT_CHAR_QUOTATION,    // closing single-quote: '

    // Operators (explicit)
    TOKEN_PLUS_OPERATOR,
    TOKEN_MINUS_OPERATOR,
    TOKEN_MULTIPLY_OPERATOR,
    TOKEN_DIVIDE_OPERATOR,
    TOKEN_MODULO_OPERATOR,
    TOKEN_EXPONENT_OPERATOR,         // **
    TOKEN_DIV_OPERATOR,              // $

    TOKEN_ASSIGN_OPERATOR,           // =
    TOKEN_EQUAL_OPERATOR,            // ==
    TOKEN_LOGICAL_NOT_OPERATOR,      // !
    TOKEN_NOT_EQUAL_OPERATOR,        // !=
    TOKEN_LESS_THAN_OPERATOR,        // <
    TOKEN_GREATER_THAN_OPERATOR,     // >
    TOKEN_LESS_EQUAL_OPERATOR,       // <=
    TOKEN_GREATER_EQUAL_OPERATOR,    // >=

    TOKEN_INCREMENT_OPERATOR,        // ++
    TOKEN_DECREMENT_OPERATOR,        // --
    TOKEN_PLUS_ASSIGN_OPERATOR,      // +=
    TOKEN_MINUS_ASSIGN_OPERATOR,     // -=
    TOKEN_MULTIPLY_ASSIGN_OPERATOR,  // *=
    TOKEN_DIVIDE_ASSIGN_OPERATOR,    // /=
    TOKEN_MODULO_ASSIGN_OPERATOR,    // %=

    TOKEN_LOGICAL_AND_OPERATOR,      // &&
    TOKEN_LOGICAL_OR_OPERATOR        // ||
} TokenType;

/* ============================
   TOKEN STRUCT
   ============================ */
typedef struct {
    TokenType type;
    char *lexeme; // null-terminated string (malloc'd)
} Token;

/* ============================
   LEXER STRUCT (with quote state)
   ============================ */
typedef struct {
    const char *source;
    int position;
    char currentChar;

    /* state for multi-step quote token emission */
    int inStringContent;           // set after LEFT_STRING_QUOTATION, before returning STRING_LITERAL
    int pendingRightStringQuote;   // set after STRING_LITERAL to emit RIGHT_STRING_QUOTATION next

    int inCharContent;             // set after LEFT_CHAR_QUOTATION
    int pendingRightCharQuote;     // set after CHAR_LITERAL to emit RIGHT_CHAR_QUOTATION next
} Lexer;

/* ============================
   LEXER INIT / UTIL
   ============================ */
void initLexer(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->currentChar = source[0];
    lexer->inStringContent = 0;
    lexer->pendingRightStringQuote = 0;
    lexer->inCharContent = 0;
    lexer->pendingRightCharQuote = 0;
}

void advance(Lexer *lexer) {
    lexer->position++;
    lexer->currentChar = lexer->source[lexer->position];
}

char peek(Lexer *lexer) {
    return lexer->source[lexer->position + 1];
}

void skipWhitespace(Lexer *lexer) {
    /* don't skip inside content or pending closing quote */
    if (lexer->inStringContent || lexer->inCharContent || lexer->pendingRightStringQuote || lexer->pendingRightCharQuote) return;
    while (lexer->currentChar == ' ' ||
           lexer->currentChar == '\t' ||
           lexer->currentChar == '\n' ||
           lexer->currentChar == '\r') {
        advance(lexer);
    }
}

/* ============================
   CHARACTER CLASS HELPERS
   ============================ */
int isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

int isDigit(char c) {
    return (c >= '0' && c <= '9');
}

int isOperatorChar(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
           c == '=' || c == '<' || c == '>' || c == '!' || c == '&' ||
           c == '|' || c == '$';
}

int isDelimiterChar(char c) {
    /* do not treat quotes here — handled separately */
    return c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ',' || c == ';' ||
           c == '.' || c == ':';
}

/* ============================
   WORD CLASSIFICATION (LETTER-BY-LETTER)
   ============================ */
/* ...existing isKeyword/isReservedWord/isNoiseWord implementations... */
int isKeyword(const char *lexeme) {
    if (!lexeme || !lexeme[0]) return 0;
    switch (lexeme[0]) {
        case 'c': //check, cycle
            if (lexeme[1]=='h' && lexeme[2]=='e' && lexeme[3]=='c' && lexeme[4]=='k' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='y' && lexeme[2]=='c' && lexeme[3]=='l' && lexeme[4]=='e' && lexeme[5]=='\0') return 1;
            break;
        case 'd': // digit, during
            if (lexeme[1]=='i' && lexeme[2]=='g' && lexeme[3]=='i' && lexeme[4]=='t' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='u' && lexeme[2]=='r' && lexeme[3]=='i' && lexeme[4]=='n' && lexeme[5]=='g' && lexeme[6]=='\0') return 1;
            break;
        case 'i': // indiv, instead
            if (lexeme[1]=='n' && lexeme[2]=='d' && lexeme[3]=='i' && lexeme[4]=='v' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='n' && lexeme[2]=='s' && lexeme[3]=='t' && lexeme[4]=='e' && lexeme[5]=='a' && lexeme[6]=='d' && lexeme[7]=='\0') return 1;
            break;
        case 'l': // large, long
            if (lexeme[1]=='a' && lexeme[2]=='r' && lexeme[3]=='g' && lexeme[4]=='e' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='o' && lexeme[2]=='n' && lexeme[3]=='g' && lexeme[4]=='\0') return 1;
            break;
        case 'o': // otherwise
            if (lexeme[1]=='t' && lexeme[2]=='h' && lexeme[3]=='e' && lexeme[4]=='r' && lexeme[5]=='w' && lexeme[6]=='i' && lexeme[7]=='s' && lexeme[8]=='e' && lexeme[9]=='\0') return 1;
            break;
        case 'p': // perform
            if (lexeme[1]=='e' && lexeme[2]=='r' && lexeme[3]=='f' && lexeme[4]=='o' && lexeme[5]=='r' && lexeme[6]=='m' && lexeme[7]=='\0') return 1;
            break;
        case 's': // select, short, skip, stop
            if (lexeme[1]=='e' && lexeme[2]=='l' && lexeme[3]=='e' && lexeme[4]=='c' && lexeme[5]=='t' && lexeme[6]=='\0') return 1;
            if (lexeme[1]=='h' && lexeme[2]=='o' && lexeme[3]=='r' && lexeme[4]=='t' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='k' && lexeme[2]=='i' && lexeme[3]=='p' && lexeme[4]=='\0') return 1;
            if (lexeme[1]=='t' && lexeme[2]=='o' && lexeme[3]=='p' && lexeme[4]=='\0') return 1;
            break;
        case 't': // tiny
            if (lexeme[1]=='i' && lexeme[2]=='n' && lexeme[3]=='y' && lexeme[4]=='\0') return 1;
            break;
        case 'w': // when
            if (lexeme[1]=='h' && lexeme[2]=='e' && lexeme[3]=='n' && lexeme[4]=='\0') return 1;
            break;
    }
    return 0;
}

int isReservedWord(const char *lexeme) {
    if (!lexeme || !lexeme[0]) return 0;
    switch (lexeme[0]) {
        case 'a': // alias
            if (lexeme[1]=='l' && lexeme[2]=='i' && lexeme[3]=='a' && lexeme[4]=='s' && lexeme[5]=='\0') return 1;
            break;
        case 'b': // blank, bothsign
            if (lexeme[1]=='l' && lexeme[2]=='a' && lexeme[3]=='n' && lexeme[4]=='k' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='o' && lexeme[2]=='t' && lexeme[3]=='h' && lexeme[4]=='s' && lexeme[5]=='i' && lexeme[6]=='g' && lexeme[7]=='n' && lexeme[8]=='\0') return 1;
            break;
        case 'f': // false, fixed
            if (lexeme[1]=='a' && lexeme[2]=='l' && lexeme[3]=='s' && lexeme[4]=='e' && lexeme[5]=='\0') return 1;
            if (lexeme[1]=='i' && lexeme[2]=='x' && lexeme[3]=='e' && lexeme[4]=='d' && lexeme[5]=='\0') return 1;
            break;
        case 'g': // group
            if (lexeme[1]=='r' && lexeme[2]=='o' && lexeme[3]=='u' && lexeme[4]=='p' && lexeme[5]=='\0') return 1;
            break;
        case 'j': // jumpto
            if (lexeme[1]=='u' && lexeme[2]=='m' && lexeme[3]=='p' && lexeme[4]=='t' && lexeme[5]=='o' && lexeme[6]=='\0') return 1;
            break;
        case 'k': // keep
            if (lexeme[1]=='e' && lexeme[2]=='e' && lexeme[3]=='p' && lexeme[4]=='\0') return 1;
            break;
        case 'l': // link, live, local
            if (lexeme[1]=='i' && lexeme[2]=='n' && lexeme[3]=='k' && lexeme[4]=='\0') return 1;
            if (lexeme[1]=='i' && lexeme[2]=='v' && lexeme[3]=='e' && lexeme[4]=='\0') return 1;
            if (lexeme[1]=='o' && lexeme[2]=='c' && lexeme[3]=='a' && lexeme[4]=='l' && lexeme[5]=='\0') return 1;
            break;
        case 'o': // output
            if (lexeme[1]=='u' && lexeme[2]=='t' && lexeme[3]=='p' && lexeme[4]=='u' && lexeme[5]=='t' && lexeme[6]=='\0') return 1;
            break;
        case 'r': // register
            if (lexeme[1]=='e' && lexeme[2]=='g' && lexeme[3]=='i' && lexeme[4]=='s' && lexeme[5]=='t' && lexeme[6]=='e' && lexeme[7]=='r' && lexeme[8]=='\0') return 1;
            break;
        case 's': // set, shared, size
            if (lexeme[1]=='e' && lexeme[2]=='t' && lexeme[3]=='\0') return 1;
            if (lexeme[1]=='h' && lexeme[2]=='a' && lexeme[3]=='r' && lexeme[4]=='e' && lexeme[5]=='d' && lexeme[6]=='\0') return 1;
            if (lexeme[1]=='i' && lexeme[2]=='z' && lexeme[3]=='e' && lexeme[4]=='\0') return 1;
            break;
        case 't': // true
            if (lexeme[1]=='r' && lexeme[2]=='u' && lexeme[3]=='e' && lexeme[4]=='\0') return 1;
            break;
        case 'w': // whole
            if (lexeme[1]=='h' && lexeme[2]=='o' && lexeme[3]=='l' && lexeme[4]=='e' && lexeme[5]=='\0') return 1;
            break;
    }
    return 0;
}

int isNoiseWord(const char *lexeme) {
    if (!lexeme || !lexeme[0]) return 0;
    switch (lexeme[0]) {
        case 'f': // from
            if (lexeme[1]=='r' && lexeme[2]=='o' && lexeme[3]=='m' && lexeme[4]=='\0') return 1;
            break;
        case 't': // then
            if (lexeme[1]=='h' && lexeme[2]=='e' && lexeme[3]=='n' && lexeme[4]=='\0') return 1;
            break;
        case 'w': // with
            if (lexeme[1]=='i' && lexeme[2]=='t' && lexeme[3]=='h' && lexeme[4]=='\0') return 1;
            break;
    }
    return 0;
}

/* ============================
   GET NEXT TOKEN
   ============================ */
Token getNextToken(Lexer *lexer) {
    /* Emit pending right-quote tokens first (if any) */
    Token token;
    token.lexeme = NULL;
    token.type = TOKEN_UNKNOWN;

    if (lexer->pendingRightStringQuote) {
        /* currentChar should be '"' if well-formed */
        token.lexeme = (char *)malloc(2); token.lexeme[0] = '"'; token.lexeme[1] = '\0';
        token.type = TOKEN_RIGHT_STRING_QUOTATION;
        lexer->pendingRightStringQuote = 0;
        /* consume closing quote */
        if (lexer->currentChar == '"') advance(lexer);
        return token;
    }
    if (lexer->pendingRightCharQuote) {
        token.lexeme = (char *)malloc(2); token.lexeme[0] = '\''; token.lexeme[1] = '\0';
        token.type = TOKEN_RIGHT_CHAR_QUOTATION;
        lexer->pendingRightCharQuote = 0;
        if (lexer->currentChar == '\'') advance(lexer);
        return token;
    }

    /* If we are inside a previously-opened string, return only the content token (do NOT consume closing quote) */
    if (lexer->inStringContent) {
        int start = lexer->position;
        while (lexer->currentChar != '\0') {
            if (lexer->currentChar == '\\' && peek(lexer) != '\0') {
                /* include escaped char in the content */
                advance(lexer); advance(lexer);
                continue;
            }
            if (lexer->currentChar == '"') {
                /* stop before closing quote */
                break;
            }
            advance(lexer);
        }
        int len = lexer->position - start;
        token.lexeme = (char *)malloc(len + 1);
        for (int i = 0; i < len; i++) token.lexeme[i] = lexer->source[start + i];
        token.lexeme[len] = '\0';
        token.type = TOKEN_STRING_LITERAL;
        lexer->inStringContent = 0;
        lexer->pendingRightStringQuote = (lexer->currentChar == '"') ? 1 : 0;
        return token;
    }

    /* If we are inside a previously-opened char, return the CHAR_LITERAL (do NOT consume closing ') */
    if (lexer->inCharContent) {
        int start = lexer->position;
        if (lexer->currentChar == '\\' && peek(lexer) != '\0') {
            advance(lexer); advance(lexer);
        } else {
            if (lexer->currentChar != '\0') advance(lexer);
        }
        int len = lexer->position - start;
        if (len < 0) len = 0;
        token.lexeme = (char *)malloc(len + 1);
        for (int i = 0; i < len; i++) token.lexeme[i] = lexer->source[start + i];
        token.lexeme[len] = '\0';
        token.type = TOKEN_CHAR_LITERAL;
        lexer->inCharContent = 0;
        lexer->pendingRightCharQuote = (lexer->currentChar == '\'') ? 1 : 0;
        return token;
    }

    /* Normal flow */
    skipWhitespace(lexer);

    char c = lexer->currentChar;

    if (c == '\0') {
        token.type = TOKEN_EOF;
        token.lexeme = (char *)malloc(4);
        token.lexeme[0] = 'E'; token.lexeme[1] = 'O'; token.lexeme[2] = 'F'; token.lexeme[3] = '\0';
        return token;
    }

    /* COMMENTS (handled first because '/' can be operator) */
    if (c == '/') {
        char nextChar = peek(lexer);
        if (nextChar == '/') {
            // Line comment: consume '//', then until newline or EOF
            advance(lexer); advance(lexer); // skip '/'
            int start = lexer->position;
            while (lexer->currentChar != '\0' && lexer->currentChar != '\n') advance(lexer);
            int len = lexer->position - start;
            token.lexeme = (char *)malloc(len + 1);
            for (int i = 0; i < len; i++) token.lexeme[i] = lexer->source[start + i];
            token.lexeme[len] = '\0';
            token.type = TOKEN_LINE_COMMENT;
            return token;
        } else if (nextChar == '*') {
            // Block comment: consume '/*' ... '*/'
            advance(lexer); advance(lexer); // skip '/*'
            int start = lexer->position;
            while (!(lexer->currentChar == '*' && peek(lexer) == '/') && lexer->currentChar != '\0') {
                advance(lexer);
            }
            int len = lexer->position - start;
            token.lexeme = (char *)malloc(len + 1);
            for (int i = 0; i < len; i++) token.lexeme[i] = lexer->source[start + i];
            token.lexeme[len] = '\0';
            token.type = TOKEN_BLOCK_COMMENT;
            if (lexer->currentChar != '\0') { advance(lexer); advance(lexer); } // skip '*/'
            return token;
        }
    }

    /* QUOTE DELIMITERS: opening quotes create delimiter tokens and enter content mode */
    if (c == '"') {
        token.lexeme = (char *)malloc(2); token.lexeme[0] = '"'; token.lexeme[1] = '\0';
        token.type = TOKEN_LEFT_STRING_QUOTATION;
        advance(lexer); /* consume opening quote */
        lexer->inStringContent = 1;
        lexer->pendingRightStringQuote = 0;
        return token;
    }
    if (c == '\'') {
        token.lexeme = (char *)malloc(2); token.lexeme[0] = '\''; token.lexeme[1] = '\0';
        token.type = TOKEN_LEFT_CHAR_QUOTATION;
        advance(lexer); /* consume opening quote */
        lexer->inCharContent = 1;
        lexer->pendingRightCharQuote = 0;
        return token;
    }

    /* IDENTIFIERS / KEYWORD / RESERVED / NOISE */
    if (isAlpha(c)) {
        int start = lexer->position;
        while (isAlpha(lexer->currentChar) || isDigit(lexer->currentChar)) {
            advance(lexer);
        }
        int len = lexer->position - start;
        token.lexeme = (char *)malloc(len + 1);
        for (int i = 0; i < len; i++) token.lexeme[i] = lexer->source[start + i];
        token.lexeme[len] = '\0';

        /* check order: noise -> reserved -> keyword -> identifier */
        if (isNoiseWord(token.lexeme)) token.type = TOKEN_NOISEWORD;
        else if (isReservedWord(token.lexeme)) token.type = TOKEN_RESERVEDWORD;
        else if (isKeyword(token.lexeme)) token.type = TOKEN_KEYWORD;
        else token.type = TOKEN_IDENTIFIER;

        return token;
    }

    /* NUMBERS: INT or FLOAT */
    if (isDigit(c)) {
        int start = lexer->position;
        int hasDot = 0;
        while (isDigit(lexer->currentChar) || (lexer->currentChar == '.' && !hasDot)) {
            if (lexer->currentChar == '.') {
                hasDot = 1;
            }
            advance(lexer);
        }
        int len = lexer->position - start;
        token.lexeme = (char *)malloc(len + 1);
        for (int i = 0; i < len; i++) token.lexeme[i] = lexer->source[start + i];
        token.lexeme[len] = '\0';
        token.type = hasDot ? TOKEN_FLOAT_LITERAL : TOKEN_INT_LITERAL;
        return token;
    }

    /* DELIMITERS (not quotes) */
    if (isDelimiterChar(c)) {
        advance(lexer);
        token.lexeme = (char *)malloc(2);
        token.lexeme[0] = c; token.lexeme[1] = '\0';
        switch (c) {
            case '(' : token.type = TOKEN_LEFT_PARENTHESIS; break;
            case ')' : token.type = TOKEN_RIGHT_PARENTHESIS; break;
            case '{' : token.type = TOKEN_LEFT_BRACE; break;
            case '}' : token.type = TOKEN_RIGHT_BRACE; break;
            case '[' : token.type = TOKEN_LEFT_BRACKET; break;
            case ']' : token.type = TOKEN_RIGHT_BRACKET; break;
            case ',' : token.type = TOKEN_COMMA; break;
            case ';' : token.type = TOKEN_SEMICOLON; break;
            case ':' : token.type = TOKEN_COLON; break;
            case '.' : token.type = TOKEN_DOT; break;
            default: token.type = TOKEN_UNKNOWN; break;
        }
        return token;
    }

    /* OPERATORS (single and two-character) */
    if (isOperatorChar(c)) {
        char next = peek(lexer);
        // prepare lexeme buffer (max 3 chars)
        token.lexeme = (char *)malloc(3);
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';

        int twoChar = 0;

        switch (c) {
            case '+': if (next == '+' || next == '=') twoChar = 1; break;
            case '-': if (next == '-' || next == '=') twoChar = 1; break;
            case '*': if (next == '=' || next == '*') twoChar = 1; break;
            case '/': if (next == '=') twoChar = 1; break;
            case '%': if (next == '=') twoChar = 1; break;
            case '=': if (next == '=') twoChar = 1; break;
            case '!': if (next == '=') twoChar = 1; break;
            case '<': if (next == '=') twoChar = 1; break;
            case '>': if (next == '=') twoChar = 1; break;
            case '&': if (next == '&') twoChar = 1; break;
            case '|': if (next == '|') twoChar = 1; break;
        }

        if (twoChar) {
            token.lexeme[1] = next; token.lexeme[2] = '\0';
            advance(lexer); // consume second char
        }
        advance(lexer); // consume first (or only) char

        // assign token type based on characters (explicit)
        if (token.lexeme[0] == '+' && token.lexeme[1] == '+') token.type = TOKEN_INCREMENT_OPERATOR;
        else if (token.lexeme[0] == '-' && token.lexeme[1] == '-') token.type = TOKEN_DECREMENT_OPERATOR;
        else if (token.lexeme[0] == '+' && token.lexeme[1] == '=') token.type = TOKEN_PLUS_ASSIGN_OPERATOR;
        else if (token.lexeme[0] == '-' && token.lexeme[1] == '=') token.type = TOKEN_MINUS_ASSIGN_OPERATOR;
        else if (token.lexeme[0] == '*' && token.lexeme[1] == '=') token.type = TOKEN_MULTIPLY_ASSIGN_OPERATOR;
        else if (token.lexeme[0] == '*' && token.lexeme[1] == '*') token.type = TOKEN_EXPONENT_OPERATOR;
        else if (token.lexeme[0] == '/' && token.lexeme[1] == '=') token.type = TOKEN_DIVIDE_ASSIGN_OPERATOR;
        else if (token.lexeme[0] == '%' && token.lexeme[1] == '=') token.type = TOKEN_MODULO_ASSIGN_OPERATOR;
        else if (token.lexeme[0] == '=' && token.lexeme[1] == '=') token.type = TOKEN_EQUAL_OPERATOR;
        else if (token.lexeme[0] == '!' && token.lexeme[1] == '=') token.type = TOKEN_NOT_EQUAL_OPERATOR;
        else if (token.lexeme[0] == '<' && token.lexeme[1] == '=') token.type = TOKEN_LESS_EQUAL_OPERATOR;
        else if (token.lexeme[0] == '>' && token.lexeme[1] == '=') token.type = TOKEN_GREATER_EQUAL_OPERATOR;
        else if (token.lexeme[0] == '&' && token.lexeme[1] == '&') token.type = TOKEN_LOGICAL_AND_OPERATOR;
        else if (token.lexeme[0] == '|' && token.lexeme[1] == '|') token.type = TOKEN_LOGICAL_OR_OPERATOR;

        else {
            // single-char operators
            switch (token.lexeme[0]) {
                case '+' : token.type = TOKEN_PLUS_OPERATOR; break;
                case '-' : token.type = TOKEN_MINUS_OPERATOR; break;
                case '*' : token.type = TOKEN_MULTIPLY_OPERATOR; break;
                case '/' : token.type = TOKEN_DIVIDE_OPERATOR; break;
                case '%' : token.type = TOKEN_MODULO_OPERATOR; break;
                case '$' : token.type = TOKEN_DIV_OPERATOR; break;
                case '=' : token.type = TOKEN_ASSIGN_OPERATOR; break;
                case '!' : token.type = TOKEN_LOGICAL_NOT_OPERATOR; break;
                case '<' : token.type = TOKEN_LESS_THAN_OPERATOR; break;
                case '>' : token.type = TOKEN_GREATER_THAN_OPERATOR; break;
                default: token.type = TOKEN_UNKNOWN; break;
            }
        }
        return token;
    }

    /* UNKNOWN CHARACTER - return as TOKEN_UNKNOWN with lexeme */
    token.lexeme = (char *)malloc(2);
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    token.type = TOKEN_UNKNOWN;
    advance(lexer);
    return token;
}

/* ============================
   TOKEN NAME MAP
   ============================ */
const char *tokenTypeName(TokenType t) {
    switch (t) {
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_RESERVEDWORD: return "RESERVEDWORD";
        case TOKEN_NOISEWORD: return "NOISEWORD";
        case TOKEN_INT_LITERAL: return "INT_LITERAL";
        case TOKEN_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TOKEN_STRING_LITERAL: return "STRING_LITERAL";
        case TOKEN_CHAR_LITERAL: return "CHAR_LITERAL";
        case TOKEN_LINE_COMMENT: return "LINE_COMMENT";
        case TOKEN_BLOCK_COMMENT: return "BLOCK_COMMENT";
        case TOKEN_LEFT_PARENTHESIS: return "LEFT_PAREN";
        case TOKEN_RIGHT_PARENTHESIS: return "RIGHT_PAREN";
        case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
        case TOKEN_LEFT_BRACKET: return "LEFT_BRACKET";
        case TOKEN_RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TOKEN_LEFT_STRING_QUOTATION: return "LEFT_STRING_QUOTATION";
        case TOKEN_RIGHT_STRING_QUOTATION: return "RIGHT_STRING_QUOTATION";
        case TOKEN_LEFT_CHAR_QUOTATION: return "LEFT_CHAR_QUOTATION";
        case TOKEN_RIGHT_CHAR_QUOTATION: return "RIGHT_CHAR_QUOTATION";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COLON: return "COLON";
        case TOKEN_DOT: return "DOT";
        case TOKEN_PLUS_OPERATOR: return "PLUS";
        case TOKEN_MINUS_OPERATOR: return "MINUS";
        case TOKEN_MULTIPLY_OPERATOR: return "MULTIPLY";
        case TOKEN_DIVIDE_OPERATOR: return "DIVIDE";
        case TOKEN_MODULO_OPERATOR: return "MODULO";
        case TOKEN_EXPONENT_OPERATOR: return "EXPONENT";
        case TOKEN_DIV_OPERATOR: return "DIV";
        case TOKEN_ASSIGN_OPERATOR: return "ASSIGN";
        case TOKEN_EQUAL_OPERATOR: return "EQUAL";
        case TOKEN_LOGICAL_NOT_OPERATOR: return "NOT";
        case TOKEN_NOT_EQUAL_OPERATOR: return "NOT_EQUAL";
        case TOKEN_LESS_THAN_OPERATOR: return "LESS";
        case TOKEN_GREATER_THAN_OPERATOR: return "GREATER";
        case TOKEN_LESS_EQUAL_OPERATOR: return "LESS_EQUAL";
        case TOKEN_GREATER_EQUAL_OPERATOR: return "GREATER_EQUAL";
        case TOKEN_INCREMENT_OPERATOR: return "INCREMENT";
        case TOKEN_DECREMENT_OPERATOR: return "DECREMENT";
        case TOKEN_PLUS_ASSIGN_OPERATOR: return "PLUS_ASSIGN";
        case TOKEN_MINUS_ASSIGN_OPERATOR: return "MINUS_ASSIGN";
        case TOKEN_MULTIPLY_ASSIGN_OPERATOR: return "MULT_ASSIGN";
        case TOKEN_DIVIDE_ASSIGN_OPERATOR: return "DIV_ASSIGN";
        case TOKEN_MODULO_ASSIGN_OPERATOR: return "MOD_ASSIGN";
        case TOKEN_LOGICAL_AND_OPERATOR: return "AND";
        case TOKEN_LOGICAL_OR_OPERATOR: return "OR";
        default: return "UNKNOWN_TYPE";
    }
}

/* ============================
   FILE IO + MAIN
   ============================ */
/* simple file reader */
static char *readFile(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;
    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return NULL; }
    long size = ftell(f);
    if (size < 0) size = 0;
    rewind(f);
    char *buf = (char*)malloc((size_t)size + 1);
    if (!buf) { fclose(f); return NULL; }
    size_t n = fread(buf, 1, (size_t)size, f);
    buf[n] = '\0';
    fclose(f);
    return buf;
}

int main(int argc, char **argv) {
    const char *input = (argc >= 2) ? argv[1] : "test.bsc";

    /* Validate extension .bsc (basic check) */
    size_t L = strlen(input);
    if (L < 4 || strcasecmp(input + L - 4, ".bsc") != 0) {
        fprintf(stderr, "Error: expected a .bsc file (got '%s')\n", input);
        return 1;
    }

    char *source = readFile(input);
    if (!source) {
        fprintf(stderr, "Error: cannot open file '%s'\n", input);
        return 1;
    }

    Lexer lexer;
    initLexer(&lexer, source);

    FILE *out = fopen("symbol_table.txt", "w");
    if (!out) { fprintf(stderr, "Error: cannot create symbol_table.txt\n"); free(source); return 1; }

    Token tok;
    do {
        tok = getNextToken(&lexer);
        const char *tname = tokenTypeName(tok.type);
        const char *lex = tok.lexeme ? tok.lexeme : "";
        printf("Lexeme: %-30s Token: %s\n", lex, tname);
        fprintf(out, "Lexeme: %-30s Token: %s\n", lex, tname);
        if (tok.lexeme) free(tok.lexeme);
    } while (tok.type != TOKEN_EOF);

    fclose(out);
    free(source);
    return 0;
}