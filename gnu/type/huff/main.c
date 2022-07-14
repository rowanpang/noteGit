#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "list.h"


#define FAKE_NODE       0xFF000000

#define NODE_FAKENODE   0x80000000
#define NODE_INTREE     0x00000001

#define MAX_WEIGHT MAX_INT

struct btnode{
    unsigned int weight;
    struct btnode* parent;
    struct btnode* lchild;
    struct btnode* rchild;
    int value;
    int flags;
    int codec;
    int high;
    struct list_head entry;
};

LIST_HEAD(inputs);

struct btnode* bt_allocnode(int val)
{
    struct btnode *node;
    node = malloc(sizeof(struct btnode));

    node->weight = 1;
    node->parent = NULL;
    node->lchild = NULL;
    node->rchild = NULL;
    node->value = val;
    node->flags = 0;
    node->codec = 0;
    node->high = 0;

    if(val & FAKE_NODE){
        /*printf("alloc fake node\n");*/
        node->flags |= NODE_FAKENODE;
    }

    INIT_LIST_HEAD(&node->entry);
    return node;
}

int bt_insert(struct btnode *node,struct list_head* list)
{
    list_add_tail(&node->entry,list);
}

int input_getnode(char val)
{
    int ret = 0;
    struct btnode *node = NULL;

    list_for_each_entry(node,&inputs,entry){
        if (node->value == val){
            node->weight++;
            return ret;
        }
    }

    node = bt_allocnode(val);
    ret = bt_insert(node,&inputs);
}


int input_iter(struct list_head *list)
{
    struct btnode *node;

    list_for_each_entry(node,&inputs,entry){
        printf("node val:%c,weight:%d\n",node->value,node->weight);
    }
}

int input_sort(struct list_head *list){
    /*
     * 1,需要按照输入顺序mktree
     * 2,mktree过程中会new node
     * 3,所以不再按照权重进行排序,无意义且增加工作量
     */
    ;
}

struct btnode* input_mktree(struct list_head *list)
{
    int ret;
    struct btnode *tmp,*node,*new;
    struct btnode *ndl,*ndr;
    int wl,wr;

    while (1) {
        ndl = NULL;
        wl = MAX_WEIGHT;

        ndr = NULL;
        wr = MAX_WEIGHT;

        list_for_each_entry(node,list,entry) {
            if (node->flags & NODE_INTREE){
                //printf("choosing node intree continue\n");
                continue;
            }

            //printf("choosing node:%c,%d,%s\n",node->value,node->weight,(node->flags & NODE_FAKENODE)?"FAKE":"INPUT");

            if (node->weight < wl) {
                /*printf("update ndl\n");*/
                wr = wl;
                ndr = ndl;
                ndl = node;
                wl = node->weight;
            } else if (node->weight < wr) {
                /*printf("update ndr\n");*/
                wr = node->weight;
                ndr = node;
            }
        }

        if (!ndr) {
            printf("got root node,break\n");
            break;
        }

        printf("choosen ");
        if (ndl->flags & NODE_FAKENODE) {
            printf("ndl:%c-%c,%d FAKE; ",(ndl->value >> 8) & 0xff,ndl->value & 0xff,ndl->weight);
        } else {
            printf("ndl:%c,%d INPUT; ",ndl->value,ndl->weight);
        }

        if (ndr->flags & NODE_FAKENODE) {
            printf("ndr:%c-%c,%d FAKE",(ndr->value >> 8) & 0xff,ndr->value & 0xff,ndr->weight);
        } else {
            printf("ndr:%c,%d INPUT",ndr->value,ndr->weight);
        }
        printf("\n");

        new = bt_allocnode(ndl->value << 8 | ndr->value | FAKE_NODE);       //不精确,如果需要精确显示需要判断ndl/ndr 是否为fake node, 如果是fake node时则无法反映child.

        ndr->parent = new;
        ndr->flags |= NODE_INTREE;
        ndl->parent = new;
        ndl->flags |= NODE_INTREE;

        new->lchild = ndl;
        new->rchild = ndr;
        new->weight = wl+wr;

        list_add(&new->entry,&ndl->entry);
        //bt_insert(new,list);

        printf("\n");
    }

    return ndr;
}

int input_codec(struct list_head *list)
{
    int ret = 0;
    int i;
    struct btnode *tmp;
    struct btnode *child,*parent;
    list_for_each_entry(tmp,list,entry){
        if (tmp->flags & NODE_FAKENODE){
            /*printf("input codec fake node continue\n");*/
            continue;
        }

        child = tmp;
        parent = tmp->parent;
        i = 0;
        while (parent) {
            if (parent->lchild == child){
                //printf("on left\n");
            } else {
                //printf("on right\n");
                tmp->codec |= 0x01 << i;
            }

            i++;
            child = parent;
            parent = parent->parent;
        }
        tmp->high = i;

        i = tmp->high;
        printf("%c's high:%d, codec:%#x, binary: ",tmp->value,tmp->high,tmp->codec);
        while (i--) {                               //第i级由第(i-1)位表示左右.位数从0开始计数.
            if (tmp->codec >> i & 0x01){
                printf("1");
            } else {
                printf("0");
            }
        }
        printf("\n");


    }

    return ret;
}

char *btinput = "myinputkkk";
int main(int argc,char** argv)
{
    int ret = 0;
    char c;
    char* intmp;
    struct btnode *root;
    intmp = btinput;

    if(argc >1 ) {
        intmp = argv[1];
    }

    printf("get inputs: ");
    while (c=*intmp++) {
        printf("%c ",c);
        ret = input_getnode(c);
    }
    printf("\n");

    input_iter(&inputs);
    root = input_mktree(&inputs);

    printf("\n");
    ret = input_codec(&inputs);

    return 0;
}
