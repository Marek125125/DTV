
#ifndef SRC_LISTBUFF_H_
#define SRC_LISTBUFF_H_


#if defined(__cplusplus)
extern "C" {
#endif

#define MAX_SPAN_VAL(x)     _Generic((x),   uint8_t : 0xFFu, \
                                            uint16_t: 0xFFFFu, \
                                            uint32_t: 0xFFFFFFFFu, \
                                            default:  0u)

#define INVALID_LISTBUFF_INDEX  (  MAX_SPAN_VAL((seq_span_t)0) )

typedef uint8_t seq_span_t;

typedef enum
{
    LISTBUFF_NODE_FREE = 0,
    LISTBUFF_NODE_RESERVED,
    LISTBUFF_NODE_HAVE_DATA

} enum_LISTBUFF_NODE_STATUS;

typedef struct
{
    bool                        b_visited;
    bool                        b_hidden;
    enum_LISTBUFF_NODE_STATUS   status;
    seq_span_t                  next_unvisited;
    seq_span_t                  marker;
} __attribute__((packed)) struct_LISTBUFF_FEATURE;

typedef struct
{
    void * const ptr_buff;
    seq_span_t n;
    seq_span_t first_avail;
    seq_span_t last_avail;
    seq_span_t const size;
    seq_span_t const elem_size;
} struct_LISTBUFF;

#define BASE_256_POWER_1    (256u)
#define BASE_256_POWER_2    (BASE_256_POWER_1 * BASE_256_POWER_1)
#define BASE_256_POWER_4    (BASE_256_POWER_2 * BASE_256_POWER_2)
#define COUNT_TYPE_SIZE     sizeof( seq_span_t )

#define DEFINE_STATIC_LIST_BUFFER(type, name, sz)    \
                            _Static_assert(1u == COUNT_TYPE_SIZE && sz < BASE_256_POWER_1 ? true : \
                                           2u == COUNT_TYPE_SIZE && sz < BASE_256_POWER_2 ? true : \
                                           4u == COUNT_TYPE_SIZE && sz < BASE_256_POWER_4 ? true : \
                                           false,   \
                                           "The specified list buffer size cannot be held by type 'seq_span_t'. Decrease the size of the list buffer or change the type of 'seq_span_t'"); \
                                                    \
                            _Static_assert(sz <= INVALID_LISTBUFF_INDEX ? true : false, \
                                           "The 'sz' argument exceeds the allowed range. Please decrease it or change the type 'seq_span_t' to hold larger values"); \
    typedef struct                                  \
    {                                               \
        type obj;                                   \
        struct_LISTBUFF_FEATURE prop;               \
    } __attribute__((packed)) type ## _ ## name ## _visitable;              \
                                                    \
    static type ## _ ## name ## _visitable mgl_listbuff_ ## type ## _ ## name [sz]; \
    static struct_LISTBUFF name = {                     \
        .ptr_buff = (void*)(& mgl_listbuff_ ## type ## _ ## name [0]), \
        .n               = 0u,                                    \
        .first_avail     = INVALID_LISTBUFF_INDEX,  \
        .last_avail      = INVALID_LISTBUFF_INDEX,  \
        .size            = sz,                      \
        .elem_size       = sizeof(type)             \
    }


seq_span_t  listbuff_get_count      (const struct_LISTBUFF *ptr_buff) __attribute__((unused));

seq_span_t  listbuff_get_free_space (const struct_LISTBUFF *ptr_buff) __attribute__((unused));

void        listbuff_reset          (struct_LISTBUFF *ptr_buff) __attribute__((unused));

enum_CODE   listbuff_get            (struct_LISTBUFF *ptr_buff, void **ptr_ptr_obj) __attribute__((unused));

enum_CODE   listbuff_pop            (struct_LISTBUFF *ptr_buff, void *ptr_obj) __attribute__((unused));

enum_CODE   listbuff_reserve        (struct_LISTBUFF *ptr_buff, seq_span_t marker, void **ptr_ptr_obj) __attribute__((unused));

enum_CODE   listbuff_signal_new     (struct_LISTBUFF *ptr_buff, seq_span_t marker, uint8_t b_hidden) __attribute__((unused));

enum_CODE   listbuff_get_reserved   (struct_LISTBUFF *ptr_buff, seq_span_t marker, void **ptr_ptr_obj) __attribute__((unused));

enum_CODE   listbuff_push           (struct_LISTBUFF *ptr_buff, void *ptr_obj, uint8_t b_hidden) __attribute__((unused));

enum_CODE      listbuff_free_if_visited       (struct_LISTBUFF *ptr_buff) __attribute__((unused));

enum_CODE      listbuff_unhide_if_hidden       (struct_LISTBUFF *ptr_buff) __attribute__((unused));

#ifdef __cplusplus
}
#endif

#endif 

