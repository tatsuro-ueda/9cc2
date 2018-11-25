#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの型を返す値
enum {
    TK_NUM = 256,  // 整数のノードの型
    TK_EOF,  // 入力の終わりを表すトークン
};

// トークンの型
typedef struct {
    int ty;  // トークンの型
    int val;  // tyがTK_NUMの場合、その数値
} Token;

// トークナイズした結果のトークン列はこの配列に保存する
// 100個以上のトークンは来ないものとする
Token tokens[100];

// pが指している文字列をトークンに分割してtokensに保存する
void tokenize(char *p) {
    int i = 0;
    while (*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }

		// *pが'+'か'-'ならば…
        if (*p == '+' || *p == '-') {
            tokens[i].ty = *p;  // トークンのASCIIコードをそのトークンの型とする
            i++;
            p++;
            continue;
        }

		// *pが数値ならば…
        if (isdigit(*p)) {
            tokens[i].ty = TK_NUM;
            tokens[i].val = strtol(p, &p, 10);
            i++;
            continue;
        }

        fprintf(stderr, "トークナイズできません: %s\n", p);
        exit(1);
    }
    tokens[i].ty = TK_EOF;
}

/*
// エラーを報告するための関数
void error(int i) {
	// '%'もよくわからない
    fprintf(stderr, "予期せぬトークンです: %s\n", tokens[i].input);
    exit(1);
}
*/

// ノードの型を返す値
enum {
    ND_NUM = 256,  // 整数のノードの型
};

typedef struct Node {
    int ty;  // 演算子かND_NUM
    struct Node *lhs;  // left-hand side
    struct Node *rhs;  // right-hand side
    int val;  // tyがND_NUMの場合のみ使う
} Node;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Number of parameters is invalid\n");
        return 1;
    }

    // トークン化する
	// すべての標準入力は空白を除いたかたちでtokens[]に格納される
    tokenize(argv[1]);

	// Output former part of assembly
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    
	// 式の最初は数でなければならないので、
    // それをチェックして最初のmov命令を出力
    if (tokens[0].ty != TK_NUM)
		error(0);  // エラーチェック
    printf("    mov rax, %d\n", tokens[0].val);
    
	// `+　<数>`あるいは`- <数>`というトークンの並びを消費しつつ
    // アセンブリを出力
    int i = 1;  // 2番目のトークンに進む
    while (tokens[i].ty != TK_EOF) {
        if (tokens[i].ty == '+') {
            i++;  // 次のトークンに進んでおく
            if (tokens[i].ty != TK_NUM)
                error(1);  // エラーチェック
            printf("    add rax, %d\n", tokens[i].val);
            i++;
            continue;
        }
        
        if (tokens[i].ty == '-') {
            i++;
            if (tokens[i].ty != TK_NUM)
                error(i);
            printf("    sub rax, %d\n", tokens[i].val);
            i++;
            continue;
        }
        error(i);
    }
    printf("    ret\n");
    return 0;
}