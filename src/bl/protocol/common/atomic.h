
#ifndef SRC_COMMON_ATOMIC_H_
#define SRC_COMMON_ATOMIC_H_


#if defined (__GNUC__)
#define ENABLE_INTERRUPTS() __asm volatile ("cpsie i" : : : "memory");
#else
#define ENABLE_INTERRUPTS() __asm("cpsie i")
#endif


#if defined (__GNUC__)
#define DISABLE_INTERRUPTS() __asm volatile ("cpsid i" : : : "memory");
#else
#define DISABLE_INTERRUPTS() __asm("cpsid i")
#endif

typedef enum
{
    ATOMIC_RELEASED             = 0x00,
    ATOMIC_ATTEMPT_TO_ACQUIRE   = 0x01,
    ATOMIC_ACQUIRED             = 0x02,

} enum_ATOMIC_STATE;

#define DECLARE_ATOMIC_VAR(var) static enum_ATOMIC_STATE var = ATOMIC_RELEASED

#ifdef __GNUC__


    #define ATOMIC_ACQUIRE( var ) __extension__ ({ \
                                                    DISABLE_INTERRUPTS(); \
                                                    var |= ( ATOMIC_RELEASED == var ? ATOMIC_ACQUIRED : ATOMIC_ATTEMPT_TO_ACQUIRE); \
                                                    ENABLE_INTERRUPTS(); \
                                                    (var); \
                                                })

#else
    #warning "WARNING! Atomic access requires GCC compiler. Access will not be exclusive!"

    #define ATOMIC_ACQUIRE( var )   ATOMIC_ACQUIRED

#endif 


#define ATOMIC_RELEASE( var ) \
                                            DISABLE_INTERRUPTS(); \
                                            var = ATOMIC_RELEASED; \
                                            ENABLE_INTERRUPTS()


#endif 

