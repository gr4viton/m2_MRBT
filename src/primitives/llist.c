/** @defgroup linkedlist_file Linked list implementation
 *
 * @brief Basic implementation of the Linked List
 */

#include <stdint.h>
#include <string.h>
#include "primitives/llist.h"

/*---------------------------------------------------------------------------*/
/** @brief Insert new linked list structure to chain
 *
 * @param[in] rb      Linked list structure to init
 * @param[in] buffer  The buffer to store the data
 * @param[in] bufsize The size of the buffer to store the data
 *
 */

struct llist *llist_insert_new(struct llist *parent, uint8_t *buffer, size_t bufsize)
{
    struct llist *ll = (struct llist *)(void*)buffer;
    ll->bufsiz = bufsize - sizeof(struct llist);
    ll->buf = buffer + sizeof(struct llist);
    ll->flags = 0;
    if (parent != NULL)
    {
        ll->next = parent->next;
        parent->next = ll;
    }
    else
        ll->next = NULL;

    // clear the buffer
    memset(ll->buf, 0, ll->bufsiz);

    return ll;
}

struct llist *llist_find_empty(struct llist *parent, size_t reqsiz)
{
    while (parent != NULL)
    {
        if ((parent->fillsiz == 0) && (parent->bufsiz >= reqsiz))
            return parent;

        parent = parent->next;
    }

    return NULL;
}

struct llist *llist_find_filled(struct llist *parent)
{
    while (parent != NULL)
    {
        if (parent->fillsiz != 0)
            return parent;

        parent = parent->next;
    }

    return NULL;
}


