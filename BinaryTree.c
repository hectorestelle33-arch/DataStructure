#include <stdio.h>
#include <stdlib.h>

typedef struct BiTNode {
    char data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
} BiTNode, *BiTree;

void CreateBiTree(BiTree *T) {
    char ch;
    scanf(" %c", &ch);

    if (ch == '#') {
        *T = NULL;
    } else {
        *T = (BiTree)malloc(sizeof(BiTNode));
        (*T)->data = ch;
        CreateBiTree(&((*T)->lchild));
        CreateBiTree(&((*T)->rchild));
    }
}

void PreOrder(BiTree T) {
    if (T != NULL) {
        printf("%c ", T->data);
        PreOrder(T->lchild);
        PreOrder(T->rchild);
    }
}

void InOrder(BiTree T) {
    if (T != NULL) {
        InOrder(T->lchild);
        printf("%c ", T->data);
        InOrder(T->rchild);
    }
}

void PostOrder(BiTree T) {
    if (T != NULL) {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        printf("%c ", T->data);
    }
}

int TreeHeight(BiTree T) {
    int leftHeight, rightHeight;

    if (T == NULL) {
        return 0;
    }

    leftHeight = TreeHeight(T->lchild);
    rightHeight = TreeHeight(T->rchild);

    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    } else {
        return rightHeight + 1;
    }
}

void DestroyTree(BiTree T) {
    if (T != NULL) {
        DestroyTree(T->lchild);
        DestroyTree(T->rchild);
        free(T);
    }
}

int main() {
    BiTree T = NULL;

    printf("请输入带空树标记#的先序序列：\n");
    CreateBiTree(&T);

    printf("\n先序遍历序列:");
    PreOrder(T);

    printf("\n中序遍历序列:");
    InOrder(T);

    printf("\n后序遍历序列:");
    PostOrder(T);

    printf("\n二叉树的高度为:%d\n", TreeHeight(T));

    DestroyTree(T);

    return 0;
}