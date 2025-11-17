#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

/* Convert TokenType to human-readable string */
const char* tokenTypeToString(TokenType type) {
    switch(type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_RESERVEDWORD: return "RESERVED_WORD";
        case TOKEN_NOISEWORD: return "NOISE_WORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INT_LITERAL: return "INT_LITERAL";
        case TOKEN_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TOKEN_STRING_LITERAL: return "STRING_LITERAL";
        case TOKEN_CHAR_LITERAL: return "CHAR_LITERAL";
        case TOKEN_PLUS_OPERATOR: return "PLUS_OPERATOR";
        case TOKEN_MINUS_OPERATOR: return "MINUS_OPERATOR";
        case TOKEN_MULTIPLY_OPERATOR: return "MULTIPLY_OPERATOR";
        case TOKEN_DIVIDE_OPERATOR: return "DIVIDE_OPERATOR";
        case TOKEN_MODULO_OPERATOR: return "MODULO_OPERATOR";
        case TOKEN_ASSIGN_OPERATOR: return "ASSIGN_OPERATOR";
        case TOKEN_EQUAL_OPERATOR: return "EQUAL_OPERATOR";
        case TOKEN_PLUS_ASSIGN_OPERATOR: return "PLUS_ASSIGN_OPERATOR";
        case TOKEN_MINUS_ASSIGN_OPERATOR: return "MINUS_ASSIGN_OPERATOR";
        case TOKEN_MULTIPLY_ASSIGN_OPERATOR: return "MULTIPLY_ASSIGN_OPERATOR";
        case TOKEN_DIVIDE_ASSIGN_OPERATOR: return "DIVIDE_ASSIGN_OPERATOR";
        case TOKEN_MODULO_ASSIGN_OPERATOR: return "MODULO_ASSIGN_OPERATOR";
        case TOKEN_LOGICAL_NOT_OPERATOR: return "LOGICAL_NOT_OPERATOR";
        case TOKEN_NOT_EQUAL_OPERATOR: return "NOT_EQUAL_OPERATOR";
        case TOKEN_LESS_THAN_OPERATOR: return "LESS_THAN_OPERATOR";
        case TOKEN_GREATER_THAN_OPERATOR: return "GREATER_THAN_OPERATOR";
        case TOKEN_LESS_EQUAL_OPERATOR: return "LESS_EQUAL_OPERATOR";
        case TOKEN_GREATER_EQUAL_OPERATOR: return "GREATER_EQUAL_OPERATOR";
        case TOKEN_LOGICAL_AND_OPERATOR: return "LOGICAL_AND_OPERATOR";
        case TOKEN_LOGICAL_OR_OPERATOR: return "LOGICAL_OR_OPERATOR";
        case TOKEN_INCREMENT_OPERATOR: return "INCREMENT_OPERATOR";
        case TOKEN_DECREMENT_OPERATOR: return "DECREMENT_OPERATOR";
        case TOKEN_LEFT_PARENTHESIS: return "LEFT_PARENTHESIS";
        case TOKEN_RIGHT_PARENTHESIS: return "RIGHT_PARENTHESIS";
        case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
        case TOKEN_LEFT_BRACKET: return "LEFT_BRACKET";
        case TOKEN_RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        // case TOKEN_DOT: return "DOT";
        case TOKEN_COLON: return "COLON";
        case TOKEN_STRING_QUOTATION: return "STRING_QUOTATION";
        case TOKEN_CHAR_QUOTATION: return "CHAR_QUOTATION";
        case TOKEN_LINE_COMMENT: return "LINE_COMMENT";
        case TOKEN_BLOCK_COMMENT: return "BLOCK_COMMENT";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

/* Read entire file into a string */
char* readFile(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file.bsc>\n", argv[0]);
        return 1;
    }

    char *sourceCode = readFile(argv[1]);
    if (!sourceCode) return 1;

    Lexer lexer;
    initLexer(&lexer, sourceCode);

    FILE *symbolTable = fopen("symbol_table.txt", "w");
    if (!symbolTable) {
        fprintf(stderr, "Error: Could not create symbol_table.txt\n");
        free(sourceCode);
        return 1;
    }

    Token token;
    do {
        token = getNextToken(&lexer);

        /* Lexeme for EOF is "EOF" */
        char *lexemeToPrint = (token.type == TOKEN_EOF) ? "EOF" : (token.lexeme ? token.lexeme : "NULL");

        /* Print to console */
        printf("Lexeme: %-15s Token: %s\n", lexemeToPrint, tokenTypeToString(token.type));

        /* Print to symbol table */
        fprintf(symbolTable, "Lexeme: %-15s Token: %s\n", lexemeToPrint, tokenTypeToString(token.type));

        if (token.lexeme) free(token.lexeme);
    } while (token.type != TOKEN_EOF);

    fclose(symbolTable);
    free(sourceCode);
    return 0;
}