#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_SIZE 25
#define BLOCK_SIZE 5
#define BLOCK_COUNT 5

typedef struct {
    int maxKey;
    int start;
    int end;
} BlockIndex;

static const int g_data[DATA_SIZE] = {
    133, 145, 152, 135, 139,
    157, 156, 169, 172, 159,
    100, 94, 88, 96, 87,
    71, 78, 68, 80, 85,
    40, 38, 54, 66, 46
};

static int compare_int(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
}

static void build_block_index(const int sorted[], BlockIndex index[])
{
    int i;
    for (i = 0; i < BLOCK_COUNT; ++i) {
        index[i].start = i * BLOCK_SIZE;
        index[i].end = index[i].start + BLOCK_SIZE - 1;
        index[i].maxKey = sorted[index[i].end];
    }
}

static void block_search(const int sorted[], const BlockIndex index[], int key)
{
    int i, j;
    int comparisons = 0;
    int found = 0;
    int position = -1;

    for (i = 0; i < BLOCK_COUNT; ++i) {
        ++comparisons;
        if (key <= index[i].maxKey) {
            for (j = index[i].start; j <= index[i].end; ++j) {
                ++comparisons;
                if (sorted[j] == key) {
                    found = 1;
                    position = j;
                    break;
                }
            }
            break;
        }
    }

    if (found) {
        printf("分块查找: 成功找到 %d, 在排序后数组中的位置 = %d, 总比较次数 = %d\n", key, position, comparisons);
    } else {
        printf("分块查找: 未找到 %d, 总比较次数 = %d\n", key, comparisons);
    }
}

static void print_array(const char *title, const int data[], int length)
{
    int i;
    printf("%s\n", title);
    for (i = 0; i < length; ++i) {
        printf("%4d", data[i]);
        if ((i + 1) % 5 == 0) printf("\n");
    }
}

int main(void)
{
    int sorted[DATA_SIZE];
    BlockIndex blockIndex[BLOCK_COUNT];
    char input[64];
    int i;

    for (i = 0; i < DATA_SIZE; ++i) {
        sorted[i] = g_data[i];
    }
    qsort(sorted, DATA_SIZE, sizeof(sorted[0]), compare_int);

    build_block_index(sorted, blockIndex);

    print_array("原始数据集:", g_data, DATA_SIZE);
    print_array("\n排序后的数据集（用于分块）:", sorted, DATA_SIZE);

    printf("\n--- 自动化样例测试 ---\n");
    block_search(sorted, blockIndex, 133);
    block_search(sorted, blockIndex, 101);

    printf("\n请输入要查找的整数（输入 q 退出）:\n");
    while (1) {
        int key;
        printf("\nkey> ");
        if (scanf("%63s", input) != 1) break;
        if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) break;
        
        if (sscanf(input, "%d", &key) != 1) {
            printf("输入无效，请输入整数。\n");
            continue;
        }
        block_search(sorted, blockIndex, key);
    }

    printf("程序已退出。\n");
    return 0;
}
