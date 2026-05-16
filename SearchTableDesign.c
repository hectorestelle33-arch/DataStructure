#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_SIZE 25
#define BLOCK_SIZE 5
#define BLOCK_COUNT ((DATA_SIZE + BLOCK_SIZE - 1) / BLOCK_SIZE)
#define HASH_SIZE 29

typedef struct {
    int maxKey;
    int start;
    int end;
} BlockIndex;

typedef struct TreeNode {
    int key;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct HashNode {
    int key;
    struct HashNode *next;
} HashNode;

typedef struct {
    int found;
    int position;
    int comparisons;
} SearchResult;

static const int g_data[DATA_SIZE] = {
    133, 145, 152, 135, 139,
    157, 156, 169, 172, 159,
    100, 94, 88, 96, 87,
    71, 78, 68, 80, 85,
    40, 38, 54, 66, 46
};

static int compare_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;

    return (x > y) - (x < y);
}

static void copy_and_sort(int sorted[])
{
    int i;

    for (i = 0; i < DATA_SIZE; ++i) {
        sorted[i] = g_data[i];
    }
    qsort(sorted, DATA_SIZE, sizeof(sorted[0]), compare_int);
}

static void build_block_index(const int sorted[], BlockIndex index[])
{
    int i;

    for (i = 0; i < BLOCK_COUNT; ++i) {
        int start = i * BLOCK_SIZE;
        int end = start + BLOCK_SIZE - 1;
        if (end >= DATA_SIZE) {
            end = DATA_SIZE - 1;
        }

        index[i].start = start;
        index[i].end = end;
        index[i].maxKey = sorted[end];
    }
}

static SearchResult block_search(const int sorted[], const BlockIndex index[], int key)
{
    int i;
    SearchResult result = {0, -1, 0};

    for (i = 0; i < BLOCK_COUNT; ++i) {
        ++result.comparisons;
        if (key <= index[i].maxKey) {
            int j;
            for (j = index[i].start; j <= index[i].end; ++j) {
                ++result.comparisons;
                if (sorted[j] == key) {
                    result.found = 1;
                    result.position = j;
                    return result;
                }
            }
            return result;
        }
    }

    return result;
}

static TreeNode *create_tree_node(int key)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Out of memory while creating tree node.\n");
        exit(EXIT_FAILURE);
    }

    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static TreeNode *bst_insert(TreeNode *root, int key)
{
    if (root == NULL) {
        return create_tree_node(key);
    }

    if (key < root->key) {
        root->left = bst_insert(root->left, key);
    } else if (key > root->key) {
        root->right = bst_insert(root->right, key);
    }

    return root;
}

static TreeNode *build_bst(void)
{
    int i;
    TreeNode *root = NULL;

    for (i = 0; i < DATA_SIZE; ++i) {
        root = bst_insert(root, g_data[i]);
    }

    return root;
}

static SearchResult bst_search(TreeNode *root, int key)
{
    SearchResult result = {0, -1, 0};
    TreeNode *current = root;

    while (current != NULL) {
        ++result.comparisons;
        if (key == current->key) {
            result.found = 1;
            return result;
        }
        current = (key < current->key) ? current->left : current->right;
    }

    return result;
}

static void free_bst(TreeNode *root)
{
    if (root == NULL) {
        return;
    }
    free_bst(root->left);
    free_bst(root->right);
    free(root);
}

static int hash_func(int key)
{
    int value = key % HASH_SIZE;
    return value < 0 ? value + HASH_SIZE : value;
}

static void hash_insert(HashNode *table[], int key)
{
    int address = hash_func(key);
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    if (node == NULL) {
        fprintf(stderr, "Out of memory while creating hash node.\n");
        exit(EXIT_FAILURE);
    }

    node->key = key;
    node->next = table[address];
    table[address] = node;
}

static void build_hash_table(HashNode *table[])
{
    int i;

    for (i = 0; i < HASH_SIZE; ++i) {
        table[i] = NULL;
    }
    for (i = 0; i < DATA_SIZE; ++i) {
        hash_insert(table, g_data[i]);
    }
}

