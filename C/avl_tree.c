#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#define LL 1
#define LR 2
#define RR 3
#define RL 4

#define QUEUE_LEN 3000000

struct tree_node {
    uint64_t value;
    unsigned long index;
    char bf;
    char branch; //relative to its parent.
    struct tree_node * parent, * left, * right;
};


static struct tree_node * balance_LL(struct tree_node *);
static struct tree_node * balance_RR(struct tree_node *);
static struct tree_node * balance_LR(struct tree_node *);
static struct tree_node * balance_RL(struct tree_node *);
static void traverse_tree(struct tree_node *);
void traverse_tree_h(struct tree_node *);
struct tree_node * make_avl_tree(struct tree_node ** const, uint64_t, unsigned long *);


struct tree_node *
make_avl_tree(struct tree_node ** const tree_node_c, uint64_t value, unsigned long * index)
{
    struct tree_node * tree_node = NULL, * new_node = NULL, * child_node = NULL;
    struct tree_node * A_node = NULL, * B_node = NULL, * C_node = NULL, * keep = NULL, * tmp_node = NULL;
    //printf(">%lu\n", value);
    tree_node = *tree_node_c;
    //printf("%p\n", tree_node);
    /*
      'r' for go right.
      'l' for go left.
      0 for root node.
    */
    char branch = 0;
    char keep_branch = 0;
    /* 0 not need balance,
       1 LL A node left and A node's subtree left.
       2 LR A node left and A node's subtree right.
       3 RR A node right and A node's subtree right.
       4 RL A node right and A node's subtree left.
     */
    char balance_type = 0;

    while(tree_node){
        keep = tree_node;
        if (value > tree_node -> value){
            if ((tree_node -> bf == 1) || (tree_node -> bf == -1)){
                A_node = tree_node;
                branch = 'r';
            }
            keep_branch = 'r';
            tree_node = tree_node -> right;
        } else if (value < tree_node -> value){
            if (tree_node -> bf == 1 || tree_node -> bf == -1){
                A_node = tree_node;
                branch = 'l';
            }
            keep_branch = 'l';
            tree_node = tree_node -> left;
        } else{
            return tree_node;
        }
    }
    //printf("passed\n");
    if (keep){
        new_node = (struct tree_node *)malloc(sizeof(struct tree_node));
        new_node -> value = value;
        new_node -> index = *index;
        (*index)++;
        new_node -> bf = 0;
        new_node -> branch = keep_branch;
        new_node -> parent = keep;
        new_node -> left = NULL;
        new_node -> right = NULL;
        if (keep_branch == 'l'){
            keep -> left = new_node;
        } else if (keep_branch == 'r'){
            keep -> right = new_node;
        } else{
            fprintf(stderr, "can not possible\n");
        }

    } else{
        new_node = (struct tree_node *)malloc(sizeof(struct tree_node));
        new_node -> value = value;
        new_node -> index = *index;
        (*index)++;
        new_node -> bf = 0;
        new_node -> branch = 0;
        new_node -> parent = NULL;
        new_node -> left = NULL;
        new_node -> right = NULL;
        *tree_node_c = new_node;
        return new_node;
    }


    if (!A_node){
        //printf("value: %lu\n", value);
        balance_type = 0;
        child_node = new_node;
        while(keep){
            if (child_node -> branch == 'l'){
                keep -> bf += 1;
            } else if (child_node -> branch == 'r'){
                keep -> bf -= 1;
            } else{
                fprintf(stderr, "can not possible\n");
                exit(1);
            }
            child_node = keep;
            keep = keep -> parent;
        }
    } else if (A_node == keep){
        balance_type = 0;
        keep -> bf = 0;
    } else if (A_node -> bf == -1 && branch == 'l'){
        balance_type = 0;
        A_node -> bf = 0;
        child_node = new_node;
        while(keep != A_node){
            if (child_node -> branch == 'l'){
                keep -> bf += 1;
            } else if (child_node -> branch == 'r'){
                keep -> bf -= 1;
            } else{
                fprintf(stderr, "can not possible.\n");
                exit(1);
            }
            child_node = keep;
            keep = keep -> parent;
        }

    } else if (A_node -> bf == 1 && branch == 'r'){
        balance_type = 0;
        A_node -> bf = 0;
        child_node = new_node;
        while(keep != A_node){
            if (child_node -> branch == 'l'){
                keep -> bf += 1;
            } else if (child_node -> branch == 'r'){
                keep -> bf -= 1;
            } else{
                fprintf(stderr, "can not possible.\n");
                exit(1);
            }
            child_node = keep;
            keep = keep -> parent;
        }

    } else if (A_node -> bf == -1 && branch == 'r'){
        B_node = A_node -> right;
        tmp_node = new_node;
        while(tmp_node != B_node){
            if (tmp_node -> branch == 'l'){
                tmp_node -> parent -> bf += 1;
            } else{
                tmp_node -> parent -> bf -= 1;
            }
            C_node = tmp_node;
            tmp_node = tmp_node -> parent;
        }

        A_node -> bf -= 1;
        if (C_node -> branch == 'r'){
            balance_type = RR;
        } else if (C_node -> branch == 'l'){
            balance_type = RL;
        }

    } else if (A_node -> bf == 1 && branch == 'l'){
        B_node = A_node -> left;
        tmp_node = new_node;
        while(tmp_node != B_node){
            if (tmp_node -> branch == 'l'){
                tmp_node -> parent -> bf += 1;
            } else{
                tmp_node -> parent -> bf -= 1;
            }
            C_node = tmp_node;
            tmp_node = tmp_node -> parent;
        }

        A_node -> bf += 1;
        if (C_node -> branch == 'l'){
            balance_type = LL;
        } else if (C_node -> branch == 'r'){
            balance_type = LR;
        }
    }
    //printf("balance_type: %d\n", balance_type);


    if (balance_type == LL){
        //printf("A_node: %lu\n", A_node -> value);
        A_node = balance_LL(A_node);
        if (!A_node -> parent){
            *tree_node_c = A_node;
        }
    } else if (balance_type == LR){
        //printf("A_node: %lu\n", A_node -> value);
        A_node = balance_LR(A_node);
        if (!A_node -> parent){
            *tree_node_c = A_node;
        }

    } else if (balance_type == RR){
        //printf("A_node: %lu\n", A_node -> value);
        A_node = balance_RR(A_node);
        if (!A_node -> parent){
            *tree_node_c = A_node;
        }
    } else if (balance_type == RL){
        //printf("A_node: %lu\n", A_node -> value);
        A_node = balance_RL(A_node);
        if (!A_node -> parent){
            *tree_node_c = A_node;
        }
    }


    return new_node;
}


