#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100

typedef struct {
    int weight;
    int parent;
    int lchild;
    int rchild;
    char ch;
} HTNode;

typedef char* HuffmanCode[MAXN];

void SelectMin(HTNode ht[], int end, int *s1, int *s2) {
    int i;
    *s1 = *s2 = -1;

    for (i = 0; i < end; i++) {
        if (ht[i].parent == -1) {
            if (*s1 == -1 || ht[i].weight < ht[*s1].weight) {
                *s2 = *s1;
                *s1 = i;
            } else if (*s2 == -1 || ht[i].weight < ht[*s2].weight) {
                *s2 = i;
            }
        }
    }
}

void CreateHuffmanTree(HTNode ht[], char chars[], int weights[], int n) {
    int i, s1, s2;
    int m = 2 * n - 1;

    for (i = 0; i < m; i++) {
        ht[i].weight = 0;
        ht[i].parent = -1;
        ht[i].lchild = -1;
        ht[i].rchild = -1;
        ht[i].ch = '\0';
    }

    for (i = 0; i < n; i++) {
        ht[i].weight = weights[i];
        ht[i].ch = chars[i];
    }

    for (i = n; i < m; i++) {
        SelectMin(ht, i, &s1, &s2);

        ht[s1].parent = i;
        ht[s2].parent = i;

        ht[i].lchild = s1;
        ht[i].rchild = s2;
        ht[i].weight = ht[s1].weight + ht[s2].weight;
    }
}

void CreateHuffmanCode(HTNode ht[], HuffmanCode hc, int n) {
    char temp[MAXN];
    int i, start, c, p;

    for (i = 0; i < n; i++) {
        start = MAXN - 1;
        temp[start] = '\0';

        c = i;
        p = ht[c].parent;

        while (p != -1) {
            start--;

            if (ht[p].lchild == c) {
                temp[start] = '0';
            } else {
                temp[start] = '1';
            }

            c = p;
            p = ht[c].parent;
        }

        hc[i] = (char*)malloc((MAXN - start) * sizeof(char));
        strcpy(hc[i], &temp[start]);
    }
}

void PrintCodes(char chars[], HuffmanCode hc, int n) {
    int i;

    printf("各字符的哈夫曼编码如下：\n");
    for (i = 0; i < n; i++) {
        printf("%c: %s\n", chars[i], hc[i]);
    }
}

int FindChar(char chars[], int n, char ch) {
    int i;
    for (i = 0; i < n; i++) {
        if (chars[i] == ch) {
            return i;
        }
    }
    return -1;
}

void Encode(char chars[], HuffmanCode hc, int n) {
    char str[MAXN];
    int i, index;

    printf("\n请输入要编码的字符串：");
    scanf("%s", str);

    printf("编码结果为：");
    for (i = 0; str[i] != '\0'; i++) {
        index = FindChar(chars, n, str[i]);
        if (index != -1) {
            printf("%s", hc[index]);
        } else {
            printf("\n字符 %c 不在字符集中，无法编码。\n", str[i]);
            return;
        }
    }
    printf("\n");
}

void Decode(HTNode ht[], int n) {
    char code[MAXN];
    int root = 2 * n - 2;
    int p, i;

    printf("\n请输入要译码的01串：");
    scanf("%s", code);

    printf("译码结果为：");

    p = root;
    for (i = 0; code[i] != '\0'; i++) {
        if (code[i] == '0') {
            p = ht[p].lchild;
        } else if (code[i] == '1') {
            p = ht[p].rchild;
        } else {
            printf("\n输入中含有非法字符，只能输入0或1。\n");
            return;
        }

        if (ht[p].lchild == -1 && ht[p].rchild == -1) {
            printf("%c", ht[p].ch);
            p = root;
        }
    }

    if (p != root) {
        printf("\n注意：输入的编码串不完整，最后一段无法译码。");
    }

    printf("\n");
}

void PrintTree(HTNode ht[], int n) {
    int i;
    int m = 2 * n - 1;

    printf("\n哈夫曼树数组存储如下：\n");
    printf("下标\t字符\t权值\t父结点\t左孩子\t右孩子\n");

    for (i = 0; i < m; i++) {
        if (ht[i].ch == '\0') {
            printf("%d\t%c\t%d\t%d\t%d\t%d\n",
                   i, '-', ht[i].weight, ht[i].parent, ht[i].lchild, ht[i].rchild);
        } else {
            printf("%d\t%c\t%d\t%d\t%d\t%d\n",
                   i, ht[i].ch, ht[i].weight, ht[i].parent, ht[i].lchild, ht[i].rchild);
        }
    }
}

int main() {
    int n = 9;
    char chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
    int weights[] = {64, 13, 22, 32, 50, 21, 15, 47, 8};

    HTNode ht[MAXN];
    HuffmanCode hc;

    CreateHuffmanTree(ht, chars, weights, n);
    CreateHuffmanCode(ht, hc, n);

    PrintTree(ht, n);
    printf("\n");
    PrintCodes(chars, hc, n);

    Encode(chars, hc, n);
    Decode(ht, n);

    return 0;
}