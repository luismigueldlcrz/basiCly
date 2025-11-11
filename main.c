#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

static const char *tokenTypeName(TokenType t) {
    switch (t) {
        case TOKEN_KEYWORD:       return "KEYWORD";
        case TOKEN_RESERVEDWORD:  return "RESERVEDWORD";
        case TOKEN_NOISEWORD:     return "NOISEWORD";
        case TOKEN_IDENTIFIER:    return "IDENTIFIER";
        case TOKEN_NUMBER:        return "NUMBER";
        case TOKEN_STRING:        return "STRING";
        case TOKEN_OPERATOR:      return "OPERATOR";
        case TOKEN_DELIMITER:     return "DELIMITER";
        case TOKEN_COMMENT:       return "COMMENT";
        case TOKEN_EOF:           return "EOF";
        case TOKEN_UNKNOWN:       return "UNKNOWN";
        default:                  return "UNDEFINED";
    }
}

int main() {
    
    FILE *file;
    char *buffer;
    long file_size;
    Lexer lexer;
    Token token;

    // Open the source code file (e.g., "code.bsaiCly")
    file = fopen("text.bsc", "r");
    if (!file) {
        printf("Error: Cannot open file.\n");
        return 1;
    }

    // Move to the end to measure file size
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // Allocate memory for the entire file content
    buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    // Read file content into buffer
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0'; // null-terminate string
    fclose(file);

    // Initialize lexer with file content
    initLexer(&lexer, buffer);

    // Open symbol table output file
    FILE *out = fopen("symbol_table.txt", "w");
    if (!out) {
        printf("Warning: Cannot open symbol_table.txt for writing. Continuing without file output.\n");
    }

    // Tokenize the input and write to symbol_table.txt
    do {
        token = getNextToken(&lexer);
        const char *lex = token.lexeme ? token.lexeme : "(null)";
        const char *type = tokenTypeName(token.type);

        // print to console
        printf("Token: %-15s Type: %s\n", lex, type);

        // write to file if available
        if (out) {
            fprintf(out, "Token: %-15s Type: %s\n", lex, type);
        }

        if (token.lexeme) free(token.lexeme); // free memory allocated for token lexeme
    } while (token.type != TOKEN_EOF);

    if (out) fclose(out);

    // Free the buffer holding file content
    free(buffer);

    return 0;
}