static struct tree_node *
balance_LL(struct tree_node * A_node)
{
    struct tree_node * B_node, * parent;
    char branch = 0;
    B_node = A_node -> left;
    parent = A_node -> parent;
    branch = A_node -> branch;

    A_node -> left = B_node -> right;
    if (B_node -> right){
        B_node -> right -> parent = A_node;
        B_node -> right -> branch = 'l';
    }
    A_node -> parent = B_node;
    A_node -> branch = 'r';
    B_node -> right = A_node;
    B_node -> parent = parent;
    B_node -> branch = branch;
    if (branch == 'r'){
        parent -> right = B_node;
    } else if (branch == 'l'){
        parent -> left = B_node;
    }
    A_node -> bf = 0;
    B_node -> bf = 0;

    return B_node;
}


static struct tree_node *
balance_RR(struct tree_node * A_node)
{
    struct tree_node * B_node, * parent;
    char branch = 0;
    B_node = A_node -> right;
    parent = A_node -> parent;
    branch = A_node -> branch;

    A_node -> right = B_node -> left;
    if (B_node -> left){
        B_node -> left -> parent = A_node;
        B_node -> left -> branch = 'r';
    }
    A_node -> parent = B_node;
    A_node -> branch = 'l';
    B_node -> left = A_node;
    B_node -> parent = parent;
    B_node -> branch = branch;
    if (branch == 'r'){
        parent -> right = B_node;
    } else if (branch == 'l'){
        parent -> left = B_node;
    }
    A_node -> bf = 0;
    B_node -> bf = 0;

    return B_node;
}


static struct tree_node *
balance_LR(struct tree_node * A_node)
{
    struct tree_node * B_node, * C_node, * parent;
    char branch = 0;
    B_node = A_node -> left;
    C_node = B_node -> right;
    parent = A_node -> parent;
    branch = A_node -> branch;

    B_node -> right = C_node -> left;
    if (C_node -> left){
        C_node -> left -> parent = B_node;
        C_node -> left -> branch = 'r';
    }
    B_node -> parent = C_node;
    B_node -> branch = 'l';
    A_node -> left = C_node -> right;
    if (C_node -> right){
        C_node -> right -> parent = A_node;
        C_node -> right -> branch = 'l';
    }
    A_node -> parent = C_node;
    A_node -> branch = 'r';
    C_node -> left = B_node;
    C_node -> right = A_node;
    C_node -> parent = parent;
    C_node -> branch = branch;
    if (branch == 'l'){
        parent -> left = C_node;
    } else if (branch == 'r'){
        parent -> right = C_node;
    }
    if (C_node -> bf == 0){
        A_node -> bf = 0;
        B_node -> bf = 0;
    } else if (C_node -> bf == -1){
        C_node -> bf = 0;
        A_node -> bf = 0;
        B_node -> bf = -1;
    } else {
        C_node -> bf = 0;
        A_node -> bf = 0;
        B_node -> bf = 1;
    }
    return C_node;
}


