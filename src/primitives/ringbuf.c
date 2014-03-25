/** @defgroup ringbuf_file RING BUFFER implementation
 *
 * @brief Basic implementation of the Ring buffer for caching of serial data.
 */

#include <stdint.h>
#include <string.h>
#include "primitives/ringbuf.h"

/*---------------------------------------------------------------------------*/
/** @brief Initialize the ring buffer structure.
 *
 * @param[in] rb      Ringbuf structure to init
 * @param[in] buffer  The buffer to store the data
 * @param[in] bufsize The size of the buffer to store the data
 *
 */

void ringbuf_init(struct ringbuf *rb, uint8_t *buffer, size_t bufsize)
{
    // TODO check if bufsize is 2^N, if not, select nearest less 2^N buffer
    rb->buffer = buffer;
    rb->mask = bufsize - 1;     // MASK!

    rb->rptr = rb->wptr = 0;

    memset(buffer, 0, bufsize);   // clear the buffer
}

/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/** @brief Get available bytes for writing.
 *
 * @param[in] rb     Ringbuf structure
 * @returns number of bytes that is available for writing.
 *
 */
size_t ringbuf_write_available(struct ringbuf *rb)
{
    return (rb->rptr + rb->mask - rb->wptr) & rb->mask;
}

/*---------------------------------------------------------------------------*/
/** @brief Write single byte to the ring buffer.
 *
 * @param[in] rb     Ringbuf structure
 * @param[in] data   Byte to write
 * @returns true if success
 *
 */

bool ringbuf_write_byte(struct ringbuf *rb, uint8_t data)
{
    if (((rb->wptr + 1 - rb->rptr) & rb->mask) == 0)
        return false;

    rb->buffer[rb->wptr] = data;
    rb->wptr = (rb->wptr + 1) & rb->mask;

    return true;
}

/*---------------------------------------------------------------------------*/
/** @brief Write buffer to the ring buffer.
 *
 * @param[in] rb     Ringbuf structure
 * @param[in] bufsiz Buffer size
 * @param[in] data   Data buffer
 * @returns true if success
 *
 */

bool ringbuf_write_buffer(struct ringbuf *rb, size_t bufsiz, const uint8_t *data)
{
    if (bufsiz == 0)
        return true;

    if (ringbuf_write_available(rb) < bufsiz)
        return false;

    while (bufsiz-- > 0)
    {
        rb->buffer[rb->wptr] = *data++;
        rb->wptr = (rb->wptr + 1) & rb->mask;
    }

    return true;
}

/*---------------------------------------------------------------------------*/
/** @brief Try to write the buffer to the ring, and returns count of bytes
 * successfully written.
 *
 * @param[in] rb     Ringbuf structure
 * @param[in] bufsiz Buffer size
 * @param[in] data   Data buffer
 * @returns count of bytes successfully written to the ring
 *
 */

size_t ringbuf_write_buffer_partial(struct ringbuf *rb, size_t bufsiz, const uint8_t *data)
{
    if (bufsiz == 0)
        return 0;

    int cnt = 0;

    while (((rb->wptr + 1 - rb->rptr) & rb->mask) > 0)
    {
        rb->buffer[rb->wptr] = *data++;
        rb->wptr = (rb->wptr + 1) & rb->mask;
        cnt++;

        if (--bufsiz == 0)
            return cnt;
    }

    return cnt;
}

/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/** @brief Get available bytes for reading
 *
 * @param[in] rb     Ringbuf structure
 * @returns number of bytes that is available for read.
 */
size_t ringbuf_read_available(struct ringbuf *rb)
{
    return (rb->wptr + rb->mask + 1 - rb->rptr) & rb->mask;
}

/*---------------------------------------------------------------------------*/
/** @brief Read single byte from the ring
 *
 * @param[in] rb     Ringbuf structure
 * \param[out] data Buffer to hold the result
 * \returns true, if success
 *
 */

bool ringbuf_read_byte(struct ringbuf *rb, uint8_t *data)
{
    if (rb->wptr == rb->rptr)
        return false;

    *data = rb->buffer[rb->rptr];
    rb->rptr = (rb->rptr + 1) & rb->mask;

    return true;
}

/*---------------------------------------------------------------------------*/
/** @brief Read buffer from the ring
 *
 * @param[in] rb     Ringbuf structure
 * @param[in] bufsiz Buffer size
 * @param[out] data  Data buffer to hold the result
 * @returns true, if success
 *
 */

bool ringbuf_read_buffer(struct ringbuf *rb, size_t bufsiz, uint8_t *data)
{
    if (rb->wptr == rb->rptr)
        return false;

    if (ringbuf_read_available(rb) < bufsiz)
        return false;

    while (bufsiz-- > 0)
    {
        *data++ = rb->buffer[rb->rptr];
        rb->rptr = (rb->rptr + 1) & rb->mask;
    }

    return true;
}

/*---------------------------------------------------------------------------*/
/** @brief Skip data inside the ring
 *
 * @param[in] rb     Ringbuf structure
 * @param[in] len    Skip length
 * @returns true, if success
 *
 */

bool ringbuf_skip(struct ringbuf *rb, size_t len)
{
    if (rb->wptr == rb->rptr)
        return false;

    if (ringbuf_read_available(rb) < len)
        return false;

    rb->rptr = (rb->rptr + len) & rb->mask;
    return true;
}

/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/** @brief Peek the data byte from ring buffer at offset
 *
 * @param[in] rb     Ringbuf structure
 * @param[in]  offset Offset from current reading position
 * @param[out] data   Pointer to data byte
 * @returns true if success
 */
bool ringbuf_peek_byte(struct ringbuf *rb, uint32_t offset, uint8_t *data)
{
    if (rb->wptr == rb->rptr)
        return false;

    if (ringbuf_read_available(rb) < offset)
        return false;

    *data = rb->buffer[(rb->rptr + offset) & rb->mask];
    return true;
}

/**
 * @brief Peek the data buffer from ring buffer at offset
 *
 * @param[in] rb     Ringbuf structure
 * @param[in]  offset Offset from current reading position to start of buffer
 * @param[in]  bufsiz The readed buffer size
 * @param[out] data   Pointer to data buffer
 * @returns true if success
 */
bool ringbuf_peek_buffer(struct ringbuf *rb, uint32_t offset, size_t bufsiz, uint8_t *data)
{
    if (rb->wptr == rb->rptr)
        return false;

    if (ringbuf_read_available(rb) < (offset + bufsiz))
        return false;

    while (bufsiz-- > 0)
        *data++ = rb->buffer[(rb->rptr + offset++) & rb->mask];

    return true;
}


