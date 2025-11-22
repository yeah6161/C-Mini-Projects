#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const char *s; int pos;

void skip() { while (s[pos]==' '||s[pos]=='\t') pos++; }

double parse_expr();

double parse_number() {
    skip();
    double sign = 1;
    if (s[pos]=='-') { sign=-1; pos++; }
    double val = 0;
    while (isdigit((unsigned char)s[pos])) { val = val*10 + (s[pos]-'0'); pos++; }
    if (s[pos]=='.') {
        pos++; double frac=0, base=1;
        while (isdigit((unsigned char)s[pos])) { base/=10; frac += (s[pos]-'0')*base; pos++; }
        val += frac;
    }
    return sign*val;
}

double parse_factor() {
    skip();
    if (s[pos]=='(') { pos++; double v = parse_expr(); if (s[pos]==')') pos++; return v; }
    return parse_number();
}

double parse_term() {
    double v = parse_factor();
    while (1) {
        skip();
        if (s[pos]=='*') { pos++; v *= parse_factor(); }
        else if (s[pos]=='/') { pos++; v /= parse_factor(); }
        else break;
    }
    return v;
}

double parse_expr() {
    double v = parse_term();
    while (1) {
        skip();
        if (s[pos]=='+') { pos++; v += parse_term(); }
        else if (s[pos]=='-') { pos++; v -= parse_term(); }
        else break;
    }
    return v;
}

int main() {
    char line[1024];
    printf("Enter expression (or empty to exit):\n");
    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        if (line[0]=='\n') break;
        s = line; pos = 0;
        double res = parse_expr();
        printf("= %.10g\n", res);
    }
    return 0;
}