static struct tree_node *
balance_RL(struct tree_node * A_node)
{
    struct tree_node * B_node, * C_node, * parent;
    char branch = 0;
    B_node = A_node -> right;
    C_node = B_node -> left;
    parent = A_node -> parent;
    branch = A_node -> branch;

    B_node -> left = C_node -> right;
    if (C_node -> right){
        C_node -> right -> parent = B_node;
        C_node -> right -> branch = 'l';
    }
    B_node -> parent = C_node;
    B_node -> branch = 'r';
    A_node -> right = C_node -> left;
    if (C_node -> left){
        C_node -> left -> parent = A_node;
        C_node -> left -> branch = 'r';
    }
    A_node -> parent = C_node;
    A_node -> branch = 'l';
    C_node -> left = A_node;
    C_node -> right = B_node;
    C_node -> parent = parent;
    C_node -> branch = branch;
    if (branch == 'l'){
        parent -> left = C_node;
    } else if (branch == 'r'){
        parent -> right = C_node;
    }
    if (C_node -> bf == 0){
        A_node -> bf = 0;
        B_node -> bf = 0;
    } else if (C_node -> bf == 1){
        C_node -> bf = 0;
        A_node -> bf = 0;
        B_node -> bf = -1;
    } else if (C_node -> bf == -1){
        C_node -> bf = 0;
        A_node -> bf = 1;
        B_node -> bf = 0;
    }

    return C_node;
}


static void
traverse_tree(struct tree_node * tree)
{
    if (tree){
        printf("node:%lu\n", tree -> value);
        traverse_tree(tree -> left);
        traverse_tree(tree -> right);
    }
    return;
}


void
traverse_tree_h(struct tree_node * tree)
{

    unsigned long head = 0;
    unsigned long tail = 0;
    unsigned int i = 0;
    struct tree_node * out_node = NULL;
    struct tree_node ** queue = (struct tree_node **)malloc(sizeof(struct tree_node *) * QUEUE_LEN);
    for (i = 0; i < QUEUE_LEN; i++){
        queue[i] = NULL;
    }

    queue[tail] = tree;
    tail++;
    i = 0;
    while((out_node = queue[head])){
        queue[head] = NULL;
        i += 1;
        if (out_node -> parent){
            printf("out_node:%u %lu %p |%lu %p\n", i, out_node -> value, out_node, out_node -> parent -> value, out_node -> parent);
        } else{
            printf("out_node:%u %lu %p|N N\n", i, out_node -> value, out_node);
        }
        if(out_node -> left){
            queue[tail] = out_node -> left;
            tail++;
            tail %= QUEUE_LEN;
        }
        if (out_node -> right){
            queue[tail] = out_node -> right;
            tail++;
            tail %= QUEUE_LEN;
        }
        head++;
        head %= QUEUE_LEN;
    }
    return;
}


#ifdef AVL_TEST
int
main(int argc, char * argv[])
{
    unsigned long i = 0;
    unsigned long index = 0;
    struct tree_node ** tree_pt_pt;
    struct tree_node * tree_pt;
    struct tree_node * res_node;
    tree_pt_pt = &tree_pt;
    tree_pt = NULL;

    for (i = 0; i < 10; i++){
        res_node = make_avl_tree(tree_pt_pt, i, &index);
        traverse_tree_h(tree_pt);
        printf("res: %lu %lu\n", res_node -> value, res_node -> index);
    }


    for (i = 19; i >= 6; i--){
        res_node = make_avl_tree(tree_pt_pt, i, &index);
        traverse_tree_h(tree_pt);
        printf("res: %lu %lu\n", res_node -> value, res_node -> index);
    }

    int vv[] = {41, 42, 43, 44, 45, 46, 47, 48, 29, 28, 27, 26, 25, 24, 21};
    for (i = 0; i < sizeof(vv) / 4; i++){
        res_node = make_avl_tree(tree_pt_pt, vv[i], &index);
        traverse_tree_h(tree_pt);
        printf("res: %lu %lu\n", res_node -> value, res_node -> index);
    }


    return 0;
}
#endif
