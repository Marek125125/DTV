
#ifndef MBEDTLS_BIGNUM_CORE_H
#define MBEDTLS_BIGNUM_CORE_H

#include "common.h"

#if defined(MBEDTLS_BIGNUM_C)
#include "mbedtls/bignum.h"
#endif

size_t mbedtls_mpi_core_clz( mbedtls_mpi_uint a );

size_t mbedtls_mpi_core_bitlen( const mbedtls_mpi_uint *A, size_t A_limbs );

void mbedtls_mpi_core_bigendian_to_host( mbedtls_mpi_uint *A,
                                         size_t A_limbs );

int mbedtls_mpi_core_read_le( mbedtls_mpi_uint *X,
                              size_t X_limbs,
                              const unsigned char *input,
                              size_t input_length );

int mbedtls_mpi_core_read_be( mbedtls_mpi_uint *X,
                              size_t X_limbs,
                              const unsigned char *input,
                              size_t input_length );

int mbedtls_mpi_core_write_le( const mbedtls_mpi_uint *A,
                               size_t A_limbs,
                               unsigned char *output,
                               size_t output_length );

int mbedtls_mpi_core_write_be( const mbedtls_mpi_uint *A,
                               size_t A_limbs,
                               unsigned char *output,
                               size_t output_length );

#define ciL    ( sizeof(mbedtls_mpi_uint) )   
#define biL    ( ciL << 3 )                   
#define biH    ( ciL << 2 )                   

#define BITS_TO_LIMBS(i)  ( (i) / biL + ( (i) % biL != 0 ) )
#define CHARS_TO_LIMBS(i) ( (i) / ciL + ( (i) % ciL != 0 ) )
#define GET_BYTE( X, i )                                \
    ( ( (X)[(i) / ciL] >> ( ( (i) % ciL ) * 8 ) ) & 0xff )

#endif 