static SearchResult hash_search(HashNode *table[], int key)
{
    SearchResult result = {0, -1, 0};
    int address = hash_func(key);
    HashNode *current = table[address];

    while (current != NULL) {
        ++result.comparisons;
        if (current->key == key) {
            result.found = 1;
            result.position = address;
            return result;
        }
        current = current->next;
    }

    return result;
}

static void free_hash_table(HashNode *table[])
{
    int i;

    for (i = 0; i < HASH_SIZE; ++i) {
        HashNode *current = table[i];
        while (current != NULL) {
            HashNode *next = current->next;
            free(current);
            current = next;
        }
        table[i] = NULL;
    }
}

static void print_array(const char *title, const int data[], int length)
{
    int i;

    printf("%s\n", title);
    for (i = 0; i < length; ++i) {
        printf("%4d", data[i]);
        if ((i + 1) % 5 == 0 || i == length - 1) {
            printf("\n");
        }
    }
}

static void print_block_index(const BlockIndex index[])
{
    int i;

    printf("\nBlock index table:\n");
    printf("Block  Range      MaxKey\n");
    for (i = 0; i < BLOCK_COUNT; ++i) {
        printf("%5d  [%2d,%2d]  %6d\n", i + 1, index[i].start, index[i].end, index[i].maxKey);
    }
}

static void print_bst_in_order(TreeNode *root)
{
    if (root == NULL) {
        return;
    }
    print_bst_in_order(root->left);
    printf("%4d", root->key);
    print_bst_in_order(root->right);
}

static void print_hash_table(HashNode *table[])
{
    int i;

    printf("\nHash table, divisor is %d and collision method is chaining:\n", HASH_SIZE);
    for (i = 0; i < HASH_SIZE; ++i) {
        HashNode *current = table[i];
        if (current == NULL) {
            continue;
        }

        printf("%2d:", i);
        while (current != NULL) {
            printf(" %d", current->key);
            current = current->next;
            if (current != NULL) {
                printf(" ->");
            }
        }
        printf("\n");
    }
}

static void print_result(const char *method, SearchResult result)
{
    printf("%-14s: ", method);
    if (result.found) {
        printf("found");
        if (result.position >= 0) {
            printf(", position/address = %d", result.position);
        }
    } else {
        printf("not found");
    }
    printf(", comparisons = %d\n", result.comparisons);
}

static void run_search(int key, const int sorted[], const BlockIndex index[],
                       TreeNode *root, HashNode *hashTable[])
{
    printf("\nSearch key: %d\n", key);
    print_result("Block search", block_search(sorted, index, key));
    print_result("BST search", bst_search(root, key));
    print_result("Hash search", hash_search(hashTable, key));
}

int main(void)
{
    int sorted[DATA_SIZE];
    BlockIndex blockIndex[BLOCK_COUNT];
    TreeNode *bstRoot;
    HashNode *hashTable[HASH_SIZE];
    int samples[] = {133, 85, 46, 101};
    int i;
    char input[64];

    copy_and_sort(sorted);
    build_block_index(sorted, blockIndex);
    bstRoot = build_bst();
    build_hash_table(hashTable);

    print_array("Original data:", g_data, DATA_SIZE);
    print_array("\nSorted data for block search:", sorted, DATA_SIZE);
    print_block_index(blockIndex);
    printf("\nBST in-order traversal:\n");
    print_bst_in_order(bstRoot);
    printf("\n");
    print_hash_table(hashTable);

    printf("\nSample search tests:\n");
    for (i = 0; i < (int)(sizeof(samples) / sizeof(samples[0])); ++i) {
        run_search(samples[i], sorted, blockIndex, bstRoot, hashTable);
    }

    printf("\nInput an integer to search, or q to quit.\n");
    while (1) {
        int key;
        printf("\nkey> ");
        if (scanf("%63s", input) != 1) {
            break;
        }
        if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) {
            break;
        }
        if (sscanf(input, "%d", &key) != 1) {
            printf("Invalid input.\n");
            continue;
        }
        run_search(key, sorted, blockIndex, bstRoot, hashTable);
    }

    free_bst(bstRoot);
    free_hash_table(hashTable);
    return 0;
}
