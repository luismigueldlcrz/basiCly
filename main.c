#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *tokenTypeName(TokenType t) {
    switch (t) {
        case TOKEN_KEYWORD:       return "KEYWORD";
        case TOKEN_RESERVEDWORD:  return "RESERVEDWORD";
        case TOKEN_NOISEWORD:     return "NOISEWORD";
        case TOKEN_IDENTIFIER:    return "IDENTIFIER";
        case TOKEN_NUMBER:        return "NUMBER";
        case TOKEN_LITERAL:       return "LITERAL";
        case TOKEN_OPERATOR:      return "OPERATOR";
        case TOKEN_DELIMITER:     return "DELIMITER";
        case TOKEN_COMMENT:       return "COMMENT";
        case TOKEN_EOF:           return "EOF";
        case TOKEN_UNKNOWN:       return "UNKNOWN";
        default:                  return "UNDEFINED";
    }
}

int main(int argc, char **argv) {
    const char *input_file = "text.bsc";
    if (argc >= 2) {
        input_file = argv[1];
        size_t len = strlen(input_file);
        if (len < 4 ||
            input_file[len-4] != '.' ||
            input_file[len-3] != 'b' ||
            input_file[len-2] != 's' ||
            input_file[len-1] != 'c') {
            fprintf(stderr, "Error: input file must have .bsc extension\n");
            return 1;
        }
    }

    FILE *file = fopen(input_file, "rb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", input_file);
        return 1;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error: fseek failed\n");
        fclose(file);
        return 1;
    }
    long file_size = ftell(file);
    if (file_size < 0) file_size = 0;
    rewind(file);

    char *buffer = (char *)malloc((size_t)file_size + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    size_t read_bytes = fread(buffer, 1, (size_t)file_size, file);
    buffer[read_bytes] = '\0';
    fclose(file);

    Lexer lexer;
    initLexer(&lexer, buffer);

    FILE *out = fopen("symbol_table.txt", "w");
    if (!out) {
        fprintf(stderr, "Warning: Cannot open symbol_table.txt for writing. Continuing without file output.\n");
    }

    Token token;
    do {
        token = getNextToken(&lexer);

        /* skip comments and noise words */
        if (token.type == TOKEN_COMMENT || token.type == TOKEN_NOISEWORD) {
            if (token.lexeme) free(token.lexeme);
            continue;
        }

        const char *lex = token.lexeme ? token.lexeme : "(null)";
        const char *type = tokenTypeName(token.type);

        printf("Token: %-15s Type: %s\n", lex, type);
        if (out) fprintf(out, "Token: %-15s Type: %s\n", lex, type);

        if (token.lexeme) free(token.lexeme);
    } while (token.type != TOKEN_EOF);

    if (out) fclose(out);
    free(buffer);
    return 0;
}
