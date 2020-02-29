#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    /* TODO: What if malloc returned NULL? */
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    /*Free the list elements and the strings? */
    list_ele_t *cur = q->head;
    list_ele_t *nxt = NULL;
    while (cur) {
        nxt = cur->next;
        free(cur->value);
        free(cur);
        cur = nxt;
    }
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* TODO: What should you do if the q is NULL? */
    if (!q)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    /*Allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    int str_len = strlen(s);
    char *str;
    str = malloc(sizeof(char) * (str_len + 1));
    if (!str) {
        free(newh);
        return false;
    }

    str[str_len] = '\0';
    strncpy(str, s, str_len);

    if (!q->head)
        q->tail = newh;

    newh->next = q->head;
    q->head = newh;
    newh->value = str;

    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* Remember: It should operate in O(1) time */
    if (!q)
        return false;

    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;

    int str_len = strlen(s);
    char *str;

    str = malloc(sizeof(char) * (str_len + 1));
    if (!str) {
        free(newt);
        return false;
    }

    str[str_len] = '\0';
    strncpy(str, s, str_len);

    if (!q->head)
        q->head = newt;
    else
        q->tail->next = newt;

    newt->next = NULL;
    newt->value = str;
    q->tail = newt;
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0)
        return false;

    if (sp) {
        memset(sp, '\0', bufsize);
        strncpy(sp, q->head->value, bufsize - 1);
    }

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);

    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* Remember: It should operate in O(1) time */
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->head == 0)
        return;

    list_ele_t *pre = NULL;
    list_ele_t *cur = q->head;
    list_ele_t *nxt = NULL;


    while (cur) {
        nxt = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nxt;
    }
    q->tail = q->head;
    q->head = pre;
}


void split(list_ele_t *start, list_ele_t **left, list_ele_t **right)
{
    list_ele_t *slow = start;
    list_ele_t *fast = start;

    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    *left = start;
    *right = slow->next;
    slow->next = NULL;

    return;
}

list_ele_t *sort(list_ele_t *start)
{
    if (!start || !start->next)
        return start;

    list_ele_t *left;
    list_ele_t *right;

    split(start, &left, &right);

    left = sort(left);
    right = sort(right);

    for (list_ele_t *merge = NULL; left || right;) {
        if (!right || (left && strcmp((left->value), (right->value)) < 0)) {
            if (!merge) {
                start = merge = left;
            } else {
                merge->next = left;
                merge = merge->next;
            }
            left = left->next;
        } else {
            if (!merge) {
                start = merge = right;
            } else {
                merge->next = right;
                merge = merge->next;
            }
            right = right->next;
        }
    }
    return start;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;
    q->head = sort(q->head);

    while (q->tail->next)
        q->tail = q->tail->next;
}
