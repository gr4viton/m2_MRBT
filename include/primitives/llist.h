#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

struct llist {
    struct llist *next;
	uint8_t *buf;
	size_t bufsiz;
	size_t fillsiz;
    uint32_t flags;
};

struct llist *llist_insert_new(struct llist *parent, uint8_t *buffer, size_t bufsize);
struct llist *llist_find_empty(struct llist *parent, size_t reqsiz);
struct llist *llist_find_filled(struct llist *parent);

#endif // LINKEDLIST_H_INCLUDED
