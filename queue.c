#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>

// Queue Structure
struct queue {
    void **queue; // Array to hold elements
    int back; // Index of the back of the queue
    int front; // Index of the front of the queue
    int max; // Size of the queue
    int in_q; // Number of elements in the queue
};
typedef struct queue queue_t;

pthread_mutex_t m; // Used to lock the critical region.
pthread_cond_t c_pop; // Used to signal when something was poped
pthread_cond_t c_push; // Used to signal when something was pushed

// Used to create a new queue
queue_t *queue_new(int size) {
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    q->queue = (void **) malloc(size * sizeof(void *));
    q->max = size;
    q->front = 0;
    q->back = 0;
    q->in_q = 0;
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c_pop, NULL);
    pthread_cond_init(&c_push, NULL);
    return q;
}

// Frees all allocated memory that was allocated be queue_new();
void queue_delete(queue_t **q) {
    free((*(*q)).queue);
    free(*q);
    *q = NULL;
    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c_pop);
    pthread_cond_destroy(&c_push);
    return;
}

// Push elem into q
bool queue_push(queue_t *q, void *elem) {
    if (elem == NULL) {
        return false;
    }

    pthread_mutex_lock(&m); // Beginning of critical region
    while (q->in_q == q->max) { // Check if the q is full
        pthread_cond_wait(&c_pop, &m);
    }

    q->queue[q->back] = elem; // Push element
    q->in_q += 1;
    if (q->back == (q->max - 1)) {
        q->back = 0;
    } else {
        q->back += 1;
    }

    pthread_mutex_unlock(&m); // End of critical region
    pthread_cond_broadcast(&c_push); // Signal that element was pushed
    return true;
}

bool queue_pop(queue_t *q, void **elem) {
    if (elem == NULL) {
        return false;
    }
    pthread_mutex_lock(&m); // Beginning of critical region
    while (q->in_q == 0) { // Check if the q is empty
        pthread_cond_wait(&c_push, &m);
    }

    *elem = q->queue[q->front]; // Pop and store element into elem
    q->in_q -= 1;
    if (q->front == (q->max - 1)) {
        q->front = 0;
    } else {
        q->front += 1;
    }
    pthread_mutex_unlock(&m); // End of critical region
    pthread_cond_broadcast(&c_pop); // Signal that an element was popped
    return true;
}
